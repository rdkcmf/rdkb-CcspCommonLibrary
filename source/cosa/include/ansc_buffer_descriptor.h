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

    module:	ansc_buffer_descriptor.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Buffer Descriptor Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/25/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_BUFFER_DESCRIPTOR_
#define  _ANSC_BUFFER_DESCRIPTOR_


/***********************************************************
     ANSC BUFFER DESCRIPTOR COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * The Internet Protocol is designed for use in interconnected systems of packet-switched computer
 * communication networks. Such a system has been called a "catenet". The internet protocol
 * provides for transmitting blocks of data called datagrams from sources to destinations, where
 * sources and destinations are hosts identified by fixed length addresses. The internet protocol
 * also provides for fragmentation and reassembly of long datagrams, if necessary, for transmission
 * through "small packet" networks.
 */
#define  ANSC_BDO_OS_SPECIFIC_SIZE                  16

typedef  ANSC_STATUS
(*PFN_BDO_FREE_ME)
    (
        ANSC_HANDLE                 hOwnerObject,
        ANSC_HANDLE                 hBdo
    );

#ifdef _ANSC_TRACE_PACKET_
typedef struct
_ANSC_BUFFER_REFCOUNT_TRACE
{
    DOUBLE_LINK_ENTRY               Link;
    char                            RefFile[32];
    int                             RefLine;
    int                             RefCountInc;
    int                             RefCount;
}
ANSC_BUFFER_REFCOUNT_TRACE, *PANSC_BUFFER_REFCOUNT_TRACE;
#endif

typedef  struct
_ANSC_BUFFER_DESCRIPTOR
{
    SINGLE_LINK_ENTRY               Linkage;
    BOOL                            bOwner;
    BOOL                            bUseMmap;
    PVOID                           Buffer;
    ULONG                           BufferSize;
    ULONG                           Offset;
    ULONG                           BlockSize;
    ULONG                           RefCount;
    ULONG                           Reserved1;
    UCHAR                           OsSpecific[ANSC_BDO_OS_SPECIFIC_SIZE];

    ANSC_HANDLE                     hOwnerObject;
    PFN_BDO_FREE_ME                 pfnFreeMe;

#ifdef _ANSC_TRACE_PACKET_
    char                            AllocFile[32];
    int                             AllocLine;
    DOUBLE_LINK_ENTRY               AllocLink;
    DLIST_HEADER                    RefHead;
    const char *                    TmpFile;
    int                             TmpLine;
#endif /* _ANSC_TRACE_PACKET_ */

}
ANSC_BUFFER_DESCRIPTOR,  *PANSC_BUFFER_DESCRIPTOR;

#define  ACCESS_ANSC_BUFFER_DESCRIPTOR(p)           \
         ACCESS_CONTAINER(p, ANSC_BUFFER_DESCRIPTOR, Linkage)

/*
 * It's probably not a very good idea to encapsulate the Packet Descriptor as an ansc-style object
 * since the object-creation and invocation of member functions will certainly slow things down. So
 * instead, we use following macros to do the job.
 */
#define  AnscBdoGetOwner(p)                         ( ((PANSC_BUFFER_DESCRIPTOR)p)->bOwner       )
#define  AnscBdoGetBuffer(p)                        ( ((PANSC_BUFFER_DESCRIPTOR)p)->Buffer       )
#define  AnscBdoGetBufferSize(p)                    ( ((PANSC_BUFFER_DESCRIPTOR)p)->BufferSize   )
#define  AnscBdoGetOffset(p)                        ( ((PANSC_BUFFER_DESCRIPTOR)p)->Offset       )
#define  AnscBdoGetBlockSize(p)                     ( ((PANSC_BUFFER_DESCRIPTOR)p)->BlockSize    )
#define  AnscBdoGetRefCount(p)                      ( ((PANSC_BUFFER_DESCRIPTOR)p)->RefCount     )
#define  AnscBdoGetReserved1(p)                     ( ((PANSC_BUFFER_DESCRIPTOR)p)->Reserved1    )
#define  AnscBdoGetOwnerObject(p)                   ( ((PANSC_BUFFER_DESCRIPTOR)p)->hOwnerObject )
#define  AnscBdoGetFreeFunc(p)                      ( ((PANSC_BUFFER_DESCRIPTOR)p)->pfnFreeMe    )

