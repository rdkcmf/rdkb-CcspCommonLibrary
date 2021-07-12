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

    module:	bwrm_fmo_management.c

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Bwrm File Manager Object.

        *   BwrmFmoGetFile
        *   BwrmFmoSetFile
        *   BwrmFmoAddFile
        *   BwrmFmoDelFile

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/01/03    initial revision.

**********************************************************************/


#include "bwrm_fmo_global.h"
#include "bree_spo_srm.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmFmoGetFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path,
                char*                       file_path,
                void**                      file_data,
                PULONG                      file_size
            );

    description:

        This function is called to retrieve raw data from the specified
        file.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies the root_path of the file to be retrieved.

                char*                       file_path
                Specifies the file_path of the file to be retrieved.

                void**                      file_data
                Specifies the raw file data to be returned.

                PULONG                      file_size
                Specifies the raw file size to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmFmoGetFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path,
        void**                      file_data,
        PULONG                      file_size
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    ANSC_HANDLE                     hAnscFile    = (ANSC_HANDLE                )NULL;
    char*                           pFileName    = (char*                      )NULL;
    ULONG                           ulFileSize   = (ULONG                      )0;
    PVOID                           pFileData    = (PVOID                      )NULL;

    *file_data = NULL;
    *file_size = 0;
    errno_t rc = -1;

    size_t len = strlen(root_path) + strlen(file_path) + 2;
    pFileName = (char*)AnscAllocateMemory(len);

    if ( !pFileName )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    rc = sprintf_s(pFileName, len, "%s%s", root_path, file_path );
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

#ifdef   _BREE_SPO_USE_SRMO
    hAnscFile =
        (ANSC_HANDLE)BreeSpoCookedResOpen
            (
                (const char*)pFileName,
                ANSC_FILE_MODE_READ,
                ANSC_FILE_TYPE_READ
            );
#else
    hAnscFile =
        AnscOpenFile
            (
                pFileName,
                ANSC_FILE_MODE_READ | ANSC_FILE_MODE_ZLIB_COMPRESSED,
                ANSC_FILE_TYPE_READ
            );
#endif

    if ( !hAnscFile )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT1;
    }
    else
    {
#ifdef   _BREE_SPO_USE_SRMO
        ulFileSize = BreeSpoGetCookedResLength((int)hAnscFile);
#else
        /*CID: 59651 Improper use of negative value- Added negative check in def part*/
        ulFileSize = AnscGetFileSize(hAnscFile);
#endif

        pFileData  = (PVOID)AnscAllocateMemory(ulFileSize + 16);
    }

    if ( !pFileData )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT2;
    }
    else
    {
#ifdef   _BREE_SPO_USE_SRMO
        ulFileSize =
            BreeSpoCookedResRead
                (
                    (int)hAnscFile,
                    pFileData,
                    ulFileSize
                );

        returnStatus = ANSC_STATUS_SUCCESS;
#else
        returnStatus =
            AnscReadFile
                (
                    hAnscFile,
                    pFileData,
                    &ulFileSize
                );
#endif
    }

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        AnscFreeMemory(pFileData); /*RDKB-5888 , CID-24282, free resources*/
        goto  EXIT2;
    }
    else
    {
        *file_data = pFileData;
        *file_size = ulFileSize;

        returnStatus = ANSC_STATUS_SUCCESS;
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( hAnscFile )
    {
#ifdef   _BREE_SPO_USE_SRMO
        BreeSpoCookedResClose((int)hAnscFile);
#else
        AnscCloseFile(hAnscFile);
#endif
    }

EXIT1:

    if ( pFileName )
    {
        AnscFreeMemory(pFileName);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmFmoSetFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path,
                char*                       file_path,
                void*                       file_data,
                ULONG                       file_size
            );

    description:

        This function is called to write raw data into the specified
        file.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies the root_path of the file to be written.

                char*                       file_path
                Specifies the file_path of the file to be written.

                void*                       file_data
                Specifies the raw file data to be written.

                ULONG                       file_size
                Specifies the raw file size to be written.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmFmoSetFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path,
        void*                       file_data,
        ULONG                       file_size
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    ANSC_HANDLE                     hAnscFile    = (ANSC_HANDLE                )NULL;
    char*                           pFileName    = (char*                      )NULL;
    errno_t   rc = -1;

    size_t len = strlen(root_path) + strlen(file_path) + 2;
    pFileName = (char*)AnscAllocateMemory(len);

    if ( !pFileName )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    rc = sprintf_s(pFileName, len, "%s%s", root_path, file_path );
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

    hAnscFile =
        AnscOpenFile
            (
                pFileName,
                ANSC_FILE_MODE_WRITE | ANSC_FILE_MODE_TRUNC | ANSC_FILE_MODE_ZLIB_COMPRESSED,
                ANSC_FILE_TYPE_WRITE
            );

    if ( !hAnscFile )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT1;
    }
    else
    {
        returnStatus =
            AnscWriteFile
                (
                    hAnscFile,
                    file_data,
                    &file_size
                );

        goto  EXIT2;
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( hAnscFile )
    {
        AnscCloseFile(hAnscFile);
    }

EXIT1:

    if ( pFileName )
    {
        AnscFreeMemory(pFileName);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmFmoNewFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path,
                char*                       file_path,
                void*                       file_data,
                ULONG                       file_size
            );

    description:

        This function is called to write raw data into the specified
        file.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies the root_path of the file to be written.

                char*                       file_path
                Specifies the file_path of the file to be written.

                void*                       file_data
                Specifies the raw file data to be written.

                ULONG                       file_size
                Specifies the raw file size to be written.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmFmoNewFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path,
        void*                       file_data,
        ULONG                       file_size
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    ANSC_HANDLE                     hAnscFile    = (ANSC_HANDLE                )NULL;
    char*                           pFileName    = (char*                      )NULL;
    errno_t         rc   = -1;

    size_t len = strlen(root_path) + strlen(file_path) + 2;
    pFileName = (char*)AnscAllocateMemory(len);

    if ( !pFileName )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    rc = sprintf_s(pFileName, len, "%s%s", root_path, file_path );
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

    hAnscFile =
        AnscOpenFile
            (
                pFileName,
                ANSC_FILE_MODE_WRITE | ANSC_FILE_MODE_CREATE | ANSC_FILE_MODE_APPEND | ANSC_FILE_MODE_ZLIB_COMPRESSED,
                ANSC_FILE_TYPE_WRITE
            );

    if ( !hAnscFile )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT1;
    }
    else
    {
        returnStatus =
            AnscWriteFile
                (
                    hAnscFile,
                    file_data,
                    &file_size
                );

        goto  EXIT2;
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( hAnscFile )
    {
        AnscCloseFile(hAnscFile);
    }

EXIT1:

    if ( pFileName )
    {
        AnscFreeMemory(pFileName);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmFmoDelFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path,
                char*                       file_path
            );

    description:

        This function is called to delete the specified file.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies the root_path of the file to be deleted.

                char*                       file_path
                Specifies the file_path of the file to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmFmoDelFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    char*                           pFileName    = (char*                      )NULL;
    errno_t    rc = -1;

    size_t len = strlen(root_path) + strlen(file_path) + 2;
    pFileName = (char*)AnscAllocateMemory(len);

    if ( !pFileName )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    rc = sprintf_s(pFileName, len, "%s%s", root_path, file_path );
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pFileName )
    {
        AnscFreeMemory(pFileName);
    }

    return  returnStatus;
}
