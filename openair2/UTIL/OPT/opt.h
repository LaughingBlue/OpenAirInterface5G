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

/*____________________________OPT/opt.h___________________________
Authors:  Navid NIKAIEN
Company: EURECOM
Emails:
*This file include all defined structures & function headers of this module
This header file must be included */
/**
 * Include bloc
 * */

#ifndef OPT_H_
#define OPT_H_

#ifndef sys_include
  #define sys_include
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <errno.h>
  #include <unistd.h>
  #include <time.h>
#endif
#ifndef project_include
  #define project_include
  #include "common/utils/LOG/log_if.h"
  #include "PHY/impl_defs_lte.h"
#endif

#define PACKET_MAC_LTE_DEFAULT_UDP_PORT (9999)

typedef uint8_t  guint8;
typedef uint16_t guint16;
typedef uint32_t guint32;
typedef guint8   gboolean;

#include "packet-mac-lte.h"
#include "mac_pcap.h"

/* OPT parameters definitions */
#define OPT_CONFIGPREFIX "opt"

#define CONFIG_HLP_TYPEMON       "Type of L2 monitoring messages: none,pcap,wireshark  \n"
#define CONFIG_HLP_L2MONIP       "ip address for wireshark messages \n"
#define CONFIG_HLP_L2MONPATH     "file path for pcap  messages on localhost \n"
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*                                            command line parameters for LOG utility                                                                                    */
/*   optname                     helpstr                paramflags                      XXXptr                  defXXXval                            type       numelt   */
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define OPT_PARAMS_DESC {  \
    {"type" ,                    CONFIG_HLP_TYPEMON,     0,                      strptr:&in_type,                  defstrval:"none",               TYPE_STRING,    0},       \
    {"ip" ,                      CONFIG_HLP_L2MONIP,     0,                      strptr:&in_ip,                    defstrval:"127.0.0.1",          TYPE_STRING,    0},       \
    {"path" ,                    CONFIG_HLP_L2MONPATH,   0,                      strptr:&in_path,                  defstrval:"/tmp/oai_opt.pcap",  TYPE_STRING,    0},       \
  }

#define OPTTYPE_IDX 0
/* check function for opt parameters */
#define OPTTYPE_OKSTRINGS {"none","pcap","wireshark"}
#define OPTTYPE_VALUES    {OPT_NONE,OPT_PCAP,OPT_WIRESHARK}
#define OPTPARAMS_CHECK_DESC { \
    { .s3a= { config_checkstr_assign_integer,  OPTTYPE_OKSTRINGS,OPTTYPE_VALUES ,3}} ,\
    { .s5= {NULL }} ,                   \
    { .s5= {NULL }} ,                   \
  }

#ifdef OCP_FRAMEWORK
#include <enums.h>
#else
typedef enum trace_mode_e {
  OPT_WIRESHARK,
  OPT_PCAP,
  OPT_TSHARK,
  OPT_NONE
} trace_mode_t;
#endif

typedef enum radio_type_e {
  RADIO_TYPE_FDD = 1,
  RADIO_TYPE_TDD = 2,
  RADIO_TYPE_MAX
} radio_type_t;



/**
 * function def
*/

void trace_pdu(int direction, uint8_t *pdu_buffer, unsigned int pdu_buffer_size,
               int ueid, int rntiType, int rnti, uint16_t sysFrame, uint8_t subframe,
               int oob_event, int oob_event_value);

int init_opt(void);

void terminate_opt(void);

extern int opt_enabled;
//double *timing_analyzer(int index, int direction );

#endif /* OPT_H_ */
