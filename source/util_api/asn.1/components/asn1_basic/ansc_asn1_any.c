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

    module: ANSC_ASN1_ANY.c

        For ASN.1 ANY object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by the ASN.1 base class  "ASN1_ANY"

        *   AnscAsn1AnyCreate
        *   AnscAsn1AnyFree
        *   AnscAsn1AnyInitialize
        *   AnscAsn1AnyClone
        *   AnscAsn1AnyCopyTo
        *   AnscAsn1AnyEqualsTo
        *   AnscAsn1AnyGetSizeOfEncoded
        *   AnscAsn1AnyDecodingData
        *   AnscAsn1AnyEncodingData
        *   AnscAsn1AnyDumpObject
        *   AnscAsn1AnyTraceObject

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/26/02    initial revision.

**********************************************************************/

#include "ansc_asn1_local.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1AnyCreate
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
AnscAsn1AnyCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_ANY                  pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_ANY)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_ANY)AnscAllocateMemory(sizeof(ANSC_ASN1_ANY));
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
    pMyObject->Oid                 = ANSC_ASN1_ANY_ID;
    pMyObject->uType               = ASN1_ANY_TYPE;
    pMyObject->Create              = AnscAsn1AnyCreate;
    pMyObject->AsnFree             = AnscAsn1AnyFree;
    
    AnscAsn1AnyInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_ANY_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_ANY_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1AnyFree
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
AnscAsn1AnyFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_ANY                  pMyObject     = (PANSC_ASN1_ANY)hThisObject;


    if( pMyObject != NULL)
    {
        /*
         *  Free the binary value buffer;
         */
        if( pMyObject->pStringBuffer != NULL)
        {
            AnscFreeMemory(pMyObject->pStringBuffer);
        }

        /*
         *  Free the tag info object.
         */
        if( pMyObject->pTagInfo != NULL)
        {
            pMyObject->pTagInfo->Remove(pMyObject->pTagInfo);
        }
    }

    AnscAsn1Free(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1AnyInitialize
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
AnscAsn1AnyInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_ANY                  pMyObject    = (PANSC_ASN1_ANY)hThisObject;

    /*
     *  Call base initialization first
     */
    AnscAsn1Initialize((ANSC_HANDLE)pMyObject);

    /* set other functions */
    pMyObject->Clone                = AnscAsn1AnyClone;
    pMyObject->CopyTo               = AnscAsn1AnyCopyTo;
    pMyObject->EqualsTo             = AnscAsn1AnyEqualsTo;
    pMyObject->DecodingData         = AnscAsn1AnyDecodingData;
    pMyObject->EncodingData         = AnscAsn1AnyEncodingData;
    pMyObject->GetSizeOfEncoded     = AnscAsn1AnyGetSizeOfEncoded;
    pMyObject->DumpObject           = AnscAsn1AnyDumpObject;

    pMyObject->pStringBuffer        = NULL;
    pMyObject->uLength              = 0;
    pMyObject->pTagInfo             = AnscTagInfoCreate();

    pMyObject->bOptional            = TRUE;
    pMyObject->bCanBeOptional       = TRUE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1AnyClone
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
AnscAsn1AnyClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_ANY                  pMyObject    = (PANSC_ASN1_ANY)hThisObject;
    PANSC_ASN1_ANY                  pNewObject   = NULL;

    pNewObject = (PANSC_ASN1_ANY)
                 pMyObject->Create
                    (
                        pMyObject->hContainerContext
                    );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1AnyClone()\n");

        return NULL;
    }

    /*
     *  Copy all the attributes;
     */
    /* AttrListCopyAllAttributes( &pNewObject->sAttrList, &pMyObject->sAttrList);*/
    pNewObject->SetName(pNewObject, pMyObject->Name);

    pMyObject->CopyTo(pMyObject, pNewObject);

    return pNewObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        BOOLEAN
        AnscAsn1AnyCopyTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDestObject
            );

    description:

        This function copies value to the dest object.

    argument:   ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDestObject
                The dest object;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1AnyCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    PANSC_ASN1_ANY                  pMyObject    = (PANSC_ASN1_ANY)hThisObject;
    PANSC_ASN1_ANY                  pNewObject   = (PANSC_ASN1_ANY)hDestObject;

    if( pNewObject == NULL || pMyObject == NULL)
    {
        AnscTrace("Failed to copyto at AnscAsn1AnyCopyTo()\n");

        return FALSE;
    }

    /*
     *  Copy the value;
     */
    pNewObject->uLength             = pMyObject->uLength;

    if( pMyObject->pStringBuffer != NULL)
    {
        pNewObject->pStringBuffer   = AnscAllocateMemory(pNewObject->uLength);

        if( pNewObject->pStringBuffer != NULL)
        {
            AnscCopyMemory
                ( 
                    pNewObject->pStringBuffer, 
                    pMyObject->pStringBuffer,
                    pMyObject->uLength
                );
        }
    }
  
    /*
     *  Copy the tag info;
     */
    AnscCopyMemory
        (
            pNewObject->pTagInfo,
            pMyObject->pTagInfo,
            sizeof(ANSC_TAGINFO_OBJECT)
        );

    /*
     *  Copy the optional state.
     */
    pNewObject->bOptional       = pMyObject->bOptional;
    pNewObject->bCanBeOptional  = pMyObject->bCanBeOptional;

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1AnyEqualsTo
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
                Check value only or not;

    return:     If they have the same type and value, return TRUE;
                otherwise return FALSE.

