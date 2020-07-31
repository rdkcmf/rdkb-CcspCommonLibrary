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

    module:	slap_spo_operation.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Slap Srv Proxy Object.

        *   SlapSpoEngage
        *   SlapSpoCancel
        *   SlapSpoInvokeDispatch

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

        ANSC_STATUS
        SlapSpoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapSpoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapSpoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapSpoInvokeDispatch
            (
                ANSC_HANDLE                 hThisObject,
                char*                       method_name,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST**       params_out,
                SLAP_VARIABLE**             return_var
            );

    description:

        This function is called to invoke a runtime service method.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       method_name
                Specifies the object method to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST**       params_out
                Specifies the list of output parameters to be returned.

                SLAP_VARIABLE**             return_var
                Specifies the variable to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapSpoInvokeDispatch
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSLAP_SRV_PROXY_OBJECT          pMyObject    = (PSLAP_SRV_PROXY_OBJECT)hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf   = (PSLAP_GOA_INTERFACE   )pMyObject->hSlapGoaIf;

    /*RDKB-6306, CID-24234 & CID-24105, Null check before assignement*/
    if(params_out != NULL)
    {
        *params_out = NULL;
    }
    if(return_var != NULL)
    {
        *return_var = NULL;
    }

    if ( params_in )
    {
        returnStatus =
            pMyObject->PoaToGoaParamList
                (
                    (ANSC_HANDLE)pMyObject,
                    params_in
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
    }

    returnStatus =
        pSlapGoaIf->InvokeObject2
            (
                pSlapGoaIf->hOwnerContext,
                pMyObject->hSlapGoaObj,
                (ANSC_HANDLE)NULL,
                method_name,
                params_in,
                params_out,
                return_var,
                pMyObject->CallTimeout
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }

    if ( params_out && *params_out )
    {
        returnStatus =
            pMyObject->GoaToPoaParamList
                (
                    (ANSC_HANDLE)pMyObject,
                    *params_out
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
    }

    if ( return_var && *return_var )
    {
        returnStatus =
            pMyObject->GoaToPoaVariable
                (
                    (ANSC_HANDLE)pMyObject,
                    *return_var
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}
