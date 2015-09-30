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

    module:	http_mbofile_access.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2005
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Mbo File Object.

        *   HttpMboFileGetTransferSize
        *   HttpMboFileAppendBodyBdo
        *   HttpMboFileAppendBodyFile
        *   HttpMboFileClearBody
        *   HttpMboFileCopyBodyFrom
        *   HttpMboFileCopyBodyInto
        *   HttpMboFileSaveAsBody
        *   HttpMboFileSaveBodyAsFile

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/18/05    initial revision.

**********************************************************************/


#include "http_mbofile_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpMboFileGetTransferSize
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
HttpMboFileGetTransferSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_FILE_OBJECT           pMyObject    = (PHTTP_MBO_FILE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE  )pMyObject->hHfpIf;
    ANSC_HANDLE                     hFile        = (ANSC_HANDLE          )NULL;
    ULONG                           ulFileSize   = 0;
    ULONG                           ulBodySize   = 0;

    if ( !pMyObject->FileName )
    {
        return  0;
    }
    else
    {
        hFile =
            AnscOpenFile
                (
                    pMyObject->FileName,
                    ANSC_FILE_MODE_READ,
                    ANSC_FILE_TYPE_READ
                );
    }

    if ( !hFile )
    {
        return  0;
    }
    else
    {
        ulFileSize = AnscGetFileSize(hFile);
    }

    AnscCloseFile(hFile);

    if ( (ULONG)pMyObject->RangeFrom >= ulFileSize )
    {
        return  0;
    }
    else if ( pMyObject->RangeTo == -1 )
    {
        return  ulFileSize - (ULONG)pMyObject->RangeFrom;
    }
    else if ( (ULONG)pMyObject->RangeTo <= ulFileSize )
    {
        return  (ULONG)pMyObject->RangeTo - (ULONG)pMyObject->RangeFrom + 1;
    }
    else
    {
        return  ulFileSize - (ULONG)pMyObject->RangeFrom;
    }

    return  ulBodySize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboFileAppendBodyBdo
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
HttpMboFileAppendBodyBdo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_FILE_OBJECT           pMyObject    = (PHTTP_MBO_FILE_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE    )pMyObject->hHfpIf;
    PANSC_BUFFER_DESCRIPTOR         pBodyBdo     = (PANSC_BUFFER_DESCRIPTOR)hBdo;

    AnscFreeBdo(pBodyBdo);

    return  ANSC_STATUS_NOT_SUPPORTED;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboFileAppendBodyFile
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
HttpMboFileAppendBodyFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFileName,
        int                         iRangeFrom,
        int                         iRangeTo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_FILE_OBJECT           pMyObject    = (PHTTP_MBO_FILE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE  )pMyObject->hHfpIf;

    pMyObject->ClearBody((ANSC_HANDLE)pMyObject);

    if ( pFileName )
    {
        pMyObject->FileName = AnscCloneString(pFileName);
    }

    pMyObject->RangeFrom = iRangeFrom;
    pMyObject->RangeTo   = iRangeTo;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboFileClearBody
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
HttpMboFileClearBody
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_FILE_OBJECT           pMyObject    = (PHTTP_MBO_FILE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE  )pMyObject->hHfpIf;

    if ( pMyObject->FileName )
    {
        AnscFreeMemory(pMyObject->FileName);

        pMyObject->FileName = NULL;
    }

    pMyObject->RangeFrom = 0;
    pMyObject->RangeTo   = -1;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboFileCopyBodyFrom
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
HttpMboFileCopyBodyFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_FILE_OBJECT           pMyObject    = (PHTTP_MBO_FILE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE  )pMyObject->hHfpIf;
    ANSC_HANDLE                     hFile        = (ANSC_HANDLE          )NULL;
    ULONG                           ulFileSize   = 0;
    ULONG                           ulCopySize   = 0;

    if ( !pMyObject->FileName )
    {
        *pulSize = 0;

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        ulCopySize = pMyObject->GetBodySize((ANSC_HANDLE)pMyObject);
    }

    if ( ulCopySize == 0 )
    {
        *pulSize = 0;

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        hFile =
            AnscOpenFile
                (
                    pMyObject->FileName,
                    ANSC_FILE_MODE_READ,
                    ANSC_FILE_TYPE_READ
                );
    }

    if ( !hFile )
    {
        *pulSize = 0;

        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        AnscSeekFile(hFile, (ULONG)pMyObject->RangeFrom, ANSC_FILE_SEEK_SET);

        *pulSize     = ulCopySize;
        returnStatus =
            AnscReadFile
                (
                    hFile,
                    buffer,
                    pulSize
                );
    }

    AnscCloseFile(hFile);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboFileCopyBodyInto
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
HttpMboFileCopyBodyInto
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_FILE_OBJECT           pMyObject    = (PHTTP_MBO_FILE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE  )pMyObject->hHfpIf;

    *pulSize = 0;

    return  ANSC_STATUS_NOT_SUPPORTED;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboFileSaveAsBody
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
HttpMboFileSaveAsBody
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_FILE_OBJECT           pMyObject    = (PHTTP_MBO_FILE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE  )pMyObject->hHfpIf;

    return  ANSC_STATUS_NOT_SUPPORTED;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboFileSaveBodyAsFile
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
HttpMboFileSaveBodyAsFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_name,
        BOOL                        bFlushBody
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_FILE_OBJECT           pMyObject    = (PHTTP_MBO_FILE_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE  )pMyObject->hHfpIf;

    return  ANSC_STATUS_NOT_SUPPORTED;
}
