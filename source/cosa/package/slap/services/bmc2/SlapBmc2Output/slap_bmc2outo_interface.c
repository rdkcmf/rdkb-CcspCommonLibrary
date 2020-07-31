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

    module:	slap_bmc2outo_interface.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions for Slap Bmc2 Output Object.

        *   SlapCreateBmc2Output
        *   SlapBmc2OutoGetSlapObjCallMap
        *   SlapBmc2OutoGetSlapObjDescriptor

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/29/05    initial revision.

**********************************************************************/


#include "slap_bmc2outo_global.h"


/*
 * To simplify the method registration process, we introduce following light-weighted descriptor
 * which is easy to construct and can be generated automatically by a Code Generator in the future.
 * However, parsing such descriptors is apparently much slower than the regular registration.
 * Hopefully object pooling can help to ease the pain in that area.
 */
SLAP_STD_CALL_DESCRIPTOR  g_slapObjCallMap_SlapBmc2Outo[] =
{
    /* -- BEGIN -- */

    #if  1
    {
        "Clear",                                            /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_BMC2_OUTPUT_FIELD(Clear),               /* call entry point  */
        NULL,                                               /* property name     */
        NULL,                                               /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "Write",                                            /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_BMC2_OUTPUT_FIELD(Write),               /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "EraseLine",                                        /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_BMC2_OUTPUT_FIELD(EraseLine),           /* call entry point  */
        NULL,                                               /* property name     */
        NULL,                                               /* input parameters  */
        NULL,                                               /* output parameters */
        "uint32"                                            /* returned value    */
    },
    #endif

    #if  1
    {
        "MoveCursor",                                       /* method name       */
        0,                                                  /* method id         */
        0,                                                  /* call options      */
        ACCESS_SLAP_BMC2_OUTPUT_FIELD(MoveCursor),          /* call entry point  */
        NULL,                                               /* property name     */
        "string",                                           /* input parameters  */
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

SLAP_OBJECT_DESCRIPTOR  g_slapObjDescriptor_SlapBmc2Outo =
{
    #if  1

        SLAP_BMC2OUTO_UOA_NAME,                             /* object name           */
        SLAP_OBJ_TYPE_scoStandard | SLAP_OBJ_TYPE_poolable, /* object type           */
        SlapCreateBmc2Output,                               /* object constructor    */
        8,                                                  /* object opo table size */
        16,                                                 /* object mdo table size */
        g_slapObjCallMap_SlapBmc2Outo                       /* object call map       */

    #endif
};


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        SlapCreateBmc2Output
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            )

    description:

        This function is called to create a new Slap Bmc2 Output
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
SlapCreateBmc2Output
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{

    return  SlapBmc2OutoCreate(hContainerContext, hOwnerContext, hAnscReserved);
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        SlapBmc2OutoGetSlapObjCallMap
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
SlapBmc2OutoGetSlapObjCallMap
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    return  (ANSC_HANDLE)g_slapObjCallMap_SlapBmc2Outo;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        SlapBmc2OutoGetSlapObjDescriptor
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
SlapBmc2OutoGetSlapObjDescriptor
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    return  (ANSC_HANDLE)&g_slapObjDescriptor_SlapBmc2Outo;
}
