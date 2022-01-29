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

    module: ansc_asn1_oid.c

        For ASN.1 OID object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by the ASN.1 base class "ASN1_OID"

        *   AnscAsn1OIDCreate
        *   AnscAsn1OIDFree
        *   AnscAsn1OIDInitialize
        *   AnscAsn1OIDClone
        *   AnscAsn1OIDCopyTo
        *   AnscAsn1OIDEqualsTo
        *   AnscAsn1OIDGetSizeOfEncoded
        *   AnscAsn1OIDDecodingData
        *   AnscAsn1OIDEncodingData
        *   AnscAsn1OIDDumpObject
        *   AnscAsn1OIDTraceObject
        *   AnscAsn1OIDReadyToEncode

        *   AnscAsn1OIDSetOIDValue
        *   AnscAsn1OIDGetOIDValue
        *   AnscAsn1OIDSetStringOIDValue
        *   AnscAsn1OIDGetStringOIDValue
        *   AnscAsn1OIDEqualToOIDStringValue

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/18/02    initial revision.
        09/24/02    an api to check equaling to OID value or not;

**********************************************************************/

#include "ansc_asn1_local.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1OIDCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs an ASN.1 object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
AnscAsn1OIDCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_OIDEN                pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_OIDEN)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_OIDEN)AnscAllocateMemory(sizeof(ANSC_ASN1_OIDEN));
    }

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_OID_ID;
    pMyObject->uType               = ASN1_OID_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1OIDCreate;
    pMyObject->AsnFree             = AnscAsn1OIDFree;
    
    AnscAsn1OIDInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_OID_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_OID_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1OIDFree
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAsn1OIDFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_OIDEN                pMyObject   = (PANSC_ASN1_OIDEN)hThisObject;

    if( hThisObject != NULL)
    {
        if( pMyObject->pAdvOID != NULL)
        {
            AnscFreeMemory(pMyObject->pAdvOID);
        }

    }

    AnscAsn1Free(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1OIDInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAsn1OIDInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_OIDEN                pMyObject    = (PANSC_ASN1_OIDEN)hThisObject;

    /*
     *  Call base initialization first
     */
    AnscAsn1Initialize((ANSC_HANDLE)pMyObject);

    /* set other functions */
    pMyObject->Clone                = AnscAsn1OIDClone;
    pMyObject->CopyTo               = AnscAsn1OIDCopyTo;
    pMyObject->EqualsTo             = AnscAsn1OIDEqualsTo;
    pMyObject->DecodingData         = AnscAsn1OIDDecodingData;
    pMyObject->EncodingData         = AnscAsn1OIDEncodingData;
    pMyObject->GetSizeOfEncoded     = AnscAsn1OIDGetSizeOfEncoded;
    pMyObject->DumpObject           = AnscAsn1OIDDumpObject;
    pMyObject->ReadyToEncode        = AnscAsn1OIDReadyToEncode;

    pMyObject->GetOIDBuffer         = AnscAsn1OIDGetOIDBuffer;
    pMyObject->GetOIDValue          = AnscAsn1OIDGetOIDValue;
    pMyObject->SetOIDValue          = AnscAsn1OIDSetOIDValue;
    pMyObject->SetStringOIDValue    = AnscAsn1OIDSetStringOIDValue; 
    pMyObject->GetStringOIDValue    = AnscAsn1OIDGetStringOIDValue;
    pMyObject->EqualToOIDString     = AnscAsn1OIDEqualToOIDString;

    pMyObject->uLength              = 0;
    pMyObject->pAdvOID              = NULL;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1OIDClone
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function clones a same ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The cloned container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1OIDClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_OIDEN                  pMyObject    = (PANSC_ASN1_OIDEN)hThisObject;
    PANSC_ASN1_OIDEN                  pNewObject   = NULL;

    pNewObject = (PANSC_ASN1_OIDEN)
                    pMyObject->Create
                        (
                            pMyObject->hContainerContext
                        );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1OIDClone()\n");

        return NULL;
    }

    /*
     *  Copy all the attributes;
     */
    /* AttrListCopyAllAttributes( &pNewObject->sAttrList, &pMyObject->sAttrList);*/
    pNewObject->SetName(pNewObject, pMyObject->Name);

    if(!pMyObject->CopyTo(pMyObject, pNewObject))
    {
        pNewObject->AsnFree(pNewObject);

        return NULL;
    }

    return pNewObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        BOOLEAN
        AnscAsn1OIDCopyTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDestObject
            );

    description:

        This function copies the value to the other one.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDestObject
                The dest object.

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1OIDCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    PANSC_ASN1_OIDEN                pMyObject    = (PANSC_ASN1_OIDEN)hThisObject;
    PANSC_ASN1_OIDEN                pNewObject   = (PANSC_ASN1_OIDEN)hDestObject;

    if( pNewObject == NULL || pMyObject == NULL)
    {
        return FALSE;
    }

    /*
     * Copy the optional state;
     */
    pNewObject->bOptional       = pMyObject->bOptional;
    pNewObject->bCanBeOptional  = pMyObject->bCanBeOptional;

    /*
     *  Copy the values;
     */
    pNewObject->uLength             = pMyObject->uLength;

    AnscCopyMemory
        (
            pNewObject->pOIDChain,
            pMyObject->pOIDChain,
            sizeof(ULONG) * MAXIMUM_OID_DEPTH
        );

    if( pMyObject->pAdvOID != NULL)
    {
        pNewObject->pAdvOID = (PULONG)AnscAllocateMemory(sizeof(ULONG) * pNewObject->uLength);

        if( pNewObject->pAdvOID == NULL)
        {
            AnscTrace("Failed to allocate memory in AnscAsn1OIDCopyTo().\n");
            return FALSE;
        }
        /*CID: 71198 Dereference after null check*/
        AnscCopyMemory
            (
                pNewObject->pAdvOID,
                pMyObject->pAdvOID,
                sizeof(ULONG) * pNewObject->uLength
            );
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1OIDEqualsTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOtherObject,
                BOOLEAN                     bValueOnly
            );

    description:

        This function compares the value of the 2 ASN.1 objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOtherObject
                This handle is the other object will be compared to;

                BOOLEAN                     bValueOnly
                Check the value only or not;

    return:     If they have the same type and value, return TRUE;
                otherwise return FALSE.

