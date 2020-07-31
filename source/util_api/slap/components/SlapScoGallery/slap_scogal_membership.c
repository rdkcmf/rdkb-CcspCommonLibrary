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

    module:	slap_scogal_membership.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object membership
        functions of the Slap Sco Gallery Object.

        *   SlapScoGalJoinGallery
        *   SlapScoGalQuitGallery

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/12/03    initial revision.

**********************************************************************/


#include "slap_scogal_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoGalJoinGallery
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_VARIABLE*              gallery_var
            );

    description:

        This function is called to add a variable into the gallery.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_VARIABLE*              gallery_var
                Specifies the slap variable to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapScoGalJoinGallery
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              gallery_var
    )
{
    PSLAP_SCO_GALLERY_OBJECT        pMyObject     = (PSLAP_SCO_GALLERY_OBJECT)hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf    = (PSLAP_GOA_INTERFACE     )pMyObject->GetSlapGoaIf   ((ANSC_HANDLE)pMyObject);
    ANSC_HANDLE                     hOlmContainer = (ANSC_HANDLE             )pMyObject->GetOlmContainer((ANSC_HANDLE)pMyObject);
    ULONG                           i             = 0;

    /*
     * If OLM (Object Life-cycle Management) is enabled, we need to examine the variable syntax and
     * handle 'handle' and 'object' variables differently.
     *
     *      - attach the 'handle'/'object' to the OLM container
     */
    if ( !pMyObject->bOlmEnabled || !pSlapGoaIf || !hOlmContainer)
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !gallery_var )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    switch ( gallery_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_object :

                if ( gallery_var->Variant.varObject )
                {
                    pSlapGoaIf->AttachObject
                            (
                                pSlapGoaIf->hOwnerContext,
                                gallery_var->Variant.varObject,
                                hOlmContainer
                            );
                }

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( gallery_var->Variant.varObjectArray )
                {
                    for ( i = 0; i < gallery_var->Variant.varObjectArray->VarCount; i++ )
                    {
                        if ( gallery_var->Variant.varObjectArray->Array.arrayObject[i] )
                        {
                            pSlapGoaIf->AttachObject
                                    (
                                        pSlapGoaIf->hOwnerContext,
                                        gallery_var->Variant.varObjectArray->Array.arrayObject[i],
                                        hOlmContainer
                                    );
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
        SlapScoGalQuitGallery
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_VARIABLE*              gallery_var
            );

    description:

        This function is called to remove a variable from the gallery.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_VARIABLE*              gallery_var
                Specifies the slap variable to be removed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapScoGalQuitGallery
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              gallery_var
    )
{
    PSLAP_SCO_GALLERY_OBJECT        pMyObject     = (PSLAP_SCO_GALLERY_OBJECT)hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf    = (PSLAP_GOA_INTERFACE     )pMyObject->GetSlapGoaIf   ((ANSC_HANDLE)pMyObject);
    ANSC_HANDLE                     hOlmContainer = (ANSC_HANDLE             )pMyObject->GetOlmContainer((ANSC_HANDLE)pMyObject);
    ULONG                           i             = 0;

    /*
     * If OLM (Object Life-cycle Management) is enabled, we need to examine the variable syntax and
     * handle 'handle' and 'object' variables differently.
     *
     *      - detach the 'handle'/'object' from its OLM container
     *      - depends on whether the 'handle'/'object' is acquired, referenced or created,
     *        we either release or delete the 'object' or do nothing.
     */
    if ( !pMyObject->bOlmEnabled || !pSlapGoaIf || !hOlmContainer)
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !gallery_var )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    switch ( gallery_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_object :

                if ( gallery_var->Variant.varObject )
                {
                    pSlapGoaIf->UnlinkObject
                            (
                                pSlapGoaIf->hOwnerContext,
                                gallery_var->Variant.varObject
                            );
                }

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( gallery_var->Variant.varObjectArray )
                {
                    for ( i = 0; i < gallery_var->Variant.varObjectArray->VarCount; i++ )
                    {
                        if ( gallery_var->Variant.varObjectArray->Array.arrayObject[i] )
                        {
                            pSlapGoaIf->UnlinkObject
                                    (
                                        pSlapGoaIf->hOwnerContext,
                                        gallery_var->Variant.varObjectArray->Array.arrayObject[i]
                                    );
                        }
                    }
                }

                break;

        default :

                break;
    }

    return  ANSC_STATUS_SUCCESS;
}
