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

    module:	slap_tlv_format.h

        For SLAP
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the SLAP TLV memory format

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin     Zhu     

    ---------------------------------------------------------------

    revision:

        05/12/07    initial revision.

**********************************************************************/


#ifndef  _SLAP_TLV_FORMAT_H_
#define  _SLAP_TLV_FORMAT_H_


/***********************************************************
         SLAP CONFIGURATION FORMAT DEFINITION
***********************************************************/

#define  SLAP_TLV_TYPE_PAD                          0       /* length = n/a,      value = n/a                       */
#define  SLAP_TLV_TYPE_INTEGER                      1       /* length = 4,        value = integer                   */         
#define  SLAP_TLV_TYPE_UINT32                       2       /* length = 4,        value = uint32                    */         
#define  SLAP_TLV_TYPE_BOOL                         3       /* length = 1,        value = 0/1                       */            
#define  SLAP_TLV_TYPE_STRING                       4       /* length = variable, value = string value              */         
#define  SLAP_TLV_TYPE_UCHARARRAY                   5       /* length = variable, value = uchar array               */         
#define  SLAP_TLV_TYPE_END_OF_DATA                  255     /* length = n/a,      value = n/a                       */

typedef  struct
_SLAP_TLV_TUPLE
{
    UCHAR                           Type;                           /* specifies how to interpret the "value" field       */
    ULONG                           Length;                         /* specifies the length of "value" field in octets    */
    char                            Value[ANSC_ZERO_ARRAY_SIZE];    /* its content is type-specific, and module-dependent */
}_struct_pack_
SLAP_TLV_TUPLE,  *PSLAP_TLV_TUPLE;

#define  SlapTlvGetType(p)                          ( p->Type                                        )
#define  SlapTlvGetLength(p)                        ( AnscUlongFromNToH(AnscReadUlong(&p->Length)) )
#define  SlapTlvGetValue(p)                         ( p->Value                                       )
#define  SlapTlvGetSize(p)                          ( sizeof(SLAP_TLV_TUPLE) + SlapTlvGetLength(p)   )
#define  SlapTlvCopyValue(p, d)                     AnscCopyMemory(d, p->Value, SlapTlvGetLength(p))

#define  SlapTlvSetType(p, t)                       ( p->Type  = t                                   )
#define  SlapTlvSetLength(p, l)                     AnscWriteUlong(&p->Length, AnscUlongFromHToN(l))
#define  SlapTlvSetValue(p, v)                      AnscCopyMemory(p->Value, v, SlapTlvGetLength(p))

#define  SlapTlvGetNextTuple(p)                     (PSLAP_TLV_TUPLE)((ULONG)p + SlapTlvGetSize(p)     )
#define  SlapTlvGetNextLevel(p)                     (PSLAP_TLV_TUPLE)((ULONG)p + sizeof(SLAP_TLV_TUPLE))

#define  SlapTlvVerifyTupleStream(p, size, v_result)                                        \
         {                                                                                  \
            PSLAP_TLV_TUPLE         cur_tuple = (PSLAP_TLV_TUPLE)p;                         \
            ULONG                   rest_size = size;                                       \
                                                                                            \
            if ( size == 0 )                                                                \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else if ( (size                      == 1                        ) &&           \
                      (SlapTlvGetType(cur_tuple) != SLAP_TLV_TYPE_END_OF_DATA) &&           \
                      (SlapTlvGetType(cur_tuple) != SLAP_TLV_TYPE_PAD        ) )            \
            {                                                                               \
                v_result = FALSE;                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                v_result = TRUE;                                                            \
                                                                                            \
                while ( (SlapTlvGetType(cur_tuple) != SLAP_TLV_TYPE_END_OF_DATA) &&         \
                        (SlapTlvGetType(cur_tuple) != SLAP_TLV_TYPE_PAD        ) )          \
                {                                                                           \
                    if ( SlapTlvGetSize(cur_tuple) > rest_size )                            \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        rest_size -= SlapTlvGetSize     (cur_tuple);                        \
                        cur_tuple  = SlapTlvGetNextTuple(cur_tuple);                        \
                    }                                                                       \
                                                                                            \
                    if ( rest_size == 0 )                                                   \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                                                                                            \
                        break;                                                              \
                    }                                                                       \
                }                                                                           \
                                                                                            \
                if ( v_result )                                                             \
                {                                                                           \
                    if ( SlapTlvGetType(cur_tuple) != SLAP_TLV_TYPE_END_OF_DATA )           \
                    {                                                                       \
                        v_result = FALSE;                                                   \
                    }                                                                       \
                }                                                                           \
            }                                                                               \
         }

#define  SlapTlvGetTupleByType(p, type, tuple)                                              \
         {                                                                                  \
            PSLAP_TLV_TUPLE         cur_tuple = (PSLAP_TLV_TUPLE)p;                         \
                                                                                            \
            while ( (SlapTlvGetType(cur_tuple) != SLAP_TLV_TYPE_END_OF_DATA) &&             \
                    (SlapTlvGetType(cur_tuple) != SLAP_TLV_TYPE_PAD        ) )              \
            {                                                                               \
                if ( SlapTlvGetType(cur_tuple) == type )                                    \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    cur_tuple = SlapTlvGetNextTuple(cur_tuple);                             \
                }                                                                           \
            }                                                                               \
                                                                                            \
            if ( SlapTlvGetType(cur_tuple) == type )                                        \
            {                                                                               \
                tuple = cur_tuple;                                                          \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                tuple = NULL;                                                               \
            }                                                                               \
         }

#define  SlapTlvGetTupleCount(p, type, counter)                                             \
         {                                                                                  \
            PSLAP_TLV_TUPLE         cur_tuple = (PSLAP_TLV_TUPLE)p;                         \
                                                                                            \
            counter = 0;                                                                    \
                                                                                            \
            while ( SlapTlvGetType(cur_tuple) != SLAP_TLV_TYPE_END )                        \
            {                                                                               \
                if ( SlapTlvGetType(cur_tuple) == type )                                    \
                {                                                                           \
                    counter++;                                                              \
                }                                                                           \
                                                                                            \
                cur_tuple = SlapTlvGetNextTuple(cur_tuple);                                 \
            }                                                                               \
         }

/*
 * At most times the CTLV tuples will be used to store some popular data types: integers with
 * variable length, ASCII string, binary string, and so forth. We define some helper macros here
 * to facilitate the access to such tuples.
 */
#define  SlapTlvGetValueBool(p)                     ( (BOOL )p->Value[0]                           )
#define  SlapTlvGetValueUchar(p)                    ( (UCHAR)p->Value[0]                           )
#define  SlapTlvGetValueUlong(p)                    ( AnscUlongFromNToH (AnscReadUlong (p->Value)) )
#define  SlapTlvGetValueCstr(p)                     ( (char* )p->Value                             )
#define  SlapTlvGetValueOstr(p)                     ( (PUCHAR)p->Value                             )

#define  SlapTlvSetValueBool(p, v)                  ( p->Value[0] = (char)v                        )
#define  SlapTlvSetValueUchar(p, v)                 ( p->Value[0] = (char)v                        )
#define  SlapTlvSetValueUlong(p, v)                 AnscWriteUlong (p->Value, AnscUlongFromHToN (v))
#define  SlapTlvSetValueCstr(p, v)                  SlapTlvSetValue(p, v)
#define  SlapTlvSetValueOstr(p, v)                  SlapTlvSetValue(p, v)


#endif

