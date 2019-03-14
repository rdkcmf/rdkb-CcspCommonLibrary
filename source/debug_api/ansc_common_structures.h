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

    module:	ansc_common structures.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        common data structures.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/15/01    initial revision.
        01/19/01    restructure the container object, add and
                    remove a few things.
        04/01/01    add user mode wrapper functions.
        06/18/02    separated from ansc_wrapper_base.h.

**********************************************************************/


#ifndef  _ANSC_COMMON_STRUCTURES_
#define  _ANSC_COMMON_STRUCTURES_


/***********************************************************
         DEFINITION OF ENUMERATION DATA STRUCTURES
***********************************************************/

/*
 * Originally I thought to encapsulate each enumeration data structure inside an object, but it turned
 * out to be too complicated and confused to handle and explain. If we need switch to object-based
 * enumeration implementation, the definitions in ansc_enumeration.h and implementations in
 * ansc_enumeration.c can be used.
 */
typedef  struct
_SINGLE_LINK_ENTRY
{
    struct  _SINGLE_LINK_ENTRY*     Next;
}
SINGLE_LINK_ENTRY,  *PSINGLE_LINK_ENTRY;

typedef  struct
_DOUBLE_LINK_ENTRY
{
    struct  _DOUBLE_LINK_ENTRY*     Next;
    struct  _DOUBLE_LINK_ENTRY*     Last;
}
DOUBLE_LINK_ENTRY,  *PDOUBLE_LINK_ENTRY;

typedef  struct
_SLIST_HEADER
{
    SINGLE_LINK_ENTRY               Next;
    USHORT                          Depth;
    USHORT                          Sequence;
}
SLIST_HEADER,  *PSLIST_HEADER;

typedef  struct
_DLIST_HEADER
{
    DOUBLE_LINK_ENTRY               Heads;
    USHORT                          Depth;
    USHORT                          Sequence;
}
DLIST_HEADER,  *PDLIST_HEADER;

/*
 * special single linked list, the difference is that this one keeps track of the last element; we use this list as the
 * FIFO queue
 */
typedef  struct
_QUEUE_HEADER
{
    SINGLE_LINK_ENTRY               Next;
    USHORT                          Depth;
    SINGLE_LINK_ENTRY               Last;
}
QUEUE_HEADER,  *PQUEUE_HEADER;

#define  AnscInitializeQueue                        AnscQueueInitializeHeader

#define  AnscQueueInitializeHeader(QueueHead)                                               \
         {                                                                                  \
            (QueueHead)->Next.Next = NULL;                                                  \
            (QueueHead)->Depth     = 0;                                                     \
            (QueueHead)->Last.Next = NULL;                                                  \
         }

#define  AnscQueuePushEntry(QueueHead, Entry)                                               \
         {                                                                                  \
            PSINGLE_LINK_ENTRY      LastEntry;                                              \
                                                                                            \
            LastEntry     = (QueueHead)->Last.Next;                                         \
            (Entry)->Next = NULL;                                                           \
                                                                                            \
            if ( LastEntry == NULL )                                                        \
            {                                                                               \
                (QueueHead)->Next.Next = (Entry);                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                LastEntry->Next = (Entry);                                                  \
            }                                                                               \
                                                                                            \
            (QueueHead)->Last.Next = (Entry);                                               \
            (QueueHead)->Depth++;                                                           \
         }

#define  AnscQueuePushEntryAtFront(QueueHead, Entry)                                        \
         {                                                                                  \
            PSINGLE_LINK_ENTRY      LastEntry;                                              \
                                                                                            \
            (Entry)->Next          = (QueueHead)->Next.Next;                                \
            (QueueHead)->Next.Next = (Entry);                                               \
            LastEntry              = (QueueHead)->Last.Next;                                \
                                                                                            \
            if ( LastEntry == NULL )                                                        \
            {                                                                               \
                (QueueHead)->Last.Next = (Entry);                                           \
            }                                                                               \
                                                                                            \
            (QueueHead)->Depth++;                                                           \
         }

