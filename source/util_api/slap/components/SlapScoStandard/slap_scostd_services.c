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

    module:	slap_scostd_services.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced services functions of
        the Slap Sco Standard Object.

        *   SlapScoStdCreateObject
        *   SlapScoStdCreateObject2
        *   SlapScoStdDeleteObject
        *   SlapScoStdCloneObject

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/15/03    initial revision.

**********************************************************************/


#include "slap_scostd_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_OBJECT
        SlapScoStdCreateObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to create the specified service object
        within the same container.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the slap service object to be created.

    return:     object handle.

**********************************************************************/

SLAP_OBJECT
SlapScoStdCreateObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_SCO_STANDARD_OBJECT       pMyObject    = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf   = (PSLAP_UOA_INTERFACE      )pMyObject->hSlapUoaIf;
    ANSC_HANDLE                     hSlapObject  = (ANSC_HANDLE              )NULL;

    hSlapObject =
        pSlapUoaIf->CreateObject
            (
                pSlapUoaIf->hOwnerContext,
                pMyObject->hSlapObjContainer,
                obj_name,
                (ANSC_HANDLE)NULL
            );

    return  hSlapObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_OBJECT
        SlapScoStdCreateObject2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name,
                SLAP_HANDLE                 hInsContext
            );

    description:

        This function is called to create the specified service object
        within the same container.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the slap service object to be created.

                SLAP_HANDLE                 hInsContext
                Specifies the runtime instance context to be configured.

    return:     object handle.

**********************************************************************/

SLAP_OBJECT
SlapScoStdCreateObject2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        SLAP_HANDLE                 hInsContext
    )
{
    PSLAP_SCO_STANDARD_OBJECT       pMyObject    = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf   = (PSLAP_UOA_INTERFACE      )pMyObject->hSlapUoaIf;
    ANSC_HANDLE                     hSlapObject  = (ANSC_HANDLE              )NULL;

    hSlapObject =
        pSlapUoaIf->CreateObject
            (
                pSlapUoaIf->hOwnerContext,
                pMyObject->hSlapObjContainer,
                obj_name,
                hInsContext
            );

    return  hSlapObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoStdDeleteObject
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_OBJECT                 hSlapObject
            );

    description:

        This function is called to delete the specified service object
        from the same container.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_OBJECT                 hSlapObject
                Specifies the slap object to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapScoStdDeleteObject
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_OBJECT                 hSlapObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_SCO_STANDARD_OBJECT       pMyObject    = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf   = (PSLAP_UOA_INTERFACE      )pMyObject->hSlapUoaIf;

    returnStatus =
        pSlapUoaIf->DeleteObject
            (
                pSlapUoaIf->hOwnerContext,
                hSlapObject
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_OBJECT
        SlapScoStdCreateObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name,
                SLAP_HANDLE                 hInsContext
            );

    description:

        This function is called to clone this service object within the
        same container.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object handle.

**********************************************************************/

SLAP_OBJECT
SlapScoStdCloneObject
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SCO_STANDARD_OBJECT       pMyObject    = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf   = (PSLAP_UOA_INTERFACE      )pMyObject->hSlapUoaIf;
    ANSC_HANDLE                     hSlapObject  = (ANSC_HANDLE              )NULL;

    hSlapObject =
        pSlapUoaIf->CreateObject
            (
                pSlapUoaIf->hOwnerContext,
                pMyObject->hSlapObjContainer,
                pMyObject->ObjName,
                pMyObject->hInsContext
            );

    return  hSlapObject;
}
