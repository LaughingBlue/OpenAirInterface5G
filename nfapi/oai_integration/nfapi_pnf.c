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

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "debug.h"
#include "nfapi/oai_integration/vendor_ext.h"
#include "nfapi_pnf_interface.h"
#include "nfapi.h"
#include "nfapi_pnf.h"
#include "common/ran_context.h"
#include "openair2/PHY_INTERFACE/phy_stub_UE.h"
//#include "openair1/PHY/vars.h"
extern RAN_CONTEXT_t RC;

#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <assert.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>

#include <vendor_ext.h>
#include "fapi_stub.h"
//#include "fapi_l1.h"
#include "common/utils/LOG/log.h"
#include "openair2/LAYER2/MAC/mac_proto.h"

#include "PHY/INIT/phy_init.h"
#include "PHY/LTE_TRANSPORT/transport_proto.h"

#define NUM_P5_PHY 2

#define _GNU_SOURCE

extern void phy_init_RU(RU_t *);
extern int config_sync_var;

extern pthread_cond_t nfapi_sync_cond;
extern pthread_mutex_t nfapi_sync_mutex;
extern int nfapi_sync_var;

extern int sync_var;
char uecap_xer_in;

extern void init_eNB_afterRU(void);
extern void init_UE_stub(int nb_inst,int,int);
extern void handle_nfapi_dci_dl_pdu(PHY_VARS_eNB *eNB, int frame, int subframe, L1_rxtx_proc_t *proc, nfapi_dl_config_request_pdu_t *dl_config_pdu);
extern void handle_nfapi_ul_pdu(PHY_VARS_eNB *eNB,L1_rxtx_proc_t *proc, nfapi_ul_config_request_pdu_t *ul_config_pdu, uint16_t frame,uint8_t subframe,uint8_t srs_present);
extern void handle_nfapi_dlsch_pdu(PHY_VARS_eNB *eNB,int frame, int subframe, L1_rxtx_proc_t *proc,  nfapi_dl_config_request_pdu_t *dl_config_pdu, uint8_t codeword_index, uint8_t *sdu);
extern void handle_nfapi_hi_dci0_dci_pdu(PHY_VARS_eNB *eNB,int frame, int subframe, L1_rxtx_proc_t *proc, nfapi_hi_dci0_request_pdu_t *hi_dci0_config_pdu);
extern void handle_nfapi_hi_dci0_hi_pdu(PHY_VARS_eNB *eNB,int frame, int subframe, L1_rxtx_proc_t *proc, nfapi_hi_dci0_request_pdu_t *hi_dci0_config_pdu);
extern void handle_nfapi_bch_pdu(PHY_VARS_eNB *eNB,L1_rxtx_proc_t *proc, nfapi_dl_config_request_pdu_t *dl_config_pdu, uint8_t *sdu);



nfapi_tx_request_pdu_t *tx_request_pdu[1023][10][10]; // [frame][subframe][max_num_pdus]

uint8_t tx_pdus[32][8][4096];


uint16_t phy_antenna_capability_values[] = { 1, 2, 4, 8, 16 };

nfapi_pnf_param_response_t g_pnf_param_resp;


nfapi_pnf_p7_config_t *p7_config_g = NULL;

void *pnf_allocate(size_t size) {
  return malloc(size);
}

void pnf_deallocate(void *ptr) {
  free(ptr);
}


typedef struct {
  //public:
  uint8_t enabled;
  uint32_t rx_port;
  uint32_t tx_port;
  //std::string tx_addr;
  char tx_addr[80];
} udp_data;

typedef struct {
  uint16_t index;
  uint16_t id;
  uint8_t rfs[2];
  uint8_t excluded_rfs[2];

  udp_data udp;

  char local_addr[80];
  int local_port;

  char *remote_addr;
  int remote_port;

  uint8_t duplex_mode;
  uint16_t dl_channel_bw_support;
  uint16_t ul_channel_bw_support;
  uint8_t num_dl_layers_supported;
  uint8_t num_ul_layers_supported;
  uint16_t release_supported;
  uint8_t nmm_modes_supported;

  uint8_t dl_ues_per_subframe;
  uint8_t ul_ues_per_subframe;

  uint8_t first_subframe_ind;

  // timing information recevied from the vnf
  uint8_t timing_window;
  uint8_t timing_info_mode;
  uint8_t timing_info_period;

} phy_info;

typedef struct {
  //public:
  uint16_t index;
  uint16_t band;
  int16_t max_transmit_power;
  int16_t min_transmit_power;
  uint8_t num_antennas_supported;
  uint32_t min_downlink_frequency;
  uint32_t max_downlink_frequency;
  uint32_t max_uplink_frequency;
  uint32_t min_uplink_frequency;
} rf_info;


typedef struct {

  int release;
  phy_info phys[2];
  rf_info rfs[2];

  uint8_t sync_mode;
  uint8_t location_mode;
  uint8_t location_coordinates[6];
  uint32_t dl_config_timing;
  uint32_t ul_config_timing;
  uint32_t tx_timing;
  uint32_t hi_dci0_timing;

  uint16_t max_phys;
  uint16_t max_total_bw;
  uint16_t max_total_dl_layers;
  uint16_t max_total_ul_layers;
  uint8_t shared_bands;
  uint8_t shared_pa;
  int16_t max_total_power;
  uint8_t oui;

  uint8_t wireshark_test_mode;

} pnf_info;

typedef struct {
  uint16_t phy_id;
  nfapi_pnf_config_t *config;
  phy_info *phy;
  nfapi_pnf_p7_config_t *p7_config;
} pnf_phy_user_data_t;

static pnf_info pnf;
static pthread_t pnf_start_pthread;

int nfapitooai_level(int nfapi_level) {
  switch(nfapi_level) {
    case NFAPI_TRACE_ERROR:
      return LOG_ERR;

    case NFAPI_TRACE_WARN:
      return LOG_WARNING;

    case NFAPI_TRACE_NOTE:
      return LOG_INFO;

    case NFAPI_TRACE_INFO:
      return LOG_DEBUG;
  }

  return LOG_ERR;
}

void pnf_nfapi_trace(nfapi_trace_level_t nfapi_level, const char *message, ...) {
  va_list args;
  va_start(args, message);
  VLOG( NFAPI_PNF, nfapitooai_level(nfapi_level), message, args);
  va_end(args);
}

void pnf_set_thread_priority(int priority) {
  pthread_attr_t ptAttr;
  struct sched_param schedParam;
  schedParam.__sched_priority = priority;

  if(sched_setscheduler(0, SCHED_RR, &schedParam) != 0) {
    printf("failed to set SCHED_RR\n");
  }

  if(pthread_attr_setschedpolicy(&ptAttr, SCHED_RR) != 0) {
    printf("failed to set pthread SCHED_RR %d\n", errno);
  }

  pthread_attr_setinheritsched(&ptAttr, PTHREAD_EXPLICIT_SCHED);
  struct sched_param thread_params;
  thread_params.sched_priority = 20;

  if(pthread_attr_setschedparam(&ptAttr, &thread_params) != 0) {
    printf("failed to set sched param\n");
  }
}

void *pnf_p7_thread_start(void *ptr) {
  NFAPI_TRACE(NFAPI_TRACE_INFO, "[PNF] P7 THREAD %s\n", __FUNCTION__);
  pnf_set_thread_priority(79);
  nfapi_pnf_p7_config_t *config = (nfapi_pnf_p7_config_t *)ptr;
  nfapi_pnf_p7_start(config);
  return 0;
}

