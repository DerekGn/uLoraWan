/**
 * \file
 *
 * \brief LoraWan MAC command types
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

#ifndef ULORAWAN_MAC_CMDS_H_
#define ULORAWAN_MAC_CMDS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "crosscompile.h"

//! The end-device is connected to an external power source.
#define ULORAWAN_MAC_BATTERY_EXTERNAL_SOURCE 0

//! The end-device was not able to measure the battery level.
#define ULORAWAN_MAC_BATTERY_UNABLE_TO_MEASURE 0xFF

//! The size of the network security key
#define ULORAWAN_MAC_NWK_S_KEY_SIZE 16

//! The size of the application security key
#define ULORAWAN_MAC_APP_S_KEY_SIZE 16

//! The AppKey is an AES-128 root key size
#define ULORAWAN_MAC_APP_KEY_SIZE 16

//! The JoinNonce size in bytes
#define ULORAWAN_MAC_JOIN_NONCE_SIZE 3

//! The NetId size in bytes
#define ULORAWAN_MAC_NET_ID_SIZE 3

//! The max size of network CFList
#define ULORAWAN_MAC_CF_LIST_SIZE 16

/**
 * \brief LoraWan Mac LinkADRAns payload
 */
union ulorawan_mac_adr_ans {
  //! The value
  uint8_t value;
  struct __CROSS_ATTR_PACKED {
    //! The channel mask acknowledge
    uint8_t ch_mask_ack : 1;
    //! The data rate acknowledge
    uint8_t data_rate_ack : 1;
    //! The power level acknowledge
    uint8_t power_ack : 1;
    //! Reserved
    uint8_t rfu : 5;
  } bits;
};

/**
 * \brief LoraWan Mac PingSlotInfoReq
 */
union __CROSS_ATTR_PACKED ulorawan_mac_beacon_freq_ans {
  //! The value
  uint8_t value;
  struct __CROSS_ATTR_PACKED {
    //! reserved
    uint8_t rfu : 6;
    //!
    uint8_t beacon_freq_ok : 1;
  } bits;
};

/**
 * \brief LoraWan end device commands
 */
enum ulorawan_mac_dev_cmds {
  //! Used by an end-device to validate its connectivity to a network.
  DEV_MAC_LINK_CHECK_REQ = 0x02,
  //! Acknowledges a LinkADRReq command.
  DEV_MAC_LINK_ADR_ANS = 0x03,
  //! Acknowledges a DutyCycleReq command.
  DEV_MAC_DUTY_CYCLE_ANS = 0x04,
  //! Acknowledges a RXParamSetupReq command.
  DEV_MAC_RX_PARAM_SETUP_ANS = 0x05,
  //! Returns the status of the end-device, namely its battery level and its
  //! radio status.
  DEV_MAC_DEV_STATUS_ANS = 0x06,
  //! Acknowledges NewChannelReq command.
  DEV_MAC_NEW_CHANNEL_ANS = 0x07,
  /// < Acknowledges RXTimingSetupReq command.
  DEV_MAC_RX_TIMING_SETUP_ANS = 0x08,
  //! Acknowledges a TXParamSetupReq command.
  DEV_MAC_TX_PARAM_SETUP_ANS = 0x09,
  //! Acknowledges a DlChannelReq command.
  DEV_MAC_DL_CHANNEL_ANS = 0x0A,
  //! Used by an end-device to request the current GPS time.
  DEV_MAC_DEV_TIME_REQ = 0x0D,
  //! PingSlotInfoReq Used by the end-device to communicate the unicast
  //! ping-slot periodicity to the Network Serve
  DEV_MAC_PING_SLOT_INFO_REQ = 0x10,
  //! Used by the end-device to acknowledge a PingSlotChannelReq command
  DEV_MAC_PING_SLOT_CH_ANS = 0x11,
  //! Used by the end-device to acknowledge a BeaconFreqReq command
  SVR_MAC_BEACON_FREQ_ANS = 0x13,
};

