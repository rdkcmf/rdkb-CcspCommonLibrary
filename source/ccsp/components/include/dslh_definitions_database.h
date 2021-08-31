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

    module:	dslh_definitions_database.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the data structures used in parameter
        database management.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/02/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_DEFINITIONS_DATABASE_
#define  _DSLH_DEFINITIONS_DATABASE_

#include "slap_definitions.h"

/***********************************************************
       CENTRALIZED CWMP DATABASE STRUCTURE DEFINITIONS
***********************************************************/
/* the string of table count in TR69 */
#define  TR69_NUMBER_OF_ENTRIES_STRING                      "NumberOfEntries"

/*
 * Each CPE parameter is associated with an array of zero or more entities for which write access
 * is granted. If there're no entities, access is only allowed from an ACS. At present, only one
 * type of entity is defined and that can be included:
 *
 *      - "Subscriber": indicates write access by a device controlled by the the sub-
 *                      scriber on the LAN, such as via the LAN-Side DSL CPE Configura-
 *                      tion protocol or via UPnP.
 */
#define  DSLH_MPA_ENTITY_ACS                        "ACS"
#define  DSLH_MPA_ENTITY_Subscriber                 "Subscriber"
#define  DSLH_MPA_ENTITY_System                     "System"        /* software itself */

#define  DSLH_MPA_ACCESS_CONTROL_WEBUI              0x00000001
#define  DSLH_MPA_ACCESS_CONTROL_SNMP               0x00000002
#define  DSLH_MPA_ACCESS_CONTROL_ACS                0x00000004
#define  DSLH_MPA_ACCESS_CONTROL_CLI                0x00000008
#define  DSLH_MPA_ACCESS_CONTROL_WEBPA              0x0000000A
#define  DSLH_MPA_ACCESS_CONTROL_XPC                0x0000000B
#define  DSLH_MPA_ACCESS_CONTROL_NOTIFY_COMP        0x0000000C
#define  DSLH_MPA_ACCESS_CONTROL_CLIENTTOOL         0x00000010
#define  DSLH_MPA_ACCESS_CONTROL_LM                 0x00000020
#define  DSLH_MPA_ACCESS_CONTROL_WIFI               0x00000040
#define  DSLH_MPA_ACCESS_CONTROL_XMPP               0x00000080
#define  DSLH_MPA_ACCESS_CONTROL_WEBCONFIG          0x00000100
#define  DSLH_MAP_ACCESS_CONTROL_ANY                0xFFFFFFFF

/*
 * Parameter names used a hierarchical form similar to a directory tree. The name of a particular
 * Parameter is represented by the concatenation of each successive node in the hierarchical sepa-
 * rated with a '.', starting at the trunk of the hierarchy and leading to the leaves. When specify-
 * ing a partial path, indicating an intermediate node in the hierarchy, the trailing '.' is always
 * used as the last character.
 */
#define  DSLH_PARAM_NAME_SEPARATORS                 "."
#define  DSLH_DATA_TYPE_TOKEN_SEPARATORS            "()[]:"
#define  DSLH_DATA_TYPE_TOKEN_ALPHABETS             "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define  DSLH_DATA_TYPE_TOKEN_BASE64                "base64"
#define  DSLH_DATA_TYPE_LIMIT_SEPARATORS            "()[]:"
#define  DSLH_DATA_TYPE_LIMIT_ALPHABETS             "+-0123456789Kk"
#define  DSLH_DATA_TYPE_ENUMS_ALPHABETS             "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ _.+-0123456789"
#define  DSLH_SYNTAX_TOKEN_SEPARATORS               "/"

/*
 * The CWMP (CPE WAN Management Protocol) parameter management model is implemented by aggregating
 * the data-oriented service calls provided by different SLAP objects (which may be located in diff-
 * erent processes). Even though the real configuration parameters are managed in a distributed
 * fashion, it's useful to maintain a centralized parameter database in a tree-like structure. For
 * example, this database will maintain the attribute and access control information for each para-
 * meter while the target module doesn't have to be aware of it.
 */
#define  DSLH_CWMP_DATA_TYPE_unspecified            0
#define  DSLH_CWMP_DATA_TYPE_object                 1
#define  DSLH_CWMP_DATA_TYPE_string                 2
#define  DSLH_CWMP_DATA_TYPE_int                    3
#define  DSLH_CWMP_DATA_TYPE_unsignedInt            4
#define  DSLH_CWMP_DATA_TYPE_boolean                5
#define  DSLH_CWMP_DATA_TYPE_dateTime               6
#define  DSLH_CWMP_DATA_TYPE_base64                 7

