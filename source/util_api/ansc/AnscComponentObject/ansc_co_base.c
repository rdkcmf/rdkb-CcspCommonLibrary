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

    module:	ansc_co_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of a general Ansc Component Object.

        *   AnscCoCreate
        *   AnscCoRemove
        *   AnscCoEnrollObjects
        *   AnscCoInitialize
        *   AnscCoGetOwner
        *   AnscCoSetOwner
        *   AnscCoGetName
        *   AnscCoGetOid
        *   AnscCoGetRid
        *   AnscCoDispatchEvent

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/07/01    initial revision.

**********************************************************************/


#include "ansc_co_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscCoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs an abstract Ansc Component Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
AnscCoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pBaseObject = (PANSC_COMPONENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_COMPONENT_OBJECT));

    if ( !pBaseObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    /* AnscCopyString(pBaseObject->Name, ANSC_COMPONENT_OBJECT_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_COMPONENT_OBJECT_ID;
    pBaseObject->Create            = AnscCoCreate;
    pBaseObject->Remove            = AnscCoRemove;
    pBaseObject->EnrollObjects     = AnscCoEnrollObjects;
    pBaseObject->Initialize        = AnscCoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pBaseObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_COMPONENT_OBJECT          pBaseObject  = (PANSC_COMPONENT_OBJECT)hThisObject;

    if ( pBaseObject )
    	AnscFreeMemory(pBaseObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCoEnrollObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function enrolls all the fellow objects used by this
        object implementation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCoEnrollObjects
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
        AnscCoInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initializes the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_COMPONENT_OBJECT          pBaseObject  = (PANSC_COMPONENT_OBJECT)hThisObject;

    pBaseObject->Rid           = (ULONG)pBaseObject;
    /*
    pBaseObject->GetOwner      = AnscCoGetOwner;
    pBaseObject->SetOwner      = AnscCoSetOwner;
    pBaseObject->GetName       = AnscCoGetName;
    pBaseObject->GetOid        = AnscCoGetOid;
    pBaseObject->GetRid        = AnscCoGetRid;
    pBaseObject->DispatchEvent = AnscCoDispatchEvent;
    */

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscCoGetOwner
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function retrieves the handle of the owner of the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     owner's context.

**********************************************************************/

ANSC_HANDLE
AnscCoGetOwner
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_COMPONENT_OBJECT          pBaseObject  = (PANSC_COMPONENT_OBJECT)hThisObject;

    return  pBaseObject->hOwnerContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCoSetOwner
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOwner
            );

    description:

        This function configures the handle of the owner of the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOwner
                Specifies the owner's context to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCoSetOwner
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwner
    )
{
    PANSC_COMPONENT_OBJECT          pBaseObject  = (PANSC_COMPONENT_OBJECT)hThisObject;

    pBaseObject->hOwnerContext = hOwner;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        AnscCoGetName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the name of this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     pointer to the object name string.

**********************************************************************/

char*
AnscCoGetName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    /*
     * We simply return the object name pointing to our internal buffer; if the caller wants to do
     * anything about it, he has to duplicate the string.
     */
    return  NULL; /* pBaseObject->Name; */
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscCoGetOid
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the oid of this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object id.

**********************************************************************/

ULONG
AnscCoGetOid
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_COMPONENT_OBJECT          pBaseObject  = (PANSC_COMPONENT_OBJECT)hThisObject;

    return  pBaseObject->Oid;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscCoGetRid
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the rid of this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     runtime id.

**********************************************************************/

ULONG
AnscCoGetRid
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_COMPONENT_OBJECT          pBaseObject  = (PANSC_COMPONENT_OBJECT)hThisObject;

    return  pBaseObject->Rid;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscCoDispatchEvent
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEvent
            );

    description:

        This function dispatches the event to the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEvent
                Specifies the event object to be dispatched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCoDispatchEvent
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEvent
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hEvent);
    return  ANSC_STATUS_SUCCESS;
}
