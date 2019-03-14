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

    module:	sys_cpo_provision.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the provisioning functions
        of the Sys Ctlv Processor Object.

        *   SysCpoProvisionFolder

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

        ANSC_STATUS
        SysCpoProvisionFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hParentFolder,
                PUCHAR                      pCtlvBuffer,
                ULONG                       ulCtlvSize
            );

    description:

        This function is to populate System Repository from the CTLV
        file.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hParentFolder
                Specifies the parent folder on which the CTLV tuple
                should be populated.

                PUCHAR                      pCtlvBuffer
                Specifies the CTLV data buffer.

                ULONG                       ulCtlvSize
                Specifies the size of CTLV data.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysCpoProvisionFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParentFolder,
        PUCHAR                      pCtlvBuffer,
        ULONG                       ulCtlvSize
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject               = (PSYS_CTLV_PROCESSOR_OBJECT)hThisObject;
    PSYS_IRA_INTERFACE              pSysIraIf               = (PSYS_IRA_INTERFACE        )pMyObject->hSysIraIf;

    ANSC_HANDLE                     hFolder                 = (ANSC_HANDLE               )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTuple              = (PSYS_CTLV_TUPLE           )pCtlvBuffer;
    PSYS_CTLV_TUPLE                 pCtlvTupleFolder        = NULL;
    ULONG                           ulFolderTupleSize       = 0;

    PSYS_CTLV_TUPLE                 pCtlvTupleData          = 0;
    ULONG                           ulTupleSize             = 0;

    PSYS_CTLV_TUPLE                 pCtlvTupleFolderName    = NULL;
    UCHAR                           ucTemp                  = 0;
    UCHAR                           ucTemp2                 = 0;

    PSYS_CTLV_TUPLE                 pCtlvTupleRecord        = NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleRecordName    = NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleRecordData    = NULL;
    ULONG                           ulRecordClass           = 0;
    static SYS_RRO_RENDER_ATTR      RenderAttr              = {0};      /* we are fine since exclusive access was gained */

    PSYS_CTLV_TUPLE                 pCtlvTupleSubFolder     = NULL;

    AnscTrace("SysCpoProvisionFolder...\n");

    pCtlvTupleFolder    = SysCtlvGetNextLevel(pCtlvTuple);
    ulFolderTupleSize   = SysCtlvGetSize(pCtlvTuple) - ((PUCHAR)pCtlvTupleFolder - (PUCHAR)pCtlvBuffer);

    /*
     *  Get the folder name and create the folder
     */
    SysCtlvGetTupleByType2(pCtlvTupleFolder, ulFolderTupleSize, SYS_CPO_TYPE_folderName, pCtlvTupleFolderName);

    if ( !pCtlvTupleFolderName )
    {
        AnscTrace("SysCpoProvisionFolder -- cannot find folder name tuple!!!\n");

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        ucTemp = SysCtlvGetValueCstr(pCtlvTupleFolderName)[SysCtlvGetLength(pCtlvTupleFolderName)];
        SysCtlvGetValueCstr(pCtlvTupleFolderName)[SysCtlvGetLength(pCtlvTupleFolderName)] = '\0';

        AnscTrace("SysCpoProvisionFolder -- add folder %s.\n", SysCtlvGetValueCstr(pCtlvTupleFolderName));

        hFolder = (ANSC_HANDLE)
            pSysIraIf->AddFolder
                (
                    pSysIraIf->hOwnerContext,
                    hParentFolder,
                    SysCtlvGetValueCstr(pCtlvTupleFolderName)
                );

        if ( !hFolder )
        {
            AnscTrace("SysCpoProvisionFolder -- failed to add folder %s!\n", SysCtlvGetValueCstr(pCtlvTupleFolderName));

            SysCtlvGetValueCstr(pCtlvTupleFolderName)[SysCtlvGetLength(pCtlvTupleFolderName)] = ucTemp;

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT;
        }
        else
        {
            SysCtlvGetValueCstr(pCtlvTupleFolderName)[SysCtlvGetLength(pCtlvTupleFolderName)] = ucTemp;
        }
    }

    /*
     *  Add all the records
     */
    SysCtlvGetTupleByType2(pCtlvTupleFolder, ulFolderTupleSize, SYS_CPO_TYPE_record, pCtlvTupleRecord);

    while ( pCtlvTupleRecord )
    {
        pCtlvTupleData      = SysCtlvGetNextLevel(pCtlvTupleRecord);
        ulTupleSize         = SysCtlvGetLength   (pCtlvTupleRecord);

        SysCtlvGetTupleByType2(pCtlvTupleData, ulTupleSize, SYS_CPO_TYPE_recordName, pCtlvTupleRecordName);
        SysCtlvGetTupleByType2(pCtlvTupleData, ulTupleSize, SYS_CPO_TYPE_recordData, pCtlvTupleRecordData);

        if ( !pCtlvTupleRecordName || !pCtlvTupleRecordData )
        {
            AnscTrace("SysCpoProvisionFolder -- cannot find either record name or data!\n");

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT;
        }

        if ( TRUE )
        {
            ulRecordClass = SysCtlvGetClass(pCtlvTupleRecordData);

            ucTemp = SysCtlvGetValueCstr(pCtlvTupleRecordName)[SysCtlvGetLength(pCtlvTupleRecordName)];
            SysCtlvGetValueCstr(pCtlvTupleRecordName)[SysCtlvGetLength(pCtlvTupleRecordName)] = '\0';

            AnscTrace("SysCpoProvisionFolder -- add record %s.\n", SysCtlvGetValueCstr(pCtlvTupleRecordName));
        }

        switch ( ulRecordClass )
        {
            case    SYS_CTLV_CLASS_SINT:
                {
                    USHORT          usRecordValue   = SysCtlvGetValueUshort(pCtlvTupleRecordData);

                    returnStatus =
                        pSysIraIf->AddRecord
                            (
                                pSysIraIf->hOwnerContext,
                                hFolder,
                                SysCtlvGetValueCstr(pCtlvTupleRecordName),
                                SYS_REP_RECORD_TYPE_SINT,
                                &usRecordValue,
                                sizeof(usRecordValue)
                            );

                    break;
                }
            case    SYS_CTLV_CLASS_UINT:
            case    SYS_CTLV_CLASS_IP4_ADDR:
            case    SYS_CTLV_CLASS_MAPPED:
            case    SYS_CTLV_CLASS_TCPUDP_PORT:
            case    SYS_CTLV_CLASS_SECONDS:
            case    SYS_CTLV_CLASS_MINUTES:
            case    SYS_CTLV_CLASS_HOURS:
            case    SYS_CTLV_CLASS_MASKED:
                {
                    ULONG           ulRecordValue   = 0;

                    if ( ulRecordClass == SYS_CTLV_CLASS_IP4_ADDR )
                    {
                        ulRecordValue          = SysCtlvGetValueIpAddrValue(pCtlvTupleRecordData);
                        RenderAttr.ContentType = SYS_RECORD_CONTENT_IP4_ADDR;
                    }
                    else
                    {
                        ulRecordValue   = SysCtlvGetValueUlong(pCtlvTupleRecordData);

                        if ( ulRecordClass == SYS_CTLV_CLASS_MAPPED )
                        {
                            RenderAttr.ContentType = SYS_RECORD_CONTENT_MAPPED;
                        }
                        else if ( ulRecordClass == SYS_CTLV_CLASS_TCPUDP_PORT )
                        {
                            RenderAttr.ContentType = SYS_RECORD_CONTENT_TCPUDP_PORT;
                        }
                        else if ( ulRecordClass == SYS_CTLV_CLASS_SECONDS )
                        {
                            RenderAttr.ContentType = SYS_RECORD_CONTENT_SECONDS;
                        }
                        else if ( ulRecordClass == SYS_CTLV_CLASS_MINUTES )
                        {
                            RenderAttr.ContentType = SYS_RECORD_CONTENT_MINUTES;
                        }
                        else if ( ulRecordClass == SYS_CTLV_CLASS_HOURS )
                        {
                            RenderAttr.ContentType = SYS_RECORD_CONTENT_HOURS;
                        }
                        else if ( ulRecordClass == SYS_CTLV_CLASS_MASKED )
                        {
                            RenderAttr.ContentType = SYS_RECORD_CONTENT_MASKED;
                        }
                        else
                        {
                            RenderAttr.ContentType = SYS_RECORD_CONTENT_DEFAULT;
                        }
                    }

                    returnStatus =
                        pSysIraIf->AddRecord2
                            (
                                pSysIraIf->hOwnerContext,
                                hFolder,
                                SysCtlvGetValueCstr(pCtlvTupleRecordName),
                                SYS_RRO_PERMISSION_ALL,
                                SYS_REP_RECORD_TYPE_UINT,
                                &RenderAttr,
                                &ulRecordValue,
                                sizeof(ulRecordValue)
                            );

                    break;
                }
            case    SYS_CTLV_CLASS_BOOL:
                {
                    BOOL            bRecordValue    = SysCtlvGetValueBool(pCtlvTupleRecordData);

                    returnStatus =
                        pSysIraIf->AddRecord
                            (
                                pSysIraIf->hOwnerContext,
                                hFolder,
                                SysCtlvGetValueCstr(pCtlvTupleRecordName),
                                SYS_REP_RECORD_TYPE_BOOL,
                                &bRecordValue,
                                sizeof(bRecordValue)
                            );

                    break;
                }
            case    SYS_CTLV_CLASS_CSTR:
            case    SYS_CTLV_CLASS_PASSWORD:
                {
                    if ( ulRecordClass == SYS_CTLV_CLASS_PASSWORD )
                    {
                        RenderAttr.ContentType = SYS_RECORD_CONTENT_PASSWORD;
                    }
                    else
                    {
                        RenderAttr.ContentType = SYS_RECORD_CONTENT_DEFAULT;
                    }

                    ucTemp2 = SysCtlvGetValueCstr(pCtlvTupleRecordData)[SysCtlvGetLength(pCtlvTupleRecordData)];
                    SysCtlvGetValueCstr(pCtlvTupleRecordData)[SysCtlvGetLength(pCtlvTupleRecordData)] = '\0';

                    returnStatus =
                        pSysIraIf->AddRecord2
                            (
                                pSysIraIf->hOwnerContext,
                                hFolder,
                                SysCtlvGetValueCstr(pCtlvTupleRecordName),
                                SYS_RRO_PERMISSION_ALL,
                                SYS_REP_RECORD_TYPE_ASTR,
                                &RenderAttr,
                                SysCtlvGetValue(pCtlvTupleRecordData),
                                SysCtlvGetLength(pCtlvTupleRecordData)
                            );

                    SysCtlvGetValueCstr(pCtlvTupleRecordData)[SysCtlvGetLength(pCtlvTupleRecordData)] = ucTemp2;

                    break;
                }
            case    SYS_CTLV_CLASS_OSTR:
            case    SYS_CTLV_CLASS_MAC_ADDR:
            case    SYS_CTLV_CLASS_IP4_ADDR_LIST:
            case    SYS_CTLV_CLASS_CALENDAR_TIME:
                {
                    if ( ulRecordClass == SYS_CTLV_CLASS_MAC_ADDR )
                    {
                        RenderAttr.ContentType = SYS_RECORD_CONTENT_MAC_ADDR;
                    }
                    else if ( ulRecordClass == SYS_CTLV_CLASS_IP4_ADDR_LIST )
                    {
                        RenderAttr.ContentType = SYS_RECORD_CONTENT_IP4_ADDR_LIST;
                    }
                    else if ( ulRecordClass == SYS_CTLV_CLASS_CALENDAR_TIME )
                    {
                        RenderAttr.ContentType = SYS_RECORD_CONTENT_CALENDAR_TIME;
                    }
                    else
                    {
                        RenderAttr.ContentType = SYS_RECORD_CONTENT_DEFAULT;
                    }

                    returnStatus =
                        pSysIraIf->AddRecord2
                            (
                                pSysIraIf->hOwnerContext,
                                hFolder,
                                SysCtlvGetValueCstr(pCtlvTupleRecordName),
                                SYS_RRO_PERMISSION_ALL,
                                SYS_REP_RECORD_TYPE_BSTR,
                                &RenderAttr,
                                SysCtlvGetValue(pCtlvTupleRecordData),
                                SysCtlvGetLength(pCtlvTupleRecordData)
                            );

                    break;
                }

            case    SYS_CTLV_CLASS_SINT_LIST:
            case    SYS_CTLV_CLASS_UINT_LIST:
                {
                    ULONG               ulTemp      = 0;
                    PUCHAR              pData       = NULL;

                    pData   = AnscAllocateMemory(SysCtlvGetLength(pCtlvTupleRecordData));

                    if ( !pData )
                    {
                        AnscTrace
                            (
                                "SysCpoProvisionFolder -- failed to allocate data memory for record %s!\n",
                                SysCtlvGetValueCstr(pCtlvTupleRecordName)
                            );

                        returnStatus = ANSC_STATUS_RESOURCES;
                    }
                    else
                    {
                        if ( ulRecordClass == SYS_CTLV_CLASS_SINT_LIST )
                        {
                            SysCtlvGetValueUshortList(pCtlvTupleRecordData, pData, ulTemp);
                            RenderAttr.ContentType = SYS_RECORD_CONTENT_SINT_LIST;
                        }
                        else
                        {
                            SysCtlvGetValueUlongList(pCtlvTupleRecordData, pData, ulTemp);
                            RenderAttr.ContentType = SYS_RECORD_CONTENT_UINT_LIST;
                        }

                        returnStatus =
                            pSysIraIf->AddRecord2
                                (
                                    pSysIraIf->hOwnerContext,
                                    hFolder,
                                    SysCtlvGetValueCstr(pCtlvTupleRecordName),
                                    SYS_RRO_PERMISSION_ALL,
                                    SYS_REP_RECORD_TYPE_BSTR,
                                    &RenderAttr,
                                    pData,
                                    SysCtlvGetLength(pCtlvTupleRecordData)
                                );

                        AnscFreeMemory(pData);
                    }

                    break;
                }

            default:
                    SysCtlvGetValueCstr(pCtlvTupleRecordName)[SysCtlvGetLength(pCtlvTupleRecordName)] = ucTemp;

                    AnscTrace("SysCpoProvisionFolder -- invalid record data class %u!\n", SysCtlvGetClass(pCtlvTupleRecordData));
                    returnStatus = ANSC_STATUS_FAILURE;
                    goto  EXIT;
        }

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            AnscTrace
                (
                    "SysCpoProvisionFolder -- failed to add record %s, status = %d!\n",
                    SysCtlvGetValueCstr(pCtlvTupleRecordName),
                    returnStatus
                );

            SysCtlvGetValueCstr(pCtlvTupleRecordName)[SysCtlvGetLength(pCtlvTupleRecordName)] = ucTemp;

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT;
        }
        else
        {
            SysCtlvGetValueCstr(pCtlvTupleRecordName)[SysCtlvGetLength(pCtlvTupleRecordName)] = ucTemp;
        }

        pCtlvTupleData = SysCtlvGetNextTuple(pCtlvTupleRecord);
        ulTupleSize    = ulFolderTupleSize - (((PUCHAR)pCtlvTupleRecord) - ((PUCHAR)pCtlvTupleFolder));

        SysCtlvGetTupleByType2(pCtlvTupleData, ulTupleSize, SYS_CPO_TYPE_record, pCtlvTupleRecord);
    }


    /* 
     *  Add all sub folders
     */
    SysCtlvGetTupleByType2(pCtlvTupleFolder, ulFolderTupleSize, SYS_CPO_TYPE_folder, pCtlvTupleSubFolder);

    while ( pCtlvTupleSubFolder )
    {
        ulTupleSize = ulFolderTupleSize - (((PUCHAR)pCtlvTupleSubFolder) - ((PUCHAR)pCtlvTupleFolder));

        returnStatus =
            pMyObject->ProvisionFolder
                (
                    (ANSC_HANDLE)pMyObject,
                    hFolder,
                    (PUCHAR)pCtlvTupleSubFolder,
                    ulTupleSize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto  EXIT;
        }
        else
        {
            pCtlvTupleData = SysCtlvGetNextTuple(pCtlvTupleSubFolder);

            if ( SysCtlvGetType(pCtlvTupleData) == SYS_CTLV_TYPE_END )
            {
                break;
            }
            else if ( SysCtlvIsTupleTheEnd(pCtlvTupleData, pCtlvTupleFolder, ulFolderTupleSize) )
            {
                break;
            }
            else
            {
                SysCtlvGetTupleByType2(pCtlvTupleData, ulTupleSize, SYS_CPO_TYPE_folder, pCtlvTupleSubFolder);
            }
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;

EXIT:

    if ( hFolder )
    {
        pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hFolder);
    }

    if ( TRUE )
    {
        ucTemp = SysCtlvGetValueCstr(pCtlvTupleFolderName)[SysCtlvGetLength(pCtlvTupleFolderName)];
        SysCtlvGetValueCstr(pCtlvTupleFolderName)[SysCtlvGetLength(pCtlvTupleFolderName)] = '\0';

        AnscTrace("SysCpoProvisionFolder -- folder %s done, status = %d.\n", SysCtlvGetValueCstr(pCtlvTupleFolderName), returnStatus);

        SysCtlvGetValueCstr(pCtlvTupleFolderName)[SysCtlvGetLength(pCtlvTupleFolderName)] = ucTemp;
    }

    return  returnStatus;
}