int pnf_param_request(nfapi_pnf_config_t *config, nfapi_pnf_param_request_t *req) {
  printf("[PNF] pnf param request\n");
  nfapi_pnf_param_response_t resp;
  memset(&resp, 0, sizeof(resp));
  resp.header.message_id = NFAPI_PNF_PARAM_RESPONSE;
  resp.error_code = NFAPI_MSG_OK;
  pnf_info *pnf = (pnf_info *)(config->user_data);
  resp.pnf_param_general.tl.tag = NFAPI_PNF_PARAM_GENERAL_TAG;
  resp.pnf_param_general.nfapi_sync_mode = pnf->sync_mode;
  resp.pnf_param_general.location_mode = pnf->location_mode;
  resp.pnf_param_general.dl_config_timing = pnf->dl_config_timing;
  resp.pnf_param_general.tx_timing = pnf->tx_timing;
  resp.pnf_param_general.ul_config_timing = pnf->ul_config_timing;
  resp.pnf_param_general.hi_dci0_timing = pnf->hi_dci0_timing;
  resp.pnf_param_general.maximum_number_phys = pnf->max_phys;
  resp.pnf_param_general.maximum_total_bandwidth = pnf->max_total_bw;
  resp.pnf_param_general.maximum_total_number_dl_layers = pnf->max_total_dl_layers;
  resp.pnf_param_general.maximum_total_number_ul_layers = pnf->max_total_ul_layers;
  resp.pnf_param_general.shared_bands = pnf->shared_bands;
  resp.pnf_param_general.shared_pa = pnf->shared_pa;
  resp.pnf_param_general.maximum_total_power = pnf->max_total_power;
  resp.pnf_phy.tl.tag = NFAPI_PNF_PHY_TAG;
  resp.pnf_phy.number_of_phys = 1;

  for(int i = 0; i < 1; ++i) {
    resp.pnf_phy.phy[i].phy_config_index = pnf->phys[i].index;
    resp.pnf_phy.phy[i].downlink_channel_bandwidth_supported = pnf->phys[i].dl_channel_bw_support;
    resp.pnf_phy.phy[i].uplink_channel_bandwidth_supported = pnf->phys[i].ul_channel_bw_support;
    resp.pnf_phy.phy[i].number_of_dl_layers_supported = pnf->phys[i].num_dl_layers_supported;
    resp.pnf_phy.phy[i].number_of_ul_layers_supported = pnf->phys[i].num_ul_layers_supported;
    resp.pnf_phy.phy[i].maximum_3gpp_release_supported = pnf->phys[i].release_supported;
    resp.pnf_phy.phy[i].nmm_modes_supported = pnf->phys[i].nmm_modes_supported;
    resp.pnf_phy.phy[i].number_of_rfs = 2;

    for(int j = 0; j < 1; ++j) {
      resp.pnf_phy.phy[i].rf_config[j].rf_config_index = pnf->phys[i].rfs[j];
    }

    resp.pnf_phy.phy[i].number_of_rf_exclusions = 0;

    for(int j = 0; j < 0; ++j) {
      resp.pnf_phy.phy[i].excluded_rf_config[j].rf_config_index = pnf->phys[i].excluded_rfs[j];
    }
  }

  resp.pnf_rf.tl.tag = NFAPI_PNF_RF_TAG;
  resp.pnf_rf.number_of_rfs = 2;

  for(int i = 0; i < 2; ++i) {
    resp.pnf_rf.rf[i].rf_config_index = pnf->rfs[i].index;
    resp.pnf_rf.rf[i].band = pnf->rfs[i].band;
    resp.pnf_rf.rf[i].maximum_transmit_power = pnf->rfs[i].max_transmit_power;
    resp.pnf_rf.rf[i].minimum_transmit_power = pnf->rfs[i].min_transmit_power;
    resp.pnf_rf.rf[i].number_of_antennas_suppported = pnf->rfs[i].num_antennas_supported;
    resp.pnf_rf.rf[i].minimum_downlink_frequency = pnf->rfs[i].min_downlink_frequency;
    resp.pnf_rf.rf[i].maximum_downlink_frequency = pnf->rfs[i].max_downlink_frequency;
    resp.pnf_rf.rf[i].minimum_uplink_frequency = pnf->rfs[i].min_uplink_frequency;
    resp.pnf_rf.rf[i].maximum_uplink_frequency = pnf->rfs[i].max_uplink_frequency;
  }

  if(pnf->release >= 10) {
    resp.pnf_phy_rel10.tl.tag = NFAPI_PNF_PHY_REL10_TAG;
    resp.pnf_phy_rel10.number_of_phys = 1;

    for(int i = 0; i < 1; ++i) {
      resp.pnf_phy_rel10.phy[i].phy_config_index = pnf->phys[i].index;
      resp.pnf_phy_rel10.phy[i].transmission_mode_7_supported = 0;
      resp.pnf_phy_rel10.phy[i].transmission_mode_8_supported = 1;
      resp.pnf_phy_rel10.phy[i].two_antenna_ports_for_pucch = 0;
      resp.pnf_phy_rel10.phy[i].transmission_mode_9_supported = 1;
      resp.pnf_phy_rel10.phy[i].simultaneous_pucch_pusch = 0;
      resp.pnf_phy_rel10.phy[i].four_layer_tx_with_tm3_and_tm4 = 1;
    }
  }

  if(pnf->release >= 11) {
    resp.pnf_phy_rel11.tl.tag = NFAPI_PNF_PHY_REL11_TAG;
    resp.pnf_phy_rel11.number_of_phys = 1;

    for(int i = 0; i < 1; ++i) {
      resp.pnf_phy_rel11.phy[i].phy_config_index = pnf->phys[i].index;
      resp.pnf_phy_rel11.phy[i].edpcch_supported = 0;
      resp.pnf_phy_rel11.phy[i].multi_ack_csi_reporting = 1;
      resp.pnf_phy_rel11.phy[i].pucch_tx_diversity = 0;
      resp.pnf_phy_rel11.phy[i].ul_comp_supported = 1;
      resp.pnf_phy_rel11.phy[i].transmission_mode_5_supported = 0;
    }
  }

  if(pnf->release >= 12) {
    resp.pnf_phy_rel12.tl.tag = NFAPI_PNF_PHY_REL12_TAG;
    resp.pnf_phy_rel12.number_of_phys = 1;

    for(int i = 0; i < 1; ++i) {
      resp.pnf_phy_rel12.phy[i].phy_config_index = pnf->phys[i].index;
      resp.pnf_phy_rel12.phy[i].csi_subframe_set = 0;
      resp.pnf_phy_rel12.phy[i].enhanced_4tx_codebook = 2;
      resp.pnf_phy_rel12.phy[i].drs_supported = 0;
      resp.pnf_phy_rel12.phy[i].ul_64qam_supported = 1;
      resp.pnf_phy_rel12.phy[i].transmission_mode_10_supported = 0;
      resp.pnf_phy_rel12.phy[i].alternative_bts_indices = 1;
    }
  }

  if(pnf->release >= 13) {
    resp.pnf_phy_rel13.tl.tag = NFAPI_PNF_PHY_REL13_TAG;
    resp.pnf_phy_rel13.number_of_phys = 1;

    for(int i = 0; i < 1; ++i) {
      resp.pnf_phy_rel13.phy[i].phy_config_index = pnf->phys[i].index;
      resp.pnf_phy_rel13.phy[i].pucch_format4_supported = 0;
      resp.pnf_phy_rel13.phy[i].pucch_format5_supported = 1;
      resp.pnf_phy_rel13.phy[i].more_than_5_ca_support = 0;
      resp.pnf_phy_rel13.phy[i].laa_supported = 1;
      resp.pnf_phy_rel13.phy[i].laa_ending_in_dwpts_supported = 0;
      resp.pnf_phy_rel13.phy[i].laa_starting_in_second_slot_supported = 1;
      resp.pnf_phy_rel13.phy[i].beamforming_supported = 0;
      resp.pnf_phy_rel13.phy[i].csi_rs_enhancement_supported = 1;
      resp.pnf_phy_rel13.phy[i].drms_enhancement_supported = 0;
      resp.pnf_phy_rel13.phy[i].srs_enhancement_supported = 1;
    }

    resp.pnf_phy_rel13_nb_iot.tl.tag = NFAPI_PNF_PHY_REL13_NB_IOT_TAG;
    resp.pnf_phy_rel13_nb_iot.number_of_phys = 1;

    for(int i = 0; i < 1; ++i) {
      resp.pnf_phy_rel13_nb_iot.phy[i].phy_config_index = pnf->phys[i].index;
      resp.pnf_phy_rel13_nb_iot.phy[i].number_of_rfs = 1;

      for(int j = 0; j < 1; ++j) {
        resp.pnf_phy_rel13_nb_iot.phy[i].rf_config[j].rf_config_index = pnf->phys[i].rfs[j];
      }

      resp.pnf_phy_rel13_nb_iot.phy[i].number_of_rf_exclusions = 1;

      for(int j = 0; j < 1; ++j) {
        resp.pnf_phy_rel13_nb_iot.phy[i].excluded_rf_config[j].rf_config_index = pnf->phys[i].excluded_rfs[j];
      }

      resp.pnf_phy_rel13_nb_iot.phy[i].number_of_dl_layers_supported = pnf->phys[i].num_dl_layers_supported;
      resp.pnf_phy_rel13_nb_iot.phy[i].number_of_ul_layers_supported = pnf->phys[i].num_ul_layers_supported;
      resp.pnf_phy_rel13_nb_iot.phy[i].maximum_3gpp_release_supported = pnf->phys[i].release_supported;
      resp.pnf_phy_rel13_nb_iot.phy[i].nmm_modes_supported = pnf->phys[i].nmm_modes_supported;
    }
  }

  nfapi_pnf_pnf_param_resp(config, &resp);
  return 0;
}

int pnf_config_request(nfapi_pnf_config_t *config, nfapi_pnf_config_request_t *req) {
  printf("[PNF] pnf config request\n");
  pnf_info *pnf = (pnf_info *)(config->user_data);
  phy_info *phy = pnf->phys;
  phy->id = req->pnf_phy_rf_config.phy_rf_config[0].phy_id;
  printf("[PNF] pnf config request assigned phy_id %d to phy_config_index %d\n", phy->id, req->pnf_phy_rf_config.phy_rf_config[0].phy_config_index);
  nfapi_pnf_config_response_t resp;
  memset(&resp, 0, sizeof(resp));
  resp.header.message_id = NFAPI_PNF_CONFIG_RESPONSE;
  resp.error_code = NFAPI_MSG_OK;
  nfapi_pnf_pnf_config_resp(config, &resp);
  printf("[PNF] Sent pnf_config_resp\n");
  return 0;
}

void nfapi_send_pnf_start_resp(nfapi_pnf_config_t *config, uint16_t phy_id) {
  printf("Sending NFAPI_START_RESPONSE config:%p phy_id:%d\n", config, phy_id);
  nfapi_start_response_t start_resp;
  memset(&start_resp, 0, sizeof(start_resp));
  start_resp.header.message_id = NFAPI_START_RESPONSE;
  start_resp.header.phy_id = phy_id;
  start_resp.error_code = NFAPI_MSG_OK;
  nfapi_pnf_start_resp(config, &start_resp);
}

int pnf_start_request(nfapi_pnf_config_t *config, nfapi_pnf_start_request_t *req) {
  printf("Received NFAPI_PNF_START_REQUEST\n");
  pnf_info *pnf = (pnf_info *)(config->user_data);
  // start all phys that have been configured
  phy_info *phy = pnf->phys;

  if(phy->id != 0) {
    nfapi_pnf_start_response_t resp;
    memset(&resp, 0, sizeof(resp));
    resp.header.message_id = NFAPI_PNF_START_RESPONSE;
    resp.error_code = NFAPI_MSG_OK;
    nfapi_pnf_pnf_start_resp(config, &resp);
    printf("[PNF] Sent NFAPI_PNF_START_RESP\n");
  }

  return 0;
}

int pnf_stop_request(nfapi_pnf_config_t *config, nfapi_pnf_stop_request_t *req) {
  printf("[PNF] Received NFAPI_PNF_STOP_REQ\n");
  nfapi_pnf_stop_response_t resp;
  memset(&resp, 0, sizeof(resp));
  resp.header.message_id = NFAPI_PNF_STOP_RESPONSE;
  resp.error_code = NFAPI_MSG_OK;
  nfapi_pnf_pnf_stop_resp(config, &resp);
  printf("[PNF] Sent NFAPI_PNF_STOP_REQ\n");
  return 0;
}

int param_request(nfapi_pnf_config_t *config, nfapi_pnf_phy_config_t *phy, nfapi_param_request_t *req) {
  printf("[PNF] Received NFAPI_PARAM_REQUEST phy_id:%d\n", req->header.phy_id);
  //pnf_info* pnf = (pnf_info*)(config->user_data);
  nfapi_param_response_t nfapi_resp;
  pnf_info *pnf = (pnf_info *)(config->user_data);
  memset(&nfapi_resp, 0, sizeof(nfapi_resp));
  nfapi_resp.header.message_id = NFAPI_PARAM_RESPONSE;
  nfapi_resp.header.phy_id = req->header.phy_id;
  nfapi_resp.error_code = 0; // DJP - what value???
  struct sockaddr_in pnf_p7_sockaddr;
  pnf_p7_sockaddr.sin_addr.s_addr = inet_addr(pnf->phys[0].local_addr);
  nfapi_resp.nfapi_config.p7_pnf_address_ipv4.tl.tag = NFAPI_NFAPI_P7_PNF_ADDRESS_IPV4_TAG;
  memcpy(nfapi_resp.nfapi_config.p7_pnf_address_ipv4.address, &pnf_p7_sockaddr.sin_addr.s_addr, 4);
  nfapi_resp.num_tlv++;
  // P7 PNF Port
  nfapi_resp.nfapi_config.p7_pnf_port.tl.tag = NFAPI_NFAPI_P7_PNF_PORT_TAG;
  nfapi_resp.nfapi_config.p7_pnf_port.value = 32123; // DJP - hard code alert!!!! FIXME TODO
  nfapi_resp.num_tlv++;
  nfapi_pnf_param_resp(config, &nfapi_resp);
  printf("[PNF] Sent NFAPI_PARAM_RESPONSE phy_id:%d number_of_tlvs:%u\n", req->header.phy_id, nfapi_resp.num_tlv);
  printf("[PNF] param request .. exit\n");
  return 0;
}

