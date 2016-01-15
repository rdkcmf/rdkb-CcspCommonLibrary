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

#ifndef  _ANSC_UTILBOX_DEF_
#define  _ANSC_UTILBOX_DEF_


typedef  struct
_ANSC_UTILBOX_IANA_MEDIA
{
    const ULONG                     Type;
    const PUCHAR                    Name;
}
ANSC_UTILBOX_IANA_MEDIA, *PANSC_UTILBOX_IANA_MEDIA;


typedef  struct _ANSC_UTILBOX_IANA_MEDIA            ANSC_UTILBOX_IANA_MEDIA_SUBTYPE, *PANSC_UTILBOX_IANA_MEDIA_SUBTYPE;


typedef  struct
_ANSC_UTILBOX_IANA_MEDIA_SUBTYPE_LIST
{
    const ULONG                             MediaType;
    const ANSC_UTILBOX_IANA_MEDIA_SUBTYPE   *pSubtypes;
    const ULONG                             Count;
}
ANSC_UTILBOX_IANA_MEDIA_SUBTYPE_LIST, *PANSC_UTILBOX_IANA_MEDIA_SUBTYPE_LIST;


#endif

