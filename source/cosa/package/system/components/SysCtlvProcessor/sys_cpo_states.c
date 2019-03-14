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

    module:	sys_cpo_states.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Sys Ctlv Processor Object.

        *   SysCpoSetSysIraIf
        *   SysCpoGetSysSfcIf
        *   SysCpoGetProperty
        *   SysCpoSetProperty
        *   SysCpoResetProperty
        *   SysCpoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        01/13/06    initial revision.

**********************************************************************/


#include "sys_cpo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysCpoSetSysIraIf
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

    return:     object state.

**********************************************************************/

ANSC_STATUS
SysCpoSetSysIraIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject    = (PSYS_CTLV_PROCESSOR_OBJECT  )hThisObject;
    PSYS_CTLV_PROCESSOR_PROPERTY    pProperty    = (PSYS_CTLV_PROCESSOR_PROPERTY)&pMyObject->Property;

    pMyObject->hSysIraIf = hInterface;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysCpoGetSysSfcIf
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
SysCpoGetSysSfcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject    = (PSYS_CTLV_PROCESSOR_OBJECT  )hThisObject;
    PSYS_CTLV_PROCESSOR_PROPERTY    pProperty    = (PSYS_CTLV_PROCESSOR_PROPERTY)&pMyObject->Property;

    return  pMyObject->hSysSfcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysCpoGetProperty
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
SysCpoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject    = (PSYS_CTLV_PROCESSOR_OBJECT  )hThisObject;
    PSYS_CTLV_PROCESSOR_PROPERTY    pProperty    = (PSYS_CTLV_PROCESSOR_PROPERTY)&pMyObject->Property;

    *(PSYS_CTLV_PROCESSOR_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysCpoSetProperty
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
SysCpoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject    = (PSYS_CTLV_PROCESSOR_OBJECT  )hThisObject;
    PSYS_CTLV_PROCESSOR_PROPERTY    pProperty    = (PSYS_CTLV_PROCESSOR_PROPERTY)&pMyObject->Property;

    *pProperty = *(PSYS_CTLV_PROCESSOR_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysCpoResetProperty
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
SysCpoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject    = (PSYS_CTLV_PROCESSOR_OBJECT  )hThisObject;
    PSYS_CTLV_PROCESSOR_PROPERTY    pProperty    = (PSYS_CTLV_PROCESSOR_PROPERTY)&pMyObject->Property;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysCpoReset
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
SysCpoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_CTLV_PROCESSOR_OBJECT      pMyObject    = (PSYS_CTLV_PROCESSOR_OBJECT  )hThisObject;
    PSYS_CTLV_PROCESSOR_PROPERTY    pProperty    = (PSYS_CTLV_PROCESSOR_PROPERTY)&pMyObject->Property;

    return  ANSC_STATUS_SUCCESS;
}
