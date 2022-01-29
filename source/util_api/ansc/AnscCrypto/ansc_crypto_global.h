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

    module:	ansc_crypto_global.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Crypto Object implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/22/01    initial revision.
        06/02/04    zhubin added 3 rc2 related apis
        03/09/05    zhubin added uuid generation apis
        08/03/05    zhubin added zlib and zip support

**********************************************************************/


#ifndef  _ANSC_CRYPTO_GLOBAL_
#define  _ANSC_CRYPTO_GLOBAL_


#include "ansc_platform.h"

#include "ansc_crypto_interface.h"
#include "ansc_crypto_internal_api.h"
#include "ansc_crypto_external_api.h"

#ifdef   _ANSC_LZS_USED_
	#include "lzsc.h"
#endif


	#ifdef _ANSC_DES_USED_
	#include "openssl/des.h"
	#endif

	#ifdef _ANSC_MD2_USED_
		#include "openssl/md2.h"
	#endif

	#ifdef _ANSC_MD4_USED_
	#include "openssl/md4.h"
	#endif

	#include "openssl/md5.h"
	#include "openssl/sha.h"

	#ifdef _ANSC_IDEA_USED_
		#include "openssl/idea.h"
	#endif

	#ifdef _ANSC_BF_USED_
		#include "openssl/blowfish.h"
	#endif

	#ifdef _ANSC_RC4_USED_
		#include "rc4_2.h"
		#include "openssl/rc4.h"
	#endif

	#ifdef _ANSC_RC2_USED_
		#include "openssl/rc2.h"
	#endif

	#ifdef _ANSC_AES_USED_
		#include "openssl/aes.h"
	#endif


#endif
