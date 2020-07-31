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

    module:	http_mbo_access.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Message Body Object.

        *   HttpMboGetBodySize
        *   HttpMboGetBodySize2
        *   HttpMboGetTransferSize
        *   HttpMboGetEntitySize
        *   HttpMboGetArrivedSize
        *   HttpMboAppendBodyBdo
        *   HttpMboAppendBodyFile
        *   HttpMboClearBody
        *   HttpMboCopyBodyFrom
        *   HttpMboCopyBodyInto
        *   HttpMboSaveAsBody
        *   HttpMboSaveBodyAsFile

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/01/02    initial revision.

**********************************************************************/


#include "http_mbo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboGetBodySize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
HttpMboGetBodySize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    
    return  pMyObject->GetEntitySize((ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboGetBodySize2
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
HttpMboGetBodySize2
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulBodySize   = 0;

    if ( pMyObject->Mode == HTTP_MBO_MODE_EMPTY )
    {
        ulBodySize = pMyObject->TransferSize;
    }
    else if ( pMyObject->Mode == HTTP_MBO_MODE_RELAY )
    {
        ulBodySize = pMyObject->TransferSize;
    }
    else
    {
        AnscAcquireLock(&pMyObject->BdoQueueLock);

        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->BdoQueue);

        while ( pSLinkEntry )
        {
            pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            ulBodySize += AnscBdoGetBlockSize(pBodyBdo);
        }

        AnscReleaseLock(&pMyObject->BdoQueueLock);
    }

    return  ulBodySize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboGetTransferSize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
HttpMboGetTransferSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulBodySize   = 0;

    if ( pMyObject->Mode == HTTP_MBO_MODE_EMPTY )
    {
        ulBodySize = pMyObject->TransferSize;
    }
    else if ( pMyObject->Mode == HTTP_MBO_MODE_RELAY )
    {
        ulBodySize = pMyObject->TransferSize;
    }
    else if ( pMyObject->Mode == HTTP_MBO_MODE_STORE_EXTERNAL )
    {
        ulBodySize = pMyObject->TransferSize;
    }
    else
    {
        AnscAcquireLock(&pMyObject->BdoQueueLock);

        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->BdoQueue);

        while ( pSLinkEntry )
        {
            pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            ulBodySize += AnscBdoGetBlockSize(pBodyBdo);
        }

        AnscReleaseLock(&pMyObject->BdoQueueLock);
    }

    return  ulBodySize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboGetEntitySize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