#define  AnscQueueGetFirstEntry(QueueHead)                      (QueueHead)->Next.Next
#define  AnscQueueGetLastEntry(QueueHead)                       (QueueHead)->Last.Next
#define  AnscQueueGetNextEntry(Entry)                           (Entry)->Next
#define  AnscQueueQueryDepth(QueueHead)                         (QueueHead)->Depth
#define  AnscQueueGetEntryByIndex                               AnscQueueSearchEntryByIndex

/*
 * this following MACROs and inline functions are pulled down from AH/ESP/COMP engine
 */
#define  AnscInitializeSList                        AnscSListInitializeHeader

#define  AnscSListInitializeHeader(ListHead)                                                \
         {                                                                                  \
            (ListHead)->Next.Next = NULL;                                                   \
            (ListHead)->Depth     = 0;                                                      \
            (ListHead)->Sequence  = 0;                                                      \
         }

#define  AnscSListPushEntry(ListHead, Entry)                                                \
         {                                                                                  \
            (Entry)->Next         = (ListHead)->Next.Next;                                  \
            (ListHead)->Next.Next = (Entry);                                                \
            (ListHead)->Depth++;                                                            \
         }

#define  AnscSListPushEntryAtBack(ListHead, Entry)                                          \
         {                                                                                  \
             if ( !AnscSListGetFirstEntry(ListHead) )                                       \
             {                                                                              \
                AnscSListPushEntry(ListHead, Entry);                                        \
             }                                                                              \
             else                                                                           \
             {                                                                              \
                 (Entry)->Next = NULL;                                                      \
                 AnscSListGetLastEntry(ListHead)->Next = Entry;                             \
                 (ListHead)->Depth++;                                                       \
             }                                                                              \
         }

#define  AnscSListGetFirstEntry(ListHead)                       (ListHead)->Next.Next
#define  AnscSListGetNextEntry(Entry)                           (Entry)->Next
#define  AnscSListGetEntryByIndex                               AnscSListSearchEntryByIndex
#define  AnscSListQueryDepth(ListHead)                          (ListHead)->Depth

/*
 * this following MACROs and inline functions are operations on a double linked list
 */
#define  AnscDListInitializeHeader(ListHead)                                                \
         {                                                                                  \
            (ListHead)->Heads.Next = NULL;                                                  \
            (ListHead)->Heads.Last = NULL;                                                  \
            (ListHead)->Depth      = 0;                                                     \
            (ListHead)->Sequence  = 0;                                                      \
         }

#define  AnscDListGetHeadEntryLeft(ListHead)                    (ListHead)->Heads.Next
#define  AnscDListGetHeadEntryRight(ListHead)                   (ListHead)->Heads.Last
#define  AnscDListGetEntryLeft(Entry)                           (Entry)->Last
#define  AnscDListGetEntryRight(Entry)                          (Entry)->Next
#define  AnscDListQueryDepth(ListHead)                          (ListHead)->Depth

/*
 * trace back to the head of the structure by knowing the address of a field contained in the structure
 */
#define  ACCESS_CONTAINER(address, type, field)     \
         ((type*)((PCHAR)(address) - (ULONG)(&((type*)0)->field)))
#define  ACCESS_FIELD_OFFSET(type, field)           \
         (ULONG)(&((type*)0)->field)


/***********************************************************
         DEFINITION OF SOME UTILITY DATA STRUCTURES
***********************************************************/

/*
 * Just like URI string, we sometime need to chain mutiple tokens together into a single string.
 * Each token in the compound string is separated by a pre-defined symbol, which is usually the
 * '/' or '\'. To facilitate the searching and sorting of such strings, we define following data
 * structure internally.
 */
typedef  struct
_ANSC_STRING_TOKEN
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hChain;
    char                            Name[ANSC_URI_PATH_SIZE];
}
ANSC_STRING_TOKEN,  *PANSC_STRING_TOKEN;

#define  ACCESS_ANSC_STRING_TOKEN(p)                \
         ACCESS_CONTAINER(p, ANSC_STRING_TOKEN, Linkage)

typedef  struct
_ANSC_TOKEN_CHAIN
{
    QUEUE_HEADER                    TokensQueue;
}
ANSC_TOKEN_CHAIN,  *PANSC_TOKEN_CHAIN;

