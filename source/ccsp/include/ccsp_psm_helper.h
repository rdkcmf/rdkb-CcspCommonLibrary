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

#ifndef CCSP_PSM_H
#define CCSP_PSM_H

#include "ccsp_base_api.h"
#include "slap_definitions.h"

#define    PSM_TRUE                     "1"
#define    PSM_FALSE                    "0"

#ifdef PSM_SLAP_VAR

int PSM_Set_Record_Value
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName,
    unsigned int const          ulRecordType,
    PSLAP_VARIABLE              pValue
);

int PSM_Get_Record_Value
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName,
    unsigned int*               ulRecordType,
    PSLAP_VARIABLE              pValue
);

#endif
int PSM_Del_Record
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName
);


//caller need free ppInstanceArray
int PsmGetNextLevelInstances
(
   void*                        bus_handle,
   char const * const           pSubSystemPrefix,
   char const * const           pParentPath, 
   unsigned int*                pulNumInstance, 
   unsigned int**               ppInstanceArray
);


//caller need free ppRecArray
int PsmEnumRecords
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pParentPath,
    dbus_bool                   nextLevel,
    unsigned int *              pulNumRec,
    PCCSP_BASE_RECORD*          ppRecArray
);


int PSM_Set_Record_Value2
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName,
    unsigned int const          ulRecordType,
    char const * const          pVal
);



int PSM_Get_Record_Value2
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName,
    unsigned int*               ulRecordType,
    char**                      pValue
);

int PSM_Reset_UserChangeFlag
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName
);

#endif
