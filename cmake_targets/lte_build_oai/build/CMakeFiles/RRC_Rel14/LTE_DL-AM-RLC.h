/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/RRC/LTE/MESSAGES/asn1c/ASN1_files/lte-rrc-14.7.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/RRC_Rel14`
 */

#ifndef	_LTE_DL_AM_RLC_H_
#define	_LTE_DL_AM_RLC_H_


#include <asn_application.h>

/* Including external dependencies */
#include "LTE_T-Reordering.h"
#include "LTE_T-StatusProhibit.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* LTE_DL-AM-RLC */
typedef struct LTE_DL_AM_RLC {
	LTE_T_Reordering_t	 t_Reordering;
	LTE_T_StatusProhibit_t	 t_StatusProhibit;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LTE_DL_AM_RLC_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LTE_DL_AM_RLC;
extern asn_SEQUENCE_specifics_t asn_SPC_LTE_DL_AM_RLC_specs_1;
extern asn_TYPE_member_t asn_MBR_LTE_DL_AM_RLC_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _LTE_DL_AM_RLC_H_ */
#include <asn_internal.h>