**********************************************************************/
BOOLEAN
AnscAsn1OIDEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    PANSC_ASN1_OIDEN                pMyObject  = (PANSC_ASN1_OIDEN)hThisObject;
    PANSC_ASN1_OIDEN                pNewObject = (PANSC_ASN1_OIDEN)hOtherObject;

    /* check the pointer first */
    if( hThisObject == hOtherObject)
    {
        return TRUE;
    }

    if( hThisObject == NULL || hOtherObject == NULL)
    {
        AnscTrace("One of the parameter in AnscAsn1OIDEqualsTo() is NULL pointer.\n");

        return FALSE;
    }

    /* check the type */
    if( pMyObject->uType != pNewObject->uType)
    {
        AnscTrace("Different type in AnscAsn1OIDEqualsTo().\n");

        return FALSE;
    }

    /* check the optional state */
    if( pMyObject->bOptional != pNewObject->bOptional)
    {
        return FALSE;
    }

    /* check the attribute list */
    if( !bValueOnly)
    {
        if(!AttrListCompareAttributeList(&pMyObject->sAttrList, &pNewObject->sAttrList))
        {
            return FALSE;
        }
    }

    /* check the value */
    if(!pMyObject->bOptional)
    {
        if( pMyObject->uLength  != pNewObject->uLength)
        {
            return FALSE;
        }

        if( pMyObject->pAdvOID != NULL && pNewObject->pAdvOID != NULL)
        {
            return
                AnscEqualMemory
                    (
                        (PVOID)pMyObject->pAdvOID,
                        (PVOID)pNewObject->pAdvOID,
                        sizeof(ULONG) * pMyObject->uLength
                    );
        }
        else if(pMyObject->pAdvOID != NULL || pNewObject->pAdvOID != NULL)
        {
            return FALSE;
        }
        else
        {
            return
                AnscEqualMemory
                    (
                        (PVOID)pMyObject->pOIDChain,
                        (PVOID)pNewObject->pOIDChain,
                        sizeof(ULONG) * pMyObject->uLength
                    );
        }
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1OIDGetSizeOfEncoded
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function returns the encoding size of the 
        ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The encoding size of the ASN.1 object.
                If the object is not ready-to-encode, -1 will be 
                returned.

**********************************************************************/
static ULONG 
GetSizeOfEncodedASN1SubOID
    (
        ULONG                       ulResult
    )
{
    /***********************************************************
                    DEFINITION OF LOCAL VARIABLES
    ***********************************************************/

    /*
     * temporary counters
     */
    ULONG                           counter = 0;


    /***********************************************************
           EXTRACT THE CONTENT OF ASN.1 OBJECT IDENTIFIER
    ***********************************************************/

    /*
     * each subidentifier is represented as a series of (one or more) octets. Bits 7-1 of the octets of the series
     * collectively encode the subidentifier.
     */
    if( ulResult == 0)
    {
        return 1;
    }
            
    while ( ulResult )
    {
        ulResult                    = ulResult / 0x80;
        counter                     ++;
    }

    return  counter;
}

LONG
AnscAsn1OIDGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_OIDEN                pMyObject    = (PANSC_ASN1_OIDEN)hThisObject;
    ULONG                           ulSize       = 0;
    ULONG                           i            = 0;
    PULONG                          pOIDArray;

    /*
     * If it's optional, don't need encode
     */
    if( pMyObject->bOptional)
    {
        return 0;
    }

    if( !pMyObject->ReadyToEncode(pMyObject))
    {
        AnscTrace("The value of the OID object is not ready to encode.\n");

        return -1;
    }

    /***********************************************************
         CALCULATE THE NUMBER OF OCTETS IN THE ENCODED DATA
    ***********************************************************/
    pOIDArray = pMyObject->GetOIDBuffer(pMyObject);

    if( pMyObject->uLength > 0)
    {
        ulSize += 1;  /* the first 2 will be encoded in one byte */

        for ( i = 2; i < pMyObject->uLength; i ++ )
        {
            ulSize += 
                GetSizeOfEncodedASN1SubOID
                (
                    pOIDArray[i]
                );
        }
    }

    /*
     *  check the attribute list, from the end of the list;
     *  the tag length value is 1.
     */ 
    return AttrListGetSizeOfEncoded(&pMyObject->sAttrList, ulSize, 1);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1OIDDecodingData
            (
                ANSC_HANDLE                 hThisObject,
                PVOID*                      ppEncoding
            );

    description:

        This function is decoding data for ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID*                      ppEncoding
                The pointer of the input binary data

    return:     status of decoding operation

**********************************************************************/
static ULONG 
GetULONGFromOID
    (
        PVOID                       pContentEncoding,
        ULONG                       ulContentSize
    )
{
    /***********************************************************
                    DEFINITION OF LOCAL VARIABLES
    ***********************************************************/

    /*
     * pointer to the start of unexamined raw encoding data
     */
    PUCHAR                          pEncodingUnit   = (PUCHAR)pContentEncoding;
    ULONG                           ulOctetValue;

    /*
     * temporary counters
     */
    ULONG                          i                = 0;
    ULONG                          j                = 0;
    ULONG                          tmp              = 0;


    /***********************************************************
           EXTRACT THE CONTENT OF ASN.1 OBJECT IDENTIFIER
    ***********************************************************/

    /*
     * each subidentifier is represented as a series of (one or more) octets. Bits 7-1 of the octets of the series
     * collectively encode the subidentifier.
     */
    if ( ulContentSize > sizeof(ULONG) + 1 )
    {
        /*
         * I don't believe it, European is stupid ...
         */
        return 0;
    }

    if( ulContentSize == sizeof(ULONG) + 1 )
    {
        if( pEncodingUnit[0] > 0x8F)
        {
            return 0; /* impossible */
        }
    }

    for ( i = 0; i < ulContentSize; i++ )
    {
        ulOctetValue = pEncodingUnit[i] & ~0x80;

        for ( j = 0; j < ulContentSize - i - 1; j++ )
        {
            ulOctetValue            *= 0x80;
        }

        tmp                         += ulOctetValue;
    }

    return tmp;
}

ANSC_STATUS
AnscAsn1OIDDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_OIDEN                pMyObject    = (PANSC_ASN1_OIDEN)hThisObject;
    PANSC_TAGINFO_OBJECT            pTagInfo     = NULL;
    BOOLEAN                         bTagIncluded = TRUE;
    ULONG                           uLeftSize,uInfiniteTags;
    ULONG                           counter,ulOctetsLeft,ulNumberOfOctetsInSubOID;
    PULONG                          pOIDArray;


    PUCHAR                          pTmpBuffer;


    /*
     * pointer to the start of unexamined raw encoding data
     */
    PUCHAR                          pEncodingUnit;

    /* check the parameters */
    if( hThisObject == NULL  || ppEncoding == NULL)
    {
        return ANSC_ASN1_BAD_PARAMETER;
    }

    /*
     *  check before decoding;
     */
    if( pMyObject->BeforeDecoding != NULL)
    {
        returnStatus = pMyObject->BeforeDecoding(pMyObject, ppEncoding);

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }
    }

    /* parsing the data */
    pEncodingUnit   = (PUCHAR)*ppEncoding;

    /*
     * Decode the attrlist first;
     */
    returnStatus = 
        AttrListDecodingData
            (
                &pMyObject->sAttrList,
                (PVOID*)&pEncodingUnit,
                pMyObject->IsConstructive(pMyObject),
                &bTagIncluded,
                &uLeftSize,
                &uInfiniteTags
            );
    
    if( ANSC_STATUS_SUCCESS != returnStatus)
    {
        return returnStatus;
    }

    if( bTagIncluded)
    {
        pTagInfo        = (PANSC_TAGINFO_OBJECT)AnscTagInfoCreate();

        if( pTagInfo == NULL)
        {
            return ANSC_STATUS_RESOURCES;
        }

        /* parsing the data */
        pEncodingUnit   = (PUCHAR)*ppEncoding;
        returnStatus    = pTagInfo->ParsingData((ANSC_HANDLE)pTagInfo, pEncodingUnit);
        
        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return returnStatus;
        }

        /* check the tag matched or not */
        if( pTagInfo->FirstOctetOfTag
                != pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject))
        {
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNEXPECTED_TAG;
        }

        if( pTagInfo->GetSizeOfTag(pTagInfo)     != 1) 
        {           
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);
            AnscTrace("The length of OID value is unacceptable.\n");

            return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
        }

         /* check the tag */
        if( pTagInfo->bIsInfinite)
        {
            AnscTrace("Infinite form is unacceptable for BOOL object.\n");
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_INVALID_ENCODE_LENGTH;
        }

        pEncodingUnit   += 1 + pTagInfo->GetSizeOfLength(pTagInfo);
        uLeftSize       =  pTagInfo->ulContentLength;

        pTagInfo->Remove((ANSC_HANDLE)pTagInfo);
    }

    if( uInfiniteTags > 0)
    {
        AnscTrace("Infinite form is unacceptable for OID object.\n");

        return ANSC_ASN1_INVALID_ENCODE_LENGTH;
    }

    /**************************************************
     *
     *  Decode the value;
     *
     **************************************************/
    if( uLeftSize == 0)
    {
        pMyObject->uLength          = 0;
        pMyObject->bOptional        = FALSE;

        returnStatus = ANSC_STATUS_SUCCESS;
    }
    else
    {
        /* 
         * zhubin updated here on 09/16/2004 
         *
         * To protect from ASN.1 attack
         */
        if( uLeftSize > 4000)
        {
            AnscTrace("The length of oid value is unreasonable '%lu'\n", uLeftSize);

            pMyObject->uLength = 0;

            return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;            
        }

        /*
         *  BER encoding. Primitive. Contents octets are as follows,
         *  where value1, ..., valuen denote the integer values of the
         *  components in the complete object identifier:
         *
         *       1.   The first octet has value 40 * value1 + value2.
         *            (This is unambiguous, since value1 is limited to
         *            values 0, 1, and 2; value2 is limited to the range
         *            0 to 39 when value1 is 0 or 1; and, according to
         *            X.208, n is always at least 2.)
         *
         *       2.   The following octets, if any, encode value3, ...,
         *            valuen. Each value is encoded base 128, most
         *            significant digit first, with as few digits as
         *            possible, and the most significant bit of each
         *            octet except the last in the value's encoding set
         *            to "1."
         * 
         *  Example: The first octet of the BER encoding of RSA Data
         *  Security, Inc.'s object identifier is 40 * 1 + 2 = 42 =
         *  0x2A. The encoding of 840 = 0x06 * 0x80 + 0x48 is 86 48 and the
         *  encoding of 113549 = 0x06 * 0x80 * 0x80 + 0x77 * 0x80 + 0x0D is 
         *  86 F7 0D. 
         *
         *  (1.2.840.113549) will be : "06 06 2A 86 48 86 F7 0D"
         */
        ulOctetsLeft                 = uLeftSize;
        counter                      = 0;

        while ( ulOctetsLeft )
        {
            ulNumberOfOctetsInSubOID = 1;

            while ( pEncodingUnit[ulNumberOfOctetsInSubOID - 1] & 0x80 )
            {
                ulNumberOfOctetsInSubOID ++;
            }

            if ( ulNumberOfOctetsInSubOID > sizeof(ULONG) + 1)
            {
                /*
                 * I don't think we will ever have OID that big
                 */
                AnscTrace("The size of the ULONG value in OID is %lu, unacceptable.\n", ulNumberOfOctetsInSubOID);
                return  ANSC_ASN1_INVALID_VALUE;
            }

            if ( ulNumberOfOctetsInSubOID == sizeof(ULONG) + 1)
            {
                if( pEncodingUnit[0] > 0x8F)
                {
                    AnscTrace("The ulong value in OID is too big.\n");
                    return  ANSC_ASN1_INVALID_VALUE;
                }
            }

            /* check do we need to create oid or not */
            if( counter != 0 && counter % MAXIMUM_OID_DEPTH == 0)
            {
                /* create the  dynamic oid array */
                if( pMyObject->pAdvOID == NULL)
                {
                    pMyObject->pAdvOID = (PULONG)
                        AnscAllocateMemory(sizeof(ULONG) * (counter + MAXIMUM_OID_DEPTH));

                    AnscCopyMemory
                        (
                            pMyObject->pAdvOID,
                            pMyObject->pOIDChain,
                            sizeof(ULONG) * MAXIMUM_OID_DEPTH
                        );
                }
                else /* reallocate the memory and add MAXIMUM_OID_DEPTH more */
                {
                  /* since reallocate memory is not supported in some platforms such as
                   * LINUX, we have to allocate new memory and copy it over */

                  pTmpBuffer = (UCHAR*)pMyObject->pAdvOID;

                  pMyObject->pAdvOID = 
                    AnscAllocateMemory
                    (
                        sizeof(ULONG) * (counter + MAXIMUM_OID_DEPTH)
                    );

                  AnscCopyMemory
                    (
                        pMyObject->pAdvOID,
                        pTmpBuffer,
                        counter * sizeof(ULONG)
                    );

                  AnscFreeMemory(pTmpBuffer);

                }
            }

            /* get the OID buffer */
            pOIDArray = pMyObject->GetOIDBuffer(pMyObject);

            pOIDArray[counter] = 
                GetULONGFromOID
                    (
                        pEncodingUnit, 
                        ulNumberOfOctetsInSubOID
                    ); 

            if ( counter == 0 )
            {
                /*
                 * the first subOID was calculated like this: (X * 40) + Y, here X is the first subOID and Y is the second
                 * subOID
                 */
                if ( pOIDArray[0] >= 80 )
                {
                    if( pOIDArray[0] >= 120)
                    {
                        AnscTrace("Invalid OID value, first byte is 0x%lX.\n", pOIDArray[0]);

                        return ANSC_ASN1_BAD_OID_VALUE;
                    }

                    /*
                     * in this case, X = 2 and Y = Value - 80;
                     */
                    pOIDArray[1] = pOIDArray[0] - 80;
                    pOIDArray[0] = 2;
                }
                else
                {
                    /*
                     * in this case, X = Value / 40 and Y = Value % 40;
                     */
                    pOIDArray[1] = pOIDArray[0] % 40;
                    pOIDArray[0] = pOIDArray[0] / 40;
                }

                counter   ++;
            }

            counter             ++;
            pEncodingUnit       += ulNumberOfOctetsInSubOID;

            if ( ulOctetsLeft < ulNumberOfOctetsInSubOID )
            {
                return  ANSC_ASN1_INVALID_ENCODE_LENGTH;
            }

            ulOctetsLeft        -= ulNumberOfOctetsInSubOID;
        }

        pMyObject->uLength      = counter; 

    }

    if( ANSC_STATUS_SUCCESS == returnStatus)
    {
        /*
         *  check after decoding;
         */
        if( pMyObject->AfterDecoding != NULL)
        {
            returnStatus = pMyObject->AfterDecoding(pMyObject, (PVOID*)&pEncodingUnit);

            if( ANSC_STATUS_SUCCESS != returnStatus)
            {
                return returnStatus;
            }
        }

        *ppEncoding             = (PVOID)pEncodingUnit;
        pMyObject->bOptional    = FALSE;
    }

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1OIDEncodingData
            (
                ANSC_HANDLE                 hThisObject,
                PVOID*                      ppEncoding
            );

    description:

        This function is decoding data for ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID*                      ppEncoding
                The pointer of the output binary data

    return:     status of encoding operation