#define  AnscBdoSetOwner(p, r)                      ( ((PANSC_BUFFER_DESCRIPTOR)p)->bOwner       = r )
#define  AnscBdoSetBuffer(p, b)                     ( ((PANSC_BUFFER_DESCRIPTOR)p)->Buffer       = b )
#define  AnscBdoSetBufferSize(p, s)                 ( ((PANSC_BUFFER_DESCRIPTOR)p)->BufferSize   = s )
#define  AnscBdoSetOffset(p, o)                     ( ((PANSC_BUFFER_DESCRIPTOR)p)->Offset       = o )
#define  AnscBdoSetBlockSize(p, s)                  ( ((PANSC_BUFFER_DESCRIPTOR)p)->BlockSize    = s )
#define  AnscBdoSetRefCount(p, c)                   ( ((PANSC_BUFFER_DESCRIPTOR)p)->RefCount     = c )
#define  AnscBdoSetReserved1(p, r)                  ( ((PANSC_BUFFER_DESCRIPTOR)p)->Reserved1    = r )
#define  AnscBdoSetOwnerObject(p, o)                ( ((PANSC_BUFFER_DESCRIPTOR)p)->hOwnerObject = o )
#define  AnscBdoSetFreeFunc(p, f)                   ( ((PANSC_BUFFER_DESCRIPTOR)p)->pfnFreeMe    = f )

#define  AnscBdoGetBlock(p)                         ( (PVOID)((ULONG)p->Buffer + p->Offset) )
#define  AnscBdoGetEndOfBlock(p)                    ( (PVOID)((ULONG)p->Buffer + p->Offset + p->BlockSize) )
#define  AnscBdoGetUsableSize(p)                    ( p->BufferSize - p->Offset )
#define  AnscBdoGetLeftSize(p)                      ( p->BufferSize - p->Offset - p->BlockSize )

#define  AnscBdoAttachBlock(p, r, b, s1, o, s2)                                             \
         {                                                                                  \
            PANSC_BUFFER_DESCRIPTOR          pBdo = p;                                      \
                                                                                            \
            if ( pBdo->bOwner && pBdo->Buffer )                                             \
            {                                                                               \
                AnscFreeMemory(pBdo->Buffer);                                               \
                                                                                            \
                pBdo->Buffer = NULL;                                                        \
            }                                                                               \
                                                                                            \
            pBdo->bOwner     = r;                                                           \
            pBdo->Buffer     = b;                                                           \
            pBdo->BufferSize = s1;                                                          \
            pBdo->Offset     = o;                                                           \
            pBdo->BlockSize  = s2;                                                          \
         }

#define  AnscBdoExpandLeft(p, size)                                                         \
         {                                                                                  \
            p->Offset    -= size;                                                           \
            p->BlockSize += size;                                                           \
         }

#define  AnscBdoExpandRight(p, size)                                                        \
         {                                                                                  \
            p->BlockSize += size;                                                           \
         }

#define  AnscBdoShrinkLeft(p, size)                                                         \
         {                                                                                  \
            p->BlockSize -= size;                                                           \
         }

#define  AnscBdoShrinkRight(p, size)                                                        \
         {                                                                                  \
            p->Offset    += size;                                                           \
            p->BlockSize -= size;                                                           \
         }

#define  AnscBdoIncRefCount(p, n)                                                           \
         {                                                                                  \
            p->RefCount += n;                                                               \
         }

#define  AnscBdoDecRefCount(p, n)                                                           \
         {                                                                                  \
            p->RefCount -= n;                                                               \
         }

#define  AnscBdoAppendChar(p, c)                                                            \
         {                                                                                  \
            char*                   end_block = (char*)AnscBdoGetBlock(p);                  \
                                                                                            \
            end_block += p->BlockSize;                                                      \
            *end_block = c;                                                                 \
            p->BlockSize++;                                                                 \
         }

#define  AnscBdoAppendData(p, data, size)                                                   \
         {                                                                                  \
            char*                   end_block = (char*)AnscBdoGetBlock(p);                  \
                                                                                            \
            end_block    += p->BlockSize;                                                   \
            p->BlockSize += size;                                                           \
                                                                                            \
            AnscCopyMemory(end_block, data, size);                                          \
         }


#endif
