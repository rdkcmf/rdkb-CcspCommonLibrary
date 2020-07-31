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

    module:	ansc_object_descriptor_operation.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the General Object Descriptor Object.

        *   AnscOdoCreateObject
        *   AnscOdoRemoveObject

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/19/01    initial revision.

**********************************************************************/


#include "ansc_object_mapper_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscOdoCreateObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOwnerContext
            );

    description:

        This function is called to create a new object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOwnerContext
                Specifies the context handle of the object owner.

    return:     handle of the newly created object.

**********************************************************************/

ANSC_HANDLE
AnscOdoCreateObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerContext
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    if ( !pObjectDescriptor->pfnObjectCreate )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  pObjectDescriptor->pfnObjectCreate
                (
                    pObjectDescriptor->hContainerContext,
                    hOwnerContext,
                    pObjectDescriptor->hMapperReserved
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOdoRemoveObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObject
            );

    description:

        This function is called to remove an existing object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObject
                Specifies the object to be removed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOdoRemoveObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObject
    )
{
    PANSC_OBJECT_DESCRIPTOR_OBJECT  pObjectDescriptor = (PANSC_OBJECT_DESCRIPTOR_OBJECT)hThisObject;

    if ( !pObjectDescriptor->pfnObjectRemove )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pObjectDescriptor->pfnObjectRemove(hObject);
    }

    return  ANSC_STATUS_SUCCESS;
}
