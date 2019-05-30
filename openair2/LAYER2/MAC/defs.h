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

/*! \file LAYER2/MAC/defs.h
* \brief MAC data structures, constant, and function prototype
* \author Navid Nikaein and Raymond Knopp
* \date 2011
* \version 0.5
* \email navid.nikaein@eurecom.fr

*/
/** @defgroup _oai2  openair2 Reference Implementation
 * @ingroup _ref_implementation_
 * @{
 */

/*@}*/

#ifndef __LAYER2_MAC_DEFS_H__
#define __LAYER2_MAC_DEFS_H__



#ifdef USER_MODE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

//#include "COMMON/openair_defs.h"



#include "PHY/defs.h"
#include "PHY/LTE_TRANSPORT/defs.h"
#include "COMMON/platform_constants.h"
#include "BCCH-BCH-Message.h"
#include "RadioResourceConfigCommon.h"
#include "RadioResourceConfigDedicated.h"
#include "MeasGapConfig.h"
#include "SchedulingInfoList.h"
#include "TDD-Config.h"
#include "RACH-ConfigCommon.h"
#include "MeasObjectToAddModList.h"
#include "MobilityControlInfo.h"
#if (LTE_RRC_VERSION >= MAKE_VERSION(10, 0, 0))
#include "MBSFN-AreaInfoList-r9.h"
#include "MBSFN-SubframeConfigList.h"
#include "PMCH-InfoList-r9.h"
#include "SCellToAddMod-r10.h"
#endif
#if (LTE_RRC_VERSION >= MAKE_VERSION(14, 0, 0))
#include "SystemInformationBlockType1-v1310-IEs.h"
#endif

#include "nfapi_interface.h"
#include "PHY_INTERFACE/IF_Module.h"

/** @defgroup _mac  MAC
 * @ingroup _oai2
 * @{
 */

#define BCCH_PAYLOAD_SIZE_MAX 128
#define CCCH_PAYLOAD_SIZE_MAX 128
#define PCCH_PAYLOAD_SIZE_MAX 128
#define RAR_PAYLOAD_SIZE_MAX 128

#define SCH_PAYLOAD_SIZE_MAX 4096
/// Logical channel ids from 36-311 (Note BCCH is not specified in 36-311, uses the same as first DRB)

#if (LTE_RRC_VERSION >= MAKE_VERSION(10, 0, 0))

// Mask for identifying subframe for MBMS
#define MBSFN_TDD_SF3 0x80// for TDD
#define MBSFN_TDD_SF4 0x40
#define MBSFN_TDD_SF7 0x20
#define MBSFN_TDD_SF8 0x10
#define MBSFN_TDD_SF9 0x08
#define MBSFN_FDD_SF1 0x80// for FDD
#define MBSFN_FDD_SF2 0x40
#define MBSFN_FDD_SF3 0x20
#define MBSFN_FDD_SF6 0x10
#define MBSFN_FDD_SF7 0x08
#define MBSFN_FDD_SF8 0x04

#define MAX_MBSFN_AREA 8
#define MAX_PMCH_perMBSFN 15
/*!\brief MAX MCCH payload size  */
#define MCCH_PAYLOAD_SIZE_MAX 128
//#define MCH_PAYLOAD_SIZE_MAX 16384// this value is using in case mcs and TBS index are high
#endif

#ifdef USER_MODE
#define printk printf
#endif //USER_MODE

/*!\brief Maximum number of logical channl group IDs */
#define MAX_NUM_LCGID 4
/*!\brief logical channl group ID 0 */
#define LCGID0 0
/*!\brief logical channl group ID 1 */
#define LCGID1 1
/*!\brief logical channl group ID 2 */
#define LCGID2 2
/*!\brief logical channl group ID 3 */
#define LCGID3 3
/*!\brief Maximum number of logical chanels */
#define MAX_NUM_LCID 11
/*!\brief Maximum number od control elemenets */
#define MAX_NUM_CE 5
/*!\brief Maximum number of random access process */
#define NB_RA_PROC_MAX 4
/*!\brief size of buffer status report table */
#define BSR_TABLE_SIZE 64
/*!\brief The power headroom reporting range is from -23 ...+40 dB and beyond, with step 1 */
#define PHR_MAPPING_OFFSET 23  // if ( x>= -23 ) val = floor (x + 23)
/*!\brief maximum number of resource block groups */
#define N_RBG_MAX 25 // for 20MHz channel BW
/*!\brief minimum value for channel quality indicator */
#define MIN_CQI_VALUE  0
/*!\brief maximum value for channel quality indicator */
#define MAX_CQI_VALUE  15
/*!\briefmaximum number of supported bandwidth (1.4, 5, 10, 20 MHz) */
#define MAX_SUPPORTED_BW  4
/*!\brief CQI values range from 1 to 15 (4 bits) */
#define CQI_VALUE_RANGE 16

/*!\brief value for indicating BSR Timer is not running */
#define MAC_UE_BSR_TIMER_NOT_RUNNING   (0xFFFF)

#define LCID_EMPTY 0
#define LCID_NOT_EMPTY 1

/*!\brief minimum RLC PDU size to be transmitted = min RLC Status PDU or RLC UM PDU SN 5 bits */
#define MIN_RLC_PDU_SIZE    (2)

/*!\brief minimum MAC data needed for transmitting 1 min RLC PDU size + 1 byte MAC subHeader */
#define MIN_MAC_HDR_RLC_SIZE    (1 + MIN_RLC_PDU_SIZE)

/*!\brief maximum number of slices / groups */
#define MAX_NUM_SLICES 4

/*
 * eNB part
 */


/*
 * UE/ENB common part
 */
/*!\brief MAC header of Random Access Response for Random access preamble identifier (RAPID) */
typedef struct {
  uint8_t RAPID:6;
  uint8_t T:1;
  uint8_t E:1;
} __attribute__((__packed__))RA_HEADER_RAPID;