#define  DSLH_CWMP_DATA_NAME_object                 "object"
#define  DSLH_CWMP_DATA_NAME_string                 "string"
#define  DSLH_CWMP_DATA_NAME_int                    "int"
#define  DSLH_CWMP_DATA_NAME_unsignedInt            "unsignedInt"
#define  DSLH_CWMP_DATA_NAME_boolean                "boolean"
#define  DSLH_CWMP_DATA_NAME_dateTime               "dateTime"
#define  DSLH_CWMP_DATA_NAME_base64                 "base64"

#define  DSLH_CWMP_DATA_INVALID_VALUE_TYPE          1

#define  DSLH_CWMP_NOTIFY_STATUS_configurable       1
#define  DSLH_CWMP_NOTIFY_STATUS_alwaysOn           2
#define  DSLH_CWMP_NOTIFY_STATUS_alwaysOff          3
#define  DSLH_CWMP_NOTIFY_STATUS_configActive       4

#define  DSLH_CWMP_BS_UPDATE_firmware               1
#define  DSLH_CWMP_BS_UPDATE_rfcUpdate              2
#define  DSLH_CWMP_BS_UPDATE_allUpdate              3

typedef  SLAP_STRING         (*PFN_DSLHPARAM_GET_STRING)(ANSC_HANDLE  hThisObject);
typedef  SLAP_INT            (*PFN_DSLHPARAM_GET_INT   )(ANSC_HANDLE  hThisObject);
typedef  SLAP_UINT32         (*PFN_DSLHPARAM_GET_UINT32)(ANSC_HANDLE  hThisObject);
typedef  SLAP_BOOL           (*PFN_DSLHPARAM_GET_BOOL  )(ANSC_HANDLE  hThisObject);
typedef  SLAP_HANDLE         (*PFN_DSLHPARAM_GET_HANDLE)(ANSC_HANDLE  hThisObject);
typedef  SLAP_UCHAR_ARRAY*   (*PFN_DSLHPARAM_GET_BUFFER)(ANSC_HANDLE  hThisObject);
typedef  SLAP_UINT32_ARRAY*  (*PFN_DSLHPARAM_GET_ULLIST)(ANSC_HANDLE  hThisObject);

typedef  BOOL                (*PFN_DSLHPARAM_TST_STRING)(ANSC_HANDLE  hThisObject, SLAP_STRING         var_string);
typedef  BOOL                (*PFN_DSLHPARAM_TST_INT   )(ANSC_HANDLE  hThisObject, SLAP_INT            var_int   );
typedef  BOOL                (*PFN_DSLHPARAM_TST_UINT32)(ANSC_HANDLE  hThisObject, SLAP_UINT32         var_uint32);
typedef  BOOL                (*PFN_DSLHPARAM_TST_BOOL  )(ANSC_HANDLE  hThisObject, SLAP_BOOL           var_bool  );
typedef  BOOL                (*PFN_DSLHPARAM_TST_HANDLE)(ANSC_HANDLE  hThisObject, SLAP_HANDLE         var_handle);
typedef  BOOL                (*PFN_DSLHPARAM_TST_BUFFER)(ANSC_HANDLE  hThisObject, SLAP_UCHAR_ARRAY*   var_buffer);
typedef  BOOL                (*PFN_DSLHPARAM_TST_ULLIST)(ANSC_HANDLE  hThisObject, SLAP_UINT32_ARRAY*  var_ullist);

typedef  ANSC_STATUS         (*PFN_DSLHPARAM_SET_STRING)(ANSC_HANDLE  hThisObject, SLAP_STRING         var_string);
typedef  ANSC_STATUS         (*PFN_DSLHPARAM_SET_INT   )(ANSC_HANDLE  hThisObject, SLAP_INT            var_int   );
typedef  ANSC_STATUS         (*PFN_DSLHPARAM_SET_UINT32)(ANSC_HANDLE  hThisObject, SLAP_UINT32         var_uint32);
typedef  ANSC_STATUS         (*PFN_DSLHPARAM_SET_BOOL  )(ANSC_HANDLE  hThisObject, SLAP_BOOL           var_bool  );
typedef  ANSC_STATUS         (*PFN_DSLHPARAM_SET_HANDLE)(ANSC_HANDLE  hThisObject, SLAP_HANDLE         var_handle);
typedef  ANSC_STATUS         (*PFN_DSLHPARAM_SET_BUFFER)(ANSC_HANDLE  hThisObject, SLAP_UCHAR_ARRAY*   var_buffer);
typedef  ANSC_STATUS         (*PFN_DSLHPARAM_SET_ULLIST)(ANSC_HANDLE  hThisObject, SLAP_UINT32_ARRAY*  var_ullist);

