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

    module:	ansc_slo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Sort List Objects. When an object needs to
        sort a tempt list according to some criterion, 
        (1)it first call SLO reset function to clear the state
        (2)then insert entries to the temp list maintained by the SLO. 
        (3)After that, the object should set the SLO compare function 
           to its own specific compare function
        (4)then call SLO sort function to get the sorted list.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Na Li

    ---------------------------------------------------------------

    revision:

        09/22/01    Xuechen Yang    template
        11/07/01    Na Li           initial revision

**********************************************************************/


#ifndef  _ANSC_SLO_INTERFACE_
#define  _ANSC_SLO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
      ANSC TUNNEL MANAGER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  ANSC_SLO_TEMP_LIST_SIZE                    200

#define  ANSC_SLO_COMPARE_ERROR                     0
#define  ANSC_SLO_COMPARE_A_LESS_B                  1
#define  ANSC_SLO_COMPARE_A_EQUAL_B                 2
#define  ANSC_SLO_COMPARE_A_LARGER_B                3

typedef  ANSC_STATUS
(*PFN_ANSCSLO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCSLO_INSERT_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntry
    );

typedef  ANSC_STATUS
(*PFN_ANSCSLO_REMOVE_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntry
    );

typedef  PANSC_HANDLE
(*PFN_ANSCSLO_SORT)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulEntryNum,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_ANSCSLO_COMPARE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEntryA,
        ANSC_HANDLE                 hEntryB,
        UCHAR*                      pResult,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_HANDLE
(*PFN_ANSCSLO_GET_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Index
    );


typedef  ULONG
(*PFN_ANSCSLO_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCSLO_SET_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       value
    );

typedef  ANSC_STATUS
(*PFN_ANSCSLO_ALLOCATE_LIST)
    (
        ANSC_HANDLE                 hThisObject
    );


typedef  ANSC_STATUS
(*PFN_ANSCSLO_SET_COMPARE)
    (
        ANSC_HANDLE                 hThisObject,
        PFN_ANSCSLO_COMPARE         pFunction,
        ANSC_HANDLE                 pCompareContext
    );

typedef  PANSC_HANDLE
(*PFN_ANSCSLO_SORT_IDX)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEntryIndex,
        PULONG                      pulEntryNum,
        ANSC_HANDLE                 hReserved
    );

/********************************************************************
                ANSC_SORT_LIST_ENTRY

  ListIndex                         Indicate the index of the entry
                                    in the sorted list
*********************************************************************/

#define  ANSC_SORT_LIST_ENTRY_CONTENT                                                       \
    ULONG                           ListIndex;

typedef  struct
_ANSC_SORT_LIST_ENTRY
{
    ANSC_SORT_LIST_ENTRY_CONTENT
}
ANSC_SORT_LIST_ENTRY,  *PANSC_SORT_LIST_ENTRY;

/********************************************************************************************
                        ANSC_SORT_LIST_OBJECT

  It is used for sorting objects according to their values. The Compare function is set
  to be the actual implementation before SortIncrease or SortDecrease can be called.

  SortIncreaseFirstOnly             Assume that the list is already sorted in increasing order, 
                                    and only the first entry should be sorted to right position
  SortDecreaseFirstOnly             Assume that the list is already sorted in decreasing order, 
                                    and only the first entry should be sorted to right position  
*********************************************************************************************/

#define  ANSC_SORT_LIST_CLASS_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hCompareContext;                                        \
    PANSC_SORT_LIST_ENTRY*          pSortList;                                              \
    ANSC_LOCK                       SortListLock;                                           \
    ANSC_LOCK                       ListOperationLock;                                      \
    ULONG                           MaxEntryNum;                                            \
    ULONG                           EntryNum;                                               \
                                                                                            \
    PFN_ANSCSLO_GET_ULONG           GetMaxEntryNum;                                         \
    PFN_ANSCSLO_SET_ULONG           SetMaxEntryNum;                                         \
    PFN_ANSCSLO_GET_ENTRY           GetEntryByIndex;                                        \
    PFN_ANSCSLO_GET_ULONG           GetCurrEntryNum;                                        \
    PFN_ANSCSLO_RESET               Reset;                                                  \
    PFN_ANSCSLO_SET_COMPARE         SetCompareFunc;                                         \
                                                                                            \
    PFN_ANSCSLO_ALLOCATE_LIST       AllocateEntryList;                                      \
    PFN_ANSCSLO_INSERT_ENTRY        InsertEntry;                                            \
    PFN_ANSCSLO_REMOVE_ENTRY        RemoveEntry;                                            \
                                                                                            \
    PFN_ANSCSLO_SORT                SortIncrease;                                           \
    PFN_ANSCSLO_SORT                SortDecrease;                                           \
    PFN_ANSCSLO_SORT                SortIncreaseFirstOnly;                                  \
    PFN_ANSCSLO_SORT                SortDecreaseFirstOnly;                                  \
    PFN_ANSCSLO_SORT_IDX            SortIncreaseSingleEntry;                                \
    PFN_ANSCSLO_SORT_IDX            SortDecreaseSingleEntry;                                \
    PFN_ANSCSLO_COMPARE             Compare;                                                \
     /* end of object class content */                                                      

typedef  struct
_ANSC_SORT_LIST_OBJECT
{
    ANSC_SORT_LIST_CLASS_CONTENT
}
ANSC_SORT_LIST_OBJECT,  *PANSC_SORT_LIST_OBJECT;

#define  ACCESS_ANSC_SORT_LIST_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_SORT_LIST_OBJECT, Linkage)



#endif
