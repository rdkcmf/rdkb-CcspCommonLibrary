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

    module:	dslh_ifo_tr69.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the implementation of TR69 data model profiles.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        10/05/2010    initial revision.
        03/10/2011    redesign this interface since we need to support
                      calls from SLAP and full parameter names;

**********************************************************************/


#ifndef  _DSLH_IFO_TR69_
#define  _DSLH_IFO_TR69_

#include "slap_definitions.h"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

typedef  BOOL
(*PFN_DSLHTR69_GET_BULK)
    (
        ANSC_HANDLE                 hInsContext,
        char**                      pParamArray,
        PSLAP_VARIABLE*             ppVarArray,
        ULONG                       ulArraySize
    );

typedef  BOOL
(*PFN_DSLHTR69_SET_BULK)
    (
        ANSC_HANDLE                 hInsContext,
        char**                      pParamArray,
        PSLAP_VARIABLE*             ppVarArray,
        ULONG                       ulArraySize,
        PULONG                      pUlErrorIndex
    );

typedef  BOOL
(*PFN_DSLHTR69_GET_BOOL)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

typedef  BOOL
(*PFN_DSLHTR69_GET_INT)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

typedef  BOOL
(*PFN_DSLHTR69_GET_ULONG)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

typedef  ULONG
(*PFN_DSLHTR69_GET_STRING)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

typedef  BOOL
(*PFN_DSLHTR69_SET_INT)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

typedef  BOOL
(*PFN_DSLHTR69_SET_ULONG)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    );

typedef  BOOL
(*PFN_DSLHTR69_SET_BOOL)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

typedef  BOOL
(*PFN_DSLHTR69_SET_STRING)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

typedef  ULONG
(*PFN_DSLHTR69_GET_COUNT)
    (
        ANSC_HANDLE                 hInsContext
    );

typedef  BOOL
(*PFN_DSLHTR69_GET_STATUS)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pStatusName
    );


typedef  ANSC_HANDLE
(*PFN_DSLHTR69_GET_ENTRY)
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        PULONG                      pInsNumber
    );


typedef  ANSC_HANDLE
(*PFN_DSLHTR69_ADD_ENTRY)
    (
        ANSC_HANDLE                 hInsContext,
        PULONG                      pInsNumber
    );

typedef  ULONG
(*PFN_DSLHTR69_DEL_ENTRY)
    (
        ANSC_HANDLE                 hInsContext,
        ANSC_HANDLE                 hInstance
    );


typedef  BOOL
(*PFN_DSLHTR69_VALID)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        PULONG                      puLength
    );

typedef  ULONG
(*PFN_DSLHTR69_ACTION)
    (
        ANSC_HANDLE                 hInsContext
    );

typedef  BOOL
(*PFN_DSLHTR69_UPDATE)
    (
        ANSC_HANDLE                 hInsContext
    );

/* apis with full name */
typedef  ULONG
(*PFN_DSLHTR69_GET_COUNT1)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pParentName
    );

typedef  BOOL
(*PFN_DSLHTR69_GET_STATUS1)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pParentName,
        char*                       pStatusName        
    );

typedef  ANSC_HANDLE
(*PFN_DSLHTR69_GET_ENTRY1)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pParentName,
        ULONG                       nIndex,
        PULONG                      pInsNumber
    );

typedef  ANSC_HANDLE
(*PFN_DSLHTR69_ADD_ENTRY1)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pParentName,
        PULONG                      pInsNumber
    );

typedef  ULONG
(*PFN_DSLHTR69_DEL_ENTRY1)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pParentName,
        ANSC_HANDLE                 hInstance
    );

typedef  BOOL
(*PFN_DSLHTR69_VALID1)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pParentName,
        char*                       pReturnParamName,
        PULONG                      puLength
    );

typedef  ULONG
(*PFN_DSLHTR69_ACTION1)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pParentName
    );

typedef  BOOL
(*PFN_DSLHTR69_UPDATE1)
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pParentName
    );

