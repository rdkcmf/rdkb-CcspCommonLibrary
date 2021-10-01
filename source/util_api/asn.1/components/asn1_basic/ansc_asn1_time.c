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

    module: ansc_asn1_time.c

        For ASN.1 TIME object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by the ASN.1 base class "ASN1_UNIVERSAL_TIME"
        and "ASN1_GENERALIZEDTIME".

        *   AnscAsn1TimeCreate
        *   AnscAsn1UniversalTimeCreate
        *   AnscAsn1GeneralizedTimeCreate
        *   AnscAsn1TimeFree
        *   AnscAsn1TimeInitialize
        *   AnscAsn1TimeClone
        *   AnscAsn1TimeCopyTo
        *   AnscAsn1TimeEqualsTo
        *   AnscAsn1TimeGetSizeOfEncoded
        *   AnscAsn1TimeDecodingData
        *   AnscAsn1TimeEncodingData
        *   AnscAsn1TimeDumpObject
        *   AnscAsn1TimeTraceObject
        *   AnscAsn1TimeReadyToEncode

        *   AnscAsn1TimeSetTimeValue
        *   AnscAsn1TimeGetTimeValue
        *   AnscAsn1TimeSetUniversalTime
        *   AnscAsn1TimeIsBefore
        *   AnscAsn1TimeIsAfter

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/17/02    initial revision.

**********************************************************************/

#include "ansc_asn1_local.h"
#include "safec_lib_common.h"

