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

/*! \file LAYER2/PDCP_v10.1.0/pdcp.h
* \brief pdcp interface with RLC, RRC
* \author  Lionel GAUTHIER and Navid Nikaein
* \date 2009-2012
* \version 1.0
*/

/** @defgroup _pdcp PDCP
* @ingroup _oai2
* @{
*/

#ifndef __PDCP_H__
#    define __PDCP_H__
//-----------------------------------------------------------------------------
#ifndef NON_ACCESS_STRATUM
  #include "UTIL/MEM/mem_block.h"
  #include "UTIL/LISTS/list.h"
#endif //NON_ACCESS_STRATUM
//-----------------------------------------------------------------------------
#include "RRC/LTE/rrc_defs.h"
#include "COMMON/platform_constants.h"
#include "COMMON/platform_types.h"
#include "LTE_DRB-ToAddMod.h"
#include "LTE_DRB-ToAddModList.h"
#include "LTE_SRB-ToAddMod.h"
#include "LTE_SRB-ToAddModList.h"
#if (LTE_RRC_VERSION >= MAKE_VERSION(9, 0, 0))
  #include "LTE_MBMS-SessionInfoList-r9.h"
  #include "LTE_PMCH-InfoList-r9.h"
#endif

typedef rlc_op_status_t  (*send_rlc_data_req_func_t)(const protocol_ctxt_t *const,
    const srb_flag_t, const MBMS_flag_t,
    const rb_id_t, const mui_t,
    confirm_t, sdu_size_t, mem_block_t *,const uint32_t *const, const uint32_t *const);
typedef boolean_t (*pdcp_data_ind_func_t)( const protocol_ctxt_t *, const srb_flag_t,
    const MBMS_flag_t, const rb_id_t, const sdu_size_t,
    mem_block_t *,const uint32_t *const, const uint32_t *const);
/* maximum number of tun interfaces that will be created to emulates UEs */
/* UEs beyond that will be multiplexed on the same tun   */
#define MAX_NUMBER_NETIF           16

#define PDCP_USE_NETLINK_BIT            (1<< 11)
#define LINK_ENB_PDCP_TO_IP_DRIVER_BIT  (1<< 13)
#define LINK_ENB_PDCP_TO_GTPV1U_BIT     (1<< 14)
#define UE_NAS_USE_TUN_BIT              (1<< 15)
#define ENB_NAS_USE_TUN_BIT             (1<< 16)
typedef struct {
  uint64_t optmask;
  send_rlc_data_req_func_t send_rlc_data_req_func;
  pdcp_data_ind_func_t pdcp_data_ind_func;
} pdcp_params_t;



#define PDCP_USE_NETLINK            ( get_pdcp_optmask() & PDCP_USE_NETLINK_BIT)
#define LINK_ENB_PDCP_TO_IP_DRIVER  ( get_pdcp_optmask() & LINK_ENB_PDCP_TO_IP_DRIVER_BIT)
#define LINK_ENB_PDCP_TO_GTPV1U     ( get_pdcp_optmask() & LINK_ENB_PDCP_TO_GTPV1U_BIT)
#define UE_NAS_USE_TUN              ( get_pdcp_optmask() & UE_NAS_USE_TUN_BIT)
#define ENB_NAS_USE_TUN             ( get_pdcp_optmask() & ENB_NAS_USE_TUN_BIT)
uint64_t get_pdcp_optmask(void);

extern pthread_t       pdcp_thread;
extern pthread_attr_t  pdcp_thread_attr;
extern pthread_mutex_t pdcp_mutex;
extern pthread_cond_t  pdcp_cond;
extern int             pdcp_instance_cnt;

#define PROTOCOL_PDCP_CTXT_FMT PROTOCOL_CTXT_FMT"[%s %02u] "

#define PROTOCOL_PDCP_CTXT_ARGS(CTXT_Pp, pDCP_Pp) PROTOCOL_CTXT_ARGS(CTXT_Pp),\
  (pDCP_Pp->is_srb) ? "SRB" : "DRB",\
  pDCP_Pp->rb_id
int init_pdcp_thread(void);
void cleanup_pdcp_thread(void);

