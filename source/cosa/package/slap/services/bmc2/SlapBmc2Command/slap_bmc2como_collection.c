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

    module:	slap_bmc2como_collection.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced collection functions
        of the Slap Bmc2 Command Object.

        *   SlapBmc2ComoGetCount

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/28/05    initial revision.

**********************************************************************/


#include "slap_bmc2como_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UINT32
        SlapBmc2ComoGetCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the element count.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     element count.

**********************************************************************/

SLAP_UINT32
SlapBmc2ComoGetCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_BMC2_COMMAND_OBJECT       pMyObject            = (PSLAP_BMC2_COMMAND_OBJECT  )hThisObject;
    PBMC2_REQ_CONTROLLER_OBJECT     pBmc2ReqController   = (PBMC2_REQ_CONTROLLER_OBJECT)pMyObject->hInsContext;

    return  pBmc2ReqController->CliArgumentCount;
}