#define  UNI_TIME_ENCODING_LENGTH           13
#define  GEN_TIME_ENCODING_LENGTH           15

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1TimeCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ULONG                       uTimeType
            );

    description:

        This function constructs an ASN.1 object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ULONG                       uTimeType,
                The object type of time:
                ASN1_UNIVERSALTIME_TYPE or ASN1_GENERALIZEDTIME_TYPE

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
AnscAsn1TimeCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ULONG                       uTimeType
    )
{
    switch ( uTimeType )
    {
        case ASN1_UNIVERSALTIME_TYPE:

                return AnscAsn1UniversalTimeCreate
                            (
                                hContainerContext
                            );

        case ASN1_GENERALIZEDTIME_TYPE:

                return AnscAsn1GeneralizedTimeCreate
                            (
                                hContainerContext
                            );

        default:

                return NULL;
    }
}
/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1UniversalTimeCreate
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
AnscAsn1UniversalTimeCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_ALTIME               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_ALTIME)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_ALTIME)AnscAllocateMemory(sizeof(ANSC_ASN1_ALTIME));
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
    pMyObject->Oid                 = ANSC_ASN1_UNIVERSAL_TIME_ID;
    pMyObject->uType               = ASN1_UNIVERSALTIME_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->Create              = AnscAsn1UniversalTimeCreate;
    pMyObject->AsnFree             = AnscAsn1TimeFree;
    
    AnscAsn1TimeInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_UNIVERSAL_TIME_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_UNIVERSAL_TIME_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1GeneralizedTimeCreate
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
AnscAsn1GeneralizedTimeCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_ALTIME               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_ALTIME)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_ALTIME)AnscAllocateMemory(sizeof(ANSC_ASN1_ALTIME));
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
    pMyObject->Oid                 = ANSC_ASN1_GENERALIZEDTIME_ID;
    pMyObject->uType               = ASN1_GENERALIZEDTIME_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->Create              = AnscAsn1GeneralizedTimeCreate;
    pMyObject->AsnFree             = AnscAsn1TimeFree;

    AnscAsn1TimeInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_GENERALIZED_TIME_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_GENERALIZED_TIME_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1TimeFree
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
AnscAsn1TimeFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    AnscAsn1Free(hThisObject);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1TimeInitialize
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
AnscAsn1TimeInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;

    /*
     *  Call base initialization first
     */
    AnscAsn1Initialize((ANSC_HANDLE)pMyObject);

    /* set other functions */
    pMyObject->Clone                = AnscAsn1TimeClone;
    pMyObject->CopyTo               = AnscAsn1TimeCopyTo;
    pMyObject->EqualsTo             = AnscAsn1TimeEqualsTo;
    pMyObject->DecodingData         = AnscAsn1TimeDecodingData;
    pMyObject->EncodingData         = AnscAsn1TimeEncodingData;
    pMyObject->GetSizeOfEncoded     = AnscAsn1TimeGetSizeOfEncoded;
    pMyObject->DumpObject           = AnscAsn1TimeDumpObject;
    pMyObject->ReadyToEncode        = AnscAsn1TimeReadyToEncode;

    pMyObject->GetTimeValue         = AnscAsn1TimeGetTimeValue;
    pMyObject->SetTimeValue         = AnscAsn1TimeSetTimeValue;
    pMyObject->SetUniveralTime      = AnscAsn1TimeSetUniversalTime;
    pMyObject->IsBefore             = AnscAsn1TimeIsBefore;
    pMyObject->IsAfter              = AnscAsn1TimeIsAfter;

    AnscZeroMemory((PVOID)&pMyObject->mTime, sizeof(ANSC_UNIVERSAL_TIME));

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1TimeClone
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
AnscAsn1TimeClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;
    PANSC_ASN1_ALTIME               pNewObject   = NULL;

    pNewObject = (PANSC_ASN1_ALTIME)
                    pMyObject->Create
                        (
                            pMyObject->hContainerContext
                        );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1TimeClone()\n");

        return NULL;
    }

    /*
     *  Copy all the attributes;
     */
    /* AttrListCopyAllAttributes( &pNewObject->sAttrList, &pMyObject->sAttrList);*/
    pNewObject->SetName(pNewObject, pMyObject->Name);

    if( ! pMyObject->CopyTo(pMyObject, pNewObject))
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
        AnscAsn1TimeClone
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDestObject
            );

    description:

        This function copies the value to the other one;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDestObject
                the dest object;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1TimeCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;
    PANSC_ASN1_ALTIME               pNewObject   = (PANSC_ASN1_ALTIME)hDestObject;

    if( pNewObject == NULL || pMyObject == NULL )
    {
        return FALSE;
    }

    /*
     * Copy the optional state;
     */
    pNewObject->bOptional =  pMyObject->bOptional;

    /*
     *  Copy values;
     */
    AnscCopyMemory( (PVOID)&pNewObject->mTime, (PVOID)&pMyObject->mTime, sizeof(ANSC_UNIVERSAL_TIME));

    pNewObject->bCanBeOptional  = pMyObject->bCanBeOptional;

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1TimeEqualsTo
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
AnscAsn1TimeEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    PANSC_ASN1_ALTIME               pMyObject  = (PANSC_ASN1_ALTIME)hThisObject;
    PANSC_ASN1_ALTIME               pNewObject = (PANSC_ASN1_ALTIME)hOtherObject;

    /* check the pointer first */
    if( hThisObject == hOtherObject)
    {
        return TRUE;
    }

    if( hThisObject == NULL || hOtherObject == NULL)
    {
        AnscTrace("One of the parameter in AnscAsn1TimeEqualsTo() is NULL pointer.\n");

        return FALSE;
    }

    /* check the type */
    if( !bValueOnly)
    {
        if( pMyObject->uType != pNewObject->uType)
        {
            AnscTrace("Different type in AnscAsn1TimeEqualsTo().\n");

            return FALSE;
        }
    }

    /* check the attribute list */
    if(!AttrListCompareAttributeList(&pMyObject->sAttrList, &pNewObject->sAttrList))
    {
        return FALSE;
    }
    
    /* check the optional state */
    if( pMyObject->bOptional != pNewObject->bOptional)
    {
        return FALSE;
    }

    /* check the value */
    if(!pMyObject->bOptional)
    {
        return AnscEqualMemory
                    (
                        (PVOID)&pMyObject->mTime,
                        (PVOID)&pNewObject->mTime,
                        sizeof(ANSC_UNIVERSAL_TIME)
                    );            
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1TimeGetSizeOfEncoded
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
AnscAsn1TimeGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;
    ULONG                           ulSize       = 0;

    /*
     * If it's optional, don't need encode
     */
    if( pMyObject->bOptional)
    {
        return 0;
    }

    if( !pMyObject->ReadyToEncode(pMyObject))
    {
        AnscTrace("The value of the TIME object is not ready to encode.\n");

        return -1;
    }

    if( pMyObject->uType == ASN1_UNIVERSALTIME_TYPE)
    {
        ulSize          = UNI_TIME_ENCODING_LENGTH;
    }
    else
    {
        ulSize          = GEN_TIME_ENCODING_LENGTH;
    }


    /*
     *  check the attribute list, from the end of the list;
     */ 
    return AttrListGetSizeOfEncoded(&pMyObject->sAttrList, ulSize, 1);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1TimeDecodingData
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
AnscAsn1TimeDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;
    PANSC_TAGINFO_OBJECT            pTagInfo     = NULL;
    BOOLEAN                         bTagIncluded = TRUE;
    ULONG                           uLeftSize,uInfiniteTags;

    /*
     * we use this string to hold the visiable string of the year, month, day, hour, miniute and second
     */
    char                           timeChar[5]   = { 0 };
    int                            counter       = 0;
    ULONG                          moreHour      = 0;
    ULONG                          moreMinute    = 0;  
    BOOLEAN                        less          = FALSE;

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
    
    /* if tag included, decode the tag */
    if( bTagIncluded)
    {
        pTagInfo        = (PANSC_TAGINFO_OBJECT)AnscTagInfoCreate();

        if( pTagInfo == NULL)
        {
            return ANSC_STATUS_RESOURCES;
        }

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
            /*
            AnscTrace
                (
                    "Unexpected tag in ASN1_TIME - '0x%.2X', expected - '0x%.2X'\n",
                    pTagInfo->FirstOctetOfTag,
                    pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject)
                );
            */
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNEXPECTED_TAG;
        }

        if( pTagInfo->ulContentLength == 0 ||
            pTagInfo->GetSizeOfTag(pTagInfo)     != 1)
        {
            AnscTrace("The length of TIME tag is unacceptable.\n");

            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
        }

        /* check the tag */
        if( pTagInfo->bIsInfinite)
        {
            AnscTrace("Infinite form is unacceptable for TIME object.\n");
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_INVALID_ENCODE_LENGTH;
        }

        pEncodingUnit   += 1 + pTagInfo->GetSizeOfLength(pTagInfo);
        uLeftSize       =  pTagInfo->ulContentLength;

        pTagInfo->Remove((ANSC_HANDLE)pTagInfo);
    }

    if( uInfiniteTags > 0)
    {
        AnscTrace("Infinite form is unacceptable for TIME object.\n");

        return ANSC_ASN1_INVALID_ENCODE_LENGTH;
    }

    /**************************************************
     *
     *  Decode the value;
     *
     **************************************************/
    /*
     *  Get year;
     */
    timeChar[0]                     = pEncodingUnit[counter++];
    timeChar[1]                     = pEncodingUnit[counter++];

    /* 
     * ASN1_UNIVERSALTIME_TYPE time have 2 digits of year,
     * but ASN1_GENERALIZEDTIME_TYPE  time  have  4 digits  of  year;
     */
    if(pMyObject->uType == ASN1_GENERALIZEDTIME_TYPE)
    {
        timeChar[2]                 = pEncodingUnit[counter++];
        timeChar[3]                 = pEncodingUnit[counter++];
        timeChar[4]                 = 0;
    }
    else
    {
        timeChar[2]                 = 0;
    }

    pMyObject->mTime.Year           = (USHORT)AnscString2Int(timeChar);

    /*
     *  Get month;
     */
    timeChar[0]                     = pEncodingUnit[counter++];
    timeChar[1]                     = pEncodingUnit[counter++];
    timeChar[2]                     = 0;

    pMyObject->mTime.Month          = (USHORT)AnscString2Int(timeChar);

    /*
     *  Get day;
     */
    timeChar[0]                     = pEncodingUnit[counter++];
    timeChar[1]                     = pEncodingUnit[counter++];
    timeChar[2]                     = 0;

    pMyObject->mTime.DayOfMonth     = (USHORT)AnscString2Int(timeChar);

    /*
     *  Get hour;
     */
    timeChar[0]                     = pEncodingUnit[counter++];
    timeChar[1]                     = pEncodingUnit[counter++];
    timeChar[2]                     = 0;

    pMyObject->mTime.Hour           = (USHORT)AnscString2Int(timeChar);

    /*
     *  Get minute;
     */
    timeChar[0]                     = pEncodingUnit[counter++];
    timeChar[1]                     = pEncodingUnit[counter++];
    timeChar[2]                     = 0;

    pMyObject->mTime.Minute         = (USHORT)AnscString2Int(timeChar);

    /*
     *  Get second;
     */
    timeChar[0]                     = pEncodingUnit[counter++];
    timeChar[1]                     = pEncodingUnit[counter++];
    timeChar[2]                     = 0;

    pMyObject->mTime.Second         = (USHORT)AnscString2Int(timeChar);

    if ( pEncodingUnit[counter] == '+')
    {
        timeChar[2]                 = 0;
        timeChar[0]                 = pEncodingUnit[counter++];
        timeChar[1]                 = pEncodingUnit[counter++];
        moreHour                    = (USHORT)AnscString2Int(timeChar);


        timeChar[0]                 = pEncodingUnit[counter++];
        timeChar[1]                 = pEncodingUnit[counter++];
        moreMinute                  = (USHORT)AnscString2Int(timeChar);

        if(pMyObject->mTime.Minute >= moreMinute)
        {
            pMyObject->mTime.Minute -= (USHORT)moreMinute;
        }
        else
        {
            pMyObject->mTime.Minute += 60;
            pMyObject->mTime.Minute -= (USHORT)moreMinute;
            less                    = TRUE;
        }

        if(less)
        {
            moreHour ++;
        }

        if(pMyObject->mTime.Hour <= moreHour)
        {
            less                    = TRUE;
            pMyObject->mTime.Hour   += 24;
            pMyObject->mTime.Hour   -= (USHORT)moreHour;
        }
        else
        {
            less                    = FALSE;
            pMyObject->mTime.Hour   -= (USHORT)moreHour;
        }

        if(less)
        {
            pMyObject->mTime.DayOfMonth -=1;
        }
    }
    else if( pEncodingUnit[counter] == '-')
    {
        timeChar[2]                 = 0;
        timeChar[0]                 = pEncodingUnit[counter++];
        timeChar[1]                 = pEncodingUnit[counter++];
        moreHour                    = (USHORT)AnscString2Int(timeChar);

        timeChar[0]                 = pEncodingUnit[counter++];
        timeChar[1]                 = pEncodingUnit[counter++];
        moreMinute                  = (USHORT)AnscString2Int(timeChar);

        pMyObject->mTime.Minute     += (USHORT)moreMinute;
        if(pMyObject->mTime.Minute >= 60)
        {
            pMyObject->mTime.Minute -= 60;
            pMyObject->mTime.Hour   += 1;
        }
        
        pMyObject->mTime.Hour       += (USHORT)moreHour;

        if(pMyObject->mTime.Hour >23)
        {
            pMyObject->mTime.Hour               -= 24;
            pMyObject->mTime.DayOfMonth         += 1;

            if(pMyObject->mTime.DayOfMonth > 31)
            {
                pMyObject->mTime.DayOfMonth     = 1;
                pMyObject->mTime.Month          +=1;
                if(pMyObject->mTime.Month > 12 )
                {
                    pMyObject->mTime.Month      = 1;
                    pMyObject->mTime.Year       +=1;
                }
            }
        }
    }
    else if ( pEncodingUnit[counter] != 'Z' && pEncodingUnit[counter] != '.' )
    {
        AnscTrace("Invalid Time Encoding in AnscAsn1TimeDecodingWithoutTag()!\n");

        return  ANSC_ASN1_INVALID_TIME_VALUE;
    }

    /*
     *  the last 'Z';
     */
    counter ++;

    if ( pMyObject->mTime.Year          > 5999  || 
         pMyObject->mTime.Month         > 12    || 
         pMyObject->mTime.DayOfMonth    > 31    ||
         pMyObject->mTime.Hour          > 23    || 
         pMyObject->mTime.Minute        > 59    || 
         pMyObject->mTime.Second        > 59 
        )
    {
        AnscTrace("Invalid Time value in AnscAsn1TimeDecodingWithoutTag()!\n");

        return  ANSC_ASN1_INVALID_TIME_VALUE;
    }

    /*
     * If the time type is ASN1_UNIVERSALTIME_TYPE, turn  the  year  value  to
     * four digit value;
     */
    if(pMyObject->uType == ASN1_UNIVERSALTIME_TYPE)
    {
        if ( pMyObject->mTime.Year >= 50 )
        {
            pMyObject->mTime.Year   += 1900;
        }
        else
        {
            pMyObject->mTime.Year   += 2000;
        }
    }

    if( uLeftSize != (ULONG)counter)
    {
        AnscTrace
            (
               "Decoding size of TIME object is '%lu', but actually it's %d.\n",
               uLeftSize,
               counter
            );

       return ANSC_ASN1_INVALID_ENCODE_LENGTH;
    }

    /*
     * Add  the size to the end of this  value;
     */ 
    pMyObject->bOptional            = FALSE;
    pEncodingUnit                   += uLeftSize;

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

    *ppEncoding                     =  (PVOID)pEncodingUnit;

    /* AnscTrace("Total decoding length for TIME object: %d\n", counter); */

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1TimeEncodingData
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
AnscAsn1TimeEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_ALTIME               pMyObject       = (PANSC_ASN1_ALTIME)hThisObject;
    BOOLEAN                         bTagIncluded    = TRUE;
    LONG                            uSizeOfEncoded;
    ULONG                           uLeftSize;

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
     *  check before Enecoding;
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
        AnscTrace("Failed to encode the attrlist of TIME objects.\n");

        return returnStatus;
    }

    /*
     *  Encode the tag first;
     */
    if ( bTagIncluded)
    {
        *pCharData                  = pMyObject->GetFirstOctet(pMyObject);
        pCharData++;

        if( pMyObject->uType == ASN1_UNIVERSALTIME_TYPE)
        {
            EncodeLength((PVOID*)&pCharData, UNI_TIME_ENCODING_LENGTH);
        }
        else
        {
            EncodeLength((PVOID*)&pCharData, GEN_TIME_ENCODING_LENGTH);
        }
    }

   /*
    *   Encode the value;
    */
    if( pMyObject->uType == ASN1_GENERALIZEDTIME_TYPE)
    {
        *pCharData                  = (UCHAR)(pMyObject->mTime.Year/1000 + 0x30);
        pCharData++;

        *pCharData                  = (UCHAR)((pMyObject->mTime.Year/100) % 10 + 0x30);
        pCharData++;        
    }

    *pCharData                      = (UCHAR)((pMyObject->mTime.Year / 10) % 10 + 0x30);
    pCharData++;        

    *pCharData                      = (UCHAR)(pMyObject->mTime.Year % 10 + 0x30);
    pCharData++;        

    *pCharData                      = (UCHAR)(pMyObject->mTime.Month / 10 + 0x30);
    pCharData++;        

    *pCharData                      = (UCHAR)(pMyObject->mTime.Month % 10 + 0x30);
    pCharData++;        

    *pCharData                      = (UCHAR)(pMyObject->mTime.DayOfMonth / 10 + 0x30);
    pCharData++;        

    *pCharData                      = (UCHAR)(pMyObject->mTime.DayOfMonth % 10 + 0x30);
    pCharData++;        

    *pCharData                      = (UCHAR)(pMyObject->mTime.Hour / 10 + 0x30);
    pCharData++;        

    *pCharData                      = (UCHAR)(pMyObject->mTime.Hour % 10 + 0x30);
    pCharData++;        

    *pCharData                      = (UCHAR)(pMyObject->mTime.Minute / 10 + 0x30);
    pCharData++;        

    *pCharData                      = (UCHAR)(pMyObject->mTime.Minute % 10 + 0x30);
    pCharData++;        

    *pCharData                      = (UCHAR)(pMyObject->mTime.Second / 10 + 0x30);
    pCharData++;        

    *pCharData                      = (UCHAR)(pMyObject->mTime.Second % 10 + 0x30);
    pCharData++;        

    *pCharData                      = 'Z';
    pCharData++;

    /*
     *  Set the buffer before exit;
     */
    if( pCharData - pBackData != uSizeOfEncoded)
    {
        /*
        AnscTrace
            (
                "Warning in Encoding TIME, the size of encoding \
                is expected to be %d, but it's %d.\n",
                uSizeOfEncoded,
                (pCharData - pBackData)
            );
        */
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
        AnscAsn1TimeDumpObject
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
AnscAsn1TimeDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    )
{
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;
    CHAR                            pAttrBuf[512]= { 0 };
    ULONG                           attrLength   = 512;
    PCHAR                           pName;
    errno_t                         rc           = -1;

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
        rc =
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s (%.4d/%.2d/%.2d %.2d:%.2d:%.2d)",
                    pName,
                    pAttrBuf,
                    ASN1Type2String(pMyObject->uType),
                    pMyObject->mTime.Year,
                    pMyObject->mTime.Month,
                    pMyObject->mTime.DayOfMonth,
                    pMyObject->mTime.Hour,
                    pMyObject->mTime.Minute,
                    pMyObject->mTime.Second
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
        AnscAsn1TimeTraceObject
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
AnscAsn1TimeTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    )
{

    UNREFERENCED_PARAMETER(bRecursive);
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;
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
                "%s ::=%s %s (%.4d/%.2d/%.2d %.2d:%.2d:%.2d)\n",
                pName,
                pAttrBuf,
                ASN1Type2String(pMyObject->uType),
                pMyObject->mTime.Year,
                pMyObject->mTime.Month,
                pMyObject->mTime.DayOfMonth,
                pMyObject->mTime.Hour,
                pMyObject->mTime.Minute,
                pMyObject->mTime.Second
             );
    }

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1TimeReadyToEncode
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
AnscAsn1TimeReadyToEncode
    (   
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;

    if( pMyObject->bOptional)
    {
        return TRUE;
    }

    /*
     * Check the validity of the value
     */
    if( pMyObject->mTime.Year       == 0    || 
        pMyObject->mTime.Year       >  5999 || 
        pMyObject->mTime.Month      > 12    ||
        pMyObject->mTime.DayOfMonth > 31   || 
        pMyObject->mTime.Hour       > 24   ||
        pMyObject->mTime.Minute     > 60   || 
        pMyObject->mTime.Second     > 60
      )
    {
        return FALSE;
    }

    return TRUE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1TimeGetTimeValue
            (
                ANSC_HANDLE                 hThisObject,
                PULONG                      pYear,
                PULONG                      pMonth,
                PULONG                      pDay,
                PULONG                      pHour,
                PULONG                      pMinute,
                PULONG                      pSecond
            );

    description:

        This function returns the time value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PULONG                      pYear,
                PULONG                      pMonth,
                PULONG                      pDay,
                PULONG                      pHour,
                PULONG                      pMinute,
                PULONG                      pSecond
                The value buffers of the time.

    return:     the status of the operation.

**********************************************************************/
ANSC_STATUS
AnscAsn1TimeGetTimeValue
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pYear,
        PULONG                      pMonth,
        PULONG                      pDay,
        PULONG                      pHour,
        PULONG                      pMinute,
        PULONG                      pSecond
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;

    if( pYear != NULL)
    {
        *pYear                      = pMyObject->mTime.Year;
    }

    if( pMonth != NULL)
    {
        *pMonth                     = pMyObject->mTime.Month;
    }

    if( pDay != NULL)
    {
        *pDay                       = pMyObject->mTime.DayOfMonth;
    }

    if( pHour != NULL)
    {
        *pHour                      = pMyObject->mTime.Hour;
    }

    if( pMinute != NULL)
    {
        *pMinute                    = pMyObject->mTime.Minute;
    }

    if( pSecond != NULL)
    {
        *pSecond                     = pMyObject->mTime.Second;
    }

    return returnStatus;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1TimeSetTimeValue
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       uYear,
                ULONG                       uMonth,
                ULONG                       uDay,
                ULONG                       uHour,
                ULONG                       uMinute,
                ULONG                       uSecond
            );

    description:

        This function set the time value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       uYear,
                ULONG                       uMonth,
                ULONG                       uDay,
                ULONG                       uHour,
                ULONG                       uMinute,
                ULONG                       uSecond
                The input time value;

    return:     the status of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1TimeSetTimeValue
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uYear,
        ULONG                       uMonth,
        ULONG                       uDay,
        ULONG                       uHour,
        ULONG                       uMinute,
        ULONG                       uSecond
    )
{
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;

    /*
     * Check the validity of the value
     */
    if( uYear   == 0    || uMonth  >= 13 ||
        uDay    >  31   || uHour   >= 24 ||
        uMinute >= 60   || uSecond >= 60
      )
    {
        return ANSC_ASN1_INVALID_TIME_VALUE;
    }

    pMyObject->mTime.Year           = (USHORT)uYear;
    pMyObject->mTime.Month          = (USHORT)uMonth;
    pMyObject->mTime.DayOfMonth     = (USHORT)uDay;
    pMyObject->mTime.Hour           = (USHORT)uHour;
    pMyObject->mTime.Minute         = (USHORT)uMinute;
    pMyObject->mTime.Second         = (USHORT)uSecond;

    /*
     *  set as no-optional
     */
    pMyObject->bOptional            = FALSE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1TimeSetUniversalTime
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_UNIVERSAL_TIME         uniTime
            );

    description:

        This function set the univseral time

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_UNIVERSAL_TIME         uniTime;
                The input time value;

    return:     the status of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1TimeSetUniversalTime
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_UNIVERSAL_TIME         uniTime
    )
{
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;


    AnscCopyMemory
        (
            &pMyObject->mTime,
            &uniTime,
            sizeof(ANSC_UNIVERSAL_TIME)
        );

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1TimeIsBefore
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       uYear,
                ULONG                       uMonth,
                ULONG                       uDay,
                ULONG                       uHour,
                ULONG                       uMinute,
                ULONG                       uSecond
            );

    description:

        This function check the time value is before the given time or 
        not.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       uYear,
                ULONG                       uMonth,
                ULONG                       uDay,
                ULONG                       uHour,
                ULONG                       uMinute,
                ULONG                       uSecond
                The given time value;

    return:     before or not

