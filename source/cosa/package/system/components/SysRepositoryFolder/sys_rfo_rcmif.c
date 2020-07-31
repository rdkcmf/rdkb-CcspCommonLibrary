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

    module:	sys_rfo_rcmif.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Sys Repository Folder Object.

        *   SysRfoRcmNotify
        *   SysRfoRcmInit
        *   SysRfoRcmStop

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/20/02    initial revision.

**********************************************************************/


#include "sys_rfo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoRcmNotify
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder,
                ULONG                       ulEvent,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to inform the interested parties when
        something interesting happens.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the Repository Folder Object with which the
                indicated event is associated.

                ULONG                       ulEvent
                Specifies the event to be informed.

                ANSC_HANDLE                 hReserved
                Specifies the event-specific context handle.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoRcmNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_RCM_INTERFACE              pOpenRcmIf   = (PSYS_RCM_INTERFACE             )pMyObject->hOpenRcmIf;

    if ( pOpenRcmIf )
    {
        returnStatus =
            pOpenRcmIf->Notify
                (
                    pOpenRcmIf->hOwnerContext,
                    hFolder,
                    ulEvent,
                    hReserved
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoRcmInit
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder
            );

    description:

        This function is called when the folder is opened for the
        first time.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the Repository Folder Object whose content is
                to be checked.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoRcmInit
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_RCM_INTERFACE              pOpenRcmIf   = (PSYS_RCM_INTERFACE             )pMyObject->hOpenRcmIf;

    if ( pOpenRcmIf )
    {
        returnStatus =
            pOpenRcmIf->Init
                (
                    pOpenRcmIf->hOwnerContext,
                    hFolder
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoRcmStop
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder
            );

    description:

        This function is called when the folder is closed by the last
        user who is done accessing the folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the Repository Folder Object on which the
                management session is to be closed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoRcmStop
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_RCM_INTERFACE              pOpenRcmIf   = (PSYS_RCM_INTERFACE             )pMyObject->hOpenRcmIf;

    if ( pOpenRcmIf )
    {
        returnStatus =
            pOpenRcmIf->Stop
                (
                    pOpenRcmIf->hOwnerContext,
                    hFolder
                );
    }

    return  returnStatus;
}
