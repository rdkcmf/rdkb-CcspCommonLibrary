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

    module: ansc_asn1_attr_interface.c

        For attribute of ASN.1 object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by TagInfo 

        *   AnscAsn1AttrCreate
        *   AnscAsn1AttrParsingData
        *   AnscAsn1AttrFree
        *   AnscAsn1AttrGetSizeOfEncodedTag
        *   AnscAsn1AttrClone
        *   AnscAsn1AttrEqualsTo
        *   AnscAsn1AttrCopyValueFrom
        *   AnscAsn1AttrEncodingData
        *   AnscAsn1AttrDump

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/21/02    initial revision.
        04/01/02    Function "AnscAsn1AttrDump" was added;

**********************************************************************/

#include "ansc_asn1_local.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1AttrCreate
            (
                ASN_OBJECT_FORM_TYPE        Type,
                ULONG                       ulNumber,
                BOOLEAN                     bImplicit
            );

    description:

        This function constructs an attribute with given form form

    argument:   
                ASN_OBJECT_FORM_TYPE        Type,
                The form type

                ULONG                       ulNumber,
                The class number;

                BOOLEAN                     bImplicit
                implicit or not

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1AttrCreate
    (
        ASN_OBJECT_FORM_TYPE        Type,
        ULONG                       ulNumber,
        BOOLEAN                     bImplicit

    )
{
    PANSC_ATTR_OBJECT               pBaseObject  = NULL;
    
    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pBaseObject = (PANSC_ATTR_OBJECT)AnscAllocateMemory(sizeof(ANSC_ATTR_OBJECT));

    if ( !pBaseObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pBaseObject->Remove             = AnscAsn1AttrFree;

    pBaseObject->Clone              = AnscAsn1AttrClone;
    pBaseObject->CopyVlaueFrom      = AnscAsn1AttrCopyValueFrom;
    pBaseObject->EncodingData       = AnscAsn1AttrEncodingData;
    pBaseObject->EqualsTo           = AnscAsn1AttrEqualsTo;
    pBaseObject->GetSizeOfEncodedTag= AnscAsn1AttrGetSizeOfEncodedTag;
    pBaseObject->Dump               = AnscAsn1AttrDump;

    pBaseObject->bImplicit          = bImplicit;
    pBaseObject->FormType           = Type;
    pBaseObject->ulClassNumber      = ulNumber;
    pBaseObject->ulFixedLen         = 0;
    pBaseObject->ulMaxiLen          = 0;
    

    return  (ANSC_HANDLE)pBaseObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1AttrParsingData
            (
                PUCHAR                      pData
            );

    description:

        This function constructs an attribute based on the input binary
        data

    argument:   
                PUCHAR                      pData
                The input binary data;

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1AttrParsingData
    (
        PUCHAR                      pData
    )
{
    ASN_OBJECT_FORM_TYPE            formType     = EMPTY_FORM;
    UCHAR                           uChar;
    ULONG                           ulClass      = 0;
    ULONG                           counter, i, times, uValue;

    if( pData == NULL)
    {
        return NULL;
    }

    /*
     * Find out the form type first;
     */
    uChar = pData[0] & 0xC0;

    if(uChar == 0x40)
    {
        formType = APPLICATION_FORM;
    }
    else if(uChar == 0x80)
    {
        formType = CONTEXT_FORM;
    }
    else if( uChar == 0xC0)
    {
        formType = PRIVATE_FORM;
    }
    else
    {
        formType = UNIVERSAL_FORM;
    }

    /*
     *  Find out the class number;
     */
    uChar = pData[0] & 0x1F;

    if( uChar != 0x1F)
    {
        /* one tag only */
        ulClass  = (ULONG)uChar;
    }
    else
    /* two or more */
    {
       /*
        * check out how many tag 
        */
        counter = 1;
        uChar   = pData[counter];

        while( uChar >= 0x80) /* first bit as 1 */
        {
            counter ++;
            uChar   = pData[counter];
        }

        /* calculate the class number */
        for( i = 0; i < counter; i ++)
        {
            uChar   =  pData[ i + 1];
            uChar   &= 0x7F;
            uValue  =  (ULONG)uChar;
            times   =  counter - i - 1;

            while( times > 0)
            {
                uValue  *= 128;
                times   --;
            }

            ulClass     += uValue;
        }
       
    }

    return AnscAsn1AttrCreate
                (
                    formType,
                    ulClass,
                    FALSE
                );
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscAsn1AttrFree
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function free the memory of this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
AnscAsn1AttrFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    AnscFreeMemory(hThisObject);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ULONG
        AnscAsn1AttrGetSizeOfEncodedTag
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the encoding size of the tag;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the encoding size;

**********************************************************************/
ULONG
AnscAsn1AttrGetSizeOfEncodedTag
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATTR_OBJECT               pMyObject    = (PANSC_ATTR_OBJECT)hThisObject;

    if( pMyObject->FormType == EMPTY_FORM)
    {
        return 0;
    }

    /*
     * if class Number <= 0x1E,
     * one octet for  tag is  OK
     */
    if( pMyObject->ulClassNumber <= 0x1E)
    {
        return 1;
    }
    
    /* High-tag-number form (> 0x1E),Two or more octets
     *
     * If class number >0x1E, the first byte should be abc11111(B)
     * ab is the form type: Universal, Application , Context or others,
     * c  is Primitive or Constructed object type.
     * Second and following octets give the tag number, 
     * base 128, most significant digit first, with as few 
     * digits as possible, and with the bit 8 of each 
     * octet except the last set to "1."
     * 
     * I cannot believe the classnumber will be more than 128 * 128 * 128,
     */

    /* 
     * e.g.    Sender     : : = [ APPLICATION 196 ] VisibleString
     * encode: 5F 81 44 
     * e.g.    Recipient  : : = [ APPLICATION 182 ] VisibleString
     * encode: 5F 81 36 
     */

    if( pMyObject->ulClassNumber >= 128 * 128 * 128 )
    {
        return 5;
    }
    else if( pMyObject->ulClassNumber >= 128 * 128 )
    {
        return 4;
    }
    else if(pMyObject->ulClassNumber >= 128 )
    {
        return 3;
    }
    else
    {
        return 2;
    }
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1AttrClone
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function clones a sample attribute object

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the cloned attr object;

**********************************************************************/
ANSC_HANDLE
AnscAsn1AttrClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATTR_OBJECT               pMyObject    = (PANSC_ATTR_OBJECT)hThisObject;
    PANSC_ATTR_OBJECT               pNewObject;

    pNewObject  =  
        AnscAsn1AttrCreate
                (
                    pMyObject->FormType,
                    pMyObject->ulClassNumber,
                    pMyObject->bImplicit
                );
    
    if( pNewObject == NULL)
    {
        return NULL;
    }

    pNewObject->ulFixedLen          = pMyObject->ulFixedLen;
    pNewObject->ulMaxiLen           = pMyObject->ulMaxiLen;

    return pNewObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        BOOLEAN
        AnscAsn1AttrEqualsTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOtherObject
            );

    description:

        This function compares two attr objects the same value or not;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOtherObject
                The other object.

    return:     equals or not;

**********************************************************************/
BOOLEAN
AnscAsn1AttrEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject
    )
{
    PANSC_ATTR_OBJECT               pMyObject    = (PANSC_ATTR_OBJECT)hThisObject;
    PANSC_ATTR_OBJECT               pNewObject   = (PANSC_ATTR_OBJECT)hOtherObject;

    if( hThisObject == hOtherObject)
    {
        return TRUE;
    }

    if( hOtherObject == NULL || hThisObject == NULL)
    {
        return FALSE;
    }

    /*
     *  Actually, we don't care about the attr name matched or not;
     */
    if( 
        pMyObject->bImplicit    != pNewObject->bImplicit    ||
        pMyObject->FormType     != pNewObject->FormType     ||
        pMyObject->ulClassNumber!= pNewObject->ulClassNumber||
        pMyObject->ulFixedLen   != pNewObject->ulFixedLen   ||
        pMyObject->ulMaxiLen    != pNewObject->ulMaxiLen
      )
    {
       return FALSE;
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscAsn1AttrCopyValueFrom
            (
                ANSC_HANDLE                 hDestObject,
                ANSC_HANDLE                 hSourceObject
            );

    description:

        This function copies the value from another attr object.

    argument:   ANSC_HANDLE                 hDestObject
                This destination handle

                ANSC_HANDLE                 hSourceObject
                The source handle;

    return:     the status of the operation;

**********************************************************************/
ANSC_STATUS
AnscAsn1AttrCopyValueFrom
    (
        ANSC_HANDLE                 hDestObject,
        ANSC_HANDLE                 hSourceObject
    )
{
    PANSC_ATTR_OBJECT               pMyObject    = (PANSC_ATTR_OBJECT)hSourceObject;
    PANSC_ATTR_OBJECT               pNewObject   = (PANSC_ATTR_OBJECT)hDestObject;

    if( hDestObject == NULL || hSourceObject == NULL)
    {
        return ANSC_ASN1_NULL_OBJCET;
    }

    /*
     *  we don't care about the attr name value.
     */
    pNewObject->ulFixedLen          = pMyObject->ulFixedLen;
    pNewObject->ulMaxiLen           = pMyObject->ulMaxiLen;
    pNewObject->FormType            = pMyObject->FormType;
    pNewObject->ulClassNumber       = pMyObject->ulClassNumber;
    pNewObject->bImplicit           = pNewObject->bImplicit;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscAsn1AttrEncodingData
            (
                ANSC_HANDLE                 hThisObject,
                PVOID*                      ppEncoding,
                BOOLEAN                     bConstructed
            );

    description:

        This function encodes the attribute tag.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID*                      ppEncoding,
                The buffer of output binary data;

                BOOLEAN                     bConstructed
                The base object is constructed or not;

    return:     the status of the operation.

**********************************************************************/
ANSC_STATUS
AnscAsn1AttrEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding,
        BOOLEAN                     bConstructed
    )
{
    PANSC_ATTR_OBJECT               pMyObject    = (PANSC_ATTR_OBJECT)hThisObject;
    PUCHAR                          pCharData    = (PUCHAR)*ppEncoding;
    ULONG                           uRestValue;

    if( pMyObject->FormType == EMPTY_FORM)
    {
        return ANSC_STATUS_SUCCESS;
    }

    *pCharData                      = 0x00;

    switch( pMyObject->FormType)
    {
        case UNIVERSAL_FORM:

            break;

        case APPLICATION_FORM:

            *pCharData                  |= 0x40;

            break;

        case CONTEXT_FORM:

            *pCharData                  |= 0x80;

            break;

        case PRIVATE_FORM:

            *pCharData                  |= 0xC0;

            break;

        default:

            break;
    }

    if( bConstructed || !pMyObject->bImplicit)
    {
        *pCharData                      |= 0x20;
    }   

    /*
     * if class Number <= 0x1E,
     * one octet for  tag is  OK
     */
    if( pMyObject->ulClassNumber <= 0x1E)
    {
        *pCharData                     += (UCHAR)pMyObject->ulClassNumber; 
        pCharData ++;
    }
    else
    {    
        *pCharData                     += 0x1F;
        pCharData ++;

        /* High-tag-number form (> 0x1E),Two or more octets
         *
         * If class number >0x1E, the first byte should be abc11111(B)
         * ab is the form type: Universal, Application , Context or others,
         * c  is Primitive or Constructed object type.
         * Second and following octets give the tag number, 
         * base 128, most significant digit first, with as few 
         * digits as possible, and with the bit 8 of each 
         * octet except the last set to "1."
         * 
         * I cannot believe the classnumber will be more than 128 * 128 * 128,
         */

        /* 
         * e.g.    Sender     : : = [ APPLICATION 196 ] VisibleString
         * encode: 5F 81 44 
         * e.g.    Recipient  : : = [ APPLICATION 182 ] VisibleString
         * encode: 5F 81 36 
         */
        uRestValue                  = pMyObject->ulClassNumber;

        if( uRestValue >= 128 * 128 * 128 )
        {
            *pCharData              = (UCHAR)(uRestValue / (128 * 128 * 128)) | 0x80;
            uRestValue              = uRestValue % (128 * 128 * 128);
            pCharData ++;
        }
        if( uRestValue >= 128 * 128 )
        {
            *pCharData              = (UCHAR)(uRestValue / (128 * 128))| 0x80;
            uRestValue              = uRestValue % (128 * 128);
            pCharData ++;
        }
        if(uRestValue >= 128 )
        {
            *pCharData              = (UCHAR)(uRestValue / 128) | 0x80;
            uRestValue              = uRestValue % 128;
            pCharData ++;
        }

        *pCharData                  = (UCHAR)uRestValue;
        pCharData ++;
    }

    *ppEncoding                     = (PVOID)pCharData;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        BOOLEAN
        AnscAsn1AttrDump
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pBuffer,
                PULONG                      pLength
            );

    description:

        This function dumps the value to a buffer;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pBuffer,
                The output buffer;

                PULONG                      pLength
                The buffer of the output size;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1AttrDump
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength
    )
{

#ifndef _PKI_KERNEL

    PANSC_ATTR_OBJECT               pMyObject    = (PANSC_ATTR_OBJECT)hThisObject;
    ULONG                           length = 0; /*RDKB-6196, CID-24573, initialize before use*/
    errno_t                         rc     = -1;

    if( pBuffer == NULL || pLength == NULL || *pLength < 10)
    {
        return FALSE;
    }

    switch ( pMyObject->FormType)
    {
        case APPLICATION_FORM:
            if( pMyObject->bImplicit)
            {
                rc = 
                    sprintf_s
                        (
                            pBuffer,
                            *pLength,
                            "[APP %d IMP]", 
                            (int)pMyObject->ulClassNumber
                        );
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                length = rc;
            }
            else
            {
                rc = 
                    sprintf_s
                        (
                            pBuffer,
                            *pLength,
                            "[APP %d]", 
                            (int)pMyObject->ulClassNumber
                        );
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                length = rc;
            }
            break;

        case CONTEXT_FORM:
            if( pMyObject->bImplicit)
            {
                rc = 
                    sprintf_s
                        (
                            pBuffer,
                            *pLength,
                            "[CON %d IMP]", 
                            (int)pMyObject->ulClassNumber
                        );
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                length = rc;
            }
            else
            {
                rc = 
                    sprintf_s
                        (
                            pBuffer,
                            *pLength,
                            "[CON %d]", 
                            (int)pMyObject->ulClassNumber
                        );
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                length = rc;
            }
            break;

        case PRIVATE_FORM:
            if( pMyObject->bImplicit)
            {
                rc = 
                    sprintf_s
                        (
                            pBuffer,
                            *pLength,
                            "[PRI %d IMP]", 
                            (int)pMyObject->ulClassNumber
                        );
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                length = rc;
            }
            else
            {
                rc = 
                    sprintf_s
                        (
                            pBuffer,
                            *pLength,
                            "[PRI %d]", 
                            (int)pMyObject->ulClassNumber
                        );
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                    return FALSE;
                }
                length = rc;
            }
            break;

        case UNIVERSAL_FORM:
        default:
            break;   
    }

    *pLength = length;

#endif

    return TRUE;
}

