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

#ifndef  _COSA_EEM_CDS_SERVICE_DEF_
#define  _COSA_EEM_CDS_SERVICE_DEF_


#include "cosa_pd194_smm_interface.h"

int
CosaEemInitCds
    (
        void*                                   hCosaContext,
        void*                                   hCosaPuaIf,
        PFN_COSA_DUStateChangeComplete_CB       pfnDuStateChangeCompleteCB,
        PFN_COSA_AutonDUStateChangeComplete_CB  pfnAutonDuStateChangeCompleteCB,
        PFN_COSA_Register_DMSL_CB               pfnRegisterDmslCB,
        PFN_COSA_Deregister_DMSL_CB             pfnDeregisterDmslCB
    );

int 
CosaEemChangeDUState
    (
        void*                       hCosaContext,               
        PCOSA_SMM_ChangeDUStateReq  pChangeDuStateReq
    );


#endif
