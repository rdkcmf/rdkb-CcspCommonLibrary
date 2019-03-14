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

    module:	ccsp_cr_definitions.h

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This header file defines CR exported structures and interfaces.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu 

    ---------------------------------------------------------------

    revision:

        05/24/2011    initial revision.

**********************************************************************/
#ifndef  _CCSP_CR_DEFINITIONS_H
#define  _CCSP_CR_DEFINITIONS_H

/**************************************************************

 CCSP CR (Component Registrar) Component Name Definition

***************************************************************/
#define CCSP_CR_NAME                    CCSP_DBUS_INTERFACE_CR

/**************************************************************

 CCSP CR (Component Registrar) Component Version

***************************************************************/
#define CCSP_CR_VERSION                 1  /* unsigned int */

/**************************************************************
 CCSP CR External APIs
***************************************************************/
ANSC_HANDLE
CcspCreateCR
  (
  );

void
CcspFreeCR
  (
        ANSC_HANDLE                 hCcspCR
  );

char*
CcspCrCloneString
  (
        const char*                 pString
  );

PVOID
CcspCrAllocateMemory
  (
        ULONG                       uSize
  );

void
CcspCrFreeMemory
  (
        PVOID                       pMemory
  );

#endif