int config_request(nfapi_pnf_config_t *config, nfapi_pnf_phy_config_t *phy, nfapi_config_request_t *req) {
  printf("[PNF] Received NFAPI_CONFIG_REQ phy_id:%d\n", req->header.phy_id);
  pnf_info *pnf = (pnf_info *)(config->user_data);
  uint8_t num_tlv = 0;
  //struct PHY_VARS_eNB_s *eNB = RC.eNB[0][0];
  //  In the case of nfapi_mode = 3 (UE = PNF) we should not have dependency on any eNB var. So we aim
  // to keep only the necessary just to keep the nfapi FSM rolling by sending a dummy response.
  LTE_DL_FRAME_PARMS *fp;

  if (NFAPI_MODE!=NFAPI_UE_STUB_PNF) {
    struct PHY_VARS_eNB_s *eNB = RC.eNB[0][0];
    fp = &eNB->frame_parms;
  } else {
    fp = (LTE_DL_FRAME_PARMS *) malloc(sizeof(LTE_DL_FRAME_PARMS));
  }

  phy_info *phy_info = pnf->phys;

  if(req->nfapi_config.timing_window.tl.tag == NFAPI_NFAPI_TIMING_WINDOW_TAG) {
    phy_info->timing_window = req->nfapi_config.timing_window.value;
    printf("Phy_info:Timing window:%u NFAPI_CONFIG:timing_window:%u\n", phy_info->timing_window, req->nfapi_config.timing_window.value);
    num_tlv++;
  }

  if(req->nfapi_config.timing_info_mode.tl.tag == NFAPI_NFAPI_TIMING_INFO_MODE_TAG) {
    printf("timing info mode:%d\n", req->nfapi_config.timing_info_mode.value);
    phy_info->timing_info_mode = req->nfapi_config.timing_info_mode.value;
    num_tlv++;
  } else {
    phy_info->timing_info_mode = 0;
    printf("NO timing info mode provided\n");
  }

  if(req->nfapi_config.timing_info_period.tl.tag == NFAPI_NFAPI_TIMING_INFO_PERIOD_TAG) {
    printf("timing info period provided value:%d\n", req->nfapi_config.timing_info_period.value);
    phy_info->timing_info_period = req->nfapi_config.timing_info_period.value;
    num_tlv++;
  } else {
    phy_info->timing_info_period = 0;
  }

  if(req->rf_config.dl_channel_bandwidth.tl.tag == NFAPI_RF_CONFIG_DL_CHANNEL_BANDWIDTH_TAG) {
    phy_info->dl_channel_bw_support = req->rf_config.dl_channel_bandwidth.value;
    fp->N_RB_DL = req->rf_config.dl_channel_bandwidth.value;
    num_tlv++;
    NFAPI_TRACE(NFAPI_TRACE_ERROR, "%s() NFAPI_RF_CONFIG_DL_CHANNEL_BANDWIDTH_TAG N_RB_DL:%u\n", __FUNCTION__, fp->N_RB_DL);
  } else {
    NFAPI_TRACE(NFAPI_TRACE_ERROR, "%s() Missing NFAPI_RF_CONFIG_DL_CHANNEL_BANDWIDTH_TAG\n", __FUNCTION__);
  }

  if(req->rf_config.ul_channel_bandwidth.tl.tag == NFAPI_RF_CONFIG_UL_CHANNEL_BANDWIDTH_TAG) {
    phy_info->ul_channel_bw_support = req->rf_config.ul_channel_bandwidth.value;
    fp->N_RB_UL = req->rf_config.ul_channel_bandwidth.value;
    num_tlv++;
  }

  if(req->nfapi_config.rf_bands.tl.tag == NFAPI_NFAPI_RF_BANDS_TAG) {
    pnf->rfs[0].band = req->nfapi_config.rf_bands.rf_band[0];
    fp->eutra_band = req->nfapi_config.rf_bands.rf_band[0];
    num_tlv++;
  }

  if(req->nfapi_config.earfcn.tl.tag == NFAPI_NFAPI_EARFCN_TAG) {
    fp->dl_CarrierFreq = from_earfcn(fp->eutra_band, req->nfapi_config.earfcn.value);
    fp->ul_CarrierFreq = fp->dl_CarrierFreq - (get_uldl_offset(fp->eutra_band) * 1e5);
    num_tlv++;
    NFAPI_TRACE(NFAPI_TRACE_INFO, "%s() earfcn:%u dl_carrierFreq:%u ul_CarrierFreq:%u band:%u N_RB_DL:%u\n",
                __FUNCTION__, req->nfapi_config.earfcn.value, fp->dl_CarrierFreq, fp->ul_CarrierFreq, pnf->rfs[0].band, fp->N_RB_DL);
  }

  if (req->subframe_config.duplex_mode.tl.tag == NFAPI_SUBFRAME_CONFIG_DUPLEX_MODE_TAG) {
    fp->frame_type = req->subframe_config.duplex_mode.value==0 ? TDD : FDD;
    num_tlv++;
    NFAPI_TRACE(NFAPI_TRACE_INFO, "%s() frame_type:%d\n", __FUNCTION__, fp->frame_type);
  }

  if (req->subframe_config.dl_cyclic_prefix_type.tl.tag == NFAPI_SUBFRAME_CONFIG_DL_CYCLIC_PREFIX_TYPE_TAG) {
    fp->Ncp = req->subframe_config.dl_cyclic_prefix_type.value;
    num_tlv++;
  }

  if (req->subframe_config.ul_cyclic_prefix_type.tl.tag == NFAPI_SUBFRAME_CONFIG_UL_CYCLIC_PREFIX_TYPE_TAG) {
    fp->Ncp_UL = req->subframe_config.ul_cyclic_prefix_type.value;
    num_tlv++;
  }

  fp->num_MBSFN_config = 0; // DJP - hard code alert

  if (req->sch_config.physical_cell_id.tl.tag == NFAPI_SCH_CONFIG_PHYSICAL_CELL_ID_TAG) {
    fp->Nid_cell = req->sch_config.physical_cell_id.value;
    fp->nushift = fp->Nid_cell%6;
    num_tlv++;
  }

  if (req->rf_config.tx_antenna_ports.tl.tag == NFAPI_RF_CONFIG_TX_ANTENNA_PORTS_TAG) {
    fp->nb_antennas_tx = req->rf_config.tx_antenna_ports.value;
    fp->nb_antenna_ports_eNB = 1;
    num_tlv++;
  }

  if (req->rf_config.rx_antenna_ports.tl.tag == NFAPI_RF_CONFIG_RX_ANTENNA_PORTS_TAG) {
    fp->nb_antennas_rx = req->rf_config.rx_antenna_ports.value;
    num_tlv++;
  }

  if (req->phich_config.phich_resource.tl.tag == NFAPI_PHICH_CONFIG_PHICH_RESOURCE_TAG) {
    fp->phich_config_common.phich_resource = req->phich_config.phich_resource.value;
    num_tlv++;
  }

  if (req->phich_config.phich_duration.tl.tag == NFAPI_PHICH_CONFIG_PHICH_DURATION_TAG) {
    fp->phich_config_common.phich_duration = req->phich_config.phich_duration.value;
    num_tlv++;
  }

  if (req->phich_config.phich_power_offset.tl.tag == NFAPI_PHICH_CONFIG_PHICH_POWER_OFFSET_TAG) {
    LOG_E(PHY, "%s() NFAPI_PHICH_CONFIG_PHICH_POWER_OFFSET_TAG tag:%d not supported\n", __FUNCTION__, req->phich_config.phich_power_offset.tl.tag);
    //fp->phich_config_common.phich_power_offset = req->phich_config.
    num_tlv++;
  }

  // UL RS Config
  if (req->uplink_reference_signal_config.cyclic_shift_1_for_drms.tl.tag == NFAPI_UPLINK_REFERENCE_SIGNAL_CONFIG_CYCLIC_SHIFT_1_FOR_DRMS_TAG) {
    fp->pusch_config_common.ul_ReferenceSignalsPUSCH.cyclicShift = req->uplink_reference_signal_config.cyclic_shift_1_for_drms.value;
    num_tlv++;
  }

  if (req->uplink_reference_signal_config.uplink_rs_hopping.tl.tag == NFAPI_UPLINK_REFERENCE_SIGNAL_CONFIG_UPLINK_RS_HOPPING_TAG) {
    fp->pusch_config_common.ul_ReferenceSignalsPUSCH.groupHoppingEnabled = req->uplink_reference_signal_config.uplink_rs_hopping.value;
    num_tlv++;
  }

  if (req->uplink_reference_signal_config.group_assignment.tl.tag == NFAPI_UPLINK_REFERENCE_SIGNAL_CONFIG_GROUP_ASSIGNMENT_TAG) {
    fp->pusch_config_common.ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH = req->uplink_reference_signal_config.group_assignment.value;
    num_tlv++;
  }

  if (req->pusch_config.hopping_mode.tl.tag == NFAPI_PUSCH_CONFIG_HOPPING_MODE_TAG) {
  }  // DJP - not being handled?

  if (req->pusch_config.hopping_offset.tl.tag == NFAPI_PUSCH_CONFIG_HOPPING_OFFSET_TAG) {
  }  // DJP - not being handled?

  if (req->pusch_config.number_of_subbands.tl.tag == NFAPI_PUSCH_CONFIG_NUMBER_OF_SUBBANDS_TAG) {
  }  // DJP - not being handled?

  if (req->prach_config.configuration_index.tl.tag == NFAPI_PRACH_CONFIG_CONFIGURATION_INDEX_TAG) {
    fp->prach_config_common.prach_ConfigInfo.prach_ConfigIndex=req->prach_config.configuration_index.value;
    num_tlv++;
  }

  if (req->prach_config.root_sequence_index.tl.tag == NFAPI_PRACH_CONFIG_ROOT_SEQUENCE_INDEX_TAG) {
    fp->prach_config_common.rootSequenceIndex=req->prach_config.root_sequence_index.value;
    num_tlv++;
  }

  if (req->prach_config.zero_correlation_zone_configuration.tl.tag == NFAPI_PRACH_CONFIG_ZERO_CORRELATION_ZONE_CONFIGURATION_TAG) {
    fp->prach_config_common.prach_ConfigInfo.zeroCorrelationZoneConfig=req->prach_config.zero_correlation_zone_configuration.value;
    num_tlv++;
  }

  if (req->prach_config.high_speed_flag.tl.tag == NFAPI_PRACH_CONFIG_HIGH_SPEED_FLAG_TAG) {
    fp->prach_config_common.prach_ConfigInfo.highSpeedFlag=req->prach_config.high_speed_flag.value;
    num_tlv++;
  }

  if (req->prach_config.frequency_offset.tl.tag == NFAPI_PRACH_CONFIG_FREQUENCY_OFFSET_TAG) {
    fp->prach_config_common.prach_ConfigInfo.prach_FreqOffset=req->prach_config.frequency_offset.value;
    num_tlv++;
  }

  if(NFAPI_MODE!=NFAPI_UE_STUB_PNF) {
    printf("[PNF] CONFIG_REQUEST[num_tlv:%d] TLVs processed:%d\n", req->num_tlv, num_tlv);
    printf("[PNF] Simulating PHY CONFIG - DJP\n");
    PHY_Config_t phy_config;
    phy_config.Mod_id = 0;
    phy_config.CC_id=0;
    phy_config.cfg = req;
    phy_config_request(&phy_config);
    dump_frame_parms(fp);
  }

  phy_info->remote_port = req->nfapi_config.p7_vnf_port.value;
  struct sockaddr_in vnf_p7_sockaddr;
  memcpy(&vnf_p7_sockaddr.sin_addr.s_addr, &(req->nfapi_config.p7_vnf_address_ipv4.address[0]), 4);
  phy_info->remote_addr = inet_ntoa(vnf_p7_sockaddr.sin_addr);
  printf("[PNF] %d vnf p7 %s:%d timing %d %d %d\n", phy_info->id, phy_info->remote_addr, phy_info->remote_port,
         phy_info->timing_window, phy_info->timing_info_mode, phy_info->timing_info_period);
  nfapi_config_response_t nfapi_resp;
  memset(&nfapi_resp, 0, sizeof(nfapi_resp));
  nfapi_resp.header.message_id = NFAPI_CONFIG_RESPONSE;
  nfapi_resp.header.phy_id = phy_info->id;
  nfapi_resp.error_code = 0; // DJP - some value resp->error_code;
  nfapi_pnf_config_resp(config, &nfapi_resp);
  printf("[PNF] Sent NFAPI_CONFIG_RESPONSE phy_id:%d\n", phy_info->id);

  if(NFAPI_MODE==NFAPI_UE_STUB_PNF)
    free(fp);

  return 0;
}

