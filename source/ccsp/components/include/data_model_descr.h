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

#ifndef  _DATA_MODEL_DESCR_
#define  _DATA_MODEL_DESCR_

#define  DATAMODEL_DATA_TYPE_BOOLEAN                0
#define  DATAMODEL_DATA_TYPE_UINT                   1
#define  DATAMODEL_DATA_TYPE_STRING                 2


#define  DSLH_FTM_DW_FILE_TYPE_firmwareUpgrade      1


#define  DOWNLOAD_PRIORITY_NORMAL                   0
#define  DOWNLOAD_PRIORITY_IMMEDIATE                1
#define  DOWNLOAD_PRIORITY_EMERGENCY                2


#define  DOWNLOAD_STATUS_STALE                      1
#define  DOWNLOAD_STATUS_INPROGRESS                 2
#define  DOWNLOAD_STATUS_SUCCESS                    3
#define  DOWNLOAD_STATUS_FAIL                       4


typedef  struct
_DSLH_CWMP_PARAM_DESCR
{
    char*                           Name;
    ULONG                           DataType;               /* e.g. "string(64)", "unsignedint[0:4095]"... etc     */
    
    BOOL                            bWritable;              /* indicates whether the parameter is write-able       */
    ULONG                           NotifyStatus;           /* three options: "on", "off", and "configurable"      */
    BOOL                            bInvisible;             /* indicates that this parameter is for internal use   */
    BOOL                            bNeedReboot;            /* if TRUE, CPE must be reboot after value is changed  */
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
         }

#define  DslhCwmpFreeParamDescr(param_descr)                                                \
         {                                                                                  \
            DslhCwmpCleanParamDescr(param_descr);                                           \
            AnscFreeMemory         (param_descr);                                           \
         }


#endif
