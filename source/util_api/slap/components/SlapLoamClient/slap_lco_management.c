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

    module:	slap_lco_management.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Slap Loam Client Object.

        *   SlapLcoGetObjectStubByName
        *   SlapLcoGetObjectStubByHandle
        *   SlapLcoAddObjectStub
        *   SlapLcoDelObjectStub
        *   SlapLcoDelAllObjectStubs

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/22/03    initial revision.

**********************************************************************/


#include "slap_lco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapLcoGetObjectStubByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to retrieve an object stub.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name to be matched.

    return:     object stub.

**********************************************************************/

ANSC_HANDLE
SlapLcoGetObjectStubByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject         = (PSLAP_LOAM_CLIENT_OBJECT  )hThisObject;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;
    PSLAP_LOA_OBJECT_STUB           pObjectStub       = (PSLAP_LOA_OBJECT_STUB     )NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddr        = (PANSC_LPC_PARTY_ADDR      )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY        )NULL;
    ULONG                           ulHashIndex       = AnscHashString(obj_name, AnscSizeOfString(obj_name), SLAP_LCO_OSO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->OsoTableLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->OsoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pObjectStub = ACCESS_SLAP_LOA_OBJECT_STUB(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
        pPartyAddr  = (PANSC_LPC_PARTY_ADDR)pObjectStub->hLpcPartyAddr;

        if ( strcasecmp(pObjectStub->ObjName,obj_name) == 0 )
        {
            /*
             * If there's no LOA objects associated with this binding, we need to check whether the
             * remote LPC party is still connected. If it's not, we MUST remove this binding right
             * away from the internal cache table. Otherwise, a broken connection can block further
             * connections to be established ever again.
             */
            if ( (pObjectStub->RefCount == 0) &&
                 !pAnscLpcConnector->ImcIsPartyConnected
                    (
                        (ANSC_HANDLE)pAnscLpcConnector,
                        pObjectStub->LpcPartyName,
                        TRUE
                    ) )
            {
                AnscSListPopEntryByLink(&pMyObject->OsoTable[ulHashIndex], &pObjectStub->Linkage);

                if ( pPartyAddr )
                {
                    pPartyAddr->RefCount--;
                }

                SlapLockLoaObjectStub  (pObjectStub);
                SlapUnlockLoaObjectStub(pObjectStub);
                SlapFreeLoaObjectStub  (pObjectStub);

                break;
            }
            else
            {
                SlapLockLoaObjectStub(pObjectStub);

                AnscReleaseLock(&pMyObject->OsoTableLock);

                return  (ANSC_HANDLE)pObjectStub;
            }
        }
    }

    AnscReleaseLock(&pMyObject->OsoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapLcoGetObjectStubByHandle
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 obj_handle
            );

    description:

        This function is called to retrieve an object stub.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 obj_handle
                Specifies the obj_handle to be matched.

    return:     object stub.

**********************************************************************/

ANSC_HANDLE
SlapLcoGetObjectStubByHandle
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 obj_handle
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject         = (PSLAP_LOAM_CLIENT_OBJECT  )hThisObject;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;
    PSLAP_LOA_OBJECT_STUB           pObjectStub       = (PSLAP_LOA_OBJECT_STUB     )NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddr        = (PANSC_LPC_PARTY_ADDR      )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY        )NULL;
    ULONG                           i                 = 0;

    AnscAcquireLock(&pMyObject->OsoTableLock);

    for ( i = 0; i < SLAP_LCO_OSO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->OsoTable[i]);

        while ( pSLinkEntry )
        {
            pObjectStub = ACCESS_SLAP_LOA_OBJECT_STUB(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
            pPartyAddr  = (PANSC_LPC_PARTY_ADDR)pObjectStub->hLpcPartyAddr;

            if ( pObjectStub->hLoaSlapObject == obj_handle )
            {
                /*
                 * If there's no LOA objects associated with this binding, we need to check whether the
                 * remote LPC party is still connected. If it's not, we MUST remove this binding right
                 * away from the internal cache table. Otherwise, a broken connection can block further
                 * connections to be established ever again.
                 */
                if ( (pObjectStub->RefCount == 0) &&
                     !pAnscLpcConnector->ImcIsPartyConnected
                        (
                            (ANSC_HANDLE)pAnscLpcConnector,
                            pObjectStub->LpcPartyName,
                            TRUE
                        ) )
                {
                    AnscSListPopEntryByLink(&pMyObject->OsoTable[i], &pObjectStub->Linkage);

                    if ( pPartyAddr )
                    {
                        pPartyAddr->RefCount--;
                    }

                    SlapLockLoaObjectStub  (pObjectStub);
                    SlapUnlockLoaObjectStub(pObjectStub);
                    SlapFreeLoaObjectStub  (pObjectStub);

                    break;
                }
                else
                {
                    SlapLockLoaObjectStub(pObjectStub);

                    AnscReleaseLock(&pMyObject->OsoTableLock);

                    return  (ANSC_HANDLE)pObjectStub;
                }
            }
        }
    }

    AnscReleaseLock(&pMyObject->OsoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoAddObjectStub
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 obj_handle,
                char*                       obj_name,
                char*                       lpc_party_name,
                ANSC_HANDLE                 lpc_party_addr
            );

    description:

        This function is called to add an object stub.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 obj_handle
                Specifies the obj_handle to be added.

                char*                       obj_name
                Specifies the obj_name to be added.

                char*                       lpc_party_name
                Specifies the lpc_party_name to be added.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the lpc_party_addr to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoAddObjectStub
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 obj_handle,
        char*                       obj_name,
        char*                       lpc_party_name,
        ANSC_HANDLE                 lpc_party_addr
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject    = (PSLAP_LOAM_CLIENT_OBJECT)hThisObject;
    PSLAP_LOA_OBJECT_STUB           pObjectStub  = (PSLAP_LOA_OBJECT_STUB   )NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR    )lpc_party_addr;
    ULONG                           ulHashIndex  = AnscHashString(obj_name, AnscSizeOfString(obj_name), SLAP_LCO_OSO_TABLE_SIZE);

    pObjectStub = (PSLAP_LOA_OBJECT_STUB)AnscAllocateMemory(sizeof(SLAP_LOA_OBJECT_STUB));

    if ( !pObjectStub )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pObjectStub->HashIndex      = ulHashIndex;
        pObjectStub->hLoaSlapObject = obj_handle;
        pObjectStub->ObjName        = AnscCloneString(obj_name      );
        pObjectStub->LpcPartyName   = AnscCloneString(lpc_party_name);
        pObjectStub->hLpcPartyAddr  = lpc_party_addr;

        pObjectStub->LastAccessAt   = AnscGetTickInSeconds();
        pObjectStub->LastImcpError  = 0;
        pObjectStub->RefCount       = 1;

        if ( pPartyAddr )
        {
            pPartyAddr->RefCount++;
        }

        AnscInitializeLock(&pObjectStub->AccessLock);
    }

    AnscAcquireLock   (&pMyObject->OsoTableLock);
    AnscSListPushEntry(&pMyObject->OsoTable[ulHashIndex], &pObjectStub->Linkage);
    AnscReleaseLock   (&pMyObject->OsoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoDelObjectStub
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 obj_handle
            );

    description:

        This function is called to delete an object stub.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 obj_handle
                Specifies the obj_handle to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoDelObjectStub
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 obj_handle
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject    = (PSLAP_LOAM_CLIENT_OBJECT)hThisObject;
    PSLAP_LOA_OBJECT_STUB           pObjectStub  = (PSLAP_LOA_OBJECT_STUB   )NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY      )NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->OsoTableLock);

    for ( i = 0; i < SLAP_LCO_OSO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->OsoTable[i]);

        while ( pSLinkEntry )
        {
            pObjectStub = ACCESS_SLAP_LOA_OBJECT_STUB(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
            pPartyAddr  = (PANSC_LPC_PARTY_ADDR)pObjectStub->hLpcPartyAddr;

            if ( pObjectStub->hLoaSlapObject == obj_handle )
            {
                AnscSListPopEntryByLink(&pMyObject->OsoTable[i], &pObjectStub->Linkage);

                if ( pPartyAddr )
                {
                    pMyObject->LoaRelObjectAccess2
                            (
                                (ANSC_HANDLE)pMyObject,
                                pObjectStub->hLpcPartyAddr,
                                pObjectStub->hLoaSlapObject
                            );

                    pPartyAddr->RefCount--;
                }

                SlapLockLoaObjectStub  (pObjectStub);
                SlapUnlockLoaObjectStub(pObjectStub);
                SlapFreeLoaObjectStub  (pObjectStub);

                break;
            }
        }
    }

    AnscReleaseLock(&pMyObject->OsoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoDelAllObjectStubs
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all object stubs.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoDelAllObjectStubs
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject    = (PSLAP_LOAM_CLIENT_OBJECT)hThisObject;
    PSLAP_LOA_OBJECT_STUB           pObjectStub  = (PSLAP_LOA_OBJECT_STUB   )NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY      )NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->OsoTableLock);

    for ( i = 0; i < SLAP_LCO_OSO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListPopEntry(&pMyObject->OsoTable[i]);

        while ( pSLinkEntry )
        {
            pObjectStub = ACCESS_SLAP_LOA_OBJECT_STUB(pSLinkEntry);
            pSLinkEntry = AnscSListPopEntry(&pMyObject->OsoTable[i]);
            pPartyAddr  = (PANSC_LPC_PARTY_ADDR)pObjectStub->hLpcPartyAddr;

            if ( pPartyAddr )
            {
                pMyObject->LoaRelObjectAccess2
                        (
                            (ANSC_HANDLE)pMyObject,
                            pObjectStub->hLpcPartyAddr,
                            pObjectStub->hLoaSlapObject
                        );

                pPartyAddr->RefCount--;
            }

            SlapLockLoaObjectStub  (pObjectStub);
            SlapUnlockLoaObjectStub(pObjectStub);
            SlapFreeLoaObjectStub  (pObjectStub);
        }
    }

    AnscReleaseLock(&pMyObject->OsoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