nfapi_p7_message_header_t *pnf_phy_allocate_p7_vendor_ext(uint16_t message_id, uint16_t *msg_size) {
  if(message_id == P7_VENDOR_EXT_REQ) {
    (*msg_size) = sizeof(vendor_ext_p7_req);
    return (nfapi_p7_message_header_t *)malloc(sizeof(vendor_ext_p7_req));
  }

  return 0;
}

void pnf_phy_deallocate_p7_vendor_ext(nfapi_p7_message_header_t *header) {
  free(header);
}

int pnf_phy_hi_dci0_req(nfapi_pnf_p7_config_t *pnf_p7, nfapi_hi_dci0_request_t *req) {
  if (req->hi_dci0_request_body.number_of_dci == 0 && req->hi_dci0_request_body.number_of_hi == 0)
    LOG_D(PHY,"[PNF] HI_DCI0_REQUEST SFN/SF:%05d dci:%d hi:%d\n", NFAPI_SFNSF2DEC(req->sfn_sf), req->hi_dci0_request_body.number_of_dci, req->hi_dci0_request_body.number_of_hi);

  //phy_info* phy = (phy_info*)(pnf_p7->user_data);
  struct PHY_VARS_eNB_s *eNB = RC.eNB[0][0];
  L1_rxtx_proc_t *proc = &eNB->proc.L1_proc;

  for (int i=0; i<req->hi_dci0_request_body.number_of_dci + req->hi_dci0_request_body.number_of_hi; i++) {
    //LOG_D(PHY,"[PNF] HI_DCI0_REQ sfn_sf:%d PDU[%d]\n", NFAPI_SFNSF2DEC(req->sfn_sf), i);
    if (req->hi_dci0_request_body.hi_dci0_pdu_list[i].pdu_type == NFAPI_HI_DCI0_DCI_PDU_TYPE) {
      //LOG_D(PHY,"[PNF] HI_DCI0_REQ sfn_sf:%d PDU[%d] - NFAPI_HI_DCI0_DCI_PDU_TYPE\n", NFAPI_SFNSF2DEC(req->sfn_sf), i);
      nfapi_hi_dci0_request_pdu_t *hi_dci0_req_pdu = &req->hi_dci0_request_body.hi_dci0_pdu_list[i];
      handle_nfapi_hi_dci0_dci_pdu(eNB,NFAPI_SFNSF2SFN(req->sfn_sf),NFAPI_SFNSF2SF(req->sfn_sf),proc,hi_dci0_req_pdu);
      eNB->pdcch_vars[NFAPI_SFNSF2SF(req->sfn_sf)&1].num_dci++;
    } else if (req->hi_dci0_request_body.hi_dci0_pdu_list[i].pdu_type == NFAPI_HI_DCI0_HI_PDU_TYPE) {
      LOG_D(PHY,"[PNF] HI_DCI0_REQ sfn_sf:%d PDU[%d] - NFAPI_HI_DCI0_HI_PDU_TYPE\n", NFAPI_SFNSF2DEC(req->sfn_sf), i);
      nfapi_hi_dci0_request_pdu_t *hi_dci0_req_pdu = &req->hi_dci0_request_body.hi_dci0_pdu_list[i];
      handle_nfapi_hi_dci0_hi_pdu(eNB, NFAPI_SFNSF2SFN(req->sfn_sf),NFAPI_SFNSF2SF(req->sfn_sf), proc, hi_dci0_req_pdu);
    } else {
      LOG_E(PHY,"[PNF] HI_DCI0_REQ sfn_sf:%d PDU[%d] - unknown pdu type:%d\n", NFAPI_SFNSF2DEC(req->sfn_sf), i, req->hi_dci0_request_body.hi_dci0_pdu_list[i].pdu_type);
    }
  }

  return 0;
}

int pnf_phy_dl_config_req(nfapi_pnf_p7_config_t *pnf_p7, nfapi_dl_config_request_t *req) {
  if (RC.ru == 0) {
    return -1;
  }

  if (RC.eNB == 0) {
    return -2;
  }

  if (RC.eNB[0][0] == 0) {
    return -3;
  }

  if (sync_var != 0) {
    NFAPI_TRACE(NFAPI_TRACE_INFO, "%s() Main system not up - is this a dummy subframe?\n", __FUNCTION__);
    return -4;
  }

  int sfn = NFAPI_SFNSF2SFN(req->sfn_sf);
  int sf = NFAPI_SFNSF2SF(req->sfn_sf);
  struct PHY_VARS_eNB_s *eNB = RC.eNB[0][0];
  L1_rxtx_proc_t *proc = &eNB->proc.L1_proc;
  nfapi_dl_config_request_pdu_t *dl_config_pdu_list = req->dl_config_request_body.dl_config_pdu_list;
  LTE_eNB_PDCCH *pdcch_vars = &eNB->pdcch_vars[sf&1];
  pdcch_vars->num_pdcch_symbols = req->dl_config_request_body.number_pdcch_ofdm_symbols;
  pdcch_vars->num_dci = 0;

  if (req->dl_config_request_body.number_dci ||
      req->dl_config_request_body.number_pdu ||
      req->dl_config_request_body.number_pdsch_rnti)
    NFAPI_TRACE(NFAPI_TRACE_INFO, "%s() TX:%d/%d RX:%d/%d sfn_sf:%d pdcch:%u dl_cfg[dci:%u pdus:%d pdsch_rnti:%d] pcfich:%u\n",
                __FUNCTION__, proc->frame_tx, proc->subframe_tx, proc->frame_rx, proc->subframe_rx,
                NFAPI_SFNSF2DEC(req->sfn_sf),
                req->dl_config_request_body.number_pdcch_ofdm_symbols,
                req->dl_config_request_body.number_dci,
                req->dl_config_request_body.number_pdu,
                req->dl_config_request_body.number_pdsch_rnti,
                req->dl_config_request_body.transmission_power_pcfich);

  for (int i=0; i<req->dl_config_request_body.number_pdu; i++) {
    NFAPI_TRACE(NFAPI_TRACE_INFO, "%s() sfn/sf:%d PDU[%d] size:%d pdcch_vars->num_dci:%d\n", __FUNCTION__, NFAPI_SFNSF2DEC(req->sfn_sf), i, dl_config_pdu_list[i].pdu_size,pdcch_vars->num_dci);

    if (dl_config_pdu_list[i].pdu_type == NFAPI_DL_CONFIG_DCI_DL_PDU_TYPE) {
      handle_nfapi_dci_dl_pdu(eNB,NFAPI_SFNSF2SFN(req->sfn_sf),NFAPI_SFNSF2SF(req->sfn_sf),proc,&dl_config_pdu_list[i]);
      pdcch_vars->num_dci++; // Is actually number of DCI PDUs
      NFAPI_TRACE(NFAPI_TRACE_INFO, "%s() pdcch_vars->num_dci:%d\n", __FUNCTION__, pdcch_vars->num_dci);
    } else if (dl_config_pdu_list[i].pdu_type == NFAPI_DL_CONFIG_BCH_PDU_TYPE) {
      nfapi_dl_config_bch_pdu *bch_pdu = &dl_config_pdu_list[i].bch_pdu;
      uint16_t pdu_index = bch_pdu->bch_pdu_rel8.pdu_index;

      if (tx_request_pdu[sfn][sf][pdu_index] != NULL) {
        //NFAPI_TRACE(NFAPI_TRACE_INFO, "%s() PDU:%d BCH: pdu_index:%u pdu_length:%d sdu_length:%d BCH_SDU:%x,%x,%x\n", __FUNCTION__, i, pdu_index, bch_pdu->bch_pdu_rel8.length, tx_request_pdu[sfn][sf][pdu_index]->segments[0].segment_length, sdu[0], sdu[1], sdu[2]);
        handle_nfapi_bch_pdu(eNB, proc, &dl_config_pdu_list[i], tx_request_pdu[sfn][sf][pdu_index]->segments[0].segment_data);
        eNB->pbch_configured=1;
      } else {
        NFAPI_TRACE(NFAPI_TRACE_ERROR, "%s() BCH NULL TX PDU SFN/SF:%d PDU_INDEX:%d\n", __FUNCTION__, NFAPI_SFNSF2DEC(req->sfn_sf), pdu_index);
      }
    } else if (dl_config_pdu_list[i].pdu_type == NFAPI_DL_CONFIG_DLSCH_PDU_TYPE) {
      nfapi_dl_config_dlsch_pdu *dlsch_pdu = &dl_config_pdu_list[i].dlsch_pdu;
      nfapi_dl_config_dlsch_pdu_rel8_t *rel8_pdu = &dlsch_pdu->dlsch_pdu_rel8;
      nfapi_tx_request_pdu_t *tx_pdu = tx_request_pdu[sfn][sf][rel8_pdu->pdu_index];

      if (tx_pdu != NULL) {
        int UE_id = find_dlsch(rel8_pdu->rnti,eNB,SEARCH_EXIST_OR_FREE);
        AssertFatal(UE_id!=-1,"no free or exiting dlsch_context\n");
        AssertFatal(UE_id<NUMBER_OF_UE_MAX,"returned UE_id %d >= %d(NUMBER_OF_UE_MAX)\n",UE_id,NUMBER_OF_UE_MAX);
        LTE_eNB_DLSCH_t *dlsch0 = eNB->dlsch[UE_id][0];
        //LTE_eNB_DLSCH_t *dlsch1 = eNB->dlsch[UE_id][1];
        int harq_pid = dlsch0->harq_ids[sfn%2][sf];

        if(harq_pid >= dlsch0->Mdlharq) {
          LOG_E(PHY,"pnf_phy_dl_config_req illegal harq_pid %d\n", harq_pid);
          return(-1);
        }

        uint8_t *dlsch_sdu = tx_pdus[UE_id][harq_pid];
        memcpy(dlsch_sdu, tx_pdu->segments[0].segment_data, tx_pdu->segments[0].segment_length);
        //NFAPI_TRACE(NFAPI_TRACE_INFO, "%s() DLSCH:pdu_index:%d handle_nfapi_dlsch_pdu(eNB, proc_rxtx, dlsch_pdu, transport_blocks:%d sdu:%p) eNB->pdcch_vars[proc->subframe_tx & 1].num_pdcch_symbols:%d\n", __FUNCTION__, rel8_pdu->pdu_index, rel8_pdu->transport_blocks, dlsch_sdu, eNB->pdcch_vars[proc->subframe_tx & 1].num_pdcch_symbols);
        handle_nfapi_dlsch_pdu( eNB, sfn,sf, &eNB->proc.L1_proc, &dl_config_pdu_list[i], rel8_pdu->transport_blocks-1, dlsch_sdu);
      } else {
        NFAPI_TRACE(NFAPI_TRACE_ERROR, "%s() DLSCH NULL TX PDU SFN/SF:%d PDU_INDEX:%d\n", __FUNCTION__, NFAPI_SFNSF2DEC(req->sfn_sf), rel8_pdu->pdu_index);
      }
    } else {
      NFAPI_TRACE(NFAPI_TRACE_ERROR, "%s() UNKNOWN:%d\n", __FUNCTION__, dl_config_pdu_list[i].pdu_type);
    }
  }

  if(req->vendor_extension)
    free(req->vendor_extension);

  return 0;
}

