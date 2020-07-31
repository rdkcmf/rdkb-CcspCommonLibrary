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

    module:	slap_lso_states.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Slap Loam Server Object.

        *   SlapLsoGetAnscCpcIf
        *   SlapLsoGetAnscLpcConnector
        *   SlapLsoSetAnscLpcConnector
        *   SlapLsoGetSlapUoaIf
        *   SlapLsoSetSlapUoaIf
        *   SlapLsoGetMobilityOn
        *   SlapLsoSetMobilityOn
        *   SlapLsoGetProperty
        *   SlapLsoSetProperty
        *   SlapLsoResetProperty
        *   SlapLsoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/02/03    initial revision.

**********************************************************************/


#include "slap_lso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapLsoGetAnscCpcIf
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
SlapLsoGetAnscCpcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;

    return  pMyObject->hAnscCpcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapLsoGetAnscLpcConnector
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
SlapLsoGetAnscLpcConnector
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;

    return  pMyObject->hAnscLpcConnector;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoSetAnscLpcConnector
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hLpcConnector
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hLpcConnector
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLsoSetAnscLpcConnector
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcConnector
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject         = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;

    pMyObject->hAnscLpcConnector = hLpcConnector;
    pAnscLpcConnector            = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;

    if ( pAnscLpcConnector )
    {
        pAnscLpcConnector->SetAnscCpcIf((ANSC_HANDLE)pAnscLpcConnector, pMyObject->hAnscCpcIf);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapLsoGetSlapUoaIf
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
SlapLsoGetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;

    return  pMyObject->hSlapUoaIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoSetSlapUoaIf
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
SlapLsoSetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;

    pMyObject->hSlapUoaIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapLsoGetMobilityOn
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

BOOL
SlapLsoGetMobilityOn
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PSLAP_LOAM_SERVER_PROPERTY      pProperty    = (PSLAP_LOAM_SERVER_PROPERTY)&pMyObject->Property;

    return  pProperty->bMobilityOn;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoSetMobilityOn
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bMobilityOn
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bMobilityOn
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLsoSetMobilityOn
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bMobilityOn
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PSLAP_LOAM_SERVER_PROPERTY      pProperty    = (PSLAP_LOAM_SERVER_PROPERTY)&pMyObject->Property;

    pProperty->bMobilityOn = bMobilityOn;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoGetProperty
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
SlapLsoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PSLAP_LOAM_SERVER_PROPERTY      pProperty    = (PSLAP_LOAM_SERVER_PROPERTY)&pMyObject->Property;

    *(PSLAP_LOAM_SERVER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoSetProperty
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
SlapLsoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PSLAP_LOAM_SERVER_PROPERTY      pProperty    = (PSLAP_LOAM_SERVER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PSLAP_LOAM_SERVER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoResetProperty
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
SlapLsoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PSLAP_LOAM_SERVER_PROPERTY      pProperty    = (PSLAP_LOAM_SERVER_PROPERTY)&pMyObject->Property;

    pProperty->bMobilityOn = FALSE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoReset
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
SlapLsoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}