**********************************************************************/
static ANSC_STATUS
EncodeASN1SubOID
    (
        ULONG                       ulResult,
        PVOID*                      ppEncodedData
    )

{
    /***********************************************************
                    DEFINITION OF LOCAL VARIABLES
    ***********************************************************/

    /*
     * temporary counters
     */
    ULONG                           counter         = 0;
    int                             i               = 0;
    ULONG                           backup          = ulResult;

    /*
     * shortcut pointer to a char array
     */
    PUCHAR                          pCharData       = (PUCHAR)*ppEncodedData;


    /***********************************************************
           EXTRACT THE CONTENT OF ASN.1 OBJECT IDENTIFIER
    ***********************************************************/

    /*
     * each subidentifier is represented as a series of (one or more) octets. Bits 7-1 of the octets of the series
     * collectively encode the subidentifier.
     */
    if( ulResult == 0)
    {
        counter = 1;
    }

    while ( ulResult )
    {
        ulResult                    = ulResult / 0x80;
        counter++;
    }

    ulResult                        = backup;

    for ( i = counter - 1; i >= 0; i-- )
    {
        pCharData[i]                = (UCHAR)(ulResult % 0x80);
        ulResult                    = ulResult / 0x80;

        if ( i != (int)(counter - 1) )
        {
            pCharData[i]            |= 0x80;
        }
    }

    pCharData                       += counter;
    *ppEncodedData                  = (PVOID)pCharData;

    return  ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
AnscAsn1OIDEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_OIDEN                pMyObject       = (PANSC_ASN1_OIDEN)hThisObject;
    int                             i;
    BOOLEAN                         bTagIncluded    = TRUE;
    LONG                            uSizeOfEncoded;
    ULONG                           uLeftSize;
    PULONG                          pOIDArray;
    int                             iEncodedLength = 0;

    /*
     * shortcut pointer to a char array
     */
    PUCHAR                          pCharData       = (PUCHAR)*ppEncoding;
    PUCHAR                          pBackData       = pCharData;

    /*
     * If it's optional, don't need encode
     */
    if( pMyObject->bOptional)
    {
        return ANSC_STATUS_SUCCESS;
    }

    /*
     *  check before encoding;
     */
    if( pMyObject->BeforeEncoding != NULL)
    {
        returnStatus = pMyObject->BeforeEncoding(pMyObject, ppEncoding);

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }
    }

    uSizeOfEncoded = pMyObject->GetSizeOfEncoded(pMyObject);

    if( uSizeOfEncoded < 0)
    {
        return ANSC_ASN1_NOT_READY_TO_ENCODE;
    }

    /*
     * Encode all attrlist first; 
     */
    returnStatus = 
        AttrListEncodingData
            (
                &pMyObject->sAttrList,
                (PVOID*)&pCharData,
                (ULONG)uSizeOfEncoded,
                pMyObject->IsConstructive(pMyObject),
                &bTagIncluded,
                &uLeftSize
            );

    if( ANSC_STATUS_SUCCESS != returnStatus)
    {
        AnscTrace("Failed to encode the attrlist of BOOL objects.\n");

        return returnStatus;
    }

    /*
     *  Encode the tag first;
     */
    if ( bTagIncluded)
    {
        *pCharData                  = pMyObject->GetFirstOctet(pMyObject);
        pCharData++;

          /*CID: 75029 Improper use of negative value*/
	iEncodedLength = GetPureEncodedLength( uLeftSize - 1);
	if ( iEncodedLength < 0 )
	{
		return ANSC_ASN1_NOT_READY_TO_ENCODE;
	}
        returnStatus = 
            EncodeLength
                (
                    (PVOID*)&pCharData, 
                    (ULONG)iEncodedLength
                );

        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            return returnStatus;
        }
    }

    /*
     *  Encode the value;
     */
    pOIDArray   = pMyObject->GetOIDBuffer(pMyObject);

    if( pMyObject->uLength > 0)
    {
        if( pMyObject->uLength == 1)
        {
            EncodeASN1SubOID
                (
                    pOIDArray[0] * 40,
                    (PVOID*)&pCharData
                );
        }
        else
        {
            EncodeASN1SubOID
                (
                    pOIDArray[0] * 40 + pOIDArray[1],
                    (PVOID*)&pCharData
                );
        }

        for ( i = 2; i < (int)pMyObject->uLength; i++ )
        {
            EncodeASN1SubOID
                (
                   pOIDArray[i], 
                   (PVOID*)&pCharData
                );
        }
    }

    /*
     *  Set the buffer before exit;
     */
    if( pCharData - pBackData != uSizeOfEncoded)
    {
        AnscTrace
            (
                "Warning in Encoding OID, the size of encoding is expected to be %lu, but it's %ld.\n",
                uSizeOfEncoded,
                (long int)(pCharData - pBackData)
            );
    }

    /*
     *  check after encoding;
     */
    if( pMyObject->AfterEncoding != NULL)
    {
        returnStatus = pMyObject->AfterEncoding(pMyObject, (PVOID*)&pCharData);

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }
    }

    *ppEncoding                     = (PVOID)pCharData;

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1OIDDumpObject
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pBuffer,
                PULONG                      pLength,
                BOOLEAN                     bIsRoot,
                BOOLEAN                     bShowValue
            );

    description:

        This function dump out the values in a tree structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pBuffer,
                The output buffer;

                PULONG                      pLength,
                The pointer of buffer size;

                BOOLEAN                     bIsRoot,
                The object is the root or not;

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

    return:     status of operation.

