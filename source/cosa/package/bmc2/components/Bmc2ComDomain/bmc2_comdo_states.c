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

    module:	bmc2_comdo_states.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Bmc2 Com Domain Object.

        *   Bmc2ComdoGetBmc2ComTerminal
        *   Bmc2ComdoSetBmc2ComTerminal
        *   Bmc2ComdoGetBmc2EnvController
        *   Bmc2ComdoSetBmc2EnvController
        *   Bmc2ComdoGetBmc2ReqController
        *   Bmc2ComdoSetBmc2ReqController
        *   Bmc2ComdoGetDomainTitle
        *   Bmc2ComdoSetDomainTitle
        *   Bmc2ComdoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/27/05    initial revision.

**********************************************************************/


#include "bmc2_comdo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ComdoGetBmc2ComTerminal
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
Bmc2ComdoGetBmc2ComTerminal
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

    return  pMyObject->hBmc2ComTerminal;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoSetBmc2ComTerminal
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComdoSetBmc2ComTerminal
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

    pMyObject->hBmc2ComTerminal = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ComdoGetBmc2EnvController
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
Bmc2ComdoGetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

    return  pMyObject->hBmc2EnvController;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoSetBmc2EnvController
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEnvController
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEnvController
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComdoSetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEnvController
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

    pMyObject->hBmc2EnvController = hEnvController;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ComdoGetBmc2ReqController
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
Bmc2ComdoGetBmc2ReqController
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

    return  pMyObject->hBmc2ReqController;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoSetBmc2ReqController
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqController
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReqController
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComdoSetBmc2ReqController
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqController
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

    pMyObject->hBmc2ReqController = hReqController;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        Bmc2ComdoGetDomainTitle
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

char*
Bmc2ComdoGetDomainTitle
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

    return  pMyObject->DomainTitle;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoSetDomainTitle
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pDomainTitle
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pDomainTitle
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComdoSetDomainTitle
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDomainTitle
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

    if ( pMyObject->DomainTitle )
    {
        AnscFreeMemory(pMyObject->DomainTitle);

        pMyObject->DomainTitle = NULL;
    }

    pMyObject->DomainTitle = AnscCloneString(pDomainTitle);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoReset
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
Bmc2ComdoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

    pMyObject->DelAllCommandProperties((ANSC_HANDLE)pMyObject);

    if ( pMyObject->DomainTitle )
    {
        AnscFreeMemory(pMyObject->DomainTitle);

        pMyObject->DomainTitle = NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
