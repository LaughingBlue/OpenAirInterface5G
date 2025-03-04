/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/RRC/LTE/MESSAGES/asn1c/ASN1_files/lte-rrc-14.7.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/RRC_Rel14`
 */

#ifndef	_LTE_BCCH_BCH_MessageType_H_
#define	_LTE_BCCH_BCH_MessageType_H_


#include <asn_application.h>

/* Including external dependencies */
#include "LTE_MasterInformationBlock.h"

#ifdef __cplusplus
extern "C" {
#endif

/* LTE_BCCH-BCH-MessageType */
typedef LTE_MasterInformationBlock_t	 LTE_BCCH_BCH_MessageType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LTE_BCCH_BCH_MessageType;
asn_struct_free_f LTE_BCCH_BCH_MessageType_free;
asn_struct_print_f LTE_BCCH_BCH_MessageType_print;
asn_constr_check_f LTE_BCCH_BCH_MessageType_constraint;
ber_type_decoder_f LTE_BCCH_BCH_MessageType_decode_ber;
der_type_encoder_f LTE_BCCH_BCH_MessageType_encode_der;
xer_type_decoder_f LTE_BCCH_BCH_MessageType_decode_xer;
xer_type_encoder_f LTE_BCCH_BCH_MessageType_encode_xer;
per_type_decoder_f LTE_BCCH_BCH_MessageType_decode_uper;
per_type_encoder_f LTE_BCCH_BCH_MessageType_encode_uper;
per_type_decoder_f LTE_BCCH_BCH_MessageType_decode_aper;
per_type_encoder_f LTE_BCCH_BCH_MessageType_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _LTE_BCCH_BCH_MessageType_H_ */
#include <asn_internal.h>
