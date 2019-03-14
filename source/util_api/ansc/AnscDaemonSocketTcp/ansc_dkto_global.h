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

    module:	ansc_dkto_global.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Daemon Socket Tcp Object implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/08/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_DKTO_GLOBAL_
#define  _ANSC_DKTO_GLOBAL_


#include "ansc_platform.h"
#include "ansc_socket.h"
#include "ansc_socket_external_api.h"

#include "ansc_dkto_interface.h"
#include "ansc_dkto_external_api.h"
#include "ansc_dkto_internal_api.h"

#include "ansc_dsto_interface.h"
#include "ansc_dsto_external_api.h"
#include "ansc_deto_interface.h"
#include "ansc_deto_external_api.h"

#ifdef _ANSC_USE_OPENSSL_
#include <openssl/ssl.h>
#include "linux/user_openssl.h"
#else
#include "tls_definitions.h"
#endif

#endif
