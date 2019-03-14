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

    module: pki_export_utils.h

        For Abstract Syntax Notation One (ASN.1)
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file implements the utility functions for 
        the PKI obejcts.

        *   PKIIsExtensionsObjectValid
        *   PKIFindAttributeWithOIDType
        *   PKIOIDStringToSignatureType
        *   PKISignTypeToOIDString
        *   PKIOIDStringToKeyType
        *   PKIKeyTypeToOIDString
        *   PKICopyStringAfterTrim

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        05/09/2002    initial revision.
        08/21/2002    add PKICopyStringAfterTrim;

**********************************************************************/


#ifndef  _PKI_EXPORT_UTILS_090_H
#define  _PKI_EXPORT_UTILS_090_H

/**************************************************************

  CHECK EXTENSIONS VALIDATION

  IMPLEMENTED IN "PKI_EXPORT_UTILS.C"

  *************************************************************/
BOOLEAN
PKIIsExtensionsObjectValid
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
FindAttributeWithOIDType
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    );

SIGNATURE_TYPE
PKIOIDStringToSignatureType
    (
        PCHAR                       pOIDString
    );

PCHAR
PKISignTypeToOIDString
    (
        SIGNATURE_TYPE              sigType
    );

PKI_KEY_TYPE
PKIOIDStringToKeyType
    (
        PCHAR                       pOIDString
    );

PCHAR
PKIKeyTypeToOIDString
    (
        PKI_KEY_TYPE                keyType
    );

/**************************************************************
 *  Copy String after trim
 *
 *  Ingore the space at the both side.
 *  Copy "value" from " value  ".
 ***************************************************************/
BOOLEAN
PKICopyStringAfterTrim
    (
        PCHAR                       pDest,
        PCHAR                       pSource
    );


#endif /*_PKI_EXPORT_UTILS_090_H*/