/**
 * \brief LoraWan server commands
 */
enum ulorawan_mac_srv_cmds {
  //! Answers LinkCheckReq. Contains the received signal power
  //! estimation, which indicates the quality of reception (link
  //! margin) to the end-device.
  SRV_MAC_LINK_CHECK_ANS = 0x02,
  //! Requests the end-device to change data rate, TX power, redundancy, or
  //! channel  mask.
  SRV_MAC_LINK_ADR_REQ = 0x03,
  //! Sets the maximum aggregated transmit duty cycle of an end-device.
  SRV_MAC_DUTY_CYCLE_REQ = 0x04,
  //! Sets the reception slot parameters.
  SRV_MAC_RX_PARAM_SETUP_REQ = 0x05,
  //! Requests the status of the end-device.
  SRV_MAC_DEV_STATUS_REQ = 0x06,
  //! Creates or modifies the definition of a radio channel
  SRV_MAC_NEW_CHANNEL_REQ = 0x07,
  //! Sets the timing of the reception slots.
  SRV_MAC_RX_TIMING_SETUP_REQ = 0x08,
  //! Used by a Network Server to set the maximum allowed dwell time and
  //! MaxEIRP of end-device, based on local regulations.
  SRV_MAC_TX_PARAM_SETUP_REQ = 0x09,
  //! Modifies the definition of a downlink RX1 radio channel by shifting the
  //! downlink frequency from the uplink frequencies (i.e. creating an
  //! asymmetric channel)
  SRV_MAC_DL_CHANNEL_REQ = 0x0A,
  //! Answers DeviceTimeReq
  SRV_MAC_DEV_TIME_ANS = 0x0D,
  //! PingSlotInfoAns. Used by the Network to acknowledge a PingInfoSlotReq
  //! command
  SRV_MAC_PING_SLOT_INFO_ANS = 0x10,
  //! Used by the Network Server to set the unicast ping channel frequency and
  //! data rate of an end-device
  SRV_MAC_PING_SLOT_CH_REQ = 0x11,
  //! Command used by the Network Server to modify the frequency at which the
  //! end-device expects to receive a beacon broadcast
  SVR_MAC_BEACON_FREQ_REQ = 0x13
};

/**
 * \brief LoraWan Mac LinkCheckAns payload
 */
struct __CROSS_ATTR_PACKED ulorawan_mac_link_check_ans {
  //! The demodulation margin in the range of 0..254
  uint8_t margin;
  //! The gateway count (GwCnt) is the number of gateways that received the
  //! most recent LinkCheckReq command.
  uint8_t gw_cnt;
};

union __CROSS_ATTR_PACKED ulorawan_mac_redundancy {
  //! The value
  uint8_t value;
  struct __CROSS_ATTR_PACKED {
    //! the number of transmissions for each uplink frame
    uint8_t nbtrans : 4;
    //! The channel mask control.
    uint8_t ch_mask_ctl : 3;
    //! Reserved
    uint8_t rfu : 1;
  } bits;
};

/**
 * \brief LoraWan Mac LinkADRReq payload
 */
struct __CROSS_ATTR_PACKED ulorawan_mac_link_adr_req {
  //! The data rate and tx power
  union __CROSS_ATTR_PACKED tx_power {
    //! The value
    uint8_t value;
    struct __CROSS_ATTR_PACKED {
      //! The tx power
      uint8_t tx_power : 4;
      //! The data rate
      uint8_t data_rate : 4;
    };
    //! The channel mask
    uint8_t ch_mask;
  } bits;
};

/**
 * \brief LoraWan Mac DutyCycleReq payload
 */
union ulorawan_mac_duty_cycle_req {
  //! The value
  uint8_t value;
  struct __CROSS_ATTR_PACKED {
    //! The maximum duty cycle
    uint8_t max_duty_cycle : 4;
    //! Reserved
    uint8_t rfu : 4;
  } bits;
};

/**
 * \brief LoraWan Mac RXParamSetupReq payload
 */
