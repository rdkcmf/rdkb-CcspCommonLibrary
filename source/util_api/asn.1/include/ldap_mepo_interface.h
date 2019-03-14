/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/


/**********************************************************************

    module: ldap_mepo_interface.h

        For Lightweight Directory Access Protocol (V3) (LDAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the LDAP Message Element Parser
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Arun Vijay Mani

    ---------------------------------------------------------------

    revision:

        12/28/05    initial revision.

**********************************************************************/


#ifndef  _LDAP_MEPO_INTERFACE_
#define  _LDAP_MEPO_INTERFACE_
/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_lco_interface.h"
#include "ansc_lco_external_api.h"


/**********************************************************************
    PLATFORM INDEPENDENT BASIC MESSAGE ELEMENT PARSER OBJECT DEFINITION
***********************************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_LDAPMEP_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  LONG
(*PFN_LDAPMEP_GET_MSGID)
    (
        ANSC_HANDLE                 hThisObject,
		PVOID						pBuffer,
		ULONG						ulSize
    );

typedef  LONG
(*PFN_LDAPMEP_GET_MSGTYPE)
    (
        ANSC_HANDLE                 hThisObject,
		PVOID						pBuffer,
		ULONG						ulSize
    );

typedef  ANSC_STATUS
(*PFN_LDAPMEP_ENCODE_REQ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLdapReq,
		PVOID                       pBuffer,
		PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_LDAPMEP_DECODE_RES)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLdapResp,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );

typedef	 ANSC_STATUS
(*PFN_LDAP_MEP_REQ)
	(
		ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 hLdapMsg,
		ANSC_HANDLE                 hLdapReq
	);

typedef	 ANSC_STATUS
(*PFN_LDAP_MEP_RES)
	(
		ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 hLdapMsg,
		ANSC_HANDLE                 hLdapRes
	);

typedef	 ANSC_STATUS
(*PFN_LDAP_MEP_SET_URL)
	(
		ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 hLdapResult,
		ANSC_HANDLE                 hReferral
	);

typedef  ANSC_STATUS
(*PFN_LDAP_MEP_SET_MSG_ID)
	(
		ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 hMsgId,
		ULONG		                messageid
	);

typedef  ULONG
(*PFN_LDAP_MEP_GET_MSG_ID)
	(
		ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 hMsgId
	);

typedef  ULONG
(*PFN_LDAP_MEP_GET_MSG_TYPE)
	(
		ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 hMsgId
	);

typedef  ANSC_STATUS
(*PFN_LDAP_MEP_SET_BOOL)
	(
		ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 hObject,
		BOOL		                bboolval
	);

typedef  ANSC_STATUS
(*PFN_LDAP_MEP_GET_SIZE)
	(
		ANSC_HANDLE                 hThisObject,
		PUCHAR		                pString,
		PULONG						psize
	);

typedef  ANSC_STATUS
(*PFN_LDAP_MEP_GET_FILTER)
	(
		ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 hLdapMsg,
		PUCHAR		                pString,
		PULONG						psize
	);

typedef  ANSC_STATUS
(*PFN_LDAP_MEP_SET_LDAP_STR)
	(
		ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 hMsgId,
		PUCHAR		                pstr
	);

typedef  ANSC_STATUS
(*PFN_LDAP_MEP_SET_ATTR)
	(
		ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 hLdapAttr,
		ANSC_HANDLE	                pAttr
	);
typedef ANSC_STATUS
(*PFN_LDAP_MEP_SET_ATTR_TYP)
	(
		ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 hLdapAttr,
		PUCHAR		                pstr
	);

typedef ANSC_STATUS
(*PFN_LDAP_MEP_GET_LDAP_DN)
	(
		ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hStrObject,
		PUCHAR						pBuffer,
		PULONG						psize
	);

typedef ANSC_STATUS
(*PFN_LDAP_MEP_GET_RES_CD)
    (
		ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 hResObject,
		ULONG		                resultcode
    );

#define  LDAP_MESSAGE_ELEMENT_PARSER_CLASS_CONTENT                                          \
    /* duplication of the base object class content */                                      \
    ANSCLCO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
																							\
	ANSC_HANDLE						hLdapMepIf;												\
																						    \
	PFN_LDAPMEP_GET_IF				LdapGetMepIf;											\
	PFN_LDAPMEP_GET_MSGID			LdapGetMsgId;											\
    PFN_LDAPMEP_GET_MSGTYPE			LdapGetMsgType;											\
    PFN_LDAPMEP_ENCODE_REQ			LdapEnReqAdd;											\
    PFN_LDAPMEP_ENCODE_REQ			LdapEnReqAbandon;										\
    PFN_LDAPMEP_ENCODE_REQ			LdapEnReqBind;											\
	PFN_LDAPMEP_ENCODE_REQ			LdapEnReqUnBind;										\
    PFN_LDAPMEP_ENCODE_REQ			LdapEnReqCompare;										\
    PFN_LDAPMEP_ENCODE_REQ			LdapEnReqDel;											\
    PFN_LDAPMEP_ENCODE_REQ			LdapEnReqExt;											\
    PFN_LDAPMEP_ENCODE_REQ			LdapEnReqModDN;											\
    PFN_LDAPMEP_ENCODE_REQ			LdapEnReqModify;										\
    PFN_LDAPMEP_ENCODE_REQ			LdapEnReqSearch;										\
    PFN_LDAPMEP_DECODE_RES			LdapDeResAdd;											\
	PFN_LDAPMEP_DECODE_RES			LdapDeResDel;											\
	PFN_LDAPMEP_DECODE_RES			LdapDeResModDn;											\
	PFN_LDAPMEP_DECODE_RES			LdapDeResCompare;										\
	PFN_LDAPMEP_DECODE_RES			LdapDeResExt;											\
	PFN_LDAPMEP_DECODE_RES			LdapDeResModify;										\
	PFN_LDAPMEP_DECODE_RES			LdapDeResBind;											\
	PFN_LDAPMEP_DECODE_RES			LdapDeResSearch;										\
	PFN_LDAP_MEP_SET_MSG_ID			LdapMepSetMsgId;										\
	PFN_LDAP_MEP_SET_LDAP_STR		LdapMepSetLdapDn;										\
	PFN_LDAP_MEP_SET_LDAP_STR		LdapMepSetLdapOid;										\
	PFN_LDAP_MEP_SET_LDAP_STR		LdapMepSetLdapStr;										\
	PFN_LDAP_MEP_SET_ATTR			LdapMepSetAttr;											\
	PFN_LDAP_MEP_SET_ATTR			LdapMepGetPartAttr;										\
	PFN_LDAP_MEP_SET_ATTR			LdapMepSetAva;											\
	PFN_LDAP_MEP_SET_ATTR			LdapMepSetMod;											\
	PFN_LDAP_MEP_SET_ATTR_TYP		LdapMepSetAttrTyp;										\
	PFN_LDAP_MEP_SET_ATTR			LdapMepSetAttrVal;										\
	PFN_LDAP_MEP_SET_ATTR			LdapMepSetAttrDescLst;									\
	PFN_LDAP_MEP_SET_ATTR			LdapMepGetAttrVal;										\
	PFN_LDAP_MEP_SET_BOOL			LdapMepSetBool;											\
	PFN_LDAP_MEP_REQ				LdapMepReqAdd;											\
	PFN_LDAP_MEP_REQ				LdapMepReqDel;											\
	PFN_LDAP_MEP_REQ				LdapMepReqAbandon;										\
	PFN_LDAP_MEP_REQ				LdapMepReqCompare;										\
	PFN_LDAP_MEP_REQ				LdapMepReqModDN;										\
	PFN_LDAP_MEP_REQ				LdapMepReqBind;											\
	PFN_LDAP_MEP_REQ				LdapMepReqUnBind;										\
	PFN_LDAP_MEP_REQ				LdapMepReqModify;										\
	PFN_LDAP_MEP_REQ				LdapMepReqExt;											\
	PFN_LDAP_MEP_REQ				LdapMepReqSearch;										\
	PFN_LDAP_MEP_RES				LdapMepResAdd;											\
	PFN_LDAP_MEP_RES				LdapMepResDel;											\
	PFN_LDAP_MEP_RES				LdapMepResModDn;										\
	PFN_LDAP_MEP_RES				LdapMepResCompare;										\
	PFN_LDAP_MEP_RES				LdapMepResExt;											\
	PFN_LDAP_MEP_RES				LdapMepResModify;										\
	PFN_LDAP_MEP_RES				LdapMepResBind;											\
	PFN_LDAP_MEP_RES				LdapMepResSearch;										\
	PFN_LDAP_MEP_RES				LdapMepResSrcEntry;										\
	PFN_LDAP_MEP_RES				LdapMepResSrcRef;										\
	PFN_LDAP_MEP_RES				LdapMepResSrcDn;										\
	PFN_LDAP_MEP_GET_FILTER			LdapMepGetFilt;											\
	PFN_LDAP_MEP_GET_FILTER			LdapMepGetFiltChoice;									\
	PFN_LDAP_MEP_GET_FILTER			LdapMepGetFiltAnd;										\
	PFN_LDAP_MEP_GET_FILTER			LdapMepGetFiltOr;										\
	PFN_LDAP_MEP_GET_FILTER			LdapMepGetFiltNot;										\
	PFN_LDAP_MEP_GET_FILTER			LdapMepGetFiltEqual;									\
	PFN_LDAP_MEP_GET_FILTER			LdapMepGetFiltItem;										\
	PFN_LDAP_MEP_GET_FILTER			LdapMepGetFiltAV;										\
	PFN_LDAP_MEP_GET_FILTER			LdapMepGetFiltExt;										\
	PFN_LDAP_MEP_GET_FILTER			LdapMepGetFiltSubStr;									\
	PFN_LDAP_MEP_GET_MSG_TYPE		LdapMepGetMsgType;										\
	PFN_LDAP_MEP_GET_MSG_ID			LdapMepGetMsgId;										\
	PFN_LDAP_MEP_GET_LDAP_DN		LdapMepGetLdapDn;										\
	PFN_LDAP_MEP_GET_LDAP_DN		LdapMepGetString;										\
	PFN_LDAP_MEP_GET_RES_CD			LdapMepGetResCode;										\
	PFN_LDAP_MEP_GET_SIZE			LdapMepGetSize;											\
	PFN_LDAP_MEP_SET_URL			LdapMepSetUrl;											\
	/* end of object class content */                                                       \




typedef  struct
_LDAP_MESSAGE_ELEMENT_PARSER_OBJECT
{
    LDAP_MESSAGE_ELEMENT_PARSER_CLASS_CONTENT
}
LDAP_MEP_OBJECT,  *PLDAP_MEP_OBJECT;

#define  ACCESS_LDAP_MEP_INTERFACE(p)         \
         ACCESS_CONTAINER(p, LDAP_MEP_INTERFACE, Linkage)

#endif
