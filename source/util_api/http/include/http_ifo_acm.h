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

    module:	http_ifo_acm.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the HTTP Authentication Credential Management
        Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        10/07/05    initial revision.

**********************************************************************/


#ifndef  _HTTP_IFO_ACM_
#define  _HTTP_IFO_ACM_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
 HTTP AUTH CREDENTIAL MANAGEMENT INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

typedef  ANSC_STATUS
(*PFN_HTTPACMIF_GET_CREDENTIAL)
    (      
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pHostName,
        USHORT                      HostPort,
        PUCHAR                      pUriPath,
        PUCHAR*                     ppUserName,
        PUCHAR*                     ppPassword
    );

/*
 * HTTP Authentication Credential Management interface is defined to
 * retrieve user credential on the request URI.
 */
#define  HTTP_ACM_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_HTTPACMIF_GET_CREDENTIAL    GetCredential;                                          \
    /* end of object class content */                                                       \

typedef  struct
 
{
    HTTP_ACM_INTERFACE_CLASS_CONTENT
}
HTTP_ACM_INTERFACE,  *PHTTP_ACM_INTERFACE;

#define  ACCESS_HTTP_ACM_INTERFACE(p)               \
         ACCESS_CONTAINER(p, HTTP_ACM_INTERFACE, Linkage)


#endif

