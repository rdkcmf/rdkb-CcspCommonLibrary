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

    module:	slap_dslh_paramto_interface.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions for Slap Dslh ParamTree Object.

        *   SlapCreateDslhParamTree
        *   SlapDslhParamtoGetSlapObjCallMap
        *   SlapDslhParamtoGetSlapObjDescriptor

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin     Zhu

    ---------------------------------------------------------------

    revision:

        10/20/05    initial revision.
		11/17/06    Bin Zhu added new apis to support GUI;
        03/22/10    Bin added api "GetParamInfoFromCLI" to support CLI;
		10/06/11	Ported to CCSP Cosa framework.

**********************************************************************/


#include "slap_dslh_paramto_global.h"


/*
 * To simplify the method registration process, we introduce following light-weighted descriptor
 * which is easy to construct and can be generated automatically by a Code Generator in the future.
 * However, parsing such descriptors is apparently much slower than the regular registration.
 * Hopefully object pooling can help to ease the pain in that area.
 */
SLAP_STD_CALL_DESCRIPTOR  g_slapObjCallMap_SlapDslhParamto[] =
{
    /* -- BEGIN -- */
    #if  1
    {
        "GetChildObjNames",                                 /* method name       */
        0,                                                  /* method id         */
        0,								                    /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(GetChildObjNames), /* call entry point  */
        NULL,                                               /* property name     */
        "string, string",                                   /* input parameters  */
        "stringArray",              						/* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "GetParamNames",                                    /* method name       */
        0,                                                  /* method id         */
        0,								                    /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(GetParamNames),    /* call entry point  */
        NULL,                                               /* property name     */
        "string, string, uint32",                           /* input parameters  */
        "stringArray",              						/* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "GetParamValue",                                    /* method name       */
        0,                                                  /* method id         */
        SLAP_CALL_OPTION_passReturnVar,                     /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(GetParamValue),    /* call entry point  */
        NULL,                                               /* property name     */
        "string, string",                                   /* input parameters  */
        NULL,                                               /* output parameters */
        NULL                                                /* returned value    */
    },
    #endif

    #if  1
    {
        "GetParamTypeAndValue",                             /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(GetParamTypeAndValue),   
                                                            /* call entry point  */
        NULL,                                               /* property name     */
        "string, string",                                   /* input parameters  */
        "string,string",                                    /* output parameters */
        NULL                                                /* returned value    */
    },
    #endif

    #if  1
    {
        "SetParamValue",                                    /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(SetParamValue),    /* call entry point  */
        NULL,                                               /* property name     */
        "string, int, uint32, bool, string, variable",      /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "SetParamValueString",                              /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(SetParamValueString),   
                                                            /* call entry point  */
        NULL,                                               /* property name     */
        "string, int, uint32, bool, string, string",        /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "SetParamValueInt",                                 /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(SetParamValueInt), /* call entry point  */
        NULL,                                               /* property name     */
        "string, int, uint32, bool, string, int",           /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "SetParamValueUint",                                /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(SetParamValueUint),/* call entry point  */
        NULL,                                               /* property name     */
        "string, int, uint32, bool, string, uint32",        /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "SetParamValueBool",                                /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(SetParamValueBool),/* call entry point  */
        NULL,                                               /* property name     */
        "string, int, uint32, bool, string, bool",          /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "GetParamInfo",                                     /* method name       */
        0,                                                  /* method id         */
        0,								                    /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(GetParamInfo),     /* call entry point  */
        NULL,                                               /* property name     */
        "string, string, uint32",                           /* input parameters  */
        "stringArray, intArray, stringArray, stringArray, stringArray",
															/* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "GetParamInfoShort",                              /* method name       */
        0,                                                  /* method id         */
        0,								                    /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(GetParamInfoShort),
                                                            /* call entry point  */
        NULL,                                               /* property name     */
        "string, string",                                   /* input parameters  */
        "stringArray, stringArray, stringArray",	    	/* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "GetHiddenInfo",                                    /* method name       */
        0,                                                  /* method id         */
        0,								                    /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(GetHiddenInfo),    /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                          /* input parameters  */
        "stringArray, stringArray",  						/* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "ApplyChanges",                                     /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(ApplyChanges),     /* call entry point  */
        NULL,                                               /* property name     */
        "string, int, uint32, string",						/* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "AddObject",                                        /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(AddObject),        /* call entry point  */
        NULL,                                               /* property name     */
        "string, int, string",                             /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "DelObject",                                        /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(DelObject),        /* call entry point  */
        NULL,                                               /* property name     */
        "string, int, string",                                  /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "GetObjectInfo",                                    /* method name       */
        0,                                                  /* method id         */
        0,								                    /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(GetObjectInfo),    /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        "stringArray, stringArray",  						/* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "AcqWriteAccess",                                   /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(AcqWriteAccess),   /* call entry point  */
        NULL,                                               /* property name     */
        "string, int",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "int"                                              /* returned value    */
    },
    #endif

    #if  1
    {
        "RelWriteAccess",                                   /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(RelWriteAccess),   /* call entry point  */
        NULL,                                               /* property name     */
        "string, int",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "IsParamTreeReadOnly",                              /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(IsParamTreeReadOnly),
                                                            /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "bool"                                              /* returned value    */
    },
    #endif

    #if  1
    {
        "IsParameterReadOnly",                              /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(IsParameterReadOnly),
                                                            /* call entry point  */
        NULL,                                               /* property name     */
        "string, string",                                   /* input parameters  */
        NULL,                                               /* output parameters */
        "bool"                                              /* returned value    */
    },
    #endif

    #if  1
    {
        "EscapeXmlString",                                  /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_DSLH_PARAMTREE_FIELD(EscapeXmlString),  /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                             /* returned value    */
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

SLAP_OBJECT_DESCRIPTOR  g_slapObjDescriptor_SlapDslhParamto =
{
    #if  1

        SLAP_DSLHPARAMTO_UOA_NAME,                          /* object name           */
        SLAP_OBJ_TYPE_scoStandard | SLAP_OBJ_TYPE_poolable, /* object type           */
        SlapCreateDslhParamTree,                            /* object constructor    */
        8,                                                  /* object opo table size */
        16,                                                 /* object mdo table size */
        g_slapObjCallMap_SlapDslhParamto                    /* object call map       */

    #endif
};


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        SlapCreateDslhParamTree
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            )

    description:

        This function is called to create a new Slap Dslh ParamTree
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
SlapCreateDslhParamTree
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{

    return  SlapDslhParamtoCreate(hContainerContext, hOwnerContext, hAnscReserved);
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        SlapDslhParamtoGetSlapObjCallMap
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
SlapDslhParamtoGetSlapObjCallMap
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    return  (ANSC_HANDLE)g_slapObjCallMap_SlapDslhParamto;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        SlapDslhParamtoGetSlapObjDescriptor
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
SlapDslhParamtoGetSlapObjDescriptor
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    return  (ANSC_HANDLE)&g_slapObjDescriptor_SlapDslhParamto;
}