/*!\brief  MAC header of Random Access Response for backoff indicator (BI)*/
typedef struct {
  uint8_t BI:4;
  uint8_t R:2;
  uint8_t T:1;
  uint8_t E:1;
} __attribute__((__packed__))RA_HEADER_BI;
/*
typedef struct {
  uint64_t padding:16;
  uint64_t t_crnti:16;
  uint64_t hopping_flag:1;
  uint64_t rb_alloc:10;
  uint64_t mcs:4;
  uint64_t TPC:3;
  uint64_t UL_delay:1;
  uint64_t cqi_req:1;
  uint64_t Timing_Advance_Command:11;  // first/2nd octet LSB
  uint64_t R:1;                        // octet MSB
  } __attribute__((__packed__))RAR_PDU;

typedef struct {
  uint64_t padding:16;
  uint64_t R:1;                        // octet MSB
  uint64_t Timing_Advance_Command:11;  // first/2nd octet LSB
  uint64_t cqi_req:1;
  uint64_t UL_delay:1;
  uint64_t TPC:3;
  uint64_t mcs:4;
  uint64_t rb_alloc:10;
  uint64_t hopping_flag:1;
  uint64_t t_crnti:16;
  } __attribute__((__packed__))RAR_PDU;

#define sizeof_RAR_PDU 6
*/
/*!\brief  MAC subheader short with 7bit Length field */
typedef struct {
  uint8_t LCID:5;  // octet 1 LSB
  uint8_t E:1;
  uint8_t R:2;     // octet 1 MSB
  uint8_t L:7;     // octet 2 LSB
  uint8_t F:1;     // octet 2 MSB
} __attribute__((__packed__))SCH_SUBHEADER_SHORT;
/*!\brief  MAC subheader long  with 15bit Length field */
typedef struct {
  uint8_t LCID:5;   // octet 1 LSB
  uint8_t E:1;
  uint8_t R:2;      // octet 1 MSB
  uint8_t L_MSB:7;
  uint8_t F:1;      // octet 2 MSB
  uint8_t L_LSB:8;
  uint8_t padding;
} __attribute__((__packed__))SCH_SUBHEADER_LONG;
/*!\brief MAC subheader short without length field */
typedef struct {
  uint8_t LCID:5;
  uint8_t E:1;
  uint8_t R:2;
} __attribute__((__packed__))SCH_SUBHEADER_FIXED;

/*!\brief  mac control element: short buffer status report for a specific logical channel group ID*/
typedef struct {
  uint8_t Buffer_size:6;  // octet 1 LSB
  uint8_t LCGID:2;        // octet 1 MSB
} __attribute__((__packed__))BSR_SHORT;

typedef BSR_SHORT BSR_TRUNCATED;
/*!\brief  mac control element: long buffer status report for all logical channel group ID*/
typedef struct {
  uint8_t Buffer_size3:6;
  uint8_t Buffer_size2:6;
  uint8_t Buffer_size1:6;
  uint8_t Buffer_size0:6;
} __attribute__((__packed__))BSR_LONG;

#define BSR_LONG_SIZE  (sizeof(BSR_LONG))
/*!\brief  mac control element: timing advance  */
typedef struct {
  uint8_t TA:6;
  uint8_t R:2;
} __attribute__((__packed__))TIMING_ADVANCE_CMD;
/*!\brief  mac control element: power headroom report  */
typedef struct {
  uint8_t PH:6;
  uint8_t R:2;
} __attribute__((__packed__))POWER_HEADROOM_CMD;

/*! \brief MIB payload */
typedef struct {
  uint8_t payload[3] ;
} __attribute__((__packed__))MIB_PDU;
/*! \brief CCCH payload */
typedef struct {
  uint8_t payload[CCCH_PAYLOAD_SIZE_MAX] ;
} __attribute__((__packed__))CCCH_PDU;
/*! \brief BCCH payload */
typedef struct {
  uint8_t payload[BCCH_PAYLOAD_SIZE_MAX] ;
} __attribute__((__packed__))BCCH_PDU;
/*! \brief RAR payload */
typedef struct {
  uint8_t payload[RAR_PAYLOAD_SIZE_MAX];
} __attribute__ ((__packed__)) RAR_PDU;
/*! \brief BCCH payload */
typedef struct {
  uint8_t payload[PCCH_PAYLOAD_SIZE_MAX] ;
} __attribute__((__packed__))PCCH_PDU;

#if (LTE_RRC_VERSION >= MAKE_VERSION(10, 0, 0))
/*! \brief MCCH payload */
typedef struct {
  uint8_t payload[MCCH_PAYLOAD_SIZE_MAX] ;
} __attribute__((__packed__))MCCH_PDU;
/*!< \brief MAC control element for activation and deactivation of component carriers */
typedef struct {
  uint8_t C7:1;/*!< \brief Component carrier 7 */
  uint8_t C6:1;/*!< \brief Component carrier 6 */
  uint8_t C5:1;/*!< \brief Component carrier 5 */
  uint8_t C4:1;/*!< \brief Component carrier 4 */
  uint8_t C3:1;/*!< \brief Component carrier 3 */
  uint8_t C2:1;/*!< \brief Component carrier 2 */
  uint8_t C1:1;/*!< \brief Component carrier 1 */
  uint8_t R:1;/*!< \brief Reserved  */
} __attribute__((__packed__))CC_ELEMENT;
/*! \brief MAC control element: MCH Scheduling Information */
typedef struct {
  uint8_t stop_sf_MSB:3; // octet 1 LSB
  uint8_t lcid:5;        // octet 2 MSB
  uint8_t stop_sf_LSB:8;
} __attribute__((__packed__))MSI_ELEMENT;
#endif
/*! \brief Values of CCCH LCID for DLSCH */
#define CCCH_LCHANID 0
/*!\brief Values of BCCH logical channel (fake)*/
#define BCCH 3  // SI
/*!\brief Values of PCCH logical channel (fake)*/
#define PCCH 4  // Paging
/*!\brief Values of PCCH logical channel (fake) */
#define MIBCH 5  // MIB
/*!\brief Values of BCCH SIB1_BR logical channel (fake) */
#define BCCH_SIB1_BR 6  // SIB1_BR
/*!\brief Values of BCCH SIB_BR logical channel (fake) */
#define BCCH_SI_BR 7  // SI-BR
/*!\brief Value of CCCH / SRB0 logical channel */
#define CCCH 0  // srb0
/*!\brief DCCH / SRB1 logical channel */
#define DCCH 1  // srb1
/*!\brief DCCH1 / SRB2  logical channel */
#define DCCH1 2 // srb2
/*!\brief DTCH DRB1  logical channel */
#define DTCH 3 // LCID
/*!\brief MCCH logical channel */
#define MCCH 4
/*!\brief MTCH logical channel */
#define MTCH 1
// DLSCH LCHAN ID
/*!\brief LCID of UE contention resolution identity for DLSCH*/
#define UE_CONT_RES 28
/*!\brief LCID of timing advance for DLSCH */
#define TIMING_ADV_CMD 29
/*!\brief LCID of discontinous reception mode for DLSCH */
#define DRX_CMD 30
/*!\brief LCID of padding LCID for DLSCH */
#define SHORT_PADDING 31

#if (LTE_RRC_VERSION >= MAKE_VERSION(10, 0, 0))
// MCH LCHAN IDs (table6.2.1-4 TS36.321)
/*!\brief LCID of MCCH for DL */
#define MCCH_LCHANID 0
/*!\brief LCID of MCH scheduling info for DL */
#define MCH_SCHDL_INFO 3
/*!\brief LCID of Carrier component activation/deactivation */
#define CC_ACT_DEACT 27
#endif

