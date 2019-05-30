/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/RRC/LTE/MESSAGES/asn1c/ASN1_files/lte-rrc-14.7.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/RRC_Rel14`
 */

#include "LTE_TrafficPatternInfo-r14.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static int
memb_LTE_timingOffset_r14_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 10239)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_LTE_logicalChannelIdentityUL_r14_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 3 && value <= 10)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_LTE_messageSize_r14_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	if(st->size > 0) {
		/* Size in bits */
		size = 8 * st->size - (st->bits_unused & 0x07);
	} else {
		size = 0;
	}
	
	if((size == 6)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_type_LTE_trafficPeriodicity_r14_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  11 }	/* (0..11) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_LTE_timingOffset_r14_constr_15 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 14,  14,  0,  10239 }	/* (0..10239) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_LTE_logicalChannelIdentityUL_r14_constr_17 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 3,  3,  3,  10 }	/* (3..10) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_LTE_messageSize_r14_constr_18 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  6,  6 }	/* (SIZE(6..6)) */,
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_LTE_trafficPeriodicity_r14_value2enum_2[] = {
	{ 0,	4,	"sf20" },
	{ 1,	4,	"sf50" },
	{ 2,	5,	"sf100" },
	{ 3,	5,	"sf200" },
	{ 4,	5,	"sf300" },
	{ 5,	5,	"sf400" },
	{ 6,	5,	"sf500" },
	{ 7,	5,	"sf600" },
	{ 8,	5,	"sf700" },
	{ 9,	5,	"sf800" },
	{ 10,	5,	"sf900" },
	{ 11,	6,	"sf1000" }
};
static const unsigned int asn_MAP_LTE_trafficPeriodicity_r14_enum2value_2[] = {
	2,	/* sf100(2) */
	11,	/* sf1000(11) */
	0,	/* sf20(0) */
	3,	/* sf200(3) */
	4,	/* sf300(4) */
	5,	/* sf400(5) */
	1,	/* sf50(1) */
	6,	/* sf500(6) */
	7,	/* sf600(7) */
	8,	/* sf700(8) */
	9,	/* sf800(9) */
	10	/* sf900(10) */
};
static const asn_INTEGER_specifics_t asn_SPC_LTE_trafficPeriodicity_r14_specs_2 = {
	asn_MAP_LTE_trafficPeriodicity_r14_value2enum_2,	/* "tag" => N; sorted by tag */
	asn_MAP_LTE_trafficPeriodicity_r14_enum2value_2,	/* N => "tag"; sorted by N */
	12,	/* Number of elements in the maps */
	0,	/* Enumeration is not extensible */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_LTE_trafficPeriodicity_r14_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_LTE_trafficPeriodicity_r14_2 = {
	"trafficPeriodicity-r14",
	"trafficPeriodicity-r14",
	&asn_OP_NativeEnumerated,
	asn_DEF_LTE_trafficPeriodicity_r14_tags_2,
	sizeof(asn_DEF_LTE_trafficPeriodicity_r14_tags_2)
		/sizeof(asn_DEF_LTE_trafficPeriodicity_r14_tags_2[0]) - 1, /* 1 */
	asn_DEF_LTE_trafficPeriodicity_r14_tags_2,	/* Same as above */
	sizeof(asn_DEF_LTE_trafficPeriodicity_r14_tags_2)
		/sizeof(asn_DEF_LTE_trafficPeriodicity_r14_tags_2[0]), /* 2 */
	{ 0, &asn_PER_type_LTE_trafficPeriodicity_r14_constr_2, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_LTE_trafficPeriodicity_r14_specs_2	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_LTE_TrafficPatternInfo_r14_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct LTE_TrafficPatternInfo_r14, trafficPeriodicity_r14),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LTE_trafficPeriodicity_r14_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"trafficPeriodicity-r14"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct LTE_TrafficPatternInfo_r14, timingOffset_r14),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ 0, &asn_PER_memb_LTE_timingOffset_r14_constr_15,  memb_LTE_timingOffset_r14_constraint_1 },
		0, 0, /* No default value */
		"timingOffset-r14"
		},
	{ ATF_POINTER, 2, offsetof(struct LTE_TrafficPatternInfo_r14, priorityInfoSL_r14),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LTE_SL_Priority_r13,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"priorityInfoSL-r14"
		},
	{ ATF_POINTER, 1, offsetof(struct LTE_TrafficPatternInfo_r14, logicalChannelIdentityUL_r14),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ 0, &asn_PER_memb_LTE_logicalChannelIdentityUL_r14_constr_17,  memb_LTE_logicalChannelIdentityUL_r14_constraint_1 },
		0, 0, /* No default value */
		"logicalChannelIdentityUL-r14"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct LTE_TrafficPatternInfo_r14, messageSize_r14),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ 0, &asn_PER_memb_LTE_messageSize_r14_constr_18,  memb_LTE_messageSize_r14_constraint_1 },
		0, 0, /* No default value */
		"messageSize-r14"
		},
};
static const int asn_MAP_LTE_TrafficPatternInfo_r14_oms_1[] = { 2, 3 };
static const ber_tlv_tag_t asn_DEF_LTE_TrafficPatternInfo_r14_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_LTE_TrafficPatternInfo_r14_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* trafficPeriodicity-r14 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* timingOffset-r14 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* priorityInfoSL-r14 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* logicalChannelIdentityUL-r14 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* messageSize-r14 */
};
asn_SEQUENCE_specifics_t asn_SPC_LTE_TrafficPatternInfo_r14_specs_1 = {
	sizeof(struct LTE_TrafficPatternInfo_r14),
	offsetof(struct LTE_TrafficPatternInfo_r14, _asn_ctx),
	asn_MAP_LTE_TrafficPatternInfo_r14_tag2el_1,
	5,	/* Count of tags in the map */
	asn_MAP_LTE_TrafficPatternInfo_r14_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_LTE_TrafficPatternInfo_r14 = {
	"TrafficPatternInfo-r14",
	"TrafficPatternInfo-r14",
	&asn_OP_SEQUENCE,
	asn_DEF_LTE_TrafficPatternInfo_r14_tags_1,
	sizeof(asn_DEF_LTE_TrafficPatternInfo_r14_tags_1)
		/sizeof(asn_DEF_LTE_TrafficPatternInfo_r14_tags_1[0]), /* 1 */
	asn_DEF_LTE_TrafficPatternInfo_r14_tags_1,	/* Same as above */
	sizeof(asn_DEF_LTE_TrafficPatternInfo_r14_tags_1)
		/sizeof(asn_DEF_LTE_TrafficPatternInfo_r14_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_LTE_TrafficPatternInfo_r14_1,
	5,	/* Elements count */
	&asn_SPC_LTE_TrafficPatternInfo_r14_specs_1	/* Additional specs */
};
