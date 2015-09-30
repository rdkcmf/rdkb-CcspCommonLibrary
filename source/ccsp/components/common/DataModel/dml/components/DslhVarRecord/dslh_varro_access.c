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

    module: dslh_varro_access.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2005
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh Var Record Object.

        *   DslhVarroGetValue
        *   DslhVarroTstValue
        *   DslhVarroSetValue
        *   DslhVarroSynValue
        *   DslhVarroSaveOldValue
        *   DslhVarroCommitChange
        *   DslhVarroCancelChange

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/28/05    initial revision.
        11/12/06    Bin Zhu added the unknown datetime value;

**********************************************************************/


#include "dslh_varro_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        DslhVarroGetValue
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     parameter value.

**********************************************************************/

SLAP_VARIABLE*
DslhVarroGetValue
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PDSLH_VAR_RECORD_OBJECT         pMyObject            = (PDSLH_VAR_RECORD_OBJECT    )hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity           = (PDSLH_VAR_ENTITY_OBJECT    )pMyObject->hDslhVarEntity;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hDslhObjRecord;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity           = (PDSLH_OBJ_ENTITY_OBJECT    )pObjRecord->hDslhObjEntity;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController       = (PDSLH_OBJ_CONTROLLER_OBJECT)pObjRecord->hDslhObjController;
    /*PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoParameter = (PPOAM_IREP_FOLDER_OBJECT   )pMyObject->hIrepFoParameter;*/
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController   = (PDSLH_CPE_CONTROLLER_OBJECT)pObjRecord->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase     = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_VAREO_TOKEN_VALUE         pVareoTokenValue     = (PDSLH_VAREO_TOKEN_VALUE    )NULL;
    PSLAP_VARIABLE                  pSlapVariable        = (PSLAP_VARIABLE             )NULL;
    void*                           pfnGetValueMethod    = (void*                      )NULL;
    SLAP_UCHAR_ARRAY*               pUcharArray          = (SLAP_UCHAR_ARRAY*          )NULL;
    PANSC_UNIVERSAL_TIME            pUniversalTime       = (PANSC_UNIVERSAL_TIME       )NULL;
    ULONG                           uLength              = 0;

    /*ULONG                           permission              = SYS_RRO_ACCESS_MODE_ALL;*/
    ULONG                           contentType             = 0;
    /*SYS_RRO_RENDER_ATTR             renderAttr              = { 0 };*/
    ULONG                           recordType              = 0;


    /*renderAttr.ContentType = SYS_RECORD_CONTENT_DEFAULT;*/


    if ( pVarEntity->ParamDescr->bPersistent && pObjEntity->ObjDescr->bGroupCommit && pMyObject->TempParamValue )
    {
        SlapAllocVariable(pSlapVariable);

        if ( !pSlapVariable )
        {
            return  NULL;
        }
        else
        {
            SlapCloneVariable(pMyObject->TempParamValue, pSlapVariable);
        }

        return  pSlapVariable;
    }
    else if ( pVarEntity->ParamDescr->bPersistent && TRUE /* (pVarEntity->CallEntry_GetValue == 0) */)
    {
        if( FALSE/*pPoamIrepFoParameter*/ )
        {
            /*
            pSlapVariable =
                (PSLAP_VARIABLE)pPoamIrepFoParameter->GetRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoParameter,
                        DSLH_VARRO_RR_NAME_Value,
                        NULL
                    );
            */
        }
        else
        {
            /* construct a default value */
            pSlapVariable = (PSLAP_VARIABLE)AnscAllocateMemory(sizeof(SLAP_VARIABLE));

            if( pSlapVariable != NULL)
            {
                pSlapVariable->Syntax          = pVarEntity->Syntax;

                if( pSlapVariable->Syntax == SLAP_VAR_SYNTAX_bool)
                {
                    pSlapVariable->Variant.varBool = TRUE;
                }
                else if( pSlapVariable->Syntax == SLAP_VAR_SYNTAX_string)
                {
                    pSlapVariable->Variant.varString = NULL;
                }
                else if( pSlapVariable->Syntax == SLAP_VAR_SYNTAX_int)
                {
                    pSlapVariable->Variant.varInt = 0;
                }
                else if( pSlapVariable->Syntax == SLAP_VAR_SYNTAX_uint32)
                {
                    pSlapVariable->Variant.varUint32 = 0;
                }
            }
        }

        return  pSlapVariable;
    }

    if ( !pObjController)
    {
        return  NULL;
    }

    if( TRUE /* pVarEntity->CallEntry_GetValue == 0 */)
    {
        if( pObjController->GetParamValueByName != NULL)
        {
            SlapAllocVariable(pSlapVariable);

            if ( !pSlapVariable )
            {
                return  NULL;
            }
            else
            {
                pSlapVariable->Name        = AnscCloneString(pVarEntity->ParamDescr->Name);
                pSlapVariable->ContentType = pVarEntity->ContentType;
                pSlapVariable->Syntax      = pVarEntity->Syntax;
            }

            if( pObjController->GetParamValueByName
                (
                    pObjController,
                    pMyObject->GetLastName(pMyObject),
                    pSlapVariable
                ) != ANSC_STATUS_SUCCESS)
            {
                SlapFreeVariable(pSlapVariable);

                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        pfnGetValueMethod = 0; /* *(void**)((ULONG)pObjController + pVarEntity->CallEntry_GetValue); */

        /*
         * Bin added here on 05/07/2007 in case it comes from monitor process while this record is ready to be removed
         */
        if( pfnGetValueMethod == 0)
        {

            AnscTrace("NULL GetValueMethod for object '%s'\n", pMyObject->GetLastName(pMyObject));

            return NULL;
        }

        SlapAllocVariable(pSlapVariable);

        if ( !pSlapVariable )
        {
            return  NULL;
        }
        else
        {
            pSlapVariable->Name        = AnscCloneString(pVarEntity->ParamDescr->Name);
            pSlapVariable->ContentType = pVarEntity->ContentType;
            pSlapVariable->Syntax      = pVarEntity->Syntax;
        }

        switch ( pVarEntity->Syntax )
        {
            case    SLAP_VAR_SYNTAX_bool :

                    pSlapVariable->Variant.varBool =
                        ((PFN_DSLHPARAM_GET_BOOL)pfnGetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController
                            );

                    break;

            case    SLAP_VAR_SYNTAX_int :

                    pSlapVariable->Variant.varInt =
                        ((PFN_DSLHPARAM_GET_INT)pfnGetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController
                            );

                    break;

            case    SLAP_VAR_SYNTAX_string :

                    pSlapVariable->Variant.varString =
                        ((PFN_DSLHPARAM_GET_STRING)pfnGetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController
                            );

                    break;

            case    SLAP_VAR_SYNTAX_ucharArray :

                    pSlapVariable->Variant.varUcharArray =
                        ((PFN_DSLHPARAM_GET_BUFFER)pfnGetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController
                            );

                    break;

            case    SLAP_VAR_SYNTAX_uint32 :

                    pSlapVariable->Variant.varUint32 =
                        ((PFN_DSLHPARAM_GET_UINT32)pfnGetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController
                            );

                    break;

            case    SLAP_VAR_SYNTAX_uint32Array :

                    pSlapVariable->Variant.varUint32Array =
                        ((PFN_DSLHPARAM_GET_ULLIST)pfnGetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController
                            );

                    break;

            case    SLAP_VAR_SYNTAX_handle :

                    pSlapVariable->Variant.varHandle =
                        ((PFN_DSLHPARAM_GET_HANDLE)pfnGetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController
                            );

                    break;

            default:

                    SlapFreeVariable(pSlapVariable);

                    return  NULL;

                    break;
        }
    }

    return  pSlapVariable;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhVarroTstValue
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_VARIABLE*              pNewValue
            );

    description:

        This function is called to test the parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_VARIABLE*              pNewValue
                Specifies the parameter value to be tested.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
DslhVarroTstValue
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              pNewValue
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PDSLH_VAR_RECORD_OBJECT         pMyObject               = (PDSLH_VAR_RECORD_OBJECT    )hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity              = (PDSLH_VAR_ENTITY_OBJECT    )pMyObject->hDslhVarEntity;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord              = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hDslhObjRecord;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity              = (PDSLH_OBJ_ENTITY_OBJECT    )pObjRecord->hDslhObjEntity;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController          = (PDSLH_OBJ_CONTROLLER_OBJECT)pObjRecord->hDslhObjController;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController      = (PDSLH_CPE_CONTROLLER_OBJECT)pObjRecord->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase        = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_VAREO_TOKEN_VALUE         pVareoTokenValue        = (PDSLH_VAREO_TOKEN_VALUE    )NULL;
    void*                           pfnTstValueMethod       = (void*                      )NULL;
    SLAP_UINT32                     ulIp4Addr               = (ULONG                      )0;
    SLAP_UINT32_ARRAY*              pUint32ArrayIp4AddrList = (SLAP_UINT32_ARRAY*         )NULL;
    SLAP_UCHAR_ARRAY*               pUcharArrayMacAddr      = (SLAP_UCHAR_ARRAY*          )NULL;
    SLAP_UCHAR_ARRAY*               pUcharArrayIp6Addr      = (SLAP_UCHAR_ARRAY*          )NULL;
    SLAP_UCHAR_ARRAY*               pUcharArrayMacAddrList  = (SLAP_UCHAR_ARRAY*          )NULL;
    SLAP_UCHAR_ARRAY*               pUcharArrayIp6AddrList  = (SLAP_UCHAR_ARRAY*          )NULL;
    PANSC_UNIVERSAL_TIME            pUniversalTime          = (PANSC_UNIVERSAL_TIME       )NULL;
    BOOL                            bTestResult             = FALSE;
    BOOL                            bTestBypass             = FALSE;
    ULONG                           uTickCount              = 0;

    /*
     * The parameters defined in TR-069 make use of a limited subset of the default SOAP data types.
     * If the specified parameter value contradicts the data type of the target parameter, we must
     * reject the request.
     */
    switch ( pVarEntity->DataType )
    {
        case    DSLH_CWMP_DATA_TYPE_base64   :

                if ( pNewValue->Syntax != SLAP_VAR_SYNTAX_string )
                {
                    return  FALSE;
                }

                if ( (pVarEntity->FormatValue1 != 0) && (pVarEntity->FormatValue1 != 0xFFFFFFFF) )
                {
                    /*
                     * For all strings a maximum length is either explicitly indicated or implicted
                     * by the size of the elements composing the string.
                     */
                    if ( pNewValue->Variant.varString &&
                         (AnscSizeOfString(pNewValue->Variant.varString) > pVarEntity->FormatValue1) )
                    {
                        return  FALSE;
                    }
                }

                if( !DslhVarroIsValidBase64String(pNewValue->Variant.varString))
                {
                    AnscTrace("Invalid Base64 string value: %s\n", pNewValue->Variant.varString);

                    return FALSE;
                }

                break;

        case    DSLH_CWMP_DATA_TYPE_string   :
        case    DSLH_CWMP_DATA_TYPE_dateTime :

                if ( pNewValue->Syntax != SLAP_VAR_SYNTAX_string )
                {
                    return  FALSE;
                }
                else if ( pVarEntity->DataType == DSLH_CWMP_DATA_TYPE_dateTime )
                {
                    /*
                     * Three type of times supported
                     * 1. Unknown or unapplicable time - 0001-01-01T00:00:00Z
                     * 2. Absolute time - YYYY-MM-DDThh:mm:ssZ
                     * 3. Relative time - YYYY-MM-DDThh:mm:ss
                     */

                    if ( !pNewValue->Variant.varString ||
                         (AnscSizeOfString(pNewValue->Variant.varString) != pVarEntity->FormatValue1    &&
                          AnscSizeOfString(pNewValue->Variant.varString) != pVarEntity->FormatValue1 + 1) )
                    {
                        return  FALSE;
                    }
                }
                else if ( (pVarEntity->FormatValue1 != 0) && (pVarEntity->FormatValue1 != 0xFFFFFFFF) )
                {
                    /*
                     * For all strings a maximum length is either explicitly indicated or implicted
                     * by the size of the elements composing the string.
                     */
                    if ( pNewValue->Variant.varString &&
                         (AnscSizeOfString(pNewValue->Variant.varString) > pVarEntity->FormatValue1) )
                    {
                        return  FALSE;
                    }
                }

                if ( pVarEntity->TokenTable && (pVarEntity->ContentType == SLAP_CONTENT_TYPE_MAPPED) )
                {
                    if( pNewValue->Variant.varString == NULL)
                    {
                        return FALSE;
                    }

                    pVareoTokenValue =
                        (PDSLH_VAREO_TOKEN_VALUE)DslhVareoGetTokenValueByString
                            (
                                (ANSC_HANDLE)pVarEntity,
                                pNewValue->Variant.varString
                            );

                    if ( !pVareoTokenValue )
                    {
                        return  FALSE;
                    }
                }
                else if( pNewValue->Variant.varString)
                {
                    if( SLAP_CONTENT_TYPE_IP4_ADDR == pVarEntity->ContentType)
                    {
                        if(!AnscIsValidIpString(pNewValue->Variant.varString))
                        {
                            AnscTrace("Invalid IP Address value is %s\n", pNewValue->Variant.varString);

                            return FALSE;
                        }
                    }
                    else if(SLAP_CONTENT_TYPE_MAC_ADDR == pVarEntity->ContentType)
                    {
                        if( ! DslhVarroIsValidMacAddress(pNewValue->Variant.varString))
                        {
                            AnscTrace("Invalid MAC Address value: %s\n", pNewValue->Variant.varString);

                            return FALSE;
                        }
                    }
                    else if( SLAP_CONTENT_TYPE_IP4_ADDR_LIST == pVarEntity->ContentType)
                    {
                        if( ! DslhVarroIsValidIpAddList(pNewValue->Variant.varString))
                        {
                            AnscTrace("Invalid IpAddList value: %s\n", pNewValue->Variant.varString);

                            return FALSE;
                        }
                    }
                    else if( SLAP_CONTENT_TYPE_IP4_ADDR_RANGE_LIST == pVarEntity->ContentType)
                    {
                        if( ! DslhVarroIsValidIpAddRangeList(pNewValue->Variant.varString))
                        {
                            AnscTrace("Invalid IpAddRangeList value: %s\n", pNewValue->Variant.varString);

                            return FALSE;
                        }
                    }
					else if(SLAP_CONTENT_TYPE_IP6_ADDR == pVarEntity->ContentType)
					{
						if( ! AnscIsValidIp6String(pNewValue->Variant.varString))
						{
							AnscTrace("Invalid ip6 Address value: %s\n", pNewValue->Variant.varString);

							return FALSE;
						}
					}
					else if( SLAP_CONTENT_TYPE_IP6_ADDR_LIST == pVarEntity->ContentType)
					{
						if( ! DslhVarroIsValidIp6AddList(pNewValue->Variant.varString))
						{
							AnscTrace("Invalid IP6 Address List value: %s\n", pNewValue->Variant.varString);

							return FALSE;
                        } 
					}
                    else if( SLAP_CONTENT_TYPE_MAC_ADDR_LIST == pVarEntity->ContentType)
                    {
                        if( ! DslhVarroIsValidMacAddList(pNewValue->Variant.varString))
                        {
                            AnscTrace("Invalid MAC Address List value: %s\n", pNewValue->Variant.varString);

                            return FALSE;
                        }
                    }
                    else if( SLAP_CONTENT_TYPE_HEX_STRING == pVarEntity->ContentType)
                    {
                        if( ! DslhVarroIsValidHexString(pNewValue->Variant.varString))
                        {
                            AnscTrace("Invalid HEX string value: %s\n", pNewValue->Variant.varString);

                            return FALSE;
                        }
                    }
                    else if( SLAP_CONTENT_TYPE_CALENDAR_TIME == pVarEntity->ContentType)
                    {
                        if( ! DslhVarroIsValidCalendarString(pNewValue->Variant.varString))
                        {
                            AnscTrace("Invalid Calendar string value: %s\n", pNewValue->Variant.varString);

                            return FALSE;
                        }
                    }
                    else if( SLAP_CONTENT_TYPE_URL_STRING == pVarEntity->ContentType)
                    {
                        if( ! DslhVarroIsValidUrlString(pNewValue->Variant.varString))
                        {
                            AnscTrace("Invalid Url string value: %s\n", pNewValue->Variant.varString);

                            return FALSE;
                        }
                    }
                    else if( SLAP_CONTENT_TYPE_SUBNET_MASK == pVarEntity->ContentType)
                    {
                        if( ! DslhVarroIsValidMaskString(pNewValue->Variant.varString))
                        {
                            AnscTrace("Invalid Subnet Mask string value: %s\n", pNewValue->Variant.varString);

                            return FALSE;
                        }
                    }
                }
                else
                {



                }

                break;

        case    DSLH_CWMP_DATA_TYPE_int :

                if ( pNewValue->Syntax != SLAP_VAR_SYNTAX_int )
                {
                    return  FALSE;
                }
                else if ( (pNewValue->Variant.varInt < (int)pVarEntity->FormatValue1) ||
                          (pNewValue->Variant.varInt > (int)pVarEntity->FormatValue2) )
                {
                    return  FALSE;
                }

                break;

        case    DSLH_CWMP_DATA_TYPE_unsignedInt :

                if ( pNewValue->Syntax != SLAP_VAR_SYNTAX_uint32 )
                {
                    return  FALSE;
                }
                else if ( (pNewValue->Variant.varUint32 < pVarEntity->FormatValue1) ||
                          (pNewValue->Variant.varUint32 > pVarEntity->FormatValue2) )
                {
                    return  FALSE;
                }

                break;

        case    DSLH_CWMP_DATA_TYPE_boolean :

                if ( (pNewValue->Syntax != SLAP_VAR_SYNTAX_bool  ) &&
                     (pNewValue->Syntax != SLAP_VAR_SYNTAX_int   ) &&
                     (pNewValue->Syntax != SLAP_VAR_SYNTAX_uint32) )
                {
                    return  FALSE;
                }

                break;

        default :

                return  FALSE;

                break;
    }

    /*
     * To ensure parameter values are translated properly, we need to execute the following switch
     * statements even if the underlying ObjController didn't provide the corresopnding TstValue()
     * callback function.
     */
    if ( !pObjController )
    {
        return  FALSE;
    }
    else if ( (pObjEntity->ObjDescr->bGroupCommit) || TRUE /* (pVarEntity->CallEntry_TstValue == 0)*/ )
    {
        bTestResult = TRUE;
        bTestBypass = TRUE;
    }
    else
    {
        pfnTstValueMethod = 0; /* *(void**)((ULONG)pObjController + pVarEntity->CallEntry_TstValue); */
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
    if ( pVarEntity->ContentType == SLAP_CONTENT_TYPE_UNSPECIFIED )
    {
        switch ( pVarEntity->Syntax )
        {
            case    SLAP_VAR_SYNTAX_bool :

                    if ( !bTestBypass )
                    {
                        bTestResult =
                            ((PFN_DSLHPARAM_TST_BOOL)pfnTstValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    (pNewValue->Variant.varBool != 0)? TRUE : FALSE
                                );
                    }

                    break;

            case    SLAP_VAR_SYNTAX_int :

                    if ( !bTestBypass )
                    {
                        bTestResult =
                            ((PFN_DSLHPARAM_TST_INT)pfnTstValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pNewValue->Variant.varInt
                                );
                    }

                    break;

            case    SLAP_VAR_SYNTAX_string :

                    if ( !bTestBypass )
                    {
                        bTestResult =
                            ((PFN_DSLHPARAM_TST_STRING)pfnTstValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pNewValue->Variant.varString
                                );
                    }

                    break;

            case    SLAP_VAR_SYNTAX_ucharArray :

                    if ( !bTestBypass )
                    {
                        bTestResult =
                            ((PFN_DSLHPARAM_TST_BUFFER)pfnTstValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pNewValue->Variant.varUcharArray
                                );
                    }

                    break;

            case    SLAP_VAR_SYNTAX_uint32 :

                    if ( !bTestBypass )
                    {
                        bTestResult =
                            ((PFN_DSLHPARAM_TST_UINT32)pfnTstValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pNewValue->Variant.varUint32
                                );
                    }

                    break;

            case    SLAP_VAR_SYNTAX_handle :

                    if ( !bTestBypass )
                    {
                        bTestResult =
                            ((PFN_DSLHPARAM_TST_HANDLE)pfnTstValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pNewValue->Variant.varHandle
                                );
                    }

                    break;

            default:

                    bTestResult = FALSE;

                    break;
        }
    }
    else
    {
        if ( !pNewValue->Variant.varString )
        {
            if( SLAP_CONTENT_TYPE_IP4_ADDR_LIST        == pVarEntity->ContentType ||
                SLAP_CONTENT_TYPE_IP4_ADDR             == pVarEntity->ContentType ||
                SLAP_CONTENT_TYPE_IP4_ADDR_WILDCARD    == pVarEntity->ContentType ||
                SLAP_CONTENT_TYPE_HEX_STRING           == pVarEntity->ContentType ||
                SLAP_CONTENT_TYPE_CALENDAR_TIME        == pVarEntity->ContentType ||
                SLAP_CONTENT_TYPE_URL_STRING           == pVarEntity->ContentType ||
                SLAP_CONTENT_TYPE_SUBNET_MASK          == pVarEntity->ContentType ||
                SLAP_CONTENT_TYPE_MAC_ADDR             == pVarEntity->ContentType ||
                SLAP_CONTENT_TYPE_IP6_ADDR             == pVarEntity->ContentType ||
                SLAP_CONTENT_TYPE_IP6_ADDR_LIST        == pVarEntity->ContentType ||
                SLAP_CONTENT_TYPE_MAC_ADDR_LIST        == pVarEntity->ContentType ||
                SLAP_CONTENT_TYPE_IP4_ADDR_RANGE_LIST  == pVarEntity->ContentType )
            {
                goto EXIT1;
            }
            else
            {
                return  FALSE;
            }
        }

        switch ( pVarEntity->ContentType )
        {
            case    SLAP_CONTENT_TYPE_MAPPED :

                    if ( !pVareoTokenValue )
                    {
                        pVareoTokenValue =
                            (PDSLH_VAREO_TOKEN_VALUE)DslhVareoGetTokenValueByString
                                (
                                    (ANSC_HANDLE)pVarEntity,
                                    pNewValue->Variant.varString
                                );
                    }

                    if ( !pVareoTokenValue )
                    {
                        bTestResult = FALSE;
                    }
                    else
                    {
                        if ( !bTestBypass )
                        {
                            bTestResult =
                                ((PFN_DSLHPARAM_TST_UINT32)pfnTstValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        pVareoTokenValue->EnumCode
                                    );
                        }

                        if ( bTestResult )
                        {
                            SlapAllocVariable(pMyObject->TempParamValueTrans);

                            pMyObject->TempParamValueTrans->Syntax            = SLAP_VAR_SYNTAX_uint32;
                            pMyObject->TempParamValueTrans->Variant.varUint32 = pVareoTokenValue->EnumCode;
                        }
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP4_ADDR :
            case    SLAP_CONTENT_TYPE_SUBNET_MASK :

                    if ( !pNewValue->Variant.varString || (AnscSizeOfString(pNewValue->Variant.varString) == 0) )
                    {
                        ulIp4Addr = 0;
                    }
                    else
                    {
                        ulIp4Addr =
                            SlapVcoStringToIp4Addr
                                (
                                    NULL,
                                    pNewValue->Variant.varString
                                );
                    }

                    if ( !bTestBypass )
                    {
                        bTestResult =
                            ((PFN_DSLHPARAM_TST_UINT32)pfnTstValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    ulIp4Addr
                                );
                    }

                    if ( bTestResult )
                    {
                        SlapAllocVariable(pMyObject->TempParamValueTrans);

                        pMyObject->TempParamValueTrans->Syntax            = SLAP_VAR_SYNTAX_uint32;
                        pMyObject->TempParamValueTrans->Variant.varUint32 = ulIp4Addr;
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP4_ADDR_WILDCARD :

                    if ( !pNewValue->Variant.varString || (AnscSizeOfString(pNewValue->Variant.varString) == 0) )
                    {
                        ulIp4Addr = 0;
                    }
                    else
                    {
                        ulIp4Addr =
                            SlapVcoStringToIp4Addr
                                (
                                    NULL,
                                    pNewValue->Variant.varString
                                );
                    }

                    if ( !bTestBypass )
                    {
                        bTestResult =
                            ((PFN_DSLHPARAM_TST_UINT32)pfnTstValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    ulIp4Addr
                                );
                    }

                    if ( bTestResult )
                    {
                        SlapAllocVariable(pMyObject->TempParamValueTrans);

                        pMyObject->TempParamValueTrans->Syntax            = SLAP_VAR_SYNTAX_uint32;
                        pMyObject->TempParamValueTrans->Variant.varUint32 = ulIp4Addr;
                    }

                    break;

            case    SLAP_CONTENT_TYPE_MAC_ADDR :

                    pUcharArrayMacAddr =
                        SlapVcoStringToMacAddr
                            (
                                NULL,
                                pNewValue->Variant.varString
                            );

                    if ( pUcharArrayMacAddr )
                    {
                        if ( !bTestBypass )
                        {
                            bTestResult =
                                ((PFN_DSLHPARAM_TST_BUFFER)pfnTstValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        pUcharArrayMacAddr
                                    );
                        }

                        if ( bTestResult )
                        {
                            SlapAllocVariable(pMyObject->TempParamValueTrans);

                            pMyObject->TempParamValueTrans->Syntax                 = SLAP_VAR_SYNTAX_string;
                            pMyObject->TempParamValueTrans->Variant.varString      = AnscCloneString(pNewValue->Variant.varString);
                        }

                        SlapFreeVarArray(pUcharArrayMacAddr);
                    }
                    else
                    {
                        bTestResult = FALSE;
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP6_ADDR :

                    pUcharArrayIp6Addr =
                        SlapVcoStringToIp6Addr
                            (
                                (ANSC_HANDLE)NULL,
                                pNewValue->Variant.varString
                            );

                    if ( pUcharArrayIp6Addr )
                    {
                        if ( !bTestBypass )
                        {
                            bTestResult =
                                ((PFN_DSLHPARAM_TST_BUFFER)pfnTstValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        pUcharArrayIp6Addr
                                    );
                        }

                        if ( bTestResult )
                        {
                            SlapAllocVariable(pMyObject->TempParamValueTrans);

                            pMyObject->TempParamValueTrans->Syntax                 = SLAP_VAR_SYNTAX_string;
                            pMyObject->TempParamValueTrans->Variant.varString      = AnscCloneString(pNewValue->Variant.varString);
                        }
                        else
                        {
                            SlapFreeVarArray(pUcharArrayIp6Addr);
                        }
                    }
                    else
                    {
                        bTestResult = FALSE;
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP6_ADDR_LIST :

                    pUcharArrayIp6AddrList =
                        SlapVcoStringToIp6AddrList
                            (
                                (ANSC_HANDLE)NULL,
                                pNewValue->Variant.varString
                            );

                    if ( pUcharArrayIp6AddrList )
                    {
                        if ( !bTestBypass )
                        {
                            bTestResult =
                                ((PFN_DSLHPARAM_TST_BUFFER)pfnTstValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        pUcharArrayIp6AddrList
                                    );
                        }

                        if ( bTestResult )
                        {
                            SlapAllocVariable(pMyObject->TempParamValueTrans);

                            pMyObject->TempParamValueTrans->Syntax                 = SLAP_VAR_SYNTAX_string;
                            pMyObject->TempParamValueTrans->Variant.varString      = AnscCloneString(pNewValue->Variant.varString);
                        }
                        else
                        {
                            SlapFreeVarArray(pUcharArrayIp6AddrList);
                        }
                    }
                    else
                    {
                        bTestResult = FALSE;
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP4_ADDR_LIST :

                    pUint32ArrayIp4AddrList =
                        SlapVcoStringToIp4AddrList
                            (
                                NULL,
                                pNewValue->Variant.varString
                            );

                    if ( pUint32ArrayIp4AddrList )
                    {
                        if ( !bTestBypass )
                        {
                            bTestResult =
                                ((PFN_DSLHPARAM_TST_ULLIST)pfnTstValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        pUint32ArrayIp4AddrList
                                    );
                        }

                        if ( bTestResult )
                        {
                            SlapAllocVariable(pMyObject->TempParamValueTrans);

                            pMyObject->TempParamValueTrans->Syntax                 = SLAP_VAR_SYNTAX_string;
                            pMyObject->TempParamValueTrans->Variant.varString      = AnscCloneString(pNewValue->Variant.varString);
                        }

                        SlapFreeVarArray(pUint32ArrayIp4AddrList);
                    }
                    else
                    {
                        bTestResult = FALSE;
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP4_ADDR_RANGE_LIST :

                    SlapAllocVariable(pMyObject->TempParamValueTrans);

                    pMyObject->TempParamValueTrans->Syntax                 = SLAP_VAR_SYNTAX_string;
                    pMyObject->TempParamValueTrans->Variant.varString      = AnscCloneString(pNewValue->Variant.varString);

                    break;

            case    SLAP_CONTENT_TYPE_MAC_ADDR_LIST :

                    pUcharArrayMacAddrList =
                        SlapVcoStringToMacAddrList
                            (
                                NULL,
                                pNewValue->Variant.varString
                            );

                    if ( pUcharArrayMacAddrList )
                    {
                        if ( !bTestBypass )
                        {
                            bTestResult =
                                ((PFN_DSLHPARAM_TST_BUFFER)pfnTstValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        pUcharArrayMacAddrList
                                    );
                        }

                        if ( bTestResult )
                        {
                            SlapAllocVariable(pMyObject->TempParamValueTrans);

                            pMyObject->TempParamValueTrans->Syntax                 = SLAP_VAR_SYNTAX_string;
                            pMyObject->TempParamValueTrans->Variant.varString      = AnscCloneString(pNewValue->Variant.varString);
                        }

                        SlapFreeVarArray(pUcharArrayMacAddrList);
                    }
                    else
                    {
                        bTestResult = FALSE;
                    }

                    break;

            case    SLAP_CONTENT_TYPE_CALENDAR_TIME :

                    pUniversalTime =
                        (PANSC_UNIVERSAL_TIME)SlapVcoStringToCalendarTime
                            (
                                NULL,
                                pNewValue->Variant.varString
                            );

                    if ( pUniversalTime )
                    {
                        if ( !bTestBypass )
                        {
                            bTestResult =
                                ((PFN_DSLHPARAM_TST_HANDLE)pfnTstValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        (ANSC_HANDLE)pUniversalTime
                                    );
                        }

                        if ( bTestResult )
                        {
                            SlapAllocVariable(pMyObject->TempParamValueTrans);

                            pMyObject->TempParamValueTrans->Syntax                 = SLAP_VAR_SYNTAX_string;
                            pMyObject->TempParamValueTrans->Variant.varString      = AnscCloneString(pNewValue->Variant.varString);

                        }

                        AnscFreeMemory(pUniversalTime);
                    }
                    else
                    {
                        bTestResult = FALSE;
                    }

                    break;

            case    SLAP_CONTENT_TYPE_HEX_STRING:

                    AnscTrace("Since most hex_strings are saved and used as a string, we don't convert to a UcharArray here.\n");

                    break;

            case    SLAP_CONTENT_TYPE_URL_STRING :
                    break;

            default :

                    bTestResult = FALSE;

                    break;
        }
    }

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( bTestResult && (pVarEntity->ParamDescr->bPersistent || pObjEntity->ObjDescr->bGroupCommit) )
    {
        SlapAllocVariable(pMyObject->TempParamValue);

        if ( !pMyObject->TempParamValue )
        {
            return  FALSE;
        }
        else
        {
            SlapCloneVariable(pNewValue, pMyObject->TempParamValue);
        }
    }

    return  bTestResult;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVarroSetValue
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_VARIABLE*              pNewValue
            );

    description:

        This function is called to configure the parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_VARIABLE*              pNewValue
                Specifies the parameter value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVarroSetValue
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              pNewValue
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PDSLH_VAR_RECORD_OBJECT         pMyObject               = (PDSLH_VAR_RECORD_OBJECT    )hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity              = (PDSLH_VAR_ENTITY_OBJECT    )pMyObject->hDslhVarEntity;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord              =  (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hDslhObjRecord;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity              = (PDSLH_OBJ_ENTITY_OBJECT    )pObjRecord->hDslhObjEntity;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController          = (PDSLH_OBJ_CONTROLLER_OBJECT)pObjRecord->hDslhObjController;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController      = (PDSLH_CPE_CONTROLLER_OBJECT)pObjRecord->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase        = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_VAREO_TOKEN_VALUE         pVareoTokenValue        = (PDSLH_VAREO_TOKEN_VALUE    )NULL;
    void*                           pfnSetValueMethod       = (void*                      )NULL;
    SLAP_UINT32                     ulIp4Addr               = (ULONG                      )0;
    SLAP_UINT32_ARRAY*              pUint32ArrayIp4AddrList = (SLAP_UINT32_ARRAY*         )NULL;
    SLAP_UCHAR_ARRAY*               pUcharArrayMacAddr      = (SLAP_UCHAR_ARRAY*          )NULL;
    SLAP_UCHAR_ARRAY*               pUcharArrayMacAddrList  = (SLAP_UCHAR_ARRAY*          )NULL;
    SLAP_UCHAR_ARRAY*               pUcharArrayIp6Addr      = (SLAP_UCHAR_ARRAY*          )NULL;
    SLAP_UCHAR_ARRAY*               pUcharArrayIp6AddrList  = (SLAP_UCHAR_ARRAY*          )NULL;
    PANSC_UNIVERSAL_TIME            pUniversalTime          = (PANSC_UNIVERSAL_TIME       )NULL;
    ULONG                           uTickCount              = 0;


    if ( TRUE /* pVarEntity->CallEntry_SetValue == 0 */)
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( pObjEntity->ObjDescr->bGroupCommit )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    if ( !pObjController )
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        pfnSetValueMethod = 0; /* *(void**)((ULONG)pObjController + pVarEntity->CallEntry_SetValue); */
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


    if ( pVarEntity->ContentType == SLAP_CONTENT_TYPE_UNSPECIFIED )
    {
        switch ( pVarEntity->Syntax )
        {
            case    SLAP_VAR_SYNTAX_bool :

                    returnStatus =
                        ((PFN_DSLHPARAM_SET_BOOL)pfnSetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController,
                                (pNewValue->Variant.varBool != 0)? TRUE : FALSE
                            );

                    break;

            case    SLAP_VAR_SYNTAX_int :

                    returnStatus =
                        ((PFN_DSLHPARAM_SET_INT)pfnSetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController,
                                pNewValue->Variant.varInt
                            );

                    break;

            case    SLAP_VAR_SYNTAX_string :

                    returnStatus =
                        ((PFN_DSLHPARAM_SET_STRING)pfnSetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController,
                                pNewValue->Variant.varString
                            );

                    break;

            case    SLAP_VAR_SYNTAX_ucharArray :

                    returnStatus =
                        ((PFN_DSLHPARAM_SET_BUFFER)pfnSetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController,
                                pNewValue->Variant.varUcharArray
                            );

                    break;

            case    SLAP_VAR_SYNTAX_uint32 :

                    returnStatus =
                        ((PFN_DSLHPARAM_SET_UINT32)pfnSetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController,
                                pNewValue->Variant.varUint32
                            );

                    break;

            case    SLAP_VAR_SYNTAX_handle :

                    returnStatus =
                        ((PFN_DSLHPARAM_SET_HANDLE)pfnSetValueMethod)
                            (
                                (ANSC_HANDLE)pObjController,
                                pNewValue->Variant.varHandle
                            );

                    break;

            default:

                    break;
        }
    }
    else
    {
        switch ( pVarEntity->ContentType )
        {
            case    SLAP_CONTENT_TYPE_MAPPED :

                    if ( pMyObject->TempParamValueTrans )
                    {
                        returnStatus =
                            ((PFN_DSLHPARAM_SET_UINT32)pfnSetValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pMyObject->TempParamValueTrans->Variant.varUint32
                                );
                    }
                    else
                    {
                        if ( !pVareoTokenValue )
                        {
                            pVareoTokenValue =
                                (PDSLH_VAREO_TOKEN_VALUE)DslhVareoGetTokenValueByString
                                    (
                                        (ANSC_HANDLE)pVarEntity,
                                        pNewValue->Variant.varString
                                    );
                        }

                        if ( !pVareoTokenValue )
                        {
                            returnStatus = ANSC_STATUS_INTERNAL_ERROR;
                        }
                        else
                        {
                            returnStatus =
                                ((PFN_DSLHPARAM_SET_UINT32)pfnSetValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        pVareoTokenValue->EnumCode
                                    );
                        }
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP4_ADDR :
            case    SLAP_CONTENT_TYPE_SUBNET_MASK :

                    if ( pMyObject->TempParamValueTrans )
                    {
                        returnStatus =
                            ((PFN_DSLHPARAM_SET_UINT32)pfnSetValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pMyObject->TempParamValueTrans->Variant.varUint32
                                );
                    }
                    else
                    {
                        if ( !pNewValue->Variant.varString || (AnscSizeOfString(pNewValue->Variant.varString) == 0) )
                        {
                            ulIp4Addr = 0;
                        }
                        else
                        {
                            ulIp4Addr =
                                SlapVcoStringToIp4Addr
                                    (
                                        NULL,
                                        pNewValue->Variant.varString
                                    );
                        }

                        returnStatus =
                            ((PFN_DSLHPARAM_SET_UINT32)pfnSetValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    ulIp4Addr
                                );
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP4_ADDR_WILDCARD :

                    if ( pMyObject->TempParamValueTrans )
                    {
                        returnStatus =
                            ((PFN_DSLHPARAM_SET_UINT32)pfnSetValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pMyObject->TempParamValueTrans->Variant.varUint32
                                );
                    }
                    else
                    {
                        if ( !pNewValue->Variant.varString || (AnscSizeOfString(pNewValue->Variant.varString) == 0) )
                        {
                            ulIp4Addr = 0;
                        }
                        else
                        {
                            ulIp4Addr =
                                SlapVcoStringToIp4Addr
                                    (
                                        NULL,
                                        pNewValue->Variant.varString
                                    );
                        }

                        returnStatus =
                            ((PFN_DSLHPARAM_SET_UINT32)pfnSetValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    ulIp4Addr
                                );
                    }

                    break;

            case    SLAP_CONTENT_TYPE_MAC_ADDR :

                    if ( pMyObject->TempParamValueTrans )
                    {
                        returnStatus =
                            ((PFN_DSLHPARAM_SET_BUFFER)pfnSetValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pMyObject->TempParamValueTrans->Variant.varUcharArray
                                );
                    }
                    else
                    {
                        pUcharArrayMacAddr =
                            SlapVcoStringToMacAddr
                                (
                                    NULL,
                                    pNewValue->Variant.varString
                                );

                        if ( pUcharArrayMacAddr )
                        {
                            returnStatus =
                                ((PFN_DSLHPARAM_SET_BUFFER)pfnSetValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        pUcharArrayMacAddr
                                    );

                            SlapFreeVarArray(pUcharArrayMacAddr);
                        }
                        else
                        {
                            returnStatus = ANSC_STATUS_RESOURCES;
                        }
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP4_ADDR_LIST :

                    if ( pMyObject->TempParamValueTrans )
                    {
                        returnStatus =
                            ((PFN_DSLHPARAM_SET_ULLIST)pfnSetValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pMyObject->TempParamValueTrans->Variant.varUint32Array
                                );
                    }
                    else
                    {
                        pUint32ArrayIp4AddrList =
                            SlapVcoStringToIp4AddrList
                                (
                                    NULL,
                                    pNewValue->Variant.varString
                                );

                        if ( pUint32ArrayIp4AddrList )
                        {
                            returnStatus =
                                ((PFN_DSLHPARAM_SET_ULLIST)pfnSetValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        pUint32ArrayIp4AddrList
                                    );

                            SlapFreeVarArray(pUint32ArrayIp4AddrList);
                        }
                        else
                        {
                            returnStatus = ANSC_STATUS_RESOURCES;
                        }
                    }
                    break;

            case    SLAP_CONTENT_TYPE_IP6_ADDR :

                    if ( pMyObject->TempParamValueTrans )
                    {
                        returnStatus =
                            ((PFN_DSLHPARAM_SET_BUFFER)pfnSetValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pMyObject->TempParamValueTrans->Variant.varUcharArray
                                );
                    }
                    else
                    {
                        pUcharArrayIp6Addr =
                            SlapVcoStringToIp6Addr
                                (
                                    (ANSC_HANDLE)NULL,
                                    pNewValue->Variant.varString
                                );

                        if ( pUcharArrayIp6Addr )
                        {
                            returnStatus =
                                ((PFN_DSLHPARAM_SET_BUFFER)pfnSetValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        pUcharArrayIp6Addr
                                    );

                            SlapFreeVarArray(pUcharArrayIp6Addr);
                        }
                        else
                        {
                            returnStatus = ANSC_STATUS_RESOURCES;
                        }
                    }

                    break;

            case    SLAP_CONTENT_TYPE_MAC_ADDR_LIST :

                    if ( pMyObject->TempParamValueTrans )
                    {
                        returnStatus =
                            ((PFN_DSLHPARAM_SET_BUFFER)pfnSetValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pMyObject->TempParamValueTrans->Variant.varUcharArray
                                );
                    }
                    else
                    {
                        pUcharArrayMacAddrList =
                            SlapVcoStringToMacAddrList
                                (
                                    NULL,
                                    pNewValue->Variant.varString
                                );

                        if ( pUcharArrayMacAddrList )
                        {
                            returnStatus =
                                ((PFN_DSLHPARAM_SET_BUFFER)pfnSetValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        pUcharArrayMacAddrList
                                    );

                            SlapFreeVarArray(pUcharArrayMacAddrList);
                        }
                        else
                        {
                            returnStatus = ANSC_STATUS_RESOURCES;
                        }
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP6_ADDR_LIST :

                    if ( pMyObject->TempParamValueTrans )
                    {
                        returnStatus =
                            ((PFN_DSLHPARAM_SET_BUFFER)pfnSetValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pMyObject->TempParamValueTrans->Variant.varUcharArray
                                );
                    }
                    else
                    {
                        pUcharArrayIp6AddrList =
                            SlapVcoStringToIp6AddrList
                                (
                                    (ANSC_HANDLE)NULL,
                                    pNewValue->Variant.varString
                                );

                        if ( pUcharArrayIp6AddrList )
                        {
                            returnStatus =
                                ((PFN_DSLHPARAM_SET_BUFFER)pfnSetValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        pUcharArrayIp6AddrList
                                    );

                            SlapFreeVarArray(pUcharArrayIp6AddrList);
                        }
                        else
                        {
                            returnStatus = ANSC_STATUS_RESOURCES;
                        }
                    }

                    break;


            case    SLAP_CONTENT_TYPE_CALENDAR_TIME :

                    if ( pMyObject->TempParamValueTrans )
                    {
                        returnStatus =
                            ((PFN_DSLHPARAM_SET_HANDLE)pfnSetValueMethod)
                                (
                                    (ANSC_HANDLE)pObjController,
                                    pMyObject->TempParamValueTrans->Variant.varHandle
                                );
                    }
                    else
                    {
                        pUniversalTime =
                            (PANSC_UNIVERSAL_TIME)SlapVcoStringToCalendarTime
                                (
                                    NULL,
                                    pNewValue->Variant.varString
                                );

                        if ( pUniversalTime )
                        {
                            returnStatus =
                                ((PFN_DSLHPARAM_SET_HANDLE)pfnSetValueMethod)
                                    (
                                        (ANSC_HANDLE)pObjController,
                                        (ANSC_HANDLE)pUniversalTime
                                    );

                            AnscFreeMemory(pUniversalTime);
                        }
                        else
                        {
                            returnStatus = ANSC_STATUS_RESOURCES;
                        }
                    }

                    break;

            default :

                    break;
        }
    }
    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVarroSynValue
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to read the current parameter value and
        save it into the system registry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVarroSynValue
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PDSLH_VAR_RECORD_OBJECT         pMyObject            = (PDSLH_VAR_RECORD_OBJECT    )hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity           = (PDSLH_VAR_ENTITY_OBJECT    )pMyObject->hDslhVarEntity;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hDslhObjRecord;
    /*PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDatabase  = (PPOAM_IREP_FOLDER_OBJECT   )pObjRecord->hIrepFoWmpDatabase;*/
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController       = (PDSLH_OBJ_CONTROLLER_OBJECT)pObjRecord->hDslhObjController;
    /*PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoParameter = (PPOAM_IREP_FOLDER_OBJECT   )pMyObject->hIrepFoParameter;*/
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController   = (PDSLH_CPE_CONTROLLER_OBJECT)pObjRecord->hDslhCpeController;
    PSLAP_VARIABLE                  pSlapVariable        = (PSLAP_VARIABLE             )NULL;
    PCHAR                           pFullName            = NULL;

/*
    if ( !pVarEntity->ParamDescr->bPersistent )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    if( !pPoamIrepFoParameter)
    {
        pFullName = pMyObject->GetFullName(pMyObject);

        if( pFullName == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }

        pPoamIrepFoParameter = (PPOAM_IREP_FOLDER_OBJECT)
            pPoamIrepFoDatabase->AddFolder(pPoamIrepFoDatabase, pFullName, 0);

        AnscFreeMemory(pFullName);

        if( !pPoamIrepFoParameter)
        {
            return  ANSC_STATUS_NOT_READY;
        }

        pMyObject->hIrepFoParameter = pPoamIrepFoParameter;
    }

    pSlapVariable = pMyObject->GetValue((ANSC_HANDLE)pMyObject);

    if ( !pSlapVariable )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        ULONG                       ulSysRepRecordType = SYS_REP_RECORD_TYPE_ASTR;

        SlapConvToSysRecordType(pSlapVariable->Syntax, ulSysRepRecordType);

        returnStatus =
            pPoamIrepFoParameter->AddRecord
                (
                    (ANSC_HANDLE)pPoamIrepFoParameter,
                    DSLH_VARRO_RR_NAME_Value,
                    ulSysRepRecordType,
                    SYS_RECORD_CONTENT_DEFAULT,
                    pSlapVariable,
                    0
                );

        SlapFreeVariable(pSlapVariable);
    }
*/

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        DslhVarroSaveOldValue
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     parameter value.

**********************************************************************/

ANSC_STATUS
DslhVarroSaveOldValue
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PDSLH_VAR_RECORD_OBJECT         pMyObject            = (PDSLH_VAR_RECORD_OBJECT    )hThisObject;

    pMyObject->OldParamValue = pMyObject->GetValue((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVarroCommitChange
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to commit the previous change made to
        this parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVarroCommitChange
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bFromAcs
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PDSLH_VAR_RECORD_OBJECT         pMyObject            = (PDSLH_VAR_RECORD_OBJECT    )hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity           = (PDSLH_VAR_ENTITY_OBJECT    )pMyObject->hDslhVarEntity;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hDslhObjRecord;
    /*PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoDatabase  = (PPOAM_IREP_FOLDER_OBJECT   )pObjRecord->hIrepFoWmpDatabase;*/
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController       = (PDSLH_OBJ_CONTROLLER_OBJECT)pObjRecord->hDslhObjController;
    /*PPOAM_IREP_FOLDER_OBJECT        pPoamIrepFoParameter = (PPOAM_IREP_FOLDER_OBJECT   )pMyObject->hIrepFoParameter;*/
    PSLAP_VARIABLE                  pTempParamValue      = (PSLAP_VARIABLE             )pMyObject->TempParamValue;
    /*ULONG                           ulSysRepRecordType   = (ULONG                      )SYS_REP_RECORD_TYPE_ASTR;*/
    PCHAR                           pFullName            = NULL;

    /* Send parameter value change signal here */
    if ( FALSE ) /* Bin added here to prevent double ValueChange notification */
    {
        if ( !bFromAcs && pMyObject->Notification != DSLH_CWMP_NOTIFICATION_off )
        {
            returnStatus = pMyObject->NotifyValueChanged((ANSC_HANDLE)pMyObject);
        }
    }

    if ( pMyObject->TempParamValue )
    {
        /*
        if ( pVarEntity->ParamDescr->bPersistent )
        {
            
            if( !pPoamIrepFoParameter)
            {
                pFullName = pMyObject->GetFullName(pMyObject);

                if( pFullName == NULL)
                {
                    return ANSC_STATUS_RESOURCES;
                }

                pPoamIrepFoParameter = (PPOAM_IREP_FOLDER_OBJECT)
                    pPoamIrepFoDatabase->AddFolder(pPoamIrepFoDatabase, pFullName, 0);

                AnscFreeMemory(pFullName);

                if( !pPoamIrepFoParameter)
                {
                    return  ANSC_STATUS_RESOURCES;
                }

                pMyObject->hIrepFoParameter = pPoamIrepFoParameter;
            }

            SlapConvToSysRecordType(pMyObject->TempParamValue->Syntax, ulSysRepRecordType);

            returnStatus =
                pPoamIrepFoParameter->AddRecord
                    (
                        (ANSC_HANDLE)pPoamIrepFoParameter,
                        DSLH_VARRO_RR_NAME_Value,
                        ulSysRepRecordType,
                        SYS_RECORD_CONTENT_DEFAULT,
                        pMyObject->TempParamValue,
                        0
                    );
        }
        */
        SlapFreeVariable(pMyObject->TempParamValue);

        pMyObject->TempParamValue = NULL;
        
    }

    if ( pMyObject->TempParamValueTrans )
    {
        if ( (pMyObject->TempParamValueTrans->Syntax == SLAP_VAR_SYNTAX_handle) && pMyObject->TempParamValueTrans->Variant.varHandle )
        {
            AnscFreeMemory(pMyObject->TempParamValueTrans->Variant.varHandle);
        }

        SlapFreeVariable(pMyObject->TempParamValueTrans);

        pMyObject->TempParamValueTrans = NULL;
    }

    if ( pMyObject->OldParamValue )
    {
        SlapFreeVariable(pMyObject->OldParamValue);

        pMyObject->OldParamValue = NULL;
    }

    pMyObject->bAlreadySet = FALSE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVarroCancelChange
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the previous change made to
        this parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVarroCancelChange
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PDSLH_VAR_RECORD_OBJECT         pMyObject            = (PDSLH_VAR_RECORD_OBJECT    )hThisObject;
    PDSLH_VAR_ENTITY_OBJECT         pVarEntity           = (PDSLH_VAR_ENTITY_OBJECT    )pMyObject->hDslhVarEntity;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord           = (PDSLH_OBJ_RECORD_OBJECT    )pMyObject->hDslhObjRecord;
    PDSLH_OBJ_CONTROLLER_OBJECT     pObjController       = (PDSLH_OBJ_CONTROLLER_OBJECT)pObjRecord->hDslhObjController;

    if ( pMyObject->TempParamValue )
    {
        SlapFreeVariable(pMyObject->TempParamValue);

        pMyObject->TempParamValue = NULL;
    }

    if ( pMyObject->TempParamValueTrans )
    {
        if ( (pMyObject->TempParamValueTrans->Syntax == SLAP_VAR_SYNTAX_handle) && pMyObject->TempParamValueTrans->Variant.varHandle )
        {
            AnscFreeMemory(pMyObject->TempParamValueTrans->Variant.varHandle);
        }

        SlapFreeVariable(pMyObject->TempParamValueTrans);

        pMyObject->TempParamValueTrans = NULL;
    }

    if ( pMyObject->OldParamValue )
    {
        SlapFreeVariable(pMyObject->OldParamValue);

        pMyObject->OldParamValue = NULL;
    }

    pMyObject->bAlreadySet = FALSE;

    return  ANSC_STATUS_SUCCESS;
}