// ULSCH LCHAN IDs
/*!\brief LCID of extended power headroom for ULSCH */
#define EXTENDED_POWER_HEADROOM 25
/*!\brief LCID of power headroom for ULSCH */
#define POWER_HEADROOM 26
/*!\brief LCID of CRNTI for ULSCH */
#define CRNTI 27
/*!\brief LCID of truncated BSR for ULSCH */
#define TRUNCATED_BSR 28
/*!\brief LCID of short BSR for ULSCH */
#define SHORT_BSR 29
/*!\brief LCID of long BSR for ULSCH */
#define LONG_BSR 30
/*!\bitmaps for BSR Triggers */
#define	BSR_TRIGGER_NONE		(0)			/* No BSR Trigger */
#define	BSR_TRIGGER_REGULAR		(1)			/* For Regular and ReTxBSR Expiry Triggers */
#define	BSR_TRIGGER_PERIODIC	(2)			/* For BSR Periodic Timer Expiry Trigger */
#define	BSR_TRIGGER_PADDING		(4)			/* For Padding BSR Trigger */


/*! \brief Downlink SCH PDU Structure */
typedef struct {
  uint8_t payload[8][SCH_PAYLOAD_SIZE_MAX];
  uint16_t Pdu_size[8];
} __attribute__ ((__packed__)) DLSCH_PDU;


/*! \brief MCH PDU Structure */
typedef struct {
  int8_t payload[SCH_PAYLOAD_SIZE_MAX];
  uint16_t Pdu_size;
  uint8_t mcs;
  uint8_t sync_area;
  uint8_t msi_active;
  uint8_t mcch_active;
  uint8_t mtch_active;
} __attribute__ ((__packed__)) MCH_PDU;

/*! \brief Uplink SCH PDU Structure */
typedef struct {
  int8_t payload[SCH_PAYLOAD_SIZE_MAX];         /*!< \brief SACH payload */
  uint16_t Pdu_size;
} __attribute__ ((__packed__)) ULSCH_PDU;

#include "PHY/impl_defs_top.h"

/*!\brief  UE ULSCH scheduling states*/
typedef enum {
  S_UL_NONE =0,
  S_UL_WAITING,
  S_UL_SCHEDULED,
  S_UL_BUFFERED,
  S_UL_NUM_STATUS
} UE_ULSCH_STATUS;

/*!\brief  UE DLSCH scheduling states*/
typedef enum {
  S_DL_NONE =0,
  S_DL_WAITING,
  S_DL_SCHEDULED,
  S_DL_BUFFERED,
  S_DL_NUM_STATUS
} UE_DLSCH_STATUS;

/*!\brief  scheduling policy for the contention-based access */
typedef enum {
  CBA_ES=0, /// equal share of RB among groups w
  CBA_ES_S,  /// equal share of RB among groups with small allocation
  CBA_PF, /// proportional fair (kind of)
  CBA_PF_S,  /// proportional fair (kind of) with small RB allocation
  CBA_RS /// random allocation
} CBA_POLICY;


