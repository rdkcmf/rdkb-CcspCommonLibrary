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

    module:	sys_cpo_serialize.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the serialization functions
        of the Sys Ctlv Processor Object.

        *   SysCpoQueryFolderSize
        *   SysCpoSerializeFolder

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        01/13/06    initial revision.

**********************************************************************/


#include "sys_cpo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysCpoQueryFolderSize
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hParentFolder,
                char*                       pFolderName
            );

    description:

        This function is to caculate the CTLV size of the System Repository
        folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hParentFolder
                Specifies the parent folder on which the CTLV tuple
                should be populated.

                char*                       pFolderName
                Specifies the name of the folder name.

    return:     CTLV size.

**********************************************************************/

ULONG
SysCpoQueryFolderSize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParentFolder,
        char*                       pFolderName
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject               = (PSYS_CTLV_PROCESSOR_OBJECT)hThisObject;
    PSYS_IRA_INTERFACE              pSysIraIf               = (PSYS_IRA_INTERFACE        )pMyObject->hSysIraIf;

    ANSC_HANDLE                     hFolder                 = (ANSC_HANDLE               )NULL;

    ULONG                           ulCount                 = 0;
    ULONG                           ulIndex                 = 0;

    char*                           pName                   = NULL;
    ULONG                           ulNameSize              = 0;
    PUCHAR                          pData                   = NULL;
    ULONG                           ulMaxDataSize           = SYS_MAX_ATTRIB_DATA_SIZE;
    ULONG                           ulDataSize              = 0;
    ULONG                           ulRecordType            = 0;

    ULONG                           ulCtlvSize              = 0;
    ULONG                           ulCtlvRecordSize        = 0;
    ULONG                           ulCtlvFolderSize        = 0;

    AnscTrace("SysCpoQueryFolderSize -- %s...\n", pFolderName);

    hFolder = 
        pSysIraIf->OpenFolder
            (
                pSysIraIf->hOwnerContext,
                hParentFolder,
                pFolderName
            );

    if ( !hFolder )
    {
        AnscTrace("SysCpoQueryFolderSize -- failed to open folder %s...\n", pFolderName);

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pName   = AnscAllocateMemory(SYS_MAX_RECORD_NAME_SIZE + 1);
        pData   = AnscAllocateMemory(ulMaxDataSize);
    }

    if ( !pName || !pData )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT;
    }

    /*
     *  enumrate records to caculate the CTLV size for records
     */
    ulCount = pSysIraIf->GetRecordCount(pSysIraIf->hOwnerContext, hFolder);

    for ( ulIndex = 0; ulIndex < ulCount; ulIndex++ )
    {
        ulNameSize   = SYS_MAX_RECORD_NAME_SIZE + 1;
        ulDataSize   = ulMaxDataSize;

        returnStatus =
            pSysIraIf->EnumRecord
                (
                    pSysIraIf->hOwnerContext,
                    hFolder,
                    ulIndex,
                    pName,
                    &ulNameSize,
                    &ulRecordType,
                    &ulDataSize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscTrace
                (
                    "SysCpoQueryFolderSize -- folder %s, failed to enumerate record, index %lu, error status = %d!!!\n",
                    pFolderName,
                    ulIndex,
                    returnStatus
                );

            goto  EXIT;
        }
        else
        {
            if ( ulDataSize > ulMaxDataSize )
            {
                AnscFreeMemory(pData);

                pData = AnscAllocateMemory(ulDataSize);

                if ( !pData )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto  EXIT;
                }
                else
                {
                    ulMaxDataSize = ulDataSize;
                }
            }
        }

        returnStatus =
            pSysIraIf->GetRecord
                (
                    pSysIraIf->hOwnerContext,
                    hFolder,
                    pName,
                    &ulRecordType,
                    NULL,
                    pData,
                    &ulDataSize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscTrace
                (
                    "SysCpoQueryFolderSize -- folder %s, failed to get the record %s, error status = %d!!!\n",
                    pFolderName,
                    pName,
                    returnStatus
                );
        }
        else
        {
            ulCtlvRecordSize += sizeof(SYS_CTLV_TUPLE);
            ulCtlvRecordSize += sizeof(SYS_CTLV_TUPLE) + ulNameSize;
            ulCtlvRecordSize += sizeof(SYS_CTLV_TUPLE) + ulDataSize;
        }
    }

    /*
     *  enumerate subfolders
     */
    ulCount = pSysIraIf->GetSubFolderCount(pSysIraIf->hOwnerContext, hFolder);

    for ( ulIndex = 0; ulIndex < ulCount; ulIndex++ )
    {
        ulNameSize   = SYS_MAX_RECORD_NAME_SIZE + 1;

        returnStatus =
            pSysIraIf->EnumSubFolder
                (
                    pSysIraIf->hOwnerContext,
                    hFolder,
                    ulIndex,
                    pName,
                    &ulNameSize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscTrace
                (
                    "SysCpoQueryFolderSize -- folder %s, failed to enumerate sub-folder %lu, error status = %d!!!\n",
                    pFolderName,
                    ulIndex,
                    returnStatus
                );
        }
        else
        {
            ulCtlvFolderSize += 
                pMyObject->QueryFolderSize
                    (
                        (ANSC_HANDLE)pMyObject,
                        hFolder,
                        pName
                    );
        }
    }

    /*
     *  caculate the final CTLV size
     */
    ulCtlvSize += sizeof(SYS_CTLV_TUPLE);                               /* folder composite */ 
    ulCtlvSize += sizeof(SYS_CTLV_TUPLE) + _ansc_strlen(pFolderName);   /* folder name */ 
    ulCtlvSize += ulCtlvRecordSize       + ulCtlvFolderSize;

