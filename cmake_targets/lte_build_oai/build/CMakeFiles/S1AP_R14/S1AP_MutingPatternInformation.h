/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "S1AP-IEs"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair3/S1AP/MESSAGES/ASN1/R14/s1ap-14.5.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -fno-include-deps -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/S1AP_R14`
 */

#ifndef	_S1AP_MutingPatternInformation_H_
#define	_S1AP_MutingPatternInformation_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum S1AP_MutingPatternInformation__muting_pattern_period {
	S1AP_MutingPatternInformation__muting_pattern_period_ms0	= 0,
	S1AP_MutingPatternInformation__muting_pattern_period_ms1280	= 1,
	S1AP_MutingPatternInformation__muting_pattern_period_ms2560	= 2,
	S1AP_MutingPatternInformation__muting_pattern_period_ms5120	= 3,
	S1AP_MutingPatternInformation__muting_pattern_period_ms10240	= 4
	/*
	 * Enumeration is extensible
	 */
} e_S1AP_MutingPatternInformation__muting_pattern_period;

/* Forward declarations */
struct S1AP_ProtocolExtensionContainer;

/* S1AP_MutingPatternInformation */
typedef struct S1AP_MutingPatternInformation {
	long	 muting_pattern_period;
	long	*muting_pattern_offset;	/* OPTIONAL */
	struct S1AP_ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} S1AP_MutingPatternInformation_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_S1AP_muting_pattern_period_2;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_S1AP_MutingPatternInformation;

#ifdef __cplusplus
}
#endif

#endif	/* _S1AP_MutingPatternInformation_H_ */
#include <asn_internal.h>
