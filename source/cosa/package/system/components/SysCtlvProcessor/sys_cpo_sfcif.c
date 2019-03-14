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

    module:	sys_cpo_sfcif.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Sys Ctlv Processor Object.

        *   SysCpoSfcVerify
        *   SysCpoSfcProvision
        *   SysCpoSfcSerialize

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
        SysCpoSfcVerify
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pBuffer,
                ULONG                       ulSize
            );

    description:

        This function is called to verify whether the CTLV config file
        is valid.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pBuffer
                Specifies the CTLV data buffer.

                ULONG                       ulSize
                Specifies the size of CTLV data.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysCpoSfcVerify
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBuffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject       = (PSYS_CTLV_PROCESSOR_OBJECT)hThisObject;
    PSYS_CTLV_TUPLE                 pCtlvTuple      = (PSYS_CTLV_TUPLE           )pBuffer;
    PSYS_CTLV_TUPLE                 pCtlvTupleName  = (PSYS_CTLV_TUPLE           )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleVer   = (PSYS_CTLV_TUPLE           )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTuplePrev  = (PSYS_CTLV_TUPLE           )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleData  = (PSYS_CTLV_TUPLE           )NULL;

    AnscTrace("SysCpoSfcVerify...\n");

    /*
     *  Root Folder Composite
     *
    if ( ulSize != SysCtlvGetSize(pCtlvTuple) )
    {
        AnscTrace
            (
                "SysCpoSfcVerify -- data size %lu doesn't conform to the size in root folder composite %lu!!!\n",
                ulSize,
                SysCtlvGetSize(pCtlvTuple)
            );

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pCtlvTuple  = SysCtlvGetNextLevel(pCtlvTuple);
        ulSize     -= sizeof(SYS_CTLV_TUPLE);
    }
     */
    if ( ulSize < SysCtlvGetSize(pCtlvTuple) )
    {
        AnscTrace
            (
                "SysCpoSfcVerify -- data size of the first tuple %lu is larger than data size %lu!!!\n",
                SysCtlvGetSize(pCtlvTuple),
                ulSize
            );

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }

    /*
     *  check fileTypeName and version tuples
     */
    SysCtlvGetTupleByType2(pCtlvTuple, ulSize, SYS_CPO_TYPE_fileTypeName, pCtlvTupleName);
    SysCtlvGetTupleByType2(pCtlvTuple, ulSize, SYS_CPO_TYPE_version,      pCtlvTupleVer );

    if ( !pCtlvTupleName || !pCtlvTupleVer )
    {
        AnscTrace("SysCpoSfcVerify -- cannot find either fileTypeName or version tuple!!!\n");

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else if (    (ulSize < (SysCtlvGetSize(pCtlvTupleName) + ((PUCHAR)pCtlvTupleName - (PUCHAR)pCtlvTuple)))
              || (ulSize < (SysCtlvGetSize(pCtlvTupleVer)  + ((PUCHAR)pCtlvTupleVer  - (PUCHAR)pCtlvTuple))) )
    {
        AnscTrace("SysCpoSfcVerify -- either the size of fileTypeName or version tuple is invalid!!!\n");

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else if ( SysCtlvGetLength(pCtlvTupleName) != _ansc_strlen(SYS_CPO_LAYOUT_fileTypeName) )
    {
        AnscTrace("SysCpoSfcVerify -- invalid fileTypeName %s!!!\n", SysCtlvGetValueUlong(pCtlvTupleVer));

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else if ( _ansc_memcmp(SysCtlvGetValueCstr(pCtlvTupleName), SYS_CPO_LAYOUT_fileTypeName, SysCtlvGetLength(pCtlvTupleName)) )
    {
        UCHAR                       ucTemp  = SysCtlvGetValueCstr(pCtlvTupleName)[SysCtlvGetLength(pCtlvTupleName)];

        SysCtlvGetValueCstr(pCtlvTupleName)[SysCtlvGetLength(pCtlvTupleName)] = '\0';
        AnscTrace("SysCpoSfcVerify -- invalid fileTypeName %s!!!\n", SysCtlvGetValueUlong(pCtlvTupleVer));
        SysCtlvGetValueCstr(pCtlvTupleName)[SysCtlvGetLength(pCtlvTupleName)] = ucTemp;

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else if ( SysCtlvGetValueUlong(pCtlvTupleVer) != SYS_CPO_LAYOUT_version )
    {
        AnscTrace("SysCpoSfcVerify -- invalid version %lu!!!\n", SysCtlvGetValueUlong(pCtlvTupleVer));

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }

    /*
     *  check all root-level tuples are not out-of-bound
     */
    pCtlvTuplePrev = pCtlvTuple;
    pCtlvTupleData = SysCtlvGetNextTuple(pCtlvTuple);

    while ( pCtlvTupleData )
    {
        if ( SysCtlvIsTupleOutbound(pCtlvTupleData, pCtlvTuple, ulSize) )
        {
            if ( SysCtlvIsTupleTheEnd(pCtlvTupleData, pCtlvTuple, ulSize) )
            {
                break;
            }
            else
            {
                AnscTrace
                    (
                        "SysCpoSfcVerify -- invalid tuple, class %lu, type %lu, size %lu!!!\n",
                        SysCtlvGetClass(pCtlvTuplePrev),
                        SysCtlvGetType(pCtlvTuplePrev),
                        SysCtlvGetSize(pCtlvTuplePrev)
                    );

                returnStatus = ANSC_STATUS_FAILURE;

                goto  EXIT;
            }
        }
        else if ( SysCtlvGetType(pCtlvTupleData) == SYS_CTLV_TYPE_END )
        {
            break;
        }

        pCtlvTuplePrev  = pCtlvTupleData;
        pCtlvTupleData  = SysCtlvGetNextTuple(pCtlvTupleData);
    }

    returnStatus = ANSC_STATUS_SUCCESS;

EXIT:

    AnscTrace("SysCpoSfcVerify -- return status = %ld...\n", returnStatus);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysCpoSfcProvision
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pBuffer,
                ULONG                       ulSize
            );

    description:

        This function is called to load the CTLV format into System
        Repository.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pBuffer
                Specifies the CTLV data buffer.

                ULONG                       ulSize
                Specifies the size of CTLV data.

    return:     status of operation.

    notes:      

        It is assumed Verify() has been called before Provision()
        is called, therefore, this routine doesn't do sanity check.

**********************************************************************/

ANSC_STATUS
SysCpoSfcProvision
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBuffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject       = (PSYS_CTLV_PROCESSOR_OBJECT)hThisObject;
    PSYS_IRA_INTERFACE              pSysIraIf       = (PSYS_IRA_INTERFACE        )pMyObject->hSysIraIf;

    PSYS_CTLV_TUPLE                 pCtlvTuple      = (PSYS_CTLV_TUPLE           )pBuffer;
    PSYS_CTLV_TUPLE                 pCtlvTuplePrev  = (PSYS_CTLV_TUPLE           )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleFolder= (PSYS_CTLV_TUPLE           )NULL;

    ANSC_HANDLE                     hSysRoot        = (ANSC_HANDLE               )NULL;

    AnscTrace("SysCpoSfcProvision ...\n");

    if ( !pSysIraIf )
    {
        AnscTrace("SysCpoSfcProvision -- no Sys Ira If!!!\n");

        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        /*
         *  Root Folder Composite
         *
        pCtlvTuple  = SysCtlvGetNextLevel(pCtlvTuple);
        ulSize     -= sizeof(SYS_CTLV_TUPLE);
         */
        SysCtlvGetTupleByType2(pCtlvTuple, ulSize, SYS_CPO_TYPE_folder, pCtlvTupleFolder);
    }

    if ( pCtlvTupleFolder )
    {
        /* get the system root folder */
        hSysRoot =
            pSysIraIf->OpenFolder
                (
                    pSysIraIf->hOwnerContext,
                    (ANSC_HANDLE)NULL,
                    "/Configuration/SetUp"
                );

        if ( !hSysRoot )
        {
            AnscTrace("SysCpoSfcProvision -- cannot open System Repository Root Folder!!!\n");

            returnStatus =  ANSC_STATUS_ACCESS_DENIED;

            goto EXIT;
        }
    }

    while ( pCtlvTupleFolder )
    {
        if ( SysCtlvIsTupleOutbound(pCtlvTupleFolder, pCtlvTuple, ulSize) )
        {
            if ( SysCtlvIsTupleTheEnd(pCtlvTupleFolder, pCtlvTuple, ulSize) )
            {
                break;
            }
            else
            {
                AnscTrace
                    (
                        "SysCpoSfcProvision -- invalid tuple, class %lu, type %lu, size %lu!!!\n",
                        SysCtlvGetClass(pCtlvTuplePrev),
                        SysCtlvGetType(pCtlvTuplePrev),
                        SysCtlvGetSize(pCtlvTuplePrev)
                    );

                returnStatus = ANSC_STATUS_FAILURE;

                goto  EXIT;
            }
        }
        else if ( SysCtlvGetType(pCtlvTupleFolder) == SYS_CTLV_TYPE_END )
        {
            break;
        }

        if ( SysCtlvGetType(pCtlvTupleFolder) == SYS_CPO_TYPE_folder )
        {
            /*
             *  populate the folder into Repository
             */
            returnStatus =
                pMyObject->ProvisionFolder
                    (
                        (ANSC_HANDLE)pMyObject,
                        hSysRoot,
                        (PUCHAR)pCtlvTupleFolder,
                        ulSize - ((PUCHAR)pCtlvTupleFolder - (PUCHAR)pCtlvTuple)
                    );
        }

        pCtlvTuplePrev   = pCtlvTupleFolder;
        pCtlvTupleFolder = SysCtlvGetNextTuple(pCtlvTupleFolder);
    }

    returnStatus = ANSC_STATUS_SUCCESS;

EXIT:

    if( hSysRoot != NULL)
    {
        pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hSysRoot);
    }

    AnscTrace("SysCpoSfcProvision -- return status = %ld...\n", returnStatus);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysCpoSfcSerialize
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR*                     ppBuffer,
                PULONG                      pulSize
            );

    description:

        This function is called to export System Repository to CTLV
        format

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR*                     ppBuffer
                To receive the final CTLV data buffer.

                PULONG                      pulSize
                To receive the final size of CTLV data.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysCpoSfcSerialize
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR*                     ppBuffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject       = (PSYS_CTLV_PROCESSOR_OBJECT)hThisObject;
    PSYS_IRA_INTERFACE              pSysIraIf       = (PSYS_IRA_INTERFACE        )pMyObject->hSysIraIf;

    ANSC_HANDLE                     hSysRoot        = (ANSC_HANDLE               )NULL;
    ULONG                           ulCtlvSize      = 0;
    ULONG                           ulFolderSize    = 0;

    ULONG                           ulCount         = 0;
    ULONG                           ulIndex         = 0;
    char*                           pName           = NULL;
    ULONG                           ulNameSize      = 0;

    PSYS_CTLV_TUPLE                 pCtlvTuple      = NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleNext  = NULL;
    ULONG                           ulTotalSize     = 0;

    AnscTrace("SysCpoSfcSerialize ...\n");

    if ( !pSysIraIf )
    {
        AnscTrace("SysCpoSfcSerialize -- no Sys Ira If!!!\n");

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT;
    }
    else
    {
        pName   = AnscAllocateMemory(SYS_MAX_RECORD_NAME_SIZE + 1);
    }

    if ( !pName )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT;
    }
    else
    {
        /*
         *  Get the System Repository root folder
         */
        pSysIraIf->AcqWriteAccess(pSysIraIf->hOwnerContext);

        hSysRoot =
            pSysIraIf->OpenFolder
                (
                    pSysIraIf->hOwnerContext,
                    (ANSC_HANDLE)NULL,
                    "/Configuration/SetUp"
                );
    }

    if ( !hSysRoot )
    {
        AnscTrace("SysCpoSfcSerialize -- cannot open System Repository Root Folder!!!\n");

        returnStatus =  ANSC_STATUS_ACCESS_DENIED;

        goto EXIT1;
    }
    else
    {
        /*
         *  Root Folder Composite
         *
        ulCtlvSize   += sizeof(SYS_CTLV_TUPLE);
         */
        ulCtlvSize   += sizeof(SYS_CTLV_TUPLE) + _ansc_strlen(SYS_CPO_LAYOUT_fileTypeName);     /* SYS_CPO_TYPE_fileTypeName */
        ulCtlvSize   += sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);                                 /* SYS_CPO_TYPE_version */

        /*
         *  enumerate subfolders
         */
        ulCount = pSysIraIf->GetSubFolderCount(pSysIraIf->hOwnerContext, hSysRoot);

        for ( ulIndex = 0; ulIndex < ulCount; ulIndex++ )
        {
            ulNameSize   = SYS_MAX_RECORD_NAME_SIZE + 1;

            returnStatus =
                pSysIraIf->EnumSubFolder
                    (
                        pSysIraIf->hOwnerContext,
                        hSysRoot,
                        ulIndex,
                        pName,
                        &ulNameSize
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscTrace
                    (
                        "SysCpoSfcSerialize -- failed to enumerate sub-folder %lu, error status = %d!!!\n",
                        ulIndex,
                        returnStatus
                    );

                goto  EXIT1;
            }
            else
            {
                ulFolderSize += 
                    pMyObject->QueryFolderSize
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSysRoot,
                            pName
                        );
            }
        }

        ulCtlvSize   += ulFolderSize;
    }

    /*
     *  export the Repository
     */
    pCtlvTuple = (PSYS_CTLV_TUPLE)AnscAllocateMemory(ulCtlvSize);

    if ( pCtlvTuple == NULL )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT1;
    }
    else
    {
        pCtlvTupleNext = pCtlvTuple;
    }

    if ( FALSE )
    {
        /*
         *  Root Folder Composite
         */
        SysCtlvSetClass    (pCtlvTupleNext, SYS_CTLV_CLASS_COMPOSITE                     );
        SysCtlvSetType     (pCtlvTupleNext, SYS_CPO_TYPE_folder                          );
        SysCtlvSetLength   (pCtlvTupleNext, (USHORT)(ulCtlvSize - sizeof(SYS_CTLV_TUPLE)));

        ulTotalSize    += sizeof(SYS_CTLV_TUPLE);
        pCtlvTupleNext  = SysCtlvGetNextLevel(pCtlvTupleNext);
    }

    if ( TRUE )
    {
        SysCtlvSetClass    (pCtlvTupleNext, SYS_CPO_CLASS_fileTypeName                       );
        SysCtlvSetType     (pCtlvTupleNext, SYS_CPO_TYPE_fileTypeName                        );
        SysCtlvSetLength   (pCtlvTupleNext, (USHORT)_ansc_strlen(SYS_CPO_LAYOUT_fileTypeName));
        SysCtlvSetValueCstr(pCtlvTupleNext, SYS_CPO_LAYOUT_fileTypeName                      );

        ulTotalSize    += SysCtlvGetSize     (pCtlvTupleNext);
        pCtlvTupleNext  = SysCtlvGetNextTuple(pCtlvTupleNext);
    }

    if ( TRUE )
    {
        SysCtlvSetClass     (pCtlvTupleNext, SYS_CPO_CLASS_version );
        SysCtlvSetType      (pCtlvTupleNext, SYS_CPO_TYPE_version  );
        SysCtlvSetLength    (pCtlvTupleNext, (USHORT)sizeof(ULONG) );
        SysCtlvSetValueUlong(pCtlvTupleNext, SYS_CPO_LAYOUT_version);

        ulTotalSize    += SysCtlvGetSize     (pCtlvTupleNext);
        pCtlvTupleNext  = SysCtlvGetNextTuple(pCtlvTupleNext);
    }

    if ( TRUE )
    {
        /*
         *  enumerate subfolders
         */
        for ( ulIndex = 0; ulIndex < ulCount; ulIndex++ )
        {
            ulNameSize   = SYS_MAX_RECORD_NAME_SIZE + 1;

            returnStatus =
                pSysIraIf->EnumSubFolder
                    (
                        pSysIraIf->hOwnerContext,
                        hSysRoot,
                        ulIndex,
                        pName,
                        &ulNameSize
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscTrace
                    (
                        "SysCpoSfcSerialize -- failed to enumerate sub-folder %lu, error status = %d!!!\n",
                        ulIndex,
                        returnStatus
                    );

                goto  EXIT1;
            }
            else
            {
                returnStatus =
                    pMyObject->SerializeFolder
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSysRoot,
                            pName,
                            (PUCHAR)pCtlvTupleNext,
                            ulCtlvSize - ulTotalSize
                        );
            }

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                AnscTrace("SysCpoSfcSerialize -- failed to serialize the folder, return status = %ld...\n", returnStatus);

                goto  EXIT1;
            }
            else
            {
                ulTotalSize    += SysCtlvGetSize     (pCtlvTupleNext);
                pCtlvTupleNext  = SysCtlvGetNextTuple(pCtlvTupleNext);

                if ( ulTotalSize > ulCtlvSize )
                {
                    AnscTrace
                        (
                            "SysCpoSfcSerialize -- current total size %lu is already bigger than buffer size %lu!!!\n",
                            ulTotalSize,
                            ulCtlvSize
                        );

                    returnStatus = ANSC_STATUS_FAILURE;
                    goto  EXIT1;
                }
            }
        }
    }

    if ( ulTotalSize == ulCtlvSize )
    {
        returnStatus = ANSC_STATUS_SUCCESS;
        *ppBuffer    = (PUCHAR)pCtlvTuple;
        *pulSize     = ulCtlvSize;
    }
    else
    {
        AnscTrace("SysCpoSfcSerialize -- final size %lu doesn't match the CTLV size %lu queried before!!!\n", ulTotalSize, ulCtlvSize);

        returnStatus = ANSC_STATUS_FAILURE;
    }

EXIT1:

    if( hSysRoot != NULL)
    {
        pSysIraIf->CloseFolder(pSysIraIf->hOwnerContext, hSysRoot);
    }

    pSysIraIf->RelWriteAccess(pSysIraIf->hOwnerContext);

EXIT:

    if ( pName )
    {
        AnscFreeMemory(pName);
    }

    AnscTrace("SysCpoSfcSerialize -- return status = %ld...\n", returnStatus);

    return  returnStatus;
}

