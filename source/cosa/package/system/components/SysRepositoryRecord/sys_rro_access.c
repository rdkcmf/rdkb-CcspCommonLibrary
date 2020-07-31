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

    module:	sys_rro_access.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced field-access functions
        of the Sys Repository Record Object.

        *   SysRroGrantAccess
        *   SysRroGetRecordSize
        *   SysRroGetRecordData
        *   SysRroSetRecordData

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/07/02    initial revision.

**********************************************************************/


#include "sys_rro_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SysRroGrantAccess
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulAccessMode
            );

    description:

        This function is called to grant access to a request.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulAccessMode
                Specifies the access mode associated with the request.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SysRroGrantAccess
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulAccessMode
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;
    ULONG                           ulTmpMode    = ulAccessMode;

    ulTmpMode  = ulAccessMode & 0x0000FFFF;
    ulTmpMode &= pMyObject->Permission;

    if ( ulTmpMode != (ulAccessMode & 0x0000FFFF) )
    {
        return  FALSE;
    }

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRroGetRecordSize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the record size.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     record size.

**********************************************************************/

ULONG
SysRroGetRecordSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_RECORD_OBJECT   pMyObject    = (PSYS_REPOSITORY_RECORD_OBJECT)hThisObject;

    return  pMyObject->RecordSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroGetRecordData
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       pDataBuffer,
                PULONG                      pulDataSize
            );

    description:

        This function is called to retrieve the record data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       pDataBuffer
                Specifies the buffer holding the record data.

                PULONG                      pulDataSize
                Specifies the size of the record data buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRroGetRecordData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pDataBuffer,
        PULONG                      pulDataSize
    )
{
    UNREFERENCED_PARAMETER(pulDataSize);
    UNREFERENCED_PARAMETER(pDataBuffer);
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroSetRecordData
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       pDataBuffer,
                ULONG                       ulDataSize
            );

    description:

        This function is called to configure the record data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       pDataBuffer
                Specifies the buffer holding the record data.

                ULONG                       ulDataSize
                Specifies the size of the record data buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRroSetRecordData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pDataBuffer,
        ULONG                       ulDataSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pDataBuffer);
    UNREFERENCED_PARAMETER(ulDataSize);
    return  ANSC_STATUS_UNAPPLICABLE;
}
