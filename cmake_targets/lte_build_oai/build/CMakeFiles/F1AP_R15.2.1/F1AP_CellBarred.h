/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-IEs"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/F1AP/MESSAGES/ASN1/R15.2.1/F1AP-IEs.asn"
 * 	`asn1c -gen-PER -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/F1AP_R15.2.1`
 */

#ifndef	_F1AP_CellBarred_H_
#define	_F1AP_CellBarred_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum F1AP_CellBarred {
	F1AP_CellBarred_barred	= 0,
	F1AP_CellBarred_not_barred	= 1
	/*
	 * Enumeration is extensible
	 */
} e_F1AP_CellBarred;

/* F1AP_CellBarred */
typedef long	 F1AP_CellBarred_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_F1AP_CellBarred_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_F1AP_CellBarred;
extern const asn_INTEGER_specifics_t asn_SPC_F1AP_CellBarred_specs_1;
asn_struct_free_f F1AP_CellBarred_free;
asn_struct_print_f F1AP_CellBarred_print;
asn_constr_check_f F1AP_CellBarred_constraint;
ber_type_decoder_f F1AP_CellBarred_decode_ber;
der_type_encoder_f F1AP_CellBarred_encode_der;
xer_type_decoder_f F1AP_CellBarred_decode_xer;
xer_type_encoder_f F1AP_CellBarred_encode_xer;
oer_type_decoder_f F1AP_CellBarred_decode_oer;
oer_type_encoder_f F1AP_CellBarred_encode_oer;
per_type_decoder_f F1AP_CellBarred_decode_uper;
per_type_encoder_f F1AP_CellBarred_encode_uper;
per_type_decoder_f F1AP_CellBarred_decode_aper;
per_type_encoder_f F1AP_CellBarred_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _F1AP_CellBarred_H_ */
#include <asn_internal.h>
