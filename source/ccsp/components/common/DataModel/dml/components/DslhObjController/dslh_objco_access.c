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

    module:	dslh_objco_access.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh Obj Controller Object.

        *   DslhObjcoEngage
        *   DslhObjcoValidate
        *   DslhObjcoCommit
        *   DslhObjcoRollback
        *   DslhObjcoGetParamValueByName

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin  Zhu

    ---------------------------------------------------------------

    revision:

        10/03/2005    initial revision.
        10/07/2010    Bin added universal function support of "GetParamValueByName".
        09/22/2011    Bin added GetBulkParamValue apis;

**********************************************************************/


#include "dslh_objco_global.h"
#include "dslh_vareo_exported_api.h"
#include "dslh_varro_exported_api.h"
#include "dslh_varro_interface.h"
#include "slap_vco_internal_api.h"
#include "dslh_varro_internal_api.h"
#include "safec_lib_common.h"

ULONG g_currentBsUpdate;
char g_currentParamFullName[512];

char*
DslhVarroMacAddrListToString (SLAP_UCHAR_ARRAY* mac_addr_list);

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object (e.g. populate the
        instance context by calling APIs provided by external interface).

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjcoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject    = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;

    if( pMyObject->hInsContext == NULL)
    {
        pMyObject->hInsContext = pMyObject->hParentInsContext;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhObjcoValidate
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDslhRvqIf,
                char**                      ppFaultParamName
            );

    description:

        This function is called to validate the previous changes were
        made to the parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDslhRvqIf
                Specifies the DSLH_RVQ interface to be used.

                char**                      ppFaultParamName
                Returns the name of the problematic parameter if the
                result is FALSE.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
