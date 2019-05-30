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

#include <stdio.h>
#include <pthread.h>
#include "nfapi/oai_integration/vendor_ext.h"
#include "common/utils/LOG/log.h"
static char nfapi_str_mode[6][24] = {"MONOLITHIC","PNF","VNF","UE_STUB_PNF","UE_STUB_OFFNET","<UNKNOWN NFAPI MODE>"};

typedef struct {
  nfapi_mode_t nfapi_mode;
} nfapi_params_t;

static nfapi_params_t nfapi_params;

void set_thread_priority(int priority) {
  //printf("%s(priority:%d)\n", __FUNCTION__, priority);
  pthread_attr_t ptAttr;
  struct sched_param schedParam;
  schedParam.__sched_priority = priority; //79;

  if(sched_setscheduler(0, SCHED_RR, &schedParam) != 0) {
    printf("Failed to set scheduler to SCHED_RR\n");
  }

  if(pthread_attr_setschedpolicy(&ptAttr, SCHED_RR) != 0) {
    printf("Failed to set pthread sched policy SCHED_RR\n");
  }

  pthread_attr_setinheritsched(&ptAttr, PTHREAD_EXPLICIT_SCHED);
  struct sched_param thread_params;
  thread_params.sched_priority = 20;

  if(pthread_attr_setschedparam(&ptAttr, &thread_params) != 0) {
    printf("failed to set sched param\n");
  }
}

char *nfapi_get_strmode(void) {
  if (nfapi_params.nfapi_mode > NFAPI_MODE_UNKNOWN)
    return nfapi_str_mode[NFAPI_MODE_UNKNOWN];

  return nfapi_str_mode[nfapi_params.nfapi_mode];
}

void nfapi_logmode() {
  LOG_I(ENB_APP,"nfapi running mode: %s\n",nfapi_get_strmode());
}

nfapi_mode_t nfapi_getmode(void) {
  return nfapi_params.nfapi_mode;
}

void nfapi_setmode(nfapi_mode_t nfapi_mode) {
  nfapi_params.nfapi_mode = nfapi_mode;
  nfapi_logmode();
}
