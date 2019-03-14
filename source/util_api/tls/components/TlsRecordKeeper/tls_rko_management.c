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

    module:	tls_rko_management.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the TLS Record Keeper Object.

        *   TlsRkoGetRecordClient
        *   TlsRkoAddRecordClient
        *   TlsRkoPopRecordClient
        *   TlsRkoDelRecordClient
        *   TlsRkoDelAllRecordClients

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/27/03    initial revision.

**********************************************************************/


#include "tls_rko_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsRkoGetRecordClient
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       record_type
            );

    description:

        This function is called to retrieve a record client by matching
        the record type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       record_type
                Specifies the record type to be matched.

    return:     record client.

**********************************************************************/

ANSC_HANDLE
TlsRkoGetRecordClient
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       record_type
    )
{
    PTLS_RECORD_KEEPER_OBJECT       pMyObject        = (PTLS_RECORD_KEEPER_OBJECT  )hThisObject;
    PTLS_RECORD_CLIENT_OBJECT       pTlsRecordClient = (PTLS_RECORD_CLIENT_OBJECT  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY         )NULL;
    ULONG                           ulHashIndex      = AnscHashUlong(record_type, TLS_RKO_RCO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->RcoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->RcoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pTlsRecordClient = ACCESS_TLS_RECORD_CLIENT_OBJECT(pSLinkEntry);
        pSLinkEntry      = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pTlsRecordClient->RecordType == record_type )
        {
            AnscReleaseLock(&pMyObject->RcoTableLock);

            return  (ANSC_HANDLE)pTlsRecordClient;
        }
    }

    AnscReleaseLock(&pMyObject->RcoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsRkoAddRecordClient
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRecordClient
            );

    description:

        This function is called to add a new record client.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRecordClient
                Specifies the record client to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsRkoAddRecordClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecordClient
    )
{
    PTLS_RECORD_KEEPER_OBJECT       pMyObject        = (PTLS_RECORD_KEEPER_OBJECT  )hThisObject;
    PTLS_RECORD_CLIENT_OBJECT       pTlsRecordClient = (PTLS_RECORD_CLIENT_OBJECT  )hRecordClient;
    ULONG                           ulHashIndex      = AnscHashUlong(pTlsRecordClient->RecordType, TLS_RKO_RCO_TABLE_SIZE);

    pTlsRecordClient->hTlsRecordKeeper = (ANSC_HANDLE)pMyObject;

    AnscAcquireLock   (&pMyObject->RcoTableLock);
    AnscQueuePushEntry(&pMyObject->RcoTable[ulHashIndex], &pTlsRecordClient->Linkage);
    AnscReleaseLock   (&pMyObject->RcoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsRkoPopRecordClient
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       record_type
            );

    description:

        This function is called to unlink a record client by matching
        the record type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       record_type
                Specifies the record type to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsRkoPopRecordClient
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       record_type
    )
{
    PTLS_RECORD_KEEPER_OBJECT       pMyObject        = (PTLS_RECORD_KEEPER_OBJECT  )hThisObject;
    PTLS_RECORD_CLIENT_OBJECT       pTlsRecordClient = (PTLS_RECORD_CLIENT_OBJECT  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY         )NULL;
    ULONG                           ulHashIndex      = AnscHashUlong(record_type, TLS_RKO_RCO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->RcoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->RcoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pTlsRecordClient = ACCESS_TLS_RECORD_CLIENT_OBJECT(pSLinkEntry);
        pSLinkEntry      = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pTlsRecordClient->RecordType == record_type )
        {
            AnscQueuePopEntryByLink(&pMyObject->RcoTable[ulHashIndex], &pTlsRecordClient->Linkage);

            break;
        }
    }

    AnscReleaseLock(&pMyObject->RcoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsRkoDelRecordClient
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       record_type
            );

    description:

        This function is called to delete a record client by matching
        the record type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       record_type
                Specifies the record type to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsRkoDelRecordClient
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       record_type
    )
{
    PTLS_RECORD_KEEPER_OBJECT       pMyObject        = (PTLS_RECORD_KEEPER_OBJECT  )hThisObject;
    PTLS_RECORD_CLIENT_OBJECT       pTlsRecordClient = (PTLS_RECORD_CLIENT_OBJECT  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY         )NULL;
    ULONG                           ulHashIndex      = AnscHashUlong(record_type, TLS_RKO_RCO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->RcoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->RcoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pTlsRecordClient = ACCESS_TLS_RECORD_CLIENT_OBJECT(pSLinkEntry);
        pSLinkEntry      = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pTlsRecordClient->RecordType == record_type )
        {
            AnscQueuePopEntryByLink(&pMyObject->RcoTable[ulHashIndex], &pTlsRecordClient->Linkage);

            pTlsRecordClient->Remove((ANSC_HANDLE)pTlsRecordClient);

            break;
        }
    }

    AnscReleaseLock(&pMyObject->RcoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsRkoDelAllRecordClients
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all record clients.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsRkoDelAllRecordClients
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_RECORD_KEEPER_OBJECT       pMyObject        = (PTLS_RECORD_KEEPER_OBJECT  )hThisObject;
    PTLS_RECORD_CLIENT_OBJECT       pTlsRecordClient = (PTLS_RECORD_CLIENT_OBJECT  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY         )NULL;
    ULONG                           i                = 0;

    AnscAcquireLock(&pMyObject->RcoTableLock);

    for ( i = 0; i < TLS_RKO_RCO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->RcoTable[i]);

        while ( pSLinkEntry )
        {
            pTlsRecordClient = ACCESS_TLS_RECORD_CLIENT_OBJECT(pSLinkEntry);
            pSLinkEntry      = AnscQueuePopEntry(&pMyObject->RcoTable[i]);

            pTlsRecordClient->Remove((ANSC_HANDLE)pTlsRecordClient);
        }
    }

    AnscReleaseLock(&pMyObject->RcoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
