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

    module:	bmc2_comeo_helper_api.h

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

#ifndef  _BMC2_COMEO_HELPER_API_DEC_
#define  _BMC2_COMEO_HELPER_API_DEC_


BOOL
Bmc2ComeoReaderValid
    (
        ANSC_HANDLE                 hThisObject
    );


ULONG
Bmc2ComeoReaderRead
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );


ANSC_STATUS
Bmc2ComeoReaderClose
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_STATUS
Bmc2ComeoReaderOpenExternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName
    );


ANSC_STATUS
Bmc2ComeoReaderOpenInternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        char                        *pContent,
        ULONG                       ulContentLen
    );


char *
Bmc2ComeoReaderGetTemplateName
    (
        ANSC_HANDLE                 hThisObject
    );


#endif



