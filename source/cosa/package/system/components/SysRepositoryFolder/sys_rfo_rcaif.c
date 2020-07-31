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

    module:	sys_rfo_rcaif.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Sys Repository Folder Object.

        *   SysRfoRcaNotify
        *   SysRfoRcaReview

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


#include "sys_rfo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoRcaNotify
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
SysRfoRcaNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_RCA_INTERFACE              pOpenRcaIf   = (PSYS_RCA_INTERFACE             )pMyObject->hOpenRcaIf;

    if ( pOpenRcaIf )
    {
        returnStatus =
            pOpenRcaIf->Notify
                (
                    pOpenRcaIf->hOwnerContext,
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
        SysRfoRcaReview
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder
            );

    description:

        This function is called to inform the interested parties when
        a content-review is requested.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies the Repository Folder Object with which the
                the content-review should be done.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoRcaReview
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_RCA_INTERFACE              pOpenRcaIf   = (PSYS_RCA_INTERFACE             )pMyObject->hOpenRcaIf;

    if ( pOpenRcaIf )
    {
        returnStatus =
            pOpenRcaIf->Review
                (
                    pOpenRcaIf->hOwnerContext,
                    hFolder
                );
    }

    return  returnStatus;
}
