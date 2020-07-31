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

    module:	tls_cpo_decompress.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced decompress crypto
        functions of the TLS Crypto Provider Object.

        *   TlsCpoDecompress

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/23/03    initial revision.

**********************************************************************/


#include "tls_cpo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoDecompress
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       compress_algorithm,
                void*                       i_data,
                ULONG                       i_data_size,
                void*                       o_decomp,
                PULONG                      o_decomp_size
            );

    description:

        This function is called to perform the specified symmetric
        decompress algorithm.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       compress_algorithm
                Specifies the decompress algorithm to be performed.

                void*                       i_data
                Specifies the data buffer to be decompressed.

                ULONG                       i_data_size
                Specifies the size of the data buffer to be
                decompressed.

                void*                       o_decomp
                Specifies the output buffer of decompress result.

                PULONG                      o_decomp_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoDecompress
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       compress_algorithm,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_decomp,
        PULONG                      o_decomp_size
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(compress_algorithm);
    UNREFERENCED_PARAMETER(i_data);
    UNREFERENCED_PARAMETER(i_data_size);
    UNREFERENCED_PARAMETER(o_decomp);
    UNREFERENCED_PARAMETER(o_decomp_size);
    return  0;
}