**********************************************************************/
BOOLEAN
AnscAsn1TimeIsBefore
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uYear,
        ULONG                       uMonth,
        ULONG                       uDay,
        ULONG                       uHour,
        ULONG                       uMinute,
        ULONG                       uSecond
    )
{
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;

    /*
     * Check the year;
     */
    if( uYear > pMyObject->mTime.Year)
    {
        return TRUE;
    }
    else if( uYear < pMyObject->mTime.Year)
    {
        return FALSE;
    }

    /*
     * Check the month;
     */
    if( uMonth > pMyObject->mTime.Month)
    {
        return TRUE;
    }
    else if( uMonth < pMyObject->mTime.Month)
    {
        return FALSE;
    }

    /*
     * Check the day;
     */
    if( uDay > pMyObject->mTime.DayOfMonth)
    {
        return TRUE;
    }
    else if( uDay < pMyObject->mTime.DayOfMonth)
    {
        return FALSE;
    }

    /*
     * Check the hour;
     */
    if( uHour > pMyObject->mTime.Hour)
    {
        return TRUE;
    }
    else if( uHour < pMyObject->mTime.Hour)
    {
        return FALSE;
    }

    /*
     * Check the Minute;
     */
    if( uMinute > pMyObject->mTime.Minute)
    {
        return TRUE;
    }
    else if( uMinute < pMyObject->mTime.Minute)
    {
        return FALSE;
    }

    /*
     * Check the second;
     */
    if( uSecond > pMyObject->mTime.Second)
    {
        return TRUE;
    }
    else if( uSecond < pMyObject->mTime.Second)
    {
        return FALSE;
    }
    
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1TimeIsAfter
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       uYear,
                ULONG                       uMonth,
                ULONG                       uDay,
                ULONG                       uHour,
                ULONG                       uMinute,
                ULONG                       uSecond
            );

    description:

        This function check the time value is after the given time or 
        not.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       uYear,
                ULONG                       uMonth,
                ULONG                       uDay,
                ULONG                       uHour,
                ULONG                       uMinute,
                ULONG                       uSecond
                The given time value;

    return:     after or not

