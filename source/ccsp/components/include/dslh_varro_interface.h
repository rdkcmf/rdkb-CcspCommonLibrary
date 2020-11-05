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

    module: dslh_varro_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh Var Record Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/23/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_VARRO_INTERFACE_
#define  _DSLH_VARRO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "dslh_properties.h"


/***********************************************************
         DSLH VAR RECORD COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DSLHVARRO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_DSLHVARRO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_DSLHVARRO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_DSLHVARRO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    );

typedef  BOOL
(*PFN_DSLHVARRO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  int
(*PFN_DSLHVARRO_GET_INT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_SET_INT)
    (
        ANSC_HANDLE                 hThisObject,
        int                         iType
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_LOAD_CONFIG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_SAVE_CONFIG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  SLAP_VARIABLE*
(*PFN_DSLHVARRO_GET_VALUE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_DSLHVARRO_TST_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              pNewValue
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_SET_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              pNewValue
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_SYN_VALUE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_ACTION2)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bFromAcs
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_NOTIFYVC)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHVARRO_SAVE_OLDVALUE)
    (
        ANSC_HANDLE                 hThisObject
    );  

/*
 * The CWMP (CPE WAN Management Protocol) parameter management model is implemented by aggregating
 * the data-oriented service calls provided by different SLAP objects (which may be located in diff-
 * erent processes). Even though the real configuration parameters are managed in a distributed
 * fashion, it's useful to maintain a centralized parameter database in a tree-like structure. For
 * example, this database will maintain the attribute and access control information for each para-
 * meter while the target module doesn't have to be aware of it.
 */
#define  DSLH_VAR_RECORD_CLASS_CONTENT                                                      \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hDslhObjRecord;                                         \
    ANSC_HANDLE                     hDslhVarEntity;                                         \
    ANSC_HANDLE                     hIrepFoParameter;                                       \
    int                             Notification;                                           \
    ULONG                           NotificationLimit;                                      \
	ULONG                           ReqSenderID;  											\
    ULONG                           RequesterID;                                            \
    char*                           AccessList;                                             \
    SLAP_VARIABLE*                  TempParamValue;                                         \
    SLAP_VARIABLE*                  TempParamValueTrans;                                    \
    SLAP_VARIABLE*                  OldParamValue;                                          \
                                                                                            \
    PFN_DSLHVARRO_SAVE_OLDVALUE     SaveOldValue;                                           \
                                                                                            \
    PFN_DSLHVARRO_GET_NAME          GetLastName;                                            \
    PFN_DSLHVARRO_GET_NAME          GetFullName;                                            \
    PFN_DSLHVARRO_SET_INT           SetNotification;                                        \
    PFN_DSLHVARRO_SET_NAME          SetAccessList;                                          \
    PFN_DSLHVARRO_RESET             Reset;                                                  \
    PFN_DSLHVARRO_GET_BOOL          IsRecordWritable;                                       \
    PFN_DSLHVARRO_NOTIFYVC          NotifyValueChanged;                                     \
                                                                                            \
    PFN_DSLHVARRO_ACTION            Remove;                                                 \
                                                                                            \
    PFN_DSLHVARRO_LOAD_CONFIG       LoadConfig;                                             \
    PFN_DSLHVARRO_SAVE_CONFIG       SaveConfig;                                             \
                                                                                            \
    PFN_DSLHVARRO_GET_VALUE         GetValue;                                               \
    PFN_DSLHVARRO_TST_VALUE         TstValue;                                               \
    PFN_DSLHVARRO_SET_VALUE         SetValue;                                               \
    PFN_DSLHVARRO_SYN_VALUE         SynValue;                                               \
    PFN_DSLHVARRO_ACTION2           CommitChange;                                           \
    PFN_DSLHVARRO_ACTION            CancelChange;                                           \
    BOOL                            bUserChanged;                                           \
    BOOL                            bUserChangeFlagLoaded;                                  \
    BOOL                            bAlreadySet;                                            \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_VAR_RECORD_OBJECT
{
    DSLH_VAR_RECORD_CLASS_CONTENT
}
DSLH_VAR_RECORD_OBJECT,  *PDSLH_VAR_RECORD_OBJECT;

#define  ACCESS_DSLH_VAR_RECORD_OBJECT(p)           \
         ACCESS_CONTAINER(p, DSLH_VAR_RECORD_OBJECT, Linkage)


#endif
