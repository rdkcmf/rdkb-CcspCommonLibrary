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

    module:	bmc2_comto_interface.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bmc2 Com Terminal Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/23/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_COMTO_INTERFACE_
#define  _BMC2_COMTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "bmc2_properties.h"


/***********************************************************
        BMC2 COM TERMINAL COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BMC2COMTO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BMC2COMTO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_BMC2COMTO_GET_USER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_SET_USER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pUserIdentifier
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_SET_PTDN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSeudoTermDevName
    );

typedef  ULONG
(*PFN_BMC2COMTO_GET_PERM)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_SET_PERM)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPermission
    );

typedef  ULONG
(*PFN_BMC2COMTO_GET_MAXN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_SET_MAXN)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMaxNumber
    );

typedef  BOOL
(*PFN_BMC2COMTO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_ABORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_ACQ_ACCESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_REL_ACCESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_BMC2COMTO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BMC2COMTO_GET_COMDO1)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_ADD_COMDO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComDomain
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_DEL_COMDO1)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_DEL_COMDO2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComDomain
    );

typedef  ANSC_STATUS
(*PFN_BMC2COMTO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The Bmc2 Com Terminal Object encapsulates the implementation of a Virtual Terminal. The Shell
 * program creates a Virtual Terminal for each login user. Multiple Virtual Terminals share the
 * same backend object access layer (i.e. SLAP). Each Virtual Terminal can only be associated with
 * a single user through its life cycle. The BMC Input and Output objects are unique within each
 * Virtual Terminal.
 */
#define  BMC2_COM_TERMINAL_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BMC2_COM_TERMINAL_PROPERTY      Property;                                               \
    ANSC_HANDLE                     hBmc2IceIf;                                             \
    ANSC_HANDLE                     hBmc2EnvController;                                     \
    ANSC_LOCK                       AccessLock;                                             \
    char*                           pPseudoTermDevName;                                     \
    BOOL                            bActive;                                                \
                                                                                            \
    ANSC_HANDLE                     hSlapContainerTerminal;                                 \
    ANSC_HANDLE                     hSlapBmc2Terminal;                                      \
                                                                                            \
    SLIST_HEADER                    ComdoSList;                                             \
    ANSC_LOCK                       ComdoSListLock;                                         \
                                                                                            \
    PFN_BMC2COMTO_GET_IF            GetBmc2IceIf;                                           \
    PFN_BMC2COMTO_SET_IF            SetBmc2IceIf;                                           \
    PFN_BMC2COMTO_GET_CONTEXT       GetBmc2EnvController;                                   \
    PFN_BMC2COMTO_SET_CONTEXT       SetBmc2EnvController;                                   \
    PFN_BMC2COMTO_GET_USER          GetUserIdentifier;                                      \
    PFN_BMC2COMTO_SET_USER          SetUserIdentifier;                                      \
    PFN_BMC2COMTO_GET_PERM          GetUserPermission;                                      \
    PFN_BMC2COMTO_SET_PERM          SetUserPermission;                                      \
    PFN_BMC2COMTO_GET_MAXN          GetMaxLineNumber;                                       \
    PFN_BMC2COMTO_SET_MAXN          SetMaxLineNumber;                                       \
    PFN_BMC2COMTO_GET_MAXN          GetMaxColumnNumber;                                     \
    PFN_BMC2COMTO_SET_MAXN          SetMaxColumnNumber;                                     \
    PFN_BMC2COMTO_GET_BOOL          GetReadOnly;                                            \
    PFN_BMC2COMTO_SET_BOOL          SetReadOnly;                                            \
    PFN_BMC2COMTO_SET_PTDN          SetPseudoTermDevName;                                   \
                                                                                            \
    PFN_BMC2COMTO_GET_PROPERTY      GetProperty;                                            \
    PFN_BMC2COMTO_SET_PROPERTY      SetProperty;                                            \
    PFN_BMC2COMTO_RESET             ResetProperty;                                          \
    PFN_BMC2COMTO_RESET             Reset;                                                  \
                                                                                            \
    PFN_BMC2COMTO_ACQ_ACCESS        AcqAccess;                                              \
    PFN_BMC2COMTO_REL_ACCESS        RelAccess;                                              \
    PFN_BMC2COMTO_ENGAGE            Engage;                                                 \
    PFN_BMC2COMTO_CANCEL            Cancel;                                                 \
    PFN_BMC2COMTO_SETUP_ENV         SetupEnv;                                               \
    PFN_BMC2COMTO_CLOSE_ENV         CloseEnv;                                               \
                                                                                            \
    PFN_BMC2COMTO_GET_COUNT         GetDomainCount;                                         \
    PFN_BMC2COMTO_GET_COMDO1        GetCurComDomain;                                        \
    PFN_BMC2COMTO_ADD_COMDO         AddComDomain;                                           \
    PFN_BMC2COMTO_DEL_COMDO1        DelCurComDomain;                                        \
    PFN_BMC2COMTO_DEL_COMDO2        DelComDomain;                                           \
    PFN_BMC2COMTO_DEL_ALL           DelAllComDomains;                                       \
    /* end of object class content */                                                       \

typedef  struct
_BMC2_COM_TERMINAL_OBJECT
{
    BMC2_COM_TERMINAL_CLASS_CONTENT
}
BMC2_COM_TERMINAL_OBJECT,  *PBMC2_COM_TERMINAL_OBJECT;

#define  ACCESS_BMC2_COM_TERMINAL_OBJECT(p)         \
         ACCESS_CONTAINER(p, BMC2_COM_TERMINAL_OBJECT, Linkage)


#endif
