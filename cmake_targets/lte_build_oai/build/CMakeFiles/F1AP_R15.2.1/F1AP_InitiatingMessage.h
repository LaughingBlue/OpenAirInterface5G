/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "F1AP-PDU-Descriptions"
 * 	found in "/home/labuser/Desktop/openairinterface5g_f1ap/openair2/F1AP/MESSAGES/ASN1/R15.2.1/F1AP-PDU-Descriptions.asn"
 * 	`asn1c -gen-PER -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -D /home/labuser/Desktop/openairinterface5g_f1ap/cmake_targets/lte_build_oai/build/CMakeFiles/F1AP_R15.2.1`
 */

#ifndef	_F1AP_InitiatingMessage_H_
#define	_F1AP_InitiatingMessage_H_


#include <asn_application.h>

/* Including external dependencies */
#include "F1AP_ProcedureCode.h"
#include "F1AP_Criticality.h"
#include <ANY.h>
#include <asn_ioc.h>
#include "F1AP_Reset.h"
#include "F1AP_ResetAcknowledge.h"
#include "F1AP_F1SetupRequest.h"
#include "F1AP_F1SetupResponse.h"
#include "F1AP_F1SetupFailure.h"
#include "F1AP_GNBDUConfigurationUpdate.h"
#include "F1AP_GNBDUConfigurationUpdateAcknowledge.h"
#include "F1AP_GNBDUConfigurationUpdateFailure.h"
#include "F1AP_GNBCUConfigurationUpdate.h"
#include "F1AP_GNBCUConfigurationUpdateAcknowledge.h"
#include "F1AP_GNBCUConfigurationUpdateFailure.h"
#include "F1AP_UEContextSetupRequest.h"
#include "F1AP_UEContextSetupResponse.h"
#include "F1AP_UEContextSetupFailure.h"
#include "F1AP_UEContextReleaseCommand.h"
#include "F1AP_UEContextReleaseComplete.h"
#include "F1AP_UEContextModificationRequest.h"
#include "F1AP_UEContextModificationResponse.h"
#include "F1AP_UEContextModificationFailure.h"
#include "F1AP_UEContextModificationRequired.h"
#include "F1AP_UEContextModificationConfirm.h"
#include "F1AP_WriteReplaceWarningRequest.h"
#include "F1AP_WriteReplaceWarningResponse.h"
#include "F1AP_PWSCancelRequest.h"
#include "F1AP_PWSCancelResponse.h"
#include "F1AP_GNBDUResourceCoordinationRequest.h"
#include "F1AP_GNBDUResourceCoordinationResponse.h"
#include "F1AP_ErrorIndication.h"
#include "F1AP_UEContextReleaseRequest.h"
#include "F1AP_DLRRCMessageTransfer.h"
#include "F1AP_ULRRCMessageTransfer.h"
#include "F1AP_UEInactivityNotification.h"
#include "F1AP_PrivateMessage.h"
#include "F1AP_InitialULRRCMessageTransfer.h"
#include "F1AP_SystemInformationDeliveryCommand.h"
#include "F1AP_Paging.h"
#include "F1AP_Notify.h"
#include "F1AP_PWSRestartIndication.h"
#include "F1AP_PWSFailureIndication.h"
#include <OPEN_TYPE.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum F1AP_InitiatingMessage__value_PR {
	F1AP_InitiatingMessage__value_PR_NOTHING,	/* No components present */
	F1AP_InitiatingMessage__value_PR_Reset,
	F1AP_InitiatingMessage__value_PR_F1SetupRequest,
	F1AP_InitiatingMessage__value_PR_GNBDUConfigurationUpdate,
	F1AP_InitiatingMessage__value_PR_GNBCUConfigurationUpdate,
	F1AP_InitiatingMessage__value_PR_UEContextSetupRequest,
	F1AP_InitiatingMessage__value_PR_UEContextReleaseCommand,
	F1AP_InitiatingMessage__value_PR_UEContextModificationRequest,
	F1AP_InitiatingMessage__value_PR_UEContextModificationRequired,
	F1AP_InitiatingMessage__value_PR_WriteReplaceWarningRequest,
	F1AP_InitiatingMessage__value_PR_PWSCancelRequest,
	F1AP_InitiatingMessage__value_PR_GNBDUResourceCoordinationRequest,
	F1AP_InitiatingMessage__value_PR_ErrorIndication,
	F1AP_InitiatingMessage__value_PR_UEContextReleaseRequest,
	F1AP_InitiatingMessage__value_PR_DLRRCMessageTransfer,
	F1AP_InitiatingMessage__value_PR_ULRRCMessageTransfer,
	F1AP_InitiatingMessage__value_PR_UEInactivityNotification,
	F1AP_InitiatingMessage__value_PR_PrivateMessage,
	F1AP_InitiatingMessage__value_PR_InitialULRRCMessageTransfer,
	F1AP_InitiatingMessage__value_PR_SystemInformationDeliveryCommand,
	F1AP_InitiatingMessage__value_PR_Paging,
	F1AP_InitiatingMessage__value_PR_Notify,
	F1AP_InitiatingMessage__value_PR_PWSRestartIndication,
	F1AP_InitiatingMessage__value_PR_PWSFailureIndication
} F1AP_InitiatingMessage__value_PR;

