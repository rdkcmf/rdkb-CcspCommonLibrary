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

    module:	slap_lbo_states.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Slap Loam Broker Object.

        *   SlapLboGetAnscCpcIf
        *   SlapLboGetAnscLpcConnector
        *   SlapLboGetProperty
        *   SlapLboSetProperty
        *   SlapLboResetProperty
        *   SlapLboReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/03/03    initial revision.

**********************************************************************/


#include "slap_lbo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapLboGetAnscCpcIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapLboGetAnscCpcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;

    return  pMyObject->hAnscCpcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapLboGetAnscLpcConnector
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
SlapLboGetAnscLpcConnector
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;

    return  pMyObject->hAnscLpcConnector;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboGetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to retrieve object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;
    PSLAP_LOAM_BROKER_PROPERTY      pProperty    = (PSLAP_LOAM_BROKER_PROPERTY)&pMyObject->Property;

    *(PSLAP_LOAM_BROKER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboSetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to configure object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be copied.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;
    PSLAP_LOAM_BROKER_PROPERTY      pProperty    = (PSLAP_LOAM_BROKER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PSLAP_LOAM_BROKER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboResetProperty
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;
    PSLAP_LOAM_BROKER_PROPERTY      pProperty    = (PSLAP_LOAM_BROKER_PROPERTY)&pMyObject->Property;

    pProperty->MyAddress.Value  = IPV4_LOOPBACK_ADDRESS;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_BROKER_OBJECT        pMyObject    = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;

    pMyObject->DelAllObjBindings((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
