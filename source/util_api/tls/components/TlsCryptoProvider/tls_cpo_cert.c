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

    module:	tls_cpo_cert.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced certificate handling
        functions of the TLS Crypto Provider Object.

        *   TlsCpoAuthenticateCertChain

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/22/03    initial revision.

**********************************************************************/


#include "tls_cpo_global.h"

/*
 *  The pki header files should be included
 *
 *  please get them from "\bisga\isc\asn.1\include" 
 */
#include "ansc_pki_global.h"

#define _NO_CA_EXPIRATION_CHECK 

/*********************************************************************************
 *
 *   static functions implemented by Bin
 *
 *********************************************************************************/
static ANSC_HANDLE
anscGetCertHandle
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    )
{
    PANSC_ASN1_CERTIFICATE          pAsnCert        = NULL;
    PUCHAR                          pBack           = (PUCHAR)raw_cert_data;

    if( raw_cert_size < 32 || raw_cert_data == NULL)
    {
        return NULL;
    }

    /* create the asn.1 certificate handle */
    pAsnCert = (PANSC_ASN1_CERTIFICATE)AnscAsn1CreateCertificate(NULL);

    if( pAsnCert == NULL)
    {
        return NULL;
    }

    /* decode it */
    if( ANSC_STATUS_SUCCESS != pAsnCert->DecodingData(pAsnCert, (PVOID*)&pBack))
    {
        pAsnCert->AsnFree(pAsnCert);

        return NULL;
    }

    return (ANSC_HANDLE)pAsnCert;
}

#if 0
static void
anscFreeCertHandle
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_CERTIFICATE          pThisObject  = (PANSC_ASN1_CERTIFICATE)hThisObject;

    if( pThisObject != NULL)
    {
        pThisObject->AsnFree(pThisObject);
    }
}
#endif

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TlsCpoAuthenticateCertChain
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 cert_chain
            );

    description:

        This function is called to authenticate a complete certificate
        chain.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 cert_chain
                Specifies the certificate chain to be authenticated.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
TlsCpoAuthenticateCertChain
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 cert_chain
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_X509_CERTIFICATE_CHAIN*    pCertChain   = (ANSC_X509_CERTIFICATE_CHAIN*)cert_chain;
    PANSC_ASN1_CERTIFICATE          pUserCert    = NULL;
    PANSC_ASN1_CERTIFICATE          pCACert      = NULL;
    PANSC_X509_CERTIFICATE          pX509Cert    = NULL;
    BOOL                            bVerified    = FALSE;
    LONG                            i;

    if( pCertChain->Depth < 1 )
    {
        AnscTrace("No certificate in the certchain.\n");

        return TRUE;
    }

    /* the last one is supposed to be aselfsigned ca cert */
    pX509Cert = pCertChain->Array[pCertChain->Depth - 1];

    if( !pX509Cert->bSelfSigned)
    {
        AnscTrace("The last one in the certchain is not a selfsigned ca cert, failed in TlsCpoAuthenticateCertChain().\n");
    
        return FALSE;
    }

    pCACert = (PANSC_ASN1_CERTIFICATE)anscGetCertHandle( pX509Cert->CertData, pX509Cert->CertSize);

    if( pCACert == NULL)
    {
        AnscTrace("Failed to decode the selfsigned root CA cert in TlsCpoAuthenticateCertChain().\n");

        return FALSE;
    }

#ifndef _NO_CA_EXPIRATION_CHECK

    if( pCACert->CheckValidity(pCACert, TRUE) != ANSC_STATUS_SUCCESS)
    {
        AnscTrace("The root selfsigned ca cert expired.\n");

        goto EXIT;
    }

#endif

    if(!pCACert->VerifySignature(pCACert, NULL))
    {
        AnscTrace("Failed to verify the signature of the selfsigned root ca cert.\n");

        goto EXIT;
    }

    if( pCertChain->Depth == 1)
    {
        bVerified = TRUE;

        goto EXIT;
    }

    /* check  the user cert one by one */
    for( i = pCertChain->Depth - 2; i >= 0; i --)
    {
        pX509Cert = pCertChain->Array[i];

        pUserCert = (PANSC_ASN1_CERTIFICATE)anscGetCertHandle( pX509Cert->CertData, pX509Cert->CertSize);

        if( pUserCert == NULL)
        {
            AnscTrace("Failed to decode cert at '%lu' in certchain.\n", i);

            goto EXIT;
        }
       
        if( pUserCert->CheckValidity(pUserCert, TRUE) != ANSC_STATUS_SUCCESS)
        {
            AnscTrace("The cert '%lu' in certchain expired.\n", i);

            goto EXIT;     
        }

        if( pCACert->VerifyChildCert(pCACert, pUserCert) != ANSC_STATUS_SUCCESS)
        {
            AnscTrace("Failed to verify cert '%lu' in certchain.\n", i);

            goto EXIT;
        }

        /* check the path valid or not */
        if( !pCACert->IsPathLengthValid(pCACert, i))
        {
            AnscTrace("The cert '%lu' is invalid, failed in IsPathLengthValid()\n", i);

            goto EXIT;
        }

        /* save the last ca cert */
        pCACert->AsnFree(pCACert);
        pCACert     = pUserCert;
        pUserCert   = NULL;
    }

    /* verified successfully */
    bVerified       = TRUE;

EXIT:

    /* free the memory */
    if( pCACert != NULL)
    {
        pCACert->AsnFree(pCACert);
    }

    if( pUserCert != NULL)
    {
        pUserCert->AsnFree(pUserCert);
    }

    return  bVerified;
}
