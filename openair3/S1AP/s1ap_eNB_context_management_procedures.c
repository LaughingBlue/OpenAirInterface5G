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

/*! \file s1ap_eNB_context_management_procedures.c
 * \brief S1AP context management procedures
 * \author  S. Roux and Navid Nikaein
 * \date 2010 - 2015
 * \email: navid.nikaein@eurecom.fr
 * \version 1.0
 * @ingroup _s1ap
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "assertions.h"
#include "conversions.h"

#include "intertask_interface.h"

#include "s1ap_common.h"
#include "s1ap_eNB_defs.h"

#include "s1ap_eNB_itti_messaging.h"

#include "s1ap_eNB_encoder.h"
#include "s1ap_eNB_nnsf.h"
#include "s1ap_eNB_ue_context.h"
#include "s1ap_eNB_nas_procedures.h"
#include "s1ap_eNB_management_procedures.h"
#include "s1ap_eNB_context_management_procedures.h"
#include "msc.h"


int s1ap_ue_context_release_complete(instance_t instance,
                                     s1ap_ue_release_complete_t *ue_release_complete_p)
{
  s1ap_eNB_instance_t                 *s1ap_eNB_instance_p = NULL;
  struct s1ap_eNB_ue_context_s        *ue_context_p        = NULL;
  S1AP_S1AP_PDU_t                      pdu;
  S1AP_UEContextReleaseComplete_t     *out;
  S1AP_UEContextReleaseComplete_IEs_t *ie;
  uint8_t  *buffer;
  uint32_t length;

  /* Retrieve the S1AP eNB instance associated with Mod_id */
  s1ap_eNB_instance_p = s1ap_eNB_get_instance(instance);

  DevAssert(ue_release_complete_p != NULL);
  DevAssert(s1ap_eNB_instance_p != NULL);

  /*RB_FOREACH(ue_context_p, s1ap_ue_map, &s1ap_eNB_instance_p->s1ap_ue_head) {
    S1AP_WARN("in s1ap_ue_map: UE context eNB_ue_s1ap_id %u mme_ue_s1ap_id %u state %u\n",
        ue_context_p->eNB_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id,
        ue_context_p->ue_state);
  }*/
  if ((ue_context_p = s1ap_eNB_get_ue_context(s1ap_eNB_instance_p,
                      ue_release_complete_p->eNB_ue_s1ap_id)) == NULL) {
    /* The context for this eNB ue s1ap id doesn't exist in the map of eNB UEs */
    S1AP_WARN("Failed to find ue context associated with eNB ue s1ap id: %u\n",
              ue_release_complete_p->eNB_ue_s1ap_id);
    return -1;
  }

  /* Prepare the S1AP message to encode */
  memset(&pdu, 0, sizeof(pdu));
  pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
  pdu.choice.successfulOutcome.procedureCode = S1AP_ProcedureCode_id_UEContextRelease;
  pdu.choice.successfulOutcome.criticality = S1AP_Criticality_reject;
  pdu.choice.successfulOutcome.value.present = S1AP_SuccessfulOutcome__value_PR_UEContextReleaseComplete;
  out = &pdu.choice.successfulOutcome.value.choice.UEContextReleaseComplete;

  /* mandatory */
  ie = (S1AP_UEContextReleaseComplete_IEs_t *)calloc(1, sizeof(S1AP_UEContextReleaseComplete_IEs_t));
  ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
  ie->criticality = S1AP_Criticality_ignore;
  ie->value.present = S1AP_UEContextReleaseComplete_IEs__value_PR_MME_UE_S1AP_ID;
  ie->value.choice.MME_UE_S1AP_ID = ue_context_p->mme_ue_s1ap_id;
  ASN_SEQUENCE_ADD(&out->protocolIEs.list, ie);

  /* mandatory */
  ie = (S1AP_UEContextReleaseComplete_IEs_t *)calloc(1, sizeof(S1AP_UEContextReleaseComplete_IEs_t));
  ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
  ie->criticality = S1AP_Criticality_ignore;
  ie->value.present = S1AP_UEContextReleaseComplete_IEs__value_PR_ENB_UE_S1AP_ID;
  ie->value.choice.ENB_UE_S1AP_ID = ue_release_complete_p->eNB_ue_s1ap_id;
  ASN_SEQUENCE_ADD(&out->protocolIEs.list, ie);

  /* optional */
  if (0) {
    ie = (S1AP_UEContextReleaseComplete_IEs_t *)calloc(1, sizeof(S1AP_UEContextReleaseComplete_IEs_t));
    ie->id = S1AP_ProtocolIE_ID_id_CriticalityDiagnostics;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UEContextReleaseComplete_IEs__value_PR_CriticalityDiagnostics;
    // ie->value.choice.CriticalityDiagnostics = ;
    ASN_SEQUENCE_ADD(&out->protocolIEs.list, ie);
  }

  /* release 12, optional */