EXIT:

    if ( pData )
    {
        AnscFreeMemory(pData);
    }

    if ( pName )
    {
        AnscFreeMemory(pName);
    }

    if ( hFolder )
    {
        pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hFolder);
    }

    AnscTrace("SysCpoQueryFolderSize -- %s size %lu.\n", pFolderName, ulCtlvSize);

    return  ulCtlvSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysCpoSerializeFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hParentFolder,
                char*                       pFolderName,
                PUCHAR                      pCtlvBuffer,
                ULONG                       ulBufferSize
            );

    description:

        This function is to export System Repository to the CTLV
        file.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hParentFolder
                Specifies the parent folder on which the CTLV tuple
                should be populated.

                char*                       pFolderName
                Specifies the name of the folder name.

                PUCHAR                      pCtlvBuffer
                Specifies the CTLV data buffer.

                ULONG                       ulBufferSize
                Specifies the buffer size for CTLV data.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysCpoSerializeFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParentFolder,
        char*                       pFolderName,
        PUCHAR                      pCtlvBuffer,
        ULONG                       ulBufferSize
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject               = (PSYS_CTLV_PROCESSOR_OBJECT)hThisObject;
    PSYS_IRA_INTERFACE              pSysIraIf               = (PSYS_IRA_INTERFACE        )pMyObject->hSysIraIf;

    ANSC_HANDLE                     hFolder                 = (ANSC_HANDLE               )NULL;

    PSYS_CTLV_TUPLE                 pCtlvTuple              = (PSYS_CTLV_TUPLE           )pCtlvBuffer;
    PSYS_CTLV_TUPLE                 pCtlvTupleNext	        = (PSYS_CTLV_TUPLE           )pCtlvBuffer;

    ULONG                           ulCount                 = 0;
    ULONG                           ulIndex                 = 0;

    char*                           pName                   = NULL;
    ULONG                           ulNameSize              = 0;
    PUCHAR                          pData                   = NULL;
    ULONG                           ulMaxDataSize           = SYS_MAX_ATTRIB_DATA_SIZE;
    ULONG                           ulDataSize              = 0;
    ULONG                           ulRecordType            = 0;
    ULONG                           ulContentType           = 0;

    ULONG                           ulCtlvSize              = 0;
    ULONG                           ulCtlvRecordSize        = 0;

    AnscTrace("SysCpoSerializeFolder -- %s, buffer size %lu ...\n", pFolderName, ulBufferSize);

    hFolder = 
        pSysIraIf->OpenFolder
            (
                pSysIraIf->hOwnerContext,
                hParentFolder,
                pFolderName
            );

    if ( !hFolder )
    {
        AnscTrace("SysCpoSerializeFolder -- failed to open folder %s...\n", pFolderName);

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pName   = AnscAllocateMemory(SYS_MAX_RECORD_NAME_SIZE + 1);
        pData   = AnscAllocateMemory(ulMaxDataSize);
    }

    if ( !pName || !pData )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT;
    }

    /*
     *  fill in the Folder composite and the folder name tuple
     */
    if ( TRUE )
    {
        SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_COMPOSITE);
        SysCtlvSetType     (pCtlvTupleNext, SYS_CPO_TYPE_folder     );
        SysCtlvSetLength   (pCtlvTupleNext, (USHORT)ulCtlvSize      );

        ulCtlvSize     += sizeof(SYS_CTLV_TUPLE);
        pCtlvTupleNext  = SysCtlvGetNextLevel(pCtlvTupleNext);
    }

    if ( TRUE )
    {
        SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_CSTR                  );
        SysCtlvSetType     (pCtlvTupleNext, SYS_CPO_TYPE_folderName              );
        SysCtlvSetLength   (pCtlvTupleNext, (USHORT)AnscSizeOfString(pFolderName));
        SysCtlvSetValueCstr(pCtlvTupleNext, pFolderName                          );

        ulCtlvSize     += SysCtlvGetSize     (pCtlvTupleNext);
        pCtlvTupleNext  = SysCtlvGetNextTuple(pCtlvTupleNext);
    }

    /*
     *  enumrate records to caculate the CTLV size for records
     */
    ulCount = pSysIraIf->GetRecordCount(pSysIraIf->hOwnerContext, hFolder);

    for ( ulIndex = 0; ulIndex < ulCount; ulIndex++ )
    {
        ulNameSize   = SYS_MAX_RECORD_NAME_SIZE + 1;
        ulDataSize   = ulMaxDataSize;

        returnStatus =
            pSysIraIf->EnumRecord
                (
                    pSysIraIf->hOwnerContext,
                    hFolder,
                    ulIndex,
                    pName,
                    &ulNameSize,
                    &ulRecordType,
                    &ulDataSize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscTrace
                (
                    "SysCpoSerializeFolder -- folder %s, failed to enumerate record, index %lu, error status = %d!!!\n",
                    pFolderName,
                    ulIndex,
                    returnStatus
                );

            goto  EXIT;
        }
        else
        {
            if ( ulDataSize > ulMaxDataSize )
            {
                AnscFreeMemory(pData);

                pData = AnscAllocateMemory(ulDataSize);

                if ( !pData )
                {
                    returnStatus = ANSC_STATUS_RESOURCES;

                    goto  EXIT;
                }
                else
                {
                    ulMaxDataSize = ulDataSize;
                }
            }
        }

        AnscTrace
            (
                "SysCpoSerializeFolder -- retrieving the record %s.\n",
                pName
            );

        returnStatus =
            pSysIraIf->GetRecord
                (
                    pSysIraIf->hOwnerContext,
                    hFolder,
                    pName,
                    &ulRecordType,
                    NULL,
                    pData,
                    &ulDataSize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscTrace
                (
                    "SysCpoSerializeFolder -- folder %s, failed to get the record %s, error status = %d!!!\n",
                    pFolderName,
                    pName,
                    returnStatus
                );

            goto  EXIT;
        }
        else
        {
            returnStatus =
                pSysIraIf->QueryRecord
                    (
                        pSysIraIf->hOwnerContext,
                        hFolder,
                        pName,
                        NULL,                       /* don't care time stamp         */
                        NULL,                       /* don't care permission         */
                        NULL,                       /* already retrieved record type */
                        NULL,                       /* already retrieved record size */
                        &ulContentType,
                        NULL                        /* don't care render attribute   */
                    );
        }

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscTrace
                (
                    "SysCpoSerializeFolder -- folder %s, failed to get the record %s, error status = %d!!!\n",
                    pFolderName,
                    pName,
                    returnStatus
                );

            goto  EXIT;
        }
        else
        {
            ulCtlvRecordSize  = sizeof(SYS_CTLV_TUPLE);
            ulCtlvRecordSize += sizeof(SYS_CTLV_TUPLE) + ulNameSize;
            ulCtlvRecordSize += sizeof(SYS_CTLV_TUPLE) + ulDataSize;

            ulCtlvSize       += ulCtlvRecordSize;
        }

        /*
         *  fill the record composite, name and data tuples.
         */
        if ( TRUE )
        {
            SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_COMPOSITE);
            SysCtlvSetType     (pCtlvTupleNext, SYS_CPO_TYPE_record     );
            SysCtlvSetLength   (pCtlvTupleNext, (USHORT)(ulCtlvRecordSize - sizeof(SYS_CTLV_TUPLE)));

            pCtlvTupleNext  = SysCtlvGetNextLevel(pCtlvTupleNext);
        }

        if ( TRUE )
        {
            SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_CSTR            );
            SysCtlvSetType     (pCtlvTupleNext, SYS_CPO_TYPE_recordName        );
            SysCtlvSetLength   (pCtlvTupleNext, (USHORT)AnscSizeOfString(pName));
            SysCtlvSetValueCstr(pCtlvTupleNext, pName                          );

            pCtlvTupleNext  = SysCtlvGetNextTuple(pCtlvTupleNext);
        }

        if ( TRUE )
        {
            SysCtlvSetType     (pCtlvTupleNext, SYS_CPO_TYPE_recordData);
            SysCtlvSetLength   (pCtlvTupleNext, (USHORT)ulDataSize     );
        }

        switch ( ulRecordType )
        {
            case    SYS_REP_RECORD_TYPE_SINT:

                    SysCtlvSetClass      (pCtlvTupleNext, SYS_CTLV_CLASS_SINT);
                    SysCtlvSetValueUshort(pCtlvTupleNext, (*((PUSHORT)pData)));

                    break;

            case    SYS_REP_RECORD_TYPE_HCXT:
            case    SYS_REP_RECORD_TYPE_UINT:

                    if ( ulContentType == SYS_RECORD_CONTENT_IP4_ADDR )
                    {
                        SysCtlvSetClass         (pCtlvTupleNext, SYS_CTLV_CLASS_IP4_ADDR);
                        SysCtlvSetValueIpAddrDot(pCtlvTupleNext, pData                  );
                    }
                    else
                    {
                        if ( ulContentType == SYS_RECORD_CONTENT_MAPPED )
                        {
                            SysCtlvSetClass     (pCtlvTupleNext, SYS_CTLV_CLASS_MAPPED);
                        }
                        else if ( ulContentType == SYS_RECORD_CONTENT_TCPUDP_PORT )
                        {
                            SysCtlvSetClass     (pCtlvTupleNext, SYS_CTLV_CLASS_TCPUDP_PORT);
                        }
                        else if ( ulContentType == SYS_RECORD_CONTENT_SECONDS )
                        {
                            SysCtlvSetClass     (pCtlvTupleNext, SYS_CTLV_CLASS_SECONDS);
                        }
                        else if ( ulContentType == SYS_RECORD_CONTENT_MINUTES )
                        {
                            SysCtlvSetClass     (pCtlvTupleNext, SYS_CTLV_CLASS_MINUTES);
                        }
                        else if ( ulContentType == SYS_RECORD_CONTENT_HOURS )
                        {
                            SysCtlvSetClass     (pCtlvTupleNext, SYS_CTLV_CLASS_HOURS);
                        }
                        else if ( ulContentType == SYS_RECORD_CONTENT_MASKED )
                        {
                            SysCtlvSetClass     (pCtlvTupleNext, SYS_CTLV_CLASS_MASKED);
                        }
                        else
                        {
                            SysCtlvSetClass     (pCtlvTupleNext, SYS_CTLV_CLASS_UINT);
                        }

                        SysCtlvSetValueUlong(pCtlvTupleNext, (*((PULONG)pData)) );
                    }

                    break;

            case    SYS_REP_RECORD_TYPE_BOOL:
                {
                    BOOL            bRecordValue    = *(PBOOL)pData;

                    SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_BOOL);
                    SysCtlvSetValueBool(pCtlvTupleNext, bRecordValue       );

                    break;
                }
            case    SYS_REP_RECORD_TYPE_ASTR:

                    if ( ulContentType == SYS_RECORD_CONTENT_PASSWORD )
                    {
                        SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_PASSWORD);
                    }
                    else
                    {
                        SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_CSTR);
                    }

                    SysCtlvSetValueCstr(pCtlvTupleNext, pData              );

                    break;

            case    SYS_REP_RECORD_TYPE_BSTR:

                    if ( ulContentType == SYS_RECORD_CONTENT_IP4_ADDR )
                    {
                        SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_IP4_ADDR );
                        SysCtlvSetValueOstr(pCtlvTupleNext, pData                   );
                    }
                    else if ( ulContentType == SYS_RECORD_CONTENT_MAC_ADDR )
                    {
                        SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_MAC_ADDR );
                        SysCtlvSetValueOstr(pCtlvTupleNext, pData                   );
                    }
                    else if ( ulContentType == SYS_RECORD_CONTENT_SINT_LIST )
                    {
                        SysCtlvSetClass          (pCtlvTupleNext, SYS_CTLV_CLASS_SINT_LIST          );
                        SysCtlvSetValueUshortList(pCtlvTupleNext, pData, ulDataSize / sizeof(USHORT));
                    }
                    else if ( ulContentType == SYS_RECORD_CONTENT_UINT_LIST )
                    {
                        SysCtlvSetClass         (pCtlvTupleNext, SYS_CTLV_CLASS_UINT_LIST         );
                        SysCtlvSetValueUlongList(pCtlvTupleNext, pData, ulDataSize / sizeof(ULONG));
                    }
                    else if ( ulContentType == SYS_RECORD_CONTENT_CALENDAR_TIME )
                    {
                        SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_CALENDAR_TIME);
                        SysCtlvSetValueOstr(pCtlvTupleNext, pData                       );
                    }
                    else if ( ulContentType == SYS_RECORD_CONTENT_IP4_ADDR_LIST )
                    {
                        SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_IP4_ADDR_LIST);
                        SysCtlvSetValueOstr(pCtlvTupleNext, pData                       );
                    }
                    else
                    {
                        SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_OSTR);
                        SysCtlvSetValueOstr(pCtlvTupleNext, pData              );
                    }

                    break;

            default:

                    AnscTrace
                        (
                            "SysCpoSerializeFolder -- folder %s, record %s, invalid record type %lu!!!",
                            pFolderName,
                            pName,
                            ulRecordType
                        );

                    returnStatus = ANSC_STATUS_FAILURE;

                    goto  EXIT;
        }

        pCtlvTupleNext  = SysCtlvGetNextTuple(pCtlvTupleNext);
    }

    /*
     *  enumerate subfolders
     */
    ulCount = pSysIraIf->GetSubFolderCount(pSysIraIf->hOwnerContext, hFolder);

    for ( ulIndex = 0; ulIndex < ulCount; ulIndex++ )
    {
        ulNameSize   = SYS_MAX_RECORD_NAME_SIZE + 1;

        returnStatus =
            pSysIraIf->EnumSubFolder
                (
                    pSysIraIf->hOwnerContext,
                    hFolder,
                    ulIndex,
                    pName,
                    &ulNameSize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscTrace
                (
                    "SysCpoSerializeFolder -- folder %s, failed to enumerate sub-folder %lu, error status = %d!!!\n",
                    pFolderName,
                    ulIndex,
                    returnStatus
                );

            goto  EXIT;
        }
        else
        {
            returnStatus =
                pMyObject->SerializeFolder
                    (
                        (ANSC_HANDLE)pMyObject,
                        hFolder,
                        pName,
                        (PUCHAR)pCtlvTupleNext,
                        ulBufferSize - ulCtlvSize
                    );
        }

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscTrace
                (
                    "SysCpoSerializeFolder -- folder %s, failed to serialize sub-folder %s, error status = %d!!!\n",
                    pFolderName,
                    pName,
                    returnStatus
                );

            goto  EXIT;
        }
        else
        {
            ulCtlvSize     += SysCtlvGetSize     (pCtlvTupleNext);
            pCtlvTupleNext  = SysCtlvGetNextTuple(pCtlvTupleNext);
        }
    }

    /*
     *  update the length of folder composite at last
     */
    if ( TRUE )
    {
        /* Do not count in this Composite tuple */
        SysCtlvSetLength   (pCtlvTuple, (USHORT)(ulCtlvSize - sizeof(SYS_CTLV_TUPLE)));
    }

EXIT:

    if ( pData )
    {
        AnscFreeMemory(pData);
    }

    if ( pName )
    {
        AnscFreeMemory(pName);
    }

    if ( hFolder )
    {
        pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hFolder);
    }

    AnscTrace("SysCpoSerializeFolder -- %s, return status %d, size = %lu.\n", pFolderName, returnStatus, ulCtlvSize);

    return  returnStatus;
}

