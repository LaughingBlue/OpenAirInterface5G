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

#ifndef F1AP_CU_TASK_H_
#define F1AP_CU_TASK_H_

void cu_task_handle_sctp_association_ind(instance_t instance, sctp_new_association_ind_t *sctp_new_association_ind);
void cu_task_handle_sctp_association_resp(instance_t instance, sctp_new_association_resp_t *sctp_new_association_resp);
void cu_task_handle_sctp_data_ind(instance_t instance, sctp_data_ind_t *sctp_data_ind);
void cu_task_send_sctp_init_req(instance_t enb_id);

void *F1AP_CU_task(void *arg);

#endif /* F1AP_CU_TASK_H_ */
