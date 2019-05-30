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

openair0_device openair0;
volatile int oai_exit=0;

void exit_function(const char* file, const char* function, const int line,const char *s) {
   const char * msg= s==NULL ? "no comment": s;
   printf("Exiting at: %s:%d %s(), %s\n", file, line, function, msg);
   exit(-1);
}

extern unsigned int dlsch_tbs25[27][25],TBStable[27][110];
extern unsigned char offset_mumimo_llr_drange_fix;

extern unsigned short dftsizes[34];
extern short *ul_ref_sigs[30][2][34];
