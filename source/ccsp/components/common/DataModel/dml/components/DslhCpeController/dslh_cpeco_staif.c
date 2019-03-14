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

    module: dslh_cpeco_staif.c

        For DSLH
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions
        of the DSLH Stat Interface

        *   DslhCpecoStatIfConnectAcs
        *   DslhCpecoStatIfIncTcpSuccess
        *   DslhCpecoStatIfIncTcpFailure
        *   DslhCpecoStatIfIncTlsFailure
        *   DslhCpecoStatIfGetTcpSuccessCount
        *   DslhCpecoStatIfGetTcpFailureCount
        *   DslhCpecoStatIfGetTlsFailureCount
        *   DslhCpecoResetStats
        *   DslhCpecoStatIfGetLastConnectionTime
        *   DslhCpecoStatIfGetLastConnectionStatus
        *   DslhCpecoStatIfGetLastInformResponseTime
        *   DslhCpecoStatIfGetLastReceivedSPVTime

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        11/12/08    initial revision.
        02/12/09    add more statistics apis

**********************************************************************/


#include "dslh_cpeco_global.h"

