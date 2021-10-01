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

    module: ansc_asn1_taginfo_interface.c

        For TagInfo (TCL) of ASN.1 object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by TagInfo 

        *   AnscTagInfoCreate
        *   AnscTagInfoFree
        *   AnscTagInfoParsingData
        *   AnscTagInfoGetSizeOfTag
        *   AnscTagInfoGetSizeOfLength

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

**********************************************************************/

#include "ansc_asn1_local.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscTagInfoCreate
            (
            );

    description:

        This function constructs a TagInfo (TCL) object.

    argument:   

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscTagInfoCreate
    (
    )
{
    PANSC_TAGINFO_OBJECT            pBaseObject  = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pBaseObject = (PANSC_TAGINFO_OBJECT)AnscAllocateMemory(sizeof(ANSC_TAGINFO_OBJECT));

    if ( !pBaseObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pBaseObject->Remove             = AnscTagInfoFree;
    pBaseObject->GetSizeOfLength    = AnscTagInfoGetSizeOfLength;
    pBaseObject->GetSizeOfTag       = AnscTagInfoGetSizeOfTag;
    pBaseObject->ParsingData        = AnscTagInfoParsingData;

    pBaseObject->bIsInfinite        = FALSE;
    pBaseObject->FirstOctetOfTag    = 0x00;
    pBaseObject->ulContentLength    = 0;
    pBaseObject->ulSizeOfLength     = 1;
    pBaseObject->ulSizeOfTag        = 1;

    return  (ANSC_HANDLE)pBaseObject;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscTagInfoFree
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
AnscTagInfoFree
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

        ANSC_STATUS
        AnscTagInfoParsingData
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pEncodingUnit
            );

    description:

        This function parses the given binary data and build up the tag
        information.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pEncodingUnit
                The input binary buffer;

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
AnscTagInfoParsingData
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pEncodingUnit
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_TAGINFO_OBJECT            pMyObject    = (PANSC_TAGINFO_OBJECT)hThisObject;

    /*
     * temporary counters
     */
    ULONG                           i            = 0;
    ULONG                           ulStartOfLength;

    if(pEncodingUnit[0] == 0x00)
    {
        /* AnscTrace("Unexpected tag '0x00' in AnscTagInfoParsingData().\n"); */

        return ANSC_ASN1_UNEXPECTED_TAG;
    }

    /***********************************************************
    TAG AND LENGTH DECODING
    ***********************************************************/
    
    /*
     * almost all the tags can be encoded in a single octet
     */
    if ( (pEncodingUnit[0] & TYPE_MASK) == 0x1F )
    {
        i = 1;
        
        while ( pEncodingUnit[i] & 0x80 )
        {
            i ++;
        }
    }

    if( i >= 4)
    {
        AnscTrace("Unbelievable! The tag length is more than 4 - '%lu'\n", ( i + 1 ));

        return ANSC_ASN1_UNEXPECTED_TAG;
    }

    pMyObject->ulSizeOfTag          = ( i + 1 );
    pMyObject->FirstOctetOfTag      = pEncodingUnit[0];
    pMyObject->bIsInfinite          = FALSE;

    ulStartOfLength                 = i + 1;

    /*
     * we consider it's a fatal error if the number of octets of content exceeds 0xFFFFFFFF
     */
    if ( pEncodingUnit[ulStartOfLength] & 0x80 )
    {
        if( pEncodingUnit[ulStartOfLength] == 0x80)
        {
            /* infinite mode */
            pMyObject->ulSizeOfLength   = 1;
            pMyObject->bIsInfinite      = TRUE;
            pMyObject->ulContentLength  = -1;
        }
        if ( (pEncodingUnit[ulStartOfLength] & 0x7F) > sizeof(ULONG) )
        {
            AnscTrace("Invalid encode length in AnscTagInfoParsingData(), it's more than 0xFFFFFFFF.\n");

            return ANSC_ASN1_INVALID_ENCODE_LENGTH;
        }
        else
        {
            pMyObject->ulSizeOfLength   = (pEncodingUnit[ulStartOfLength] & 0x7F) + 1;

            pMyObject->ulContentLength  = 
                GetLengthFromBuffer
                    (
                        &pEncodingUnit[ulStartOfLength + 1],
                        pEncodingUnit[ulStartOfLength] & 0x7F
                    ); 

            /* AnscTrace("The long content length is %d.\n", pMyObject->ulContentLength);*/
        }
    }
    else
    {
        pMyObject->ulSizeOfLength   = 1;
        pMyObject->ulContentLength  = (ULONG)pEncodingUnit[ulStartOfLength];
    }

    return returnStatus;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        LONG
        AnscTagInfoGetSizeOfTag
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the size of tag.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the size of tag.

**********************************************************************/
LONG
AnscTagInfoGetSizeOfTag
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TAGINFO_OBJECT            pMyObject    = (PANSC_TAGINFO_OBJECT)hThisObject;

    return (LONG)pMyObject->ulSizeOfTag;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        LONG
        AnscTagInfoGetSizeOfLength
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function returns the size of the content length.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the size of content length.

**********************************************************************/
LONG
AnscTagInfoGetSizeOfLength
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_TAGINFO_OBJECT            pMyObject    = (PANSC_TAGINFO_OBJECT)hThisObject;

    return (LONG)pMyObject->ulSizeOfLength;
}