**********************************************************************/
BOOLEAN
AnscAsn1AnyEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    UNREFERENCED_PARAMETER(bValueOnly);
    PANSC_ASN1_ANY                  pMyObject  = (PANSC_ASN1_ANY)hThisObject;
    PANSC_ASN1_ANY                  pNewObject = (PANSC_ASN1_ANY)hOtherObject;

    /* check the pointer first */
    if( hThisObject == hOtherObject)
    {
        return TRUE;
    }

    if( hThisObject == NULL || hOtherObject == NULL)
    {
        AnscTrace("One of the parameter in AnscAsn1AnyEqualsTo() is NULL pointer.\n");

        return FALSE;
    }

    /* check the type */
    if( pMyObject->uType != pNewObject->uType)
    {
        AnscTrace("Different type in AnscAsn1AnyEqualsTo().\n");

        return FALSE;
    }

    /* check the optional state */
    if( pMyObject->bOptional != pNewObject->bOptional)
    {
        return FALSE;
    }

    if(!pMyObject->bOptional)
    {
        if( pMyObject->uLength != pNewObject->uLength)
        {
            return FALSE;
        }

        if( pMyObject->pStringBuffer != NULL)
        {
            if(!AnscEqualMemory
                    (
                        pMyObject->pStringBuffer,
                        pNewObject->pStringBuffer,
                        pMyObject->uLength
                    )
              )
            {
                return FALSE;
            }
        }

        if( !AnscEqualMemory
                (
                    pMyObject->pTagInfo,
                    pNewObject->pTagInfo,
                    sizeof(ANSC_TAGINFO_OBJECT)
                )   
          )
        {
            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1AnyGetSizeOfEncoded
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
LONG
AnscAsn1AnyGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_ANY                  pMyObject    = (PANSC_ASN1_ANY)hThisObject;

    if( pMyObject->bOptional)
    {
        return 0;
    }

    return pMyObject->uLength;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1AnyDecodingData
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
ANSC_STATUS
AnscAsn1AnyDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_ANY                  pMyObject    = NULL;
    PANSC_TAGINFO_OBJECT            pTagInfo     = NULL;

    /*
     * pointer to the start of unexamined raw encoding data
     */
    PUCHAR                          pEncodingUnit = NULL;

    /* check the parameters */
    if( hThisObject == NULL  || ppEncoding == NULL)
    {
        return ANSC_ASN1_BAD_PARAMETER;
    }

    /*RDKB-6195, CID-24365, assign after null check*/
    pMyObject    = (PANSC_ASN1_ANY)hThisObject;
    pTagInfo     = pMyObject->pTagInfo;

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
    returnStatus    = pTagInfo->ParsingData((ANSC_HANDLE)pTagInfo, pEncodingUnit);
        
    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }

    /* 
     *  Free the old memory;
     */
    if( pMyObject->pStringBuffer != NULL)
    {
        AnscFreeMemory(pMyObject->pStringBuffer);
    }
    
    /*
     * Copy the new value;
     */
    pMyObject->uLength  = 
        pTagInfo->GetSizeOfTag(pTagInfo)    + 
        pTagInfo->GetSizeOfLength(pTagInfo) +
        pTagInfo->ulContentLength;

    pMyObject->pStringBuffer = AnscAllocateMemory( pMyObject->uLength);

    if( pMyObject->pStringBuffer != NULL)
    {
        AnscCopyMemory( pMyObject->pStringBuffer, pEncodingUnit, pMyObject->uLength);
    }
    else
    {
        pMyObject->uLength = 0;
    }

    pEncodingUnit       += pMyObject->uLength;

    /*
     *  check after decoding;
     */
    if( pMyObject->AfterDecoding != NULL)
    {
        returnStatus = pMyObject->AfterDecoding(pMyObject,(PVOID*)&pEncodingUnit);

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }
    }

    /*
     *  Set the new position.
     */
    *ppEncoding          = (PVOID) pEncodingUnit;

    /*
     * Reset the optional state;
     */
    pMyObject->bOptional = FALSE;

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1AnyEncodingData
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
ANSC_STATUS
AnscAsn1AnyEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_ANY                  pMyObject    = (PANSC_ASN1_ANY)hThisObject;

    /*
     * pointer to the start of unexamined raw encoding data
     */
    PUCHAR                          pEncodingUnit;

    /* check the parameters */
    if( hThisObject == NULL  || ppEncoding == NULL)
    {
        return ANSC_ASN1_BAD_PARAMETER;
    }

    if( pMyObject->bOptional)
    {
        return ANSC_STATUS_SUCCESS;
    }

    /*
     *  check before encoding
     */
    if( pMyObject->BeforeEncoding != NULL)
    {
        returnStatus = pMyObject->BeforeEncoding(pMyObject, ppEncoding);

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }
    }

    /* check the buffer */
    if( pMyObject->pStringBuffer == NULL || pMyObject->uLength == 0)
    {
        return ANSC_STATUS_SUCCESS;
    }

    /* parsing the data */
    pEncodingUnit   = (PUCHAR)*ppEncoding;

    AnscCopyMemory( pEncodingUnit, pMyObject->pStringBuffer, pMyObject->uLength);

    pEncodingUnit += pMyObject->uLength;

    /*
     *  check after encoding;
     */
    if( pMyObject->AfterEncoding != NULL)
    {
        returnStatus = pMyObject->AfterEncoding(pMyObject, (PVOID*)&pEncodingUnit);

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }
    }

    *ppEncoding     = (PVOID)( pEncodingUnit);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1AnyDumpObject
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

                BOOLEAN                     bIsRoot
                It's a root object or not;

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

    return:     status of operation.

