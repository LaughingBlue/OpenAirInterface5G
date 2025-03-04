/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-InterNodeDefinitions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/RRC/LTE/MESSAGES/asn1c/ASN1_files/lte-rrc-14.7.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/RRC_Rel14`
 */

#ifndef	_LTE_HandoverPreparationInformation_v1250_IEs_H_
#define	_LTE_HandoverPreparationInformation_v1250_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include "LTE_ARFCN-ValueEUTRA-r9.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct LTE_AS_Config_v1250;
struct LTE_HandoverPreparationInformation_v1320_IEs;

/* LTE_HandoverPreparationInformation-v1250-IEs */
typedef struct LTE_HandoverPreparationInformation_v1250_IEs {
	LTE_ARFCN_ValueEUTRA_r9_t	*ue_SupportedEARFCN_r12;	/* OPTIONAL */
	struct LTE_AS_Config_v1250	*as_Config_v1250;	/* OPTIONAL */
	struct LTE_HandoverPreparationInformation_v1320_IEs	*nonCriticalExtension;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LTE_HandoverPreparationInformation_v1250_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LTE_HandoverPreparationInformation_v1250_IEs;
extern asn_SEQUENCE_specifics_t asn_SPC_LTE_HandoverPreparationInformation_v1250_IEs_specs_1;
extern asn_TYPE_member_t asn_MBR_LTE_HandoverPreparationInformation_v1250_IEs_1[3];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "LTE_AS-Config-v1250.h"
#include "LTE_HandoverPreparationInformation-v1320-IEs.h"

#endif	/* _LTE_HandoverPreparationInformation_v1250_IEs_H_ */
#include <asn_internal.h>