**********************************************************************/
BOOLEAN
AnscAsn1OIDDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    )
{
    PANSC_ASN1_OIDEN                pMyObject       = (PANSC_ASN1_OIDEN)hThisObject;
    CHAR                            pAttrBuf[128]   = { 0 };
    CHAR                            pOIDValue[128]  = { 0 };
    ULONG                           attrLength      = 128;
    PCHAR                           pName;
    errno_t                         rc              = -1;

    if( pBuffer == NULL || pLength == NULL)
    {
        return FALSE;
    }

    AttrListDump( &pMyObject->sAttrList, pAttrBuf, &attrLength);

    if( bIsRoot)
    {
        pName   = pMyObject->ClassName;
    }
    else
    {
        pName   = pMyObject->Name;
    }

    if( pName == NULL)
    {
        return FALSE;
    }

    if( !bShowValue)
    {
        rc =
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s",
                    pName,
                    pAttrBuf,
                    ASN1Type2String(pMyObject->uType)
                );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        *pLength = rc;
    }
    else if( pMyObject->bOptional)
    {
        rc =
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s (Optional)",
                    pName,
                    pAttrBuf,
                    ASN1Type2String(pMyObject->uType)
                );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        *pLength = rc;
    }
    else 
    {
        pMyObject->GetStringOIDValue(pMyObject, pOIDValue);

        rc =
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s (%s)",
                    pName,
                    pAttrBuf,
                    ASN1Type2String(pMyObject->uType),
                    pOIDValue
                 );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        *pLength = rc;
    }

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1OIDTraceObject
            (
                ANSC_HANDLE                 hThisObject,
                LONG                        layer,
                BOOLEAN                     bShowValue,
                BOOLEAN                     bRecursive
            );

    description:

        This function dump out the values in a tree structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                LONG                        layer,
                Specifies the layer of current object.

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

                BOOLEAN                     bRecursive
                Trace recursive or not;

    return:     status of operation.

