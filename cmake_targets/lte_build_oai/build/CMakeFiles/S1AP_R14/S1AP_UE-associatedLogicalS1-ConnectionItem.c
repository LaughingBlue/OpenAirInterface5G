/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "S1AP-IEs"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair3/S1AP/MESSAGES/ASN1/R14/s1ap-14.5.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -fno-include-deps -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/S1AP_R14`
 */

#include "S1AP_UE-associatedLogicalS1-ConnectionItem.h"

#include "S1AP_ProtocolExtensionContainer.h"
static asn_TYPE_member_t asn_MBR_S1AP_UE_associatedLogicalS1_ConnectionItem_1[] = {
	{ ATF_POINTER, 3, offsetof(struct S1AP_UE_associatedLogicalS1_ConnectionItem, mME_UE_S1AP_ID),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_S1AP_MME_UE_S1AP_ID,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"mME-UE-S1AP-ID"
		},
	{ ATF_POINTER, 2, offsetof(struct S1AP_UE_associatedLogicalS1_ConnectionItem, eNB_UE_S1AP_ID),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_S1AP_ENB_UE_S1AP_ID,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"eNB-UE-S1AP-ID"
		},
	{ ATF_POINTER, 1, offsetof(struct S1AP_UE_associatedLogicalS1_ConnectionItem, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_S1AP_ProtocolExtensionContainer_6628P111,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_S1AP_UE_associatedLogicalS1_ConnectionItem_oms_1[] = { 0, 1, 2 };
static const ber_tlv_tag_t asn_DEF_S1AP_UE_associatedLogicalS1_ConnectionItem_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_S1AP_UE_associatedLogicalS1_ConnectionItem_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* mME-UE-S1AP-ID */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* eNB-UE-S1AP-ID */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* iE-Extensions */
};
static asn_SEQUENCE_specifics_t asn_SPC_S1AP_UE_associatedLogicalS1_ConnectionItem_specs_1 = {
	sizeof(struct S1AP_UE_associatedLogicalS1_ConnectionItem),
	offsetof(struct S1AP_UE_associatedLogicalS1_ConnectionItem, _asn_ctx),
	asn_MAP_S1AP_UE_associatedLogicalS1_ConnectionItem_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_S1AP_UE_associatedLogicalS1_ConnectionItem_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_S1AP_UE_associatedLogicalS1_ConnectionItem = {
	"UE-associatedLogicalS1-ConnectionItem",
	"UE-associatedLogicalS1-ConnectionItem",
	&asn_OP_SEQUENCE,
	asn_DEF_S1AP_UE_associatedLogicalS1_ConnectionItem_tags_1,
	sizeof(asn_DEF_S1AP_UE_associatedLogicalS1_ConnectionItem_tags_1)
		/sizeof(asn_DEF_S1AP_UE_associatedLogicalS1_ConnectionItem_tags_1[0]), /* 1 */
	asn_DEF_S1AP_UE_associatedLogicalS1_ConnectionItem_tags_1,	/* Same as above */
	sizeof(asn_DEF_S1AP_UE_associatedLogicalS1_ConnectionItem_tags_1)
		/sizeof(asn_DEF_S1AP_UE_associatedLogicalS1_ConnectionItem_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_S1AP_UE_associatedLogicalS1_ConnectionItem_1,
	3,	/* Elements count */
	&asn_SPC_S1AP_UE_associatedLogicalS1_ConnectionItem_specs_1	/* Additional specs */
};