int pnf_phy_tx_req(nfapi_pnf_p7_config_t *pnf_p7, nfapi_tx_request_t *req) {
  uint16_t sfn = NFAPI_SFNSF2SFN(req->sfn_sf);
  uint16_t sf = NFAPI_SFNSF2SF(req->sfn_sf);

  if (req->tx_request_body.number_of_pdus==0)
    LOG_D(PHY,"%s() SFN/SF:%d%d PDUs:%d\n", __FUNCTION__, sfn, sf, req->tx_request_body.number_of_pdus);

  if (req->tx_request_body.tl.tag==NFAPI_TX_REQUEST_BODY_TAG) {
    for (int i=0; i<req->tx_request_body.number_of_pdus; i++) {
      LOG_D(PHY,"%s() SFN/SF:%d%d number_of_pdus:%d [PDU:%d] pdu_length:%d pdu_index:%d num_segments:%d\n",
            __FUNCTION__,
            sfn, sf,
            req->tx_request_body.number_of_pdus,
            i,
            req->tx_request_body.tx_pdu_list[i].pdu_length,
            req->tx_request_body.tx_pdu_list[i].pdu_index,
            req->tx_request_body.tx_pdu_list[i].num_segments
           );
      tx_request_pdu[sfn][sf][i] = &req->tx_request_body.tx_pdu_list[i];
    }
  }

  return 0;
}

int pnf_phy_ul_config_req(nfapi_pnf_p7_config_t *pnf_p7, nfapi_ul_config_request_t *req) {
  if (0)LOG_D(PHY,"[PNF] UL_CONFIG_REQ %s() sfn_sf:%d pdu:%d rach_prach_frequency_resources:%d srs_present:%u\n",
                __FUNCTION__,
                NFAPI_SFNSF2DEC(req->sfn_sf),
                req->ul_config_request_body.number_of_pdus,
                req->ul_config_request_body.rach_prach_frequency_resources,
                req->ul_config_request_body.srs_present
               );

  if (RC.ru == 0) {
    return -1;
  }

  if (RC.eNB == 0) {
    return -2;
  }

  if (RC.eNB[0][0] == 0) {
    return -3;
  }

  if (sync_var != 0) {
    NFAPI_TRACE(NFAPI_TRACE_INFO, "%s() Main system not up - is this a dummy subframe?\n", __FUNCTION__);
    return -4;
  }

  uint16_t curr_sfn = NFAPI_SFNSF2SFN(req->sfn_sf);
  uint16_t curr_sf = NFAPI_SFNSF2SF(req->sfn_sf);
  struct PHY_VARS_eNB_s *eNB = RC.eNB[0][0];
  L1_rxtx_proc_t *proc = &eNB->proc.L1_proc;
  nfapi_ul_config_request_pdu_t *ul_config_pdu_list = req->ul_config_request_body.ul_config_pdu_list;

  for (int i=0; i<req->ul_config_request_body.number_of_pdus; i++) {
    //LOG_D(PHY, "%s() sfn/sf:%d PDU[%d] size:%d\n", __FUNCTION__, NFAPI_SFNSF2DEC(req->sfn_sf), i, ul_config_pdu_list[i].pdu_size);
    if (
      ul_config_pdu_list[i].pdu_type == NFAPI_UL_CONFIG_ULSCH_PDU_TYPE ||
      ul_config_pdu_list[i].pdu_type == NFAPI_UL_CONFIG_ULSCH_HARQ_PDU_TYPE ||
      ul_config_pdu_list[i].pdu_type == NFAPI_UL_CONFIG_ULSCH_CQI_RI_PDU_TYPE ||
      ul_config_pdu_list[i].pdu_type == NFAPI_UL_CONFIG_ULSCH_CQI_HARQ_RI_PDU_TYPE ||
      ul_config_pdu_list[i].pdu_type == NFAPI_UL_CONFIG_UCI_HARQ_PDU_TYPE ||
      ul_config_pdu_list[i].pdu_type == NFAPI_UL_CONFIG_UCI_SR_PDU_TYPE ||
      ul_config_pdu_list[i].pdu_type == NFAPI_UL_CONFIG_UCI_SR_HARQ_PDU_TYPE
    ) {
      //LOG_D(PHY, "%s() handle_nfapi_ul_pdu() for PDU:%d\n", __FUNCTION__, i);
      handle_nfapi_ul_pdu(eNB,proc,&ul_config_pdu_list[i],curr_sfn,curr_sf,req->ul_config_request_body.srs_present);
    } else {
      NFAPI_TRACE(NFAPI_TRACE_ERROR, "%s() PDU:%i UNKNOWN type :%d\n", __FUNCTION__, i, ul_config_pdu_list[i].pdu_type);
    }
  }

  return 0;
}

int pnf_phy_lbt_dl_config_req(nfapi_pnf_p7_config_t *config, nfapi_lbt_dl_config_request_t *req) {
  //printf("[PNF] lbt dl config request\n");
  return 0;
}

int pnf_phy_vendor_ext(nfapi_pnf_p7_config_t *config, nfapi_p7_message_header_t *msg) {
  if(msg->message_id == P7_VENDOR_EXT_REQ) {
    //vendor_ext_p7_req* req = (vendor_ext_p7_req*)msg;
    //printf("[PNF] vendor request (1:%d 2:%d)\n", req->dummy1, req->dummy2);
  } else {
    printf("[PNF] unknown vendor ext\n");
  }

  return 0;
}

int pnf_phy_pack_p7_vendor_extension(nfapi_p7_message_header_t *header, uint8_t **ppWritePackedMsg, uint8_t *end, nfapi_p7_codec_config_t *codex) {
  //NFAPI_TRACE(NFAPI_TRACE_INFO, "%s\n", __FUNCTION__);
  if(header->message_id == P7_VENDOR_EXT_IND) {
    vendor_ext_p7_ind *ind = (vendor_ext_p7_ind *)(header);

    if(!push16(ind->error_code, ppWritePackedMsg, end))
      return 0;

    return 1;
  }

  return -1;
}

int pnf_phy_unpack_p7_vendor_extension(nfapi_p7_message_header_t *header, uint8_t **ppReadPackedMessage, uint8_t *end, nfapi_p7_codec_config_t *codec) {
  if(header->message_id == P7_VENDOR_EXT_REQ) {
    //NFAPI_TRACE(NFAPI_TRACE_INFO, "%s\n", __FUNCTION__);
    vendor_ext_p7_req *req = (vendor_ext_p7_req *)(header);

    if(!(pull16(ppReadPackedMessage, &req->dummy1, end) &&
         pull16(ppReadPackedMessage, &req->dummy2, end)))
      return 0;

    return 1;
  }

  return -1;
}

int pnf_phy_unpack_vendor_extension_tlv(nfapi_tl_t *tl, uint8_t **ppReadPackedMessage, uint8_t *end, void **ve, nfapi_p7_codec_config_t *config) {
  //NFAPI_TRACE(NFAPI_TRACE_INFO, "pnf_phy_unpack_vendor_extension_tlv\n");
  switch(tl->tag) {
    case VENDOR_EXT_TLV_1_TAG:
      *ve = malloc(sizeof(vendor_ext_tlv_1));

      if(!pull32(ppReadPackedMessage, &((vendor_ext_tlv_1 *)(*ve))->dummy, end))
        return 0;

      return 1;
      break;
  }

  return -1;
}