**********************************************************************/
BOOLEAN
AnscAsn1OIDTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    )
{

    UNREFERENCED_PARAMETER(bRecursive);
    PANSC_ASN1_OIDEN                pMyObject    = (PANSC_ASN1_OIDEN)hThisObject;
    CHAR                            pAttrBuf[512]= { 0 };
    ULONG                           attrLength   = 512;
    PCHAR                           pName;

    TraceTabs(layer);
    AttrListDump( &pMyObject->sAttrList, pAttrBuf, &attrLength);

    if( layer  == 0)
    {
        pName   = pMyObject->ClassName;
    }
    else
    {
        pName   = pMyObject->Name;
    }

    if( pName == NULL)
    {
        return FALSE;
    }

    if( !bShowValue)
    {
        AnscTrace
            (
                "%s ::=%s %s\n",
                pName,
                pAttrBuf,
                ASN1Type2String(pMyObject->uType)
            );
    }
    else if( pMyObject->bOptional)
    {
        AnscTrace
            (
                "%s ::=%s %s (Optional)\n",
                pName,
                pAttrBuf,
                ASN1Type2String(pMyObject->uType)
            );
    }
    else
    {
        AnscTrace
            (
                "%s ::=%s %s (",
                pName,
                pAttrBuf,
                ASN1Type2String(pMyObject->uType)
             );
        TraceOIDValue
            (
                pMyObject->GetOIDBuffer(pMyObject),
                pMyObject->uLength
            );
        AnscTrace(")\n");
    }

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1OIDReadyToEncode
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function checked the object is ready to encode or not

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     ready or not.

**********************************************************************/
BOOLEAN
AnscAsn1OIDReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return TRUE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1OIDGetOIDValue
            (
                ANSC_HANDLE                 hThisObject,
                PULONG                      pOIDArray,
                PULONG                      pSize
            );

    description:

        This function returns the oid value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PULONG                      pOIDArray,
                The output array of oid value;

                PULONG                      pSize
                The buffer of size of array;

    return:     the status of the operation.

**********************************************************************/
ANSC_STATUS
AnscAsn1OIDGetOIDValue
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pOIDArray,
        PULONG                      pSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_OIDEN                pMyObject    = (PANSC_ASN1_OIDEN)hThisObject;

    if( pSize == NULL)
    {
        return ANSC_ASN1_NULL_PARAMETER;
    }

    if( pOIDArray == NULL)
    {
        *pSize                          = pMyObject->uLength;

        return ANSC_STATUS_SUCCESS;
    }

    if( *pSize < pMyObject->uLength)
    {
        *pSize                          = pMyObject->uLength;

        AnscTrace
            (
                "Required size of OID array is '%lu' -- '%lu' is not enough.\n", 
                pMyObject->uLength, 
                *pSize
            );

        return ANSC_ASN1_NO_ENOUGH_SIZE;
    }

    AnscCopyMemory
        (
            pOIDArray,
            pMyObject->GetOIDBuffer(pMyObject),
            sizeof(ULONG) * pMyObject->uLength
        );

    *pSize                          = pMyObject->uLength;

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1OIDGetOIDStringValue
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pBuffer
            );

    description:

        This function returns the oid value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pBuffer
                The output buffer;

    return:     succeded or not