/* definition of TR69 interface */
#define  DSLH_TR69_INTERFACE_CONTENT_1                                                  \
    PFN_DSLHTR69_GET_COUNT         GetEntryCount;                                       \
    PFN_DSLHTR69_GET_STATUS        GetEntryStatus;                                      \
    PFN_DSLHTR69_GET_ENTRY         GetEntry;                                            \
    PFN_DSLHTR69_ADD_ENTRY         AddEntry;                                            \
    PFN_DSLHTR69_DEL_ENTRY         DelEntry;                                            \
    PFN_DSLHTR69_VALID             Validate;                                            \
    PFN_DSLHTR69_ACTION            Commit;                                              \
    PFN_DSLHTR69_ACTION            Rollback;                                            \
    PFN_DSLHTR69_UPDATE            IsUpdated;                                           \
    PFN_DSLHTR69_ACTION            Synchronize;                                         \
    PFN_DSLHTR69_UPDATE            CheckInstance;                                       \
    /* end of object class content */                                                   \

typedef  struct
_DSLH_TR69_INTERFACE_1
{
    DSLH_TR69_INTERFACE_CONTENT_1
}
DSLH_TR69_INTERFACE_1,  *PDSLH_TR69_INTERFACE_1;

#define  DSLH_TR69_INTERFACE_CONTENT_2                                                  \
    PFN_DSLHTR69_GET_COUNT1        GetEntryCount;                                       \
    PFN_DSLHTR69_GET_STATUS1       GetEntryStatus;                                      \
    PFN_DSLHTR69_GET_ENTRY1        GetEntry;                                            \
    PFN_DSLHTR69_ADD_ENTRY1        AddEntry;                                            \
    PFN_DSLHTR69_DEL_ENTRY1        DelEntry;                                            \
    PFN_DSLHTR69_VALID1            Validate;                                            \
    PFN_DSLHTR69_ACTION1           Commit;                                              \
    PFN_DSLHTR69_ACTION1           Rollback;                                            \
    PFN_DSLHTR69_UPDATE1           IsUpdated;                                           \
    PFN_DSLHTR69_ACTION1           Synchronize;                                         \
    PFN_DSLHTR69_UPDATE1           CheckInstance;                                       \
    /* end of object class content */                                                   \

typedef  struct
_DSLH_TR69_INTERFACE_2
{
    DSLH_TR69_INTERFACE_CONTENT_2
}
DSLH_TR69_INTERFACE_2,  *PDSLH_TR69_INTERFACE_2;

typedef  struct
_DSLH_TR69_INTERFACE
{
    BOOL                           bUseFullName;  /* whether last name or full name used in the api */                                      
    ANSC_HANDLE                    hPoamObject;   /* POAM object if using SLAP call */
    PFN_DSLHTR69_GET_BOOL          GetEntryParamBool;                                   
    PFN_DSLHTR69_GET_INT           GetEntryParamInt;                                    
    PFN_DSLHTR69_GET_ULONG         GetEntryParamUlong;                                  
    PFN_DSLHTR69_GET_STRING        GetEntryParamString;                                 
    PFN_DSLHTR69_SET_BOOL          SetEntryParamBool;                                   
    PFN_DSLHTR69_SET_INT           SetEntryParamInt;                                    
    PFN_DSLHTR69_SET_ULONG         SetEntryParamUlong;                                  
    PFN_DSLHTR69_SET_STRING        SetEntryParamString;                                 
    PFN_DSLHTR69_GET_BULK          GetEntryBulk;
    PFN_DSLHTR69_SET_BULK          SetEntryBulk;
    union
    {
        DSLH_TR69_INTERFACE_1       MethodWithLastName;
        DSLH_TR69_INTERFACE_2       MethodWithFullName;
    }Methods;
}
DSLH_TR69_INTERFACE,  *PDSLH_TR69_INTERFACE;


#define  COSACloneTR69Interface(src, dst)                                       \
         {                                                                      \
            dst =                                                               \
                AnscAllocateMemory(sizeof(DSLH_TR69_INTERFACE));                \
                                                                                \
            if ( dst )                                                          \
            {                                                                   \
                AnscCopyMemory(dst, src, sizeof(DSLH_TR69_INTERFACE));          \
            }                                                                   \
         }

#endif
