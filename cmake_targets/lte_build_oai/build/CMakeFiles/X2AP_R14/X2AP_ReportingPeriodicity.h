/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-PDU-Contents"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/X2AP/MESSAGES/ASN1/R14/x2ap-14.6.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -fno-include-deps -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/X2AP_R14`
 */

#ifndef	_X2AP_ReportingPeriodicity_H_
#define	_X2AP_ReportingPeriodicity_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum X2AP_ReportingPeriodicity {
	X2AP_ReportingPeriodicity_one_thousand_ms	= 0,
	X2AP_ReportingPeriodicity_two_thousand_ms	= 1,
	X2AP_ReportingPeriodicity_five_thousand_ms	= 2,
	X2AP_ReportingPeriodicity_ten_thousand_ms	= 3
	/*
	 * Enumeration is extensible
	 */
} e_X2AP_ReportingPeriodicity;

/* X2AP_ReportingPeriodicity */
typedef long	 X2AP_ReportingPeriodicity_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_X2AP_ReportingPeriodicity;
asn_struct_free_f X2AP_ReportingPeriodicity_free;
asn_struct_print_f X2AP_ReportingPeriodicity_print;
asn_constr_check_f X2AP_ReportingPeriodicity_constraint;
ber_type_decoder_f X2AP_ReportingPeriodicity_decode_ber;
der_type_encoder_f X2AP_ReportingPeriodicity_encode_der;
xer_type_decoder_f X2AP_ReportingPeriodicity_decode_xer;
xer_type_encoder_f X2AP_ReportingPeriodicity_encode_xer;
per_type_decoder_f X2AP_ReportingPeriodicity_decode_uper;
per_type_encoder_f X2AP_ReportingPeriodicity_encode_uper;
per_type_decoder_f X2AP_ReportingPeriodicity_decode_aper;
per_type_encoder_f X2AP_ReportingPeriodicity_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _X2AP_ReportingPeriodicity_H_ */
#include <asn_internal.h>