/*! \brief temporary struct for ULSCH sched */
typedef struct {
  rnti_t rnti;
  uint16_t subframe;
  uint16_t serving_num;
  UE_ULSCH_STATUS status;
} eNB_ULSCH_INFO;
/*! \brief temp struct for DLSCH sched */
typedef struct {
  rnti_t rnti;
  uint16_t weight;
  uint16_t subframe;
  uint16_t serving_num;
  UE_DLSCH_STATUS status;
} eNB_DLSCH_INFO;
/*! \brief eNB overall statistics */
typedef struct {
  /// num BCCH PDU per CC
  uint32_t total_num_bcch_pdu;
  /// BCCH buffer size
  uint32_t bcch_buffer;
  /// total BCCH buffer size
  uint32_t total_bcch_buffer;
  /// BCCH MCS
  uint32_t bcch_mcs;

  /// num CCCH PDU per CC
  uint32_t total_num_ccch_pdu;
  /// BCCH buffer size
  uint32_t ccch_buffer;
  /// total BCCH buffer size
  uint32_t total_ccch_buffer;
  /// BCCH MCS
  uint32_t ccch_mcs;

/// num active users
  uint16_t num_dlactive_UEs;
  ///  available number of PRBs for a give SF
  uint16_t available_prbs;
  /// total number of PRB available for the user plane
  uint32_t total_available_prbs;
  /// aggregation
  /// total avilable nccc : num control channel element
  uint16_t available_ncces;
  // only for a new transmission, should be extended for retransmission
  // current dlsch  bit rate for all transport channels
  uint32_t dlsch_bitrate;
  //
  uint32_t dlsch_bytes_tx;
  //
  uint32_t dlsch_pdus_tx;
  //
  uint32_t total_dlsch_bitrate;
  //
  uint32_t total_dlsch_bytes_tx;
  //
  uint32_t total_dlsch_pdus_tx;

  // here for RX
  //
  uint32_t ulsch_bitrate;
  //
  uint32_t ulsch_bytes_rx;
  //
  uint64_t ulsch_pdus_rx;

  uint32_t total_ulsch_bitrate;
  //
  uint32_t total_ulsch_bytes_rx;
  //
  uint32_t total_ulsch_pdus_rx;


  /// MAC agent-related stats
  /// total number of scheduling decisions
  int sched_decisions;
  /// missed deadlines
  int missed_deadlines;

} eNB_STATS;
/*! \brief eNB statistics for the connected UEs*/
typedef struct {

  /// CRNTI of UE
  rnti_t crnti; ///user id (rnti) of connected UEs
  // rrc status
  uint8_t rrc_status;
  /// harq pid
  uint8_t harq_pid;
  /// harq rounf
  uint8_t harq_round;
  /// total available number of PRBs for a new transmission
  uint16_t rbs_used;
  /// total available number of PRBs for a retransmission
  uint16_t rbs_used_retx;
  /// total nccc used for a new transmission: num control channel element
  uint16_t ncce_used;
  /// total avilable nccc for a retransmission: num control channel element
  uint16_t ncce_used_retx;

  // mcs1 before the rate adaptaion
  uint8_t dlsch_mcs1;
  /// Target mcs2 after rate-adaptation
  uint8_t dlsch_mcs2;
  //  current TBS with mcs2
  uint32_t TBS;
  //  total TBS with mcs2
  //  uint32_t total_TBS;
  //  total rb used for a new transmission
  uint32_t total_rbs_used;
  //  total rb used for retransmission
  uint32_t total_rbs_used_retx;

   /// TX
  /// Num pkt
  uint32_t num_pdu_tx[NB_RB_MAX];
  /// num bytes
  uint32_t num_bytes_tx[NB_RB_MAX];
  /// num retransmission / harq
  uint32_t num_retransmission;
  /// instantaneous tx throughput for each TTI
  //  uint32_t tti_throughput[NB_RB_MAX];

  /// overall
  //
  uint32_t  dlsch_bitrate;
  //total
  uint32_t  total_dlsch_bitrate;
  /// headers+ CE +  padding bytes for a MAC PDU
  uint64_t overhead_bytes;
  /// headers+ CE +  padding bytes for a MAC PDU
  uint64_t total_overhead_bytes;
  /// headers+ CE +  padding bytes for a MAC PDU
  uint64_t avg_overhead_bytes;
  // MAC multiplexed payload
  uint64_t total_sdu_bytes;
  // total MAC pdu bytes
  uint64_t total_pdu_bytes;

  // total num pdu
  uint32_t total_num_pdus;
  //
  //  uint32_t avg_pdu_size;

  /// RX

  /// PUCCH1a/b power (dBm)
  int32_t Po_PUCCH_dBm;
  /// Indicator that Po_PUCCH has been updated by PHY
  int32_t Po_PUCCH_update;
  /// Uplink measured RSSI
  int32_t UL_rssi;
  /// preassigned mcs after rate adaptation
  uint8_t ulsch_mcs1;
  /// adjusted mcs
  uint8_t ulsch_mcs2;

  /// estimated average pdu inter-departure time
  uint32_t avg_pdu_idt;
  /// estimated average pdu size
  uint32_t avg_pdu_ps;
  ///
  uint32_t aggregated_pdu_size;
  uint32_t aggregated_pdu_arrival;

  ///  uplink transport block size
  uint32_t ulsch_TBS;

  ///  total rb used for a new uplink transmission
  uint32_t num_retransmission_rx;
  ///  total rb used for a new uplink transmission
  uint32_t rbs_used_rx;
   ///  total rb used for a new uplink retransmission
  uint32_t rbs_used_retx_rx;
  ///  total rb used for a new uplink transmission
  uint32_t total_rbs_used_rx;
  /// normalized rx power
  int32_t      normalized_rx_power;
   /// target rx power
  int32_t    target_rx_power;

  /// num rx pdu
  uint32_t num_pdu_rx[NB_RB_MAX];
  /// num bytes rx
  uint32_t num_bytes_rx[NB_RB_MAX];
  /// instantaneous rx throughput for each TTI
  //  uint32_t tti_goodput[NB_RB_MAX];
  /// errors
  uint32_t num_errors_rx;

  uint64_t overhead_bytes_rx;
  /// headers+ CE +  padding bytes for a MAC PDU
  uint64_t total_overhead_bytes_rx;
  /// headers+ CE +  padding bytes for a MAC PDU
  uint64_t avg_overhead_bytes_rx;
 //
  uint32_t  ulsch_bitrate;
  //total
  uint32_t  total_ulsch_bitrate;
  /// overall
  ///  MAC pdu bytes
  uint64_t pdu_bytes_rx;
  /// total MAC pdu bytes
  uint64_t total_pdu_bytes_rx;
  /// total num pdu
  uint32_t total_num_pdus_rx;
  /// num of error pdus
  uint32_t total_num_errors_rx;

} eNB_UE_STATS;
/*! \brief eNB template for UE context information  */
typedef struct {
  /// C-RNTI of UE
  rnti_t rnti;
  /// NDI from last scheduling
  uint8_t oldNDI[8];
  /// mcs1 from last scheduling
  uint8_t oldmcs1[8];
  /// mcs2 from last scheduling
  uint8_t oldmcs2[8];
  /// NDI from last UL scheduling
  uint8_t oldNDI_UL[8];
  /// mcs from last UL scheduling
  uint8_t mcs_UL[8];
  /// TBS from last UL scheduling
  uint8_t TBS_UL[8];
  /// CQI_req from last scheduling
  uint8_t oldCQI_UL[8];
  /// TPC from last scheduling
  uint8_t oldTPC_UL[8];
  /// Flag to indicate UL has been scheduled at least once
  boolean_t ul_active;
  /// Flag to indicate UE has been configured (ACK from RRCConnectionSetup received)
  boolean_t configured;

  /// MCS from last scheduling
  uint8_t mcs[8];

  /// TPC from last scheduling
  uint8_t oldTPC[8];

  // PHY interface info

  /// Number of Allocated RBs for DL after scheduling (prior to frequency allocation)
  uint16_t nb_rb[8]; // num_max_harq

  /// Number of Allocated RBs for UL after scheduling
  uint16_t nb_rb_ul[8]; // num_max_harq

  /// Number of Allocated RBs for UL after scheduling
  uint16_t first_rb_ul[8]; // num_max_harq

  /// Is CQI requested for UL after scheduling 1st transmission
  uint8_t cqi_req[8];         // num_max_harq

  /// Cyclic shift for DMRS after scheduling
  uint16_t cshift[8]; // num_max_harq

  /// Number of Allocated RBs by the ulsch preprocessor
  uint8_t pre_allocated_nb_rb_ul;

  /// index of Allocated RBs by the ulsch preprocessor
  int8_t pre_allocated_rb_table_index_ul;

  /// total allocated RBs
  int8_t total_allocated_rbs;

  /// pre-assigned MCS by the ulsch preprocessor
  uint8_t pre_assigned_mcs_ul;

  /// assigned MCS by the ulsch scheduler
  uint8_t assigned_mcs_ul;

  /// DL DAI
  uint8_t DAI;

  /// UL DAI
  uint8_t DAI_ul[10];

  /// UL Scheduling Request Received
  uint8_t ul_SR;

  ///Resource Block indication for each sub-band in MU-MIMO
  uint8_t rballoc_subband[8][50];

  // Logical channel info for link with RLC

  /// Last received UE BSR info for each logical channel group id
  uint8_t bsr_info[MAX_NUM_LCGID];

  /// LCGID mapping
  long lcgidmap[11];

  /// phr information
  int8_t phr_info;

  /// phr information
  int8_t phr_info_configured;

  ///dl buffer info
  uint32_t dl_buffer_info[MAX_NUM_LCID];
  /// total downlink buffer info
  uint32_t dl_buffer_total;
  /// total downlink pdus
  uint32_t dl_pdus_total;
  /// downlink pdus for each LCID
  uint32_t dl_pdus_in_buffer[MAX_NUM_LCID];
  /// creation time of the downlink buffer head for each LCID
  uint32_t dl_buffer_head_sdu_creation_time[MAX_NUM_LCID];
  /// maximum creation time of the downlink buffer head across all LCID
  uint32_t  dl_buffer_head_sdu_creation_time_max;
  /// a flag indicating that the downlink head SDU is segmented
  uint8_t    dl_buffer_head_sdu_is_segmented[MAX_NUM_LCID];
  /// size of remaining size to send for the downlink head SDU
  uint32_t dl_buffer_head_sdu_remaining_size_to_send[MAX_NUM_LCID];

  /// total uplink buffer size
  uint32_t ul_total_buffer;
  /// uplink buffer creation time for each LCID
  uint32_t ul_buffer_creation_time[MAX_NUM_LCGID];
  /// maximum uplink buffer creation time across all the LCIDs
  uint32_t ul_buffer_creation_time_max;
  /// uplink buffer size per LCID
  uint32_t ul_buffer_info[MAX_NUM_LCGID];

  /// UE tx power
  int32_t ue_tx_power;

  /// stores the frame where the last TPC was transmitted
  uint32_t pusch_tpc_tx_frame;
  uint32_t pusch_tpc_tx_subframe;
  uint32_t pucch_tpc_tx_frame;
  uint32_t pucch_tpc_tx_subframe;

#ifdef LOCALIZATION
  eNB_UE_estimated_distances distance;
#endif

#if (LTE_RRC_VERSION >= MAKE_VERSION(14, 0, 0))
  uint8_t rach_resource_type;

 uint16_t mpdcch_repetition_cnt;
  struct PhysicalConfigDedicated  *physicalConfigDedicated;
  frame_t Msg2_frame;
  sub_frame_t Msg2_subframe;
#endif
} UE_TEMPLATE;

