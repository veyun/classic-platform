/* -------------------------------- Arctic Core ------------------------------
 * Arctic Core - the open source AUTOSAR platform http://arccore.com
 *
 * Copyright (C) 2009  ArcCore AB <contact@arccore.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * -------------------------------- Arctic Core ------------------------------*/








#ifndef COM_H_
#define COM_H_

#include "Std_Types.h"
#include "Com_Types.h"
#include "Com_EcoreTypes.h"


#ifdef COM_DEV_ERROR_DETECT
#include "Det.h"
#endif

#include "Com_Cfg.h"
#include "Com_PbCfg.h"
#include "Com_Com.h"
#include "Com_Sched.h"

const Com_ConfigType * ComConfig;

ComEcoreConfig_type ComEcoreConfig;



#ifdef COM_DEV_ERROR_DETECT

#undef DET_REPORTERROR
#define DET_REPORTERROR(_x,_y,_z,_q) Det_ReportError(_x,_y,_z,_q)


// Define macro for parameter check.
#define PduIdCheck(PduId,ApiId,...) \
	if (PduId >= ComEcoreConfig.ComNIPdu) { \
		DET_REPORTERROR(COM_MODULE_ID, COM_INSTANCE_ID, ApiId, COM_INVALID_PDU_ID); \
		return __VA_ARGS__; \
	} \

#define COM_VALIDATE_SIGNAL(SignalId, ApiId, ...) \
	if (ComConfig->ComSignal[SignalId].ComEcoreIsSignalGroup) { \
		DET_REPORTERROR(COM_MODULE_ID, COM_INSTANCE_ID, ApiId, COM_ERROR_SIGNAL_IS_SIGNALGROUP); \
		return __VA_ARGS__; \
	} \


#else

#undef DET_REPORTERROR
#define DET_REPORTERROR(_x,_y,_z,_q)

#define PduIdCheck(PduId,ApiId,...)
#define COM_VALIDATE_SIGNAL(PduId, ApiId, ...)
#endif


#define testBit(source,bit) (*((uint8 *)source + (bit / 8)) & (1 << (bit % 8)))
#define setBit(dest,bit) *((uint8 *)dest + (bit / 8)) |= (1 << (bit % 8))
#define clearBit(dest,bit) *((uint8 *)dest + (bit / 8)) &= ~(1 << (bit % 8))

#define ComGetSignal(SignalId) \
	const ComSignal_type * Signal = &ComConfig->ComSignal[SignalId]\

#define ComGetEcoreSignal(SignalId) \
	ComEcoreSignal_type * EcoreSignal = &ComEcoreConfig.ComSignal[SignalId]\

#define ComGetIPdu(IPduId) \
	const ComIPdu_type *IPdu = &ComConfig->ComIPdu[IPduId]\

#define ComGetEcoreIPdu(IPduId) \
	ComEcoreIPdu_type *EcoreIPdu = &ComEcoreConfig.ComIPdu[IPduId]\

#define ComGetGroupSignal(GroupSignalId) \
	const ComGroupSignal_type *GroupSignal = &ComConfig->ComGroupSignal[GroupSignalId]\

#define ComGetEcoreGroupSignal(GroupSignalId) \
	ComEcoreGroupSignal_type *EcoreGroupSignal = &ComEcoreConfig.ComGroupSignal[GroupSignalId]\

//-------------------------------------------------------------------
// From OSEK_VDX spec...
//
//typedef uint32 MessageIdentifier;
// TODO: Have no idea here..
//typedef void * ApplicationDataRef;






//-------------------------------------------------------------------

// From OSEK_VDX

/* The service SendMessage updates the message object identified by
 * <Message> with the application message referenced by the
 * <DataRef> parameter.
 *
 * Internal communication:
 * The message <Message> is routed to the receiving part of the IL.
  */
// Update 2008-10-30, SendMessage and ReceiveMessage should not be required. ensured by RTE. COM013
//StatusType SendMessage(MessageIdentifier , ApplicationDataRef );

// The service  ReceiveMessage updates the application message
// referenced by <DataRef> with the data in the message object
// identified by <Message>. It resets all flags (Notification classes 1 and
// 3) associated with <Message>.
//StatusType ReceiveMessage ( MessageIdentifier , ApplicationDataRef );


// From Autosar
void Com_Init( const Com_ConfigType * ConfigPtr);
void Com_DeInit( void );

void Com_IpduGroupStart(Com_PduGroupIdType IpduGroupId, boolean Initialize);
void Com_IpduGroupStop(Com_PduGroupIdType IpduGroupId);


#endif /*COM_H_*/