#if (S1AP_VERSION >= MAKE_VERSION(12, 0, 0))
  if (0) {
    ie = (S1AP_UEContextReleaseComplete_IEs_t *)calloc(1, sizeof(S1AP_UEContextReleaseComplete_IEs_t));
    ie->id = S1AP_ProtocolIE_ID_id_UserLocationInformation;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UEContextReleaseComplete_IEs__value_PR_UserLocationInformation;
    // ie->value.choice.UserLocationInformation = ;
    ASN_SEQUENCE_ADD(&out->protocolIEs.list, ie);
  }

  /* release 13, optional */
#if (S1AP_VERSION >= MAKE_VERSION(13, 0, 0))
  if (0) {
    ie = (S1AP_UEContextReleaseComplete_IEs_t *)calloc(1, sizeof(S1AP_UEContextReleaseComplete_IEs_t));
    ie->id = S1AP_ProtocolIE_ID_id_InformationOnRecommendedCellsAndENBsForPaging;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UEContextReleaseComplete_IEs__value_PR_InformationOnRecommendedCellsAndENBsForPaging;
    // ie->value.choice.InformationOnRecommendedCellsAndENBsForPaging = ;
    ASN_SEQUENCE_ADD(&out->protocolIEs.list, ie);

    ie = (S1AP_UEContextReleaseComplete_IEs_t *)calloc(1, sizeof(S1AP_UEContextReleaseComplete_IEs_t));
    ie->id = S1AP_ProtocolIE_ID_id_CellIdentifierAndCELevelForCECapableUEs;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UEContextReleaseComplete_IEs__value_PR_CellIdentifierAndCELevelForCECapableUEs;
    // ie->value.choice.CellIdentifierAndCELevelForCECapableUEs = ;
    ASN_SEQUENCE_ADD(&out->protocolIEs.list, ie);
  }
