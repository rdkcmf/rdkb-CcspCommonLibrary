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

    module:	http_fco_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Form Content Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/17/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_FCO_INTERFACE_
#define  _HTTP_FCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"


/***********************************************************
  PLATFORM INDEPENDENT HTTP FORM CONTENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_FCO_SCRATCH_PAD1_SIZE                 4096
#define  HTTP_FCO_SCRATCH_PAD2_SIZE                 256
#define  HTTP_FCO_FEO_TABLE_SIZE                    16

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPFCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPFCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPFCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPFCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_HTTPFCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPFCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPFCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPFCO_GET_INPUT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_HTTPFCO_GET_PARAM)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  void*
(*PFN_HTTPFCO_GET_PARAM2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulSize
    );

typedef  ANSC_HANDLE
(*PFN_HTTPFCO_GET_FEO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_HTTPFCO_ADD_FEO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFeo
    );

typedef  ANSC_STATUS
(*PFN_HTTPFCO_DEL_FEO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_HTTPFCO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * HTML forms let you create a variety of user interface controls to collect input on a web page.
 * Each of the controls typically has a name and a value, where the name is specified in the HTML
 * and the value comes either from the HTML or by means of user input. The entire form is asso-
 * ciated with the URL of a program that will process the data, and when the user submits the form
 * (usually by pressing a button), the names and values of the controls are sent to the designated
 * URL as a string of the form:
 *
 *      Name1=Value1&Name2=Value2...NameN=ValueN
 *
 * This string can be sent to the designated program in one of two ways. The first, which uses the
 * HTTP GET method, appends the string to the end of the specified URL, after a question mark. The
 * second way data can be sent is by the HTTP POST method. Here, the POST request line, the HTTP
 * request headers, and a blank line are first sent the server, and then the data string is sent
 * on the following line.
 */
#define  HTTP_FORM_CONTENT_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hUriParams;                                             \
    ANSC_HANDLE                     hFormInput;                                             \
    ULONG                           FormIndex;                                              \
    BOOL                            bActive;                                                \
                                                                                            \
    QUEUE_HEADER                    FeoTable[HTTP_FCO_FEO_TABLE_SIZE];                      \
    ANSC_LOCK                       FeoTableLock;                                           \
                                                                                            \
    PFN_HTTPFCO_GET_IF              GetHfpIf;                                               \
    PFN_HTTPFCO_SET_IF              SetHfpIf;                                               \
    PFN_HTTPFCO_RESET               Reset;                                                  \
                                                                                            \
    PFN_HTTPFCO_ENGAGE              Engage;                                                 \
    PFN_HTTPFCO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_HTTPFCO_GET_INPUT           GetUriParams;                                           \
    PFN_HTTPFCO_GET_INPUT           GetFormInput;                                           \
    PFN_HTTPFCO_GET_PARAM           GetFormParam;                                           \
    PFN_HTTPFCO_GET_PARAM2          GetFormParam2;                                          \
                                                                                            \
    PFN_HTTPFCO_GET_FEO             GetFormElement;                                         \
    PFN_HTTPFCO_ADD_FEO             AddFormElement;                                         \
    PFN_HTTPFCO_DEL_FEO             DelFormElement;                                         \
    PFN_HTTPFCO_DEL_ALL             DelAllFeos;                                             \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_FORM_CONTENT_OBJECT
{
    HTTP_FORM_CONTENT_CLASS_CONTENT
}
HTTP_FORM_CONTENT_OBJECT,  *PHTTP_FORM_CONTENT_OBJECT;

#define  ACCESS_HTTP_FORM_CONTENT_OBJECT(p)         \
         ACCESS_CONTAINER(p, HTTP_FORM_CONTENT_OBJECT, Linkage)


#endif