**********************************************************************/
BOOLEAN
AnscAsn1OIDGetOIDStringValue
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer
    )
{
    PANSC_ASN1_OIDEN                pMyObject    = (PANSC_ASN1_OIDEN)hThisObject;

    return ExportOIDValueToString
             (
                pMyObject->GetOIDBuffer(pMyObject),
                pMyObject->uLength,
                pBuffer
             );
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1OIDSetOIDValue
            (
                ANSC_HANDLE                 hThisObject,
                PULONG                      pOIDArray,
                ULONG                       uSize
            );

    description:

        This function set the oid value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PULONG                      pOIDArray
                The input OID value array;

                ULONG                       uSize
                The size of the value array;

    return:     the status of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1OIDSetOIDValue
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pOIDArray,
        ULONG                       uSize
    )
{
    PANSC_ASN1_OIDEN                pMyObject    = (PANSC_ASN1_OIDEN)hThisObject;
    
    if( pOIDArray == NULL || uSize == 0)
    {
        pMyObject->uLength          = 0;

        return ANSC_STATUS_SUCCESS;
    }

    if(pOIDArray[0] > 2)
    {
        AnscTrace("The first oid value is unacceptable - '%lu'\n", pOIDArray[0]);

        return ANSC_ASN1_INVALID_VALUE;
    }

    if( pMyObject->pAdvOID != NULL)
    {
        AnscFreeMemory(pMyObject->pAdvOID);
    }

    pMyObject->uLength              = uSize;

    if( uSize > MAXIMUM_OID_DEPTH)
    {
        pMyObject->pAdvOID = (PULONG)
            AnscAllocateMemory(sizeof(ULONG) * uSize);
    }
    
    AnscCopyMemory
        (
            pMyObject->GetOIDBuffer(pMyObject),
            pOIDArray,
            sizeof(ULONG) * uSize
        );

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1OIDSetStringOIDValue
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pOIDString
            );

    description:

        This function set the oid value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pOIDString
                The input OID string value;
                Sample values: "1.343.2.43", "1 343 2 43"

    return:     the status of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1OIDSetStringOIDValue
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    )
{
    PANSC_ASN1_OIDEN                pMyObject    = (PANSC_ASN1_OIDEN)hThisObject;
    ULONG                           uSize        = MAXIMUM_OID_DEPTH;
    PULONG                          pOIDArray;

    if( !ParsingOIDStringValue
            (
                pOIDString,
                NULL,
                &uSize
            )
      )
    {
        AnscTrace("Invalid OID string value - %s\n", pOIDString);

        return ANSC_ASN1_INVALID_VALUE;
    }

    if( uSize < 1 )
    {
        AnscTrace("The size of oid value array is unacceptable - '%lu'\n", uSize);

        return ANSC_ASN1_INVALID_VALUE;
    }

    if(uSize > MAXIMUM_OID_DEPTH)
    {
        pMyObject->pAdvOID = (PULONG)
            AnscAllocateMemory(sizeof(ULONG) * uSize);

        pOIDArray = pMyObject->pAdvOID;
    }
    else
    {
        pOIDArray = pMyObject->pOIDChain;
    }

    ParsingOIDStringValue
        (
            pOIDString,
            pOIDArray,
            &uSize
        );

    if(pOIDArray[0] > 2)
    {
        AnscTrace("The first oid value is unacceptable - '%lu'\n", pOIDArray[0]);

        return ANSC_ASN1_INVALID_VALUE;
    }

    pMyObject->uLength              = uSize;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1OIDGetStringOIDValue
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pStringValue
            );

    description:

        This function get the string value of oid

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pStringValue
                The ooutput buffer of string value

    return:     the status of the operation

