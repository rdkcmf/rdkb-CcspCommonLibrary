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

    module:	bmc2_comeo_state.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Bmc2 Com Executor
        Object.

        *   Bmc2ComeoGetBmc2EnvController
        *   Bmc2ComeoSetBmc2EnvController
        *   Bmc2ComeoGetProperty
        *   Bmc2ComeoSetProperty
        *   Bmc2ComeoResetProperty
        *   Bmc2ComeoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/28/05    initial revision.

**********************************************************************/


#include "bmc2_comeo_global.h"


ANSC_HANDLE
Bmc2ComeoGetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_EXECUTOR_OBJECT       pMyObject   = (PBMC2_COM_EXECUTOR_OBJECT  )hThisObject;

    return pMyObject->hBmc2EnvController;
}


ANSC_STATUS
Bmc2ComeoSetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PBMC2_COM_EXECUTOR_OBJECT       pMyObject   = (PBMC2_COM_EXECUTOR_OBJECT  )hThisObject;

    pMyObject->hBmc2EnvController   = hContext;

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
Bmc2ComeoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PBMC2_COM_EXECUTOR_OBJECT       pMyObject   = (PBMC2_COM_EXECUTOR_OBJECT  )hThisObject;
    PBMC2_COM_EXECUTOR_PROPERTY     pProperty   = &pMyObject->Property;
    PBMC2_COM_EXECUTOR_PROPERTY     pNewProperty= (PBMC2_COM_EXECUTOR_PROPERTY)hProperty;

    if ( pNewProperty )
    {
        *pNewProperty   = *pProperty;
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
Bmc2ComeoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PBMC2_COM_EXECUTOR_OBJECT       pMyObject   = (PBMC2_COM_EXECUTOR_OBJECT  )hThisObject;
    PBMC2_COM_EXECUTOR_PROPERTY     pProperty   = &pMyObject->Property;
    PBMC2_COM_EXECUTOR_PROPERTY     pNewProperty= (PBMC2_COM_EXECUTOR_PROPERTY)hProperty;

    if ( pNewProperty )
    {
        *pProperty      = *pNewProperty;
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
Bmc2ComeoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
Bmc2ComeoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return ANSC_STATUS_SUCCESS;
}


