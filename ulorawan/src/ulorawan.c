/**
 * \file
 *
 * \brief The ulorawan function implementations
 *
 * Copyright (c) 2023 Derek Goslin
 *
 * @author Derek Goslin
 *
 * \page License
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include <stddef.h>

#include <string.h>

#include "crypto_hal.h"
#include "nvm_hal.h"
#include "ulorawan.h"
#include "ulorawan_events.h"
#include "ulorawan_mac.h"
#include "ulorawan_mac_cmds.h"
#include "ulorawan_region.h"

static struct ulorawan_session session = {ULORAWAN_STATE_INIT};
static struct osal_queue event_queue;

static int32_t ulorawan_send_event(const struct ulorawan_event *const event);

static int32_t ulorawan_timer_expire_handler(enum timer_hal_timer timer);

static int32_t ulorawan_radio_irq_handle(enum radio_hal_irq_flags flags);

SESSION_ACCESS ulorawan_get_session() { return &session; }

int32_t ulorawan_init(enum ulorawan_device_class class,
                      struct ulorawan_device_security security) {

  if (osal_queue_create(&event_queue) != OSAL_ERR_NONE) {
    return ULORAWAN_ERR_QUEUE;
  }

  if (radio_hal_set_mode(MODE_SLEEP) != RADIO_HAL_ERR_NONE) {
    return ULORAWAN_ERR_RADIO;
  }

  session.state = ULORAWAN_STATE_IDLE;
  session.security = security;
  session.class = class;

  return ULORAWAN_ERR_NONE;
}

int32_t ulorawan_join() {
  if (session.state == ULORAWAN_STATE_INIT) {
    return ULORAWAN_ERR_INIT;
  }

  if (session.security.type != ACTIVATION_OTAA) {
    return ULORAWAN_ERR_ACTIVATION;
  }

  struct ulorawan_channel channel;

  if (ulorawan_region_get_channel(&channel) != ULORAWAN_REGION_ERR_NONE) {
    return ULORAWAN_ERR_NO_CHANNEL;
  }

  uint16_t nonce;

  if (nvm_hal_read_join_nonce(&nonce) != NVM_HAL_ERR_NONE) {
    return ULORAWAN_ERR_NVM;
  }

  // create join request
  struct ulorawan_mac_join_req join_req;

  memcpy(join_req.join_eui, session.security.context.otaa.join_eui,
         ULORAWAN_JOIN_EUI_SIZE);
  memcpy(join_req.device_eui, session.security.context.otaa.dev_eui,
         ULORAWAN_DEV_EUI_SIZE);
  join_req.device_nonce = ++nonce;

  struct ulorawan_mac_frame_context ctx;

  union ulorawan_mac_mhdr mhdr =
      ULORAWAN_MHDR_INIT(FRAME_TYPE_JOIN_REQ, LORAWAN_MAJOR_R1);

  if (ulorawan_mac_write_mhdr(&ctx, &mhdr) != ULORAWAN_MAC_ERR_NONE) {
    return ULORAWAN_ERR_CTX;
  }

  if (ulorawan_mac_write_join_req(&ctx, &join_req) != ULORAWAN_MAC_ERR_NONE) {
    return ULORAWAN_ERR_CTX;
  }

  uint32_t cmac;

  if (crypto_hal_aes_cmac(session.security.context.otaa.app_key,
                          (uint8_t *const) & ctx.buf, ctx.eof,
                          &cmac) != CRYPTO_HAL_ERR_NONE) {
    return ULORAWAN_ERR_CMAC;
  }

  if (ulorawan_mac_write_mic(&ctx, cmac) != ULORAWAN_MAC_ERR_NONE) {
    return ULORAWAN_ERR_CTX;
  }

  radio_hal_configure();

  if (radio_hal_set_mode(MODE_TX) != RADIO_HAL_ERR_NONE) {
    return ULORAWAN_ERR_RADIO;
  }

  if (radio_hal_fifo_write(ctx.buf, ctx.eof) != RADIO_HAL_ERR_NONE) {
    return ULORAWAN_ERR_RADIO;
  }

  session.state = ULORAWAN_STATE_TX;

  if (nvm_hal_write_join_nonce(join_req.device_nonce) != CRYPTO_HAL_ERR_NONE) {
    return ULORAWAN_ERR_NVM;
  }

  return ULORAWAN_ERR_NONE;
}

int32_t ulorawan_radio_irq(const enum radio_hal_irq_flags flags) {
  if (session.state == ULORAWAN_STATE_INIT) {
    return ULORAWAN_ERR_INIT;
  }

  struct ulorawan_event event;
  event.type = EVENT_TYPE_RADIO_IRQ;
  event.data.flags = flags;

  return ulorawan_send_event(&event);
}

int32_t ulorawan_task() {
  if (session.state == ULORAWAN_STATE_INIT) {
    return ULORAWAN_ERR_INIT;
  }

  int32_t result = ULORAWAN_ERR_NONE;

  while (!osal_queue_empty(&event_queue)) {
    struct ulorawan_event event;

    if (osal_queue_receive(&event_queue, &event) != OSAL_ERR_NONE) {
      result = ULORAWAN_ERR_QUEUE;
    } else {
      if (event.type == EVENT_TYPE_RADIO_IRQ) {
        result = ulorawan_radio_irq_handle(event.data.flags);
      } else {
        result = ulorawan_timer_expire_handler(event.data.timer);
      }
    }
  };

  return result;
}

int32_t ulorawan_timer_expired(enum timer_hal_timer timer) {
  if (session.state == ULORAWAN_STATE_INIT) {
    return ULORAWAN_ERR_INIT;
  }

  struct ulorawan_event event;
  event.type = EVENT_TYPE_TIMER_EXPIRE;
  event.data.timer = timer;

  return ulorawan_send_event(&event);
}

union version ulorawan_version() {
  union version v;

  v.value = ULORAWAN_VERSION;

  return v;
}

int32_t ulorawan_send_event(const struct ulorawan_event *const event) {

  if (osal_queue_send(&event_queue, event) != OSAL_ERR_NONE) {
    return ULORAWAN_ERR_QUEUE;
  }

  return ULORAWAN_ERR_NONE;
}

int32_t ulorawan_timer_expire_handler(enum timer_hal_timer timer) {

  if ((session.state == ULORAWAN_STATE_RX1 && timer == TIMER0) ||
      (session.state == ULORAWAN_STATE_RX2 && timer == TIMER1)) {
    return radio_hal_set_mode(MODE_RX_SINGLE);
  }

  return ULORAWAN_ERR_NONE;
}

int32_t ulorawan_radio_irq_handle(enum radio_hal_irq_flags flags) {
  switch (session.state) {
  case ULORAWAN_STATE_INIT:
    break;
  case ULORAWAN_STATE_IDLE:
    break;
  case ULORAWAN_STATE_TX:
    if (flags & RADIO_HAL_IRQ_TX_DONE) {
        session.state = ULORAWAN_STATE_RX1;
        
        //TODO get the timer default intervals..
        //timer_hal_start();
        //timer_hal_start();
    }
    break;
  case ULORAWAN_STATE_RX1:
    if (flags & RADIO_HAL_IRQ_RX_TIMEOUT) {
        session.state = ULORAWAN_STATE_RX2;
        
        // configure radio.
    }
    else if (flags & RADIO_HAL_IRQ_RX_DONE) {
        timer_hal_stop(TIMER1);
        session.state = ULORAWAN_STATE_INIT;
    }
    break;
  case ULORAWAN_STATE_RX2:
    break;
  default:
    break;
  }

  return ULORAWAN_ERR_NONE;
}