union __CROSS_ATTR_PACKED ulorawan_mac_rx_param_setup_req {
  //! The download settings
  union __CROSS_ATTR_PACKED {
    //! The value
    uint8_t value;
    struct __CROSS_ATTR_PACKED {
      //! Data rate of a downlink using the second receive window
      uint8_t rx2_data_rate : 4;
      //! Offset between up and downlink datarate of first reception slot
      uint8_t rx1_dr_offset : 3;
      uint8_t rfu : 1;
    } bits;
  } dlsettings;
  //! The the frequency of the channel used for the second receive window
  uint8_t freq[ULORAWAN_FREQ_SIZE];
};

/**
 * \brief LoraWan Mac RXParamSetupAns payload
 */
union ulorawan_mac_rx_param_setup_ans {
  //! Acknowledge the channel change
  uint8_t channel_ack : 1;
  //! Acknowledge the rx2 data rate change
  uint8_t rx2_data_rate_ack : 1;
  //! Acknowledge the rx1 data offset change
  uint8_t rx1_dr_offset_ack : 1;
  //! Reserved
  uint8_t rfu : 5;
};

/**
 * \brief LoraWan Mac DevStatusAns payload
 */
struct __CROSS_ATTR_PACKED ulorawan_mac_device_status_ans {
  //! The devices battery level
  uint8_t battry_level;
  //! The radio status
  union __CROSS_ATTR_PACKED {
    uint8_t value;
    struct __CROSS_ATTR_PACKED {
      //! The signal-to-noise ratio with a minimum value of 32 and a maximum
      //! value of 31
      int8_t snr : 6;
      //! Reserved
      uint8_t rfu : 2;
    } bits;
  } radio_status;
};

/**
 * \brief LoraWan Mac NewChannelReq payload
 */
struct __CROSS_ATTR_PACKED ulorawan_mac_new_channel_req {
  //! The index of the channel being created or modified
  uint8_t ch_index;
  //! The Frequency field is a 24-bit unsigned integer. The actual channel
  //! frequency (in Hz) is 100 X Frequency
  uint8_t freq[ULORAWAN_FREQ_SIZE];
  //! The data-rate range (DRRange) field specifies the uplink data-rate
  //! range allowed for this channel
  union __CROSS_ATTR_PACKED {
    //! The value
    uint8_t value;
    struct __CROSS_ATTR_PACKED {
      //! The maximum data rate (MaxDR) designates the highest uplink data
      //! rate.
      uint8_t maxdr : 4;
      //! The minimum data rate (MinDR) subfield designates the lowest uplink
      //! data rate allowed on this channel
      uint8_t mindr : 4;
    } bits;
  } dr_range;
};

/**
 * \brief LoraWan Mac NewChannelAns payload
 */
struct __CROSS_ATTR_PACKED ulorawan_mac_new_channel_ans {
  union __CROSS_ATTR_PACKED {
    //! The value
    uint8_t value;
    union __CROSS_ATTR_PACKED {
      //! reserved
      uint8_t rfu : 6;
      //! Data-rate range ok
      uint8_t data_rate_ok : 1;
      //! Channel frequency ok
      uint8_t ch_freq_ok : 1;
    } bits;
  } status;
};

/**
 * \brief LoraWan Mac DlChannelReq payload
 */
struct __CROSS_ATTR_PACKED ulorawan_mac_download_channel_req {
  //! The channel index (ChIndex) is the index of the channel whose downlink
  //! frequency is modified.
  uint8_t ch_index;
  //! The frequency (Frequency) field is a 24-bit unsigned integer. The
  //! actual downlink frequency (in Hz) is 100 X Frequency
  uint8_t freq[ULORAWAN_FREQ_SIZE];
};

/**
 * \brief LoraWan Mac DlChannelAns payload
 */
