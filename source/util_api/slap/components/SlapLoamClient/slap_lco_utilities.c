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

    module:	slap_lco_utilities.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced utilities functions
        of the Slap Loam Client Object.

        *   SlapLcoLpcToLoaImcpInvoAnswer
        *   SlapLcoLpcToLoaSlapObject
        *   SlapLcoLoaToLpcImcpInvoCall
        *   SlapLcoLoaToLpcSlapObject
        *   SlapLcoLoaToUoaParamList
        *   SlapLcoUoaToLoaParamList
        *   SlapLcoLoaToUoaVariable
        *   SlapLcoUoaToLoaVariable

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/29/03    initial revision.

**********************************************************************/


#include "slap_lco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLpcToLoaImcpInvoAnswer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                void*                       pImcpAnswer
            );

    description:

        This function is called to translate an IMCP SLAP_INVO_ANSWER
        message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                operation is asociated.

                void*                       pImcpAnswer
                Specifies the IMCP message to be translated.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLpcToLoaImcpInvoAnswer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        void*                       pImcpAnswer
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject           = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty           = (PSLAP_LOAM_CLIENT_PROPERTY )&pMyObject->Property;
    PIMCP_SLAP_INVO_ANSWER          pImcpSlapInvoAnswer = (PIMCP_SLAP_INVO_ANSWER     )pImcpAnswer;
    PIMCP_SLAP_VARIABLE             pImcpSlapVar        = (PIMCP_SLAP_VARIABLE        )NULL;
    PIMCP_SLAP_VARIABLE             pImcpSlapVar2       = (PIMCP_SLAP_VARIABLE        )NULL;
    PIMCP_SLAP_VAR_OBJECT           pImcpSlapVarObj     = (PIMCP_SLAP_VAR_OBJECT      )NULL;
    ULONG                           ulReturnFlag        = (ULONG                      )ImcpSlapInvoAnswerGetReturnFlag(pImcpSlapInvoAnswer);
    ULONG                           ulImcpVarCount      = (ULONG                      )0;
    ULONG                           ulSlapVarFlags      = (ULONG                      )0;
    ANSC_HANDLE                     hSlapObjectLpc      = (ANSC_HANDLE                )NULL;
    ANSC_HANDLE                     hSlapObjectLoa      = (ANSC_HANDLE                )NULL;
    ULONG                           ulSlapObjType       = (ULONG                      )0;
    char*                           pSlapObjName        = (char*                      )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList1   = (PSLAP_PARAMETER_LIST       )NULL;
    PSLAP_VARIABLE                  pReturnedVar1       = (PSLAP_VARIABLE             )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList2   = (PSLAP_PARAMETER_LIST       )NULL;
    PSLAP_VARIABLE                  pReturnedVar2       = (PSLAP_VARIABLE             )NULL;
    PSLAP_PARAMETER_LIST            pImportParamList    = (PSLAP_PARAMETER_LIST       )NULL;
    ULONG                           i                   = 0;
    ULONG                           j                   = 0;
    ULONG                           k                   = 0;
    SLAP_PARAMETER_LIST             inputParamList;

    if ( !pProperty->bMobilityOn )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    ImcpSlapInvoAnswerGetVarCount(pImcpSlapInvoAnswer, ulImcpVarCount);

    if ( TRUE )
    {
        for ( i = 0; i < ulImcpVarCount; i++ )
        {
            pOutputParamList1 = (PSLAP_PARAMETER_LIST)NULL;
            pReturnedVar1     = (PSLAP_VARIABLE      )NULL;
            pOutputParamList2 = (PSLAP_PARAMETER_LIST)NULL;
            pReturnedVar2     = (PSLAP_VARIABLE      )NULL;
            pImportParamList  = (PSLAP_PARAMETER_LIST)NULL;

            ImcpSlapInvoAnswerGetVarByIndex(pImcpSlapInvoAnswer, pImcpSlapVar, i);

            switch ( ImcpSlapVarGetSyntax(pImcpSlapVar) )
            {
                case    SLAP_VAR_SYNTAX_object :

                        hSlapObjectLpc  = (ANSC_HANDLE           )AnscUlongFromNToH(AnscReadUlong(pImcpSlapVar->Data));
                        ulSlapVarFlags  = (ULONG                 )ImcpSlapVarGetFlags(pImcpSlapVar);
                        pImcpSlapVarObj = (PIMCP_SLAP_VAR_OBJECT )ImcpSlapVarGetData (pImcpSlapVar);

                        if ( (ulSlapVarFlags & IMCP_SLAP_VAR_FLAG_objType) &&
                             (ulSlapVarFlags & IMCP_SLAP_VAR_FLAG_objName) )
                        {
                            ulSlapObjType = (ULONG)ImcpSlapVarObjGetType(pImcpSlapVarObj);
                            pSlapObjName  = (char*)ImcpSlapVarObjGetName(pImcpSlapVarObj);
                        }
                        else if ( hSlapObjectLpc )
                        {
                            if ( TRUE )
                            {
                                inputParamList.ParamCount = 0;
                                returnStatus              =
                                    pMyObject->LoaInvokeObject
                                        (
                                            (ANSC_HANDLE)pMyObject,
                                            lpc_party_addr,
                                            hSlapObjectLpc,
                                            "GetObjType",
                                            &inputParamList,
                                            &pOutputParamList1,
                                            &pReturnedVar1,
                                            FALSE,
                                            0       /* using the default timeout value */
                                        );

                                if ( returnStatus != ANSC_STATUS_SUCCESS )
                                {
                                    return  returnStatus;
                                }
                                else if ( !pReturnedVar1 ||
                                          (pReturnedVar1->Syntax != SLAP_VAR_SYNTAX_uint32) )
                                {
                                    return  ANSC_STATUS_FAILURE;
                                }
                                else
                                {
                                    ulSlapObjType = pReturnedVar1->Variant.varUint32;
                                }
                            }

                            if ( TRUE )
                            {
                                inputParamList.ParamCount = 0;
                                returnStatus              =
                                    pMyObject->LoaInvokeObject
                                        (
                                            (ANSC_HANDLE)pMyObject,
                                            lpc_party_addr,
                                            hSlapObjectLpc,
                                            "GetObjName",
                                            &inputParamList,
                                            &pOutputParamList2,
                                            &pReturnedVar2,
                                            FALSE,
                                            0       /* using the default timeout value */
                                        );

                                if ( returnStatus != ANSC_STATUS_SUCCESS )
                                {
                                    return  returnStatus;
                                }
                                else if ( !pReturnedVar2                                               ||
                                          (pReturnedVar2->Syntax            != SLAP_VAR_SYNTAX_string) ||
                                          (pReturnedVar2->Variant.varString == NULL                  ) )
                                {
                                    return  ANSC_STATUS_FAILURE;
                                }
                                else
                                {
                                    pSlapObjName = pReturnedVar2->Variant.varString;
                                }
                            }
                        }
                        else
                        {
                            ulSlapObjType = 0;
                            pSlapObjName  = NULL;
                        }

                        /*
                         * If the IMCP_SLAP_INVO_FLAG_returnedObjData flag is set in the returned
                         * INVO_ANWSER message and this is the first variable in the array, we
                         * shall try to extract the transferred object data from the message. In
                         * this case, the rest variables are considered to be the object data.
                         */
                        if ( (i == 0) && (ulReturnFlag & IMCP_SLAP_INVO_FLAG_returnedObjData) )
                        {
                            pImportParamList = (PSLAP_PARAMETER_LIST)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

                            if ( pImportParamList )
                            {
                                for ( k = 1; k < ulImcpVarCount; k++ )
                                {
                                    ImcpSlapInvoAnswerGetVarByIndex(pImcpSlapInvoAnswer, pImcpSlapVar2, k);

                                    SlapConvFromImcpVar(pImcpSlapVar2, (&pImportParamList->ParamArray[k - 1]));

                                    pImportParamList->ParamCount++;
                                }
                            }
                        }

                        hSlapObjectLoa =
                            pMyObject->LpcToLoaSlapObject
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    lpc_party_addr,
                                    hSlapObjectLpc,
                                    ulSlapObjType,
                                    pSlapObjName,
                                    pImportParamList
                                );

                        ImcpSlapVarObjSetHandle(pImcpSlapVarObj, (ULONG)hSlapObjectLoa);

                        if ( TRUE )
                        {
                            if ( pOutputParamList1 )
                            {
                                SlapFreeParamList(pOutputParamList1);

                                pOutputParamList1 = NULL;
                            }

                            if ( pReturnedVar1 )
                            {
                                SlapFreeVariable(pReturnedVar1);

                                pReturnedVar1 = NULL;
                            }

                            if ( pOutputParamList2 )
                            {
                                SlapFreeParamList(pOutputParamList2);

                                pOutputParamList2 = NULL;
                            }

                            if ( pReturnedVar2 )
                            {
                                SlapFreeVariable(pReturnedVar2);

                                pReturnedVar2 = NULL;
                            }

                            if ( pImportParamList )
                            {
                                SlapFreeParamList(pImportParamList);

                                pImportParamList = NULL;
                            }
                        }

                        break;

                case    SLAP_VAR_SYNTAX_objectArray :

                        for ( j = 0; j < ImcpSlapVarGetLength(pImcpSlapVar) / sizeof(ULONG); j++ )
                        {
                            hSlapObjectLpc = (ANSC_HANDLE)AnscUlongFromNToH(AnscReadUlong(&pImcpSlapVar->Data[j * sizeof(ULONG)]));

                            if ( hSlapObjectLpc )
                            {
                                inputParamList.ParamCount = 0;
                                returnStatus              =
                                    pMyObject->LoaInvokeObject
                                        (
                                            (ANSC_HANDLE)pMyObject,
                                            lpc_party_addr,
                                            hSlapObjectLpc,
                                            "GetObjType",
                                            &inputParamList,
                                            &pOutputParamList1,
                                            &pReturnedVar1,
                                            FALSE,
                                            0       /* using the default timeout value */
                                        );

                                if ( returnStatus != ANSC_STATUS_SUCCESS )
                                {
                                    return  returnStatus;
                                }
                                else if ( !pReturnedVar1 ||
                                          (pReturnedVar1->Syntax != SLAP_VAR_SYNTAX_uint32) )
                                {
                                    return  ANSC_STATUS_FAILURE;
                                }
                                else
                                {
                                    ulSlapObjType = pReturnedVar1->Variant.varUint32;
                                }
                            }
                            else
                            {
                                ulSlapObjType = 0;
                            }

                            if ( hSlapObjectLpc )
                            {
                                inputParamList.ParamCount = 0;
                                returnStatus              =
                                    pMyObject->LoaInvokeObject
                                        (
                                            (ANSC_HANDLE)pMyObject,
                                            lpc_party_addr,
                                            hSlapObjectLpc,
                                            "GetObjName",
                                            &inputParamList,
                                            &pOutputParamList2,
                                            &pReturnedVar2,
                                            FALSE,
                                            0       /* using the default timeout value */
                                        );

                                if ( returnStatus != ANSC_STATUS_SUCCESS )
                                {
                                    return  returnStatus;
                                }
                                else if ( !pReturnedVar2                                               ||
                                          (pReturnedVar2->Syntax            != SLAP_VAR_SYNTAX_string) ||
                                          (pReturnedVar2->Variant.varString == NULL                  ) )
                                {
                                    return  ANSC_STATUS_FAILURE;
                                }
                                else
                                {
                                    pSlapObjName = pReturnedVar2->Variant.varString;
                                }
                            }
                            else
                            {
                                pSlapObjName = NULL;
                            }

                            hSlapObjectLoa  =
                                pMyObject->LpcToLoaSlapObject
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        lpc_party_addr,
                                        hSlapObjectLpc,
                                        ulSlapObjType,
                                        pSlapObjName,
                                        NULL
                                    );

                            AnscWriteUlong(&pImcpSlapVar->Data[j * sizeof(ULONG)], AnscUlongFromHToN((ULONG)hSlapObjectLoa));

                            if ( TRUE )
                            {
                                if ( pOutputParamList1 )
                                {
                                    SlapFreeParamList(pOutputParamList1);

                                    pOutputParamList1 = NULL;
                                }

                                if ( pReturnedVar1 )
                                {
                                    SlapFreeVariable(pReturnedVar1);

                                    pReturnedVar1 = NULL;
                                }

                                if ( pOutputParamList2 )
                                {
                                    SlapFreeParamList(pOutputParamList2);

                                    pOutputParamList2 = NULL;
                                }

                                if ( pReturnedVar2 )
                                {
                                    SlapFreeVariable(pReturnedVar2);

                                    pReturnedVar2 = NULL;
                                }
                            }
                        }

                        break;

                default :

                        break;
            }
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapLcoLpcToLoaSlapObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                ANSC_HANDLE                 obj_handle,
                ULONG                       obj_type,
                char*                       obj_name,
                SLAP_PARAMETER_LIST*        import_param_list
            );

    description:

        This function is called to translate a SLAP LOA object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                operation is asociated.

                ANSC_HANDLE                 obj_handle
                Specifies the SLAP object's LPC handle to be translated.

                ULONG                       obj_type
                Specifies the obj_type of the SLAP object.

                char*                       obj_name
                Specifies the obj_name of the SLAP object.

                SLAP_PARAMETER_LIST*        import_param_list
                Specifies the object data to be imported.

    return:     loa object.

