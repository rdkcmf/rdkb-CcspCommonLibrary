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

    module:	cf_ako_interface.h

        For Content Filtering (CF),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the CF Access Key Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        09/10/02    initial revision.

**********************************************************************/


#ifndef  _CONT_FILTER_AKO_INTERFACE_
#define  _CONT_FILTER_AKO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ako_interface.h"
#include "ansc_ako_external_api.h"

#include "ansc_ifo_csp.h"
#include "cont_filter_ifo_csp.h"


/***********************************************************
      PLATFORM INDEPENDENT ACCESS KEY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  CONT_FILTER_ACCESS_KEY_NAME                "contFilterAccessKey"
#define  CONT_FILTER_ACCESS_KEY_OID                 ANSC_ACCESS_KEY_OID
#define  CONT_FILTER_ACCESS_KEY_TYPE                ANSC_ACCESS_KEY_TYPE

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

 
/*
 * One of the most popular problems every developer has to face in multi-task programming is to
 * synchronize the access to shared resources. Normally, such mechanism should be implemented by
 * the target module/object that owns the resource. As an exception, we use a separate object
 * instead to manage the access to the Configuration Interface exposed by system modules. The
 * benefit of doing this is the clear isolation between the core technology developer and the
 * system developer/packager.
 */
#define  CONT_FILTER_ACCESS_KEY_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSC_ACCESS_KEY_CLASS_CONTENT                                                           \
    /* start of object class content */                                                     \
                                                                                            \
    PFN_CFCSPIF_SET_ACCOUNT_ID      CspSetAccountId;                                        \
    PFN_CFCSPIF_SET_ACCOUNT_PSWD    CspSetAccountPassword;                                  \
    PFN_CFCSPIF_SET_USER_NAME       CspSetUserName;                                         \
    PFN_CFCSPIF_SET_USER_PSWD       CspSetUserPassword;                                     \
    PFN_CFCSPIF_SET_DEVICE_NAME     CspSetDeviceName;                                       \
    PFN_CFCSPIF_SET_DEVICE_ID       CspSetDeviceId;                                         \
    PFN_CFCSPIF_SET_DEVICE_TYPE     CspSetDeviceType;                                       \
    PFN_CFCSPIF_SET_CLIENT_VERSION  CspSetClientVersion;                                    \


typedef  struct
_CONT_FILTER_ACCESS_KEY_OBJECT
{
    CONT_FILTER_ACCESS_KEY_CLASS_CONTENT
}
CONT_FILTER_ACCESS_KEY_OBJECT,  *PCONT_FILTER_ACCESS_KEY_OBJECT;

#define  ACCESS_CONT_FILTER_ACCESS_KEY_OBJECT(p)          \
         ACCESS_CONTAINER(p, CONT_FILTER_ACCESS_KEY_OBJECT, Linkage)


#endif
