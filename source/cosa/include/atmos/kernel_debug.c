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

    module:	kernel_debug.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines.

    ---------------------------------------------------------------

    environment:

        atmos 7.1   and later

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        04/24/02    atmos port

**********************************************************************/

#include "kernel_base.h"
#include "kernel_debug.h"


/**********************************************************************
                          GLOBAL VARIABLES
**********************************************************************/

#ifdef   KERNEL_CHECKED_BUILD

    ULONG               gAnscDebugLevel             = /*KERNEL_DBG_LEVEL_VERBOSE;*/
                                                      /*KERNEL_DBG_LEVEL_TRACE_FLOW;*/
                                                      /*KERNEL_DBG_LEVEL_TEST;*/
                                                      KERNEL_DBG_LEVEL_WARNING;

    ULONG               gAnscDebugMask              = KERNEL_DBG_MASK_LEGACY
                                                      /*| KERNEL_DBG_MASK_GENERIC*/
                                                      | KERNEL_DBG_MASK_MEMORY
                                                      /*| KERNEL_DBG_MASK_LOCK*/
                                                      /*| KERNEL_DBG_MASK_EVENT*/;

#endif


ULONG                  gAnscPktLogLevel             = 0;