#endif /* #if (S1AP_VERSION >= MAKE_VERSION(13, 0, 0)) */
#endif /* #if (S1AP_VERSION >= MAKE_VERSION(12, 0, 0)) */

  if (s1ap_eNB_encode_pdu(&pdu, &buffer, &length) < 0) {
    /* Encode procedure has failed... */
    S1AP_ERROR("Failed to encode UE context release complete\n");
    return -1;
  }

  MSC_LOG_TX_MESSAGE(
    MSC_S1AP_ENB,
    MSC_S1AP_MME,
    buffer,
    length,
    MSC_AS_TIME_FMT" UEContextRelease successfulOutcome eNB_ue_s1ap_id %u mme_ue_s1ap_id %u",
    0,0, //MSC_AS_TIME_ARGS(ctxt_pP),
    ue_release_complete_p->eNB_ue_s1ap_id,
    ue_context_p->mme_ue_s1ap_id);

  /* UE associated signalling -> use the allocated stream */
  s1ap_eNB_itti_send_sctp_data_req(s1ap_eNB_instance_p->instance,
                                   ue_context_p->mme_ref->assoc_id, buffer,
                                   length, ue_context_p->tx_stream);

  //LG s1ap_eNB_itti_send_sctp_close_association(s1ap_eNB_instance_p->instance,
  //                                             ue_context_p->mme_ref->assoc_id);
  // release UE context
  struct s1ap_eNB_ue_context_s *ue_context2_p = NULL;

  if ((ue_context2_p = RB_REMOVE(s1ap_ue_map, &s1ap_eNB_instance_p->s1ap_ue_head, ue_context_p))
      != NULL) {
    S1AP_WARN("Removed UE context eNB_ue_s1ap_id %u\n",
              ue_context2_p->eNB_ue_s1ap_id);
    s1ap_eNB_free_ue_context(ue_context2_p);
  } else {
    S1AP_WARN("Removing UE context eNB_ue_s1ap_id %u: did not find context\n",
              ue_context_p->eNB_ue_s1ap_id);
  }
  /*RB_FOREACH(ue_context_p, s1ap_ue_map, &s1ap_eNB_instance_p->s1ap_ue_head) {
    S1AP_WARN("in s1ap_ue_map: UE context eNB_ue_s1ap_id %u mme_ue_s1ap_id %u state %u\n",
        ue_context_p->eNB_ue_s1ap_id, ue_context_p->mme_ue_s1ap_id,
        ue_context_p->ue_state);
  }*/

  return 0;
}


