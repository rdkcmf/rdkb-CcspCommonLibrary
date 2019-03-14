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

    module:	web_gso_operation.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Web General Session Object.

        *   WebGsoAcquireAccess
        *   WebGsoReleaseAccess
        *   WebGsoEngage
        *   WebGsoCancel
        *   WebGsoTerminate
        *   WebGsoLock
        *   WebGsoUnlock
        *   WebGsoResetTimestamp

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


#include "web_gso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoAcquireAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire the access to the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    AnscAcquireLock(&pMyObject->SyncLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoReleaseAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release the access to the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    AnscReleaseLock(&pMyObject->SyncLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to enage the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;

        /* reset timestamp */
        pMyObject->ResetTimestamp((ANSC_HANDLE)pMyObject);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoTerminate
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to terminate the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoTerminate
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    pMyObject->LsmMaxAge = 0;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoLock
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to lock the session.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoLock
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PWEB_GENERAL_SESSION_OBJECT     pMyObject     = (PWEB_GENERAL_SESSION_OBJECT   )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pActiveSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hActiveSocket;

    pMyObject->bLocked = TRUE;

    if ( pActiveSocket )
    {
        pActiveSocket->Lock((ANSC_HANDLE)pActiveSocket);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoUnlock
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to unlock the session.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoUnlock
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PWEB_GENERAL_SESSION_OBJECT     pMyObject     = (PWEB_GENERAL_SESSION_OBJECT   )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pActiveSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hActiveSocket;

    pMyObject->bLocked = FALSE;

    if ( pActiveSocket )
    {
        pActiveSocket->Unlock((ANSC_HANDLE)pActiveSocket);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoResetTimestamp
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset the session timestamp.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoResetTimestamp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PWEB_GENERAL_SESSION_OBJECT     pMyObject     = (PWEB_GENERAL_SESSION_OBJECT   )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pActiveSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hActiveSocket;

    pMyObject->TimeStampInSec = AnscGetTickInSecondsAbs();
    pMyObject->LastReqAtInSec = pMyObject->TimeStampInSec;
    pMyObject->LastRepAtInSec = pMyObject->TimeStampInSec;

    if ( pActiveSocket )
    {
        pActiveSocket->ResetTimestamp((ANSC_HANDLE)pActiveSocket);
    }

    return  returnStatus;
}
