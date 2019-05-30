/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/RRC/LTE/MESSAGES/asn1c/ASN1_files/lte-rrc-14.7.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/RRC_Rel14`
 */

#ifndef	_LTE_RRCConnectionRelease_v920_IEs_H_
#define	_LTE_RRCConnectionRelease_v920_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include "LTE_CellInfoListGERAN-r9.h"
#include "LTE_CellInfoListUTRA-FDD-r9.h"
#include "LTE_CellInfoListUTRA-TDD-r9.h"
#include "LTE_CellInfoListUTRA-TDD-r10.h"
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum LTE_RRCConnectionRelease_v920_IEs__cellInfoList_r9_PR {
	LTE_RRCConnectionRelease_v920_IEs__cellInfoList_r9_PR_NOTHING,	/* No components present */
	LTE_RRCConnectionRelease_v920_IEs__cellInfoList_r9_PR_geran_r9,
	LTE_RRCConnectionRelease_v920_IEs__cellInfoList_r9_PR_utra_FDD_r9,
	LTE_RRCConnectionRelease_v920_IEs__cellInfoList_r9_PR_utra_TDD_r9,
	/* Extensions may appear below */
	LTE_RRCConnectionRelease_v920_IEs__cellInfoList_r9_PR_utra_TDD_r10
} LTE_RRCConnectionRelease_v920_IEs__cellInfoList_r9_PR;

/* Forward declarations */
struct LTE_RRCConnectionRelease_v1020_IEs;

/* LTE_RRCConnectionRelease-v920-IEs */
typedef struct LTE_RRCConnectionRelease_v920_IEs {
	struct LTE_RRCConnectionRelease_v920_IEs__cellInfoList_r9 {
		LTE_RRCConnectionRelease_v920_IEs__cellInfoList_r9_PR present;
		union LTE_RRCConnectionRelease_v920_IEs__LTE_cellInfoList_r9_u {
			LTE_CellInfoListGERAN_r9_t	 geran_r9;
			LTE_CellInfoListUTRA_FDD_r9_t	 utra_FDD_r9;
			LTE_CellInfoListUTRA_TDD_r9_t	 utra_TDD_r9;
			/*
			 * This type is extensible,
			 * possible extensions are below.
			 */
			LTE_CellInfoListUTRA_TDD_r10_t	 utra_TDD_r10;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *cellInfoList_r9;
	struct LTE_RRCConnectionRelease_v1020_IEs	*nonCriticalExtension;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LTE_RRCConnectionRelease_v920_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LTE_RRCConnectionRelease_v920_IEs;
extern asn_SEQUENCE_specifics_t asn_SPC_LTE_RRCConnectionRelease_v920_IEs_specs_1;
extern asn_TYPE_member_t asn_MBR_LTE_RRCConnectionRelease_v920_IEs_1[2];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "LTE_RRCConnectionRelease-v1020-IEs.h"

#endif	/* _LTE_RRCConnectionRelease_v920_IEs_H_ */
#include <asn_internal.h>