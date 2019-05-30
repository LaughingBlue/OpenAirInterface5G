/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file common_lib.h
 * \brief common APIs for different RF frontend device
 * \author HongliangXU, Navid Nikaein
 * \date 2015
 * \version 0.2
 * \company Eurecom
 * \maintainer:  navid.nikaein@eurecom.fr
 * \note
 * \warning
 */

#ifndef COMMON_LIB_H
#define COMMON_LIB_H
#include <stdint.h>
#include <sys/types.h>

/* name of shared library implementing the radio front end */
#define OAI_RF_LIBNAME        "oai_device"
/* name of shared library implementing the transport */
#define OAI_TP_LIBNAME        "oai_transpro"
/* name of shared library implementing the basic/rf simulator */
#define OAI_RFSIM_LIBNAME        "rfsimulator"
/* name of shared library implementing the basic/rf simulator */
#define OAI_BASICSIM_LIBNAME        "tcp_bridge_oai"
/* flags for BBU to determine whether the attached radio head is local or remote */
#define RAU_LOCAL_RADIO_HEAD  0
#define RAU_REMOTE_RADIO_HEAD 1

#ifndef MAX_CARDS
  #define MAX_CARDS 8
#endif

typedef int64_t openair0_timestamp;
typedef volatile int64_t openair0_vtimestamp;


/*!\brief structrue holds the parameters to configure USRP devices*/
typedef struct openair0_device_t openair0_device;






//#define USRP_GAIN_OFFSET (56.0)  // 86 calibrated for USRP B210 @ 2.6 GHz to get equivalent RS EPRE in OAI to SMBV100 output

typedef enum {
  max_gain=0,med_gain,byp_gain
} rx_gain_t;

#if OCP_FRAMEWORK
#include <enums.h>
#else
typedef enum {
  duplex_mode_TDD=1,duplex_mode_FDD=0
} duplex_mode_t;
#endif


/** @addtogroup _GENERIC_PHY_RF_INTERFACE_
 * @{
 */
/*!\brief RF device types
 */
#ifdef OCP_FRAMEWORK
#include <enums.h>
#else
typedef enum {
  MIN_RF_DEV_TYPE = 0,
  /*!\brief device is ExpressMIMO */
  EXMIMO_DEV,
  /*!\brief device is USRP B200/B210*/
  USRP_B200_DEV,
  /*!\brief device is USRP X300/X310*/
  USRP_X300_DEV,
  /*!\brief device is BLADE RF*/
  BLADERF_DEV,
  /*!\brief device is LMSSDR (SoDeRa)*/
  LMSSDR_DEV,
  /*!\brief device is Iris */
  IRIS_DEV,
  /*!\brief device is NONE*/
  NONE_DEV,
  MAX_RF_DEV_TYPE

} dev_type_t;
#endif

/*!\brief transport protocol types
 */
typedef enum {
  MIN_TRANSP_TYPE = 0,
  /*!\brief transport protocol ETHERNET */
  ETHERNET_TP,
  /*!\brief no transport protocol*/
  NONE_TP,
  MAX_TRANSP_TYPE

} transport_type_t;


/*!\brief  openair0 device host type */
typedef enum {
  MIN_HOST_TYPE = 0,
  /*!\brief device functions within a RAU */
  RAU_HOST,
  /*!\brief device functions within a RRU */
  RRU_HOST,
  MAX_HOST_TYPE

} host_type_t;


/*! \brief RF Gain clibration */
typedef struct {
  //! Frequency for which RX chain was calibrated
  double freq;
  //! Offset to be applied to RX gain
  double offset;
} rx_gain_calib_table_t;

/*! \brief Clock source types */
typedef enum {
  //! This tells the underlying hardware to use the internal reference
  internal=0,
  //! This tells the underlying hardware to use the external reference
  external=1,
  //! This tells the underlying hardware to use the gpsdo reference
  gpsdo=2
} clock_source_t;