int pnf_phy_pack_vendor_extention_tlv(void *ve, uint8_t **ppWritePackedMsg, uint8_t *end, nfapi_p7_codec_config_t *config) {
  //printf("%s\n", __FUNCTION__);
  (void)ve;
  (void)ppWritePackedMsg;
  return -1;
}

int pnf_sim_unpack_vendor_extension_tlv(nfapi_tl_t *tl, uint8_t **ppReadPackedMessage, uint8_t *end, void **ve, nfapi_p4_p5_codec_config_t *config) {
  //NFAPI_TRACE(NFAPI_TRACE_INFO, "pnf_sim_unpack_vendor_extension_tlv\n");
  switch(tl->tag) {
    case VENDOR_EXT_TLV_2_TAG:
      *ve = malloc(sizeof(vendor_ext_tlv_2));

      if(!pull32(ppReadPackedMessage, &((vendor_ext_tlv_2 *)(*ve))->dummy, end))
        return 0;

      return 1;
      break;
  }

  return -1;
}

int pnf_sim_pack_vendor_extention_tlv(void *ve, uint8_t **ppWritePackedMsg, uint8_t *end, nfapi_p4_p5_codec_config_t *config) {
  //printf("%s\n", __FUNCTION__);
  (void)ve;
  (void)ppWritePackedMsg;
  return -1;
}

nfapi_dl_config_request_t dummy_dl_config_req;
nfapi_tx_request_t dummy_tx_req;

nfapi_pnf_p7_subframe_buffer_t dummy_subframe;

int start_request(nfapi_pnf_config_t *config, nfapi_pnf_phy_config_t *phy, nfapi_start_request_t *req) {
  printf("[PNF] Received NFAPI_START_REQ phy_id:%d\n", req->header.phy_id);
  nfapi_set_trace_level(NFAPI_TRACE_INFO);
  pnf_info *pnf = (pnf_info *)(config->user_data);
  phy_info *phy_info = pnf->phys;
  nfapi_pnf_p7_config_t *p7_config = nfapi_pnf_p7_config_create();
  p7_config->phy_id = phy->phy_id;
  p7_config->remote_p7_port = phy_info->remote_port;
  p7_config->remote_p7_addr = phy_info->remote_addr;
  p7_config->local_p7_port = 32123; // DJP - good grief cannot seem to get the right answer phy_info->local_port;
  //DJP p7_config->local_p7_addr = (char*)phy_info->local_addr.c_str();
  p7_config->local_p7_addr = phy_info->local_addr;
  printf("[PNF] P7 remote:%s:%d local:%s:%d\n", p7_config->remote_p7_addr, p7_config->remote_p7_port, p7_config->local_p7_addr, p7_config->local_p7_port);
  p7_config->user_data = phy_info;
  p7_config->malloc = &pnf_allocate;
  p7_config->free = &pnf_deallocate;
  p7_config->codec_config.allocate = &pnf_allocate;
  p7_config->codec_config.deallocate = &pnf_deallocate;
  p7_config->trace = &pnf_nfapi_trace;
  phy->user_data = p7_config;
  p7_config->subframe_buffer_size = phy_info->timing_window;
  printf("subframe_buffer_size configured using phy_info->timing_window:%d\n", phy_info->timing_window);

  if(phy_info->timing_info_mode & 0x1) {
    p7_config->timing_info_mode_periodic = 1;
    p7_config->timing_info_period = phy_info->timing_info_period;
  }

  if(phy_info->timing_info_mode & 0x2) {
    p7_config->timing_info_mode_aperiodic = 1;
  }

  p7_config->dl_config_req = &pnf_phy_dl_config_req;
  p7_config->ul_config_req = &pnf_phy_ul_config_req;
  p7_config->hi_dci0_req = &pnf_phy_hi_dci0_req;
  p7_config->tx_req = &pnf_phy_tx_req;
  p7_config->lbt_dl_config_req = &pnf_phy_lbt_dl_config_req;

  if (NFAPI_MODE==NFAPI_UE_STUB_PNF) {
    p7_config->dl_config_req = &memcpy_dl_config_req;
    p7_config->ul_config_req = &memcpy_ul_config_req;
    p7_config->hi_dci0_req = &memcpy_hi_dci0_req;
    p7_config->tx_req = &memcpy_tx_req;
  } else {
    p7_config->dl_config_req = &pnf_phy_dl_config_req;
    p7_config->ul_config_req = &pnf_phy_ul_config_req;
    p7_config->hi_dci0_req = &pnf_phy_hi_dci0_req;
    p7_config->tx_req = &pnf_phy_tx_req;
  }

  p7_config->lbt_dl_config_req = &pnf_phy_lbt_dl_config_req;
  memset(&dummy_dl_config_req, 0, sizeof(dummy_dl_config_req));
  dummy_dl_config_req.dl_config_request_body.tl.tag=NFAPI_DL_CONFIG_REQUEST_BODY_TAG;
  dummy_dl_config_req.dl_config_request_body.number_pdcch_ofdm_symbols=1;
  dummy_dl_config_req.dl_config_request_body.number_dci=0;
  dummy_dl_config_req.dl_config_request_body.number_pdu=0;
  dummy_dl_config_req.dl_config_request_body.number_pdsch_rnti=0;
  dummy_dl_config_req.dl_config_request_body.transmission_power_pcfich=6000;
  dummy_dl_config_req.dl_config_request_body.dl_config_pdu_list=0;
  memset(&dummy_tx_req, 0, sizeof(dummy_tx_req));
  dummy_tx_req.tx_request_body.number_of_pdus=0;
  dummy_tx_req.tx_request_body.tl.tag=NFAPI_TX_REQUEST_BODY_TAG;
  dummy_subframe.dl_config_req = &dummy_dl_config_req;
  dummy_subframe.tx_req = 0;//&dummy_tx_req;
  dummy_subframe.ul_config_req=0;
  dummy_subframe.hi_dci0_req=0;
  dummy_subframe.lbt_dl_config_req=0;
  p7_config->dummy_subframe = dummy_subframe;
  p7_config->vendor_ext = &pnf_phy_vendor_ext;
  p7_config->allocate_p7_vendor_ext = &pnf_phy_allocate_p7_vendor_ext;
  p7_config->deallocate_p7_vendor_ext = &pnf_phy_deallocate_p7_vendor_ext;
  p7_config->codec_config.unpack_p7_vendor_extension = &pnf_phy_unpack_p7_vendor_extension;
  p7_config->codec_config.pack_p7_vendor_extension = &pnf_phy_pack_p7_vendor_extension;
  p7_config->codec_config.unpack_vendor_extension_tlv = &pnf_phy_unpack_vendor_extension_tlv;
  p7_config->codec_config.pack_vendor_extension_tlv = &pnf_phy_pack_vendor_extention_tlv;
  NFAPI_TRACE(NFAPI_TRACE_INFO, "[PNF] Creating P7 thread %s\n", __FUNCTION__);
  pthread_t p7_thread;
  pthread_create(&p7_thread, NULL, &pnf_p7_thread_start, p7_config);
  //((pnf_phy_user_data_t*)(phy_info->fapi->user_data))->p7_config = p7_config;
  NFAPI_TRACE(NFAPI_TRACE_INFO, "[PNF] Calling l1_north_init_eNB() %s\n", __FUNCTION__);
  l1_north_init_eNB();
  NFAPI_TRACE(NFAPI_TRACE_INFO, "[PNF] DJP - HACK - Set p7_config global ready for subframe ind%s\n", __FUNCTION__);
  p7_config_g = p7_config;

  // Need to wait for main thread to create RU structures
  while(config_sync_var<0) {
    usleep(5000000);
    printf("[PNF] waiting for OAI to be configured (eNB/RU)\n");
  }

  printf("[PNF] OAI eNB/RU configured\n");
  //printf("[PNF] About to call phy_init_RU() for RC.ru[0]:%p\n", RC.ru[0]);
  //phy_init_RU(RC.ru[0]);
  printf("[PNF] About to call init_eNB_afterRU()\n");

  if (NFAPI_MODE!=NFAPI_UE_STUB_PNF) {
    init_eNB_afterRU();
  }

  // Signal to main thread that it can carry on - otherwise RU will startup too quickly and it is not initialised
  {
    pthread_mutex_lock(&nfapi_sync_mutex);
    nfapi_sync_var=0;
    pthread_cond_broadcast(&nfapi_sync_cond);
    pthread_mutex_unlock(&nfapi_sync_mutex);
  }

  while(sync_var<0) {
    usleep(5000000);
    printf("[PNF] waiting for OAI to be started\n");
  }

  printf("[PNF] Sending PNF_START_RESP\n");
  nfapi_send_pnf_start_resp(config, p7_config->phy_id);
  printf("[PNF] Sending first P7 subframe ind\n");
  nfapi_pnf_p7_subframe_ind(p7_config, p7_config->phy_id, 0); // DJP - SFN_SF set to zero - correct???
  printf("[PNF] Sent first P7 subframe ind\n");
  return 0;
}

int measurement_request(nfapi_pnf_config_t *config, nfapi_pnf_phy_config_t *phy, nfapi_measurement_request_t *req) {
  nfapi_measurement_response_t resp;
  memset(&resp, 0, sizeof(resp));
  resp.header.message_id = NFAPI_MEASUREMENT_RESPONSE;
  resp.header.phy_id = req->header.phy_id;
  resp.error_code = NFAPI_MSG_OK;
  nfapi_pnf_measurement_resp(config, &resp);
  return 0;
}

int rssi_request(nfapi_pnf_config_t *config, nfapi_pnf_phy_config_t *phy, nfapi_rssi_request_t *req) {
  nfapi_rssi_response_t resp;
  memset(&resp, 0, sizeof(resp));
  resp.header.message_id = NFAPI_RSSI_RESPONSE;
  resp.header.phy_id = req->header.phy_id;
  resp.error_code = NFAPI_P4_MSG_OK;
  nfapi_pnf_rssi_resp(config, &resp);
  nfapi_rssi_indication_t ind;
  memset(&ind, 0, sizeof(ind));
  ind.header.message_id = NFAPI_RSSI_INDICATION;
  ind.header.phy_id = req->header.phy_id;
  ind.error_code = NFAPI_P4_MSG_OK;
  ind.rssi_indication_body.tl.tag = NFAPI_RSSI_INDICATION_TAG;
  ind.rssi_indication_body.number_of_rssi = 1;
  ind.rssi_indication_body.rssi[0] = -42;
  nfapi_pnf_rssi_ind(config, &ind);
  return 0;
}

