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

    module: ansc_asn1_taginfo_interface.h

        For Abstract Syntax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for the
        taginfo (TCL) of ASN.1 .

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


#ifndef  _ANSC_ASN1_TAGINFO_INTERFACE_
#define  _ANSC_ASN1_TAGINFO_INTERFACE_

/***********************************************************
           GENERAL OBJECT FUNCTIONS DEFINITION
***********************************************************/
/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_TAGINFO_CREATE)
    (
    );

typedef  ANSC_STATUS
(*PFN_TAGINFO_FREE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TAGINFO_PARSING_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pData
    );

typedef  LONG
(*PFN_TAGINFO_GET_LONG_ATTR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOLEAN
(*PFN_TAGINFO_IS_INFINITE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  UCHAR
(*PFN_TAGINFO_GET_FIRST_OCTET)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The taginfo object contains one tag and encoded-length.
 */
#define  ANSC_TAGINFO_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    ULONG                           ulSizeOfTag;                                            \
    ULONG                           ulSizeOfLength;                                         \
    LONG                            ulContentLength;                                        \
    UCHAR                           FirstOctetOfTag;                                        \
    BOOLEAN                         bIsInfinite;                                            \
                                                                                            \
    PFN_TAGINFO_FREE                Remove;                                                 \
                                                                                            \
    PFN_TAGINFO_PARSING_DATA        ParsingData;                                            \
    PFN_TAGINFO_GET_LONG_ATTR       GetSizeOfTag;                                           \
    PFN_TAGINFO_GET_LONG_ATTR       GetSizeOfLength;                                        \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TAGINFO_OBJECT
{
    ANSC_TAGINFO_CLASS_CONTENT
}
ANSC_TAGINFO_OBJECT,  *PANSC_TAGINFO_OBJECT;

#define  ACCESS_ANSC_TAGINFO_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_TAGINFO_OBJECT, Linkage)

#endif /*_ANSC_ASN1_TAGINFO_INTERFACE_*/
