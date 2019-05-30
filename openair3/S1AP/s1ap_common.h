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

/** @defgroup _s1ap_impl_ S1AP Layer Reference Implementation
 * @ingroup _ref_implementation_
 * @{
 */

#if HAVE_CONFIG_H_
# include "config.h"
#endif

#ifndef S1AP_COMMON_H_
#define S1AP_COMMON_H_


#include "common/utils/LOG/log.h"
/* replace ASN_DEBUG defined in asn_internal.h by oai tracing system
   Would be cleaner to modify asn_internal.h but it seems to come
   from non oai source, with BSD license, so prefer to do that here..
*/
#ifdef ASN_DEBUG
# undef ASN_DEBUG
#endif
#define ASN_DEBUG( x... )  LOG_I(ASN, x)

#include "S1AP_ProtocolIE-Field.h"
#include "S1AP_S1AP-PDU.h"
#include "S1AP_InitiatingMessage.h"
#include "S1AP_SuccessfulOutcome.h"
#include "S1AP_UnsuccessfulOutcome.h"
#include "S1AP_ProtocolIE-Field.h"
#include "S1AP_ProtocolIE-FieldPair.h"
#include "S1AP_ProtocolIE-ContainerPair.h"
#include "S1AP_ProtocolExtensionField.h"
#include "S1AP_ProtocolExtensionContainer.h"
#include "S1AP_asn_constant.h"
#include "S1AP_SupportedTAs-Item.h"
#include "S1AP_ServedGUMMEIsItem.h"

/* Checking version of ASN1C compiler */
#if (ASN1C_ENVIRONMENT_VERSION < ASN1C_MINIMUM_VERSION)
# error "You are compiling s1ap with the wrong version of ASN1C"
#endif

#ifndef FALSE
# define FALSE (0)
#endif
#ifndef TRUE
# define TRUE  (!FALSE)
#endif

#define S1AP_UE_ID_FMT  "0x%06"PRIX32

extern int asn_debug;
extern int asn1_xer_print;

#if defined(ENB_MODE)
# include "common/utils/LOG/log.h"
# include "s1ap_eNB_default_values.h"
# define S1AP_ERROR(x, args...) LOG_E(S1AP, x, ##args)
# define S1AP_WARN(x, args...)  LOG_W(S1AP, x, ##args)
# define S1AP_TRAF(x, args...)  LOG_I(S1AP, x, ##args)
# define S1AP_INFO(x, args...) LOG_I(S1AP, x, ##args)
# define S1AP_DEBUG(x, args...) LOG_I(S1AP, x, ##args)
#else
# include "mme_default_values.h"
# define S1AP_ERROR(x, args...) do { fprintf(stdout, "[S1AP][E]"x, ##args); } while(0)
# define S1AP_WARN(x, args...)  do { fprintf(stdout, "[S1AP][W]"x, ##args); } while(0)
# define S1AP_TRAF(x, args...)  do { fprintf(stdout, "[S1AP][T]"x, ##args); } while(0)
# define S1AP_INFO(x, args...) do { fprintf(stdout, "[S1AP][I]"x, ##args); } while(0)
# define S1AP_DEBUG(x, args...) do { fprintf(stdout, "[S1AP][D]"x, ##args); } while(0)
#endif


#define S1AP_FIND_PROTOCOLIE_BY_ID(IE_TYPE, ie, container, IE_ID, mandatory) \
  do {\
    IE_TYPE **ptr; \
    ie = NULL; \
    for (ptr = container->protocolIEs.list.array; \
         ptr < &container->protocolIEs.list.array[container->protocolIEs.list.count]; \
         ptr++) { \
      if((*ptr)->id == IE_ID) { \
        ie = *ptr; \
        break; \
      } \
    } \
    if (ie == NULL ) { \
      S1AP_ERROR("S1AP_FIND_PROTOCOLIE_BY_ID: %s %d: ie is NULL\n",__FILE__,__LINE__);\
    } \
    if (mandatory)  DevAssert(ie != NULL); \
  } while(0)
/** \brief Function callback prototype.
 **/
typedef int (*s1ap_message_decoded_callback)(
    uint32_t         assoc_id,
    uint32_t         stream,
    S1AP_S1AP_PDU_t *pdu
);

/** \brief Handle criticality
 \param criticality Criticality of the IE
 @returns void
 **/
void s1ap_handle_criticality(S1AP_Criticality_t criticality);

#endif /* S1AP_COMMON_H_ */
