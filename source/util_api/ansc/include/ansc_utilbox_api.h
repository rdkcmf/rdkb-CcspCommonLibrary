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

#ifndef  _ANSC_UTILBOX_API_
#define  _ANSC_UTILBOX_API_


typedef  struct
_ANSC_ID_STRING_PAIR
{
    const ULONG                     id;
    const PUCHAR                    string;
}
ANSC_ID_STRING_PAIR, *PANSC_ID_STRING_PAIR;


ULONG
AnscGetIanaMediaTypeByName
    (
        PUCHAR                      pMediaType
    );

ULONG
AnscGetIanaMediaTypeByName2
    (
        PUCHAR                      pMediaType,
        ULONG                       ulLen
    );

PUCHAR
AnscGetIanaMediaNameByType
    (
        ULONG                       ulMediaType
    );

ULONG
AnscGetIanaMediaSubtypeByName
    (
        ULONG                       ulMediaType,
        PUCHAR                      pMediaSubtype
    );

ULONG
AnscGetIanaMediaSubtypeByName2
    (
        ULONG                       ulMediaType,
        PUCHAR                      pMediaSubtype,
        ULONG                       ulLen
    );

PUCHAR
AnscGetIanaMediaSubtypeNameByType
    (
        ULONG                       ulMediaType,
        ULONG                       ulMediaSubtype
    );

BOOL
AnscIsIanaSubtypeUnknown
    (
        ULONG                       ulMediaType,
        ULONG                       ulMediaSubType
    );

PUCHAR
AnscIsvpGetStringById
    (
        const ANSC_ID_STRING_PAIR   pIsvp[],
        ULONG                       ulIsvpLen,
        ULONG                       Id,
        PUCHAR                      pDefaultValue
    );

ULONG
AnscIsvpGetIdByString
    (
        const ANSC_ID_STRING_PAIR   pIsvp[],
        ULONG                       ulIsvpLen,
        PUCHAR                      pStr,
        ULONG                       ulStrLen,
        BOOL                        bCaseSensitive,
        ULONG                       ulUnmappedValue
    );


#endif