int cell_search_request(nfapi_pnf_config_t *config, nfapi_pnf_phy_config_t *phy, nfapi_cell_search_request_t *req) {
  nfapi_cell_search_response_t resp;
  memset(&resp, 0, sizeof(resp));
  resp.header.message_id = NFAPI_CELL_SEARCH_RESPONSE;
  resp.header.phy_id = req->header.phy_id;
  resp.error_code = NFAPI_P4_MSG_OK;
  nfapi_pnf_cell_search_resp(config, &resp);
  nfapi_cell_search_indication_t ind;
  memset(&ind, 0, sizeof(ind));
  ind.header.message_id = NFAPI_CELL_SEARCH_INDICATION;
  ind.header.phy_id = req->header.phy_id;
  ind.error_code = NFAPI_P4_MSG_OK;

  switch(req->rat_type) {
    case NFAPI_RAT_TYPE_LTE:
      ind.lte_cell_search_indication.tl.tag = NFAPI_LTE_CELL_SEARCH_INDICATION_TAG;
      ind.lte_cell_search_indication.number_of_lte_cells_found = 1;
      ind.lte_cell_search_indication.lte_found_cells[0].pci = 123;
      ind.lte_cell_search_indication.lte_found_cells[0].rsrp = 123;
      ind.lte_cell_search_indication.lte_found_cells[0].rsrq = 123;
      ind.lte_cell_search_indication.lte_found_cells[0].frequency_offset = 123;
      break;

    case NFAPI_RAT_TYPE_UTRAN: {
      ind.utran_cell_search_indication.tl.tag = NFAPI_UTRAN_CELL_SEARCH_INDICATION_TAG;
      ind.utran_cell_search_indication.number_of_utran_cells_found = 1;
      ind.utran_cell_search_indication.utran_found_cells[0].psc = 89;
      ind.utran_cell_search_indication.utran_found_cells[0].rscp = 89;
      ind.utran_cell_search_indication.utran_found_cells[0].ecno = 89;
      ind.utran_cell_search_indication.utran_found_cells[0].frequency_offset = -89;
    }
    break;

    case NFAPI_RAT_TYPE_GERAN: {
      ind.geran_cell_search_indication.tl.tag = NFAPI_GERAN_CELL_SEARCH_INDICATION_TAG;
      ind.geran_cell_search_indication.number_of_gsm_cells_found = 1;
      ind.geran_cell_search_indication.gsm_found_cells[0].bsic = 23;
      ind.geran_cell_search_indication.gsm_found_cells[0].rxlev = 23;
      ind.geran_cell_search_indication.gsm_found_cells[0].rxqual = 23;
      ind.geran_cell_search_indication.gsm_found_cells[0].frequency_offset = -23;
      ind.geran_cell_search_indication.gsm_found_cells[0].sfn_offset = 230;
    }
    break;
  }

  ind.pnf_cell_search_state.tl.tag = NFAPI_PNF_CELL_SEARCH_STATE_TAG;
  ind.pnf_cell_search_state.length = 3;
  nfapi_pnf_cell_search_ind(config, &ind);
  return 0;
}

int broadcast_detect_request(nfapi_pnf_config_t *config, nfapi_pnf_phy_config_t *phy, nfapi_broadcast_detect_request_t *req) {
  nfapi_broadcast_detect_response_t resp;
  memset(&resp, 0, sizeof(resp));
  resp.header.message_id = NFAPI_BROADCAST_DETECT_RESPONSE;
  resp.header.phy_id = req->header.phy_id;
  resp.error_code = NFAPI_P4_MSG_OK;
  nfapi_pnf_broadcast_detect_resp(config, &resp);
  nfapi_broadcast_detect_indication_t ind;
  memset(&ind, 0, sizeof(ind));
  ind.header.message_id = NFAPI_BROADCAST_DETECT_INDICATION;
  ind.header.phy_id = req->header.phy_id;
  ind.error_code = NFAPI_P4_MSG_OK;

  switch(req->rat_type) {
    case NFAPI_RAT_TYPE_LTE: {
      ind.lte_broadcast_detect_indication.tl.tag = NFAPI_LTE_BROADCAST_DETECT_INDICATION_TAG;
      ind.lte_broadcast_detect_indication.number_of_tx_antenna = 1;
      ind.lte_broadcast_detect_indication.mib_length = 4;
      //ind.lte_broadcast_detect_indication.mib...
      ind.lte_broadcast_detect_indication.sfn_offset = 77;
    }
    break;

    case NFAPI_RAT_TYPE_UTRAN: {
      ind.utran_broadcast_detect_indication.tl.tag = NFAPI_UTRAN_BROADCAST_DETECT_INDICATION_TAG;
      ind.utran_broadcast_detect_indication.mib_length = 4;
      //ind.utran_broadcast_detect_indication.mib...
      // ind.utran_broadcast_detect_indication.sfn_offset; DJP - nonsense line
    }
    break;
  }

  ind.pnf_cell_broadcast_state.tl.tag = NFAPI_PNF_CELL_BROADCAST_STATE_TAG;
  ind.pnf_cell_broadcast_state.length = 3;
  nfapi_pnf_broadcast_detect_ind(config, &ind);
  return 0;
}

int system_information_schedule_request(nfapi_pnf_config_t *config, nfapi_pnf_phy_config_t *phy, nfapi_system_information_schedule_request_t *req) {
  nfapi_system_information_schedule_response_t resp;
  memset(&resp, 0, sizeof(resp));
  resp.header.message_id = NFAPI_SYSTEM_INFORMATION_SCHEDULE_RESPONSE;
  resp.header.phy_id = req->header.phy_id;
  resp.error_code = NFAPI_P4_MSG_OK;
  nfapi_pnf_system_information_schedule_resp(config, &resp);
  nfapi_system_information_schedule_indication_t ind;
  memset(&ind, 0, sizeof(ind));
  ind.header.message_id = NFAPI_SYSTEM_INFORMATION_SCHEDULE_INDICATION;
  ind.header.phy_id = req->header.phy_id;
  ind.error_code = NFAPI_P4_MSG_OK;
  ind.lte_system_information_indication.tl.tag = NFAPI_LTE_SYSTEM_INFORMATION_INDICATION_TAG;
  ind.lte_system_information_indication.sib_type = 3;
  ind.lte_system_information_indication.sib_length = 5;
  //ind.lte_system_information_indication.sib...
  nfapi_pnf_system_information_schedule_ind(config, &ind);
  return 0;
}

int system_information_request(nfapi_pnf_config_t *config, nfapi_pnf_phy_config_t *phy, nfapi_system_information_request_t *req) {
  nfapi_system_information_response_t resp;
  memset(&resp, 0, sizeof(resp));
  resp.header.message_id = NFAPI_SYSTEM_INFORMATION_RESPONSE;
  resp.header.phy_id = req->header.phy_id;
  resp.error_code = NFAPI_P4_MSG_OK;
  nfapi_pnf_system_information_resp(config, &resp);
  nfapi_system_information_indication_t ind;
  memset(&ind, 0, sizeof(ind));
  ind.header.message_id = NFAPI_SYSTEM_INFORMATION_INDICATION;
  ind.header.phy_id = req->header.phy_id;
  ind.error_code = NFAPI_P4_MSG_OK;

  switch(req->rat_type) {
    case NFAPI_RAT_TYPE_LTE: {
      ind.lte_system_information_indication.tl.tag = NFAPI_LTE_SYSTEM_INFORMATION_INDICATION_TAG;
      ind.lte_system_information_indication.sib_type = 1;
      ind.lte_system_information_indication.sib_length = 3;
      //ind.lte_system_information_indication.sib...
    }
    break;

    case NFAPI_RAT_TYPE_UTRAN: {
      ind.utran_system_information_indication.tl.tag = NFAPI_UTRAN_SYSTEM_INFORMATION_INDICATION_TAG;
      ind.utran_system_information_indication.sib_length = 3;
      //ind.utran_system_information_indication.sib...
    }
    break;

    case NFAPI_RAT_TYPE_GERAN: {
      ind.geran_system_information_indication.tl.tag = NFAPI_GERAN_SYSTEM_INFORMATION_INDICATION_TAG;
      ind.geran_system_information_indication.si_length = 3;
      //ind.geran_system_information_indication.si...
    }
    break;
  }

  nfapi_pnf_system_information_ind(config, &ind);
  return 0;
}

int nmm_stop_request(nfapi_pnf_config_t *config, nfapi_pnf_phy_config_t *phy, nfapi_nmm_stop_request_t *req) {
  nfapi_nmm_stop_response_t resp;
  memset(&resp, 0, sizeof(resp));
  resp.header.message_id = NFAPI_NMM_STOP_RESPONSE;
  resp.header.phy_id = req->header.phy_id;
  resp.error_code = NFAPI_P4_MSG_OK;
  nfapi_pnf_nmm_stop_resp(config, &resp);
  return 0;
}

int vendor_ext(nfapi_pnf_config_t *config, nfapi_p4_p5_message_header_t *msg) {
  NFAPI_TRACE(NFAPI_TRACE_INFO, "[PNF] P5 %s %p\n", __FUNCTION__, msg);

  switch(msg->message_id) {
    case P5_VENDOR_EXT_REQ: {
      vendor_ext_p5_req *req = (vendor_ext_p5_req *)msg;
      NFAPI_TRACE(NFAPI_TRACE_INFO, "[PNF] P5 Vendor Ext Req (%d %d)\n", req->dummy1, req->dummy2);
      // send back the P5_VENDOR_EXT_RSP
      vendor_ext_p5_rsp rsp;
      memset(&rsp, 0, sizeof(rsp));
      rsp.header.message_id = P5_VENDOR_EXT_RSP;
      rsp.error_code = NFAPI_MSG_OK;
      nfapi_pnf_vendor_extension(config, &rsp.header, sizeof(vendor_ext_p5_rsp));
    }
    break;
  }

  return 0;
}

nfapi_p4_p5_message_header_t *pnf_sim_allocate_p4_p5_vendor_ext(uint16_t message_id, uint16_t *msg_size) {
  if(message_id == P5_VENDOR_EXT_REQ) {
    (*msg_size) = sizeof(vendor_ext_p5_req);
    return (nfapi_p4_p5_message_header_t *)malloc(sizeof(vendor_ext_p5_req));
  }

  return 0;
}

void pnf_sim_deallocate_p4_p5_vendor_ext(nfapi_p4_p5_message_header_t *header) {
  free(header);
}

