/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/RRC/LTE/MESSAGES/asn1c/ASN1_files/lte-rrc-14.7.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/RRC_Rel14`
 */

#ifndef	_LTE_PUSCH_ConfigDedicatedSCell_v1430_H_
#define	_LTE_PUSCH_ConfigDedicatedSCell_v1430_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct LTE_Enable256QAM_r14;

/* LTE_PUSCH-ConfigDedicatedSCell-v1430 */
typedef struct LTE_PUSCH_ConfigDedicatedSCell_v1430 {
	struct LTE_Enable256QAM_r14	*enable256QAM_r14;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LTE_PUSCH_ConfigDedicatedSCell_v1430_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LTE_PUSCH_ConfigDedicatedSCell_v1430;
extern asn_SEQUENCE_specifics_t asn_SPC_LTE_PUSCH_ConfigDedicatedSCell_v1430_specs_1;
extern asn_TYPE_member_t asn_MBR_LTE_PUSCH_ConfigDedicatedSCell_v1430_1[1];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "LTE_Enable256QAM-r14.h"

#endif	/* _LTE_PUSCH_ConfigDedicatedSCell_v1430_H_ */
#include <asn_internal.h>
