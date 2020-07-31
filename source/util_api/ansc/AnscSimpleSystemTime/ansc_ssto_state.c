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

    module:	ansc_ssto_state.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Simple System Time Object.

        *   AnscSstoGetStsIf       
        *   AnscSstoReset          
        *   AnscSstoGetLastSecond  
        *   AnscSstoGetLastTick    
        *   AnscSstoGetiTimeOffset 
        *   AnscSstoSetiTimeOffset 
        *   AnscSstoAcquireAccess  
        *   AnscSstoReleaseAccess 
        *   AnscSstoGetCurrSecond

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Na Li

    ---------------------------------------------------------------

    revision:

        09/22/01    Xuechen Yang    template
        11/25/02    Na Li           initial revision

**********************************************************************/


#include "ansc_ssto_global.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscSstoGetStsIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to get the handle of the StsIf.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     Sts interface handle.

**********************************************************************/

ANSC_HANDLE
AnscSstoGetStsIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT         pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;


    /*
    AnscTrace("AnscSstoGetStsIf...!\n");
    */

    return  pMyObject->hStsIf;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset the internal states.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;


    /*
    AnscTrace("AnscSstoReset...!\n");
    */
    pMyObject->AcquireAccess(hThisObject);

    pMyObject->LastTick    = 0;
    pMyObject->LastSecond  = 0;
    pMyObject->iTimeOffset = 0;

    pMyObject->ReleaseAccess(hThisObject);


    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscSstoGetLastSecond
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to return the lastsecond, which is set 
        by adjust time functions or get time functions.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
AnscSstoGetLastSecond
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;

    /*
    AnscTrace("AnscSstoGetLastSecond...!\n");
    */
    return  pMyObject->LastSecond;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscSstoGetLastTick
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to return the lastsecond, which is set 
        by adjust time functions or get time functions.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
AnscSstoGetLastTick
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;

    /*
    AnscTrace("AnscSstoGetLastTick...!\n");
    */


    return  pMyObject->LastTick;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        INT
        AnscSstoGetiTimeOffset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to return the lastsecond, which is set 
        by adjust time functions or get time functions.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

INT
AnscSstoGetiTimeOffset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;

    /*
    AnscTrace("AnscSstoGetiTimeOffset...!\n");
    */


    return  pMyObject->iTimeOffset;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoSetiTimeOffset
            (
                ANSC_HANDLE                 hThisObject,
                INT                         value
            );

    description:

        This function is called to set the timeoffset in seconds
        of this location from the GMT.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.


                INT                         value
                Specifies the value to set

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoSetiTimeOffset
    (
        ANSC_HANDLE                 hThisObject,
        INT                         value
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;

    pMyObject->AcquireAccess(hThisObject);

    pMyObject->iTimeOffset = value;

    pMyObject->ReleaseAccess(hThisObject);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoAcquireAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to get the object access lock.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.


    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject

    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;

    AnscAcquireLock(&pMyObject->AccessLock);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSstoReleaseAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release the object access lock.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.


    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSstoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject

    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;

    AnscReleaseLock(&pMyObject->AccessLock);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscSstoGetCurrSecond
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to get the current seonds since 
        1900-01-01 00:00:00. It is independent on the timezone
        and daylight saving. This function also adjusts the
        LastSecond and LastTick.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.


    return:     current seconds.

**********************************************************************/

ULONG
AnscSstoGetCurrSecond
    (
        ANSC_HANDLE                 hThisObject

    )
{
    PANSC_SIMPLE_SYS_TIME_OBJECT    pMyObject         = (PANSC_SIMPLE_SYS_TIME_OBJECT)hThisObject;
    ULONG                           ulCurrTick        = AnscGetTickInSeconds();
    ULONG                           ulCurrSecond      = 0;

    ulCurrSecond = ulCurrTick - pMyObject->LastTick + pMyObject->LastSecond;

    AnscAcquireLock(&pMyObject->AccessLock);

    pMyObject->LastTick    = ulCurrTick;
    pMyObject->LastSecond  = ulCurrSecond;

    AnscReleaseLock(&pMyObject->AccessLock);

    return ulCurrSecond;
}

