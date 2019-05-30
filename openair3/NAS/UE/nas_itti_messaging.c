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

#include <string.h>

#include "intertask_interface.h"
#include "nas_itti_messaging.h"
#include "msc.h"

#   define TASK_ORIGIN  TASK_NAS_UE


#if (defined(ENABLE_NAS_UE_LOGGING) && defined(NAS_BUILT_IN_UE) )
__attribute__ ((unused)) static const uint8_t emm_message_ids[] = {
  ATTACH_REQUEST,
  ATTACH_ACCEPT,
  ATTACH_COMPLETE,
  ATTACH_REJECT,
  DETACH_REQUEST,
  DETACH_ACCEPT,
  TRACKING_AREA_UPDATE_REQUEST,
  TRACKING_AREA_UPDATE_ACCEPT,
  TRACKING_AREA_UPDATE_COMPLETE,
  TRACKING_AREA_UPDATE_REJECT,
  EXTENDED_SERVICE_REQUEST,
  SERVICE_REQUEST,
  SERVICE_REJECT,
  GUTI_REALLOCATION_COMMAND,
  GUTI_REALLOCATION_COMPLETE,
  AUTHENTICATION_REQUEST,
  AUTHENTICATION_RESPONSE,
  AUTHENTICATION_REJECT,
  AUTHENTICATION_FAILURE,
  IDENTITY_REQUEST,
  IDENTITY_RESPONSE,
  SECURITY_MODE_COMMAND,
  SECURITY_MODE_COMPLETE,
  SECURITY_MODE_REJECT,
  EMM_STATUS,
  EMM_INFORMATION,
  DOWNLINK_NAS_TRANSPORT,
  UPLINK_NAS_TRANSPORT,
  CS_SERVICE_NOTIFICATION,
};

__attribute__ ((unused)) static const uint8_t esm_message_ids[] = {
  ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST,
  ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT,
  ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT,
  ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST,
  ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT,
  ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT,
  MODIFY_EPS_BEARER_CONTEXT_REQUEST,
  MODIFY_EPS_BEARER_CONTEXT_ACCEPT,
  MODIFY_EPS_BEARER_CONTEXT_REJECT,
  DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST,
  DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT,
  PDN_CONNECTIVITY_REQUEST,
  PDN_CONNECTIVITY_REJECT,
  PDN_DISCONNECT_REQUEST,
  PDN_DISCONNECT_REJECT,
  BEARER_RESOURCE_ALLOCATION_REQUEST,
  BEARER_RESOURCE_ALLOCATION_REJECT,
  BEARER_RESOURCE_MODIFICATION_REQUEST,
  BEARER_RESOURCE_MODIFICATION_REJECT,
  ESM_INFORMATION_REQUEST,
  ESM_INFORMATION_RESPONSE,
  ESM_STATUS,
};


int nas_itti_plain_msg(const char *buffer, const nas_message_t *msg, const int length, const int down_link) {
  return 0;
}

int nas_itti_protected_msg(const char *buffer, const nas_message_t *msg, const int length, const int down_link) {
  return 0;
}
#endif



extern unsigned char NB_eNB_INST;

int nas_itti_kenb_refresh_req(const Byte_t kenb[32]) {
  MessageDef *message_p;
  message_p = itti_alloc_new_message(TASK_NAS_UE, NAS_KENB_REFRESH_REQ);
  memcpy(NAS_KENB_REFRESH_REQ(message_p).kenb, kenb, sizeof(NAS_KENB_REFRESH_REQ(message_p).kenb));
  MSC_LOG_TX_MESSAGE(
    MSC_NAS_UE,
    MSC_RRC_UE,
    NULL,0,
    "0 NAS_KENB_REFRESH_REQ KeNB "
    "%02x%02x%02x%02x"
    "%02x%02x%02x%02x"
    "%02x%02x%02x%02x"
    "%02x%02x%02x%02x"
    "%02x%02x%02x%02x"
    "%02x%02x%02x%02x"
    "%02x%02x%02x%02x"
    "%02x%02x%02x%02x",
    kenb[0],  kenb[1],  kenb[2],  kenb[3],
    kenb[4],  kenb[5],  kenb[6],  kenb[7],
    kenb[8],  kenb[9],  kenb[10], kenb[11],
    kenb[12], kenb[13], kenb[14], kenb[15],
    kenb[16], kenb[17], kenb[18], kenb[19],
    kenb[20], kenb[21], kenb[22], kenb[23],
    kenb[24], kenb[25], kenb[26], kenb[27],
    kenb[28], kenb[29], kenb[30], kenb[31]);
  return itti_send_msg_to_task(TASK_RRC_UE, NB_eNB_INST + 0 /* TODO to be virtualized */, message_p);
}

