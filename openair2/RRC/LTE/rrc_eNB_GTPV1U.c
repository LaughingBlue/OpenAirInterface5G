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

/*! \file rrc_eNB_GTPV1U.c
 * \brief rrc GTPV1U procedures for eNB
 * \author Lionel GAUTHIER
 * \version 1.0
 * \company Eurecom
 * \email: lionel.gauthier@eurecom.fr
 */

# include "rrc_defs.h"
# include "rrc_extern.h"
# include "RRC/LTE/MESSAGES/asn1_msg.h"
# include "rrc_eNB_GTPV1U.h"
# include "rrc_eNB_UE_context.h"
# include "msc.h"
#   include "asn1_conversions.h"
#   include "intertask_interface.h"
# include "common/ran_context.h"

extern RAN_CONTEXT_t RC;

int
rrc_eNB_process_GTPV1U_CREATE_TUNNEL_RESP(
  const protocol_ctxt_t *const ctxt_pP,
  const gtpv1u_enb_create_tunnel_resp_t *const create_tunnel_resp_pP,
  uint8_t                         *inde_list
) {
  rnti_t                         rnti;
  int                            i;
  struct rrc_eNB_ue_context_s   *ue_context_p = NULL;

  if (create_tunnel_resp_pP) {
    LOG_D(RRC, PROTOCOL_RRC_CTXT_UE_FMT" RX CREATE_TUNNEL_RESP num tunnels %u \n",
          PROTOCOL_RRC_CTXT_UE_ARGS(ctxt_pP),
          create_tunnel_resp_pP->num_tunnels);
    rnti = create_tunnel_resp_pP->rnti;
    ue_context_p = rrc_eNB_get_ue_context(
                     RC.rrc[ctxt_pP->module_id],
                     ctxt_pP->rnti);

    for (i = 0; i < create_tunnel_resp_pP->num_tunnels; i++) {
      ue_context_p->ue_context.enb_gtp_teid[inde_list[i]]  = create_tunnel_resp_pP->enb_S1u_teid[i];
      ue_context_p->ue_context.enb_gtp_addrs[inde_list[i]] = create_tunnel_resp_pP->enb_addr;
      ue_context_p->ue_context.enb_gtp_ebi[inde_list[i]]   = create_tunnel_resp_pP->eps_bearer_id[i];
      LOG_I(RRC, PROTOCOL_RRC_CTXT_UE_FMT" rrc_eNB_process_GTPV1U_CREATE_TUNNEL_RESP tunnel (%u, %u) bearer UE context index %u, msg index %u, id %u, gtp addr len %d \n",
            PROTOCOL_RRC_CTXT_UE_ARGS(ctxt_pP),
            create_tunnel_resp_pP->enb_S1u_teid[i],
            ue_context_p->ue_context.enb_gtp_teid[inde_list[i]],            
            inde_list[i],
	    i,
            create_tunnel_resp_pP->eps_bearer_id[i],
	    create_tunnel_resp_pP->enb_addr.length);
    }

	MSC_LOG_RX_MESSAGE(
			  MSC_RRC_ENB,
			  MSC_GTPU_ENB,
			  NULL,0,
			  MSC_AS_TIME_FMT" CREATE_TUNNEL_RESP RNTI %"PRIx16" ntuns %u ebid %u enb-s1u teid %u",
			  0,0,rnti,
			  create_tunnel_resp_pP->num_tunnels,
			  ue_context_p->ue_context.enb_gtp_ebi[0],
			  ue_context_p->ue_context.enb_gtp_teid[0]);
        (void)rnti; /* avoid gcc warning "set but not used" */
    return 0;
  } else {
    return -1;
  }
}

void rrc_eNB_send_GTPV1U_ENB_DELETE_TUNNEL_REQ(
  module_id_t enb_mod_idP,
  const rrc_eNB_ue_context_t* const ue_context_pP
)
{
  if (!ue_context_pP) {
    LOG_W(RRC, "[eNB] In %s: invalid UE\n", __func__);
    return;
  }

  MSC_LOG_TX_MESSAGE(MSC_RRC_ENB, MSC_GTPU_ENB, NULL, 0,
                     "0 GTPV1U_ENB_DELETE_TUNNEL_REQ rnti %x ",
                     ue_context_pP->ue_context.eNB_ue_s1ap_id);

  MessageDef *msg = itti_alloc_new_message(TASK_RRC_ENB, GTPV1U_ENB_DELETE_TUNNEL_REQ);
  memset(&GTPV1U_ENB_DELETE_TUNNEL_REQ(msg), 0, sizeof(GTPV1U_ENB_DELETE_TUNNEL_REQ(msg)));
  GTPV1U_ENB_DELETE_TUNNEL_REQ(msg).rnti = ue_context_pP->ue_context.rnti;
  GTPV1U_ENB_DELETE_TUNNEL_REQ(msg).num_erab = ue_context_pP->ue_context.nb_of_e_rabs;
  for (int e_rab = 0; e_rab < ue_context_pP->ue_context.nb_of_e_rabs; e_rab++) {
    const rb_id_t gtp_ebi = ue_context_pP->ue_context.enb_gtp_ebi[e_rab];
    GTPV1U_ENB_DELETE_TUNNEL_REQ(msg).eps_bearer_id[e_rab] = gtp_ebi;
  }
  itti_send_msg_to_task(TASK_GTPV1_U, ENB_MODULE_ID_TO_INSTANCE(enb_mod_idP), msg);
}