**********************************************************************/

ANSC_HANDLE
SlapLcoLpcToLoaSlapObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 obj_handle,
        ULONG                       obj_type,
        char*                       obj_name,
        SLAP_PARAMETER_LIST*        import_param_list
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject          = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT)pMyObject->hSlapEnvController;
    PSLAP_OBJ_AGENT_OBJECT          pSlapObjAgent      = (PSLAP_OBJ_AGENT_OBJECT     )NULL;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE        )pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
    ANSC_HANDLE                     hClonedObject      = (ANSC_HANDLE                )NULL;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord     = (PSLAP_OBJ_RECORD_OBJECT    )NULL;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent  = (PSLAP_SRV_COMPONENT_OBJECT )NULL;
    PSLAP_SCO_STANDARD_OBJECT       pSlapScoStandard   = (PSLAP_SCO_STANDARD_OBJECT  )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList   = (PSLAP_PARAMETER_LIST       )NULL;
    PSLAP_VARIABLE                  pReturnedVar       = (PSLAP_VARIABLE             )NULL;
    SLAP_PARAMETER_LIST             inputParamList;

    if ( !obj_handle )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapObjAgent =
            (PSLAP_OBJ_AGENT_OBJECT)SlapCreateObjAgent
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSlapObjAgent )
        {
            return  (ANSC_HANDLE)NULL;
        }
    }

    if ( TRUE )
    {
        pSlapObjAgent->ObjType      = obj_type;
        pSlapObjAgent->ObjMode      = SLAP_OAO_OBJ_MODE_relay;
        pSlapObjAgent->hOriginalObj = obj_handle;
        pSlapObjAgent->hClonedObj   = (ANSC_HANDLE)NULL;
        pSlapObjAgent->bDirty       = FALSE;

        pSlapObjAgent->SetObjName((ANSC_HANDLE)pSlapObjAgent, obj_name);
    }

    if ( !(obj_type & SLAP_OBJ_TYPE_mobile) )
    {
        return  (ANSC_HANDLE)pSlapObjAgent;
    }
    else if ( !pSlapUoaIf->VerifyObjDescriptor
                (
                    pSlapUoaIf->hOwnerContext,
                    obj_name
                ) )
    {
        return  (ANSC_HANDLE)pSlapObjAgent;
    }
    else
    {
        hClonedObject =
            pSlapUoaIf->CreateObject
                (
                    pSlapUoaIf->hOwnerContext,
                    pMyObject->hMobileContainer,
                    obj_name,
                    (ANSC_HANDLE)NULL
                );
    }

    if ( !hClonedObject )
    {
        return  (ANSC_HANDLE)pSlapObjAgent;
    }
    else
    {
        pSlapObjRecord    = (PSLAP_OBJ_RECORD_OBJECT   )hClonedObject;
        pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)pSlapObjRecord->hSlapSrvComponent;
        pSlapScoStandard  = (PSLAP_SCO_STANDARD_OBJECT )pSlapSrvComponent;
    }

    if ( import_param_list )
    {
        returnStatus =
            pSlapScoStandard->ImportMe
                (
                    (ANSC_HANDLE)pSlapScoStandard,
                    import_param_list
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            pSlapObjAgent->Remove((ANSC_HANDLE)pSlapObjAgent);

            pSlapObjAgent = NULL;

            goto  EXIT1;
        }
    }
    else
    {
        inputParamList.ParamCount = 0;
        returnStatus              =
            pMyObject->LoaInvokeObject
                (
                    (ANSC_HANDLE)pMyObject,
                    lpc_party_addr,
                    obj_handle,
                    "ExportMe",
                    &inputParamList,
                    &pOutputParamList,
                    &pReturnedVar,
                    FALSE,
                    0       /* using the default timeout value */
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            pSlapObjAgent->Remove((ANSC_HANDLE)pSlapObjAgent);

            pSlapObjAgent = NULL;

            goto  EXIT1;
        }
        else if ( pOutputParamList )
        {
            returnStatus =
                pSlapScoStandard->ImportMe
                    (
                        (ANSC_HANDLE)pSlapScoStandard,
                        pOutputParamList
                    );
        }

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            pSlapObjAgent->Remove((ANSC_HANDLE)pSlapObjAgent);

            pSlapObjAgent = NULL;

            goto  EXIT1;
        }
    }

    pSlapObjAgent->ObjMode    = SLAP_OAO_OBJ_MODE_clone;
    pSlapObjAgent->hClonedObj = hClonedObject;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pOutputParamList )
    {
        SlapFreeParamList(pOutputParamList);

        pOutputParamList = NULL;
    }

    if ( pReturnedVar )
    {
        SlapFreeVariable(pReturnedVar);

        pReturnedVar = NULL;
    }

    return  (ANSC_HANDLE)pSlapObjAgent;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLoaToLpcImcpInvoCall
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                void*                       pImcpCall
            );

    description:

        This function is called to translate an IMCP SLAP_INVO_CALL
        message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                operation is asociated.

                void*                       pImcpCall
                Specifies the IMCP message to be translated.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLoaToLpcImcpInvoCall
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        void*                       pImcpCall
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject          = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty          = (PSLAP_LOAM_CLIENT_PROPERTY )&pMyObject->Property;
    PIMCP_SLAP_INVO_CALL            pImcpSlapInvoCall  = (PIMCP_SLAP_INVO_CALL       )pImcpCall;
    PIMCP_SLAP_VARIABLE             pImcpSlapVar       = (PIMCP_SLAP_VARIABLE        )NULL;
    ULONG                           ulImcpVarCount     = (ULONG                      )0;
    ANSC_HANDLE                     hSlapObjectLpc     = (ANSC_HANDLE                )NULL;
    ANSC_HANDLE                     hSlapObjectLoa     = (ANSC_HANDLE                )NULL;
    ULONG                           i                  = 0;
    ULONG                           j                  = 0;

    if ( !pProperty->bMobilityOn )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    ImcpSlapInvoCallGetVarCount(pImcpSlapInvoCall, ulImcpVarCount);

    if ( TRUE )
    {
        for ( i = 0; i < ulImcpVarCount; i++ )
        {
            ImcpSlapInvoCallGetVarByIndex(pImcpSlapInvoCall, pImcpSlapVar, i);

            switch ( ImcpSlapVarGetSyntax(pImcpSlapVar) )
            {
                case    SLAP_VAR_SYNTAX_object :

                        hSlapObjectLoa = (ANSC_HANDLE)AnscUlongFromNToH(AnscReadUlong(pImcpSlapVar->Data));
                        hSlapObjectLpc =
                            pMyObject->LoaToLpcSlapObject
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    lpc_party_addr,
                                    hSlapObjectLoa
                                );

                        AnscWriteUlong(pImcpSlapVar->Data, AnscUlongFromHToN((ULONG)hSlapObjectLpc));

                        break;

                case    SLAP_VAR_SYNTAX_objectArray :

                        for ( j = 0; j < ImcpSlapVarGetLength(pImcpSlapVar) / sizeof(ULONG); j++ )
                        {
                            hSlapObjectLoa = (ANSC_HANDLE)AnscUlongFromNToH(AnscReadUlong(&pImcpSlapVar->Data[j * sizeof(ULONG)]));
                            hSlapObjectLpc =
                                pMyObject->LoaToLpcSlapObject
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        lpc_party_addr,
                                        hSlapObjectLoa
                                    );

                            AnscWriteUlong(&pImcpSlapVar->Data[j * sizeof(ULONG)], AnscUlongFromHToN((ULONG)hSlapObjectLpc));
                        }

                        break;

                default :

                        break;
            }
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapLcoLoaToLpcSlapObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                ANSC_HANDLE                 obj_handle
            );

    description:

        This function is called to translate a SLAP LPC object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                operation is asociated.

                ANSC_HANDLE                 obj_handle
                Specifies the SLAP object's LOA handle to be translated.

    return:     lpc object.

