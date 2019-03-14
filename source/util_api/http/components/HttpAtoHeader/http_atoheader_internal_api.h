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

    module:	http_atoheader_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Ato Header
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_ATOHEADER_INTERNAL_API_
#define  _HTTP_ATOHEADER_INTERNAL_API_


/***********************************************************
     FUNCTIONS IMPLEMENTED IN HTTP_ATOHEADER_OPERATION.C
***********************************************************/

ANSC_STATUS
HttpAtoHeaderCreateCodeTable
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpAtoHeaderCreateNameTable
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpAtoHeaderGetAtomCode
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

char*
HttpAtoHeaderGetAtomName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       code
    );


#endif
