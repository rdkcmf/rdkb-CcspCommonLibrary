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

    module:	slap_vco_interface.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions for Slap Var Converter Object.

        *   SlapCreateVarConverter
        *   SlapVcoGetSlapObjCallMap
        *   SlapVcoGetSlapObjDescriptor

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/13/03    initial revision.

**********************************************************************/


#include "slap_vco_global.h"


/*
 * To simplify the method registration process, we introduce following light-weighted descriptor
 * which is easy to construct and can be generated automatically by a Code Generator in the future.
 * However, parsing such descriptors is apparently much slower than the regular registration.
 * Hopefully object pooling can help to ease the pain in that area.
 */
SLAP_STD_CALL_DESCRIPTOR  g_slapObjCallMap_SlapVco[] =
{
    /* -- BEGIN -- */

    #if  1
    {
        "BoolToString",                                     /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(BoolToString),      /* call entry point  */
        NULL,                                               /* property name     */
        "bool",                                             /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "IntToString",                                      /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(IntToString),       /* call entry point  */
        NULL,                                               /* property name     */
        "int",                                              /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "UcharArrayToString",                               /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(UcharArrayToString),/* call entry point  */
        NULL,                                               /* property name     */
        "ucharArray",                                       /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "UcharArrayToBase64String",                               /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(UcharArrayToBase64String),/* call entry point  */
        NULL,                                               /* property name     */
        "ucharArray",                                       /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "UcharArrayToString2",                              /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(UcharArrayToString2),
                                                            /* call entry point  */
        NULL,                                               /* property name     */
        "ucharArray",                                       /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "Uint32ToString",                                   /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(Uint32ToString),    /* call entry point  */
        NULL,                                               /* property name     */
        "uint32",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "Ip4AddrToString",                                  /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(Ip4AddrToString),   /* call entry point  */
        NULL,                                               /* property name     */
        "uint32/ip4_addr",                                  /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "Ip4AddrToString2",                                 /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(Ip4AddrToString2),  /* call entry point  */
        NULL,                                               /* property name     */
        "uint32/ip4_addr",                                  /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "Ip4AddrListToString",                              /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(Ip4AddrListToString),
                                                            /* call entry point  */
        NULL,                                               /* property name     */
        "uint32Array",                                      /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "Ip6AddrToString",                                  /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(Ip6AddrToString),   /* call entry point  */
        NULL,                                               /* property name     */
        "ucharArray/ip6_addr",                              /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "MacAddrToString",                                  /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(MacAddrToString),   /* call entry point  */
        NULL,                                               /* property name     */
        "ucharArray/mac_addr",                              /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "MacAddrToString2",                                  /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(MacAddrToString2),   /* call entry point  */
        NULL,                                               /* property name     */
        "ucharArray/mac_addr",                              /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "OidListToString",                                  /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(OidListToString),   /* call entry point  */
        NULL,                                               /* property name     */
        "uint32Array/oid_list",                             /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "CalendarTimeToString",                             /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(CalendarTimeToString),
                                                            /* call entry point  */
        NULL,                                               /* property name     */
        "handle/calendar_time",                             /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "Uint32ToHexString",                                /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(Uint32ToHexString), /* call entry point  */
        NULL,                                               /* property name     */
        "uint32",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "string"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "StringToBool",                                     /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(StringToBool),      /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "bool"                                              /* returned value    */
    },
    #endif

    #if  1
    {
        "StringToInt",                                      /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(StringToInt),       /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "int"                                               /* returned value    */
    },
    #endif

    #if  1
    {
        "StringToMacAddr",                                  /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(StringToMacAddr),   /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "ucharArray/mac_addr"                               /* returned value    */
    },
    #endif

    #if  1
    {
        "StringToUcharArray",                               /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(StringToUcharArray),/* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "ucharArray"                                        /* returned value    */
    },
    #endif

    #if  1
    {
        "Base64StringToUcharArray",                               /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(Base64StringToUcharArray),/* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "ucharArray"                                        /* returned value    */
    },
    #endif

    #if  1
    {
        "StringToUcharArray2",                              /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(StringToUcharArray2),
                                                            /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "ucharArray"                                        /* returned value    */
    },
    #endif

    #if  1
    {
        "StringToUint32",                                   /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(StringToUint32),    /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "StringToIp4Addr",                                  /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(StringToIp4Addr),   /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32/ip4_addr"                                   /* returned value    */
    },
    #endif

    #if  1
    {
        "StringToIp4AddrList",                              /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(StringToIp4AddrList),
                                                            /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32Array"                                       /* returned value    */
    },
    #endif

    #if  1
    {
        "StringToIp6Addr",                                  /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(StringToIp6Addr),   /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "ucharArray/ip6_addr"                               /* returned value    */
    },
    #endif

    #if  1
    {
        "StringToMacAddr",                                  /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(StringToMacAddr),   /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "ucharArray/mac_addr"                               /* returned value    */
    },
    #endif

    #if  1
    {
        "StringToOidList",                                  /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(StringToOidList),   /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32Array/oid_list"                              /* returned value    */
    },
    #endif

    #if  1
    {
        "StringToCalendarTime",                             /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(StringToCalendarTime),
                                                            /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "handle/calendar_time"                              /* returned value    */
    },
    #endif

    #if  1
    {
        "HexStringToUint32",                                /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(HexStringToUint32), /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "HexStringToDecimal",                               /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(HexStringToDecimal),/* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "Uint32ArrayToMacAddr",                             /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(Uint32ArrayToMacAddr),
                                                            /* call entry point  */
        NULL,                                               /* property name     */
        "uint32Array",                                      /* input parameters  */
        NULL,                                               /* output parameters */
        "ucharArray/mac_addr"                               /* returned value    */
    },
    #endif

    #if  1
    {
        "Uint32ToUcharArray",                               /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(Uint32ToUcharArray),/* call entry point  */
        NULL,                                               /* property name     */
        "uint32",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "ucharArray"                                        /* returned value    */
    },
    #endif

    #if  1
    {
        "Uint32ToUchar",                                    /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(Uint32ToUchar),     /* call entry point  */
        NULL,                                               /* property name     */
        "uint32",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "ucharArray"                                        /* returned value    */
    },
    #endif

    #if  1
    {
        "UcharArrayToUint32",                               /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_VAR_CONVERTER_FIELD(UcharArrayToUint32),/* call entry point  */
        NULL,                                               /* property name     */
        "ucharArray",                                       /* input parameters  */
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

SLAP_OBJECT_DESCRIPTOR  g_slapObjDescriptor_SlapVco =
{
    #if  1

        "System.Utility.VarConverter",                      /* object name           */
        SLAP_OBJ_TYPE_scoStandard | SLAP_OBJ_TYPE_poolable, /* object type           */
        SlapCreateVarConverter,                             /* object constructor    */
        4,                                                  /* object opo table size */
        16,                                                 /* object mdo table size */
        g_slapObjCallMap_SlapVco                            /* object call map       */

    #endif
};


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        SlapCreateVarConverter
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            )

    description:

        This function is called to create a new Slap Var Converter
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
SlapCreateVarConverter
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{

    return  SlapVcoCreate(hContainerContext, hOwnerContext, hAnscReserved);
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        SlapVcoGetSlapObjCallMap
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
SlapVcoGetSlapObjCallMap
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    return  (ANSC_HANDLE)g_slapObjCallMap_SlapVco;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        SlapVcoGetSlapObjDescriptor
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
SlapVcoGetSlapObjDescriptor
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    return  (ANSC_HANDLE)&g_slapObjDescriptor_SlapVco;
}
