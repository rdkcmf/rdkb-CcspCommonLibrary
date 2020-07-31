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

    module:	slap_owouoac_management.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Slap Owo Uoac Object.

        *   SlapOwoUoacAttachObjWrapper
        *   SlapOwoUoacDetachObjWrapper
        *   SlapOwoUoacRemoveObjWrapper
        *   SlapOwoUoacCleanOwoTable

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/11/03    initial revision.

**********************************************************************/


#include "slap_owouoac_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOwoUoacAttachObjWrapper
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjWrapper
            );

    description:

        This function is called to attach the specified GOA obj wrapper.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjWrapper
                Specifies the GOA obj wrapper to be attached.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOwoUoacAttachObjWrapper
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjWrapper
    )
{
    PSLAP_OWO_UOAC_OBJECT           pMyObject    = (PSLAP_OWO_UOAC_OBJECT   )hThisObject;
    PSLAP_OBJ_WRAPPER_OBJECT        pObjWrapper  = (PSLAP_OBJ_WRAPPER_OBJECT)hObjWrapper;
    ULONG                           ulHashIndex  = AnscHashUlong((ULONG)pObjWrapper, SLAP_OWOUOAC_OWO_TABLE_SIZE);

    pObjWrapper->hGoaContainer = (ANSC_HANDLE)pMyObject;

    AnscAcquireLock   (&pMyObject->OwoTableLock);
    AnscQueuePushEntry(&pMyObject->OwoTable[ulHashIndex], &pObjWrapper->Linkage);
    AnscReleaseLock   (&pMyObject->OwoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOwoUoacDetachObjWrapper
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjWrapper
            );

    description:

        This function is called to detach the specified GOA obj wrapper.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjWrapper
                Specifies the GOA obj wrapper to be detached.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOwoUoacDetachObjWrapper
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjWrapper
    )
{
    PSLAP_OWO_UOAC_OBJECT           pMyObject    = (PSLAP_OWO_UOAC_OBJECT   )hThisObject;
    PSLAP_OBJ_WRAPPER_OBJECT        pObjWrapper  = (PSLAP_OBJ_WRAPPER_OBJECT)hObjWrapper;
    ULONG                           ulHashIndex  = AnscHashUlong((ULONG)pObjWrapper, SLAP_OWOUOAC_OWO_TABLE_SIZE);
    BOOL                            bOwoFound    = TRUE;

    AnscAcquireLock(&pMyObject->OwoTableLock);

    if ( AnscQueuePopEntryByLink(&pMyObject->OwoTable[ulHashIndex], &pObjWrapper->Linkage) )
    {
        bOwoFound = TRUE;

        pObjWrapper->hGoaContainer = (ANSC_HANDLE)NULL;
    }
    else
    {
        bOwoFound = FALSE;
    }

    AnscReleaseLock(&pMyObject->OwoTableLock);

    if ( !bOwoFound )
    {
        return  ANSC_STATUS_CANT_FIND;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOwoUoacRemoveObjWrapper
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjWrapper
            );

    description:

        This function is called to remove the specified GOA obj wrapper.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjWrapper
                Specifies the GOA obj wrapper to be removed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOwoUoacRemoveObjWrapper
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjWrapper
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_OWO_UOAC_OBJECT           pMyObject    = (PSLAP_OWO_UOAC_OBJECT   )hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf   = (PSLAP_GOA_INTERFACE     )pMyObject->hSlapGoaIf;
    PSLAP_OBJ_WRAPPER_OBJECT        pObjWrapper  = (PSLAP_OBJ_WRAPPER_OBJECT)hObjWrapper;

    returnStatus =
        pMyObject->DetachObjWrapper
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pObjWrapper
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }
    else
    {
        switch ( pObjWrapper->ObjMode )
        {
            case    SLAP_OWO_OBJ_MODE_acquired :

                    if ( pSlapGoaIf )
                    {
                        returnStatus =
                            pSlapGoaIf->RelObjectAccess
                                (
                                    pSlapGoaIf->hOwnerContext,
                                    (ANSC_HANDLE)pObjWrapper
                                );
                    }

                    break;

            case    SLAP_OWO_OBJ_MODE_referenced :

                    break;

            case    SLAP_OWO_OBJ_MODE_created :

                    if ( pSlapGoaIf )
                    {
                        returnStatus =
                            pSlapGoaIf->DeleteObject
                                (
                                    pSlapGoaIf->hOwnerContext,
                                    (ANSC_HANDLE)pObjWrapper
                                );
                    }

                    break;

            default :

                    break;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOwoUoacCleanOwoTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to detach all GOA obj wrappers.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOwoUoacCleanOwoTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OWO_UOAC_OBJECT           pMyObject    = (PSLAP_OWO_UOAC_OBJECT   )hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf   = (PSLAP_GOA_INTERFACE     )pMyObject->hSlapGoaIf;
    PSLAP_OBJ_WRAPPER_OBJECT        pObjWrapper  = (PSLAP_OBJ_WRAPPER_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY      )NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->OwoTableLock);

    for ( i = 0; i < SLAP_OWOUOAC_OWO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->OwoTable[i]);

        while ( pSLinkEntry )
        {
            pObjWrapper = ACCESS_SLAP_OBJ_WRAPPER_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscQueuePopEntry(&pMyObject->OwoTable[i]);

            pObjWrapper->hGoaContainer = (ANSC_HANDLE)NULL;

            switch ( pObjWrapper->ObjMode )
            {
                case    SLAP_OWO_OBJ_MODE_acquired :

                        if ( pSlapGoaIf )
                        {
                            pSlapGoaIf->RelObjectAccess
                                    (
                                        pSlapGoaIf->hOwnerContext,
                                        (ANSC_HANDLE)pObjWrapper
                                    );
                        }

                        break;

                case    SLAP_OWO_OBJ_MODE_referenced :

                        break;

                case    SLAP_OWO_OBJ_MODE_created :

                        if ( pSlapGoaIf )
                        {
                            pSlapGoaIf->DeleteObject
                                    (
                                        pSlapGoaIf->hOwnerContext,
                                        (ANSC_HANDLE)pObjWrapper
                                    );
                        }

                        break;

                default :

                        break;
            }
        }
    }

    AnscReleaseLock(&pMyObject->OwoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
