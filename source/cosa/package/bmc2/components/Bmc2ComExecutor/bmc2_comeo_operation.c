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

    module:	bmc2_comeo_operation.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Bmc2 Com Executor
        Object.

        *   Bmc2ComeoEngage
        *   Bmc2ComeoCancel
        *   Bmc2ComeoSetupEnv
        *   Bmc2ComeoCloseEnv

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


/**********************************************************************

    prototype:

        ANSC_STATUS
        Bmc2ComeoEngage
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to engage this object.

    argument:   ANSC_HANDLE                 hThisObject
                This context handle is transparent to the BMC2 COMEO
                object.

    return:     status of the operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComeoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_EXECUTOR_OBJECT       pMyObject       = (PBMC2_COM_EXECUTOR_OBJECT)hThisObject;

    if (!pMyObject->bActive)
    {
        pMyObject->bActive  = TRUE;
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    prototype:

        ANSC_STATUS
        Bmc2ComeoCancel
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to cancel this object.

    argument:   ANSC_HANDLE                 hThisObject
                This context handle is transparent to the BMC2 COMEO
                object.

    return:     status of the operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComeoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_EXECUTOR_OBJECT       pMyObject       = (PBMC2_COM_EXECUTOR_OBJECT)hThisObject;

    if (pMyObject->bActive)
    {
        pMyObject->bActive  = FALSE;
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    prototype:

        ANSC_STATUS
        Bmc2ComeoSetupEnv
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to set up env.

    argument:   ANSC_HANDLE                 hThisObject
                This context handle is transparent to the BMC2 COMEO
                object.

    return:     status of the operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComeoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    prototype:

        ANSC_STATUS
        Bmc2ComeoCloseEnv
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to close env.

    argument:   ANSC_HANDLE                 hThisObject
                This context handle is transparent to the BMC2 COMEO
                object.

    return:     status of the operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComeoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return ANSC_STATUS_SUCCESS;
}


