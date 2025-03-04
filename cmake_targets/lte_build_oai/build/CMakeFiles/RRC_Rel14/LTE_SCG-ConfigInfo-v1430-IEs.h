/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-InterNodeDefinitions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/RRC/LTE/MESSAGES/asn1c/ASN1_files/lte-rrc-14.7.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/RRC_Rel14`
 */

#ifndef	_LTE_SCG_ConfigInfo_v1430_IEs_H_
#define	_LTE_SCG_ConfigInfo_v1430_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum LTE_SCG_ConfigInfo_v1430_IEs__makeBeforeBreakSCG_Req_r14 {
	LTE_SCG_ConfigInfo_v1430_IEs__makeBeforeBreakSCG_Req_r14_true	= 0
} e_LTE_SCG_ConfigInfo_v1430_IEs__makeBeforeBreakSCG_Req_r14;

/* Forward declarations */
struct LTE_MeasGapConfigPerCC_List_r14;

/* LTE_SCG-ConfigInfo-v1430-IEs */
typedef struct LTE_SCG_ConfigInfo_v1430_IEs {
	long	*makeBeforeBreakSCG_Req_r14;	/* OPTIONAL */
	struct LTE_MeasGapConfigPerCC_List_r14	*measGapConfigPerCC_List;	/* OPTIONAL */
	struct LTE_SCG_ConfigInfo_v1430_IEs__nonCriticalExtension {
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *nonCriticalExtension;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LTE_SCG_ConfigInfo_v1430_IEs_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_LTE_makeBeforeBreakSCG_Req_r14_2;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_LTE_SCG_ConfigInfo_v1430_IEs;
extern asn_SEQUENCE_specifics_t asn_SPC_LTE_SCG_ConfigInfo_v1430_IEs_specs_1;
extern asn_TYPE_member_t asn_MBR_LTE_SCG_ConfigInfo_v1430_IEs_1[3];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "LTE_MeasGapConfigPerCC-List-r14.h"

#endif	/* _LTE_SCG_ConfigInfo_v1430_IEs_H_ */
#include <asn_internal.h>
