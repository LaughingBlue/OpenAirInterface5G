/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "S1AP-PDU-Contents"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair3/S1AP/MESSAGES/ASN1/R14/s1ap-14.5.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -fno-include-deps -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/S1AP_R14`
 */

#include "S1AP_Inter-SystemInformationTransferType.h"

static asn_per_constraints_t asn_PER_type_S1AP_Inter_SystemInformationTransferType_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  0,  0,  0,  0 }	/* (0..0,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_S1AP_Inter_SystemInformationTransferType_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct S1AP_Inter_SystemInformationTransferType, choice.rIMTransfer),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_S1AP_RIMTransfer,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"rIMTransfer"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_S1AP_Inter_SystemInformationTransferType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* rIMTransfer */
};
static asn_CHOICE_specifics_t asn_SPC_S1AP_Inter_SystemInformationTransferType_specs_1 = {
	sizeof(struct S1AP_Inter_SystemInformationTransferType),
	offsetof(struct S1AP_Inter_SystemInformationTransferType, _asn_ctx),
	offsetof(struct S1AP_Inter_SystemInformationTransferType, present),
	sizeof(((struct S1AP_Inter_SystemInformationTransferType *)0)->present),
	asn_MAP_S1AP_Inter_SystemInformationTransferType_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0,
	1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_S1AP_Inter_SystemInformationTransferType = {
	"Inter-SystemInformationTransferType",
	"Inter-SystemInformationTransferType",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ 0, &asn_PER_type_S1AP_Inter_SystemInformationTransferType_constr_1, CHOICE_constraint },
	asn_MBR_S1AP_Inter_SystemInformationTransferType_1,
	1,	/* Elements count */
	&asn_SPC_S1AP_Inter_SystemInformationTransferType_specs_1	/* Additional specs */
};

