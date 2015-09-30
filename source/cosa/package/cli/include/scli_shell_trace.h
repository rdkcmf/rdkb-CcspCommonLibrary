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

#include "ccsp_trace.h"


#define  CcspCliTraceEmergency(msg)                         \
    CcspTraceEmergency2("CcspCli", msg)

#define  CcspCliTraceAlert(msg)                             \
    CcspTraceAlert2("CcspCli", msg)

#define  CcspCliTraceCritical(msg)                          \
    CcspTraceCritical2("CcspCli", msg)

#define  CcspCliTraceError(msg)                             \
    CcspTraceError2("CcspCli", msg)

#define  CcspCliTraceWarning(msg)                           \
    CcspTraceWarning2("CcspCli", msg)

#define  CcspCliTraceDebug(msg)                             \
    CcspTraceDebug2("CcspCli", msg)

#define  CcspCliTraceInfo(msg)                              \
    CcspTraceInfo2("CcspCli", msg)

#define  CcspCliTrace(msg)                                  \
    CcspCliTraceError(msg)

