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

    module: ansc_asn1_external_api.c

        For Abstract Syntax Notation One (ASN.1)
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file implements the all the basic exported functions
        provided by the ASN.1 base class

            *   AnscAsn1CreateAny
            *   AnscAsn1CreateInteger
            *   AnscAsn1CreateBOOL
            *   AnscAsn1CreateReal
            *   AnscAsn1CreateEnumerate
            *   AnscAsn1CreateNULL
            *   AnscAsn1CreateUniversalTime
            *   AnscAsn1CreateGeneralizedTime
            *   AnscAsn1CreateOID
            *   AnscAsn1CreateOctetString
            *   AnscAsn1CreateUTF8String
            *   AnscAsn1CreateUniversalString
            *   AnscAsn1CreateTeletexString
            *   AnscAsn1CreateIA5String
            *   AnscAsn1CreatePrintableString
            *   AnscAsn1CreateVideotexString
            *   AnscAsn1CreateVisibleString
            *   AnscAsn1CreateNumericString
            *   AnscAsn1CreateGeneralString
            *   AnscAsn1CreateGraphicString
            *   AnscAsn1CreateBMPString
            *   AnscAsn1CreateMSString
            *   AnscAsn1CreateBitString
            *   AnscAsn1CreateChoice
            *   AnscAsn1CreateSet
            *   AnscAsn1CreateSequence
            *   AnscAsn1CreateSetOf
            *   AnscAsn1CreateSequenceOf

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        04/18/02    initial revision.

**********************************************************************/

#include "ansc_asn1_local.h"

ANSC_HANDLE
AnscAsn1CreateAny
    (
        ANSC_HANDLE                 hReserved
    )
{
    return 
        AnscAsn1AnyCreate
            (
                hReserved
            );

}


ANSC_HANDLE
AnscAsn1CreateInteger
    (
        ANSC_HANDLE                 hReserved
    )
{
    return 
        AnscAsn1IntegerCreate
            (
                hReserved
            );
}


ANSC_HANDLE
AnscAsn1CreateBOOL
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1BOOLCreate
            (
                hReserved
            );
}


ANSC_HANDLE
AnscAsn1CreateReal
    (
        ANSC_HANDLE                 hReserved
    )
{
#ifndef _PKI_KERNEL

    return
        AnscAsn1RealCreate
            (
                hReserved
            );

#else

    return NULL;

#endif
}


ANSC_HANDLE
AnscAsn1CreateEnumerate
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1EnumerateCreate
            (
                hReserved
            );
}


ANSC_HANDLE
AnscAsn1CreateNULL
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1NULLCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateUniversalTime
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1UniversalTimeCreate
            (
                hReserved
            );
}


ANSC_HANDLE
AnscAsn1CreateGeneralizedTime
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1GeneralizedTimeCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateOID
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1OIDCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateOctetString
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1OctetStringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateUTF8String
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1UTF8StringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateUniversalString
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1UniversalStringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateTeletexString
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1TeletexStringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateIA5String
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1IA5StringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreatePrintableString
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1PrintableStringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateVideotexString
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1VideotexStringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateVisibleString
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1VisibleStringCreate
            (
                hReserved
            );
}


ANSC_HANDLE
AnscAsn1CreateNumericString
    (
        ANSC_HANDLE                 hReserved
    )
{
    return 
        AnscAsn1NumericStringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateGeneralString
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1GeneralStringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateGraphicString
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1GraphicStringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateBMPString
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1BMPStringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateMSString
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1MSStringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateBitString
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1BitStringCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateChoice
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1ChoiceCreate
            (
                hReserved
            );
}


ANSC_HANDLE
AnscAsn1CreateSet
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1SetCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateSequence
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1SequenceCreate
            (
                hReserved
            );
}

ANSC_HANDLE
AnscAsn1CreateSetOf
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1SetOfCreate
            (
                hReserved
            );
}


ANSC_HANDLE
AnscAsn1CreateSequenceOf
    (
        ANSC_HANDLE                 hReserved
    )
{
    return
        AnscAsn1SequenceOfCreate
            (
                hReserved
            );
}


