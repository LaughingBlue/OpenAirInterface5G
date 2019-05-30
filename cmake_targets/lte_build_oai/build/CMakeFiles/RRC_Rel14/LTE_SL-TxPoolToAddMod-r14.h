/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/RRC/LTE/MESSAGES/asn1c/ASN1_files/lte-rrc-14.7.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/RRC_Rel14`
 */

#ifndef	_LTE_SL_TxPoolToAddMod_r14_H_
#define	_LTE_SL_TxPoolToAddMod_r14_H_


#include <asn_application.h>

/* Including external dependencies */
#include "LTE_SL-V2X-TxPoolIdentity-r14.h"
#include "LTE_SL-CommResourcePoolV2X-r14.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* LTE_SL-TxPoolToAddMod-r14 */
typedef struct LTE_SL_TxPoolToAddMod_r14 {
	LTE_SL_V2X_TxPoolIdentity_r14_t	 poolIdentity_r14;
	LTE_SL_CommResourcePoolV2X_r14_t	 pool_r14;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LTE_SL_TxPoolToAddMod_r14_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LTE_SL_TxPoolToAddMod_r14;
extern asn_SEQUENCE_specifics_t asn_SPC_LTE_SL_TxPoolToAddMod_r14_specs_1;
extern asn_TYPE_member_t asn_MBR_LTE_SL_TxPoolToAddMod_r14_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _LTE_SL_TxPoolToAddMod_r14_H_ */
#include <asn_internal.h>