int pnf_sim_pack_p4_p5_vendor_extension(nfapi_p4_p5_message_header_t *header, uint8_t **ppWritePackedMsg, uint8_t *end, nfapi_p4_p5_codec_config_t *config) {
  //NFAPI_TRACE(NFAPI_TRACE_INFO, "%s\n", __FUNCTION__);
  if(header->message_id == P5_VENDOR_EXT_RSP) {
    vendor_ext_p5_rsp *rsp = (vendor_ext_p5_rsp *)(header);
    return (!push16(rsp->error_code, ppWritePackedMsg, end));
  }

  return 0;
}

int pnf_sim_unpack_p4_p5_vendor_extension(nfapi_p4_p5_message_header_t *header, uint8_t **ppReadPackedMessage, uint8_t *end, nfapi_p4_p5_codec_config_t *codec) {
  //NFAPI_TRACE(NFAPI_TRACE_INFO, "%s\n", __FUNCTION__);
  if(header->message_id == P5_VENDOR_EXT_REQ) {
    vendor_ext_p5_req *req = (vendor_ext_p5_req *)(header);
    return (!(pull16(ppReadPackedMessage, &req->dummy1, end) &&
              pull16(ppReadPackedMessage, &req->dummy2, end)));
    //NFAPI_TRACE(NFAPI_TRACE_INFO, "%s (%d %d)\n", __FUNCTION__, req->dummy1, req->dummy2);
  }

  return 0;
}

/*------------------------------------------------------------------------------*/

void *pnf_start_thread(void *ptr) {
  NFAPI_TRACE(NFAPI_TRACE_INFO, "[PNF] IN PNF NFAPI start thread %s\n", __FUNCTION__);
  nfapi_pnf_config_t *config = (nfapi_pnf_config_t *)ptr;
  struct sched_param sp;
  sp.sched_priority = 20;
  pthread_setschedparam(pthread_self(),SCHED_FIFO,&sp);
  nfapi_pnf_start(config);
  return (void *)0;
}

void configure_nfapi_pnf(char *vnf_ip_addr, int vnf_p5_port, char *pnf_ip_addr, int pnf_p7_port, int vnf_p7_port) {
  printf("%s() PNF\n\n\n\n\n\n", __FUNCTION__);

  if(NFAPI_MODE!=NFAPI_UE_STUB_PNF) {
    nfapi_setmode(NFAPI_PNF);  // PNF!
  }

  nfapi_pnf_config_t *config = nfapi_pnf_config_create();
  config->vnf_ip_addr = vnf_ip_addr;
  config->vnf_p5_port = vnf_p5_port;
  pnf.phys[0].udp.enabled = 1;
  pnf.phys[0].udp.rx_port = pnf_p7_port;
  pnf.phys[0].udp.tx_port = vnf_p7_port;
  strcpy(pnf.phys[0].udp.tx_addr, vnf_ip_addr);
  strcpy(pnf.phys[0].local_addr, pnf_ip_addr);
  printf("%s() VNF:%s:%d PNF_PHY[addr:%s UDP:tx_addr:%s:%d rx:%d]\n",
         __FUNCTION__,
         config->vnf_ip_addr, config->vnf_p5_port,
         pnf.phys[0].local_addr,
         pnf.phys[0].udp.tx_addr, pnf.phys[0].udp.tx_port,
         pnf.phys[0].udp.rx_port);
  config->pnf_param_req = &pnf_param_request;
  config->pnf_config_req = &pnf_config_request;
  config->pnf_start_req = &pnf_start_request;
  config->pnf_stop_req = &pnf_stop_request;
  config->param_req = &param_request;
  config->config_req = &config_request;
  config->start_req = &start_request;
  config->measurement_req = &measurement_request;
  config->rssi_req = &rssi_request;
  config->cell_search_req = &cell_search_request;
  config->broadcast_detect_req = &broadcast_detect_request;
  config->system_information_schedule_req = &system_information_schedule_request;
  config->system_information_req = &system_information_request;
  config->nmm_stop_req = &nmm_stop_request;
  config->vendor_ext = &vendor_ext;
  config->trace = &pnf_nfapi_trace;
  config->user_data = &pnf;
  // To allow custom vendor extentions to be added to nfapi
  config->codec_config.unpack_vendor_extension_tlv = &pnf_sim_unpack_vendor_extension_tlv;
  config->codec_config.pack_vendor_extension_tlv = &pnf_sim_pack_vendor_extention_tlv;
  config->allocate_p4_p5_vendor_ext = &pnf_sim_allocate_p4_p5_vendor_ext;
  config->deallocate_p4_p5_vendor_ext = &pnf_sim_deallocate_p4_p5_vendor_ext;
  config->codec_config.unpack_p4_p5_vendor_extension = &pnf_sim_unpack_p4_p5_vendor_extension;
  config->codec_config.pack_p4_p5_vendor_extension = &pnf_sim_pack_p4_p5_vendor_extension;
  NFAPI_TRACE(NFAPI_TRACE_INFO, "[PNF] Creating PNF NFAPI start thread %s\n", __FUNCTION__);
  pthread_create(&pnf_start_pthread, NULL, &pnf_start_thread, config);
  pthread_setname_np(pnf_start_pthread, "NFAPI_PNF");
}

void oai_subframe_ind(uint16_t sfn, uint16_t sf) {
  //LOG_D(PHY,"%s(sfn:%d, sf:%d)\n", __FUNCTION__, sfn, sf);

  //TODO FIXME - HACK - DJP - using a global to bodge it in
  if (p7_config_g != NULL && sync_var==0) {
    uint16_t sfn_sf_tx = sfn<<4 | sf;

    if ((sfn % 100 == 0) && sf==0) {
      struct timespec ts;
      clock_gettime(CLOCK_MONOTONIC, &ts);
      NFAPI_TRACE(NFAPI_TRACE_INFO, "[PNF] %s %d.%d (sfn:%u sf:%u) SFN/SF(TX):%u\n", __FUNCTION__, ts.tv_sec, ts.tv_nsec, sfn, sf, NFAPI_SFNSF2DEC(sfn_sf_tx));
    }

    int subframe_ret = nfapi_pnf_p7_subframe_ind(p7_config_g, p7_config_g->phy_id, sfn_sf_tx);

    if (subframe_ret) {
      NFAPI_TRACE(NFAPI_TRACE_INFO, "[PNF] %s(frame:%u subframe:%u) SFN/SF(TX):%u - PROBLEM with pnf_p7_subframe_ind()\n", __FUNCTION__, sfn, sf, sfn_sf_tx, NFAPI_SFNSF2DEC(sfn_sf_tx));
    } else {
      //NFAPI_TRACE(NFAPI_TRACE_INFO, "***NFAPI subframe handler finished *** \n");
    }
  } else {
  }
}

int oai_nfapi_rach_ind(nfapi_rach_indication_t *rach_ind) {
  rach_ind->header.phy_id = 1; // DJP HACK TODO FIXME - need to pass this around!!!!
  LOG_D(PHY, "%s() sfn_sf:%d preambles:%d\n", __FUNCTION__, NFAPI_SFNSF2DEC(rach_ind->sfn_sf), rach_ind->rach_indication_body.number_of_preambles);
  return nfapi_pnf_p7_rach_ind(p7_config_g, rach_ind);
}

int oai_nfapi_harq_indication(nfapi_harq_indication_t *harq_ind) {
  harq_ind->header.phy_id = 1; // DJP HACK TODO FIXME - need to pass this around!!!!
  harq_ind->header.message_id = NFAPI_HARQ_INDICATION;
  LOG_D(PHY, "%s() sfn_sf:%d number_of_harqs:%d\n", __FUNCTION__, NFAPI_SFNSF2DEC(harq_ind->sfn_sf), harq_ind->harq_indication_body.number_of_harqs);
  int retval = nfapi_pnf_p7_harq_ind(p7_config_g, harq_ind);

  if (retval != 0)
    LOG_E(PHY, "%s() sfn_sf:%d number_of_harqs:%d nfapi_pnf_p7_harq_ind()=%d\n", __FUNCTION__, NFAPI_SFNSF2DEC(harq_ind->sfn_sf), harq_ind->harq_indication_body.number_of_harqs, retval);

  return retval;
}

int oai_nfapi_crc_indication(nfapi_crc_indication_t *crc_ind) {
  crc_ind->header.phy_id = 1; // DJP HACK TODO FIXME - need to pass this around!!!!
  crc_ind->header.message_id = NFAPI_CRC_INDICATION;
  //LOG_D(PHY, "%s() sfn_sf:%d number_of_crcs:%d\n", __FUNCTION__, NFAPI_SFNSF2DEC(crc_ind->sfn_sf), crc_ind->crc_indication_body.number_of_crcs);
  return nfapi_pnf_p7_crc_ind(p7_config_g, crc_ind);
}

int oai_nfapi_cqi_indication(nfapi_cqi_indication_t *ind) {
  ind->header.phy_id = 1; // DJP HACK TODO FIXME - need to pass this around!!!!
  ind->header.message_id = NFAPI_RX_CQI_INDICATION;
  //LOG_D(PHY, "%s() sfn_sf:%d number_of_cqis:%d\n", __FUNCTION__, NFAPI_SFNSF2DEC(ind->sfn_sf), ind->cqi_indication_body.number_of_cqis);
  return nfapi_pnf_p7_cqi_ind(p7_config_g, ind);
}

int oai_nfapi_rx_ind(nfapi_rx_indication_t *ind) {
  ind->header.phy_id = 1; // DJP HACK TODO FIXME - need to pass this around!!!!
  ind->header.message_id = NFAPI_RX_ULSCH_INDICATION;
  int retval = nfapi_pnf_p7_rx_ind(p7_config_g, ind);
  //LOG_D(PHY,"%s() SFN/SF:%d pdus:%d retval:%d\n", __FUNCTION__, NFAPI_SFNSF2DEC(ind->sfn_sf), ind->rx_indication_body.number_of_pdus, retval);
  //free(ind.rx_indication_body.rx_pdu_list);
  return retval;
}

int oai_nfapi_sr_indication(nfapi_sr_indication_t *ind) {
  ind->header.phy_id = 1; // DJP HACK TODO FIXME - need to pass this around!!!!
  int retval = nfapi_pnf_p7_sr_ind(p7_config_g, ind);
  //LOG_D(PHY,"%s() SFN/SF:%d srs:%d retval:%d\n", __FUNCTION__, NFAPI_SFNSF2DEC(ind->sfn_sf), ind->sr_indication_body.number_of_srs, retval);
  //free(ind.rx_indication_body.rx_pdu_list);
  return retval;
}