uint32_t Pdcp_stats_tx_window_ms[MAX_eNB][MAX_MOBILES_PER_ENB];
uint32_t Pdcp_stats_tx_bytes[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_tx_bytes_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_tx_bytes_tmp_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_tx[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_tx_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_tx_tmp_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_tx_sn[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_tx_throughput_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_tx_aiat[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_tx_aiat_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_tx_aiat_tmp_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_tx_iat[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];

uint32_t Pdcp_stats_rx_window_ms[MAX_eNB][MAX_MOBILES_PER_ENB];
uint32_t Pdcp_stats_rx[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_rx_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_rx_tmp_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_rx_bytes[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_rx_bytes_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_rx_bytes_tmp_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_rx_sn[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_rx_goodput_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_rx_aiat[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_rx_aiat_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_rx_aiat_tmp_w[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_rx_iat[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];
uint32_t Pdcp_stats_rx_outoforder[MAX_eNB][MAX_MOBILES_PER_ENB][NB_RB_MAX];

void pdcp_update_perioidical_stats(const protocol_ctxt_t *const  ctxt_pP);


/*Packet Probing for agent PDCP*/
//uint64_t *pdcp_packet_counter;
//uint64_t *pdcp_size_packet;
typedef struct pdcp_enb_s {
  // used for eNB stats generation
  uint16_t uid[MAX_MOBILES_PER_ENB];
  rnti_t rnti[MAX_MOBILES_PER_ENB];
  uint16_t num_ues;

  uint64_t sfn;
  frame_t  frame;
  sub_frame_t subframe;

} pdcp_enb_t;

pdcp_enb_t pdcp_enb[MAX_NUM_CCs];

typedef struct pdcp_stats_s {
  time_stats_t pdcp_run;
  time_stats_t data_req;
  time_stats_t data_ind;
  time_stats_t apply_security; //
  time_stats_t validate_security;
  time_stats_t pdcp_ip;
  time_stats_t ip_pdcp; // separte thread
} pdcp_stats_t; // common to eNB and UE


typedef struct pdcp_s {
  //boolean_t     instanciated_instance;
  uint16_t       header_compression_profile;

  /* SR: added this flag to distinguish UE/eNB instance as pdcp_run for virtual
   * mode can receive data on NETLINK for eNB while eNB_flag = 0 and for UE when eNB_flag = 1
   */
  boolean_t is_ue;
  boolean_t is_srb;

  /* Configured security algorithms */
  uint8_t cipheringAlgorithm;
  uint8_t integrityProtAlgorithm;

  /* User-Plane encryption key
   * Control-Plane RRC encryption key
   * Control-Plane RRC integrity key
   * These keys are configured by RRC layer
   */
  uint8_t *kUPenc;
  uint8_t *kRRCint;
  uint8_t *kRRCenc;

  uint8_t security_activated;

  rlc_mode_t rlc_mode;
  uint8_t status_report;
  uint8_t seq_num_size;

  logical_chan_id_t lcid;
  rb_id_t           rb_id;
  /*
   * Sequence number state variables
   *
   * TX and RX window
   */
  pdcp_sn_t next_pdcp_tx_sn;
  pdcp_sn_t next_pdcp_rx_sn;
  pdcp_sn_t maximum_pdcp_rx_sn;
  /*
   * TX and RX Hyper Frame Numbers
   */
  pdcp_hfn_t tx_hfn;
  pdcp_hfn_t rx_hfn;

  /*
   * SN of the last PDCP SDU delivered to upper layers
   */
  pdcp_sn_t  last_submitted_pdcp_rx_sn;

  /*
   * Following array is used as a bitmap holding missing sequence
   * numbers to generate a PDCP Control PDU for PDCP status
   * report (see 6.2.6)
   */
  uint8_t missing_pdu_bitmap[512];
  /*
   * This is intentionally signed since we need a 'NULL' value
   * which is not also a valid sequence number
   */
  short int first_missing_pdu;

} pdcp_t;

#if (LTE_RRC_VERSION >= MAKE_VERSION(10, 0, 0))
typedef struct pdcp_mbms_s {
  boolean_t instanciated_instance;
  rb_id_t   rb_id;
} pdcp_mbms_t;
#endif
/*
 * Following symbolic constant alters the behaviour of PDCP
 * and makes it linked to PDCP test code under targets/TEST/PDCP/
 *
 * For the version at SVN repository this should be UNDEFINED!
 * XXX And later this should be configured through the Makefile
 * under targets/TEST/PDCP/
 */

/*! \fn boolean_t pdcp_data_req(const protocol_ctxt_t* const  , srb_flag_t , rb_id_t , mui_t , confirm_t ,sdu_size_t , unsigned char* , pdcp_transmission_mode_t )
* \brief This functions handles data transfer requests coming either from RRC or from IP
* \param[in] ctxt_pP        Running context.
* \param[in] rab_id         Radio Bearer ID
* \param[in] muiP
* \param[in] confirmP
* \param[in] sdu_buffer_size Size of incoming SDU in bytes
* \param[in] sdu_buffer      Buffer carrying SDU
* \param[in] mode            flag to indicate whether the userplane data belong to the control plane or data plane or transparent
* \return TRUE on success, FALSE otherwise
* \note None
* @ingroup _pdcp
*/
boolean_t pdcp_data_req(
  protocol_ctxt_t  *ctxt_pP,
  const srb_flag_t srb_flagP,
  const rb_id_t rb_id,
  const mui_t muiP,
  const confirm_t confirmP, \
  const sdu_size_t sdu_buffer_size,
  unsigned char *const sdu_buffer,
  const pdcp_transmission_mode_t mode
#if (LTE_RRC_VERSION >= MAKE_VERSION(14, 0, 0))
  ,const uint32_t *const sourceL2Id
  ,const uint32_t *const destinationL2Id
#endif
);

/*! \fn boolean_t pdcp_data_ind(const protocol_ctxt_t* const, srb_flag_t, MBMS_flag_t, rb_id_t, sdu_size_t, mem_block_t*, boolean_t)
* \brief This functions handles data transfer indications coming from RLC
* \param[in] ctxt_pP        Running context.
* \param[in] Shows if rb is SRB
* \param[in] Tells if MBMS traffic
* \param[in] rab_id Radio Bearer ID
* \param[in] sdu_buffer_size Size of incoming SDU in bytes
* \param[in] sdu_buffer Buffer carrying SDU
* \param[in] is_data_plane flag to indicate whether the userplane data belong to the control plane or data plane
* \return TRUE on success, FALSE otherwise
* \note None
* @ingroup _pdcp
*/
boolean_t pdcp_data_ind(
  const protocol_ctxt_t *const  ctxt_pP,
  const srb_flag_t srb_flagP,
  const MBMS_flag_t MBMS_flagP,
  const rb_id_t rb_id,
  const sdu_size_t sdu_buffer_size,
  mem_block_t *const sdu_buffer);

/*! \fn void rrc_pdcp_config_req(const protocol_ctxt_t* const ,uint32_t,rb_id_t,uint8_t)
* \brief This functions initializes relevant PDCP entity
* \param[in] ctxt_pP        Running context.
* \param[in] actionP flag for action: add, remove , modify
* \param[in] rb_idP Radio Bearer ID of relevant PDCP entity
* \param[in] security_modeP Radio Bearer ID of relevant PDCP entity
* \return none
* \note None
* @ingroup _pdcp
*/
void rrc_pdcp_config_req (
  const protocol_ctxt_t *const  ctxt_pP,
  const srb_flag_t  srb_flagP,
  const uint32_t    actionP,
  const rb_id_t     rb_idP,
  const uint8_t     security_modeP);

/*! \fn bool rrc_pdcp_config_asn1_req (const protocol_ctxt_t* const , SRB_ToAddModList_t* srb2add_list, DRB_ToAddModList_t* drb2add_list, DRB_ToReleaseList_t*  drb2release_list)
* \brief  Function for RRC to configure a Radio Bearer.
* \param[in]  ctxt_pP           Running context.
* \param[in]  index             index of UE or eNB depending on the eNB_flag
* \param[in]  srb2add_list      SRB configuration list to be created.
* \param[in]  drb2add_list      DRB configuration list to be created.
* \param[in]  drb2release_list  DRB configuration list to be released.
* \param[in]  security_mode     Security algorithm to apply for integrity/ciphering
* \param[in]  kRRCenc           RRC encryption key
* \param[in]  kRRCint           RRC integrity key
* \param[in]  kUPenc            User-Plane encryption key
* \param[in]  defaultDRB        Default DRB ID
* \return     A status about the processing, OK or error code.
*/
boolean_t rrc_pdcp_config_asn1_req (
  const protocol_ctxt_t *const  ctxt_pP,
  LTE_SRB_ToAddModList_t  *const srb2add_list,
  LTE_DRB_ToAddModList_t  *const drb2add_list,
  LTE_DRB_ToReleaseList_t *const drb2release_list,
  const uint8_t                   security_modeP,
  uint8_t                  *const kRRCenc,
  uint8_t                  *const kRRCint,
  uint8_t                  *const kUPenc
#if (LTE_RRC_VERSION >= MAKE_VERSION(9, 0, 0))
  ,LTE_PMCH_InfoList_r9_t  *pmch_InfoList_r9
#endif
  ,rb_id_t                 *const defaultDRB
);

/*! \fn boolean_t pdcp_config_req_asn1 (const protocol_ctxt_t* const ctxt_pP, srb_flag_t srb_flagP, uint32_t  action, rb_id_t rb_id, uint8_t rb_sn, uint8_t rb_report, uint16_t header_compression_profile, uint8_t security_mode)
* \brief  Function for RRC to configure a Radio Bearer.
* \param[in]  ctxt_pP           Running context.
* \param[in]  pdcp_pP            Pointer on PDCP structure.
* \param[in]  enb_mod_idP        Virtualized enb module identifier, Not used if eNB_flagP = 0.
* \param[in]  ue_mod_idP         Virtualized ue module identifier.
* \param[in]  frame              Frame index.
* \param[in]  eNB_flag           Flag to indicate eNB (1) or UE (0)
* \param[in]  srb_flagP          Flag to indicate SRB (1) or DRB (0)
* \param[in]  action             add, remove, modify a RB
* \param[in]  rb_id              radio bearer id
* \param[in]  rb_sn              sequence number for this radio bearer
* \param[in]  drb_report         set a pdcp report for this drb
* \param[in]  header_compression set the rohc profile
* \param[in]  security_mode      set the integrity and ciphering algs
* \param[in]  kRRCenc            RRC encryption key
* \param[in]  kRRCint            RRC integrity key
* \param[in]  kUPenc             User-Plane encryption key
* \return     A status about the processing, OK or error code.
*/
boolean_t pdcp_config_req_asn1 (
  const protocol_ctxt_t *const  ctxt_pP,
  pdcp_t         *const pdcp_pP,
  const srb_flag_t       srb_flagP,
  const rlc_mode_t       rlc_mode,
  const uint32_t         action,
  const uint16_t         lc_id,
  const uint16_t         mch_id,
  const rb_id_t          rb_id,
  const uint8_t          rb_sn,
  const uint8_t          rb_report,
  const uint16_t         header_compression_profile,
  const uint8_t          security_mode,
  uint8_t         *const kRRCenc,
  uint8_t         *const kRRCint,
  uint8_t         *const kUPenc);

/*! \fn void pdcp_add_UE(const protocol_ctxt_t* const  ctxt_pP)
* \brief  Function (for RRC) to add a new UE in PDCP module
* \param[in]  ctxt_pP           Running context.
* \return     A status about the processing, OK or error code.
*/
void pdcp_add_UE(const protocol_ctxt_t *const  ctxt_pP);

/*! \fn boolean_t pdcp_remove_UE(const protocol_ctxt_t* const  ctxt_pP)
* \brief  Function for RRC to remove UE from PDCP module hashtable
* \param[in]  ctxt_pP           Running context.
* \return     A status about the processing, OK or error code.
*/
boolean_t pdcp_remove_UE(
  const protocol_ctxt_t *const  ctxt_pP);

/*! \fn void rrc_pdcp_config_release( const protocol_ctxt_t* const, rb_id_t)
* \brief This functions is unused
* \param[in]  ctxt_pP           Running context.
* \param[in] rab_id Radio Bearer ID of relevant PDCP entity
* \return none
* \note None
* @ingroup _pdcp
*/
//void rrc_pdcp_config_release ( const protocol_ctxt_t* const  ctxt_pP, rb_id_t);

/*! \fn void pdcp_run(const protocol_ctxt_t* const  ctxt_pP)
* \brief Runs PDCP entity to let it handle incoming/outgoing SDUs
* \param[in]  ctxt_pP           Running context.
* \return none
* \note None
* @ingroup _pdcp
*/
void pdcp_run            (
  const protocol_ctxt_t *const  ctxt_pP);
uint64_t pdcp_module_init     (uint64_t pdcp_optmask);
void pdcp_module_cleanup (void);
void pdcp_layer_init     (void);
void pdcp_layer_cleanup  (void);
#define PDCP2NW_DRIVER_FIFO 21
#define NW_DRIVER2PDCP_FIFO 22

int pdcp_fifo_flush_sdus                      ( const protocol_ctxt_t *const  ctxt_pP);
int pdcp_fifo_read_input_sdus_remaining_bytes ( const protocol_ctxt_t *const  ctxt_pP);
int pdcp_fifo_read_input_sdus                 ( const protocol_ctxt_t *const  ctxt_pP);
void pdcp_fifo_read_input_sdus_from_otg       ( const protocol_ctxt_t *const  ctxt_pP);
void pdcp_set_rlc_data_req_func(send_rlc_data_req_func_t send_rlc_data_req);
void pdcp_set_pdcp_data_ind_func(pdcp_data_ind_func_t pdcp_data_ind);
pdcp_data_ind_func_t get_pdcp_data_ind_func(void);
//-----------------------------------------------------------------------------

/*
 * Following two types are utilized between NAS driver and PDCP
 */


typedef struct pdcp_data_req_header_s {
  rb_id_t             rb_id;
  sdu_size_t          data_size;
  signed int          inst;
  ip_traffic_type_t   traffic_type;
#if (LTE_RRC_VERSION >= MAKE_VERSION(14, 0, 0))
  uint32_t sourceL2Id;
  uint32_t destinationL2Id;
#endif
} pdcp_data_req_header_t;

typedef struct pdcp_data_ind_header_s {
  rb_id_t             rb_id;
  sdu_size_t          data_size;
  signed int          inst;
  ip_traffic_type_t   dummy_traffic_type;
#if (LTE_RRC_VERSION >= MAKE_VERSION(14, 0, 0))
  uint32_t sourceL2Id;
  uint32_t destinationL2Id;
#endif
} pdcp_data_ind_header_t;

struct pdcp_netlink_element_s {
  pdcp_data_req_header_t pdcp_read_header;

  /* Data part of the message */
  uint8_t *data;
};

//TTN for D2D (PC5S)
#if (LTE_RRC_VERSION >= MAKE_VERSION(14, 0, 0))
#define PDCP_SOCKET_PORT_NO 9999 //temporary value
#define PC5_SIGNALLING_PAYLOAD_SIZE   100  //should be updated with a correct value
int pdcp_pc5_sockfd;
struct sockaddr_in prose_ctrl_addr;
struct sockaddr_in prose_pdcp_addr;
struct sockaddr_in pdcp_sin;
void pdcp_pc5_socket_init(void);

typedef struct  {
  rb_id_t             rb_id;
  sdu_size_t          data_size;
  signed int          inst;
  ip_traffic_type_t   traffic_type;
  uint32_t sourceL2Id;
  uint32_t destinationL2Id;
} __attribute__((__packed__)) pc5s_header_t;

//new PC5S-message
typedef struct  {
  unsigned char bytes[PC5_SIGNALLING_PAYLOAD_SIZE];
}  __attribute__((__packed__)) PC5SignallingMessage ;

//example of PC5-S messages
typedef struct {
  pc5s_header_t pc5s_header;
  union {
    uint8_t status;
    PC5SignallingMessage pc5_signalling_message;
  } pc5sPrimitive;
} __attribute__((__packed__)) sidelink_pc5s_element;


#endif

/*
 * PDCP limit values
 */
#define PDCP_MAX_SDU_SIZE 8188 // octets, see 4.3.1 Services provided to upper layers
#define PDCP_MAX_SN_5BIT  31   // 2^5-1
#define PDCP_MAX_SN_7BIT  127  // 2^7-1
#define PDCP_MAX_SN_12BIT 4095 // 2^12-1

/*
 * Reordering_Window: half of the PDCP SN space
 */
#define REORDERING_WINDOW_SN_5BIT 16
#define REORDERING_WINDOW_SN_7BIT 64
#define REORDERING_WINDOW_SN_12BIT 2048

signed int             pdcp_2_nas_irq;
pdcp_stats_t              UE_pdcp_stats[MAX_MOBILES_PER_ENB];
pdcp_stats_t              eNB_pdcp_stats[NUMBER_OF_eNB_MAX];


// for UE code conly
rnti_t                 pdcp_UE_UE_module_id_to_rnti[MAX_MOBILES_PER_ENB];
rnti_t                 pdcp_eNB_UE_instance_to_rnti[MAX_MOBILES_PER_ENB]; // for noS1 mode
unsigned int           pdcp_eNB_UE_instance_to_rnti_index;
#if (LTE_RRC_VERSION >= MAKE_VERSION(10, 0, 0))
  pdcp_mbms_t               pdcp_mbms_array_ue[MAX_MOBILES_PER_ENB][LTE_maxServiceCount][LTE_maxSessionPerPMCH];   // some constants from openair2/RRC/LTE/MESSAGES/asn1_constants.h
  pdcp_mbms_t               pdcp_mbms_array_eNB[NUMBER_OF_eNB_MAX][LTE_maxServiceCount][LTE_maxSessionPerPMCH]; // some constants from openair2/RRC/LTE/MESSAGES/asn1_constants.h
#endif
sdu_size_t             pdcp_output_sdu_bytes_to_write;
sdu_size_t             pdcp_output_header_bytes_to_write;
list_t                 pdcp_sdu_list;
int                    pdcp_sent_a_sdu;
pdcp_data_req_header_t pdcp_input_header;
unsigned char          pdcp_input_sdu_buffer[MAX_IP_PACKET_SIZE];
sdu_size_t             pdcp_input_index_header;
sdu_size_t             pdcp_input_sdu_size_read;
sdu_size_t             pdcp_input_sdu_remaining_size_to_read;

#define PDCP_COLL_KEY_VALUE(mODULE_iD, rNTI, iS_eNB, rB_iD, iS_sRB) \
  ((hash_key_t)mODULE_iD          | \
   (((hash_key_t)(rNTI))   << 8)  | \
   (((hash_key_t)(iS_eNB)) << 24) | \
   (((hash_key_t)(rB_iD))  << 25) | \
   (((hash_key_t)(iS_sRB)) << 33) | \
   (((hash_key_t)(0x55))   << 34))

// hash key to the same PDCP as indexed by PDCP_COLL_KEY_VALUE(... rB_iD, iS_sRB=0) where rB_iD
// is the default DRB ID. The hidden code 0x55 indicates the key is indexed by (rB_iD,is_sRB)
// whereas the hidden code 0xaa indicates the key is for default DRB only
#define PDCP_COLL_KEY_DEFAULT_DRB_VALUE(mODULE_iD, rNTI, iS_eNB) \
  ((hash_key_t)mODULE_iD          | \
   (((hash_key_t)(rNTI))   << 8)  | \
   (((hash_key_t)(iS_eNB)) << 24) | \
   (((hash_key_t)(0xff))   << 25) | \
   (((hash_key_t)(0x00))   << 33) | \
   (((hash_key_t)(0xaa))   << 34))

// service id max val is maxServiceCount = 16 (asn1_constants.h)

#define PDCP_COLL_KEY_MBMS_VALUE(mODULE_iD, rNTI, iS_eNB, sERVICE_ID, sESSION_ID) \
  ((hash_key_t)mODULE_iD              | \
   (((hash_key_t)(rNTI))       << 8)  | \
   (((hash_key_t)(iS_eNB))     << 24) | \
   (((hash_key_t)(sERVICE_ID)) << 32) | \
   (((hash_key_t)(sESSION_ID)) << 37) | \
   (((hash_key_t)(0x0000000000000001))  << 63))

extern hash_table_t  *pdcp_coll_p;

#endif
/*@}*/