/*! \brief RF frontend parameters set by application */
typedef struct {
  //! Module ID for this configuration
  int Mod_id;
  //! device log level
  int log_level;
  //! duplexing mode
  duplex_mode_t duplex_mode;
  //! number of downlink resource blocks
  int num_rb_dl;
  //! number of samples per frame
  unsigned int  samples_per_frame;
  //! the sample rate for both transmit and receive.
  double sample_rate;
  //! flag to indicate that the device is doing mmapped DMA transfers
  int mmapped_dma;
  //! offset in samples between TX and RX paths
  int tx_sample_advance;
  //! samples per packet on the fronthaul interface
  int samples_per_packet;
  //! number of RX channels (=RX antennas)
  int rx_num_channels;
  //! number of TX channels (=TX antennas)
  int tx_num_channels;
  //! \brief RX base addresses for mmapped_dma
  int32_t *rxbase[4];
  //! \brief TX base addresses for mmapped_dma
  int32_t *txbase[4];
  //! \brief Center frequency in Hz for RX.
  //! index: [0..rx_num_channels[
  double rx_freq[4];
  //! \brief Center frequency in Hz for TX.
  //! index: [0..rx_num_channels[ !!! see lte-ue.c:427 FIXME iterates over rx_num_channels
  double tx_freq[4];
  //! \brief memory
  //! \brief Pointer to Calibration table for RX gains
  rx_gain_calib_table_t *rx_gain_calib_table;

  //! mode for rxgain (ExpressMIMO2)
  rx_gain_t rxg_mode[4];
  //! \brief Gain for RX in dB.
  //! index: [0..rx_num_channels]
  double rx_gain[4];
  //! \brief Gain offset (for calibration) in dB
  //! index: [0..rx_num_channels]
  double rx_gain_offset[4];
  //! gain for TX in dB
  double tx_gain[4];
  //! RX bandwidth in Hz
  double rx_bw;
  //! TX bandwidth in Hz
  double tx_bw;
  //! clock source
  clock_source_t clock_source;
  //! Manual SDR IP address
  char *sdr_addrs;
  //! Auto calibration flag
  int autocal[4];
  //! rf devices work with x bits iqs when oai have its own iq format
  //! the two following parameters are used to convert iqs
  int iq_txshift;
  int iq_rxrescale;
  //! Configuration file for LMS7002M
  char *configFilename;
#if defined(USRP_REC_PLAY)
  unsigned short sf_mode;           // 1=record, 2=replay
  char           sf_filename[1024]; // subframes file path
  unsigned int   sf_max;            // max number of recorded subframes
  unsigned int   sf_loops;          // number of loops in replay mode
  unsigned int   sf_read_delay;     // read delay in replay mode
  unsigned int   sf_write_delay;    // write delay in replay mode
  unsigned int   eth_mtu;           // ethernet MTU
#endif
} openair0_config_t;

/*! \brief RF mapping */
typedef struct {
  //! card id
  int card;
  //! rf chain id
  int chain;
} openair0_rf_map;


typedef struct {
  char *remote_addr;
  //! remote port number for Ethernet interface (control)
  uint16_t remote_portc;
  //! remote port number for Ethernet interface (user)
  uint16_t remote_portd;
  //! local IP/MAC addr for Ethernet interface (eNB/RAU, UE)
  char *my_addr;
  //! local port number (control) for Ethernet interface (eNB/RAU, UE)
  uint16_t  my_portc;
  //! local port number (user) for Ethernet interface (eNB/RAU, UE)
  uint16_t  my_portd;
  //! local Ethernet interface (eNB/RAU, UE)
  char *local_if_name;
  //! transport type preference  (RAW/UDP)
  uint8_t transp_preference;
  //! compression enable (0: No comp/ 1: A-LAW)
  uint8_t if_compress;
} eth_params_t;


typedef struct {
  //! Tx buffer for if device, keep one per subframe now to allow multithreading
  void *tx[10];
  //! Tx buffer (PRACH) for if device
  void *tx_prach;
  //! Rx buffer for if device
  void *rx;
} if_buffer_t;


/*!\brief structure holds the parameters to configure USRP devices */
struct openair0_device_t {
  /*!brief Module ID of this device */
  int Mod_id;

  /*!brief Component Carrier ID of this device */
  int CC_id;

  /*!brief Type of this device */
  dev_type_t type;

  /*!brief Transport protocol type that the device suppports (in case I/Q samples need to be transported) */
  transport_type_t transp_type;

  /*!brief Type of the device's host (RAU/RRU) */
  host_type_t host_type;

  /* !brief RF frontend parameters set by application */
  openair0_config_t *openair0_cfg;

  /* !brief ETH params set by application */
  eth_params_t *eth_params;

  /*!brief Can be used by driver to hold internal structure*/
  void *priv;

  /* Functions API, which are called by the application*/

  /*! \brief Called to start the transceiver. Return 0 if OK, < 0 if error
      @param device pointer to the device structure specific to the RF hardware target
  */
  int (*trx_start_func)(openair0_device *device);