**********************************************************************/
BOOLEAN
AnscAsn1TimeIsAfter
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uYear,
        ULONG                       uMonth,
        ULONG                       uDay,
        ULONG                       uHour,
        ULONG                       uMinute,
        ULONG                       uSecond
    )
{
    PANSC_ASN1_ALTIME               pMyObject    = (PANSC_ASN1_ALTIME)hThisObject;

    /*
     * Check the year;
     */
    if( uYear > pMyObject->mTime.Year)
    {
        return FALSE;
    }
    else if( uYear < pMyObject->mTime.Year)
    {
        return TRUE;
    }

    /*
     * Check the month;
     */
    if( uMonth > pMyObject->mTime.Month)
    {
        return FALSE;
    }
    else if( uMonth < pMyObject->mTime.Month)
    {
        return TRUE;
    }

    /*
     * Check the day;
     */
    if( uDay > pMyObject->mTime.DayOfMonth)
    {
        return FALSE;
    }
    else if( uDay < pMyObject->mTime.DayOfMonth)
    {
        return TRUE;
    }

    /*
     * Check the hour;
     */
    if( uHour > pMyObject->mTime.Hour)
    {
        return FALSE;
    }
    else if( uHour < pMyObject->mTime.Hour)
    {
        return TRUE;
    }

    /*
     * Check the Minute;
     */
    if( uMinute > pMyObject->mTime.Minute)
    {
        return FALSE;
    }
    else if( uMinute < pMyObject->mTime.Minute)
    {
        return TRUE;
    }

    /*
     * Check the second;
     */
    if( uSecond > pMyObject->mTime.Second)
    {
        return FALSE;
    }
    else if( uSecond < pMyObject->mTime.Second)
    {
        return TRUE;
    }
    
    return FALSE;
}

