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

    module:	sys_rrobool_access.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced field-access functions
        of the Sys Rro Bool Object.

        *   SysRroBoolGetRecordData
        *   SysRroBoolSetRecordData
        *   SysRroBoolGetRecordValue
        *   SysRroBoolSetRecordValue

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


#include "sys_rrobool_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroBoolGetRecordData
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
SysRroBoolGetRecordData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pDataBuffer,
        PULONG                      pulDataSize
    )
{
    PSYS_RRO_BOOL_OBJECT            pMyObject    = (PSYS_RRO_BOOL_OBJECT)hThisObject;

    if ( *pulDataSize < sizeof(BOOL) )
    {
        return  ANSC_STATUS_BAD_SIZE;
    }

    *(BOOL*)pDataBuffer = pMyObject->RecordValue;
    *pulDataSize        = sizeof(BOOL);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroBoolSetRecordData
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
SysRroBoolSetRecordData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pDataBuffer,
        ULONG                       ulDataSize
    )
{
    PSYS_RRO_BOOL_OBJECT            pMyObject    = (PSYS_RRO_BOOL_OBJECT)hThisObject;

    if ( ulDataSize != sizeof(BOOL) )
    {
        return  ANSC_STATUS_BAD_SIZE;
    }

    pMyObject->RecordValue = *(BOOL*)pDataBuffer;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SysRroBoolGetRecordValue
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the record data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     record value.

**********************************************************************/

BOOL
SysRroBoolGetRecordValue
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_RRO_BOOL_OBJECT            pMyObject    = (PSYS_RRO_BOOL_OBJECT)hThisObject;

    return  pMyObject->RecordValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRroBoolSetRecordValue
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        value
            );

    description:

        This function is called to configure the record data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        value
                Specifies the record value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRroBoolSetRecordValue
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        value
    )
{
    PSYS_RRO_BOOL_OBJECT            pMyObject    = (PSYS_RRO_BOOL_OBJECT)hThisObject;

    pMyObject->RecordValue = value;

    return  ANSC_STATUS_SUCCESS;
}