**********************************************************************/

ANSC_HANDLE
SlapLcoLoaToLpcSlapObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 obj_handle
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject          = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_OBJ_AGENT_OBJECT          pSlapObjAgent      = (PSLAP_OBJ_AGENT_OBJECT     )obj_handle;
    ANSC_HANDLE                     hOriginalObject    = (ANSC_HANDLE                )pSlapObjAgent->hOriginalObj;
    ANSC_HANDLE                     hClonedObject      = (ANSC_HANDLE                )pSlapObjAgent->hClonedObj;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord     = (PSLAP_OBJ_RECORD_OBJECT    )NULL;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent  = (PSLAP_SRV_COMPONENT_OBJECT )NULL;
    PSLAP_SCO_STANDARD_OBJECT       pSlapScoStandard   = (PSLAP_SCO_STANDARD_OBJECT  )NULL;
    PSLAP_PARAMETER_LIST            pInputParamList    = (PSLAP_PARAMETER_LIST       )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList   = (PSLAP_PARAMETER_LIST       )NULL;
    PSLAP_VARIABLE                  pReturnedVar       = (PSLAP_VARIABLE             )NULL;

    if ( (pSlapObjAgent->ObjMode == SLAP_OAO_OBJ_MODE_relay) ||
         (pSlapObjAgent->ObjMode == SLAP_OAO_OBJ_MODE_local) )
    {
        return  hOriginalObject;
    }
    else if ( !pSlapObjAgent->bDirty )
    {
        return  hOriginalObject;
    }
    else if ( !hClonedObject )
    {
        return  hOriginalObject;
    }
    else
    {
        pSlapObjRecord    = (PSLAP_OBJ_RECORD_OBJECT   )hClonedObject;
        pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)pSlapObjRecord->hSlapSrvComponent;
        pSlapScoStandard  = (PSLAP_SCO_STANDARD_OBJECT )pSlapSrvComponent;
    }

    if ( TRUE )
    {
        returnStatus =
            pSlapScoStandard->ExportMe
                (
                    (ANSC_HANDLE)pSlapScoStandard,
                    &pOutputParamList
                );

        if ( !pOutputParamList )
        {
            return  hOriginalObject;
        }

        pInputParamList = pOutputParamList;
        returnStatus    =
            pMyObject->LoaInvokeObject
                (
                    (ANSC_HANDLE)pMyObject,
                    lpc_party_addr,
                    hOriginalObject,
                    "ImportMe",
                    pInputParamList,
                    &pOutputParamList,
                    &pReturnedVar,
                    FALSE,
                    0       /* using the default timeout value */
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto  EXIT1;
        }
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pInputParamList )
    {
        SlapFreeParamList(pInputParamList);

        pInputParamList = NULL;
    }

    if ( pOutputParamList )
    {
        SlapFreeParamList(pOutputParamList);

        pOutputParamList = NULL;
    }

    if ( pReturnedVar )
    {
        SlapFreeVariable(pReturnedVar);

        pReturnedVar = NULL;
    }

    return  hOriginalObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLoaToUoaParamList
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to convert parameter list from LOA to
        UOA environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLoaToUoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject    = (PSLAP_LOAM_CLIENT_OBJECT  )hThisObject;
    ULONG                           i            = 0;

    for ( i = 0; i < param_list->ParamCount; i++ )
    {
        returnStatus =
            pMyObject->LoaToUoaVariable
                (
                    (ANSC_HANDLE)pMyObject,
                    &param_list->ParamArray[i]
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            break;
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoUoaToLoaParamList
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to convert parameter list from UOA to
        LOA environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoUoaToLoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject    = (PSLAP_LOAM_CLIENT_OBJECT  )hThisObject;
    ULONG                           i            = 0;

    for ( i = 0; i < param_list->ParamCount; i++ )
    {
        returnStatus =
            pMyObject->UoaToLoaVariable
                (
                    (ANSC_HANDLE)pMyObject,
                    &param_list->ParamArray[i]
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            break;
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLoaToUoaVariable
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_VARIABLE*              slap_var
            );

    description:

        This function is called to convert a variable from LOA to UOA
        environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_VARIABLE*              slap_var
                Specifies the variable to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLoaToUoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_OBJ_AGENT_OBJECT          pSlapObjAgent = (PSLAP_OBJ_AGENT_OBJECT    )NULL;
    ULONG                           i             = 0;

    if ( !slap_var )
    {
        return  ANSC_STATUS_FAILURE;
    }

    switch ( slap_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_handle :

                /*
                 * Unlike SLAP_GOA, SLAP_LOA cannot apply translation to 'handle' or 'handleArray'
                 * variables simply because we have no way to manage the object life cycles at this
                 * level. This limitation requires that all mobile objects MUST NOT use any 'handle'
                 * or 'handleArray' variables.
                 */

                break;

        case    SLAP_VAR_SYNTAX_handleArray :

                /*
                 * Unlike SLAP_GOA, SLAP_LOA cannot apply translation to 'handle' or 'handleArray'
                 * variables simply because we have no way to manage the object life cycles at this
                 * level. This limitation requires that all mobile objects MUST NOT use any 'handle'
                 * or 'handleArray' variables.
                 */

                break;

        case    SLAP_VAR_SYNTAX_object :

                pSlapObjAgent = (PSLAP_OBJ_AGENT_OBJECT)slap_var->Variant.varObject;

                if ( pSlapObjAgent )
                {
                    if ( pSlapObjAgent->ObjMode != SLAP_OAO_OBJ_MODE_local )
                    {
                        return  ANSC_STATUS_FAILURE;
                    }
                    else
                    {
                        slap_var->Variant.varObject = (SLAP_OBJECT)pSlapObjAgent->hOriginalObj;
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( slap_var->Variant.varObjectArray )
                {
                    for ( i = 0; i < slap_var->Variant.varObjectArray->VarCount; i++ )
                    {
                        pSlapObjAgent = (PSLAP_OBJ_AGENT_OBJECT)slap_var->Variant.varObjectArray->Array.arrayObject[i];

                        if ( pSlapObjAgent )
                        {
                            if ( pSlapObjAgent->ObjMode != SLAP_OAO_OBJ_MODE_local )
                            {
                                return  ANSC_STATUS_FAILURE;
                            }
                            else
                            {
                                slap_var->Variant.varObjectArray->Array.arrayObject[i] = (SLAP_OBJECT)pSlapObjAgent->hOriginalObj;
                            }
                        }
                    }
                }

                break;

        default :

                break;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoUoaToLoaVariable
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_VARIABLE*              slap_var
            );

    description:

        This function is called to convert a variable from UOA to LOA
        environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_VARIABLE*              slap_var
                Specifies the variable to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoUoaToLoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject          = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT)pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE        )pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
    PSLAP_OBJ_AGENT_OBJECT          pSlapObjAgent      = (PSLAP_OBJ_AGENT_OBJECT     )NULL;
    ULONG                           i                  = 0;

    if ( !slap_var )
    {
        return  ANSC_STATUS_FAILURE;
    }

    switch ( slap_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_handle :

                /*
                 * Unlike SLAP_GOA, SLAP_LOA cannot apply translation to 'handle' or 'handleArray'
                 * variables simply because we have no way to manage the object life cycles at this
                 * level. This limitation requires that all mobile objects MUST NOT use any 'handle'
                 * or 'handleArray' variables.
                 */

                break;

        case    SLAP_VAR_SYNTAX_handleArray :

                /*
                 * Unlike SLAP_GOA, SLAP_LOA cannot apply translation to 'handle' or 'handleArray'
                 * variables simply because we have no way to manage the object life cycles at this
                 * level. This limitation requires that all mobile objects MUST NOT use any 'handle'
                 * or 'handleArray' variables.
                 */

                break;

        case    SLAP_VAR_SYNTAX_object :

                if ( slap_var->Variant.varObject )
                {
                    pSlapObjAgent =
                        (PSLAP_OBJ_AGENT_OBJECT)SlapCreateObjAgent
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    if ( !pSlapObjAgent )
                    {
                        /*
                         * We cannot create a new LOA Obj Agent, we then have to dereference this
                         * UOA object because we cannot return it the way it is. For SLAP_OBJECT
                         * object, we have to explicitly delete it from UOA.
                         */
                        pSlapUoaIf->DeleteObject
                                (
                                    pSlapUoaIf->hOwnerContext,
                                    (ANSC_HANDLE)slap_var->Variant.varObject
                                );

                        slap_var->Variant.varObject = (SLAP_HANDLE)NULL;
                    }
                    else
                    {
                        pSlapObjAgent->ObjType      = 0;
                        pSlapObjAgent->ObjMode      = SLAP_OAO_OBJ_MODE_local;
                        pSlapObjAgent->hOriginalObj = (ANSC_HANDLE)slap_var->Variant.varObject;
                        pSlapObjAgent->hClonedObj   = (ANSC_HANDLE)NULL;
                        pSlapObjAgent->bDirty       = FALSE;

                        pSlapObjAgent->SetObjName((ANSC_HANDLE)pSlapObjAgent, NULL);

                        slap_var->Variant.varObject = (SLAP_HANDLE)pSlapObjAgent;
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( slap_var->Variant.varObjectArray )
                {
                    for ( i = 0; i < slap_var->Variant.varObjectArray->VarCount; i++ )
                    {
                        if ( slap_var->Variant.varObjectArray->Array.arrayObject[i] )
                        {
                            pSlapObjAgent =
                                (PSLAP_OBJ_AGENT_OBJECT)SlapCreateObjAgent
                                    (
                                        pMyObject->hContainerContext,
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)NULL
                                    );

                            if ( !pSlapObjAgent )
                            {
                                /*
                                 * We cannot create a new LOA Obj Agent, we then have to derefer-
                                 * ence this UOA object because we cannot return it the way it is.
                                 * For SLAP_OBJECT object, we have to explicitly delete it from UOA.
                                 */
                                pSlapUoaIf->DeleteObject
                                        (
                                            pSlapUoaIf->hOwnerContext,
                                            (ANSC_HANDLE)slap_var->Variant.varObjectArray->Array.arrayObject[i]
                                        );

                                slap_var->Variant.varObjectArray->Array.arrayObject[i] = (SLAP_HANDLE)NULL;
                            }
                            else
                            {
                                pSlapObjAgent->ObjType      = 0;
                                pSlapObjAgent->ObjMode      = SLAP_OAO_OBJ_MODE_local;
                                pSlapObjAgent->hOriginalObj = slap_var->Variant.varObjectArray->Array.arrayObject[i];
                                pSlapObjAgent->hClonedObj   = (ANSC_HANDLE)NULL;
                                pSlapObjAgent->bDirty       = FALSE;

                                pSlapObjAgent->SetObjName((ANSC_HANDLE)pSlapObjAgent, NULL);

                                slap_var->Variant.varObjectArray->Array.arrayObject[i] = (SLAP_HANDLE)pSlapObjAgent;
                            }
                        }
                    }
                }

                break;

        default :

                break;
    }

    return  ANSC_STATUS_SUCCESS;
}