DslhObjcoValidate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDslhRvqIf,
        char**                      ppFaultParamName
    )
{
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject          = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;
    PDSLH_RVQ_INTERFACE             pDslhRvqIf         = (PDSLH_RVQ_INTERFACE        )hDslhRvqIf;
    PDSLH_TR69_INTERFACE            pInterface         = (PDSLH_TR69_INTERFACE)pMyObject->hDslhTr69If;
    char                            pFaultName[128]    = { 0 };
    ULONG                           uLength            = 128;
    PDSLH_OBJ_RECORD_OBJECT         pDslhObjRecord     = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hDslhObjRecord;
    PDSLH_CWMP_PARAM_DESCR          pParamDescr        = NULL; 
    PSINGLE_LINK_ENTRY              pSLinkEntry        = (PSINGLE_LINK_ENTRY     )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity         = (PDSLH_OBJ_ENTITY_OBJECT)pDslhObjRecord->hDslhObjEntity;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity    = (PDSLH_VAR_ENTITY_OBJECT)NULL;
    char                            pFullName[512]     = { 0 };
    char*                           pCallingName       = NULL;
    char**                          ppNameArray        = NULL;
    PSLAP_VARIABLE*                 ppValueArray       = NULL;
    ULONG                           uTotal             = 0;
    ULONG                           uCount             = 0;
    ULONG                           uErrorIndex        = 0;
    BOOL                            bRet               = FALSE;
    errno_t                         rc                 = -1;

    *ppFaultParamName = NULL;

    if( pInterface == NULL)
    {
        return TRUE;
    }

    /* 
     * hParamArray is not reliable, we need to find a way to enumerate the parameters 
     */
    if( pObjEntity == NULL)
    {
        return FALSE;
    }

    /* check VarEntity instead of ParamArray */
    pSLinkEntry = AnscQueueGetFirstEntry(&pObjEntity->VareoTable);

    if( pInterface->SetEntryBulk != NULL)
    {
        /* pre-allocate arrays */
        uTotal = AnscQueueQueryDepth(&pObjEntity->VareoTable);
	/*CID: 74649 Sizeof not portable*/
        ppNameArray  = (char**)AnscAllocateMemory( uTotal * sizeof(char*));
        if( !ppNameArray ) /*RDKB-5790 ; CID-33099, CID- 33417; NULL check after allocation*/
        {
            return FALSE;
        }

        ppValueArray = (PSLAP_VARIABLE*)AnscAllocateMemory(uTotal * sizeof(PSLAP_VARIABLE));
        if( !ppValueArray )
        {
            AnscFreeMemory(ppNameArray);
            return FALSE;
        }

        while ( pSLinkEntry )
        {
            pChildVarEntity = ACCESS_DSLH_VAR_ENTITY_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

            pParamDescr = pChildVarEntity->ParamDescr;

            if( pParamDescr != NULL && pParamDescr->Name != NULL && !pParamDescr->bPersistent &&
                pParamDescr->bWritable && pDslhRvqIf->IsParamUpdated(pDslhRvqIf->hOwnerContext, pParamDescr->Name))
            {   
                ppNameArray[uCount]   = pParamDescr->Name;
                ppValueArray[uCount]  = pDslhRvqIf->GetParamValue(pDslhRvqIf->hOwnerContext, pParamDescr->Name);

                uCount ++;
            }
        }

        if( uCount > 0)
        {
            bRet = 
                pInterface->SetEntryBulk
                    (
                        pMyObject->hInsContext,
                        ppNameArray,
                        ppValueArray,
                        uCount,
                        &uErrorIndex
                    );

            if( !bRet)
            {
                *ppFaultParamName = AnscCloneString(ppNameArray[uErrorIndex]);
            }
        }

        AnscFreeMemory(ppNameArray);
        AnscFreeMemory(ppValueArray);

        if( !bRet)
        {
            return FALSE;
        }
    }
    else
    {
        while ( pSLinkEntry )
        {
            pChildVarEntity = ACCESS_DSLH_VAR_ENTITY_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

            pParamDescr = pChildVarEntity->ParamDescr;

            if( pParamDescr != NULL && pParamDescr->Name != NULL && !pParamDescr->bPersistent &&
                pParamDescr->bWritable && pDslhRvqIf->IsParamUpdated(pDslhRvqIf->hOwnerContext, pParamDescr->Name))
            {   
                if( pInterface->bUseFullName)
                {
                    rc = sprintf_s(pFullName, sizeof(pFullName), "%s%s", pDslhObjRecord->FullName, pParamDescr->Name);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                        return FALSE;
                    }
                    pCallingName = pFullName;
                }
                else
                {
                    pCallingName = pParamDescr->Name;
                }

                g_currentBsUpdate = pChildVarEntity->bsUpdate;
                AnscTraceWarning(("DslhObjcoValidate: %s, bsUpdate = %lu\n\n", pCallingName, pChildVarEntity->bsUpdate));

                snprintf(g_currentParamFullName, sizeof(g_currentParamFullName),"%s%s",pDslhObjRecord->FullName, pParamDescr->Name);

                switch ( pChildVarEntity->Syntax )
                {
                    case    SLAP_VAR_SYNTAX_bool :

                            {
                                if( pInterface->SetEntryParamBool != NULL)
                                {
                                    if(!pInterface->SetEntryParamBool(pMyObject->hInsContext, pCallingName, pDslhRvqIf->GetParamValueBool(pDslhRvqIf->hOwnerContext, pParamDescr->Name)))
                                    {
                                        *ppFaultParamName = AnscCloneString(pParamDescr->Name);

                                        return FALSE;
                                    }
                                }
                            }

                            break;

                    case    SLAP_VAR_SYNTAX_int :

                            {
                                if( pInterface->SetEntryParamInt != NULL)
                                {
                                    if(!pInterface->SetEntryParamInt(pMyObject->hInsContext, pCallingName, pDslhRvqIf->GetParamValueInt(pDslhRvqIf->hOwnerContext, pParamDescr->Name)))
                                    {
                                        *ppFaultParamName = AnscCloneString(pParamDescr->Name);

                                        return FALSE;
                                    }
                                }
                            }

                            break;

                    case    SLAP_VAR_SYNTAX_uint32 :

                            if( pInterface->SetEntryParamUlong != NULL)
                            {
                                if(!pInterface->SetEntryParamUlong(pMyObject->hInsContext, pCallingName, pDslhRvqIf->GetParamValueUint32(pDslhRvqIf->hOwnerContext, pParamDescr->Name)))
                                {
                                    *ppFaultParamName = AnscCloneString(pParamDescr->Name);

                                    return FALSE;
                                }
                            }

                            break;

                    case    SLAP_VAR_SYNTAX_ucharArray :
                    case    SLAP_VAR_SYNTAX_string :

                            if( pInterface->SetEntryParamString != NULL)
                            {
                                if(!pInterface->SetEntryParamString(pMyObject->hInsContext, pCallingName, pDslhRvqIf->GetParamValueString(pDslhRvqIf->hOwnerContext, pParamDescr->Name)))
                                {
                                    *ppFaultParamName = AnscCloneString(pParamDescr->Name);

                                    return FALSE;
                                }
                            }

                            break;

                    case    SLAP_VAR_SYNTAX_uint32Array :
                    case    SLAP_VAR_SYNTAX_handle :

                            AnscTrace("Warning: unsupported param '%s' with datatype %s.\n", pCallingName, pParamDescr->DataType);

                            break;
                }
                pDslhRvqIf->ClearParamUpdated(pDslhRvqIf->hOwnerContext, pParamDescr->Name);
            }
        }
    }

    if( pInterface->bUseFullName)
    {
        if( pInterface->Methods.MethodWithFullName.Validate != NULL)
        {
            if( !pInterface->Methods.MethodWithFullName.Validate(pMyObject->hInsContext, pDslhObjRecord->FullName, pFaultName, &uLength))
            {
                *ppFaultParamName = AnscCloneString(pFaultName);

                return FALSE;
            }
        }
    }
    else
    {
        if( pInterface->Methods.MethodWithLastName.Validate != NULL)
        {
            if( !pInterface->Methods.MethodWithLastName.Validate(pMyObject->hInsContext, pFaultName, &uLength))
            {
                *ppFaultParamName = AnscCloneString(pFaultName);

                return FALSE;
            }
        }
    }

    return  TRUE; 
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoCommit
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDslhRvqIf
            );

    description:

        This function is called to commit the previous changes were
        made to the parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDslhRvqIf
                Specifies the DSLH_RVQ interface to be used.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjcoCommit
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDslhRvqIf
    )
{
    UNREFERENCED_PARAMETER(hDslhRvqIf);
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject      = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;
    PDSLH_TR69_INTERFACE            pInterface     = (PDSLH_TR69_INTERFACE)pMyObject->hDslhTr69If;
    PDSLH_OBJ_RECORD_OBJECT         pDslhObjRecord = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hDslhObjRecord;

    if( pInterface == NULL)
    {
        return ANSC_STATUS_SUCCESS;
    }

    if (pMyObject->bCommitting)
    {
        CcspTraceWarning(("%s already in committing: %s\n", __FUNCTION__, pDslhObjRecord->FullName));
        return ANSC_STATUS_SUCCESS;
    }
    pMyObject->bCommitting = TRUE;

    if( pInterface->bUseFullName)
    {
        if( pInterface->Methods.MethodWithFullName.Commit != NULL)
        {
            returnStatus = pInterface->Methods.MethodWithFullName.Commit(pMyObject->hInsContext, pDslhObjRecord->FullName);
        }
    }
    else
    {
        if( pInterface->Methods.MethodWithLastName.Commit != NULL)
        {

            CcspTraceWarning(("Before committing:%s\n", pDslhObjRecord->FullName));
            returnStatus = pInterface->Methods.MethodWithLastName.Commit(pMyObject->hInsContext);
            CcspTraceWarning(("After committing:%s:%lu\n", pDslhObjRecord->FullName, returnStatus));
        }
    }

    pMyObject->bCommitting = FALSE;
    return  returnStatus;

}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoRollback
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to rollback the previous changes were
        made to the parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjcoRollback
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject    = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;
    PDSLH_TR69_INTERFACE            pInterface   = (PDSLH_TR69_INTERFACE)pMyObject->hDslhTr69If;
    PDSLH_OBJ_RECORD_OBJECT         pDslhObjRecord     = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hDslhObjRecord;

    if( pInterface == NULL)
    {
        return ANSC_STATUS_SUCCESS;
    }

    if( pInterface->bUseFullName)
    {
        if( pInterface->Methods.MethodWithFullName.Rollback != NULL)
        {
            return pInterface->Methods.MethodWithFullName.Rollback(pMyObject->hInsContext, pDslhObjRecord->FullName);
        }
    }
    else
    {
        if( pInterface->Methods.MethodWithLastName.Rollback != NULL)
        {
            return pInterface->Methods.MethodWithLastName.Rollback(pMyObject->hInsContext);
        }
    }

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoGetParamValueByName
            (
                ANSC_HANDLE                 hThisObject
                char*                       pName,
                PSLAP_VARIABLE              pVariable
            );

    description:

        This function is called to retrieve parameter value specified by
        the name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pName,
                The parameter name;

                PSLAP_VARIABLE              pVariable
                The SLAP VARIABLE handle

    return:     status of operation.

