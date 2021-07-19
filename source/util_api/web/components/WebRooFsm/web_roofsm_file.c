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

    module:	web_roofsm_file.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced file-mapping functions
        of the Web Roo Fsm Object.

        *   WebRooFsmMapDiskFile
        *   WebRooFsmGetMimeType

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/19/02    initial revision.

**********************************************************************/


#include "web_roofsm_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebRooFsmMapDiskFile
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTokenChain
            );

    description:

        This function is called to map a token chain to a disk file.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTokenChain
                Specifies the token chain to be mapped.

    return:     status of operation.

**********************************************************************/

ANSC_HANDLE
WebRooFsmMapDiskFile
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    )
{
    PWEB_ROO_FSM_OBJECT             pMyObject       = (PWEB_ROO_FSM_OBJECT  )hThisObject;
    PWEB_ROO_FSM_PROPERTY           pProperty       = (PWEB_ROO_FSM_PROPERTY)&pMyObject->Property;
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN    )hTokenChain;
    PANSC_STRING_TOKEN              pPathToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ANSC_HANDLE                     hMappedFile     = (ANSC_HANDLE)NULL;
    ULONG                           ulFileNameLen   = 0;
    char*                           pTempChar       = NULL;
    char                            sFileName[ANSC_MAX_FILE_NAME_SIZE];
    errno_t                         rc              = -1;

    pTempChar = &sFileName[0];

    AnscZeroMemory(sFileName, ANSC_MAX_FILE_NAME_SIZE);
    rc = strcpy_s(sFileName, sizeof(sFileName), pProperty->RootPath);
    ERR_CHK(rc);

    ulFileNameLen = AnscSizeOfString(sFileName);
    pTempChar    += ulFileNameLen;

    if ( sFileName[ulFileNameLen - 1] == ANSC_FILE_PATH_SEP )
    {
        sFileName[ulFileNameLen - 1] = 0;

        ulFileNameLen--;
        pTempChar    --;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pPathTokenChain->TokensQueue);

    while ( pSLinkEntry )
    {
        pPathToken  = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        *pTempChar++ = ANSC_FILE_PATH_SEP;

        rc = strcpy_s(pTempChar, ANSC_MAX_FILE_NAME_SIZE, pPathToken->Name);
        ERR_CHK(rc);

        pTempChar += AnscSizeOfString(pPathToken->Name);
    }

    hMappedFile =
        AnscOpenFile
            (
                sFileName,
                ANSC_FILE_MODE_READ,
                ANSC_FILE_TYPE_RDWR
            );

    return  hMappedFile;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooFsmGetMimeType
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFile,
                PULONG                      pulMediaType,
                PULONG                      pulSubType
            );

    description:

        This function is called to retrieve the corresponding mime
        media and sub type for a mapped disk file.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFile
                Specifies the mapped disk file to be examined.

                PULONG                      pulMediaType
                Specifies the mime media type to be filled out.

                PULONG                      pulSubType
                Specifies the mime sub type to be filled out.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooFsmGetMimeType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFile,
        PULONG                      pulMediaType,
        PULONG                      pulSubType
    )
{
    PWEB_ROO_FSM_OBJECT             pMyObject       = (PWEB_ROO_FSM_OBJECT          )hThisObject;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco        = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    char*                           pFileExtension  = AnscGetFileExt(hFile);

    *pulMediaType = IANA_MEDIA_TYPE_CODE_RESERVED;
    *pulSubType   = 0;

    if ( !pFileExtension )
    {
        return  ANSC_STATUS_UNDETERMINED;
    }
    else
    {
        *pulMediaType = pHttpHco->GetExtMediaType((ANSC_HANDLE)pHttpHco, pFileExtension);
        *pulSubType   = pHttpHco->GetExtSubType  ((ANSC_HANDLE)pHttpHco, pFileExtension);
    }

    if ( (*pulMediaType == 0) || (*pulSubType == 0) )
    {
        return  ANSC_STATUS_UNDETERMINED;
    }

    return  ANSC_STATUS_SUCCESS;
}
