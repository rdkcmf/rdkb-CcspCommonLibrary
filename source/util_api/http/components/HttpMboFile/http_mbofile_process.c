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

    module:	http_mbofile_process.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Mbo File Object.

        *   HttpMboFileOutput

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

        ANSC_STATUS
        HttpMboFileOutput
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSerializeContext
            );

    description:

        This function is called to serialize the current HTTP message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSerializeContext
                Specifies the serialize context for output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpMboFileOutput
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerializeContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PHTTP_MBO_FILE_OBJECT           pMyObject         = (PHTTP_MBO_FILE_OBJECT)hThisObject;
    PHTTP_BCC_INTERFACE             pBccIf            = (PHTTP_BCC_INTERFACE  )pMyObject->hBccIf;
    ANSC_HANDLE                     hFile             = (ANSC_HANDLE          )NULL;
    ULONG                           ulCopySize        = 0;
    PVOID                           pSerializedBuffer = NULL;
    ULONG                           ulSerializedSize  = 0;
    ULONG                           ulRoundNumber     = 0;
    ULONG                           i                 = 0;

    if ( !pMyObject->FileName )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        ulCopySize = pMyObject->GetBodySize((ANSC_HANDLE)pMyObject);
    }

    if ( ulCopySize == 0 )
    {
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
        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        AnscSeekFile(hFile, (ULONG)pMyObject->RangeFrom, ANSC_FILE_SEEK_SET);
    }

    pSerializedBuffer = AnscAllocateMemory(HTTP_MBOFILE_DEF_WRITE_UNIT);

    if ( !pSerializedBuffer )
    {
        AnscCloseFile(hFile);

        return  ANSC_STATUS_RESOURCES;
    }

    if ( ulCopySize <= HTTP_MBOFILE_DEF_WRITE_UNIT )
    {
        ulSerializedSize = HTTP_MBOFILE_DEF_WRITE_UNIT;
        returnStatus     =
            AnscReadFile
                (
                    hFile,
                    pSerializedBuffer,
                    &ulSerializedSize
                );

        returnStatus =
            pBccIf->Serialize
                (
                    pBccIf->hOwnerContext,
                    pSerializedBuffer,
                    ulSerializedSize,
                    hSerializeContext
                );
    }
    else
    {
        if ( (ulCopySize % HTTP_MBOFILE_DEF_WRITE_UNIT) == 0 )
        {
            ulRoundNumber = ulCopySize / HTTP_MBOFILE_DEF_WRITE_UNIT;
        }
        else
        {
            ulRoundNumber = ulCopySize / HTTP_MBOFILE_DEF_WRITE_UNIT + 1;
        }

        for ( i = 0; i < ulRoundNumber; i++ )
        {
            ulSerializedSize = HTTP_MBOFILE_DEF_WRITE_UNIT;
            returnStatus     =
                AnscReadFile
                    (
                        hFile,
                        pSerializedBuffer,
                        &ulSerializedSize
                    );

            returnStatus =
                pBccIf->Serialize
                    (
                        pBccIf->hOwnerContext,
                        pSerializedBuffer,
                        ulSerializedSize,
                        hSerializeContext
                    );
        }
    }

    AnscCloseFile(hFile);

    if ( pSerializedBuffer )
    {
        AnscFreeMemory(pSerializedBuffer);
    }

    return  returnStatus;
}