struct __CROSS_ATTR_PACKED ulorawan_mac_download_channel_ans {
  union __CROSS_ATTR_PACKED {
    //! The value
    uint8_t value;
    union __CROSS_ATTR_PACKED {
      //! reserved
      uint8_t rfu : 6;
      //! Uplink frequency exists
      uint8_t upload_freq_exits : 1;
      //! Channel frequency ok
      uint8_t ch_freq_ok : 1;
    } bits;
  } status;
};

/**
 * \brief LoraWan Mac RXTimingSetupReq payload
 */
struct __CROSS_ATTR_PACKED ulorawan_mac_rx_timing_setup_req {
  union __CROSS_ATTR_PACKED {
    //! The value
    uint8_t value;
    union __CROSS_ATTR_PACKED {
      //! reserved
      uint8_t rfu : 4;
      //! The rx delay interval in seconds
      uint8_t delay : 4;
    } bits;
  } rx_timing_status;
};

/**
 * \brief LoraWan Mac TXParamSetupReq payload
 */
struct __CROSS_ATTR_PACKED ulorawan_mac_tx_param_setup_req {
  union __CROSS_ATTR_PACKED {
    //! The value
    uint8_t value;
    union __CROSS_ATTR_PACKED {
      //! reserved
      uint8_t rfu : 2;
      //! The maximum downlink dwell time
      uint8_t downlink_dwell_time : 1;
      //! The maximum uplink dwell time
      uint8_t uplink_dwell_time : 1;
      //! The MaxEIRP value, the the maximum allowed end-device Effective
      //! Isotropic Radiated Power (EIRP)
      uint8_t max_erip : 4;
    } bits;
  } eirp_dwell_time;
};

/**
 * \brief LoraWan Mac DeviceTimeAns payload
 */
struct __CROSS_ATTR_PACKED ulorawan_mac_dev_time_ans {
  //! The GPS epoch (i.e., January 6, 1980 00:00:00 UTC) is used as origin.
  uint32_t seconds_since_epoch;
  //! The fractional seconds in 1/256 increments
  uint8_t fract_seconds;
};

/**
 * \brief LoraWan Mac join accept
 */
struct __CROSS_ATTR_PACKED ulorawan_mac_join_accept {
  //! The Join-Server Nonce
  uint8_t join_nonce[ULORAWAN_MAC_JOIN_NONCE_SIZE];
  //! The network id
  uint8_t net_id[ULORAWAN_MAC_NET_ID_SIZE];
  //! The end-device address
  uint32_t device_address;
  //! The downlink configuration settings
  union __CROSS_ATTR_PACKED {
    //! The value
    uint8_t value;
    struct __CROSS_ATTR_PACKED {
      //! Data rate of a downlink using the second receive window
      uint8_t RX2DataRate : 4;
      //! Offset between up and downlink datarate of first reception slot
      uint8_t RX1DRoffset : 3;
      uint8_t rfu : 1;
    } bits;
  } dl_settings;
  //! The delay between TA and RX
  uint8_t rx_delay;
  //! An OPTIONAL list of network parameters (CFList)
  uint8_t cf_list[ULORAWAN_MAC_CF_LIST_SIZE];
};

/**
 * \brief LoraWan Mac PingSlotInfoReq
 */
struct __CROSS_ATTR_PACKED ulorawan_mac_ping_slot_info_req {
  //! The ping slot parameters
  union __CROSS_ATTR_PACKED {
    //! The value
    uint8_t value;
    union __CROSS_ATTR_PACKED {
      //! reserved
      uint8_t rfu : 5;
      //! The rx delay interval in seconds
      uint8_t periodicity : 3;
    } bits;
  }ping_slot_param;
};

/**
 * \brief LoraWan Mac BeaconFreqReq. This command is sent by the server to the
 * end-device to modify the frequency on which this end-device expects the
 * beacon.
 */
struct ulorawan_mac_beacon_freq_req {
  uint8_t freq[ULORAWAN_FREQ_SIZE];
};

#ifdef __cplusplus
}
#endif

#endif /* ULORAWAN_MAC_CMDS_H_ */