typedef  struct
_DSLH_CWMP_PARAM_DESCR
{
    char*                           Name;
    char*                           DataType;               /* e.g. "string(64)", "unsignedint[0:4095]"... etc     */
    char*                           Syntax;                 /* e.g. "uint32/ip4_addr", "ucharArray/mac_addr", etc  */
    BOOL                            bPersistent;            /* if FALSE, parameter value is not saved in registry  */
    BOOL                            bWritable;              /* indicates whether the parameter is write-able       */
    ULONG                           NotifyStatus;           /* three options: "on", "off", and "configurable"      */
    BOOL                            bThresholdEnabled;      /* whether notification with threshold is enabled      */
    int                             NotifyThresholdMin;     /* min value of notify threshold                       */
    int                             NotifyThresholdMax;     /* max value of notify threshold                       */
    BOOL                            bInvisible;             /* indicates that this parameter is for internal use   */
    BOOL                            bNeedReboot;            /* if TRUE, CPE must be reboot after value is changed  */
    BOOL                            bMonitor;               /* if TRUE, parameter is monitored for user change     */
    ULONG                           bsUpdate;               /* who can update:"firmware", "rfcUpdate", "allUpdate" */
    ULONG                           CallEntry_GetValue;     /* function entry for retrieving the parameter value   */
    ULONG                           CallEntry_TstValue;     /* function entry for examining the parameter value    */
    ULONG                           CallEntry_SetValue;     /* function entry for configuring the parameter value  */
    char*                           PreventCharecters;      /* this is avoid input charecters for that string ex: <>': */


}
DSLH_CWMP_PARAM_DESCR,  *PDSLH_CWMP_PARAM_DESCR;

#define  DslhCwmpCleanParamDescr(param_descr)                                               \
         {                                                                                  \
            if ( param_descr->Name )                                                        \
            {                                                                               \
                AnscFreeMemory(param_descr->Name);                                          \
                                                                                            \
                param_descr->Name = NULL;                                                   \
            }                                                                               \
                                                                                            \
            if ( param_descr->DataType )                                                    \
            {                                                                               \
                AnscFreeMemory(param_descr->DataType);                                      \
                                                                                            \
                param_descr->DataType = NULL;                                               \
            }                                                                               \
                                                                                            \
            if ( param_descr->Syntax )                                                      \
            {                                                                               \
                AnscFreeMemory(param_descr->Syntax);                                        \
                                                                                            \
                param_descr->Syntax = NULL;                                                 \
            }                                                                               \
                                                                                            \
            if ( param_descr->PreventCharecters )                                           \
            {                                                                               \
                AnscFreeMemory(param_descr->PreventCharecters);                             \
                                                                                            \
                param_descr->PreventCharecters = NULL;                                      \
            }                                                                               \
         }

#define  DslhCwmpFreeParamDescr(param_descr)                                                \
         {                                                                                  \
            DslhCwmpCleanParamDescr(param_descr);                                           \
            AnscFreeMemory         (param_descr);                                           \
         }

#define  DSLH_CWMP_OBJECT_TYPE_reserved             0
#define  DSLH_CWMP_OBJECT_TYPE_regular              1
#define  DSLH_CWMP_OBJECT_TYPE_table                2

typedef  ANSC_HANDLE  (*PFN_DSLHOBJCO_CREATE)(ANSC_HANDLE  hContainerContext, ANSC_HANDLE  hOwnerContext, ANSC_HANDLE  hAnscReserved);

typedef  struct
_DSLH_CWMP_OBJECT_DESCR
{
    char*                           Name;
    ULONG                           Type;
    BOOL                            bWritable;
    BOOL                            bInvisible;
    BOOL                            bDynamic;
    BOOL                            bGroupCommit;
    ULONG                           MaxInstanceNumber;

    void*                           pfnObjcoConstructor;
    void*                           hParamArray;
    void*                           hDslhTr69If;
    void*                           hContextToController;
}
DSLH_CWMP_OBJECT_DESCR,  *PDSLH_CWMP_OBJECT_DESCR;

#define  DslhCwmpCleanObjectDescr(object_descr)                                             \
         {                                                                                  \
            if ( object_descr->Name )                                                       \
            {                                                                               \
                AnscFreeMemory(object_descr->Name);                                         \
                                                                                            \
                object_descr->Name = NULL;                                                  \
            }                                                                               \
            if ( object_descr->hDslhTr69If )                                                \
            {                                                                               \
                AnscFreeMemory(object_descr->hDslhTr69If);                                  \
                                                                                            \
                object_descr->hDslhTr69If = NULL;                                           \
            }                                                                               \
         }

#define  DslhCwmpFreeObjectDescr(object_descr)                                              \
         {                                                                                  \
            DslhCwmpCleanObjectDescr(object_descr);                                         \
            AnscFreeMemory          (object_descr);                                         \
         }


#endif