/*! \brief scheduling control information set through an API (not used)*/
typedef struct {
  ///UL transmission bandwidth in RBs
  uint8_t ul_bandwidth[MAX_NUM_LCID];
  ///DL transmission bandwidth in RBs
  uint8_t dl_bandwidth[MAX_NUM_LCID];

  //To do GBR bearer
  uint8_t min_ul_bandwidth[MAX_NUM_LCID];

  uint8_t min_dl_bandwidth[MAX_NUM_LCID];

  ///aggregated bit rate of non-gbr bearer per UE
  uint64_t  ue_AggregatedMaximumBitrateDL;
  ///aggregated bit rate of non-gbr bearer per UE
  uint64_t  ue_AggregatedMaximumBitrateUL;
  ///CQI scheduling interval in subframes.
  uint16_t cqiSchedInterval;
  ///Contention resolution timer used during random access
  uint8_t mac_ContentionResolutionTimer;

  uint16_t max_allowed_rbs[MAX_NUM_LCID];

  uint8_t max_mcs[MAX_NUM_LCID];

  uint16_t priority[MAX_NUM_LCID];

  // resource scheduling information

  /// Current DL harq round per harq_pid on each CC
  uint8_t       round[MAX_NUM_CCs][10];
  /// Current Active TBs per harq_pid on each CC
  uint8_t       tbcnt[MAX_NUM_CCs][10];
  /// Current UL harq round per harq_pid on each CC
  uint8_t       round_UL[MAX_NUM_CCs][8];
  uint8_t       dl_pow_off[MAX_NUM_CCs];
  uint16_t      pre_nb_available_rbs[MAX_NUM_CCs];
  unsigned char rballoc_sub_UE[MAX_NUM_CCs][N_RBG_MAX];
  uint16_t      ta_timer;
  int16_t       ta_update;
  uint16_t      ul_consecutive_errors;
  int32_t       context_active_timer;
  int32_t       cqi_req_timer;
  int32_t       ul_inactivity_timer;
  int32_t       ul_failure_timer;
  int32_t       ul_scheduled;
  int32_t       ra_pdcch_order_sent;
  int32_t       ul_out_of_sync;
  int32_t       phr_received;
  uint8_t       periodic_ri_received[NFAPI_CC_MAX];
  uint8_t       aperiodic_ri_received[NFAPI_CC_MAX];
  uint32_t      pucch_tpc_accumulated[NFAPI_CC_MAX];
  uint8_t       pucch1_cqi_update[NFAPI_CC_MAX];
  uint8_t       pucch1_snr[NFAPI_CC_MAX];
  uint8_t       pucch2_cqi_update[NFAPI_CC_MAX];
  uint8_t       pucch2_snr[NFAPI_CC_MAX];
  uint8_t       pucch3_cqi_update[NFAPI_CC_MAX];
  uint8_t       pucch3_snr[NFAPI_CC_MAX];
  uint8_t       pusch_snr[NFAPI_CC_MAX];
  uint16_t      feedback_cnt[NFAPI_CC_MAX];
  uint16_t      timing_advance;
  uint16_t      timing_advance_r9;
  uint8_t       periodic_wideband_cqi[NFAPI_CC_MAX];
  uint8_t       periodic_wideband_spatial_diffcqi[NFAPI_CC_MAX];
  uint8_t       periodic_wideband_pmi[NFAPI_CC_MAX];
  uint8_t       periodic_subband_cqi[NFAPI_CC_MAX][16];
  uint8_t       periodic_subband_spatial_diffcqi[NFAPI_CC_MAX][16];
  uint8_t       aperiodic_subband_cqi0[NFAPI_CC_MAX][25];
  uint8_t       aperiodic_subband_pmi[NFAPI_CC_MAX][25];
  uint8_t       aperiodic_subband_diffcqi0[NFAPI_CC_MAX][25];
  uint8_t       aperiodic_subband_cqi1[NFAPI_CC_MAX][25];
  uint8_t       aperiodic_subband_diffcqi1[NFAPI_CC_MAX][25];
  uint8_t       aperiodic_wideband_cqi0[NFAPI_CC_MAX];
  uint8_t       aperiodic_wideband_pmi[NFAPI_CC_MAX];
  uint8_t       aperiodic_wideband_cqi1[NFAPI_CC_MAX];
  uint8_t       aperiodic_wideband_pmi1[NFAPI_CC_MAX];
  uint8_t       dl_cqi[NFAPI_CC_MAX];
} UE_sched_ctrl;
/*! \brief eNB template for the Random access information */
typedef struct {
  /// Flag to indicate this process is active
  boolean_t RA_active;
  /// Size of DCI for RA-Response (bytes)
  uint8_t RA_dci_size_bytes1;
  /// Size of DCI for RA-Response (bits)
  uint8_t RA_dci_size_bits1;
  /// Actual DCI to transmit for RA-Response
  uint8_t RA_alloc_pdu1[(MAX_DCI_SIZE_BITS>>3)+1];
  /// DCI format for RA-Response (should be 1A)
  uint8_t RA_dci_fmt1;
  /// Size of DCI for Msg4/ContRes (bytes)
  uint8_t RA_dci_size_bytes2;
  /// Size of DCI for Msg4/ContRes (bits)
  uint8_t RA_dci_size_bits2;
  /// Actual DCI to transmit for Msg4/ContRes
  uint8_t RA_alloc_pdu2[(MAX_DCI_SIZE_BITS>>3)+1];
  /// DCI format for Msg4/ContRes (should be 1A)
  uint8_t RA_dci_fmt2;
  /// Flag to indicate the eNB should generate RAR.  This is triggered by detection of PRACH
  uint8_t generate_rar;
  /// Subframe where preamble was received
  uint8_t preamble_subframe;
  /// Subframe where Msg2 is to be sent
  uint8_t Msg2_subframe;
  /// Frame where Msg2 is to be sent
  frame_t Msg2_frame;
  /// Subframe where Msg3 is to be sent
  sub_frame_t Msg3_subframe;
  /// Frame where Msg3 is to be sent
  frame_t Msg3_frame;
  /// Subframe where Msg4 is to be sent
  sub_frame_t Msg4_subframe;
  /// Frame where Msg4 is to be sent
  frame_t Msg4_frame;
  /// Flag to indicate the eNB should generate Msg4 upon reception of SDU from RRC.  This is triggered by first ULSCH reception at eNB for new user.
  uint8_t generate_Msg4;
  /// Flag to indicate that eNB is waiting for ACK that UE has received Msg3.
  uint8_t wait_ack_Msg4;
  /// harq_pid used for Msg4 transmission
  uint8_t harq_pid;
  /// UE RNTI allocated during RAR
  rnti_t rnti;
  /// RA RNTI allocated from received PRACH
  uint16_t RA_rnti;
  /// Received preamble_index
  uint8_t preamble_index;
  /// Received UE Contention Resolution Identifier
  uint8_t cont_res_id[6];
  /// Timing offset indicated by PHY
  int16_t timing_offset;
  /// Timeout for RRC connection
  int16_t RRC_timer;
  /// Msg3 first RB
  uint8_t msg3_first_rb;
  /// Msg3 number of RB
  uint8_t msg3_nb_rb;
  /// Msg3 MCS
  uint8_t msg3_mcs;
  /// Msg3 TPC command
  uint8_t msg3_TPC;
  /// Msg3 ULdelay command
  uint8_t msg3_ULdelay;
  /// Msg3 cqireq command
  uint8_t msg3_cqireq;
  /// Round of Msg3 HARQ
  uint8_t msg3_round;
  /// TBS used for Msg4
  int msg4_TBsize;
  /// MCS used for Msg4
  int msg4_mcs;
  /// size off piggybacked RRC SDU
  uint8_t msg4_rrc_sdu_length;
  uint32_t msg4_delay;

#if (LTE_RRC_VERSION >= MAKE_VERSION(14, 0, 0))
  uint8_t rach_resource_type;
  uint8_t msg2_mpdcch_repetition_cnt;
  uint8_t msg2_mpdcch_done;
  uint8_t msg4_mpdcch_repetition_cnt;
  uint8_t msg4_mpdcch_done;
  uint8_t msg2_narrowband;
  uint32_t msg34_narrowband;
#endif
} RA_TEMPLATE;


