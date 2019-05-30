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

/*! \file flexran_agent_common.c
 * \brief common primitives for all agents
 * \author Xenofon Foukas, Mohamed Kassem and Navid Nikaein, shahab SHARIAT BAGHERI
 * \date 2017
 * \version 0.1
 */

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>

#include "flexran_agent_common.h"
#include "flexran_agent_common_internal.h"
#include "flexran_agent_extern.h"
#include "flexran_agent_net_comm.h"
#include "flexran_agent_ran_api.h"
#include "flexran_agent_phy.h"
#include "flexran_agent_mac.h"
#include "flexran_agent_rrc.h"
//#include "PHY/extern.h"
#include "common/utils/LOG/log.h"
#include "flexran_agent_mac_internal.h"

//#include "SCHED/defs.h"
#include "RRC/LTE/rrc_extern.h"
#include "RRC/L2_INTERFACE/openair_rrc_L2_interface.h"
#include "rrc_eNB_UE_context.h"

/*
 * message primitives
 */

int flexran_agent_serialize_message(Protocol__FlexranMessage *msg, void **buf, int *size) {
  *size = protocol__flexran_message__get_packed_size(msg);

  if (buf == NULL)
    goto error;

  *buf = malloc(*size);

  if (*buf == NULL)
    goto error;

  protocol__flexran_message__pack(msg, *buf);
  return 0;
error:
  LOG_E(FLEXRAN_AGENT, "an error occured\n"); // change the com
  return -1;
}



/* We assume that the buffer size is equal to the message size.
   Should be chekced durint Tx/Rx */
int flexran_agent_deserialize_message(void *data, int size, Protocol__FlexranMessage **msg) {
  *msg = protocol__flexran_message__unpack(NULL, size, data);

  if (*msg == NULL)
    goto error;

  return 0;
error:
  //LOG_E(MAC, "%s: an error occured\n", __FUNCTION__);
  return -1;
}



int flexran_create_header(xid_t xid, Protocol__FlexType type,  Protocol__FlexHeader **header) {
  *header = malloc(sizeof(Protocol__FlexHeader));

  if(*header == NULL)
    goto error;

  protocol__flex_header__init(*header);
  (*header)->version = FLEXRAN_VERSION;
  (*header)->has_version = 1;
  // check if the type is set
  (*header)->type = type;
  (*header)->has_type = 1;
  (*header)->xid = xid;
  (*header)->has_xid = 1;
  return 0;
error:
  LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}


int flexran_agent_hello(mid_t mod_id, const void *params, Protocol__FlexranMessage **msg) {
  Protocol__FlexHeader *header = NULL;
  /*TODO: Need to set random xid or xid from received hello message*/
  xid_t xid = 1;
  Protocol__FlexHello *hello_msg;
  hello_msg = malloc(sizeof(Protocol__FlexHello));

  if(hello_msg == NULL)
    goto error;

  protocol__flex_hello__init(hello_msg);

  if (flexran_create_header(xid, PROTOCOL__FLEX_TYPE__FLPT_HELLO, &header) != 0)
    goto error;

  hello_msg->header = header;
  hello_msg->bs_id  = flexran_get_bs_id(mod_id);
  hello_msg->has_bs_id = 1;
  hello_msg->n_capabilities = flexran_get_capabilities(mod_id, &hello_msg->capabilities);

  *msg = malloc(sizeof(Protocol__FlexranMessage));

  if(*msg == NULL)
    goto error;

  protocol__flexran_message__init(*msg);
  (*msg)->msg_case = PROTOCOL__FLEXRAN_MESSAGE__MSG_HELLO_MSG;
  (*msg)->msg_dir = PROTOCOL__FLEXRAN_DIRECTION__SUCCESSFUL_OUTCOME;
  (*msg)->has_msg_dir = 1;
  (*msg)->hello_msg = hello_msg;
  return 0;
error:

  if(header != NULL)
    free(header);

  if(hello_msg != NULL)
    free(hello_msg);

  if(*msg != NULL)
    free(*msg);

  LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}


