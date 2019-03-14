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

    module:	dslh_ifo_mpa.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Dslh Management Parameter Access Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/06/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_IFO_MPA_
#define  _DSLH_IFO_MPA_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    DSLH MANAGEMENT PARAMETER ACCESS INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  DSLH_MPA_INTERFACE_NAME                    "dslhManagementParameterAccessIf"
#define  DSLH_MPA_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  BOOL
(*PFN_DSLHMPAIF_LOCK_ACCESS)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPAIF_UNLOCK_ACCESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPAIF_SET_COMMIT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        int*                        piStatus,
        int                         sessionId,
        ULONG                       writeID,
        BOOL                        bCommit
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPAIF_SET_VALUES)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        void*                       pParamValueArray,
        ULONG                       ulArraySize,
        int*                        piStatus,
        int                         sessionId,
        ULONG                       writeID,
        BOOL                        bCommit,
        char**                      ppInvalidParameterName
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPAIF_GET_VALUES)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        SLAP_STRING_ARRAY*          pParamNameArray,
		ULONG						uMaxEntry,
        void**                      ppParamValueArray,
        PULONG                      pulArraySize,
        unsigned int                writeID
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPAIF_GET_NAMES)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pParamPath,
        BOOL                        bNextLevel,
        void**                      ppParamInfoArray,
        PULONG                      pulArraySize
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPAIF_SET_ATTRIBS)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        void*                       pSetParamAttribArray,
        ULONG                       ulArraySize
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPAIF_GET_ATTRIBS)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        SLAP_STRING_ARRAY*          pParamNameArray,
		ULONG						uMaxEntry,
        void**                      ppParamAttribArray,
        PULONG                      pulArraySize
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPAIF_ADD_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pObjName,
        PULONG                      pulObjInsNumber,
        int*                        piStatus
    );

typedef  ANSC_STATUS
(*PFN_DSLHMPAIF_DEL_OBJ)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pAccessEntity,
        char*                       pObjName,
        int*                        piStatus
    );

/*
 * In each direction, the order of SOAP envelopes is defined independently from the number of enve-
 * lopes per HTTP post/response pair. Specifically, envelopes are ordered from first to last within
 * a single HTTP post/response and then between successive post/response pairs.
 *
 * On reception of SOAP requests from the ACS, the CPE MUST respond to each request in the order
 * they were received, where order is defined as described above. The definition of order places no
 * constraint on whether multiple responses are sent in a single HTTP post (if the ACS can accept
 * more than one envelope), or distributed over multiple HTTP posts.
 */
#define  DSLH_MPA_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_DSLHMPAIF_LOCK_ACCESS       LockWriteAccess;                                        \
    PFN_DSLHMPAIF_UNLOCK_ACCESS     UnlockWriteAccess;                                      \
                                                                                            \
    PFN_DSLHMPAIF_SET_VALUES        SetParameterValues;                                     \
    PFN_DSLHMPAIF_SET_COMMIT        SetCommit;                                              \
    PFN_DSLHMPAIF_GET_VALUES        GetParameterValues;                                     \
    PFN_DSLHMPAIF_GET_NAMES         GetParameterNames;                                      \
    PFN_DSLHMPAIF_SET_ATTRIBS       SetParameterAttributes;                                 \
    PFN_DSLHMPAIF_GET_ATTRIBS       GetParameterAttributes;                                 \
    PFN_DSLHMPAIF_ADD_OBJ           AddObject;                                              \
    PFN_DSLHMPAIF_DEL_OBJ           DeleteObject;                                           \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_MPA_INTERFACE
{
    DSLH_MPA_INTERFACE_CLASS_CONTENT
}
DSLH_MPA_INTERFACE,  *PDSLH_MPA_INTERFACE;

#define  ACCESS_DSLH_MPA_INTERFACE(p)               \
         ACCESS_CONTAINER(p, DSLH_MPA_INTERFACE, Linkage)


#endif