/*! \brief subband bitmap confguration (for ALU icic algo purpose), in test phase */
typedef struct {
  uint8_t sbmap[NUMBER_OF_SUBBANDS_MAX]; //13 = number of SB MAX for 100 PRB
  uint8_t periodicity;
  uint8_t first_subframe;
  uint8_t sb_size;
  uint8_t nb_active_sb;
} SBMAP_CONF;
/*! \brief UE list used by eNB to order UEs/CC for scheduling*/
typedef struct {
  /// Dedicated information for UEs
  struct PhysicalConfigDedicated  *physicalConfigDedicated[MAX_NUM_CCs][NUMBER_OF_UE_MAX];
  /// DLSCH pdu
  DLSCH_PDU DLSCH_pdu[MAX_NUM_CCs][2][NUMBER_OF_UE_MAX];
  /// DCI template and MAC connection parameters for UEs
  UE_TEMPLATE UE_template[MAX_NUM_CCs][NUMBER_OF_UE_MAX];
  /// DCI template and MAC connection for RA processes
  int pCC_id[NUMBER_OF_UE_MAX];
  /// sorted downlink component carrier for the scheduler
  int ordered_CCids[MAX_NUM_CCs][NUMBER_OF_UE_MAX];
  /// number of downlink active component carrier
  int numactiveCCs[NUMBER_OF_UE_MAX];
  /// sorted uplink component carrier for the scheduler
  int ordered_ULCCids[MAX_NUM_CCs][NUMBER_OF_UE_MAX];
  /// number of uplink active component carrier
  int numactiveULCCs[NUMBER_OF_UE_MAX];
  /// number of downlink active component carrier
  uint8_t dl_CC_bitmap[NUMBER_OF_UE_MAX];
  /// eNB to UE statistics
  eNB_UE_STATS eNB_UE_stats[MAX_NUM_CCs][NUMBER_OF_UE_MAX];
  /// scheduling control info
  UE_sched_ctrl UE_sched_ctrl[NUMBER_OF_UE_MAX];
  int next[NUMBER_OF_UE_MAX];
  int head;
  int next_ul[NUMBER_OF_UE_MAX];
  int head_ul;
  int avail;
  int num_UEs;
  boolean_t active[NUMBER_OF_UE_MAX];
} UE_list_t;