#define  AnscTcGetTokenCount(p)                     AnscQueueQueryDepth      (&p->TokensQueue)
#define  AnscTcPrefixToken(p, t)                    AnscQueuePushEntryAtFront(&p->TokensQueue, &t->Linkage)
#define  AnscTcSuffixToken(p, t)                    AnscQueuePushEntry       (&p->TokensQueue, &t->Linkage)
#define  AnscTcUnlinkToken(p)                       (PANSC_STRING_TOKEN)AnscTcPopToken((ANSC_HANDLE)p)


/***********************************************************
           DEFINITION OF COMMON TREE-LIKE STRUCTURE
***********************************************************/

/*
 * Our standard approach of implementing a tree-like data structure is to designate two different
 * objects, knowingly the "node" and "leaf" object, to encapsulate the corresponding properties and
 * behavior. While such practice is very powerful and flexible, it introduces somewhat significant
 * performance and memory overhead. We need another more efficient tree-manipulation mechanism.
 */
#define  ANSC_STREE_VALUE_TYPE_UINT                 1
#define  ANSC_STREE_VALUE_TYPE_ASTR                 2
#define  ANSC_STREE_VALUE_TYPE_BOOL                 3

typedef  ANSC_STATUS
(*PFN_STREE_FREE)
    (
        ANSC_HANDLE                 hOwnerObject,
        ANSC_HANDLE                 hUserContext
    );

typedef  ANSC_STATUS
(*PFN_STREE_ENUM)
    (
        ANSC_HANDLE                 hOwnerObject,
        ANSC_HANDLE                 hUserContext,
        ANSC_HANDLE                 hEnumContext,
        ANSC_HANDLE                 hNodeContext
    );

typedef  struct
_STREE_LINK_ENTRY
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hRootNode;
    ANSC_HANDLE                     hParentNode;
    ULONG                           HashIndex;
    ULONG                           NodeTableSize;
    QUEUE_HEADER*                   NodeTable;

    ULONG                           ThisValueType;
    ULONG                           ThisValueSize;
    ULONG                           NextValueType;
    ULONG                           NextValueSize;

    union
    {
        ULONG                       UintParam;
        char*                       AstrParam;
        BOOL                        BoolParam;
    }Value;

    ANSC_HANDLE                     hOwnerObject;
    ANSC_HANDLE                     hUserContext;
    PFN_STREE_FREE                  pfnFree;
    PFN_STREE_ENUM                  pfnEnum;
}
STREE_LINK_ENTRY,  *PSTREE_LINK_ENTRY;

#define  ACCESS_STREE_LINK_ENTRY(p)                  \
         ACCESS_CONTAINER(p, STREE_LINK_ENTRY, Linkage)

/*
 * The "root node" should be no different from the other nodes in terms of searching, matching and
 * traversing. However, it ought to maintain some global information about the internal property
 * and organization.
 */
typedef  struct
_STREE_ROOT_NODE
{
    STREE_LINK_ENTRY                TopNode;
}
STREE_ROOT_NODE,  *PSTREE_ROOT_NODE;

#define  AnscStreeInitialize(root, table_size)                                              \
         {                                                                                  \
            ULONG                   index     = 0;                                          \
            ULONG                   tbuf_size = table_size * sizeof(QUEUE_HEADER);          \
                                                                                            \
            AnscZeroMemory((root), sizeof(STREE_ROOT_NODE));                                \
                                                                                            \
            (root)->TopNode.Linkage.Next  = NULL;                                           \
            (root)->TopNode.hRootNode     = (ANSC_HANDLE)(root);                            \
            (root)->TopNode.hParentNode   = (ANSC_HANDLE)NULL;                              \
            (root)->TopNode.HashIndex     = 0;                                              \
            (root)->TopNode.NodeTableSize = table_size;                                     \
            (root)->TopNode.NodeTable     = (QUEUE_HEADER*)AnscAllocateMemory(tbuf_size);   \
                                                                                            \
            (root)->TopNode.ThisValueType = ANSC_STREE_VALUE_TYPE_UINT;                     \
            (root)->TopNode.ThisValueSize = sizeof(ULONG);                                  \
                                                                                            \
            (root)->TopNode.hOwnerObject  = (ANSC_HANDLE)NULL;                              \
            (root)->TopNode.hUserContext  = (ANSC_HANDLE)NULL;                              \
            (root)->TopNode.pfnFree       = NULL;                                           \
            (root)->TopNode.pfnEnum       = NULL;                                           \
                                                                                            \
            for ( index = 0; index < table_size; index++ )                                  \
            {                                                                               \
                AnscQueueInitializeHeader(&(root)->TopNode.NodeTable[index]);               \
            }                                                                               \
         }

