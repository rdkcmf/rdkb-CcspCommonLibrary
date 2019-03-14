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

    module:	ansc_http_tokenizer_co_interface.h

        For tokenizer,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Object Container Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        11/26/01    initial revision.

        02/20/02    updated revision 1
                    add some comments
                    and change formatting.

    ---------------------------------------------------------------

    last modified:

        02/20/02

**********************************************************************/

#ifndef  _ANSC_HTTP_TOKENIZER_CO_INTERFACE
#define  _ANSC_HTTP_TOKENIZER_CO_INTERFACE

#include "ansc_tokenizer_co_interface.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_HTTP_TKNZ_COMPONENT_NAME              "HttpTokenizer"
#define  ANSC_HTTP_TKNZ_COMPONENT_OID               ANSC_OBJECT_OID_NULL
#define  ANSC_HTTP_TKNZ_COMPONENT_TYPE              ANSC_OBJECT_TYPE_NULL

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  ANSC_HTTP_TKNZ_COMPONENT_CLASS_CONTENT                                             \
    /* duplication of the base object class content */                                      \
    ANSC_TKNZ_COMPONENT_CLASS_CONTENT                                                       


/* tokenizer object  */
typedef  struct
_ANSC_HTTP_TKNZ_COMPONENT_OBJECT
{
    ANSC_HTTP_TKNZ_COMPONENT_CLASS_CONTENT
}
ANSC_HTTP_TKNZ_CO,    *PANSC_HTTP_TKNZ_CO;


#define  ACCESS_HTTP_TOKENIZER_COMPONENT_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_HTTP_TKNZ_CO, Linkage)

#endif