**********************************************************************/
BOOLEAN
AnscAsn1OIDGetStringOIDValue
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pStringValue
    )
{
    PANSC_ASN1_OIDEN                pMyObject         = (PANSC_ASN1_OIDEN)hThisObject;
    PULONG                          pOIDArray;

    if( hThisObject == NULL || pStringValue == NULL)
    {
        return FALSE;
    }

    pOIDArray = pMyObject->GetOIDBuffer(pMyObject);

    return 
        ExportOIDValueToString
            ( 
                pOIDArray,
                pMyObject->uLength,
                pStringValue
            );        
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1OIDEqualToOIDString
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pOIDString
            );

    description:

        This function check the value of this OID object equals to the 
        given OID string or not

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pOIDString
                The given OID string value

    return:     yes or not

**********************************************************************/
BOOLEAN
AnscAsn1OIDEqualToOIDString
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    )
{
    PANSC_ASN1_OIDEN                pMyObject         = (PANSC_ASN1_OIDEN)hThisObject;
    CHAR                            pValue[128]       = { 0 };
    
    if( hThisObject == NULL || pOIDString == NULL)
    {
        return FALSE;
    }

    if( !pMyObject->GetStringOIDValue(pMyObject, pOIDString))
    {
        return FALSE;
    }

    return (strcasecmp( pOIDString, pValue) == 0 );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PULONG
        AnscAsn1OIDGetOIDBuffer
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        Returns the pointer of current OID array. 

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The buffer;

**********************************************************************/
PULONG
AnscAsn1OIDGetOIDBuffer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_OIDEN                pMyObject         = (PANSC_ASN1_OIDEN)hThisObject;

    if( pMyObject->pAdvOID != NULL)
    {
        return pMyObject->pAdvOID;
    }

    return pMyObject->pOIDChain;
}