int nas_itti_cell_info_req(const plmn_t plmnID, const Byte_t rat, int user_id) {
  MessageDef *message_p;
  message_p = itti_alloc_new_message(TASK_NAS_UE, NAS_CELL_SELECTION_REQ);
  NAS_CELL_SELECTION_REQ(message_p).plmnID    = plmnID;
  NAS_CELL_SELECTION_REQ(message_p).rat       = rat;
  MSC_LOG_TX_MESSAGE(
    MSC_NAS_UE,
    MSC_RRC_UE,
    NULL,0,
    "0 NAS_CELL_SELECTION_REQ PLMN %X%X%X.%X%X%X",
    plmnID.MCCdigit1, plmnID.MCCdigit2, plmnID.MCCdigit3,
    plmnID.MNCdigit1, plmnID.MNCdigit2, plmnID.MNCdigit3);
  return itti_send_msg_to_task(TASK_RRC_UE, NB_eNB_INST + user_id, message_p);
}

int nas_itti_nas_establish_req(as_cause_t cause, as_call_type_t type, as_stmsi_t s_tmsi, plmn_t plmnID, Byte_t *data, uint32_t length, int user_id) {
  MessageDef *message_p;
  message_p = itti_alloc_new_message(TASK_NAS_UE, NAS_CONN_ESTABLI_REQ);
  NAS_CONN_ESTABLI_REQ(message_p).cause                       = cause;
  NAS_CONN_ESTABLI_REQ(message_p).type                        = type;
  NAS_CONN_ESTABLI_REQ(message_p).s_tmsi                      = s_tmsi;
  NAS_CONN_ESTABLI_REQ(message_p).plmnID                      = plmnID;
  NAS_CONN_ESTABLI_REQ(message_p).initialNasMsg.data          = data;
  NAS_CONN_ESTABLI_REQ(message_p).initialNasMsg.length        = length;
  MSC_LOG_TX_MESSAGE(
    MSC_NAS_UE,
    MSC_RRC_UE,
    NULL,0,
    "0 NAS_CONN_ESTABLI_REQ MME code %u m-TMSI %u PLMN %X%X%X.%X%X%X",
    s_tmsi.MMEcode, s_tmsi.m_tmsi,
    plmnID.MCCdigit1, plmnID.MCCdigit2, plmnID.MCCdigit3,
    plmnID.MNCdigit1, plmnID.MNCdigit2, plmnID.MNCdigit3);
  return itti_send_msg_to_task(TASK_RRC_UE, NB_eNB_INST + user_id, message_p);
}

int nas_itti_ul_data_req(const uint32_t ue_id, void *const data, const uint32_t length, int user_id) {
  MessageDef *message_p;
  message_p = itti_alloc_new_message(TASK_NAS_UE, NAS_UPLINK_DATA_REQ);
  NAS_UPLINK_DATA_REQ(message_p).UEid          = ue_id;
  NAS_UPLINK_DATA_REQ(message_p).nasMsg.data   = data;
  NAS_UPLINK_DATA_REQ(message_p).nasMsg.length = length;
  return itti_send_msg_to_task(TASK_RRC_UE, NB_eNB_INST + user_id, message_p);
}

int nas_itti_rab_establish_rsp(const as_stmsi_t s_tmsi, const as_rab_id_t rabID, const nas_error_code_t errCode, int user_id) {
  MessageDef *message_p;
  message_p = itti_alloc_new_message(TASK_NAS_UE, NAS_RAB_ESTABLI_RSP);
  NAS_RAB_ESTABLI_RSP(message_p).s_tmsi       = s_tmsi;
  NAS_RAB_ESTABLI_RSP(message_p).rabID        = rabID;
  NAS_RAB_ESTABLI_RSP(message_p).errCode      = errCode;
  MSC_LOG_TX_MESSAGE(
    MSC_NAS_UE,
    MSC_RRC_UE,
    NULL,0,
    "0 NAS_RAB_ESTABLI_RSP MME code %u m-TMSI %u rb id %u status %u",
    s_tmsi.MMEcode, s_tmsi.m_tmsi,rabID, errCode );
  return itti_send_msg_to_task(TASK_RRC_UE, NB_eNB_INST + user_id, message_p);
}
