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

    module:	slap_eco_bssif.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Slap Env Controller Object.

        *   SlapEcoBssGetPoolingFlag
        *   SlapEcoBssSetPoolingFlag
        *   SlapEcoBssGetDefPoolSize
        *   SlapEcoBssSetDefPoolSize
        *   SlapEcoBssGetVarConverter
        *   SlapEcoBssGetVarMapper

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/11/03    initial revision.

**********************************************************************/


#include "slap_eco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapEcoBssGetPoolingFlag
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a system configuration
        setting.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     configuration setting.

**********************************************************************/

BOOL
SlapEcoBssGetPoolingFlag
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;
    PSLAP_ENV_CONTROLLER_PROPERTY   pProperty         = (PSLAP_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    return  pProperty->bPoolingEnabled;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoBssSetPoolingFlag
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bPooling
            );

    description:

        This function is called to configure a system configuration
        setting.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bPooling
                Specifies the configuration setting to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoBssSetPoolingFlag
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bPooling
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;
    PSLAP_ENV_CONTROLLER_PROPERTY   pProperty         = (PSLAP_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    pProperty->bPoolingEnabled = bPooling;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapEcoBssGetDefPoolSize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a system configuration
        setting.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     configuration setting.

**********************************************************************/

ULONG
SlapEcoBssGetDefPoolSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;
    PSLAP_ENV_CONTROLLER_PROPERTY   pProperty         = (PSLAP_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    return  pProperty->ObjectPoolSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoBssSetDefPoolSize
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulPoolSize
            );

    description:

        This function is called to configure a system configuration
        setting.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulPoolSize
                Specifies the configuration setting to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoBssSetDefPoolSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPoolSize
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;
    PSLAP_ENV_CONTROLLER_PROPERTY   pProperty         = (PSLAP_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    pProperty->ObjectPoolSize = ulPoolSize;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoBssGetVarConverter
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a system service object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     service object.

**********************************************************************/

ANSC_HANDLE
SlapEcoBssGetVarConverter
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;

    return  pMyObject->hSlapVarConverter;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoBssGetVarMapper
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a system service object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     service object.

**********************************************************************/

ANSC_HANDLE
SlapEcoBssGetVarMapper
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;

    return  pMyObject->hSlapVarMapper;
}
