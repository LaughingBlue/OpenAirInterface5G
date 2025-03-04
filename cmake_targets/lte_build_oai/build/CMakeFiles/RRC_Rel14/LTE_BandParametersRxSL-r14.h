/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/RRC/LTE/MESSAGES/asn1c/ASN1_files/lte-rrc-14.7.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/RRC_Rel14`
 */

#ifndef	_LTE_BandParametersRxSL_r14_H_
#define	_LTE_BandParametersRxSL_r14_H_


#include <asn_application.h>

/* Including external dependencies */
#include "LTE_V2X-BandwidthClassSL-r14.h"
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum LTE_BandParametersRxSL_r14__v2x_HighReception_r14 {
	LTE_BandParametersRxSL_r14__v2x_HighReception_r14_supported	= 0
} e_LTE_BandParametersRxSL_r14__v2x_HighReception_r14;

/* LTE_BandParametersRxSL-r14 */
typedef struct LTE_BandParametersRxSL_r14 {
	LTE_V2X_BandwidthClassSL_r14_t	 v2x_BandwidthClassRxSL_r14;
	long	*v2x_HighReception_r14;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LTE_BandParametersRxSL_r14_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_LTE_v2x_HighReception_r14_3;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_LTE_BandParametersRxSL_r14;
extern asn_SEQUENCE_specifics_t asn_SPC_LTE_BandParametersRxSL_r14_specs_1;
extern asn_TYPE_member_t asn_MBR_LTE_BandParametersRxSL_r14_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _LTE_BandParametersRxSL_r14_H_ */
#include <asn_internal.h>