/* F1AP_InitiatingMessage */
typedef struct F1AP_InitiatingMessage {
	F1AP_ProcedureCode_t	 procedureCode;
	F1AP_Criticality_t	 criticality;
	struct F1AP_InitiatingMessage__value {
		F1AP_InitiatingMessage__value_PR present;
		union F1AP_InitiatingMessage__F1AP_value_u {
			F1AP_Reset_t	 Reset;
			F1AP_F1SetupRequest_t	 F1SetupRequest;
			F1AP_GNBDUConfigurationUpdate_t	 GNBDUConfigurationUpdate;
			F1AP_GNBCUConfigurationUpdate_t	 GNBCUConfigurationUpdate;
			F1AP_UEContextSetupRequest_t	 UEContextSetupRequest;
			F1AP_UEContextReleaseCommand_t	 UEContextReleaseCommand;
			F1AP_UEContextModificationRequest_t	 UEContextModificationRequest;
			F1AP_UEContextModificationRequired_t	 UEContextModificationRequired;
			F1AP_WriteReplaceWarningRequest_t	 WriteReplaceWarningRequest;
			F1AP_PWSCancelRequest_t	 PWSCancelRequest;
			F1AP_GNBDUResourceCoordinationRequest_t	 GNBDUResourceCoordinationRequest;
			F1AP_ErrorIndication_t	 ErrorIndication;
			F1AP_UEContextReleaseRequest_t	 UEContextReleaseRequest;
			F1AP_DLRRCMessageTransfer_t	 DLRRCMessageTransfer;
			F1AP_ULRRCMessageTransfer_t	 ULRRCMessageTransfer;
			F1AP_UEInactivityNotification_t	 UEInactivityNotification;
			F1AP_PrivateMessage_t	 PrivateMessage;
			F1AP_InitialULRRCMessageTransfer_t	 InitialULRRCMessageTransfer;
			F1AP_SystemInformationDeliveryCommand_t	 SystemInformationDeliveryCommand;
			F1AP_Paging_t	 Paging;
			F1AP_Notify_t	 Notify;
			F1AP_PWSRestartIndication_t	 PWSRestartIndication;
			F1AP_PWSFailureIndication_t	 PWSFailureIndication;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} value;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} F1AP_InitiatingMessage_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_F1AP_InitiatingMessage;
extern asn_SEQUENCE_specifics_t asn_SPC_F1AP_InitiatingMessage_specs_1;
extern asn_TYPE_member_t asn_MBR_F1AP_InitiatingMessage_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _F1AP_InitiatingMessage_H_ */
#include <asn_internal.h>
