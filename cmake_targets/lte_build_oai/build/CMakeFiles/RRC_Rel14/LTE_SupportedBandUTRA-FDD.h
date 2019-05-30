/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/RRC/LTE/MESSAGES/asn1c/ASN1_files/lte-rrc-14.7.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/RRC_Rel14`
 */

#ifndef	_LTE_SupportedBandUTRA_FDD_H_
#define	_LTE_SupportedBandUTRA_FDD_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum LTE_SupportedBandUTRA_FDD {
	LTE_SupportedBandUTRA_FDD_bandI	= 0,
	LTE_SupportedBandUTRA_FDD_bandII	= 1,
	LTE_SupportedBandUTRA_FDD_bandIII	= 2,
	LTE_SupportedBandUTRA_FDD_bandIV	= 3,
	LTE_SupportedBandUTRA_FDD_bandV	= 4,
	LTE_SupportedBandUTRA_FDD_bandVI	= 5,
	LTE_SupportedBandUTRA_FDD_bandVII	= 6,
	LTE_SupportedBandUTRA_FDD_bandVIII	= 7,
	LTE_SupportedBandUTRA_FDD_bandIX	= 8,
	LTE_SupportedBandUTRA_FDD_bandX	= 9,
	LTE_SupportedBandUTRA_FDD_bandXI	= 10,
	LTE_SupportedBandUTRA_FDD_bandXII	= 11,
	LTE_SupportedBandUTRA_FDD_bandXIII	= 12,
	LTE_SupportedBandUTRA_FDD_bandXIV	= 13,
	LTE_SupportedBandUTRA_FDD_bandXV	= 14,
	LTE_SupportedBandUTRA_FDD_bandXVI	= 15,
	/*
	 * Enumeration is extensible
	 */
	LTE_SupportedBandUTRA_FDD_bandXVII_8a0	= 16,
	LTE_SupportedBandUTRA_FDD_bandXVIII_8a0	= 17,
	LTE_SupportedBandUTRA_FDD_bandXIX_8a0	= 18,
	LTE_SupportedBandUTRA_FDD_bandXX_8a0	= 19,
	LTE_SupportedBandUTRA_FDD_bandXXI_8a0	= 20,
	LTE_SupportedBandUTRA_FDD_bandXXII_8a0	= 21,
	LTE_SupportedBandUTRA_FDD_bandXXIII_8a0	= 22,
	LTE_SupportedBandUTRA_FDD_bandXXIV_8a0	= 23,
	LTE_SupportedBandUTRA_FDD_bandXXV_8a0	= 24,
	LTE_SupportedBandUTRA_FDD_bandXXVI_8a0	= 25,
	LTE_SupportedBandUTRA_FDD_bandXXVII_8a0	= 26,
	LTE_SupportedBandUTRA_FDD_bandXXVIII_8a0	= 27,
	LTE_SupportedBandUTRA_FDD_bandXXIX_8a0	= 28,
	LTE_SupportedBandUTRA_FDD_bandXXX_8a0	= 29,
	LTE_SupportedBandUTRA_FDD_bandXXXI_8a0	= 30,
	LTE_SupportedBandUTRA_FDD_bandXXXII_8a0	= 31
} e_LTE_SupportedBandUTRA_FDD;

/* LTE_SupportedBandUTRA-FDD */
typedef long	 LTE_SupportedBandUTRA_FDD_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_LTE_SupportedBandUTRA_FDD_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_LTE_SupportedBandUTRA_FDD;
extern const asn_INTEGER_specifics_t asn_SPC_LTE_SupportedBandUTRA_FDD_specs_1;
asn_struct_free_f LTE_SupportedBandUTRA_FDD_free;
asn_struct_print_f LTE_SupportedBandUTRA_FDD_print;
asn_constr_check_f LTE_SupportedBandUTRA_FDD_constraint;
ber_type_decoder_f LTE_SupportedBandUTRA_FDD_decode_ber;
der_type_encoder_f LTE_SupportedBandUTRA_FDD_encode_der;
xer_type_decoder_f LTE_SupportedBandUTRA_FDD_decode_xer;
xer_type_encoder_f LTE_SupportedBandUTRA_FDD_encode_xer;
per_type_decoder_f LTE_SupportedBandUTRA_FDD_decode_uper;
per_type_encoder_f LTE_SupportedBandUTRA_FDD_encode_uper;
per_type_decoder_f LTE_SupportedBandUTRA_FDD_decode_aper;
per_type_encoder_f LTE_SupportedBandUTRA_FDD_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _LTE_SupportedBandUTRA_FDD_H_ */
#include <asn_internal.h>