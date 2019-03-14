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

    module:	bree_spo_helper_api.h

        For BSP Run-time Execution Engine (BREE),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BSP Run-time Execution Engine (BREE).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        07/22/03    initial revision.

**********************************************************************/

#ifndef  _BREE_SPO_HELPER_API_DEC_
#define  _BREE_SPO_HELPER_API_DEC_


BOOL
BreeSpoReaderValid
    (
        ANSC_HANDLE                 hThisObject
    );


ULONG
BreeSpoReaderRead
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );


ANSC_STATUS
BreeSpoReaderClose
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_STATUS
BreeSpoReaderOpenExternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName
    );


ANSC_STATUS
BreeSpoReaderOpenInternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        char                        *pContent,
        ULONG                       ulContentLen
    );


char *
BreeSpoReaderGetTemplateName
    (
        ANSC_HANDLE                 hThisObject
    );


#endif



