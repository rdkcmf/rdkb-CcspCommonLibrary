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

    module:	ansc_http_tokenizer_co_exported_api.h

        For tokenizer,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the component object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        01/28/02    initial revision.

        02/20/02    updated revision 1
                    add some comments
                    and change formatting.

    ---------------------------------------------------------------
    
    last modified:

        02/20/02

**********************************************************************/


#ifndef  _ANSC_HTTP_TOKENIZER_CO_EXPORTED_API_
#define  _ANSC_HTTP_TOKENIZER_CO_EXPORTED_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_TOKENIZER_OCO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
AnscCreateHttpTokenizerComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN PPLM_OCO_BASE.C
***********************************************************/

ANSC_HANDLE
AnscHttpTokenizerCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
AnscHttpTokenizerCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscHttpTokenizerCOEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscHttpTokenizerCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