HttpMboGetEntitySize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    return  pMyObject->GetTransferSize((ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboGetArrivedSize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
HttpMboGetArrivedSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    
    return  pMyObject->GetEntitySize((ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboAppendBodyBdo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBdo
            );

    description:

        This function is called to append a body-part buffer descriptor
        to the current message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBdo
                Specifies the body buffer descriptor to be appended.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboAppendBodyBdo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = (PANSC_BUFFER_DESCRIPTOR  )hBdo;

    pMyObject->TransferSize += AnscBdoGetBlockSize(pBodyBdo);
    pMyObject->EntitySize   += AnscBdoGetBlockSize(pBodyBdo);
    pMyObject->ArrivedSize  += AnscBdoGetBlockSize(pBodyBdo);

    if ( pMyObject->Mode == HTTP_MBO_MODE_STORE )
    {
        AnscAcquireLock   (&pMyObject->BdoQueueLock);
        AnscQueuePushEntry(&pMyObject->BdoQueue, &pBodyBdo->Linkage);
        AnscReleaseLock   (&pMyObject->BdoQueueLock);
    }
    else if ( pMyObject->Mode == HTTP_MBO_MODE_EMPTY )
    {
        AnscFreeBdo((ANSC_HANDLE)pBodyBdo);
    }
    else if ( pMyObject->Mode == HTTP_MBO_MODE_STORE_EXTERNAL )
    {
        returnStatus = pMyObject->ProcessMPData((ANSC_HANDLE)pMyObject, hBdo);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboAppendBodyFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pFileName,
                int                         iRangeFrom,
                int                         iRangeTo
            );

    description:

        This function is called to append the complete/partial file
        content to the current message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pFileName
                Specifies the name of the file whose content is to be
                appended.

                int                         iRangeFrom
                Specifies the starting offset of the file content.

                int                         iRangeTo
                Specifies the ending offset of the file content.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboAppendBodyFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFileName,
        int                         iRangeFrom,
        int                         iRangeTo
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pFileName);
    UNREFERENCED_PARAMETER(iRangeFrom);
    UNREFERENCED_PARAMETER(iRangeTo);

    return  ANSC_STATUS_NOT_SUPPORTED;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboClearBody
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clean the message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboClearBody
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->BdoQueueLock);

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->BdoQueue);

    while ( pSLinkEntry )
    {
        pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->BdoQueue);

        AnscFreeBdo((ANSC_HANDLE)pBodyBdo);
    }

    AnscReleaseLock(&pMyObject->BdoQueueLock);

    pMyObject->TransferSize = 0;
    pMyObject->EntitySize   = 0;
    pMyObject->ArrivedSize  = 0;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboCopyBodyFrom
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                PULONG                      pulSize
            );

    description:

        This function is called to copy the message body from the
        internal buffer into an external buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the external buffer where the body to be
                copied to.

                PULONG                      pulSize
                Specifies the size of the external buffer as the
                inbound parameter and size of copied body data as the
                outbound parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboCopyBodyFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulBodySize   = 0;
    ULONG                           ulCopySize   = 0;
    ULONG                           ulLeftSize   = *pulSize;

    AnscAcquireLock(&pMyObject->BdoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->BdoQueue);

    while ( pSLinkEntry )
    {
        pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( ulLeftSize >= AnscBdoGetBlockSize(pBodyBdo) )
        {
            ulCopySize = AnscBdoGetBlockSize(pBodyBdo);
        }
        else
        {
            ulCopySize = ulLeftSize;
        }

        if ( ulCopySize > 0 )
        {
            AnscCopyMemory
                (
                    (PVOID)((ULONG)buffer + ulBodySize),
                    AnscBdoGetBlock(pBodyBdo),
                    ulCopySize
                );
        }

        ulBodySize += ulCopySize;
        ulLeftSize -= ulCopySize;

        if ( ulLeftSize == 0 )
        {
            break;
        }
    }

    AnscReleaseLock(&pMyObject->BdoQueueLock);

    *pulSize = ulBodySize;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboCopyBodyInto
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                PULONG                      pulSize
            );

    description:

        This function is called to copy the message body from the
        external buffer into an internal buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the external buffer where the body to be
                copied from.

                PULONG                      pulSize
                Specifies the size of the external buffer as the
                inbound parameter and size of copied body data as the
                outbound parameter.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboCopyBodyInto
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulBodySize   = 0;
    ULONG                           ulCopySize   = 0;
    ULONG                           ulLeftSize   = *pulSize;

    AnscAcquireLock(&pMyObject->BdoQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->BdoQueue);

    while ( pSLinkEntry )
    {
        pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( ulLeftSize >= AnscBdoGetBlockSize(pBodyBdo) )
        {
            ulCopySize = AnscBdoGetBlockSize(pBodyBdo);
        }
        else
        {
            ulCopySize = ulLeftSize;
        }

        if ( ulCopySize > 0 )
        {
            AnscCopyMemory
                (
                    AnscBdoGetBlock(pBodyBdo),
                    (PVOID)((ULONG)buffer + ulBodySize),
                    ulCopySize
                );
        }

        ulBodySize += ulCopySize;
        ulLeftSize -= ulCopySize;

        if ( ulLeftSize == 0 )
        {
            break;
        }
    }

    AnscReleaseLock(&pMyObject->BdoQueueLock);

    *pulSize = ulBodySize;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboSaveAsBody
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to save the external buffer block as
        the HTTP message body.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the external buffer holding the message body.

                ULONG                       ulSize
                Specifies the size of the external buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboSaveAsBody
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = NULL;

    pMyObject->ClearBody((ANSC_HANDLE)pMyObject);

    pBodyBdo =
        (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
            (
                0,
                0,
                0
            );

    if ( !pBodyBdo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pBodyBdo->bOwner     = TRUE;
        pBodyBdo->Buffer     = buffer;
        pBodyBdo->BufferSize = ulSize;
        pBodyBdo->Offset     = 0;
        pBodyBdo->BlockSize  = ulSize;
    }

    returnStatus =
        pMyObject->AppendBodyBdo
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBodyBdo
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboSaveBodyAsFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       file_name,
                BOOL                        bFlushBody
            );

    description:

        This function is called to save message body into a file.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       file_name
                Specifies the file name where the body is to be saved.

                BOOL                        bFlushBody
                Specifies whether the body should be flushed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboSaveBodyAsFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_name,
        BOOL                        bFlushBody
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;
    ANSC_HANDLE                     hTargetFile  = (ANSC_HANDLE              )NULL;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulBlockSize  = 0;

    hTargetFile =
        AnscOpenFile
            (
                file_name,
                ANSC_FILE_MODE_RDWR | ANSC_FILE_MODE_CREATE | ANSC_FILE_MODE_TRUNC | ANSC_FILE_MODE_APPEND,
                ANSC_FILE_TYPE_RDWR | ANSC_FILE_TYPE_RDGO
            );

    if ( !hTargetFile )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT1;
    }

    AnscAcquireLock(&pMyObject->BdoQueueLock);

    if ( bFlushBody )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->BdoQueue);

        while ( pSLinkEntry )
        {
            pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
            pSLinkEntry = AnscQueuePopEntry(&pMyObject->BdoQueue);

            ulBlockSize  = AnscBdoGetBlockSize(pBodyBdo);
            returnStatus =
                AnscWriteFile
                    (
                        hTargetFile,
                        AnscBdoGetBlock(pBodyBdo),
                        &ulBlockSize
                    );

            AnscFreeBdo(pBodyBdo);
        }

        pMyObject->TransferSize = 0;
        pMyObject->EntitySize   = 0;
        pMyObject->ArrivedSize  = 0;
    }
    else
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->BdoQueue);

        while ( pSLinkEntry )
        {
            pBodyBdo    = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            ulBlockSize  = AnscBdoGetBlockSize(pBodyBdo);
            returnStatus =
                AnscWriteFile
                    (
                        hTargetFile,
                        AnscBdoGetBlock(pBodyBdo),
                        &ulBlockSize
                    );
        }
    }

    AnscReleaseLock(&pMyObject->BdoQueueLock);

    goto  EXIT2;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( hTargetFile )
    {
        AnscCloseFile(hTargetFile);
    }

EXIT1:

    return  returnStatus;
}