/*! \brief eNB common channels */
typedef struct {
  int                              physCellId;
  int                              p_eNB;
  int                              Ncp;
  int                              eutra_band;
  uint32_t                         dl_CarrierFreq;
  BCCH_BCH_Message_t               *mib;
  RadioResourceConfigCommonSIB_t   *radioResourceConfigCommon;
#if (LTE_RRC_VERSION >= MAKE_VERSION(14, 0, 0))
  RadioResourceConfigCommonSIB_t   *radioResourceConfigCommon_BR;
#endif
  TDD_Config_t                     *tdd_Config;
  SchedulingInfoList_t             *schedulingInfoList;
  ARFCN_ValueEUTRA_t               ul_CarrierFreq;
  long                             ul_Bandwidth;
  /// Outgoing MIB PDU for PHY
  MIB_PDU MIB_pdu;
  /// Outgoing BCCH pdu for PHY
  BCCH_PDU BCCH_pdu;
  /// Outgoing BCCH DCI allocation
  uint32_t BCCH_alloc_pdu;
  /// Outgoing CCCH pdu for PHY
  CCCH_PDU CCCH_pdu;
  /// Outgoing RAR pdu for PHY
  RAR_PDU RAR_pdu;
  /// Template for RA computations
  RA_TEMPLATE RA_template[NB_RA_PROC_MAX];
  /// VRB map for common channels
  uint8_t vrb_map[100];
  /// VRB map for common channels and retransmissions by PHICH
  uint8_t vrb_map_UL[100];
  /// MBSFN SubframeConfig
  struct MBSFN_SubframeConfig *mbsfn_SubframeConfig[8];
  /// number of subframe allocation pattern available for MBSFN sync area
  uint8_t num_sf_allocation_pattern;
#if (LTE_RRC_VERSION >= MAKE_VERSION(10, 0, 0))
  /// MBMS Flag
  uint8_t MBMS_flag;
  /// Outgoing MCCH pdu for PHY
  MCCH_PDU MCCH_pdu;
  /// MCCH active flag
  uint8_t msi_active;
  /// MCCH active flag
  uint8_t mcch_active;
  /// MTCH active flag
  uint8_t mtch_active;
  /// number of active MBSFN area
  uint8_t num_active_mbsfn_area;
  /// MBSFN Area Info
  struct  MBSFN_AreaInfo_r9 *mbsfn_AreaInfo[MAX_MBSFN_AREA];
  /// PMCH Config
  struct PMCH_Config_r9 *pmch_Config[MAX_PMCH_perMBSFN];
  /// MBMS session info list
  struct MBMS_SessionInfoList_r9 *mbms_SessionList[MAX_PMCH_perMBSFN];
  /// Outgoing MCH pdu for PHY
  MCH_PDU MCH_pdu;
#endif
#if (LTE_RRC_VERSION >= MAKE_VERSION(14, 0, 0))
  /// Rel13 parameters from SIB1
  SystemInformationBlockType1_v1310_IEs_t *sib1_v13ext;
  /// Counter for SIB1-BR scheduling
  int SIB1_BR_cnt;
  /// Outgoing BCCH-BR pdu for PHY
  BCCH_PDU BCCH_BR_pdu[20];
#endif
} COMMON_channels_t;
/*! \brief top level eNB MAC structure */
typedef struct eNB_MAC_INST_s {
  /// Ethernet parameters for northbound midhaul interface
  eth_params_t         eth_params_n;
  /// Ethernet parameters for fronthaul interface
  eth_params_t         eth_params_s;
  ///
  module_id_t Mod_id;
  /// frame counter
  frame_t frame;
  /// subframe counter
  sub_frame_t subframe;
  /// Pointer to IF module instance for PHY
  IF_Module_t *if_inst;
  /// Common cell resources
  COMMON_channels_t common_channels[MAX_NUM_CCs];
  /// current PDU index (BCH,MCH,DLSCH)
  uint16_t pdu_index[MAX_NUM_CCs];

  /// NFAPI Config Request Structure
  nfapi_config_request_t config[MAX_NUM_CCs];
  /// Preallocated DL pdu list
  nfapi_dl_config_request_pdu_t dl_config_pdu_list[MAX_NUM_CCs][MAX_NUM_DL_PDU];
  /// NFAPI DL Config Request Structure
  nfapi_dl_config_request_t DL_req[MAX_NUM_CCs];
  /// Preallocated UL pdu list
  nfapi_ul_config_request_pdu_t ul_config_pdu_list[MAX_NUM_CCs][MAX_NUM_UL_PDU];
  /// Preallocated UL pdu list for ULSCH (n+k delay)
  nfapi_ul_config_request_pdu_t ul_config_pdu_list_tmp[MAX_NUM_CCs][10][MAX_NUM_UL_PDU];
  /// NFAPI UL Config Request Structure, send to L1 4 subframes before processing takes place
  nfapi_ul_config_request_t UL_req[MAX_NUM_CCs];
  /// NFAPI "Temporary" UL Config Request Structure, holds future UL_config requests
  nfapi_ul_config_request_t UL_req_tmp[MAX_NUM_CCs][10];
  /// Preallocated HI_DCI0 pdu list
  nfapi_hi_dci0_request_pdu_t hi_dci0_pdu_list[MAX_NUM_CCs][MAX_NUM_HI_DCI0_PDU];
  /// NFAPI HI/DCI0 Config Request Structure
  nfapi_hi_dci0_request_t HI_DCI0_req[MAX_NUM_CCs];
  /// Prealocated TX pdu list
  nfapi_tx_request_pdu_t tx_request_pdu[MAX_NUM_CCs][MAX_NUM_TX_REQUEST_PDU];
  /// NFAPI DL PDU structure
  nfapi_tx_request_t TX_req[MAX_NUM_CCs];
  /// UL handle
  uint32_t ul_handle;
  UE_list_t UE_list;

  ///subband bitmap configuration
  SBMAP_CONF sbmap_conf;
  /// CCE table used to build DCI scheduling information
  int CCE_table[MAX_NUM_CCs][800];
  ///  active flag for Other lcid
  uint8_t lcid_active[NB_RB_MAX];
  /// eNB stats
  eNB_STATS eNB_stats[MAX_NUM_CCs];
  // MAC function execution peformance profiler
  /// processing time of eNB scheduler
  time_stats_t eNB_scheduler;
  /// processing time of eNB scheduler for SI
  time_stats_t schedule_si;
  /// processing time of eNB scheduler for Random access
  time_stats_t schedule_ra;
  /// processing time of eNB ULSCH scheduler
  time_stats_t schedule_ulsch;
  /// processing time of eNB DCI generation
  time_stats_t fill_DLSCH_dci;
  /// processing time of eNB MAC preprocessor
  time_stats_t schedule_dlsch_preprocessor;
  /// processing time of eNB DLSCH scheduler
  time_stats_t schedule_dlsch; // include rlc_data_req + MAC header + preprocessor
  /// processing time of eNB MCH scheduler
  time_stats_t schedule_mch;
  /// processing time of eNB ULSCH reception
  time_stats_t rx_ulsch_sdu; // include rlc_data_ind
} eNB_MAC_INST;

/*
 * UE part
 */

typedef enum {
  TYPE0,
  TYPE1,
  TYPE1A,
  TYPE2,
  TYPE2A,
  TYPEUESPEC
} MPDCCH_TYPES_t;

/*!\brief UE layer 2 status */
typedef enum {
  CONNECTION_OK=0,
  CONNECTION_LOST,
  PHY_RESYNCH,
  PHY_HO_PRACH
} UE_L2_STATE_t;

