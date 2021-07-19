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

    module:	slap_spo_states.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Slap Srv Proxy Object.

        *   SlapSpoGetCallTimeout
        *   SlapSpoSetCallTimeout
        *   SlapSpoGetObjName
        *   SlapSpoSetObjName
        *   SlapSpoGetObjType
        *   SlapSpoSetObjType
        *   SlapSpoGetObjMode
        *   SlapSpoSetObjMode
        *   SlapSpoGetSlapGoaObj
        *   SlapSpoSetSlapGoaObj
        *   SlapSpoGetSlapUoaIf
        *   SlapSpoSetSlapUoaIf
        *   SlapSpoGetSlapGoaIf
        *   SlapSpoSetSlapGoaIf
        *   SlapSpoGetSlapPoaIf
        *   SlapSpoSetSlapPoaIf
        *   SlapSpoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/06/03    initial revision.

**********************************************************************/


#include "slap_spo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapSpoGetCallTimeout
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

ULONG
SlapSpoGetCallTimeout
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    return  pMyObject->CallTimeout;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoSetCallTimeout
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       timeout
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       timeout
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapSpoSetCallTimeout
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timeout
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    pMyObject->CallTimeout = timeout;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SlapSpoGetObjName
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
SlapSpoGetObjName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    return  pMyObject->ObjName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoSetObjName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapSpoSetObjName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    if ( pMyObject->ObjName )
    {
        AnscFreeMemory(pMyObject->ObjName);

        pMyObject->ObjName = NULL;
    }

    if ( obj_name )
    {
        pMyObject->ObjName = strdup(obj_name);
    }
    else
    {
        pMyObject->ObjName = (char*)AnscAllocateMemory(1);
        pMyObject->ObjName[0] = '\0';
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapSpoGetObjType
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

ULONG
SlapSpoGetObjType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    return  pMyObject->ObjType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoSetObjType
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       obj_type
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       obj_type
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapSpoSetObjType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       obj_type
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    pMyObject->ObjType = obj_type;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapSpoGetObjMode
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

ULONG
SlapSpoGetObjMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    return  pMyObject->ObjMode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoSetObjMode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       obj_mode
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       obj_mode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapSpoSetObjMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       obj_mode
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    pMyObject->ObjMode = obj_mode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapSpoGetSlapGoaObj
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
SlapSpoGetSlapGoaObj
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    return  pMyObject->hSlapGoaObj;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoSetSlapGoaObj
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapGoaObj
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapGoaObj
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapSpoSetSlapGoaObj
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapGoaObj
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    pMyObject->hSlapGoaObj = hSlapGoaObj;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapSpoGetSlapUoaIf
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
SlapSpoGetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    return  pMyObject->hSlapUoaIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoSetSlapUoaIf
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
SlapSpoSetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    pMyObject->hSlapUoaIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapSpoGetSlapGoaIf
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
SlapSpoGetSlapGoaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    return  pMyObject->hSlapGoaIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoSetSlapGoaIf
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
SlapSpoSetSlapGoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    pMyObject->hSlapGoaIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapSpoGetSlapPoaIf
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
SlapSpoGetSlapPoaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    return  pMyObject->hSlapPoaIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoSetSlapPoaIf
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
SlapSpoSetSlapPoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;

    pMyObject->hSlapPoaIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoReset
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
SlapSpoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}
