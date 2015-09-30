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

    MODULE: rc2_h

        The header file for rc2 algorithm

    ---------------------------------------------------------------

    COPYRIGHT:

        Cisco Systems, Inc., 1999 ~ 2004

        All Rights Reserved.

    ---------------------------------------------------------------

    DESCRIPTION:

           * rc2_keyschedule
           * rc2_encrypt
           * rc2_decrypt

    ---------------------------------------------------------------

    ENVIRONMENT:

        platform independent

    ---------------------------------------------------------------

    AUTHOR:

        Bin Zhu

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   06/02/2004  initial revision

 **********************************************************************/

#define  RC2_BLOCK_SIZE                         8

void rc2_keyschedule
    ( 
        unsigned short xkey[64],
        const unsigned char *key,
        unsigned len,
        unsigned bits 
    );

/**********************************************************************\
* Encrypt an 8-byte block of plaintext using the given key.            *
\**********************************************************************/

void rc2_encrypt
    (
        const unsigned short xkey[64],
        const unsigned char *plain,
        unsigned char *cipher 
    );

/**********************************************************************\
* Decrypt an RC2_BLOCK_SIZE-byte block of ciphertext using the given key.          
\**********************************************************************/

void rc2_decrypt
    ( 
        const unsigned short xkey[64],
        unsigned char *plain,
        const unsigned char *cipher 
    );
