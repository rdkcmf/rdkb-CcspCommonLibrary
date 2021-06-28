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

    MODULE:	CERT_STATUS.H

        For Certification Framework DLL,
        by Cisco Systems, Inc.

    ---------------------------------------------------------------

    FUNCTIONAL DESCRIPTION:

        This header file defines all the internal and external
        status codes.

    ---------------------------------------------------------------

    ENVIRONMENT:

        Win32 and MFC.

    ---------------------------------------------------------------

    AUTHOR:

        Xuchen Yang

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   07/01/98    initial revision

**********************************************************************/


#ifndef     _CERT_STATUS_
#define     _CERT_STATUS_


#define  AL_STATUS_CERT_ERROR_BASE                  0xFF008000L
#define  AL_STATUS_CERT_RESOURCES                   AL_STATUS_CERT_ERROR_BASE   + 0x0001L
#define  AL_STATUS_CERT_INVALID_SPD_SCALE           AL_STATUS_CERT_ERROR_BASE   + 0x0002L
#define  AL_STATUS_CERT_INVALID_HANDLE              AL_STATUS_CERT_ERROR_BASE   + 0x0003L
#define  AL_STATUS_CERT_INTERNAL_ERROR              AL_STATUS_CERT_ERROR_BASE   + 0x0004L
#define  AL_STATUS_CERT_UNEXPECTED_TAG              AL_STATUS_CERT_ERROR_BASE   + 0x0005L
#define  AL_STATUS_CERT_INVALID_LENGTH              AL_STATUS_CERT_ERROR_BASE   + 0x0006L
#define  AL_STATUS_CERT_INVALID_ENCODING_LENGTH     AL_STATUS_CERT_ERROR_BASE   + 0x0007L
#define  AL_STATUS_CERT_INVALID_ISSUER              AL_STATUS_CERT_ERROR_BASE   + 0x0008L
#define  AL_STATUS_CERT_INVALID_VERSION             AL_STATUS_CERT_ERROR_BASE   + 0x0009L
#define  AL_STATUS_CERT_NOT_SUPPORTED               AL_STATUS_CERT_ERROR_BASE   + 0x000AL
#define  AL_STATUS_CERT_INVALID_ALGORITHM           AL_STATUS_CERT_ERROR_BASE   + 0x000BL
#define  AL_STATUS_CERT_INVALID_ATTRIBUTE           AL_STATUS_CERT_ERROR_BASE   + 0x000CL
#define  AL_STATUS_CERT_INVALID_TIME_ENCODING       AL_STATUS_CERT_ERROR_BASE   + 0x000DL
#define  AL_STATUS_CERT_INVALID_SIGNATURE           AL_STATUS_CERT_ERROR_BASE   + 0x000EL
#define  AL_STATUS_CERT_INVALID_EXTENSION           AL_STATUS_CERT_ERROR_BASE   + 0x000FL
#define  AL_STATUS_CERT_INVALID_POLICY_QUALIFIER    AL_STATUS_CERT_ERROR_BASE   + 0x0010L
#define  AL_STATUS_CERT_ITEM_NOTAVAILABLE           AL_STATUS_CERT_ERROR_BASE   + 0x0011L
#define  AL_STATUS_CERT_INVALID_SIGNING_KEY         AL_STATUS_CERT_ERROR_BASE   + 0x0012L
#define  AL_STATUS_CERT_ITEM_EXISTS                 AL_STATUS_CERT_ERROR_BASE   + 0x0013L
#define  AL_STATUS_CERT_INVALID_VALIDITY            AL_STATUS_CERT_ERROR_BASE   + 0x0014L
#define  AL_STATUS_CERT_ROOTCA_UNAVAILABLE          AL_STATUS_CERT_ERROR_BASE   + 0x0015L
#define  AL_STATUS_CERT_INVALID_PATH                AL_STATUS_CERT_ERROR_BASE   + 0x0016L
#define  AL_STATUS_CERT_CHAIN_BROKEN                AL_STATUS_CERT_ERROR_BASE   + 0x0017L
#define  AL_STATUS_CERT_PATH_CONSTRAINTS_APPLIED    AL_STATUS_CERT_ERROR_BASE   + 0x0018L
#define  AL_STATUS_CERT_INVALID_CERTIFICATE         AL_STATUS_CERT_ERROR_BASE   + 0x0019L
#define  AL_STATUS_CERT_POLICY_UNACCEPTABLE         AL_STATUS_CERT_ERROR_BASE   + 0x001AL
#define  AL_STATUS_CERT_INHIBITED_POLICY_MAPPINGS   AL_STATUS_CERT_ERROR_BASE   + 0x001BL
#define  AL_STATUS_CERT_REQUIRE_EXPLICIT_POLICY     AL_STATUS_CERT_ERROR_BASE   + 0x001CL
#define  AL_STATUS_CERT_CERTIFICATE_UNAVAILABLE     AL_STATUS_CERT_ERROR_BASE   + 0x001DL
#define  AL_STATUS_CERT_KEYTYPE_MISMATCH            AL_STATUS_CERT_ERROR_BASE   + 0x001EL
#define  AL_STATUS_CERT_INVALID_KEY_USAGE           AL_STATUS_CERT_ERROR_BASE   + 0x001FL
#define  AL_STATUS_CERT_REVOKED                     AL_STATUS_CERT_ERROR_BASE   + 0x0020L
#define  AL_STATUS_CRL_EXPIRED                      AL_STATUS_CERT_ERROR_BASE   + 0x0021L


#endif
