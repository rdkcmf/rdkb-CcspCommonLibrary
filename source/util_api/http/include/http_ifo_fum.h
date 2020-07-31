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

    module:	http_ifo_fum.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the File Upload Manager Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        08/23/07    initial revision.

**********************************************************************/


#ifndef  _HTTP_IFO_FUM_
#define  _HTTP_IFO_FUM_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    HTTP BODY CONTENT CARRIER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  HTTP_FUM_INTERFACE_ID                      0
#define  HTTP_FUM_INTERFACE_NAME                    "HttpFumInterface"

typedef  struct
_HTTP_FUM_ENTITY
{
    SINGLE_LINK_ENTRY               Linkage;
    PUCHAR                          pUri;
    ANSC_HANDLE                     hMdhIf;
}
HTTP_FUM_ENTITY, *PHTTP_FUM_ENTITY;

#define  ACCESS_HTTP_FUM_ENTITY(p)                                          \
         ACCESS_CONTAINER(p, HTTP_FUM_ENTITY, Linkage)

#define  HttpFumEntityAlloc(pEntity, pReqUri, hIf)                          \
    do {                                                                    \
        pEntity =                                                           \
            (PHTTP_FUM_ENTITY)AnscAllocateMemory                            \
                (                                                           \
                    sizeof(HTTP_FUM_ENTITY)                                 \
                );                                                          \
        if ( pEntity )                                                      \
        {                                                                   \
            pEntity->pUri   = (PUCHAR)AnscCloneString(pReqUri);                     \
            pEntity->hMdhIf = hIf;                                          \
        }                                                                   \
    } while (0)

#define  HttpFumEntityRemove(pEntity)                                       \
    do                                                                      \
    {                                                                       \
        if ( pEntity )                                                      \
        {                                                                   \
            AnscFreeMemory(pEntity->pUri);                                  \
            AnscFreeMemory(pEntity);                                        \
        }                                                                   \
    } while (0)


/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_HTTPFUMIF_REG_MDH)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pRequestUri,
        ANSC_HANDLE                 hMdhIf
    );

typedef  ANSC_HANDLE
(*PFN_HTTPFUMIF_GET_MDH)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pRequestUri
    );

/*
 * To facilitate HTTP file upload, this interface is defined to let user register
 * MDH interface (for saving file content and move it to destination later on) to
 * a given request URI and optionally with the field name.
 */
#define  HTTP_FUM_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_HTTPFUMIF_REG_MDH           RegMdhIf;                                               \
    PFN_HTTPFUMIF_GET_MDH           GetMdhIf;                                               \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_FUM_INTERFACE
{
    HTTP_FUM_INTERFACE_CLASS_CONTENT
}
HTTP_FUM_INTERFACE,  *PHTTP_FUM_INTERFACE;

#define  ACCESS_HTTP_FUM_INTERFACE(p)               \
         ACCESS_CONTAINER(p, HTTP_FUM_INTERFACE, Linkage)


#endif