#define  AnscStreeUnload(root)                                                              \
         {                                                                                  \
            AnscStreeCleanUpNode(&(root)->TopNode, TRUE, TRUE);                             \
         }

/*
 * To create a node in the "simple tree" data structure, we need to know the value type and size
 * associated with the node to be created along with a recommended size for the hashed table which
 * is used as placeholder for next-level nodes.
 */
#define  ANSC_STREE_MAX_STRING_SIZE                 32

typedef  struct
_STREE_NODE_INFO
{
    ULONG                           NodeTableSize;
    ULONG                           ValueType;
    ULONG                           ValueSize;

    union
    {
        ULONG                       UintParam;
        char*                       AstrParam;  /* must be set to point to "scratch pad" field */
        BOOL                        BoolParam;
    }Value;
    /*char                            ScratchPad[ANSC_STREE_MAX_STRING_SIZE];*/

    ANSC_HANDLE                     hOwnerObject;
    ANSC_HANDLE                     hUserContext;
    PFN_STREE_FREE                  pfnFree;
    PFN_STREE_ENUM                  pfnEnum;
}
STREE_NODE_INFO,  *PSTREE_NODE_INFO;

typedef  struct
_STREE_NODE_PATH
{
    ULONG                           NodeCount;
    STREE_NODE_INFO                 NodeArray[ANSC_ZERO_ARRAY_SIZE];
}
STREE_NODE_PATH,  *PSTREE_NODE_PATH;

/*
 * To match a particular node in a "simple tree", we need to identify every node along the path
 * from the root node to the destination node by exactly matching the value at each node (hence the
 * name - simple tree).
 */
typedef  struct
_STREE_TLV_TUPLE
{
    ULONG                           ValueType;
    ULONG                           ValueSize;

    union
    {
        ULONG                       UintParam;
        char*                       AstrParam;  /* must be set to point to "scratch pad" field */
        BOOL                        BoolParam;
    }Value;
    char                            ScratchPad[ANSC_STREE_MAX_STRING_SIZE];
}
STREE_TLV_TUPLE,  *PSTREE_TLV_TUPLE;

typedef  struct
_STREE_SEARCH_STRING
{
    ULONG                           TupleCount;
    STREE_TLV_TUPLE                 TupleArray[ANSC_ZERO_ARRAY_SIZE];
}
STREE_SEARCH_STRING,  *PSTREE_SEARCH_STRING;

#define  AnscStreeMakeTlvTupleByInfo(tlv_tuple, node_info)                                  \
         {                                                                                  \
            tlv_tuple->ValueType = node_info->ValueType;                                    \
            tlv_tuple->ValueSize = node_info->ValueSize;                                    \
                                                                                            \
            switch ( node_info->ValueType )                                                 \
            {                                                                               \
                case    ANSC_STREE_VALUE_TYPE_UINT :                                        \
                                                                                            \
                        tlv_tuple->Value.UintParam = node_info->Value.UintParam;            \
                                                                                            \
                        break;                                                              \
                                                                                            \
                case    ANSC_STREE_VALUE_TYPE_ASTR :                                        \
                                                                                            \
                        tlv_tuple->Value.AstrParam = node_info->Value.AstrParam;            \
                                                                                            \
                        break;                                                              \
                                                                                            \
                case    ANSC_STREE_VALUE_TYPE_BOOL :                                        \
                                                                                            \
                        tlv_tuple->Value.BoolParam = node_info->Value.BoolParam;            \
                                                                                            \
                        break;                                                              \
                                                                                            \
                default :                                                                   \
                                                                                            \
                        break;                                                              \
            }                                                                               \
         }


#endif
