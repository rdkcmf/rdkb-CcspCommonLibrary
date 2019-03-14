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

    module:	ansc_external_storage.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file defines the externally stored data
        structures and arrays.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/19/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_EXTERNAL_STORAGE_
#define  _ANSC_EXTERNAL_STORAGE_


extern  char*  g_FileExtArray  [];
extern  ULONG  g_MediaTypeArray[];
extern  ULONG  g_SubTypeArray  [];

extern  char*  g_ProtoNameArray[];
extern  ULONG  g_ProtoTypeArray[];

extern  char*  g_AppPortNameArray[];
extern  ULONG  g_AppPortCodeArray[];

extern  char*  g_IpProtoNameArray[];
extern  ULONG  g_IpProtoCodeArray[];

extern  char*  g_PhyLinkNameArray[];
extern  ULONG  g_PhyLinkCodeArray[];

extern  UCHAR  g_OakleyGroup1_PrimeH[96];
extern  UCHAR  g_OakleyGroup1_PrimeN[96];
extern  ULONG  g_OakleyGroup1_Generator;
extern  UCHAR  g_OakleyGroup2_PrimeH[128];
extern  UCHAR  g_OakleyGroup2_PrimeN[128];
extern  ULONG  g_OakleyGroup2_Generator;
extern  ULONG  g_OakleyGroup3_FieldSize;
extern  UCHAR  g_OakleyGroup3_Prime;
extern  ULONG  g_OakleyGroup3_Generator1;
extern  ULONG  g_OakleyGroup3_CurveA;
extern  ULONG  g_OakleyGroup3_CurveB;
extern  UCHAR  g_OakleyGroup3_Order;
extern  ULONG  g_OakleyGroup4_FieldSize;
extern  UCHAR  g_OakleyGroup4_Prime;
extern  ULONG  g_OakleyGroup4_Generator1;
extern  ULONG  g_OakleyGroup4_CurveA;
extern  ULONG  g_OakleyGroup4_CurveB;
extern  UCHAR  g_OakleyGroup4_Order;


#endif
