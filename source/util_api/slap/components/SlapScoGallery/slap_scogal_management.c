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

    module:	slap_scogal_management.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object management
        functions of the Slap Sco Gallery Object.

        *   SlapScoGalGetVariable
        *   SlapScoGalSetVariable
        *   SlapScoGalAddVariable
        *   SlapScoGalDelVariable
        *   SlapScoGalDelAllVariables

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/15/03    initial revision.

**********************************************************************/


#include "slap_scogal_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        SlapScoGalGetVariable
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to retrieve a variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     slap variable.

**********************************************************************/

SLAP_VARIABLE*
SlapScoGalGetVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    PSLAP_SCO_GALLERY_OBJECT        pMyObject       = (PSLAP_SCO_GALLERY_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoVariable    = (PANSC_ATOM_TABLE_OBJECT )pMyObject->hAtoVariable;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR   )NULL;
    PSLAP_VARIABLE                  pGalVariable    = (PSLAP_VARIABLE          )NULL;
    PSLAP_VARIABLE                  pReturnedVar    = (PSLAP_VARIABLE          )NULL;

    if ( !param_list || (param_list->ParamCount < 1) )
    {
        return  NULL;
    }
    else if ( param_list->ParamArray[0].Syntax != SLAP_VAR_SYNTAX_string )
    {
        return  NULL;
    }
    else if ( !param_list->ParamArray[0].Variant.varString )
    {
        return  NULL;
    }

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pAtoVariable->GetAtomByName
            (
                (ANSC_HANDLE)pAtoVariable,
                param_list->ParamArray[0].Variant.varString
            );

    if ( !pAtomDescriptor )
    {
        return  NULL;
    }
    else
    {
        pGalVariable = (PSLAP_VARIABLE)pAtomDescriptor->hContext;
    }

    if ( !pGalVariable )
    {
        return  NULL;
    }
    else
    {
        pReturnedVar = (PSLAP_VARIABLE)AnscAllocateMemory(sizeof(SLAP_VARIABLE));

        if ( !pReturnedVar )
        {
            return  NULL;
        }
        else
        {
            SlapInitVariable (pReturnedVar);
            SlapCloneVariable(pGalVariable, pReturnedVar);
        }
    }

    return  pReturnedVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoGalSetVariable
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to configure a variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapScoGalSetVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_GALLERY_OBJECT        pMyObject       = (PSLAP_SCO_GALLERY_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoVariable    = (PANSC_ATOM_TABLE_OBJECT )pMyObject->hAtoVariable;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR   )NULL;
    PSLAP_VARIABLE                  pGalVariable    = (PSLAP_VARIABLE          )NULL;
    PSLAP_VARIABLE                  pTbsVariable    = (PSLAP_VARIABLE          )NULL;
    BOOL                            bSameVar        = FALSE;

    if ( !param_list || (param_list->ParamCount < 2) )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( param_list->ParamArray[0].Syntax != SLAP_VAR_SYNTAX_string )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( !param_list->ParamArray[0].Variant.varString )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        pTbsVariable = &param_list->ParamArray[1];
    }

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pAtoVariable->GetAtomByName
            (
                (ANSC_HANDLE)pAtoVariable,
                param_list->ParamArray[0].Variant.varString
            );

    if ( !pAtomDescriptor )
    {
        returnStatus =
            pMyObject->AddVariable
                (
                    (ANSC_HANDLE)pMyObject,
                    param_list
                );

        return  returnStatus;
    }
    else
    {
        pGalVariable = (PSLAP_VARIABLE)pAtomDescriptor->hContext;
    }

    if ( !pGalVariable )
    {
        pGalVariable = (PSLAP_VARIABLE)AnscAllocateMemory(sizeof(SLAP_VARIABLE));

        if ( !pGalVariable )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            SlapInitVariable(pGalVariable);

            pAtomDescriptor->hContext = (ANSC_HANDLE)pGalVariable;
        }

        bSameVar = FALSE;
    }
    else
    {
        SlapEqualVariables(pGalVariable, pTbsVariable, bSameVar);
    }

    if ( bSameVar )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        returnStatus =
            pMyObject->QuitGallery
                (
                    (ANSC_HANDLE)pMyObject,
                    pGalVariable
                );

        SlapCleanVariable(pGalVariable);
        SlapInitVariable (pGalVariable);
    }

    SlapCloneVariable(pTbsVariable, pGalVariable);

    returnStatus =
        pMyObject->JoinGallery
            (
                (ANSC_HANDLE)pMyObject,
                pGalVariable
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoGalAddVariable
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to add a variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapScoGalAddVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_GALLERY_OBJECT        pMyObject       = (PSLAP_SCO_GALLERY_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoVariable    = (PANSC_ATOM_TABLE_OBJECT )pMyObject->hAtoVariable;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR   )NULL;
    PSLAP_VARIABLE                  pGalVariable    = (PSLAP_VARIABLE          )NULL;
    PSLAP_VARIABLE                  pTbsVariable    = (PSLAP_VARIABLE          )NULL;

    if ( !param_list || (param_list->ParamCount < 2) )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( param_list->ParamArray[0].Syntax != SLAP_VAR_SYNTAX_string )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( !param_list->ParamArray[0].Variant.varString )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        pTbsVariable = &param_list->ParamArray[1];
        pGalVariable = NULL;
    }

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pAtoVariable->GetAtomByName
            (
                (ANSC_HANDLE)pAtoVariable,
                param_list->ParamArray[0].Variant.varString
            );

    if ( pAtomDescriptor )
    {
        return  ANSC_STATUS_COLLISION;
    }
    else
    {
        pGalVariable = (PSLAP_VARIABLE)AnscAllocateMemory(sizeof(SLAP_VARIABLE));

        if ( !pGalVariable )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            SlapInitVariable (pGalVariable);
            SlapCloneVariable(pTbsVariable, pGalVariable);
        }
    }

    pAtoVariable->AddAtomByName
        (
            (ANSC_HANDLE)pAtoVariable,
            param_list->ParamArray[0].Variant.varString,
            0,
            (ANSC_HANDLE)pGalVariable
        );

    returnStatus =
        pMyObject->JoinGallery
            (
                (ANSC_HANDLE)pMyObject,
                pGalVariable
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoGalDelVariable
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to delete a variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list to be examined.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapScoGalDelVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_GALLERY_OBJECT        pMyObject       = (PSLAP_SCO_GALLERY_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoVariable    = (PANSC_ATOM_TABLE_OBJECT )pMyObject->hAtoVariable;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR   )NULL;
    PSLAP_VARIABLE                  pGalVariable    = (PSLAP_VARIABLE          )NULL;

    if ( !param_list || (param_list->ParamCount < 1) )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( param_list->ParamArray[0].Syntax != SLAP_VAR_SYNTAX_string )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( !param_list->ParamArray[0].Variant.varString )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pAtoVariable->GetAtomByName
            (
                (ANSC_HANDLE)pAtoVariable,
                param_list->ParamArray[0].Variant.varString
            );

    if ( !pAtomDescriptor )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        pGalVariable = (PSLAP_VARIABLE)pAtomDescriptor->hContext;

        returnStatus =
            pMyObject->QuitGallery
                (
                    (ANSC_HANDLE)pMyObject,
                    pGalVariable
                );

        if ( pGalVariable )
        {
            SlapFreeVariable(pGalVariable);
        }
    }

    returnStatus =
        pAtoVariable->DelAtomByName
            (
                pAtoVariable,
                param_list->ParamArray[0].Variant.varString
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoGalDelAllVariables
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all variables.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapScoGalDelAllVariables
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SCO_GALLERY_OBJECT        pMyObject       = (PSLAP_SCO_GALLERY_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoVariable    = (PANSC_ATOM_TABLE_OBJECT )pMyObject->hAtoVariable;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR   )NULL;
    PSLAP_VARIABLE                  pSlapVariable   = (PSLAP_VARIABLE          )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           i               = 0;

    /*AnscAcquireSpinLock(&pAtoVariable->AdoStrTableSpinLock);*/

    for ( i = 0; i < pAtoVariable->AdoStrTableSize; i++ )
    {
        pSLinkEntry = AnscSListPopEntry(&pAtoVariable->AdoStrTable[i]);

        while ( pSLinkEntry )
        {
            pAtomDescriptor = ACCESS_ANSC_ATOM_DESCRIPTOR(pSLinkEntry);
            pSLinkEntry     = AnscSListPopEntry(&pAtoVariable->AdoStrTable[i]);
            pSlapVariable   = pAtomDescriptor->hContext;

            pMyObject->QuitGallery
                    (
                        (ANSC_HANDLE)pMyObject,
                        pSlapVariable
                    );

            if ( pSlapVariable )
            {
                SlapFreeVariable(pSlapVariable);
            }

            if ( pAtomDescriptor->AtomName )
            {
                AnscFreeMemory(pAtomDescriptor->AtomName);
            }

            AnscFreeMemory(pAtomDescriptor);
        }
    }

    /*AnscReleaseSpinLock(&pAtoVariable->AdoStrTableSpinLock);*/

    return  ANSC_STATUS_SUCCESS;
}
