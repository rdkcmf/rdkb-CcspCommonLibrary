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

    module:	slap_veo_interface.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions for Slap Var Entity Object.

        *   SlapCreateVarEntity
        *   SlapVeoGetSlapObjCallMap
        *   SlapVeoGetSlapObjDescriptor

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/20/03    initial revision.

**********************************************************************/


#include "slap_veo_global.h"


/*
 * To simplify the method registration process, we introduce following light-weighted descriptor
 * which is easy to construct and can be generated automatically by a Code Generator in the future.
 * However, parsing such descriptors is apparently much slower than the regular registration.
 * Hopefully object pooling can help to ease the pain in that area.
 */
SLAP_STD_CALL_DESCRIPTOR  g_slapObjCallMap_SlapVeo[] =
{
    /* -- BEGIN -- */

    #if  1
    {
        "GetVarSyntax",                                     /* method name       */
        SLAP_METHOD_ID_GetProperty,                         /* method id         */
        SLAP_CALL_OPTION_passParamListI,                    /* call options      */
        ACCESS_SLAP_VAR_ENTITY_FIELD(GetVarSyntax),         /* call entry point  */
        "Syntax",                                           /* property name     */
        NULL,                                               /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "GetVarContentType",                                /* method name       */
        SLAP_METHOD_ID_GetProperty,                         /* method id         */
        SLAP_CALL_OPTION_passParamListI,                    /* call options      */
        ACCESS_SLAP_VAR_ENTITY_FIELD(GetVarContentType),    /* call entry point  */
        "ContentType",                                      /* property name     */
        NULL,                                               /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "SetVarContentType",                                /* method name       */
        SLAP_METHOD_ID_SetProperty,                         /* method id         */
        SLAP_CALL_OPTION_passParamListI,                    /* call options      */
        ACCESS_SLAP_VAR_ENTITY_FIELD(SetVarContentType),    /* call entry point  */
        "ContentType",                                      /* property name     */
        NULL,                                               /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "GetVarUsageType",                                  /* method name       */
        SLAP_METHOD_ID_GetProperty,                         /* method id         */
        SLAP_CALL_OPTION_passParamListI,                    /* call options      */
        ACCESS_SLAP_VAR_ENTITY_FIELD(GetVarUsageType),      /* call entry point  */
        "UsageType",                                        /* property name     */
        NULL,                                               /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "SetVarUsageType",                                  /* method name       */
        SLAP_METHOD_ID_SetProperty,                         /* method id         */
        SLAP_CALL_OPTION_passParamListI,                    /* call options      */
        ACCESS_SLAP_VAR_ENTITY_FIELD(SetVarUsageType),      /* call entry point  */
        "UsageType",                                        /* property name     */
        NULL,                                               /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    /* -- END -- */

    #if  1      /* mark the end of the obj call map */
    {
        NULL,                                               /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        0,                                                  /* call entry point  */
        NULL,                                               /* property name     */
        NULL,                                               /* input parameters  */
        NULL,                                               /* output parameters */
        NULL                                                /* returned value    */
    },
    #endif
};

SLAP_OBJECT_DESCRIPTOR  g_slapObjDescriptor_SlapVeo =
{
    #if  1

        "System.Utility.VarEntity",                         /* object name           */
        SLAP_OBJ_TYPE_scoStandard | SLAP_OBJ_TYPE_poolable, /* object type           */
        SlapCreateVarEntity,                                /* object constructor    */
        4,                                                  /* object opo table size */
        4,                                                  /* object mdo table size */
        g_slapObjCallMap_SlapVeo                            /* object call map       */

    #endif
};


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        SlapCreateVarEntity
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            )

    description:

        This function is called to create a new Slap Var Entity
        object.

    argument:   ANSC_HANDLE                 hContainerContext
                This context handle is transparent to the object mapper
                wrapper, it's only meanful to the caller.

                ANSC_HANDLE                 hOwnerContext
                This context handle is transparent to the object mapper
                wrapper, it's only meanful to the caller.

                ANSC_HANDLE                 hAnscReserved
                This context handle is transparent to the object mapper
                wrapper, it's only meanful to the caller.

    return:     handle of the object mapper object.

**********************************************************************/

ANSC_HANDLE
SlapCreateVarEntity
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{

    return  SlapVeoCreate(hContainerContext, hOwnerContext, hAnscReserved);
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        SlapVeoGetSlapObjCallMap
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function is called to retrieve the obj_call_map of this
        SLAP service object.

    argument:   ANSC_HANDLE                 hContainerContext
                This context handle is transparent to the object mapper
                wrapper, it's only meanful to the caller.

    return:     obj call map.

**********************************************************************/

ANSC_HANDLE
SlapVeoGetSlapObjCallMap
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    return  (ANSC_HANDLE)g_slapObjCallMap_SlapVeo;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        SlapVeoGetSlapObjDescriptor
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function is called to retrieve the obj_descriptor of this
        SLAP service object.

    argument:   ANSC_HANDLE                 hContainerContext
                This context handle is transparent to the object mapper
                wrapper, it's only meanful to the caller.

    return:     obj descriptor.

**********************************************************************/

ANSC_HANDLE
SlapVeoGetSlapObjDescriptor
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    return  (ANSC_HANDLE)&g_slapObjDescriptor_SlapVeo;
}
