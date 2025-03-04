/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-PDU-Descriptions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/F1AP/MESSAGES/ASN1/R15.2.1/F1AP-PDU-Descriptions.asn"
 * 	`asn1c -gen-PER -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/F1AP_R15.2.1`
 */

#ifndef	_F1AP_F1AP_PDU_H_
#define	_F1AP_F1AP_PDU_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum F1AP_F1AP_PDU_PR {
	F1AP_F1AP_PDU_PR_NOTHING,	/* No components present */
	F1AP_F1AP_PDU_PR_initiatingMessage,
	F1AP_F1AP_PDU_PR_successfulOutcome,
	F1AP_F1AP_PDU_PR_unsuccessfulOutcome
	/* Extensions may appear below */
	
} F1AP_F1AP_PDU_PR;

/* Forward declarations */
struct F1AP_InitiatingMessage;
struct F1AP_SuccessfulOutcome;
struct F1AP_UnsuccessfulOutcome;

/* F1AP_F1AP-PDU */
typedef struct F1AP_F1AP_PDU {
	F1AP_F1AP_PDU_PR present;
	union F1AP_F1AP_PDU_u {
		struct F1AP_InitiatingMessage	*initiatingMessage;
		struct F1AP_SuccessfulOutcome	*successfulOutcome;
		struct F1AP_UnsuccessfulOutcome	*unsuccessfulOutcome;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} F1AP_F1AP_PDU_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_F1AP_F1AP_PDU;

#ifdef __cplusplus
}
#endif

#endif	/* _F1AP_F1AP_PDU_H_ */
#include <asn_internal.h>