/*!\brief UE scheduling info */
typedef struct {
  /// buffer status for each lcgid
  uint8_t  BSR[MAX_NUM_LCGID]; // should be more for mesh topology
  /// keep the number of bytes in rlc buffer for each lcgid
  int32_t  BSR_bytes[MAX_NUM_LCGID];
  /// after multiplexing buffer remain for each lcid
  int32_t  LCID_buffer_remain[MAX_NUM_LCID];
  /// sum of all lcid buffer size
  uint16_t  All_lcid_buffer_size_lastTTI;
  /// buffer status for each lcid
  uint8_t  LCID_status[MAX_NUM_LCID];
  /// SR pending as defined in 36.321
  uint8_t  SR_pending;
  /// SR_COUNTER as defined in 36.321
  uint16_t SR_COUNTER;
  /// logical channel group ide for each LCID
  uint8_t  LCGID[MAX_NUM_LCID];
  /// retxBSR-Timer, default value is sf2560
  uint16_t retxBSR_Timer;
  /// retxBSR_SF, number of subframe before triggering a regular BSR
  uint16_t retxBSR_SF;
  /// periodicBSR-Timer, default to infinity
  uint16_t periodicBSR_Timer;
  /// periodicBSR_SF, number of subframe before triggering a periodic BSR
  uint16_t periodicBSR_SF;
  /// default value is 0: not configured
  uint16_t sr_ProhibitTimer;
  /// sr ProhibitTime running
  uint8_t sr_ProhibitTimer_Running;
  ///  default value to n5
  uint16_t maxHARQ_Tx;
  /// default value is false
  uint16_t ttiBundling;
  /// default value is release
  struct DRX_Config *drx_config;
  /// default value is release
  struct MAC_MainConfig__phr_Config *phr_config;
  ///timer before triggering a periodic PHR
  uint16_t periodicPHR_Timer;
  ///timer before triggering a prohibit PHR
  uint16_t prohibitPHR_Timer;
  ///DL Pathloss change value
  uint16_t PathlossChange;
  ///number of subframe before triggering a periodic PHR
  int16_t periodicPHR_SF;
  ///number of subframe before triggering a prohibit PHR
  int16_t prohibitPHR_SF;
  ///DL Pathloss Change in db
  uint16_t PathlossChange_db;

  /// default value is false
  uint16_t extendedBSR_Sizes_r10;
  /// default value is false
  uint16_t extendedPHR_r10;

  //Bj bucket usage per  lcid
  int16_t Bj[MAX_NUM_LCID];
  // Bucket size per lcid
  int16_t bucket_size[MAX_NUM_LCID];
} UE_SCHEDULING_INFO;
/*!\brief Top level UE MAC structure */
typedef struct {
  uint16_t Node_id;
  /// RX frame counter
  frame_t     rxFrame;
  /// RX subframe counter
  sub_frame_t rxSubframe;
  /// TX frame counter
  frame_t     txFrame;
  /// TX subframe counter
  sub_frame_t txSubframe;
  /// C-RNTI of UE
  uint16_t crnti;
  /// C-RNTI of UE before HO
  rnti_t crnti_before_ho; ///user id (rnti) of connected UEs
  /// uplink active flag
  uint8_t ul_active;
  /// pointer to RRC PHY configuration
  RadioResourceConfigCommonSIB_t *radioResourceConfigCommon;
  /// pointer to RACH_ConfigDedicated (NULL when not active, i.e. upon HO completion or T304 expiry)
  struct RACH_ConfigDedicated *rach_ConfigDedicated;
  /// pointer to RRC PHY configuration
  struct PhysicalConfigDedicated *physicalConfigDedicated;
#if (LTE_RRC_VERSION >= MAKE_VERSION(10, 0, 0))
  /// pointer to RRC PHY configuration SCEll
  struct PhysicalConfigDedicatedSCell_r10 *physicalConfigDedicatedSCell_r10;
#endif
  /// pointer to TDD Configuration (NULL for FDD)
  TDD_Config_t *tdd_Config;
  /// Number of adjacent cells to measure
  uint8_t  n_adj_cells;
  /// Array of adjacent physical cell ids
  uint32_t adj_cell_id[6];
  /// Pointer to RRC MAC configuration
  MAC_MainConfig_t *macConfig;
  /// Pointer to RRC Measurement gap configuration
  MeasGapConfig_t  *measGapConfig;
  /// Pointers to LogicalChannelConfig indexed by LogicalChannelIdentity. Note NULL means LCHAN is inactive.
  LogicalChannelConfig_t *logicalChannelConfig[MAX_NUM_LCID];
  /// Scheduling Information
  UE_SCHEDULING_INFO scheduling_info;
  /// Outgoing CCCH pdu for PHY
  CCCH_PDU CCCH_pdu;
  /// Outgoing RAR pdu for PHY
  RAR_PDU RAR_pdu;
  /// Incoming DLSCH pdu for PHY
  DLSCH_PDU DLSCH_pdu[NUMBER_OF_UE_MAX][2];
  /// number of attempt for rach
  uint8_t RA_attempt_number;
  /// Random-access procedure flag
  uint8_t RA_active;
  /// Random-access window counter
  int8_t RA_window_cnt;
  /// Random-access Msg3 size in bytes
  uint8_t RA_Msg3_size;
  /// Random-access prachMaskIndex
  uint8_t RA_prachMaskIndex;
  /// Flag indicating Preamble set (A,B) used for first Msg3 transmission
  uint8_t RA_usedGroupA;
  /// Random-access Resources
  PRACH_RESOURCES_t RA_prach_resources;
  /// Random-access PREAMBLE_TRANSMISSION_COUNTER
  uint8_t RA_PREAMBLE_TRANSMISSION_COUNTER;
  /// Random-access backoff counter
  int16_t RA_backoff_cnt;
  /// Random-access variable for window calculation (frame of last change in window counter)
  uint32_t RA_tx_frame;
  /// Random-access variable for window calculation (subframe of last change in window counter)
  uint8_t RA_tx_subframe;
  /// Random-access Group B maximum path-loss
  /// Random-access variable for backoff (frame of last change in backoff counter)
  uint32_t RA_backoff_frame;
  /// Random-access variable for backoff (subframe of last change in backoff counter)
  uint8_t RA_backoff_subframe;
  /// Random-access Group B maximum path-loss
  uint16_t RA_maxPL;
  /// Random-access Contention Resolution Timer active flag
  uint8_t RA_contention_resolution_timer_active;
  /// Random-access Contention Resolution Timer count value
  uint8_t RA_contention_resolution_cnt;
  /// power headroom reporitng reconfigured
  uint8_t PHR_reconfigured;
  /// power headroom state as configured by the higher layers
  uint8_t PHR_state;
  /// power backoff due to power management (as allowed by P-MPRc) for this cell
  uint8_t PHR_reporting_active;
  /// power backoff due to power management (as allowed by P-MPRc) for this cell
  uint8_t power_backoff_db[NUMBER_OF_eNB_MAX];
  /// BSR report falg management
  uint8_t BSR_reporting_active;
  /// retxBSR-Timer expires flag
  uint8_t retxBSRTimer_expires_flag;
  /// periodBSR-Timer expires flag
  uint8_t periodBSRTimer_expires_flag;

  /// MBSFN_Subframe Configuration
  struct MBSFN_SubframeConfig *mbsfn_SubframeConfig[8]; // FIXME replace 8 by MAX_MBSFN_AREA?
  /// number of subframe allocation pattern available for MBSFN sync area
  uint8_t num_sf_allocation_pattern;
#if (LTE_RRC_VERSION >= MAKE_VERSION(10, 0, 0))
  /// number of active MBSFN area
  uint8_t num_active_mbsfn_area;
  /// MBSFN Area Info
  struct  MBSFN_AreaInfo_r9 *mbsfn_AreaInfo[MAX_MBSFN_AREA];
  /// PMCH Config
  struct PMCH_Config_r9 *pmch_Config[MAX_PMCH_perMBSFN];
  /// MCCH status
  uint8_t mcch_status;
  /// MSI status
  uint8_t msi_status;// could be an array if there are >1 MCH in one MBSFN area
#endif
  //#ifdef CBA
  /// CBA RNTI for each group
  uint16_t cba_rnti[NUM_MAX_CBA_GROUP];
  /// last SFN for CBA channel access
  uint8_t cba_last_access[NUM_MAX_CBA_GROUP];
  //#endif
  /// total UE scheduler processing time
  time_stats_t ue_scheduler; // total
  /// UE ULSCH tx  processing time inlcuding RLC interface (rlc_data_req) and mac header generation
  time_stats_t tx_ulsch_sdu;
  /// UE DLSCH rx  processing time inlcuding RLC interface (mac_rrc_data_ind or mac_rlc_status_ind+mac_rlc_data_ind) and mac header parser
  time_stats_t rx_dlsch_sdu ;
  /// UE query for MCH subframe processing time
  time_stats_t ue_query_mch;
  /// UE MCH rx processing time
  time_stats_t rx_mch_sdu;
  /// UE BCCH rx processing time including RLC interface (mac_rrc_data_ind)
  time_stats_t rx_si;
  /// UE PCCH rx processing time including RLC interface (mac_rrc_data_ind)
  time_stats_t rx_p;
} UE_MAC_INST;
/*! \brief ID of the neighboring cells used for HO*/
typedef struct {
  uint16_t cell_ids[6];
  uint8_t n_adj_cells;
} neigh_cell_id_t;

#include "proto.h"
/*@}*/
#endif /*__LAYER2_MAC_DEFS_H__ */
