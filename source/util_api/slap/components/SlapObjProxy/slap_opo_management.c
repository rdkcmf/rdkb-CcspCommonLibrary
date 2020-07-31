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

    module:	slap_opo_management.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Slap Obj Proxy Object.

        *   SlapOpoNewSrvProxy

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


#include "slap_opo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOpoNewSrvProxy
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapGoaObj
            );

    description:

        This function is called to create a SLAP SrvProxy object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     srv proxy object.

**********************************************************************/

ANSC_HANDLE
SlapOpoNewSrvProxy
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapGoaObj
    )
{
    PSLAP_OBJ_PROXY_OBJECT          pMyObject        = (PSLAP_OBJ_PROXY_OBJECT )hThisObject;
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor   = (PSLAP_OBJECT_DESCRIPTOR)pMyObject->hObjDescriptor;
    PFN_ANSCLCO_CREATE              pfnSlapSpoCreate = (PFN_ANSCLCO_CREATE     )NULL;
    PSLAP_SRV_PROXY_OBJECT          pSlapSrvProxy    = (PSLAP_SRV_PROXY_OBJECT )NULL;

    if ( !pObjDescriptor )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        if ( TRUE )
        {
            pfnSlapSpoCreate = (PFN_ANSCLCO_CREATE)pObjDescriptor->ObjConstructor;
            pSlapSrvProxy    =
                (PSLAP_SRV_PROXY_OBJECT)pfnSlapSpoCreate
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );
        }

        if ( !pSlapSrvProxy )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            pSlapSrvProxy->SetObjName   ((ANSC_HANDLE)pSlapSrvProxy, pObjDescriptor->ObjName);
            pSlapSrvProxy->SetObjType   ((ANSC_HANDLE)pSlapSrvProxy, pObjDescriptor->ObjType);
            pSlapSrvProxy->SetSlapGoaObj((ANSC_HANDLE)pSlapSrvProxy, hSlapGoaObj            );
            pSlapSrvProxy->SetSlapUoaIf ((ANSC_HANDLE)pSlapSrvProxy, pMyObject->hSlapUoaIf  );
            pSlapSrvProxy->SetSlapGoaIf ((ANSC_HANDLE)pSlapSrvProxy, (ANSC_HANDLE)NULL      );
            pSlapSrvProxy->SetSlapPoaIf ((ANSC_HANDLE)pSlapSrvProxy, (ANSC_HANDLE)NULL      );
            pSlapSrvProxy->Engage       ((ANSC_HANDLE)pSlapSrvProxy);
        }
    }

    return  (ANSC_HANDLE)pSlapSrvProxy;
}