int flexran_agent_destroy_hello(Protocol__FlexranMessage *msg) {
  if(msg->msg_case != PROTOCOL__FLEXRAN_MESSAGE__MSG_HELLO_MSG)
    goto error;

  free(msg->hello_msg->header);
  free(msg->hello_msg->capabilities);
  free(msg->hello_msg);
  free(msg);
  return 0;
error:
  LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}


int flexran_agent_echo_request(mid_t mod_id, const void *params, Protocol__FlexranMessage **msg) {
  Protocol__FlexHeader *header = NULL;
  /*TODO: Need to set a random xid*/
  xid_t xid = 1;
  Protocol__FlexEchoRequest *echo_request_msg = NULL;
  echo_request_msg = malloc(sizeof(Protocol__FlexEchoRequest));

  if(echo_request_msg == NULL)
    goto error;

  protocol__flex_echo_request__init(echo_request_msg);

  if (flexran_create_header(xid, PROTOCOL__FLEX_TYPE__FLPT_ECHO_REQUEST, &header) != 0)
    goto error;

  echo_request_msg->header = header;
  *msg = malloc(sizeof(Protocol__FlexranMessage));

  if(*msg == NULL)
    goto error;

  protocol__flexran_message__init(*msg);
  (*msg)->msg_case = PROTOCOL__FLEXRAN_MESSAGE__MSG_ECHO_REQUEST_MSG;
  (*msg)->msg_dir = PROTOCOL__FLEXRAN_DIRECTION__INITIATING_MESSAGE;
  (*msg)->echo_request_msg = echo_request_msg;
  return 0;
error:

  if(header != NULL)
    free(header);

  if(echo_request_msg != NULL)
    free(echo_request_msg);

  if(*msg != NULL)
    free(*msg);

  //LOG_E(MAC, "%s: an error occured\n", __FUNCTION__);
  return -1;
}


int flexran_agent_destroy_echo_request(Protocol__FlexranMessage *msg) {
  if(msg->msg_case != PROTOCOL__FLEXRAN_MESSAGE__MSG_ECHO_REQUEST_MSG)
    goto error;

  free(msg->echo_request_msg->header);
  free(msg->echo_request_msg);
  free(msg);
  return 0;
error:
  LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}



int flexran_agent_echo_reply(mid_t mod_id, const void *params, Protocol__FlexranMessage **msg) {
  xid_t xid;
  Protocol__FlexHeader *header = NULL;
  Protocol__FlexranMessage *input = (Protocol__FlexranMessage *)params;
  Protocol__FlexEchoRequest *echo_req = input->echo_request_msg;
  xid = (echo_req->header)->xid;
  Protocol__FlexEchoReply *echo_reply_msg = NULL;
  echo_reply_msg = malloc(sizeof(Protocol__FlexEchoReply));

  if(echo_reply_msg == NULL)
    goto error;

  protocol__flex_echo_reply__init(echo_reply_msg);

  if (flexran_create_header(xid, PROTOCOL__FLEX_TYPE__FLPT_ECHO_REPLY, &header) != 0)
    goto error;

  echo_reply_msg->header = header;
  *msg = malloc(sizeof(Protocol__FlexranMessage));

  if(*msg == NULL)
    goto error;

  protocol__flexran_message__init(*msg);
  (*msg)->msg_case = PROTOCOL__FLEXRAN_MESSAGE__MSG_ECHO_REPLY_MSG;
  (*msg)->msg_dir = PROTOCOL__FLEXRAN_DIRECTION__SUCCESSFUL_OUTCOME;
  (*msg)->has_msg_dir = 1;
  (*msg)->echo_reply_msg = echo_reply_msg;
  return 0;
error:

  if(header != NULL)
    free(header);

  if(echo_reply_msg != NULL)
    free(echo_reply_msg);

  if(*msg != NULL)
    free(*msg);

  LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}