  /*! \brief Called to send a request message between RAU-RRU on control port
      @param device pointer to the device structure specific to the RF hardware target
      @param msg pointer to the message structure passed between RAU-RRU
      @param msg_len length of the message
  */
  int (*trx_ctlsend_func)(openair0_device *device, void *msg, ssize_t msg_len);

  /*! \brief Called to receive a reply  message between RAU-RRU on control port
      @param device pointer to the device structure specific to the RF hardware target
      @param msg pointer to the message structure passed between RAU-RRU
      @param msg_len length of the message
  */
  int (*trx_ctlrecv_func)(openair0_device *device, void *msg, ssize_t msg_len);

  /*! \brief Called to send samples to the RF target
      @param device pointer to the device structure specific to the RF hardware target
      @param timestamp The timestamp at whicch the first sample MUST be sent
      @param buff Buffer which holds the samples
      @param nsamps number of samples to be sent
      @param antenna_id index of the antenna if the device has multiple anteannas
      @param flags flags must be set to TRUE if timestamp parameter needs to be applied
  */
  int (*trx_write_func)(openair0_device *device, openair0_timestamp timestamp, void **buff, int nsamps,int antenna_id, int flags);

  /*! \brief Receive samples from hardware.
   * Read \ref nsamps samples from each channel to buffers. buff[0] is the array for
   * the first channel. *ptimestamp is the time at which the first sample
   * was received.
   * \param device the hardware to use
   * \param[out] ptimestamp the time at which the first sample was received.
   * \param[out] buff An array of pointers to buffers for received samples. The buffers must be large enough to hold the number of samples \ref nsamps.
   * \param nsamps Number of samples. One sample is 2 byte I + 2 byte Q => 4 byte.
   * \param antenna_id Index of antenna for which to receive samples
   * \returns the number of sample read
   */
  int (*trx_read_func)(openair0_device *device, openair0_timestamp *ptimestamp, void **buff, int nsamps,int antenna_id);

  /*! \brief print the device statistics
   * \param device the hardware to use
   * \returns  0 on success
   */
  int (*trx_get_stats_func)(openair0_device *device);

  /*! \brief Reset device statistics
   * \param device the hardware to use
   * \returns 0 in success
   */
  int (*trx_reset_stats_func)(openair0_device *device);

  /*! \brief Terminate operation of the transceiver -- free all associated resources
   * \param device the hardware to use
   */
  void (*trx_end_func)(openair0_device *device);

  /*! \brief Stop operation of the transceiver
   */
  int (*trx_stop_func)(openair0_device *device);

  /* Functions API related to UE*/

  /*! \brief Set RX feaquencies
   * \param device the hardware to use
   * \param openair0_cfg RF frontend parameters set by application
   * \param exmimo_dump_config  dump EXMIMO configuration
   * \returns 0 in success
   */
  int (*trx_set_freq_func)(openair0_device *device, openair0_config_t *openair0_cfg,int exmimo_dump_config);

  /*! \brief Set gains
   * \param device the hardware to use
   * \param openair0_cfg RF frontend parameters set by application
   * \returns 0 in success
   */
  int (*trx_set_gains_func)(openair0_device *device, openair0_config_t *openair0_cfg);

  /*! \brief RRU Configuration callback
   * \param idx RU index
   * \param arg pointer to capabilities or configuration
   */
  void (*configure_rru)(int idx, void *arg);
};

/* type of device init function, implemented in shared lib */
typedef int(*oai_device_initfunc_t)(openair0_device *device, openair0_config_t *openair0_cfg);
/* type of transport init function, implemented in shared lib */
typedef int(*oai_transport_initfunc_t)(openair0_device *device, openair0_config_t *openair0_cfg, eth_params_t *eth_params);

#ifdef __cplusplus
extern "C"
{
#endif


/*! \brief Initialize openair RF target. It returns 0 if OK */
int openair0_device_load(openair0_device *device, openair0_config_t *openair0_cfg);
/*! \brief Initialize transport protocol . It returns 0 if OK */
int openair0_transport_load(openair0_device *device, openair0_config_t *openair0_cfg, eth_params_t *eth_params);


/*! \brief Get current timestamp of USRP
 * \param device the hardware to use
 */
openair0_timestamp get_usrp_time(openair0_device *device);

/*! \brief Set RX frequencies
 * \param device the hardware to use
 * \param openair0_cfg RF frontend parameters set by application
 * \returns 0 in success
 */
int openair0_set_rx_frequencies(openair0_device *device, openair0_config_t *openair0_cfg);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif // COMMON_LIB_H