**********************************************************************/
BOOLEAN
AnscAsn1AnyDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    )
{
    PANSC_ASN1_ANY                  pMyObject    = (PANSC_ASN1_ANY)hThisObject;
    CHAR                            pAttrBuffer[512] = { 0 };
    ULONG                           attrLength       = 512;
    PCHAR                           pName;
    errno_t                         rc               = -1;

    if( pBuffer == NULL || pLength == NULL)
    {
        return FALSE;
    }

    AttrListDump( &pMyObject->sAttrList, pAttrBuffer, &attrLength);

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
                    pAttrBuffer,
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
                    pAttrBuffer,
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
        rc = 
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s (L%d)",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType),
                    (int)pMyObject->uLength
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
        AnscAsn1AnyTraceObject
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
AnscAsn1AnyTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    )
{
    UNREFERENCED_PARAMETER(bRecursive);
    PANSC_ASN1_ANY                  pMyObject        = (PANSC_ASN1_ANY)hThisObject;
    CHAR                            pAttrBuffer[512] = { 0 };
    ULONG                           attrLength       = 512;
    PCHAR                           pName;

    TraceTabs(layer);

    AttrListDump( &pMyObject->sAttrList, pAttrBuffer, &attrLength);

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

    if(!bShowValue)
    {
        AnscTrace
            (
                "%s ::=%s %s\n", 
                pName,
                pAttrBuffer,
                ASN1Type2String(pMyObject->uType)
            );
    }
    else if( pMyObject->bOptional)
    {
        AnscTrace
            (
                "%s ::=%s %s (Optional)\n", 
                pName,
                pAttrBuffer,
                ASN1Type2String(pMyObject->uType)
            );
    }
    else
    {
        AnscTrace
            (
                "%s ::=%s %s (L%lu)\n",
                pName,
                pAttrBuffer,
                ASN1Type2String(pMyObject->uType),
                pMyObject->uLength
            );
    }

    return  TRUE;
}



