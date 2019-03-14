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

    module:	bmc2_comeo_internal_api.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Bmc2 Com Executor
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/28/05    initial revision.

**********************************************************************/

#ifndef  _BMC2_COMEO_INTERNAL_API
#define  _BMC2_COMEO_INTERNAL_API


ANSC_HANDLE
Bmc2ComeoGetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComeoSetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
Bmc2ComeoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComeoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComeoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComeoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2ComeoPreparePage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        void*                       page_data,
        ULONG                       page_size
    );

ANSC_STATUS
Bmc2ComeoExecutePage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBwrmCookedPage,
        ANSC_HANDLE                 hBeepPecIf
    );


ANSC_STATUS
Bmc2ComeoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
Bmc2ComeoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
Bmc2ComeoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ComeoReset
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2ComeoCreateBmc2SoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmc2PecIf
    );

ANSC_HANDLE
Bmc2ComeoSoaGetSlapObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_HANDLE
Bmc2ComeoSoaGetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

ANSC_STATUS
Bmc2ComeoSoaRetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage
    );

ANSC_STATUS
Bmc2ComeoSoaGetCookedPageData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage,
        PUCHAR                      *ppStream,
        PULONG                      pulStreamSize
    );

ANSC_STATUS
Bmc2ComeoSoaInvokeObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBeepObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        PULONG                      pulSoaStatus
    );

BOOL
Bmc2ComeoSoaIsBuiltInObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_STATUS
Bmc2ComeoSoaWriteAString
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pAsciiStr
    );

ANSC_STATUS
Bmc2ComeoSoaWriteBString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBinaryStr,
        ULONG                       ulStrLen
    );

BOOL
Bmc2ComeoSoaIsInterrupted
    (
        ANSC_HANDLE                 hThisObject
    );

#endif

