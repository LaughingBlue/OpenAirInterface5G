/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "PC5-RRC-Definitions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/RRC/LTE/MESSAGES/asn1c/ASN1_files/lte-rrc-14.7.0.asn1"
 * 	`asn1c -pdu=all -fcompound-names -gen-PER -no-gen-OER -no-gen-example -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/RRC_Rel14`
 */

#include "LTE_SBCCH-SL-BCH-MessageType-V2X-r14.h"

/*
 * This type is implemented using LTE_MasterInformationBlock_SL_V2X_r14,
 * so here we adjust the DEF accordingly.
 */
static const ber_tlv_tag_t asn_DEF_LTE_SBCCH_SL_BCH_MessageType_V2X_r14_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
asn_TYPE_descriptor_t asn_DEF_LTE_SBCCH_SL_BCH_MessageType_V2X_r14 = {
	"SBCCH-SL-BCH-MessageType-V2X-r14",
	"SBCCH-SL-BCH-MessageType-V2X-r14",
	&asn_OP_SEQUENCE,
	asn_DEF_LTE_SBCCH_SL_BCH_MessageType_V2X_r14_tags_1,
	sizeof(asn_DEF_LTE_SBCCH_SL_BCH_MessageType_V2X_r14_tags_1)
		/sizeof(asn_DEF_LTE_SBCCH_SL_BCH_MessageType_V2X_r14_tags_1[0]), /* 1 */
	asn_DEF_LTE_SBCCH_SL_BCH_MessageType_V2X_r14_tags_1,	/* Same as above */
	sizeof(asn_DEF_LTE_SBCCH_SL_BCH_MessageType_V2X_r14_tags_1)
		/sizeof(asn_DEF_LTE_SBCCH_SL_BCH_MessageType_V2X_r14_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_LTE_MasterInformationBlock_SL_V2X_r14_1,
	6,	/* Elements count */
	&asn_SPC_LTE_MasterInformationBlock_SL_V2X_r14_specs_1	/* Additional specs */
};

