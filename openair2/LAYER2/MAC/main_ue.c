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

/*! \file main.c
 * \brief top init of Layer 2
 * \author  Navid Nikaein and Raymond Knopp
 * \date 2010 - 2014
 * \version 1.0
 * \email: navid.nikaein@eurecom.fr
 * @ingroup _mac

 */

#include "mac.h"
#include "mac_proto.h"
#include "mac_extern.h"
#include "assertions.h"
#include "PHY_INTERFACE/phy_interface_extern.h"
#include "PHY/defs_UE.h"
#include "SCHED_UE/sched_UE.h"
#include "LAYER2/PDCP_v10.1.0/pdcp.h"
#include "RRC/LTE/rrc_defs.h"
#include "common/utils/LOG/log.h"
#include "nfapi/oai_integration/vendor_ext.h"
#include "RRC/L2_INTERFACE/openair_rrc_L2_interface.h"


#include "common/ran_context.h"
extern FILL_UL_INFO_MUTEX_t fill_ul_mutex;
extern void openair_rrc_top_init_ue( int eMBMS_active, char *uecap_xer, uint8_t cba_group_active, uint8_t HO_active);

void dl_phy_sync_success(module_id_t module_idP, frame_t frameP, unsigned char eNB_index, uint8_t first_sync) { //init as MR
  LOG_D(MAC, "[UE %d] Frame %d: PHY Sync to eNB_index %d successful \n",
        module_idP, frameP, eNB_index);

  if (first_sync == 1 && !(EPC_MODE_ENABLED)) {
    //layer2_init_UE(module_idP);
    openair_rrc_ue_init(module_idP, eNB_index);
  } else {
    rrc_in_sync_ind(module_idP, frameP, eNB_index);
  }
}

void
mac_UE_out_of_sync_ind(module_id_t module_idP, frame_t frameP,
                       uint16_t eNB_index) {
  //  Mac_rlc_xface->mac_out_of_sync_ind(Mod_id, frameP, eNB_index);
}


int
mac_top_init_ue(int eMBMS_active, char *uecap_xer,
                uint8_t cba_group_active, uint8_t HO_active) {
  int i;
  LOG_I(MAC, "[MAIN] Init function start:Nb_UE_INST=%d\n", NB_UE_INST);

  if (NB_UE_INST > 0) {
    UE_mac_inst =
      (UE_MAC_INST *) malloc16(NB_UE_INST * sizeof(UE_MAC_INST));
    AssertFatal(UE_mac_inst != NULL,
                "[MAIN] Can't ALLOCATE %zu Bytes for %d UE_MAC_INST with size %zu \n",
                NB_UE_INST * sizeof(UE_MAC_INST), NB_UE_INST,
                sizeof(UE_MAC_INST));
    LOG_D(MAC, "[MAIN] ALLOCATE %zu Bytes for %d UE_MAC_INST @ %p\n",
          NB_UE_INST * sizeof(UE_MAC_INST), NB_UE_INST, UE_mac_inst);
    bzero(UE_mac_inst, NB_UE_INST * sizeof(UE_MAC_INST));

    for (i = 0; i < NB_UE_INST; i++) {
      ue_init_mac(i);
    }
  } else {
    UE_mac_inst = NULL;
  }

  // mutex below are used for multiple UE's L2 FAPI simulation.
  if (NFAPI_MODE == NFAPI_UE_STUB_PNF) {
    pthread_mutex_init(&fill_ul_mutex.rx_mutex,NULL);
    pthread_mutex_init(&fill_ul_mutex.crc_mutex,NULL);
    pthread_mutex_init(&fill_ul_mutex.sr_mutex,NULL);
    pthread_mutex_init(&fill_ul_mutex.harq_mutex,NULL);
    pthread_mutex_init(&fill_ul_mutex.cqi_mutex,NULL);
    pthread_mutex_init(&fill_ul_mutex.rach_mutex,NULL);
  }

  LOG_I(MAC, "[MAIN] calling RRC\n");
  openair_rrc_top_init_ue(eMBMS_active, uecap_xer, cba_group_active,
                          HO_active);
  LOG_I(MAC, "[MAIN][INIT] Init function finished\n");
  return (0);
}

int rlcmac_init_global_param_ue(void) {
  LOG_I(MAC, "[MAIN] CALLING RLC_MODULE_INIT...\n");

  if (rlc_module_init() != 0) {
    return (-1);
  }

  pdcp_layer_init();
  LOG_I(MAC, "[MAIN] Init Global Param Done\n");
  return 0;
}

int
l2_init_ue(int eMBMS_active, char *uecap_xer, uint8_t cba_group_active,
           uint8_t HO_active) {
  LOG_I(MAC, "[MAIN] MAC_INIT_GLOBAL_PARAM IN...\n");
  //    NB_NODE=2;
  //    NB_INST=2;
  rlcmac_init_global_param_ue();
  LOG_I(MAC, "[MAIN] init UE MAC functions \n");
  mac_top_init_ue(eMBMS_active, uecap_xer, cba_group_active, HO_active);
  return (1);
}