int s1ap_ue_context_release_req(instance_t instance,
                                s1ap_ue_release_req_t *ue_release_req_p)
{
  s1ap_eNB_instance_t                *s1ap_eNB_instance_p           = NULL;
  struct s1ap_eNB_ue_context_s       *ue_context_p                  = NULL;
  S1AP_S1AP_PDU_t                     pdu;
  S1AP_UEContextReleaseRequest_t     *out;
  S1AP_UEContextReleaseRequest_IEs_t *ie;
  uint8_t                            *buffer                        = NULL;
  uint32_t                            length;
  /* Retrieve the S1AP eNB instance associated with Mod_id */
  s1ap_eNB_instance_p = s1ap_eNB_get_instance(instance);

  DevAssert(ue_release_req_p != NULL);
  DevAssert(s1ap_eNB_instance_p != NULL);

  if ((ue_context_p = s1ap_eNB_get_ue_context(s1ap_eNB_instance_p,
                      ue_release_req_p->eNB_ue_s1ap_id)) == NULL) {
    /* The context for this eNB ue s1ap id doesn't exist in the map of eNB UEs */
    S1AP_WARN("Failed to find ue context associated with eNB ue s1ap id: %u\n",
              ue_release_req_p->eNB_ue_s1ap_id);
    return -1;
  }

  /* Prepare the S1AP message to encode */
  memset(&pdu, 0, sizeof(pdu));
  pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
  pdu.choice.initiatingMessage.procedureCode = S1AP_ProcedureCode_id_UEContextReleaseRequest;
  pdu.choice.initiatingMessage.criticality = S1AP_Criticality_ignore;
  pdu.choice.initiatingMessage.value.present = S1AP_InitiatingMessage__value_PR_UEContextReleaseRequest;
  out = &pdu.choice.initiatingMessage.value.choice.UEContextReleaseRequest;

  /* mandatory */
  ie = (S1AP_UEContextReleaseRequest_IEs_t *)calloc(1, sizeof(S1AP_UEContextReleaseRequest_IEs_t));
  ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
  ie->criticality = S1AP_Criticality_reject;
  ie->value.present = S1AP_UEContextReleaseRequest_IEs__value_PR_MME_UE_S1AP_ID;
  ie->value.choice.MME_UE_S1AP_ID = ue_context_p->mme_ue_s1ap_id;
  ASN_SEQUENCE_ADD(&out->protocolIEs.list, ie);

  /* mandatory */
  ie = (S1AP_UEContextReleaseRequest_IEs_t *)calloc(1, sizeof(S1AP_UEContextReleaseRequest_IEs_t));
  ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
  ie->criticality = S1AP_Criticality_reject;
  ie->value.present = S1AP_UEContextReleaseRequest_IEs__value_PR_ENB_UE_S1AP_ID;
  ie->value.choice.ENB_UE_S1AP_ID = ue_release_req_p->eNB_ue_s1ap_id;
  ASN_SEQUENCE_ADD(&out->protocolIEs.list, ie);

  /* mandatory */
  ie = (S1AP_UEContextReleaseRequest_IEs_t *)calloc(1, sizeof(S1AP_UEContextReleaseRequest_IEs_t));
  ie->id = S1AP_ProtocolIE_ID_id_Cause;
  ie->criticality = S1AP_Criticality_ignore;
  ie->value.present = S1AP_UEContextReleaseRequest_IEs__value_PR_Cause;

  switch (ue_release_req_p->cause) {
    case S1AP_Cause_PR_radioNetwork:
      ie->value.choice.Cause.present = S1AP_Cause_PR_radioNetwork;
      ie->value.choice.Cause.choice.radioNetwork = ue_release_req_p->cause_value;
      break;

    case S1AP_Cause_PR_transport:
      ie->value.choice.Cause.present = S1AP_Cause_PR_transport;
      ie->value.choice.Cause.choice.transport = ue_release_req_p->cause_value;
      break;

    case S1AP_Cause_PR_nas:
      ie->value.choice.Cause.present = S1AP_Cause_PR_nas;
      ie->value.choice.Cause.choice.nas = ue_release_req_p->cause_value;
      break;

    case S1AP_Cause_PR_protocol:
      ie->value.choice.Cause.present = S1AP_Cause_PR_protocol;
      ie->value.choice.Cause.choice.protocol = ue_release_req_p->cause_value;
      break;

    case S1AP_Cause_PR_misc:
      ie->value.choice.Cause.present = S1AP_Cause_PR_misc;
      ie->value.choice.Cause.choice.misc = ue_release_req_p->cause_value;
      break;

    case S1AP_Cause_PR_NOTHING:
    default:
      ie->value.choice.Cause.present = S1AP_Cause_PR_NOTHING;
      break;
  }

  ASN_SEQUENCE_ADD(&out->protocolIEs.list, ie);

  /* optional */
#if (S1AP_VERSION >= MAKE_VERSION(10, 0, 0))
  if (0) {
    ie = (S1AP_UEContextReleaseRequest_IEs_t *)calloc(1, sizeof(S1AP_UEContextReleaseRequest_IEs_t));
    ie->id = S1AP_ProtocolIE_ID_id_GWContextReleaseIndication;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_UEContextReleaseRequest_IEs__value_PR_GWContextReleaseIndication;
    ie->value.choice.GWContextReleaseIndication = TRUE;
    ASN_SEQUENCE_ADD(&out->protocolIEs.list, ie);
  }
#endif /* #if (S1AP_VERSION >= MAKE_VERSION(10, 0, 0)) */

  if (s1ap_eNB_encode_pdu(&pdu, &buffer, &length) < 0) {
    /* Encode procedure has failed... */
    S1AP_ERROR("Failed to encode UE context release complete\n");
    return -1;
  }

  MSC_LOG_TX_MESSAGE(
    MSC_S1AP_ENB,
    MSC_S1AP_MME,
    buffer,
    length,
    MSC_AS_TIME_FMT" UEContextReleaseRequest initiatingMessage eNB_ue_s1ap_id %u mme_ue_s1ap_id %u",
    0,0,//MSC_AS_TIME_ARGS(ctxt_pP),
    ue_release_req_p->eNB_ue_s1ap_id,
    ue_context_p->mme_ue_s1ap_id);

  /* UE associated signalling -> use the allocated stream */
  s1ap_eNB_itti_send_sctp_data_req(s1ap_eNB_instance_p->instance,
                                   ue_context_p->mme_ref->assoc_id, buffer,
                                   length, ue_context_p->tx_stream);

  return 0;
}

