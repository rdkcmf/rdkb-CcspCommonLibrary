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

    module:	web_mco_base.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Web Module Container Object.

        *   WebMcoCreate
        *   WebMcoRemove
        *   WebMcoEnrollObjects
        *   WebMcoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/15/02    initial revision.

**********************************************************************/


#include "web_mco_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        WebMcoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Web Module Container Object and
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
WebMcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PWEB_MODULE_CONTAINER_OBJECT    pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PWEB_MODULE_CONTAINER_OBJECT)AnscAllocateMemory(sizeof(WEB_MODULE_CONTAINER_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), WEB_MODULE_CONTAINER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = WEB_MODULE_CONTAINER_OID;
    pBaseObject->Create            = WebMcoCreate;
    pBaseObject->Remove            = WebMcoRemove;
    pBaseObject->EnrollObjects     = WebMcoEnrollObjects;
    pBaseObject->Initialize        = WebMcoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebMcoRemove
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
WebMcoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_MODULE_CONTAINER_OBJECT    pMyObject    = (PWEB_MODULE_CONTAINER_OBJECT)hThisObject;

    AnscMcoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebMcoEnrollObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function enrolls all the objects required by this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebMcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_MODULE_CONTAINER_OBJECT    pMyObject    = (PWEB_MODULE_CONTAINER_OBJECT)hThisObject;

    AnscMcoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebMcoInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebMcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_MODULE_CONTAINER_OBJECT    pMyObject     = (PWEB_MODULE_CONTAINER_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscMcoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                         = WEB_MODULE_CONTAINER_OID;
    pMyObject->Create                      = WebMcoCreate;
    pMyObject->Remove                      = WebMcoRemove;
    pMyObject->EnrollObjects               = WebMcoEnrollObjects;
    pMyObject->Initialize                  = WebMcoInitialize;

    pMyObject->EnrollExtensionObjects      = WebMcoEnrollExtensionObjects;
    pMyObject->ManufactureExtensionObjects = WebMcoManufactureExtensionObjects;
    pMyObject->DestroyExtensionObjects     = WebMcoDestroyExtensionObjects;

    pMyObject->EnrollFeatureObjects        = WebMcoEnrollFeatureObjects;
    pMyObject->ManufactureFeatureObjects   = WebMcoManufactureFeatureObjects;
    pMyObject->DestroyFeatureObjects       = WebMcoDestroyFeatureObjects;

    pMyObject->Ignite                      = WebMcoIgnite;
    pMyObject->PutOut                      = WebMcoPutOut;
    pMyObject->Switch                      = WebMcoSwitch;

    pMyObject->RegisterIfs                 = WebMcoRegisterIfs;
    pMyObject->DispatchIfs                 = WebMcoDispatchIfs;

    pMyObject->hHttpHco                    = (ANSC_HANDLE)NULL;
    pMyObject->hWebSso                     = (ANSC_HANDLE)NULL;

    pMyObject->GetHttpHco                  = WebMcoGetHttpHco;
    pMyObject->GetWebSso                   = WebMcoGetWebSso;

    return  ANSC_STATUS_SUCCESS;
}
