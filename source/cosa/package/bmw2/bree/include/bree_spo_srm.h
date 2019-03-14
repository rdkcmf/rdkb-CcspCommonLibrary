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

    module:	bree_spo_srm.h

        For BSP Run-time Execution Engine (BREE),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BSP Run-time Execution Engine (BREE).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        01/17/04    initial revision.

**********************************************************************/


#ifndef  _BREE_SPO_SRM
#define  _BREE_SPO_SRM

/*
 *  !!! Note !!!
 *
 *    Please enable the symbol "_BREE_SPO_USE_SRMO" when you want
 *  to build all BMW 2.0 resource files into image. You may use
 *  the tool "Bmw2ResConverter" to cook all the resource files
 *  offline and put all output source files (2 .c and 2 .h 
 *  files) under the folder bisga/package/bmw2/bree/component/BreeSrm
 *
 */

/*
#define  _BREE_SPO_USE_SRMO
*/

#ifdef   _BREE_SPO_USE_SRMO

#include "bree_srm_interface.h"
#include "bree_srm_exported_api.h"

#ifdef   _ANSC_FILE_ZLIB_
#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"
#endif

    extern   PBREE_SRM_OBJECT           g_pBreeSrmo;

    #ifdef   _ANSC_FILE_ZLIB_
    extern   PANSC_CRYPTO_OBJECT        g_pCryptoObj;

#endif

int
BreeSpoCookedResOpen
    (
        const char                  *filename,
        int                         oflag,
        int                         pmode
    );

int
BreeSpoCookedResClose
    (
        int                         handle
    );

int 
BreeSpoCookedResRead
    (
        int                         handle, 
        void                        *buffer, 
        unsigned int                count 
    );

long 
BreeSpoGetCookedResLength
    ( 
        int                         handle 
    );

long 
BreeSpoCookedResSeek
    ( 
        int                         handle, 
        long                        offset, 
        int                         origin 
    );

int 
BreeSpoCookedResWrite
    ( 
        int                         handle, 
        const void                  *buffer, 
        unsigned int                count 
    );

#endif

#endif

