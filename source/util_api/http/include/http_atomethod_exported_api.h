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

    module:	http_atomethod_exported_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Http Ato Method
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/15/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_ATOMETHOD_EXPORTED_API_
#define  _HTTP_ATOMETHOD_EXPORTED_API_


/***********************************************************
    FUNCTIONS IMPLEMENTED IN HTTP_ATOMETHOD_INTERFACE.C
***********************************************************/

ANSC_HANDLE
HttpCreateAtoMethod
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_ATOMETHOD_BASE.C
***********************************************************/

ANSC_HANDLE
HttpAtoMethodCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
HttpAtoMethodRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpAtoMethodEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpAtoMethodInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
