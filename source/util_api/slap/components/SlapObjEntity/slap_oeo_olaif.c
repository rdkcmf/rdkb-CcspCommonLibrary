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

    module:	slap_oeo_olaif.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Slap Obj Entity Object.

        *   SlapOeoOlaGetObjectProperty
        *   SlapOeoOlaGetMethodDescriptor

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/11/03    initial revision.

**********************************************************************/


#include "slap_oeo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOeoOlaGetObjectProperty
            (
                ANSC_HANDLE                 hThisObject,
                char*                       property_name
            );

    description:

        This function is called to retrieve an object_property by
        matching the property name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       property_name
                Specifies the property name to be matched.

    return:     object property.

**********************************************************************/

ANSC_HANDLE
SlapOeoOlaGetObjectProperty
    (
        ANSC_HANDLE                 hThisObject,
        char*                       property_name
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject       = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;

    return  pMyObject->GetObjectProperty((ANSC_HANDLE)pMyObject, property_name);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOeoOlaGetMethodDescriptor
            (
                ANSC_HANDLE                 hThisObject,
                char*                       method_name,
                SLAP_PARAMETER_LIST*        param_list_input
            );

    description:

        This function is called to retrieve a method_descriptor by
        matching the method name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       method_name
                Specifies the method name to be matched.

                SLAP_PARAMETER_LIST*        param_list_input
                Specifies the input parameter list to be matched.

    return:     method descriptor.

**********************************************************************/

ANSC_HANDLE
SlapOeoOlaGetMethodDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        param_list_input
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject         = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;

    return  pMyObject->GetMethodDescriptor2((ANSC_HANDLE)pMyObject, method_name, param_list_input);
}