int flexran_agent_destroy_echo_reply(Protocol__FlexranMessage *msg) {
  if(msg->msg_case != PROTOCOL__FLEXRAN_MESSAGE__MSG_ECHO_REPLY_MSG)
    goto error;

  free(msg->echo_reply_msg->header);
  free(msg->echo_reply_msg);
  free(msg);
  return 0;
error:
  LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}


int flexran_agent_destroy_enb_config_reply(Protocol__FlexranMessage *msg) {
  if(msg->msg_case != PROTOCOL__FLEXRAN_MESSAGE__MSG_ENB_CONFIG_REPLY_MSG)
    goto error;

  free(msg->enb_config_reply_msg->header);
  Protocol__FlexEnbConfigReply *reply = msg->enb_config_reply_msg;

  for (int i = 0; i < reply->n_cell_config; i++) {
    if (reply->cell_config[i]->mbsfn_subframe_config_rfoffset)
      free(reply->cell_config[i]->mbsfn_subframe_config_rfoffset);
    if (reply->cell_config[i]->mbsfn_subframe_config_rfperiod)
      free(reply->cell_config[i]->mbsfn_subframe_config_rfperiod);
    if (reply->cell_config[i]->mbsfn_subframe_config_sfalloc)
      free(reply->cell_config[i]->mbsfn_subframe_config_sfalloc);

    if (reply->cell_config[i]->si_config) {
      for(int j = 0; j < reply->cell_config[i]->si_config->n_si_message; j++) {
        free(reply->cell_config[i]->si_config->si_message[j]);
      }
      free(reply->cell_config[i]->si_config->si_message);
      free(reply->cell_config[i]->si_config);
    }

    if (reply->cell_config[i]->slice_config) {
      for (int j = 0; j < reply->cell_config[i]->slice_config->n_dl; ++j) {
        if (reply->cell_config[i]->slice_config->dl[j]->n_sorting > 0)
          free(reply->cell_config[i]->slice_config->dl[j]->sorting);

        free(reply->cell_config[i]->slice_config->dl[j]->scheduler_name);
        free(reply->cell_config[i]->slice_config->dl[j]);
      }

      free(reply->cell_config[i]->slice_config->dl);
      for (int j = 0; j < reply->cell_config[i]->slice_config->n_ul; ++j) {
        if (reply->cell_config[i]->slice_config->ul[j]->n_sorting > 0)
          free(reply->cell_config[i]->slice_config->ul[j]->sorting);

        free(reply->cell_config[i]->slice_config->ul[j]->scheduler_name);
        free(reply->cell_config[i]->slice_config->ul[j]);
      }

      free(reply->cell_config[i]->slice_config->ul);
      free(reply->cell_config[i]->slice_config);
    }

    free(reply->cell_config[i]);
  }

  free(reply->cell_config);
  free(reply);
  free(msg);
  return 0;
error:
  //LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}

int flexran_agent_destroy_ue_config_reply(Protocol__FlexranMessage *msg) {
  if(msg->msg_case != PROTOCOL__FLEXRAN_MESSAGE__MSG_UE_CONFIG_REPLY_MSG)
    goto error;

  free(msg->ue_config_reply_msg->header);
  int i;
  Protocol__FlexUeConfigReply *reply = msg->ue_config_reply_msg;

  for(i = 0; i < reply->n_ue_config; i++) {
    free(reply->ue_config[i]->capabilities);
    free(reply->ue_config[i]);
  }

  free(reply->ue_config);
  free(reply);
  free(msg);
  return 0;
error:
  //LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}

int flexran_agent_destroy_lc_config_reply(Protocol__FlexranMessage *msg) {
  if(msg->msg_case != PROTOCOL__FLEXRAN_MESSAGE__MSG_LC_CONFIG_REPLY_MSG)
    goto error;

  int i, j;
  free(msg->lc_config_reply_msg->header);

  for (i = 0; i < msg->lc_config_reply_msg->n_lc_ue_config; i++) {
    for (j = 0; j < msg->lc_config_reply_msg->lc_ue_config[i]->n_lc_config; j++) {
      free(msg->lc_config_reply_msg->lc_ue_config[i]->lc_config[j]);
    }

    free(msg->lc_config_reply_msg->lc_ue_config[i]->lc_config);
    free(msg->lc_config_reply_msg->lc_ue_config[i]);
  }

  free(msg->lc_config_reply_msg->lc_ue_config);
  free(msg->lc_config_reply_msg);
  free(msg);
  return 0;
error:
  //LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}


int flexran_agent_destroy_enb_config_request(Protocol__FlexranMessage *msg) {
  if(msg->msg_case != PROTOCOL__FLEXRAN_MESSAGE__MSG_ENB_CONFIG_REQUEST_MSG)
    goto error;

  free(msg->enb_config_request_msg->header);
  free(msg->enb_config_request_msg);
  free(msg);
  return 0;
error:
  //LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}

int flexran_agent_destroy_ue_config_request(Protocol__FlexranMessage *msg) {
  /* TODO: Deallocate memory for a dynamically allocated UE config message */
  return 0;
}

int flexran_agent_destroy_lc_config_request(Protocol__FlexranMessage *msg) {
  /* TODO: Deallocate memory for a dynamically allocated LC config message */
  return 0;
}

// call this function to start a nanosecond-resolution timer
struct timespec timer_start(void) {
  struct timespec start_time;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
  return start_time;
}

// call this function to end a timer, returning nanoseconds elapsed as a long
long timer_end(struct timespec start_time) {
  struct timespec end_time;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
  long diffInNanos = end_time.tv_nsec - start_time.tv_nsec;
  return diffInNanos;
}

int flexran_agent_control_delegation(mid_t mod_id, const void *params, Protocol__FlexranMessage **msg) {
  Protocol__FlexranMessage *input = (Protocol__FlexranMessage *)params;
  Protocol__FlexControlDelegation *control_delegation_msg = input->control_delegation_msg;
  //  struct timespec vartime = timer_start();
  //Write the payload lib into a file in the cache and load the lib
  char lib_name[120];
  char target[512];
  snprintf(lib_name, sizeof(lib_name), "/%s.so", control_delegation_msg->name);
  strcpy(target, RC.flexran[mod_id]->cache_name);
  strcat(target, lib_name);
  FILE *f;
  f = fopen(target, "wb");

  if (f) {
    fwrite(control_delegation_msg->payload.data, control_delegation_msg->payload.len, 1, f);
    fclose(f);
  } else {
    LOG_W(FLEXRAN_AGENT, "[%d] can not write control delegation data to %s\n",
          mod_id, target);
  }

  //  long time_elapsed_nanos = timer_end(vartime);
  *msg = NULL;
  return 0;
}

int flexran_agent_destroy_control_delegation(Protocol__FlexranMessage *msg) {
  /*TODO: Dealocate memory for a dynamically allocated control delegation message*/
  return 0;
}

int flexran_agent_reconfiguration(mid_t mod_id, const void *params, Protocol__FlexranMessage **msg) {
  Protocol__FlexranMessage *input = (Protocol__FlexranMessage *)params;
  Protocol__FlexAgentReconfiguration *agent_reconfiguration_msg = input->agent_reconfiguration_msg;
  apply_reconfiguration_policy(mod_id, agent_reconfiguration_msg->policy, strlen(agent_reconfiguration_msg->policy));
  *msg = NULL;
  return 0;
}

int flexran_agent_destroy_agent_reconfiguration(Protocol__FlexranMessage *msg) {
  /*TODO: Dealocate memory for a dynamically allocated agent reconfiguration message*/
  return 0;
}


int flexran_agent_lc_config_reply(mid_t mod_id, const void *params, Protocol__FlexranMessage **msg) {
  xid_t xid;
  Protocol__FlexHeader *header = NULL;
  Protocol__FlexranMessage *input = (Protocol__FlexranMessage *)params;
  Protocol__FlexLcConfigRequest *lc_config_request_msg = input->lc_config_request_msg;
  xid = (lc_config_request_msg->header)->xid;
  Protocol__FlexLcConfigReply *lc_config_reply_msg;
  lc_config_reply_msg = malloc(sizeof(Protocol__FlexLcConfigReply));

  if(lc_config_reply_msg == NULL)
    goto error;

  protocol__flex_lc_config_reply__init(lc_config_reply_msg);

  if(flexran_create_header(xid, PROTOCOL__FLEX_TYPE__FLPT_GET_LC_CONFIG_REPLY, &header) != 0)
    goto error;

  lc_config_reply_msg->header = header;

  /* the lc_config_reply entirely depends on MAC except for the
   * mac_eNB_get_rrc_status() function (which in the current OAI implementation
   * is reachable if F1 is present). Therefore we check here wether MAC CM is
   * present and the message gets properly filled if it is or remains empty if
   * not */
  lc_config_reply_msg->n_lc_ue_config =
      flexran_agent_get_mac_xface(mod_id) ? flexran_get_mac_num_ues(mod_id) : 0;

  Protocol__FlexLcUeConfig **lc_ue_config = NULL;
  if (lc_config_reply_msg->n_lc_ue_config > 0) {
    lc_ue_config = malloc(sizeof(Protocol__FlexLcUeConfig *) * lc_config_reply_msg->n_lc_ue_config);

    if (lc_ue_config == NULL) {
      goto error;
    }

    // Fill the config for each UE
    for (int i = 0; i < lc_config_reply_msg->n_lc_ue_config; i++) {
      lc_ue_config[i] = malloc(sizeof(Protocol__FlexLcUeConfig));
      if (!lc_ue_config[i]) goto error;

      protocol__flex_lc_ue_config__init(lc_ue_config[i]);
      const int UE_id = flexran_get_mac_ue_id(mod_id, i);
      flexran_agent_fill_mac_lc_ue_config(mod_id, UE_id, lc_ue_config[i]);
    } // end for UE

    lc_config_reply_msg->lc_ue_config = lc_ue_config;
  } // lc_config_reply_msg->n_lc_ue_config > 0

  *msg = malloc(sizeof(Protocol__FlexranMessage));

  if (*msg == NULL)
    goto error;

  protocol__flexran_message__init(*msg);
  (*msg)->msg_case = PROTOCOL__FLEXRAN_MESSAGE__MSG_LC_CONFIG_REPLY_MSG;
  (*msg)->msg_dir = PROTOCOL__FLEXRAN_DIRECTION__SUCCESSFUL_OUTCOME;
  (*msg)->lc_config_reply_msg = lc_config_reply_msg;
  return 0;
error:

  // TODO: Need to make proper error handling
  if (header != NULL)
    free(header);

  if (lc_config_reply_msg != NULL)
    free(lc_config_reply_msg);

  if(*msg != NULL)
    free(*msg);

  //LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}

/*
 * ************************************
 * UE Configuration Reply
 * ************************************
 */

int sort_ue_config(const void *a, const void *b)
{
  const Protocol__FlexUeConfig *fa = a;
  const Protocol__FlexUeConfig *fb = b;

  if (fa->rnti < fb->rnti)
    return -1;
  else if (fa->rnti < fb->rnti)
    return 1;
  return 0;
}

int flexran_agent_ue_config_reply(mid_t mod_id, const void *params, Protocol__FlexranMessage **msg) {
  xid_t xid;
  Protocol__FlexHeader *header = NULL;
  Protocol__FlexranMessage *input = (Protocol__FlexranMessage *)params;
  Protocol__FlexUeConfigRequest *ue_config_request_msg = input->ue_config_request_msg;
  xid = (ue_config_request_msg->header)->xid;
  Protocol__FlexUeConfigReply *ue_config_reply_msg;
  ue_config_reply_msg = malloc(sizeof(Protocol__FlexUeConfigReply));

  if(ue_config_reply_msg == NULL)
    goto error;

  protocol__flex_ue_config_reply__init(ue_config_reply_msg);

  if(flexran_create_header(xid, PROTOCOL__FLEX_TYPE__FLPT_GET_UE_CONFIG_REPLY, &header) != 0)
    goto error;

  ue_config_reply_msg->header = header;

  ue_config_reply_msg->n_ue_config = 0;
  if (flexran_agent_get_rrc_xface(mod_id))
    ue_config_reply_msg->n_ue_config = flexran_get_rrc_num_ues(mod_id);
  else if (flexran_agent_get_mac_xface(mod_id))
    ue_config_reply_msg->n_ue_config = flexran_get_mac_num_ues(mod_id);

  if (flexran_agent_get_rrc_xface(mod_id) && flexran_agent_get_mac_xface(mod_id)
      && flexran_get_rrc_num_ues(mod_id) != flexran_get_mac_num_ues(mod_id)) {
    const int nrrc = flexran_get_rrc_num_ues(mod_id);
    const int nmac = flexran_get_mac_num_ues(mod_id);
    ue_config_reply_msg->n_ue_config = nrrc < nmac ? nrrc : nmac;
    LOG_E(FLEXRAN_AGENT, "%s(): different numbers of UEs in RRC (%d) and MAC (%d), reporting for %lu UEs\n",
        __func__, nrrc, nmac, ue_config_reply_msg->n_ue_config);
  }

  Protocol__FlexUeConfig **ue_config;

  if (ue_config_reply_msg->n_ue_config > 0) {
    ue_config = malloc(sizeof(Protocol__FlexUeConfig *) * ue_config_reply_msg->n_ue_config);

    if (ue_config == NULL) {
      goto error;
    }
    rnti_t rntis[ue_config_reply_msg->n_ue_config];
    flexran_get_rrc_rnti_list(mod_id, rntis, ue_config_reply_msg->n_ue_config);
    for (int i = 0; i < ue_config_reply_msg->n_ue_config; i++) {
      const rnti_t rnti = rntis[i];
      ue_config[i] = malloc(sizeof(Protocol__FlexUeConfig));
      protocol__flex_ue_config__init(ue_config[i]);

      if (flexran_agent_get_rrc_xface(mod_id))
        flexran_agent_fill_rrc_ue_config(mod_id, rnti, ue_config[i]);
      if (flexran_agent_get_mac_xface(mod_id)) {
        const int UE_id = flexran_get_mac_ue_id_rnti(mod_id, rnti);
        flexran_agent_fill_mac_ue_config(mod_id, UE_id, ue_config[i]);
      }
    }
    ue_config_reply_msg->ue_config = ue_config;
  }

  *msg = malloc(sizeof(Protocol__FlexranMessage));

  if (*msg == NULL)
    goto error;

  protocol__flexran_message__init(*msg);
  (*msg)->msg_case = PROTOCOL__FLEXRAN_MESSAGE__MSG_UE_CONFIG_REPLY_MSG;
  (*msg)->msg_dir = PROTOCOL__FLEXRAN_DIRECTION__SUCCESSFUL_OUTCOME;
  (*msg)->ue_config_reply_msg = ue_config_reply_msg;
  return 0;
error:

  // TODO: Need to make proper error handling
  if (header != NULL)
    free(header);

  if (ue_config_reply_msg != NULL)
    free(ue_config_reply_msg);

  if(*msg != NULL)
    free(*msg);

  //LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}


/*
 * ************************************
 * eNB Configuration Request and Reply
 * ************************************
 */

int flexran_agent_enb_config_request(mid_t mod_id, const void *params, Protocol__FlexranMessage **msg) {
  Protocol__FlexHeader *header = NULL;
  xid_t xid = 1;
  Protocol__FlexEnbConfigRequest *enb_config_request_msg;
  enb_config_request_msg = malloc(sizeof(Protocol__FlexEnbConfigRequest));

  if(enb_config_request_msg == NULL)
    goto error;

  protocol__flex_enb_config_request__init(enb_config_request_msg);

  if(flexran_create_header(xid,PROTOCOL__FLEX_TYPE__FLPT_GET_ENB_CONFIG_REQUEST, &header) != 0)
    goto error;

  enb_config_request_msg->header = header;
  *msg = malloc(sizeof(Protocol__FlexranMessage));

  if(*msg == NULL)
    goto error;

  protocol__flexran_message__init(*msg);
  (*msg)->msg_case = PROTOCOL__FLEXRAN_MESSAGE__MSG_ENB_CONFIG_REQUEST_MSG;
  (*msg)->msg_dir = PROTOCOL__FLEXRAN_DIRECTION__INITIATING_MESSAGE;
  (*msg)->enb_config_request_msg = enb_config_request_msg;
  return 0;
error:

  // TODO: Need to make proper error handling
  if (header != NULL)
    free(header);

  if (enb_config_request_msg != NULL)
    free(enb_config_request_msg);

  if(*msg != NULL)
    free(*msg);

  //LOG_E(MAC, "%s: an error occured\n", __FUNCTION__);
  return -1;
}

int flexran_agent_enb_config_reply(mid_t mod_id, const void *params, Protocol__FlexranMessage **msg) {
  xid_t xid;
  Protocol__FlexHeader *header = NULL;
  Protocol__FlexranMessage *input = (Protocol__FlexranMessage *)params;
  Protocol__FlexEnbConfigRequest *enb_config_req_msg = input->enb_config_request_msg;
  xid = (enb_config_req_msg->header)->xid;
  Protocol__FlexEnbConfigReply *enb_config_reply_msg;
  enb_config_reply_msg = malloc(sizeof(Protocol__FlexEnbConfigReply));

  if(enb_config_reply_msg == NULL)
    goto error;

  protocol__flex_enb_config_reply__init(enb_config_reply_msg);

  if(flexran_create_header(xid, PROTOCOL__FLEX_TYPE__FLPT_GET_ENB_CONFIG_REPLY, &header) != 0)
    goto error;

  enb_config_reply_msg->header = header;
  enb_config_reply_msg->n_cell_config = MAX_NUM_CCs;
  Protocol__FlexCellConfig **cell_conf;

  if(enb_config_reply_msg->n_cell_config > 0) {
    cell_conf = malloc(sizeof(Protocol__FlexCellConfig *) * enb_config_reply_msg->n_cell_config);

    if(cell_conf == NULL)
      goto error;
    for(int i = 0; i < enb_config_reply_msg->n_cell_config; i++){
      cell_conf[i] = malloc(sizeof(Protocol__FlexCellConfig));
      if (!cell_conf[i]) goto error;
      protocol__flex_cell_config__init(cell_conf[i]);
      if (flexran_agent_get_phy_xface(mod_id))
        flexran_agent_fill_phy_cell_config(mod_id, i, cell_conf[i]);
      if (flexran_agent_get_rrc_xface(mod_id))
        flexran_agent_fill_rrc_cell_config(mod_id, i, cell_conf[i]);
      if (flexran_agent_get_mac_xface(mod_id))
        flexran_agent_fill_mac_cell_config(mod_id, i, cell_conf[i]);

      cell_conf[i]->carrier_index = i;
      cell_conf[i]->has_carrier_index = 1;
    }

    enb_config_reply_msg->cell_config=cell_conf;
  }

  *msg = malloc(sizeof(Protocol__FlexranMessage));

  if(*msg == NULL)
    goto error;

  protocol__flexran_message__init(*msg);
  (*msg)->msg_case = PROTOCOL__FLEXRAN_MESSAGE__MSG_ENB_CONFIG_REPLY_MSG;
  (*msg)->msg_dir = PROTOCOL__FLEXRAN_DIRECTION__SUCCESSFUL_OUTCOME;
  (*msg)->enb_config_reply_msg = enb_config_reply_msg;
  return 0;
error:

  // TODO: Need to make proper error handling
  if (header != NULL)
    free(header);

  if (enb_config_reply_msg != NULL)
    free(enb_config_reply_msg);

  if(*msg != NULL)
    free(*msg);

  //LOG_E(FLEXRAN_AGENT, "%s: an error occured\n", __FUNCTION__);
  return -1;
}


int flexran_agent_rrc_measurement(mid_t mod_id, const void *params, Protocol__FlexranMessage **msg) {
  protocol_ctxt_t  ctxt;
  Protocol__FlexranMessage *input = (Protocol__FlexranMessage *)params;
  Protocol__FlexRrcTriggering *triggering = input->rrc_triggering;
  agent_reconf_rrc *reconf_param = malloc(sizeof(agent_reconf_rrc));
  reconf_param->trigger_policy = triggering->rrc_trigger;
  struct rrc_eNB_ue_context_s   *ue_context_p = NULL;
  RB_FOREACH(ue_context_p, rrc_ue_tree_s, &(RC.rrc[mod_id]->rrc_ue_head)) {
    PROTOCOL_CTXT_SET_BY_MODULE_ID(&ctxt, mod_id, ENB_FLAG_YES, ue_context_p->ue_context.rnti, flexran_get_current_frame(mod_id), flexran_get_current_subframe (mod_id), mod_id);
    flexran_rrc_eNB_generate_defaultRRCConnectionReconfiguration(&ctxt, ue_context_p, 0, reconf_param);
  }
  *msg = NULL;
  return 0;
}


int flexran_agent_destroy_rrc_measurement(Protocol__FlexranMessage *msg) {
  // TODO
  return 0;
}

int flexran_agent_handle_enb_config_reply(mid_t mod_id, const void *params, Protocol__FlexranMessage **msg) {
  Protocol__FlexranMessage *input = (Protocol__FlexranMessage *)params;
  Protocol__FlexEnbConfigReply *enb_config = input->enb_config_reply_msg;

  if (enb_config->n_cell_config == 0) {
    LOG_W(FLEXRAN_AGENT,
          "received enb_config_reply message does not contain a cell_config\n");
    *msg = NULL;
    return 0;
  }

  if (enb_config->n_cell_config > 1)
    LOG_W(FLEXRAN_AGENT, "ignoring slice configs for other cell except cell 0\n");
  if (flexran_agent_get_mac_xface(mod_id) && enb_config->cell_config[0]->slice_config) {
    prepare_update_slice_config(mod_id, enb_config->cell_config[0]->slice_config);
  //} else {
  //  initiate_soft_restart(mod_id, enb_config->cell_config[0]);
  }

  *msg = NULL;
  return 0;
}

int flexran_agent_handle_ue_config_reply(mid_t mod_id, const void *params, Protocol__FlexranMessage **msg) {
  int i;
  Protocol__FlexranMessage *input = (Protocol__FlexranMessage *)params;
  Protocol__FlexUeConfigReply *ue_config_reply = input->ue_config_reply_msg;

  for (i = 0; flexran_agent_get_mac_xface(mod_id) && i < ue_config_reply->n_ue_config; i++)
    prepare_ue_slice_assoc_update(mod_id, ue_config_reply->ue_config[i]);

  *msg = NULL;
  return 0;
}
