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

#ifndef GTPV1_U_MESSAGES_TYPES_H_
#define GTPV1_U_MESSAGES_TYPES_H_

#include "LTE_asn_constant.h"


#define GTPV1U_MAX_BEARERS_PER_UE max_val_LTE_DRB_Identity

#define GTPV1U_ENB_UPDATE_TUNNEL_REQ(mSGpTR)  (mSGpTR)->ittiMsg.Gtpv1uUpdateTunnelReq
#define GTPV1U_ENB_UPDATE_TUNNEL_RESP(mSGpTR) (mSGpTR)->ittiMsg.Gtpv1uUpdateTunnelResp
#define GTPV1U_ENB_DELETE_TUNNEL_REQ(mSGpTR)  (mSGpTR)->ittiMsg.Gtpv1uDeleteTunnelReq
#define GTPV1U_ENB_DELETE_TUNNEL_RESP(mSGpTR) (mSGpTR)->ittiMsg.Gtpv1uDeleteTunnelResp
#define GTPV1U_ENB_TUNNEL_DATA_IND(mSGpTR)    (mSGpTR)->ittiMsg.Gtpv1uTunnelDataInd
#define GTPV1U_ENB_TUNNEL_DATA_REQ(mSGpTR)    (mSGpTR)->ittiMsg.Gtpv1uTunnelDataReq
#define GTPV1U_ENB_S1_REQ(mSGpTR)    (mSGpTR)->ittiMsg.gtpv1uS1Req

#define GTPV1U_ALL_TUNNELS_TEID (teid_t)0xFFFFFFFF

typedef struct gtpv1u_enb_create_tunnel_req_s {
  rnti_t                 rnti;
  int                    num_tunnels;
  teid_t                 sgw_S1u_teid[GTPV1U_MAX_BEARERS_PER_UE];  ///< Tunnel Endpoint Identifier
  ebi_t                  eps_bearer_id[GTPV1U_MAX_BEARERS_PER_UE];
  transport_layer_addr_t sgw_addr[GTPV1U_MAX_BEARERS_PER_UE];
} gtpv1u_enb_create_tunnel_req_t;

typedef struct gtpv1u_enb_create_tunnel_resp_s {
  uint8_t                status;               ///< Status of S1U endpoint creation (Failed = 0xFF or Success = 0x0)
  rnti_t                 rnti;
  int                    num_tunnels;
  teid_t                 enb_S1u_teid[GTPV1U_MAX_BEARERS_PER_UE];  ///< Tunnel Endpoint Identifier
  ebi_t                  eps_bearer_id[GTPV1U_MAX_BEARERS_PER_UE];
  transport_layer_addr_t enb_addr;
} gtpv1u_enb_create_tunnel_resp_t;

typedef struct gtpv1u_enb_update_tunnel_req_s {
  rnti_t                 rnti;
  teid_t                 enb_S1u_teid;         ///< eNB S1U Tunnel Endpoint Identifier
  teid_t                 sgw_S1u_teid;         ///< SGW S1U local Tunnel Endpoint Identifier
  transport_layer_addr_t sgw_addr;
  ebi_t                  eps_bearer_id;
} gtpv1u_enb_update_tunnel_req_t;

typedef struct gtpv1u_enb_update_tunnel_resp_s {
  rnti_t                 rnti;
  uint8_t                status;               ///< Status (Failed = 0xFF or Success = 0x0)
  teid_t                 enb_S1u_teid;         ///< eNB S1U Tunnel Endpoint Identifier
  teid_t                 sgw_S1u_teid;         ///< SGW S1U local Tunnel Endpoint Identifier
  ebi_t                  eps_bearer_id;
} gtpv1u_enb_update_tunnel_resp_t;

typedef struct gtpv1u_enb_delete_tunnel_req_s {
  rnti_t                 rnti;
  uint8_t                num_erab;
  ebi_t                  eps_bearer_id[GTPV1U_MAX_BEARERS_PER_UE];
  //teid_t                 enb_S1u_teid;         ///< local SGW S11 Tunnel Endpoint Identifier
} gtpv1u_enb_delete_tunnel_req_t;

typedef struct gtpv1u_enb_delete_tunnel_resp_s {
  rnti_t                 rnti;
  uint8_t                status;               ///< Status of S1U endpoint deleteion (Failed = 0xFF or Success = 0x0)
  teid_t                 enb_S1u_teid;         ///< local S1U Tunnel Endpoint Identifier to be deleted
} gtpv1u_enb_delete_tunnel_resp_t;

typedef struct gtpv1u_enb_tunnel_data_ind_s {
  rnti_t                 rnti;
  uint8_t               *buffer;
  uint32_t               length;
  uint32_t               offset;               ///< start of message offset in buffer
  teid_t                 enb_S1u_teid;         ///< Tunnel Endpoint Identifier
} gtpv1u_enb_tunnel_data_ind_t;

typedef struct gtpv1u_enb_tunnel_data_req_s {
  uint8_t               *buffer;
  uint32_t               length;
  uint32_t               offset;               ///< start of message offset in buffer
  rnti_t                 rnti;
  rb_id_t                rab_id;
} gtpv1u_enb_tunnel_data_req_t;

typedef struct {
  in_addr_t             enb_ip_address_for_S1u_S12_S4_up;
  tcp_udp_port_t        enb_port_for_S1u_S12_S4_up;
} Gtpv1uS1Req;
#endif /* GTPV1_U_MESSAGES_TYPES_H_ */
