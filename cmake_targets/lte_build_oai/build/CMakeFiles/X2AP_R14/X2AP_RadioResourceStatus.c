/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/X2AP/MESSAGES/ASN1/R14/x2ap-14.6.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -fno-include-deps -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/X2AP_R14`
 */

#include "X2AP_RadioResourceStatus.h"

#include "X2AP_ProtocolExtensionContainer.h"
asn_TYPE_member_t asn_MBR_X2AP_RadioResourceStatus_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct X2AP_RadioResourceStatus, dL_GBR_PRB_usage),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2AP_DL_GBR_PRB_usage,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dL-GBR-PRB-usage"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct X2AP_RadioResourceStatus, uL_GBR_PRB_usage),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2AP_UL_GBR_PRB_usage,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"uL-GBR-PRB-usage"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct X2AP_RadioResourceStatus, dL_non_GBR_PRB_usage),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2AP_DL_non_GBR_PRB_usage,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dL-non-GBR-PRB-usage"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct X2AP_RadioResourceStatus, uL_non_GBR_PRB_usage),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2AP_UL_non_GBR_PRB_usage,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"uL-non-GBR-PRB-usage"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct X2AP_RadioResourceStatus, dL_Total_PRB_usage),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2AP_DL_Total_PRB_usage,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"dL-Total-PRB-usage"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct X2AP_RadioResourceStatus, uL_Total_PRB_usage),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2AP_UL_Total_PRB_usage,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"uL-Total-PRB-usage"
		},
	{ ATF_POINTER, 1, offsetof(struct X2AP_RadioResourceStatus, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2AP_ProtocolExtensionContainer_5040P101,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_X2AP_RadioResourceStatus_oms_1[] = { 6 };
static const ber_tlv_tag_t asn_DEF_X2AP_RadioResourceStatus_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_X2AP_RadioResourceStatus_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* dL-GBR-PRB-usage */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* uL-GBR-PRB-usage */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* dL-non-GBR-PRB-usage */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* uL-non-GBR-PRB-usage */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* dL-Total-PRB-usage */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* uL-Total-PRB-usage */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_X2AP_RadioResourceStatus_specs_1 = {
	sizeof(struct X2AP_RadioResourceStatus),
	offsetof(struct X2AP_RadioResourceStatus, _asn_ctx),
	asn_MAP_X2AP_RadioResourceStatus_tag2el_1,
	7,	/* Count of tags in the map */
	asn_MAP_X2AP_RadioResourceStatus_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	7,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_X2AP_RadioResourceStatus = {
	"RadioResourceStatus",
	"RadioResourceStatus",
	&asn_OP_SEQUENCE,
	asn_DEF_X2AP_RadioResourceStatus_tags_1,
	sizeof(asn_DEF_X2AP_RadioResourceStatus_tags_1)
		/sizeof(asn_DEF_X2AP_RadioResourceStatus_tags_1[0]), /* 1 */
	asn_DEF_X2AP_RadioResourceStatus_tags_1,	/* Same as above */
	sizeof(asn_DEF_X2AP_RadioResourceStatus_tags_1)
		/sizeof(asn_DEF_X2AP_RadioResourceStatus_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_X2AP_RadioResourceStatus_1,
	7,	/* Elements count */
	&asn_SPC_X2AP_RadioResourceStatus_specs_1	/* Additional specs */
};