**********************************************************************/
void
afterGetValue
    (
        PDSLH_VAR_ENTITY_OBJECT         pVarEntity,
        PSLAP_VARIABLE                  pSlapVariable
    )
{
    PDSLH_VAREO_TOKEN_VALUE         pVareoTokenValue     = (PDSLH_VAREO_TOKEN_VALUE    )NULL;
    SLAP_UCHAR_ARRAY*               pUcharArray          = (SLAP_UCHAR_ARRAY*          )NULL;
    PANSC_UNIVERSAL_TIME            pUniversalTime       = (PANSC_UNIVERSAL_TIME       )NULL;
    ULONG                           uLength              = 0;

    if( !pVarEntity || !pSlapVariable)
    {
        return;
    }

    /*
     * Most TR-069 parameters are defined as strings. To make the framework more flexible, we use
     * the "ContentType" to distinguish different type of strings:
     *
     *      - All IP addresses and subnet masks are represented as strings in IPv4 dotted-
     *        decimal notation. Note that there is no IPv6 support at this time in the
     *        parameter list described in TR-069 appendix.
     *      - All MAC addresses are represented as strings of 12 hexadecimal digits
     *        (digits 0-9, letters A-F or a-f) displayed as six pairs of digits separated
     *        by colons.
     */
    if ( pVarEntity->DataType == DSLH_CWMP_DATA_TYPE_string )
    {
        switch ( pVarEntity->ContentType )
        {
            case    SLAP_CONTENT_TYPE_MAPPED :

                    if ( pSlapVariable->Syntax != SLAP_VAR_SYNTAX_uint32 )
                    {
                        /*
                         * Impossible, the registration information was wrong...
                         */
                        break;
                    }
                    else
                    {
                        pVareoTokenValue =
                            DslhVareoGetTokenValueByCode
                                (
                                    (ANSC_HANDLE)pVarEntity,
                                    pSlapVariable->Variant.varUint32
                                );

                        if ( pVareoTokenValue )
                        {
                            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                            pSlapVariable->Variant.varString = AnscCloneString(pVareoTokenValue->String);
                        }
                        else
                        {
                            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                            pSlapVariable->Variant.varString = AnscCloneString("");
                        }
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP4_ADDR :
            case    SLAP_CONTENT_TYPE_SUBNET_MASK:

                    if ( pSlapVariable->Syntax != SLAP_VAR_SYNTAX_uint32 )
                    {
                        /*
                         * Impossible, the registration information was wrong...
                         */
                        break;
                    }
                    else
                    {
                        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                        pSlapVariable->Variant.varString =
                            SlapVcoIp4AddrToString
                                (
                                    NULL,
                                    pSlapVariable->Variant.varUint32
                                );
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP6_ADDR :

                    if ( pSlapVariable->Syntax != SLAP_VAR_SYNTAX_ucharArray )
                    {
                        /*
                         * Impossible, the registration information was wrong...
                         */
                        AnscTraceWarning(("Invalid syntax definition for ip6_addr type.\n\n"));

                        break;
                    }
                    else
                    {
                        pUcharArray = pSlapVariable->Variant.varUcharArray;

                        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;

                        if( pUcharArray != NULL)
                        {
                            pSlapVariable->Variant.varString =
                                SlapVcoIp6AddrToString
                                    (
                                        (ANSC_HANDLE)NULL,
                                        pUcharArray
                                    );

                            SlapFreeVarArray(pUcharArray);
                        }
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP4_ADDR_WILDCARD :

                    if ( pSlapVariable->Syntax != SLAP_VAR_SYNTAX_uint32 )
                    {
                        /*
                         * Impossible, the registration information was wrong...
                         */
                        break;
                    }
                    else
                    {
                        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                        pSlapVariable->Variant.varString =
                            SlapVcoIp4AddrToString2
                                (
                                    NULL,
                                    pSlapVariable->Variant.varUint32
                                );
                    }

                    break;

            case    SLAP_CONTENT_TYPE_MAC_ADDR :

                    if ( pSlapVariable->Syntax != SLAP_VAR_SYNTAX_ucharArray )
                    {
                        /*
                         * Impossible, the registration information was wrong...
                         */
                        break;
                    }
                    else
                    {
                        pUcharArray = pSlapVariable->Variant.varUcharArray;

                        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;

                        if( pUcharArray != NULL)
                        {
                            pSlapVariable->Variant.varString =
                                SlapVcoMacAddrToString2
                                    (
                                        NULL,
                                        pUcharArray
                                    );

                            SlapFreeVarArray(pUcharArray);
                        }
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP4_ADDR_LIST :

                    if ( pSlapVariable->Syntax != SLAP_VAR_SYNTAX_uint32Array )
                    {
                        break;
                    }
                    else
                    {
                        pUcharArray = pSlapVariable->Variant.varUcharArray;

                        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                        if( pUcharArray != NULL)
                        {
                            pSlapVariable->Variant.varString =
                                SlapVcoIp4AddrListToString
                                    (
                                        NULL,
                                        pSlapVariable->Variant.varUint32Array
                                    );

                            SlapFreeVarArray(pUcharArray);
                        }
                    }

                    break;

            case    SLAP_CONTENT_TYPE_MAC_ADDR_LIST :

                    if ( pSlapVariable->Syntax != SLAP_VAR_SYNTAX_ucharArray )
                    {
                        break;
                    }
                    else
                    {
                        pUcharArray = pSlapVariable->Variant.varUcharArray;

                        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;

                        if( pUcharArray != NULL)
                        {
                            pSlapVariable->Variant.varString =
                                DslhVarroMacAddrListToString
                                    (
                                        pSlapVariable->Variant.varUcharArray
                                    );

                            SlapFreeVarArray(pUcharArray);
                        }
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP6_ADDR_LIST :

                    if ( pSlapVariable->Syntax != SLAP_VAR_SYNTAX_ucharArray )
                    {
                        /*
                         * Impossible, the registration information was wrong...
                         */
                        break;
                    }
                    else
                    {
                        pUcharArray = pSlapVariable->Variant.varUcharArray;

                        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;

                        if( pUcharArray != NULL)
                        {
                            pSlapVariable->Variant.varString =
                                SlapVcoIp6AddrListToString
                                    (
                                        (ANSC_HANDLE)NULL,
                                        pSlapVariable->Variant.varUcharArray
                                    );

                            SlapFreeVarArray(pUcharArray);
                        }
                    }

                    break;

            default :

                    break;
        }
    }
    else if ( pVarEntity->DataType == DSLH_CWMP_DATA_TYPE_dateTime )
    {
        switch ( pVarEntity->ContentType )
        {
            case    SLAP_CONTENT_TYPE_CALENDAR_TIME :

                    if ( pSlapVariable->Syntax != SLAP_VAR_SYNTAX_handle || NULL == pSlapVariable->Variant.varHandle)
                    {
                        /* According to WT151, the unknown datetime is supposed to be '0001-01-01T00:00:00Z/ */
                        AnscTrace("++++Unknown/NULL DateTime, UnknownTime value will be applied.\n");

                        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                        pSlapVariable->Variant.varString = AnscCloneString("0001-01-01T00:00:00Z");
                    }
                    else
                    {
                        pUniversalTime = pSlapVariable->Variant.varHandle;

                        /*
                         * Per spec, the year value less than 1000 must be interpreted as a relative time
                         * since boot.
                         */
                        if( pUniversalTime->Year < 1000 )
                        {
                            if( pUniversalTime->DayOfMonth == 0 && pUniversalTime->Year == 0 &&
                                pUniversalTime->Month      == 0)
                            {

                                pUniversalTime->DayOfMonth =  1;
                                pUniversalTime->Month      =  1;
                                pUniversalTime->Year       =  1;
                            }
                        }

                        pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;
                        pSlapVariable->Variant.varString =
                            SlapVcoCalendarTimeToString
                                (
                                    NULL,
                                    (ANSC_HANDLE)pUniversalTime
                                );

                        /*
                         * Relative time doesn't need the timezone suffix
                         */
                        if( pUniversalTime->Year < 1000 )
                        {
                            if( pUniversalTime->Year == 1       &&
                                pUniversalTime->Month == 1      &&
                                pUniversalTime->DayOfMonth == 1 &&
                                pUniversalTime->Hour == 0       &&
                                pUniversalTime->Minute == 0     &&
                                pUniversalTime->Second == 0 )
                            {
                                /* unknown time, keep the last 'Z' */
                            }
                            else
                            {
                                /* We have to get rid of the last 'Z' */
                                uLength = AnscSizeOfString(pSlapVariable->Variant.varString);

                                if( pSlapVariable->Variant.varString[uLength - 1] == 'Z')
                                {
                                    pSlapVariable->Variant.varString[uLength - 1] = 0;
                                }
                            }
                        }

                        AnscFreeMemory(pUniversalTime);
                    }

                    /* Hint this is a calendar time object rather than a string */
                    pSlapVariable->ContentType = SLAP_CONTENT_TYPE_CALENDAR_TIME;

                    break;

            default :

                    break;
        }
    }
    else if ( pVarEntity->DataType == DSLH_CWMP_DATA_TYPE_base64 )
    {
        if( pSlapVariable->Syntax   == SLAP_VAR_SYNTAX_ucharArray)
        {
            pUcharArray = pSlapVariable->Variant.varUcharArray;

            pSlapVariable->Syntax            = SLAP_VAR_SYNTAX_string;

            if( pUcharArray != NULL)
            {
                pSlapVariable->Variant.varString =
                    (char*)AnscBase64Encode
                        (
                            pUcharArray->Array.arrayUchar,
                            pUcharArray->VarCount
                        );

                SlapFreeVarArray(pUcharArray);
            }
        }
    }

    /* COSA DM plugin may return empty string instead, in such case, return NULL rather than empty string */
    if ( pSlapVariable->Syntax == SLAP_VAR_SYNTAX_string && 
         pSlapVariable->Variant.varString                &&
         !pSlapVariable->Variant.varString[0] )
    {
        AnscFreeMemory(pSlapVariable->Variant.varString);
        pSlapVariable->Variant.varString = NULL;
    }
}

BOOL
getChildTableCount
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pName,
        PULONG                      pUlong
    )
{
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject      = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord     = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hDslhObjRecord;
    PDSLH_OBJ_RECORD_OBJECT         pChildRecord   = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity= (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJCO_TABLE_OBJECT        pChildObjcoTable    = (PDSLH_OBJCO_TABLE_OBJECT   )NULL;
    char                            pTableName[64] = { 0 };

    AnscCopyMemory(pTableName, pName, AnscSizeOfString(pName) - AnscSizeOfString(TR69_NUMBER_OF_ENTRIES_STRING));

    pChildRecord = (PDSLH_OBJ_RECORD_OBJECT)
        pObjRecord->GetObjRecord(pObjRecord, pTableName);

    if( pChildRecord == NULL)
    {
        return FALSE;
    }

    pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT    )pChildRecord->hDslhObjEntity;

    if( !pChildObjEntity || pChildObjEntity->ObjDescr->Type != DSLH_CWMP_OBJECT_TYPE_table)
    {
        return FALSE;
    }

    pChildObjcoTable    = (PDSLH_OBJCO_TABLE_OBJECT   )pChildRecord->hDslhObjController;

    if( pChildObjcoTable == NULL)
    {
        return FALSE;
    }

    if ( pChildObjEntity->ObjDescr->bDynamic && pChildObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table)
    {
        if ( pChildObjcoTable->IsUpdated((ANSC_HANDLE)pChildObjcoTable) )
        {
            pChildRecord->DelAllObjRecords  ((ANSC_HANDLE)pChildRecord);
    
            pChildObjcoTable->Lock(pChildObjcoTable);
            pChildRecord->PopulateObjRecords((ANSC_HANDLE)pChildRecord);
            pChildRecord->PopulateVarRecords((ANSC_HANDLE)pChildRecord);
            pChildObjcoTable->Unlock(pChildObjcoTable);
        }
    }

    *pUlong = pChildObjcoTable->GetEntryObjectCount(pChildObjcoTable);

    return TRUE;
}


ANSC_STATUS
DslhObjcoGetParamValueByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pName,
        PSLAP_VARIABLE              pSlapVariable
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject      = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord     = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hDslhObjRecord;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity     = (PDSLH_VAR_ENTITY_OBJECT)NULL;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord     = (PDSLH_VAR_RECORD_OBJECT)NULL;
    PDSLH_TR69_INTERFACE            pInterface     = (PDSLH_TR69_INTERFACE)pMyObject->hDslhTr69If;
    char*                           pFind          = NULL;
    BOOL                            bReturn        = FALSE;

    /*AnscTraceVerbose(("DslhObjcoGetParamValueByName -- name %s\n", pName));*/
    
    /* check whether it's a count of table (xxxNumberOfEntries) or not */
    if(pSlapVariable->Syntax == SLAP_VAR_SYNTAX_uint32)
    {
        pFind = _ansc_strstr(pName, TR69_NUMBER_OF_ENTRIES_STRING);

        if( pFind != NULL && (ULONG)(pFind - pName) == AnscSizeOfString(pName) - AnscSizeOfString(TR69_NUMBER_OF_ENTRIES_STRING))
        {
            bReturn = getChildTableCount(pMyObject, pName, &pSlapVariable->Variant.varUint32);
            
            if(!bReturn)
            {
                /* there's no back-end support, return 0 */
                pSlapVariable->Variant.varUint32 = 0;

                /* 
                              * In order to deal with those xxxNumOfEntries parameter names which is not the same as TR-xxx,
                              * give "GetEntryParamUlong" a chance to do so.
                              */
            }
            else
            {
                return ANSC_STATUS_SUCCESS;
            }
        }
    }

    if( pInterface == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( pName == NULL || AnscSizeOfString(pName) == 0 || pSlapVariable == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( pInterface->GetEntryBulk != NULL)
    {
        return pMyObject->GetBulkParamValue(pMyObject, &pName, &pSlapVariable, 1);
    }

    snprintf(g_currentParamFullName, sizeof(g_currentParamFullName),"%s%s",pObjRecord->FullName, pName);

    switch ( pSlapVariable->Syntax )
    {
        case    SLAP_VAR_SYNTAX_bool :

                if( pInterface->GetEntryParamBool != NULL)
                {
                    bReturn = pInterface->GetEntryParamBool((ANSC_HANDLE)pMyObject->hInsContext,pName, (BOOL*)&pSlapVariable->Variant.varBool);
                }

                break;

        case    SLAP_VAR_SYNTAX_int :

                if( pInterface->GetEntryParamInt != NULL)
                {
                    bReturn = pInterface->GetEntryParamInt(pMyObject->hInsContext,pName, &pSlapVariable->Variant.varInt);
                }

                break;

        case    SLAP_VAR_SYNTAX_uint32 :

                if( pInterface->GetEntryParamUlong != NULL)
                {
                    bReturn = pInterface->GetEntryParamUlong(pMyObject->hInsContext,pName, &pSlapVariable->Variant.varUint32);
                }

                break;

        case    SLAP_VAR_SYNTAX_ucharArray :
        case    SLAP_VAR_SYNTAX_string :

                if( pInterface->GetEntryParamString != NULL)
                {
                    char pTempBuf[1024];
                    ULONG uLength;

                    memset (pTempBuf, 0, sizeof(pTempBuf));
                    uLength = sizeof(pTempBuf) - 1;

                    returnStatus = pInterface->GetEntryParamString(pMyObject->hInsContext,pName, pTempBuf, &uLength);

                    if( returnStatus != ANSC_STATUS_SUCCESS && returnStatus != ANSC_STATUS_FAILURE)
                    {
                        /* size is not big enough */
                        pSlapVariable->Variant.varString = (char*)AnscAllocateMemory(uLength + 1);

                        if( pSlapVariable->Variant.varString == NULL)
                        {
                            return ANSC_STATUS_FAILURE;
                        }

                        return pInterface->GetEntryParamString(pMyObject->hInsContext, pName, pSlapVariable->Variant.varString, &uLength);
                    }
                    else
                    {
                        pSlapVariable->Variant.varString = AnscCloneString(pTempBuf);

                        return ANSC_STATUS_SUCCESS;
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_uint32Array :
        case    SLAP_VAR_SYNTAX_handle :

                AnscTraceEntry(("SLAP Variable '%s' is not supported, type = %lu.\n", pName, pSlapVariable->Syntax));
                break;
    }

    if( bReturn)
    {   
        pVarRecord = (PDSLH_VAR_RECORD_OBJECT)pObjRecord->GetVarRecord(pObjRecord, pName);
        pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;
    
        afterGetValue(pVarEntity, pSlapVariable);

        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_FAILURE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
            DslhObjcoGetBulkParamValue
                (
                    ANSC_HANDLE                 hThisObject,
                    char**                      pParamArray,
                    PSLAP_VARIABLE*             ppVarArray,
                    ULONG                       ulArraySize
                );

    description:

        This function is called to retrieve an array of parameter values.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char**                      pParamArray,
                The parameter name array;

                PSLAP_VARIABLE*             ppVarArray,
                The parameter value array;

                ULONG                       ulArraySize
                The size of the array;

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhObjcoGetBulkParamValue
    (
        ANSC_HANDLE                 hThisObject,
        char**                      pParamArray,
        PSLAP_VARIABLE*             ppVarArray,
        ULONG                       ulArraySize
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject      = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord     = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hDslhObjRecord;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity     = (PDSLH_VAR_ENTITY_OBJECT)NULL;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord     = (PDSLH_VAR_RECORD_OBJECT)NULL;
    PDSLH_TR69_INTERFACE            pInterface     = (PDSLH_TR69_INTERFACE)pMyObject->hDslhTr69If;
    BOOL                            bReturn        = FALSE;
    char*                           pFind          = NULL;
    ULONG                           i              = 0;
    ULONG                           uCount         = 0;
    char**                          pParamCopy     = NULL;
    PSLAP_VARIABLE*                 ppVarCopy      = NULL;

    if( !pParamArray || !ppVarArray)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( pInterface != NULL && pInterface->GetEntryBulk != NULL)
    {
        /* make a copy of the array */
        pParamCopy = (char**)AnscAllocateMemory(sizeof(char*) * ulArraySize);
        if( !pParamCopy )/*RDKB-5790 ; CID-32895 & 33287, NULL check after allocation*/
        {
            return ANSC_STATUS_FAILURE;
        }

        ppVarCopy  = (PSLAP_VARIABLE*)AnscAllocateMemory(sizeof(PSLAP_VARIABLE) * ulArraySize);
        if( !ppVarCopy )
        {
            AnscFreeMemory(pParamCopy);
            return ANSC_STATUS_FAILURE;
        }

        for( i = 0; i < ulArraySize; i ++)
        {
            if(ppVarArray[i]->Syntax == SLAP_VAR_SYNTAX_uint32)
            {
                /* move "xxxNumberOfEntries" parameter out of the array */
                pFind = _ansc_strstr(pParamArray[i], TR69_NUMBER_OF_ENTRIES_STRING);

                if( pFind != NULL && (ULONG)(pFind - pParamArray[i]) == AnscSizeOfString(pParamArray[i]) - AnscSizeOfString(TR69_NUMBER_OF_ENTRIES_STRING))
                {
                    bReturn = getChildTableCount(pMyObject, pParamArray[i], &ppVarArray[i]->Variant.varUint32);
            
                    if(!bReturn)
                    {
                        /* there's no back-end support, return 0 */
                        ppVarArray[i]->Variant.varUint32 = 0;
                    }

                    continue;
                }
            }

            /* add to the copied array */
            pParamCopy[uCount]         = pParamArray[i];
            ppVarCopy[uCount]          = ppVarArray[i];
            uCount ++;
        }

        /* we need to deal with special Content types */
        if( uCount > 0)
        {
            returnStatus =
                pInterface->GetEntryBulk(pMyObject->hInsContext, pParamCopy, ppVarCopy, uCount);

            for( i = 0; i < uCount; i ++)
            {
                pVarRecord = (PDSLH_VAR_RECORD_OBJECT)pObjRecord->GetVarRecord(pObjRecord, pParamCopy[i]);
                pVarEntity = (PDSLH_VAR_ENTITY_OBJECT)pVarRecord->hDslhVarEntity;
        
                afterGetValue(pVarEntity, ppVarCopy[i]);
            }
        }

        AnscFreeMemory(pParamCopy);
        AnscFreeMemory(ppVarCopy);
    }
    else
    {
        for( i = 0; i < ulArraySize; i ++)
        {
            returnStatus = pMyObject->GetParamValueByName(pMyObject, pParamArray[i], ppVarArray[i]);

            if( returnStatus != ANSC_STATUS_SUCCESS)
            {
                AnscTraceWarning(("Failed to get parameter value of '%s'.\n", pParamArray[i]));

                /* BulkGet should not stop even if the parameter value fetch fails */
	        continue;
	    }
        }
    }

    return ANSC_STATUS_SUCCESS;
}
