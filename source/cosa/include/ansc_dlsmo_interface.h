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

    module:	ansc_dlsmo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Day Light Saving Manager Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Na Li

    ---------------------------------------------------------------

    revision:

        04/26/05    initial revision.

**********************************************************************/


#ifndef  _ANSC_DLSMO_INTERFACE_
#define  _ANSC_DLSMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_lco_interface.h"
#include "ansc_lco_external_api.h"

/***********************************************************
      PLATFORM INDEPENDENT TOD CLIENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define	 ANSC_DLSMO_SECONDS_ON_JAN01_1970	        0x83aa7e80	
#define	 ANSC_DLSMO_WDAY_ON_JAN01_1970	            4	  
#define	 ANSC_DLSMO_YEAR_1970	                    1970
#define	 ANSC_DLSMO_YEAR_1900	                    1900
#define  ANSC_DLSMO_DAYS_IN_WEEK                    7

#define	AnscDlsmoIfLeapyear(year)		                \
            ( ( (year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0) )? 1:0 )

#define	AnscDlsmoDaysInYear(year) 	                (AnscDlsmoIfLeapyear(year) ? 366 : 365)



/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_DLSMO_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DLSMO_SET_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulvalue
    );

typedef  ANSC_STATUS
(*PFN_DLSMO_OPERATION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_DLSMO_GET_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        TimezoneOffset
    );

typedef  ANSC_STATUS
(*PFN_DLSMO_ADD_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInputEntry
    );

typedef  ANSC_STATUS
(*PFN_DLSMO_DEL_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        TimezoneOffset
    );

typedef  ANSC_STATUS
(*PFN_DLSMO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DLSMO_ENUM_ENTRY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArray,
        PULONG                      pulCount
    );

typedef  BOOL
(*PFN_DLSMO_CHECK_DLS1)
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        TimezoneOffset,
        ANSC_HANDLE                 hLocalTime
    );

typedef  BOOL
(*PFN_DLSMO_GET_WEEKDAY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLocalTime,
        PULONG                      pWeekNum
    );

/*******************************************************************************************
                    ANSC_DLS_MANAGER_OBJECT

  This object provideS an API for checking whether a given date is day light saving or not.
  It cannot handle canlendar older than the year 1970.

    AddDlsEntry                     If the entry with the same offset exists, its content is
                                    set by the input. The input type is PANSC_DST_MAP_ENTRY,
                                    and it is allocated and released by the caller.
    EnumDlsEntries                  Enumerate all entries. The caller allocates the array.
                                    Its type is PANSC_DST_MAP_ENTRY.
    IsDayLightSaving1               the hLocalTime type is PANSC_UNIVERSAL_TIME.
    IsDayLightSaving2               ulUTCSeconds is the seconds from 1900.
 *******************************************************************************************/
#define  ANSC_DLS_MANAGER_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSCLCO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    BOOL                            bActive;                                                \
    QUEUE_HEADER                    DlsMapQueue;                                            \
    ANSC_SPINLOCK                   DlsMapQueueSpinLock;                                    \
                                                                                            \
    PFN_DLSMO_OPERATION             Engage;                                                 \
    PFN_DLSMO_OPERATION             Cancel;                                                 \
    PFN_DLSMO_OPERATION             Reset;                                                  \
                                                                                            \
    PFN_DLSMO_GET_ENTRY             GetDlsEntryByOffset;                                    \
    PFN_DLSMO_ADD_ENTRY             AddDlsEntry;                                            \
    PFN_DLSMO_DEL_ENTRY             DelDlsEntryByOffset;                                    \
    PFN_DLSMO_DEL_ALL               DelAllDlsEntries;                                       \
    PFN_DLSMO_GET_ULONG             GetDlsEntryCount;                                       \
    PFN_DLSMO_ENUM_ENTRY            EnumDlsEntries;                                         \
                                                                                            \
    PFN_DLSMO_CHECK_DLS1            IsDayLightSaving;                                       \
    PFN_DLSMO_GET_WEEKDAY           GetWeekDay;                                             \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_DLS_MANAGER_OBJECT
{
    ANSC_DLS_MANAGER_CLASS_CONTENT
}
ANSC_DLS_MANAGER_OBJECT,  *PANSC_DLS_MANAGER_OBJECT;

#define  ACCESS_ANSC_DLS_MANAGER_OBJECT(p)           \
         ACCESS_CONTAINER(p, ANSC_DLS_MANAGER_OBJECT, Linkage)

/*******************************************************************************************
                        ANSC_DST_MAP_ENTRY

    This entry contains the map between the timezone offset and the daylight saving time.

********************************************************************************************/

typedef  struct
_ANSC_DST_MAP_ENTRY
{
    SINGLE_LINK_ENTRY               Linkage;                                               

    LONG                            TimezoneOffset;     /* in seconds */
    ANSC_UNIVERSAL_TIME             DstStartTime;
    ANSC_UNIVERSAL_TIME             DstEndTime;
}
ANSC_DST_MAP_ENTRY,  *PANSC_DST_MAP_ENTRY;

#define  ACCESS_ANSC_DST_MAP_ENTRY(p)           \
         ACCESS_CONTAINER(p, ANSC_DST_MAP_ENTRY, Linkage)

#endif
