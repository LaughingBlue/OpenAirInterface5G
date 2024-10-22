/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/RRC/LTE/MESSAGES/asn1c/ASN1_files/lte-rrc-14.7.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/RRC_Rel14`
 */

#ifndef	_LTE_RadioResourceConfigDedicatedSCG_r12_H_
#define	_LTE_RadioResourceConfigDedicatedSCG_r12_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct LTE_DRB_ToAddModListSCG_r12;
struct LTE_MAC_MainConfig;
struct LTE_RLF_TimersAndConstantsSCG_r12;

/* LTE_RadioResourceConfigDedicatedSCG-r12 */
typedef struct LTE_RadioResourceConfigDedicatedSCG_r12 {
	struct LTE_DRB_ToAddModListSCG_r12	*drb_ToAddModListSCG_r12;	/* OPTIONAL */
	struct LTE_MAC_MainConfig	*mac_MainConfigSCG_r12;	/* OPTIONAL */
	struct LTE_RLF_TimersAndConstantsSCG_r12	*rlf_TimersAndConstantsSCG_r12;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LTE_RadioResourceConfigDedicatedSCG_r12_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LTE_RadioResourceConfigDedicatedSCG_r12;
extern asn_SEQUENCE_specifics_t asn_SPC_LTE_RadioResourceConfigDedicatedSCG_r12_specs_1;
extern asn_TYPE_member_t asn_MBR_LTE_RadioResourceConfigDedicatedSCG_r12_1[3];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "LTE_DRB-ToAddModListSCG-r12.h"
#include "LTE_MAC-MainConfig.h"
#include "LTE_RLF-TimersAndConstantsSCG-r12.h"

#endif	/* _LTE_RadioResourceConfigDedicatedSCG_r12_H_ */
#include <asn_internal.h>
