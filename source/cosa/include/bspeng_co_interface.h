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

    module:	bspeng_co_interface.h

        For BSP Engine Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        string implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        05/14/03    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/

#ifndef  _BSPENG_INTERFACE_DEC
#define  _BSPENG_INTERFACE_DEC

#include "slap_definitions.h"

/* 
 * Since some embedded systems don't support float/double operations, we
 * need to support both.
 */
#define  _BSPENG_NO_DOUBLE

/*
#define  BSP_EXEC_CONTEXT_OBJECT_NAME               "ExecContext"
#define  BSP_EXEC_CONTEXT_OBJECT_VAR_ID             ((ULONG)-1)
*/

#define  BSP_METHOD_SET_PROPERTY                    "SetProperty"
#define  BSP_METHOD_GET_PROPERTY                    "GetProperty"
#define  BSP_METHOD_SET_ITEM                        "SetItem"
#define  BSP_METHOD_GET_ITEM                        "GetItem"
#define  BSP_METHOD_EXECUTE                         "Execute"

/* reserved property of variable */
#define  BSP_VAR_CONTENT_TYPE                       "ContentType"
#define  SLAP_UTILITY_VAR_ENTITY                    "System.Utility.VarEntity"

#define  BSP_VAR_SLAP_SYNTAX_TYPE                   "SlapSyntax"

#define  BSP_ARRAY_VAR_TYPE                         "type"
#define  BSP_ARRAY_VAR_LENGTH                       "length"

#define  BSPOBJ_NAME_MAX_LEVEL                      16

#define  BSPENG_TOKENIZER_BA_GROWTH                 32768

typedef  ANSC_STATUS
(*PFN_BSPENG_OBJ_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSPENG_OBJ_INIT)
    (
        ANSC_HANDLE                 hThisObject
    );

#define  _BSP_TEMPLATE_ARCHIVE_FILE_EXT             "acv"


#ifdef   _BSPENG_NO_DOUBLE
/*
 * define the simulation of "float/double".
 */

#define  BSP_TEMPLATE_DOUBLE_SCALE                  1000            /* 3-digit fraction */
#define  BSP_TEMPLATE_DOUBLE_FORMAT                 "%d.%.3d"       /* format string */

typedef  LONG                                       BSP_TEMPLATE_DOUBLE, *PBSP_TEMPLATE_DOUBLE;


#define  BSP_TEMPATE_DOUBLE_SET(TEDouble, lInteger)                                     \
    TEDouble  = lInteger;                                                               \

#define  BSP_TEMPLATE_DOUBLE_GET_INT_TRUNK(TEDouble)                                    \
    (TEDouble)/BSP_TEMPLATE_DOUBLE_SCALE                                                \

#define  BSP_TEMPLATE_DOUBLE_GET_INT(TEDouble)                                          \
    (TEDouble)/BSP_TEMPLATE_DOUBLE_SCALE + (BSP_TEMPLATE_DOUBLE_HAS_CARRY(TEDouble)?1:0)\

#define  BSP_TEMPLATE_DOUBLE_GET_FRAC(TEDouble)                                         \
    (TEDouble - TEDouble/BSP_TEMPLATE_DOUBLE_SCALE * BSP_TEMPLATE_DOUBLE_SCALE)         \

#define  BSP_TEMPLATE_DOUBLE_HAS_CARRY(TEDouble)                                        \
    (BSP_TEMPLATE_DOUBLE_GET_FRAC(TEDouble) * 10 / BSP_TEMPLATE_DOUBLE_SCALE >= 5)      \

#define  BSP_TEMPLATE_DOUBLE_TO_STRING(pBuf, TEDouble)                                  \
        _ansc_sprintf                                                                   \
            (                                                                           \
                pBuf,                                                                   \
                BSP_TEMPLATE_DOUBLE_FORMAT,                                             \
                BSP_TEMPLATE_DOUBLE_GET_INT_TRUNK(TEDouble),                            \
                BSP_TEMPLATE_DOUBLE_GET_FRAC(TEDouble)                                  \
            );                                                                          \

#define  STRING_TO_BSP_TEMPLATE_DOUBLE(pBuf, TEDouble, pEnd, pHasFrac)                  \
    {                                                                                   \
        char                        *pPos   = NULL;                                     \
        char                        *pFrac  = NULL;                                     \
        char                        *pInt   = NULL;                                     \
        char                        ch;                                                 \
        BOOL                        bInt    = TRUE;                                     \
        ULONG                       ulZeros = 0;                                        \
                                                                                        \
        if (pHasFrac)                                                                   \
            *pHasFrac   = FALSE;                                                        \
                                                                                        \
        pPos    = (char *)pBuf;                                                                 \
        if (pEnd)                                                                       \
            *pEnd   = pPos;                                                             \
                                                                                        \
        while (pPos && *pPos != '\0')                                                   \
        {                                                                               \
            ch  = *pPos;                                                                \
                                                                                        \
            if (ch >= '0' && ch <= '9')                                                 \
            {                                                                           \
                if (bInt && !pInt)                                                      \
                    pInt    = pPos;                                                     \
                                                                                        \
                if (!bInt)                                                              \
                {                                                                       \
                    if (!pFrac)                                                         \
                        pFrac   = pPos;                                                 \
                    if (ch == '0')                                                      \
                        ulZeros ++;                                                     \
                }                                                                       \
                                                                                        \
                pPos ++;                                                                \
                continue;                                                               \
            }                                                                           \
            else                                                                        \
                if (ch == '.')                                                          \
                {                                                                       \
                    if (bInt)                                                           \
                    {                                                                   \
                        bInt    = FALSE;                                                \
                        pPos ++;                                                        \
                                                                                        \
                        if (pHasFrac)                                                   \
                            *pHasFrac   = TRUE;                                         \
                                                                                        \
                        continue;                                                       \
                    }                                                                   \
                }                                                                       \
                                                                                        \
            if (pEnd)                                                                   \
                *pEnd   = pPos;                                                         \
            break;                                                                      \
        }                                                                               \
                                                                                        \
        if (pInt || pFrac)                                                              \
        {                                                                               \
            ULONG                   i;                                                  \
            LONG                    lInteger    = 0;                                    \
            ULONG                   ulFraction  = 0;                                    \
                                                                                        \
            if (pInt)                                                                   \
                lInteger    = _ansc_atol(pInt);                                         \
                                                                                        \
            if (pFrac)                                                                  \
                ulFraction  = (ULONG)_ansc_atol(pFrac);                                 \
                                                                                        \
            *TEDouble    = BSP_TEMPLATE_DOUBLE_SCALE * lInteger;                        \
            ulFraction   = (ulFraction * BSP_TEMPLATE_DOUBLE_SCALE / 10);               \
            for (i = 0; i < ulZeros; i ++)                                              \
                ulFraction  /= 10;                                                      \
                                                                                        \
            while (ulFraction >= BSP_TEMPLATE_DOUBLE_SCALE)                             \
                ulFraction /= 10;                                                       \
                                                                                        \
            *TEDouble   += ulFraction;                                                  \
        }                                                                               \
        else                                                                            \
            *TEDouble    = 0;                                                           \
    }                                                                                   \

#endif

/* -----------------------------------------------------------------------------
 *              Some definitions
 * -------------------------------------------------------------------------- */


#define  BSP_TEMPLATE_LOOP_LIMIT                    30000
#define  BSP_TEMPLATE_RECURSIVE_LIMIT               1000
#define  BSP_TEMPLATE_WHILE_NEST_LIMIT              10
#define  BSP_TEMPLATE_DEFAULT_NAME                  "unknown"
#define  BSP_TEMPLATE_ARRAY_DIMENSION_LIMIT         3
#define  BSP_TEMPLATE_API_PARAM_LIMIT               16
#define  BSP_TEMPLATE_TEXT_INIT_SIZE                32 * 1024
#define  BSP_TEMPLATE_KEYWORD_CASE_SENSITIVE        TRUE
#define  BSP_TEMPLATE_STACK_GROWTH                  8
#define  BSP_TEMPLATE_TEMP_SPACE                    16
#define  BSP_TEMPLATE_PARSING_DEFAULT_LINE_NO       1


typedef enum _BSP_TEMPLATE_OPERATOR 
{
/*    0                                 2                                                       */
    BspOp_None, BspOp_TextBlock, BspOp_Include,
/*   3                                      6                                                   */
    BspOp_If, BspOp_ElseIf, BspOp_Else, BspOp_Endif,
/*    7                                                                         13              */
    BspOp_While, BspOp_EndWhile, BspOp_Iter, BspOp_Break, BspOp_Continue, BspOp_Return, BspOp_Exit,
/*     14            15                                                                         */
    BspOp_SetOutput, BspOp_EndSetOutput,
/*   16                                                                          22             */
    BspOp_Call, BspOp_Sys, BspOp_Env, BspOp_Obj, BspOp_Param, BspOp_EndProc, BspOp_SubStr, 
/*   23                                             26                                          */
    BspOp_Execute, BspOp_Dim, BspOp_ArrayItem, BspOp_Api, 
/*       27                                         30                                          */
    BspOp_Switch, BspOp_EndSwitch, BspOp_Case, BspOp_Default,
/*       31                                                                                     */
    BspOp_Name,
/*     32                                                                                               39          */
    BspOp_Assign, BspOp_Incr, BspOp_Decr, BspOp_IncrN, BspOp_DecrN, BspOp_MultiplyN, BspOp_DivideN, BspOp_ModuloN,
/*    40                                                                44                      */    
    BspOp_BitOrN, BspOp_BitAndN, BspOp_ShiftLeftN, BspOp_ShiftRightN, BspOp_BitXorN, 
/*    45            46                                                                          */
    BspOp_ShiftLeft, BspOp_ShiftRight,
/*   47                                                 52                                      */
    BspOp_LE, BspOp_GE, BspOp_NE, BspOp_LT, BspOp_GT, BspOp_Eq,
/*   53                                                                         59              */
    BspOp_Or, BspOp_And, BspOp_Not, BspOp_BitOr, BspOp_BitAnd, BspOp_BitXor, BspOp_BitNot,  
/*   60                                                                         65              */
    BspOp_Add, BspOp_Subtract, BspOp_Multiply, BspOp_Divide, BspOp_Modulo, BspOp_StrCat, 
/*    66                                                                    71                  */
    BspOp_ArrayL, BspOp_ArrayR, BspOp_Comma, BspOp_ParenL, BspOp_ParenR, BspOp_Period,
/*    72                                        75                                              */
    BspOp_End, BspOp_Proc, BspOp_SemiColon, BspOp_Colon, 
/*      76                                                                                      */
    BspOp_Term
}
BSP_TEMPLATE_OPERATOR, *PBSP_TEMPLATE_OPERATOR;


#define  _BSP_TEMPLATE_API_MAX_NAME_LENGTH          32

/* type of APIs */
typedef enum 
_BSP_TEMPLATE_API_CODE 
{
    BspApi_unknown  = -1, 
    BspApi_atoi,          /* must be ordered the same as APINames */
    BspApi_atof, 
    BspApi_itoa, 
    BspApi_ftoa,
    BspApi_strstr,
    BspApi_strcmp,
    BspApi_strcmpi,
    BspApi_strncmp,
    BspApi_strlen,
    BspApi_strgetchar,
    BspApi_strsetchar,
    BspApi_strleft,
    BspApi_strmid,
    BspApi_strright,
    BspApi_sleep
}
BSP_TEMPLATE_API_CODE, *PBSP_TEMPLATE_API_CODE;

/* type of variables */
typedef enum 
_BSP_TEMPLATE_VAR_TYPE 
{ 
    BspVar_Number,          /* 0 */
    BspVar_UNumber,         /* 1 */
    BspVar_Real,            /* 2 */
    BspVar_String,          /* 3 */ 
    BspVar_Object,          /* 4 */
    BspVar_Ref,             /* 5 */
    BspVar_Array,           /* 6 */
}
BSP_TEMPLATE_VAR_TYPE, *PBSP_TEMPLATE_VAR_TYPE; 

typedef enum 
_BSP_TEMPLATE_TOKEN_TYPE 
{ 
    BspToken_eText, 
    BspToken_eString, 
    BspToken_eIdent, 
    BspToken_eNumber, 
    BspToken_eReal, 
    BspToken_eOp 
}
BSP_TEMPLATE_TOKEN_TYPE, *PBSP_TEMPLATE_TOKEN_TYPE;


typedef enum 
_BSP_TEMPLATE_BRANCH_TYPE 
{ 
    BspBranch_eBranch, 
    BspBranch_eEmpty, 
    BspBranch_eString, 
    BspBranch_eNumber, 
    BspBranch_eReal, 
    BspBranch_eVar, 
    BspBranch_eArrayDim, 
    BspBranch_eArrayItem, 
    BspBranch_eApiParams 
}
BSP_TEMPLATE_BRANCH_TYPE, *PBSP_TEMPLATE_BRANCH_TYPE;


typedef struct 
_BSP_TEMPLATE_SYMBOL_PRO
{
    const char                      *pName;
}
BSP_TEMPLATE_SYMBOL_PRO, *PBSP_TEMPLATE_SYMBOL_PRO;


typedef struct 
_BSP_TEMPLATE_ARRAY_DATA
{
    ULONG                           Count;
    void                            **pBranch;
}
BSP_TEMPLATE_ARRAY_DATA, *PBSP_TEMPLATE_ARRAY_DATA;

#if 0   /* REMOVED ! */
typedef struct 
_BSP_TEMPLATE_VAR_ARRAY_REF
{
    LONG                            n;
    ANSC_HANDLE                     hTmpl;
}
BSP_TEMPLATE_VAR_ARRAY_REF, *PBSP_TEMPLATE_VAR_ARRAY_REF;
#endif

typedef  enum
_BSP_TEMPLATE_ARRAY_TYPE
{
    BspArray_Variant    = 0,
    BspArray_Uint       = 1,
    BspArray_Int        = 2,
    BspArray_String     = 3,
    BspArray_Uchar      = 4,
    BspArray_Char       = 5,
    BspArray_Bool       = 6,
    BspArray_Ptr        = 7,
    BspArray_Handle     = 8,
    BspArray_Object     = 9
}
BSP_TEMPLATE_ARRAY_TYPE, *PBSP_TEMPLATE_ARRAY_TYPE;

typedef  struct
_BSP_TEMPLATE_ARRAY_ITEMS
{
    BSP_TEMPLATE_ARRAY_TYPE         Type;

    ULONG                           ulSize;         /* maximum number of items  */
    ULONG                           ulLen;          /* current items */

    ULONG                           Dims[BSP_TEMPLATE_ARRAY_DIMENSION_LIMIT];
    ULONG                           numDims;

    BOOL                            bBulkData;      /* TRUE for bulk data returned from SLAP call */

    ANSC_HANDLE                     hVars;          /* items when bBulkData is FALSE */
    ANSC_HANDLE                     hBulkData;      /* bulk data storage if bBulkData is TRUE */
}
BSP_TEMPLATE_ARRAY_ITEMS, *PBSP_TEMPLATE_ARRAY_ITEMS;

typedef union 
_BSP_TEMPLATE_VAR_VALUE
{ 
    char                            *str; 
    
    LONG                            num; 

#ifdef   _BSPENG_NO_DOUBLE
    BSP_TEMPLATE_DOUBLE             real;
#else
    DOUBLE                          real; 
#endif
    
    PVOID                           obj; 
    
    ANSC_HANDLE                     ref;

    BSP_TEMPLATE_ARRAY_ITEMS        arrayItems;
}
BSP_TEMPLATE_VAR_VALUE, *PBSP_TEMPLATE_VAR_VALUE; 


typedef union 
_BSP_TEMPLATE_TOKEN_VALUE
{ 
    ULONG                           str;
    LONG                            num; 
#ifdef   _BSPENG_NO_DOUBLE
    BSP_TEMPLATE_DOUBLE             real;
#else
    DOUBLE                          real; 
#endif
    BSP_TEMPLATE_OPERATOR           op; 
}
BSP_TEMPLATE_TOKEN_VALUE, *PBSP_TEMPLATE_TOKEN_VALUE;


typedef struct 
_BSP_TEMPLATE_TOKEN
{
    ULONG                           LineNo;
    BSP_TEMPLATE_TOKEN_TYPE         Type;
    BSP_TEMPLATE_TOKEN_VALUE        Value;
    ULONG                           TextLen;
    struct _BSP_TEMPLATE_TOKEN      *pNext;
}
BSP_TEMPLATE_TOKEN, *PBSP_TEMPLATE_TOKEN;

typedef struct 
_BSP_TEMPLATE_BRANCH_DATA
{
	BSP_TEMPLATE_BRANCH_TYPE        type;
	union 
    { 
        ANSC_HANDLE                 b; 
        char                        *s; 
        LONG                        n; 
#ifdef   _BSPENG_NO_DOUBLE
        BSP_TEMPLATE_DOUBLE         r;
#else
        DOUBLE                      r; 
#endif
        BSP_TEMPLATE_ARRAY_DATA     a;
    } Value;
} 
BSP_TEMPLATE_BRANCH_DATA, *PBSP_TEMPLATE_BRANCH_DATA;


typedef struct 
_BSP_TEMPLATE_STATE
{ 
    ULONG                           base;
    ULONG                           top; 
}
BSP_TEMPLATE_STATE, *PBSP_TEMPLATE_STATE;

typedef enum 
_BSP_TEMPLATE_OPTION
{ 
    BspOpt_Start      = 1, 
    BspOpt_PassRef    = 1, 
    BspOpt_Param      = 1, 
    BspOpt_Iter       = 1 
}
BSP_TEMPLATE_OPTION, *PBSP_TEMPLATE_OPTION;


typedef
BOOL
(*PFN_BSPENG_OBJECT_STORE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    );

typedef
BOOL
(*PFN_BSPENG_OBJECT_LOAD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    );

/* -----------------------------------------------------------------------------
 *              Object "_TEMPLATE_READER"
 * -------------------------------------------------------------------------- */

/*
 * This object just defines the interface that template users need to
 * derive from.
 */

#define  BSPENG_READER_COMPONENT_NAME               "BspEngReaderComponent"
#define  BSPENG_READER_COMPONENT_OID                ANSC_OBJECT_OID_NULL
#define  BSPENG_READER_COMPONENT_TYPE               ANSC_OBJECT_TYPE_NULL

#define  TEMPLATE_READER_EXT_FILE_PATH_LEN          256

typedef
ULONG
(*PFN_BSPRO_READ)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );

typedef
ANSC_STATUS
(*PFN_BSPRO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_BSPRO_OPEN_EXTERNAL)
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName
    );

typedef
ANSC_STATUS
(*PFN_BSPRO_OPEN_INTERNAL)
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        char                        *pContent,
        ULONG                       ulContentLen
    );

typedef
char *
(*PFN_BSPRO_GET_TEMPLATE_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_BSPRO_SET_EXT_PATH)
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pExtPath
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  BSP_TEMPLATE_READER_CLASS_CONTENT                                              \
    /* duplication of the base object class content */                                  \
    char                            ExtPath[TEMPLATE_READER_EXT_FILE_PATH_LEN];         \
    PFN_BSPRO_READ                  Read;                                               \
    /* close current reader session */                                                  \
    PFN_BSPRO_CLOSE                 Close;                                              \
    /* open an external template required by "inc" statement  */                        \
    PFN_BSPRO_OPEN_EXTERNAL         OpenExternal;                                       \
    /* open an internal template required by "inc" statement  */                        \
    PFN_BSPRO_OPEN_INTERNAL         OpenInternal;                                       \
    /* retrieve current template (file/variable) name */                                \
    PFN_BSPRO_GET_TEMPLATE_NAME     GetTemplateName;                                    \
    /* set external relative path */                                                    \
    PFN_BSPRO_SET_EXT_PATH          SetExtPath;                                         \
    PFN_BSPENG_OBJ_REMOVE           Remove;                                             \
    PFN_BSPENG_OBJ_INIT             Initialize;                                         \

/* content filtering basic message object  */
typedef  struct
_BSP_TEMPLATE_READER_OBJECT
{
    BSP_TEMPLATE_READER_CLASS_CONTENT
}
BSP_TEMPLATE_READER_OBJECT, *PBSP_TEMPLATE_READER_OBJECT;


/* -----------------------------------------------------------------------------
 *              Object "_TEMPLATE_ACCESS"
 * -------------------------------------------------------------------------- */

/*
 * This object just defines the interface that template users need to
 * derive from. This object is defined to communicate with user's module
 * to get data from outside.
 */

#define  BSPENG_ACCESS_COMPONENT_NAME               "BspEngAccessComponent"
#define  BSPENG_ACCESS_COMPONENT_OID                ANSC_OBJECT_OID_NULL
#define  BSPENG_ACCESS_COMPONENT_TYPE               ANSC_OBJECT_TYPE_NULL

typedef
void
(*PFN_BSPAO_OUTPUT)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hArgs,
        ULONG                       ulArgs
    );

typedef
ANSC_HANDLE
(*PFN_BSPAO_MEMBER)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hArgs,
        ULONG                       ulArgs
    );

typedef
ANSC_HANDLE
(*PFN_BSPAO_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hArgs,
        ULONG                       ulArgs
    );


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  BSP_TEMPLATE_ACCESS_CLASS_CONTENT                                              \
    /* duplication of the base object class content */                                  \
    ANSC_HANDLE                     hOwnerContext;                                      \
    PFN_BSPAO_OUTPUT                Output;                                             \
    PFN_BSPAO_MEMBER                Member;                                             \
    PFN_BSPAO_VALUE                 Value;                                              \
    PFN_BSPENG_OBJ_REMOVE           Remove;                                             \
    PFN_BSPENG_OBJ_INIT             Initialize;                                         \

/* content filtering basic message object  */
typedef  struct
_BSP_TEMPLATE_ACCESS_OBJECT
{
    BSP_TEMPLATE_ACCESS_CLASS_CONTENT
}
BSP_TEMPLATE_ACCESS_OBJECT, *PBSP_TEMPLATE_ACCESS_OBJECT;


/* -----------------------------------------------------------------------------
 *              Object "_TEMPLATE_LIST"
 * -------------------------------------------------------------------------- */

/*
 * This object just defines the interface that template users need to
 * derive from. This object is defined to communicate with user's module
 * to get data from outside.
 */

#define  BSPENG_LIST_COMPONENT_NAME                 "BspEngListComponent"
#define  BSPENG_LIST_COMPONENT_OID                  ANSC_OBJECT_OID_NULL
#define  BSPENG_LIST_COMPONENT_TYPE                 ANSC_OBJECT_TYPE_NULL

#define  BSP_TEMPLATE_LIST_REG_TABLE_SIZE           16

typedef
ANSC_HANDLE
(*PFN_BSPLO_ADD_ITEM)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pText
    );

typedef
ANSC_HANDLE
(*PFN_BSPLO_UPDATE_ITEM)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pOrigText,
        const char                  *pText
    );

typedef
ANSC_HANDLE
(*PFN_BSPLO_FIND_ITEM)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName
    );

typedef
BOOL
(*PFN_BSPLO_ADD_GROUP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReader,
        BOOL                        bRemoveAll,
        const char                  *pVirtualName
    );

typedef
ANSC_STATUS
(*PFN_BSPLO_LOAD_INCLUDES)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hIncludeTmpl
    );

typedef
BOOL
(*PFN_BSPLO_IS_TEMPLATE_LOADED)
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pTemplateName
    );

typedef
ANSC_STATUS
(*PFN_BSPLO_SET_TEMPLATE_LOADED)
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pTemplateName
    );

typedef
ANSC_STATUS
(*PFN_BSPLO_REGISTER_TEMPLATE)
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        char                        *pContent,
        ULONG                       ulLen
    );

typedef
char *
(*PFN_BSPLO_GET_REG_TEMPLATE)
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        PULONG                      pulLen
    );

typedef
void
(*PFN_BSPLO_SET_TEMPLATE_SOURCE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bFileBase
    );

typedef
void
(*PFN_BSPLO_USE_ARCHIVE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bArchive
    );

typedef
void
(*PFN_BSPLO_ADD_TEMPLATE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTemplate
    );

typedef
ANSC_HANDLE
(*PFN_BSPLO_GET_TEMPLATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

typedef
void
(*PFN_BSPLO_IMPORT_TEMPLATES)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pTemplateName,
        ANSC_HANDLE                 hImportList
    );

typedef
void
(*PFN_BSPLO_CLEAR_TEMPLATES)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_BSPLO_SET_CMIF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCmif
    );

typedef
ANSC_HANDLE
(*PFN_BSPLO_GET_CMIF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_BSPLO_CACHE_TEMPLATES)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName
    );

typedef
ANSC_STATUS
(*PFN_BSPLO_LOAD_CACHE_TEMPLATES)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCacheEntry
    );

typedef  struct
_BSP_TEMPLATE_REG_PAGE_INFO
{
    SINGLE_LINK_ENTRY               Linkage;        /* used to link into the upper level list */
    char                            *pName;         /* template name */
    char                            *pContent;      /* template content */
    ULONG                           ulLen;          /* template content length */
}
BSP_TEMPLATE_REG_PAGE_INFO, *PBSP_TEMPLATE_REG_PAGE_INFO;

#define  ACCESS_BSP_TEMPLATE_REG_PAGE_INFO(p)                       \
         ACCESS_CONTAINER(p, BSP_TEMPLATE_REG_PAGE_INFO, Linkage)


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  BSP_TEMPLATE_LIST_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                  \
    ANSC_HANDLE                     hList;                                              \
    ANSC_HANDLE                     hReader;                                            \
    ANSC_HANDLE                     hLoadedTemplatesList;                               \
    QUEUE_HEADER                    RegTemplateTable[BSP_TEMPLATE_LIST_REG_TABLE_SIZE]; \
    BOOL                            bFileBasedTemplate;                                 \
    BOOL                            bSourceArchive;                                     \
    ANSC_HANDLE                     hCmif;                                              \
    ANSC_HANDLE                     hBspSoaIf;                                          \
    /* APIs */                                                                          \
    PFN_BSPLO_ADD_ITEM              AddItem;                                            \
    PFN_BSPLO_UPDATE_ITEM           UpdateItem;                                         \
    PFN_BSPLO_FIND_ITEM             FindItem;                                           \
    PFN_BSPLO_ADD_GROUP             AddGroup;                                           \
    PFN_BSPENG_OBJECT_STORE         Store;                                              \
    PFN_BSPENG_OBJECT_LOAD          Load;                                               \
    PFN_BSPLO_LOAD_INCLUDES         LoadIncludes;                                       \
    PFN_BSPLO_IS_TEMPLATE_LOADED    IsTemplateLoaded;                                   \
    PFN_BSPLO_SET_TEMPLATE_LOADED   SetTemplateLoaded;                                  \
    PFN_BSPLO_REGISTER_TEMPLATE     RegisterTemplate;                                   \
    PFN_BSPLO_GET_REG_TEMPLATE      GetRegisteredTemplate;                              \
    PFN_BSPLO_SET_TEMPLATE_SOURCE   SetTemplateSource;                                  \
    PFN_BSPLO_USE_ARCHIVE           UseArchive;                                         \
    PFN_BSPLO_ADD_TEMPLATE          AddTemplate;                                        \
    PFN_BSPLO_GET_TEMPLATE          GetTemplate;                                        \
    PFN_BSPLO_IMPORT_TEMPLATES      ImportTemplates;                                    \
    PFN_BSPLO_CLEAR_TEMPLATES       ClearTemplates;                                     \
    PFN_BSPLO_SET_CMIF              SetCmif;                                            \
    PFN_BSPLO_GET_CMIF              GetCmif;                                            \
    PFN_BSPLO_CACHE_TEMPLATES       CacheTemplates;                                     \
    PFN_BSPLO_LOAD_CACHE_TEMPLATES  LoadCacheTemplates;                                 \
    PFN_BSPENG_OBJ_REMOVE           Remove;                                             \
    PFN_BSPENG_OBJ_INIT             Initialize;                                         \
    PFN_BSPLO_SET_CMIF              SetBspSoaIf;                                        \
    PFN_BSPLO_GET_CMIF              GetBspSoaIf;                                        \

/* content filtering basic message object  */
typedef  struct
_BSP_TEMPLATE_LIST_OBJECT
{
    BSP_TEMPLATE_LIST_CLASS_CONTENT
}
BSP_TEMPLATE_LIST_OBJECT, *PBSP_TEMPLATE_LIST_OBJECT;


/* -----------------------------------------------------------------------------
 *              Object "_TEMPLATE_OUTPUT_SET"
 * -------------------------------------------------------------------------- */

/*
 * This object just defines the interface that template users need to
 * derive from. This object is defined to communicate with user's module
 * to get data from outside.
 */

#define  BSPENG_OUTPUT_SET_COMPONENT_NAME           "BspEngOutputSetComponent"
#define  BSPENG_OUTPUT_SET_COMPONENT_OID            ANSC_OBJECT_OID_NULL
#define  BSPENG_OUTPUT_SET_COMPONENT_TYPE           ANSC_OBJECT_TYPE_NULL


typedef
ANSC_HANDLE
(*PFN_BSPSO_SET)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hArgs,
        ULONG                       ulArgs
    );


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  BSP_TEMPLATE_OUTPUT_SET_CLASS_CONTENT                                          \
    /* duplication of the base object class content */                                  \
    PFN_BSPSO_SET                   Set;                                                \
    PFN_BSPENG_OBJ_REMOVE           Remove;                                             \
    PFN_BSPENG_OBJ_INIT             Initialize;                                         \

/* content filtering basic message object  */
typedef  struct
_BSP_TEMPLATE_OUTPUT_SET_OBJECT
{
    BSP_TEMPLATE_OUTPUT_SET_CLASS_CONTENT
}
BSP_TEMPLATE_OUTPUT_SET_OBJECT,    *PBSP_TEMPLATE_OUTPUT_SET_OBJECT;


/* -----------------------------------------------------------------------------
 *              Object "_TEMPLATE_VAR"
 * -------------------------------------------------------------------------- */

/*
 * This object just defines the interface that template users need to
 * derive from. This object is defined to communicate with user's module
 * to get data from outside.
 */

#define  BSPENG_VAR_COMPONENT_NAME                  "BspEngVarObj"
#define  BSPENG_VAR_COMPONENT_OID                   ANSC_OBJECT_OID_NULL
#define  BSPENG_VAR_COMPONENT_TYPE                  ANSC_OBJECT_TYPE_NULL

typedef
ANSC_HANDLE
(*PFN_BSPVO_INCREASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_DECREASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
LONG
(*PFN_BSPVO_GET_LONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_ASSIGN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
BOOL
(*PFN_BSPVO_GREATER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
BOOL
(*PFN_BSPVO_EQUAL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_PLUS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_MINUS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_MULTIPLY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_DIVIDE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_MOD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_STR_CAT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_BIT_OR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_BIT_AND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_BIT_XOR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_SHIFT_LEFT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
ANSC_HANDLE
(*PFN_BSPVO_SHIFT_RIGHT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef
void
(*PFN_BSPVO_POS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_BSPVO_NEG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ULONG
(*PFN_BSPVO_STR_TO_NUM)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_BSPVO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_BSPVO_SET_CONTENT_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pContentType
    );

typedef
void
(*PFN_BSPVO_SET_CONTENT_TYPE2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulContentType
    );

typedef
char *
(*PFN_BSPVO_GET_CONTENT_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_BSPVO_GET_BULKDATA_AT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArrayItem,
        ULONG                       ulIndex
    );

typedef
ANSC_STATUS
(*PFN_BSPVO_SET_ARRAY_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */

#define  BSP_TEMPLATE_VAR_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                  \
    /* Data members */                                                                  \
    BSP_TEMPLATE_VAR_TYPE           Type;                                               \
    BSP_TEMPLATE_VAR_VALUE          Value;                                              \
    ULONG                           Size;                                               \
    BOOL                            bTemp;                                              \
    ULONG                           ContentType;                                        \
    ULONG                           SlapSyntax;                                         \
    ANSC_HANDLE                     hRuntime;                                           \
    /* APIs */                                                                          \
    PFN_BSPVO_INCREASE              Increase;                                           \
    PFN_BSPVO_DECREASE              Decrease;                                           \
    PFN_BSPVO_GET_LONG              GetLong;                                            \
    PFN_BSPVO_ASSIGN                Assign;                                             \
    PFN_BSPVO_GREATER               Greater;                                            \
    PFN_BSPVO_EQUAL                 Equal;                                              \
    PFN_BSPVO_PLUS                  Plus;                                               \
    PFN_BSPVO_MINUS                 Minus;                                              \
    PFN_BSPVO_MULTIPLY              Multiply;                                           \
    PFN_BSPVO_DIVIDE                Divide;                                             \
    PFN_BSPVO_MOD                   Mod;                                                \
    PFN_BSPVO_STR_CAT               StrCat;                                             \
    PFN_BSPVO_BIT_OR                BitOr;                                              \
    PFN_BSPVO_BIT_AND               BitAnd;                                             \
    PFN_BSPVO_BIT_XOR               BitXor;                                             \
    PFN_BSPVO_SHIFT_LEFT            ShiftLeft;                                          \
    PFN_BSPVO_SHIFT_RIGHT           ShiftRight;                                         \
    PFN_BSPVO_POS                   Pos;                                                \
    PFN_BSPVO_NEG                   Neg;                                                \
    PFN_BSPVO_STR_TO_NUM            StrToNum;                                           \
    PFN_BSPENG_OBJ_REMOVE           Remove;                                             \
    PFN_BSPENG_OBJ_INIT             Initialize;                                         \
    PFN_BSPVO_RESET                 Reset;                                              \
    PFN_BSPVO_GET_CONTENT_TYPE      GetContentType;                                     \
    PFN_BSPVO_SET_CONTENT_TYPE      SetContentType;                                     \
    PFN_BSPVO_SET_CONTENT_TYPE2     SetContentType2;                                    \
    PFN_BSPVO_GET_BULKDATA_AT       GetBulkDataAt;                                      \
    PFN_BSPVO_SET_ARRAY_SIZE        SetArraySize;                                       \

/* content filtering basic message object  */
typedef  struct 
_BSP_TEMPLATE_VAR_OBJECT
{
    BSP_TEMPLATE_VAR_CLASS_CONTENT
}
BSP_TEMPLATE_VAR_OBJECT, *PBSP_TEMPLATE_VAR_OBJECT;


#define	 ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pVar)	\
	(pVar)->bTemp	= FALSE


typedef  struct
_BSP_TEMPLATE_VAR_POOL_ITEM
{
    SINGLE_LINK_ENTRY               Linkage;
    PBSP_TEMPLATE_VAR_OBJECT        pVar;
    BOOL                            bInUse;
}
BSP_TEMPLATE_VAR_POOL_ITEM, *PBSP_TEMPLATE_VAR_POOL_ITEM;

#define  ACCESS_BSP_TEMPLATE_VAR_POOL_ITEM(p)                                           \
         ACCESS_CONTAINER(p, BSP_TEMPLATE_VAR_POOL_ITEM, Linkage)


/* -----------------------------------------------------------------------------
 *              Object "_TEMPLATE_BRANCH"
 * -------------------------------------------------------------------------- */

/*
 * This object just defines the interface that template users need to
 * derive from.
 */

#define  BSPENG_BRANCH_COMPONENT_NAME               "BspEngBranch"
#define  BSPENG_BRANCH_COMPONENT_OID                ANSC_OBJECT_OID_NULL
#define  BSPENG_BRANCH_COMPONENT_TYPE               ANSC_OBJECT_TYPE_NULL


typedef
void
(*PFN_BSPBO_SET_CHILD_BRANCH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildBranch
    );

typedef
void
(*PFN_BSPBO_SET_OP)
    (
        ANSC_HANDLE                 hThisObject,
        BSP_TEMPLATE_OPERATOR       op
    );

typedef
BOOL
(*PFN_BSPBO_STORE_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        PSLIST_HEADER               pListHeader,
        ANSC_HANDLE                 hArchive,
        ANSC_HANDLE                 hBranchData
    );

typedef
BOOL
(*PFN_BSPBO_LOAD_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        PSLIST_HEADER               pListHeader,
        ANSC_HANDLE                 hArchive,
        ANSC_HANDLE                 hBranchData
    );

typedef
BOOL
(*PFN_BSPBO_STORE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  BSP_TEMPLATE_BRANCH_CLASS_CONTENT                                              \
    /* duplication of the base object class content */                                  \
    /* Data members */                                                                  \
    BSP_TEMPLATE_OPERATOR           op;                                                 \
    BSP_TEMPLATE_BRANCH_DATA        left;                                               \
    BSP_TEMPLATE_BRANCH_DATA        right;                                              \
    BOOL                            bTopOp;                                             \
    BOOL                            bObjSetProperty;                                    \


/* content filtering basic message object  */
typedef  struct
_BSP_TEMPLATE_BRANCH_OBJECT
{
    BSP_TEMPLATE_BRANCH_CLASS_CONTENT
}
BSP_TEMPLATE_BRANCH_OBJECT, *PBSP_TEMPLATE_BRANCH_OBJECT;


typedef  struct
_BSP_TEMPLATE_ACV_ITEM
{
    SINGLE_LINK_ENTRY               Linkage;
    BOOL                            bBranchData;
    void*                           pData;
}
BSP_TEMPLATE_ACV_ITEM, *PBSP_TEMPLATE_ACV_ITEM;

#define  ACCESS_TEMPLATE_ACV_ITEM(p)                                                \
         ACCESS_CONTAINER(p, BSP_TEMPLATE_ACV_ITEM, Linkage)

#define  BSP_TEMPLATE_PUSH_ACV_ITEM(bBrData, pBrData, pSListHeader)                             \
    do {                                                                                        \
        PBSP_TEMPLATE_ACV_ITEM      pAcvItem = NULL;                                            \
        pAcvItem = (PBSP_TEMPLATE_ACV_ITEM)AnscAllocateMemory(sizeof(BSP_TEMPLATE_ACV_ITEM));   \
        if (pAcvItem)                                                                           \
        {                                                                                       \
            pAcvItem->bBranchData = bBrData;                                                    \
            pAcvItem->pData       = pBrData;                                                    \
            AnscSListPushEntry((pSListHeader), &pAcvItem->Linkage);                             \
        }                                                                                       \
    } while (0)

/* -----------------------------------------------------------------------------
 *              Object "_TEMPLATE_TEMP"
 * -------------------------------------------------------------------------- */

/*
 * This object just defines the interface that template users need to
 * derive from.
 */

#define  BSPENG_TEMP_COMPONENT_NAME                 "BspEngTemp"
#define  BSPENG_TEMP_COMPONENT_OID                  ANSC_OBJECT_OID_NULL
#define  BSPENG_TEMP_COMPONENT_TYPE                 ANSC_OBJECT_TYPE_NULL


typedef
void
(*PFN_BSPTO_CLEAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_BSPTO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_BSPTO_SET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch
    );


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  BSP_TEMPLATE_TEMP_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                  \
    /* Data members */                                                                  \
    ANSC_HANDLE                     hBranchList[BSP_TEMPLATE_TEMP_SPACE];               \
    ULONG                           Count;                                              \
    /* APIs */                                                                          \
    PFN_BSPTO_CLEAR                 Clear;                                              \
    PFN_BSPTO_RELEASE               Release;                                            \
    PFN_BSPTO_SET                   Set;                                                \
    PFN_BSPENG_OBJ_REMOVE           Remove;                                             \
    PFN_BSPENG_OBJ_INIT             Initialize;                                         \

/* content filtering basic message object  */
typedef  struct
_BSP_TEMPLATE_TEMP_OBJECT
{
    BSP_TEMPLATE_TEMP_CLASS_CONTENT
}
BSP_TEMPLATE_TEMP_OBJECT, *PBSP_TEMPLATE_TEMP_OBJECT;


/* -----------------------------------------------------------------------------
 *              Object "_TEMPLATE_STACK"
 * -------------------------------------------------------------------------- */

/*
 * This object just defines the interface that template users need to
 * derive from.
 */

#define  BSPENG_STACK_COMPONENT_NAME                "BspEngStack"
#define  BSPENG_STACK_COMPONENT_OID                 ANSC_OBJECT_OID_NULL
#define  BSPENG_STACK_COMPONENT_TYPE                ANSC_OBJECT_TYPE_NULL

typedef
ANSC_HANDLE
(*PFN_BSPSTO_ADD)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

typedef
ANSC_HANDLE
(*PFN_BSPSTO_GET_AT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

typedef
ULONG
(*PFN_BSPSTO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ULONG
(*PFN_BSPSTO_GET_BASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ULONG
(*PFN_BSPSTO_GET_TOP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_BSPSTO_GET_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_STATE         pState
    );

typedef
void
(*PFN_BSPSTO_NEW_FUNCTION)
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_STATE         pState
    );

typedef
void
(*PFN_BSPSTO_DELETE_FUNCTION)
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_STATE         pState
    );

typedef
ANSC_HANDLE
(*PFN_BSPSTO_GET_VAR_BUFFER)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

typedef
void
(*PFN_BSPSTO_DUMP)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  BSP_TEMPLATE_STACK_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                  \
    /* Data members */                                                                  \
    ULONG                           Size;                                               \
    ULONG                           Base;                                               \
    ULONG                           Top;                                                \
    ANSC_HANDLE                     hVars;                                              \
    ANSC_HANDLE                     hRuntime;                                           \
    /* APIs */                                                                          \
    PFN_BSPSTO_ADD                  Add;                                                \
    PFN_BSPSTO_GET_AT               GetAt;                                              \
    PFN_BSPSTO_GET_COUNT            GetCount;                                           \
    PFN_BSPSTO_GET_BASE             GetBase;                                            \
    PFN_BSPSTO_GET_TOP              GetTop;                                             \
    PFN_BSPSTO_GET_STATE            GetState;                                           \
    PFN_BSPSTO_NEW_FUNCTION         NewFunction;                                        \
    PFN_BSPSTO_DELETE_FUNCTION      DeleteFunction;                                     \
    PFN_BSPSTO_GET_VAR_BUFFER       GetBufferAt;                                        \
    PFN_BSPENG_OBJ_REMOVE           Remove;                                             \
    PFN_BSPENG_OBJ_INIT             Initialize;                                         \
    PFN_BSPSTO_DUMP                 Dump;                                               \

/* content filtering basic message object  */
typedef  struct
_BSP_TEMPLATE_STACK_OBJECT
{
    BSP_TEMPLATE_STACK_CLASS_CONTENT
}
BSP_TEMPLATE_STACK_OBJECT, *PBSP_TEMPLATE_STACK_OBJECT;


/* -----------------------------------------------------------------------------
 *              Object "_TEMPLATE_RUNTIME"
 * -------------------------------------------------------------------------- */

/*
 * This object just defines the interface that template users need to
 * derive from.
 */
    
#define  BSPENG_RUNTIME_COMPONENT_NAME              "BspEngRuntime"
#define  BSPENG_RUNTIME_COMPONENT_OID               ANSC_OBJECT_OID_NULL
#define  BSPENG_RUNTIME_COMPONENT_TYPE              ANSC_OBJECT_TYPE_NULL

#define  BSPENG_RUNTIME_INIT_VARIABLE_POOL_SIZE     16
#define  BSPENG_RUNTIME_MAX_SLAP_PARAMETER_REF      64


typedef
void
(*PFN_BSPRTO_SET_ENV)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEnv
    );

typedef
void
(*PFN_BSPRTO_SET_WRITER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWriter
    );

typedef
void
(*PFN_BSPRTO_SET_OUTPUTSET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOutputset
    );

typedef
ANSC_HANDLE
(*PFN_BSPRTO_GET_STACK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_BSPRTO_CLEAR_RETURN_VALUE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_BSPRTO_SET_RETURN_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hValue
    );

typedef
ANSC_HANDLE
(*PFN_BSPRTO_GET_RETURN_VALUE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_BSPRTO_ACQ_VAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_BSPRTO_REL_VAR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

typedef 
ANSC_STATUS
(*PFN_BSPRTO_SET_PARAM_REF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 *pParamRef,
        ULONG                       ulParamCount
    );

typedef 
ANSC_HANDLE
(*PFN_BSPRTO_GET_PARAM_REF)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulParamRefCount
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  BSP_TEMPLATE_RUNTIME_CLASS_CONTENT                                             \
    /* duplication of the base object class content */                                  \
    /* Data members */                                                                  \
    ANSC_HANDLE                     hStack;                                             \
    ANSC_HANDLE                     hEnv;                                               \
    ULONG                           ulArgCount;                                         \
    ULONG                           ulNestCount;                                        \
    int                             Stop;                                               \
    ULONG                           LoopLimit;                                          \
    ANSC_HANDLE                     hOutputSet;                                         \
    ANSC_HANDLE                     hWriter;                                            \
    ANSC_HANDLE                     hExecute;                                           \
    BOOL                            InExecute;                                          \
    ANSC_HANDLE                     hReturnValue;                                       \
    SLIST_HEADER                    VariablePool;                                       \
    ANSC_HANDLE                     SlapParamRef[BSPENG_RUNTIME_MAX_SLAP_PARAMETER_REF];\
    ULONG                           SlapParamRefCount;                                  \
                                                                                        \
    /* APIs */                                                                          \
    PFN_BSPRTO_SET_ENV              SetEnv;                                             \
    PFN_BSPRTO_SET_WRITER           SetWriter;                                          \
    PFN_BSPRTO_SET_OUTPUTSET        SetOutputset;                                       \
    PFN_BSPRTO_GET_STACK            GetStack;                                           \
    PFN_BSPRTO_CLEAR_RETURN_VALUE   ClearReturnValue;                                   \
    PFN_BSPRTO_SET_RETURN_VALUE     SetReturnValue;                                     \
    PFN_BSPRTO_GET_RETURN_VALUE     GetReturnValue;                                     \
    PFN_BSPENG_OBJ_REMOVE           Remove;                                             \
    PFN_BSPENG_OBJ_INIT             Initialize;                                         \
    PFN_BSPRTO_ACQ_VAR              AcquireVariable;                                    \
    PFN_BSPRTO_REL_VAR              ReleaseVariable;                                    \
                                                                                        \
    PFN_BSPRTO_SET_PARAM_REF        SetSlapParamRef;                                    \
    PFN_BSPRTO_GET_PARAM_REF        GetSlapParamRef;                                    \


/* content filtering basic message object  */
typedef  struct
_BSP_TEMPLATE_RUNTIME_OBJECT
{
    BSP_TEMPLATE_RUNTIME_CLASS_CONTENT
}
BSP_TEMPLATE_RUNTIME_OBJECT, *PBSP_TEMPLATE_RUNTIME_OBJECT;


/* -----------------------------------------------------------------------------
 *              Object "_TEMPLATE_WRITER"
 * -------------------------------------------------------------------------- */

/*
 * This object just defines the interface that template users need to
 * derive from.
 */

#define  BSPENG_WRITER_COMPONENT_NAME               "BspEngWriter"
#define  BSPENG_WRITER_COMPONENT_OID                ANSC_OBJECT_OID_NULL
#define  BSPENG_WRITER_COMPONENT_TYPE               ANSC_OBJECT_TYPE_NULL


typedef 
BOOL
(*PFN_BSPWO_WRITE_BYTES)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pBuf,
        PULONG                      pCount
    );

typedef
BOOL
(*PFN_BSPWO_WRITE_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pString
    );

typedef
ANSC_HANDLE
(*PFN_BSPWO_WRITE_CHAR)
    (
        ANSC_HANDLE                 hThisObject,
        char                        ch
    );

typedef
ANSC_HANDLE
(*PFN_BSPWO_WRITE_LONG)
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        lNum
    );

typedef
ANSC_HANDLE
(*PFN_BSPWO_WRITE_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNum
    );

typedef
ANSC_HANDLE
(*PFN_BSPWO_WRITE_INT)
    (
        ANSC_HANDLE                 hThisObject,
        int                         nNum
    );

typedef
ANSC_HANDLE
(*PFN_BSPWO_WRITE_UINT)
    (
        ANSC_HANDLE                 hThisObject,
        UINT                        uNum
    );

typedef
ANSC_HANDLE
(*PFN_BSPWO_WRITE_DOUBLE)
    (
        ANSC_HANDLE                 hThisObject,
#ifdef   _BSPENG_NO_DOUBLE
        BSP_TEMPLATE_DOUBLE         Value
#else
        DOUBLE                      Value
#endif
    );

typedef
ANSC_HANDLE
(*PFN_BSPWO_WRITE_HEX)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNum
    );

typedef
ANSC_HANDLE
(*PFN_BSPWO_WRITE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );
    
typedef
ANSC_STATUS
(*PFN_BSPWO_OPEN_EXTERNAL)
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName
    );

typedef
ANSC_STATUS
(*PFN_BSPWO_OPEN_INTERNAL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_BSPWO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
char *
(*PFN_BSPWO_GET_CONTENT)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulContentSize
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  BSP_TEMPLATE_WRITER_CLASS_CONTENT                                              \
    /* duplication of the base object class content */                                  \
    /* Data members */                                                                  \
    /* APIs */                                                                          \
    PFN_BSPWO_WRITE_BYTES           WriteBytes;                                         \
    PFN_BSPWO_WRITE_STRING          WriteString;                                        \
    PFN_BSPWO_WRITE_CHAR            WriteChar;                                          \
    PFN_BSPWO_WRITE_LONG            WriteLong;                                          \
    PFN_BSPWO_WRITE_ULONG           WriteUlong;                                         \
    PFN_BSPWO_WRITE_INT             WriteInt;                                           \
    PFN_BSPWO_WRITE_UINT            WriteUint;                                          \
    PFN_BSPWO_WRITE_DOUBLE          WriteDouble;                                        \
    PFN_BSPWO_WRITE_HEX             WriteHex;                                           \
    PFN_BSPWO_WRITE                 Write;                                              \
    PFN_BSPWO_OPEN_EXTERNAL         OpenExternal;                                       \
    PFN_BSPWO_OPEN_INTERNAL         OpenInternal;                                       \
    PFN_BSPWO_CLOSE                 Close;                                              \
    PFN_BSPWO_GET_CONTENT           GetContent;                                         \
    PFN_BSPENG_OBJ_REMOVE           Remove;                                             \
    PFN_BSPENG_OBJ_INIT             Initialize;                                         \

/* content filtering basic message object  */
typedef  struct
_BSP_TEMPLATE_WRITER_OBJECT
{
    BSP_TEMPLATE_WRITER_CLASS_CONTENT
}
BSP_TEMPLATE_WRITER_OBJECT, *PBSP_TEMPLATE_WRITER_OBJECT;


/* -----------------------------------------------------------------------------
 *              Object "_TEMPLATE_ARCHIVE"
 * -------------------------------------------------------------------------- */

/*
 * This object just defines the interface that template users need to
 * derive from. This object is defined to communicate with user's module
 * to get data from outside.
 */

#define  BSPENG_ARCHIVE_COMPONENT_NAME              "BspEngArchive"
#define  BSPENG_ARCHIVE_COMPONENT_OID               ANSC_OBJECT_OID_NULL
#define  BSPENG_ARCHIVE_COMPONENT_TYPE              ANSC_OBJECT_TYPE_NULL

#define  BSP_TEMPLATE_ARCHIVE_BINARY                0
#define  BSP_TEMPLATE_ARCHIVE_COMMA_SEPARATED       1

/* this value shouldn't be too big if very constraint resources a system has */
#define  BSP_TEMPLATE_ARCHIVE_OBJECT_STORAGE_GROW   512 

/*
 * Write an INTEGER value
 */
typedef
BOOL
(*PFN_BSPACV_WRITE_INTEGER)
    (
        ANSC_HANDLE                 hThisObject,
        int                         Value
    );

/*
 * Write a ulong value
 */
typedef
BOOL
(*PFN_BSPACV_WRITE_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Value
    );

/*
 * Write a double value
 */
typedef
BOOL
(*PFN_BSPACV_WRITE_DOUBLE)
    (
        ANSC_HANDLE                 hThisObject,
#ifdef   _BSPENG_NO_DOUBLE
        BSP_TEMPLATE_DOUBLE         Value
#else
        DOUBLE                      Value
#endif
    );

/*
 * Write a word value
 */
typedef
BOOL
(*PFN_BSPACV_WRITE_WORD)
    (
        ANSC_HANDLE                 hThisObject,
        int                         Value
    );

/*
 * Write a BOOLEAN value
 */
typedef
BOOL
(*PFN_BSPACV_WRITE_BOOLEAN)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        Value
    );

/*
 * Write a BYTE
 */
typedef
BOOL
(*PFN_BSPACV_WRITE_BYTE)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       Value
    );

/*
 * Write a string
 */
typedef
BOOL
(*PFN_BSPACV_WRITE_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      Value
    );

/*
 * Write a binary stream
 */
typedef
BOOL
(*PFN_BSPACV_WRITE_BINARY)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pStream,
        ULONG                       StreamLen
    );

/*
 * Read an INTEGER value
 */
typedef
BOOL
(*PFN_BSPACV_READ_INTEGER)
    (
        ANSC_HANDLE                 hThisObject,
        int                         *pValue
    );

/*
 * Read a ulong value
 */
typedef
BOOL
(*PFN_BSPACV_READ_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       *pValue
    );

/*
 * Read a double value
 */
typedef
BOOL
(*PFN_BSPACV_READ_DOUBLE)
    (
        ANSC_HANDLE                 hThisObject,
#ifdef   _BSPENG_NO_DOUBLE
        BSP_TEMPLATE_DOUBLE         *pValue
#else
        DOUBLE                      *pValue
#endif
    );

/*
 * Read a word value
 */
typedef
BOOL
(*PFN_BSPACV_READ_WORD)
    (
        ANSC_HANDLE                 hThisObject,
        int                         *pValue
    );

/*
 * Read a BOOLEAN value
 */
typedef
BOOL
(*PFN_BSPACV_READ_BOOLEAN)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        *pValue
    );

/*
 * Read a BYTE
 */
typedef
BOOL
(*PFN_BSPACV_READ_BYTE)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       *pValue
    );

/*
 * Read a string
 */
typedef
BOOL
(*PFN_BSPACV_READ_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      *pValue
    );

/*
 * Read a binary stream
 */
typedef
BOOL
(*PFN_BSPACV_READ_BINARY)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pStream,
        ULONG                       StreamLen
    );


/*
 * Enlarge the storage
 */
typedef
void
(*PFN_BSPACV_ENLARGE_STORAGE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIncAtLeast
    );

/*
 * Check available storage size
 */
typedef
BOOL
(*PFN_BSPACV_BIG_ENOUGH)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Size
    );

/*
 * Check available content size
 */
typedef
ULONG
(*PFN_BSPACV_GET_REMAIN_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Set storage
 */
typedef
void
(*PFN_BSPACV_SET_STORAGE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pStorage,
        ULONG                       StorageSize,
        BOOL                        NeedRelease     /* indicating whether this buffer needs to release */
    );

/*
 * Release storage of archive
 */
typedef
void
(*PFN_BSPACV_RELEASE_STORAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Prepare loading
 */
typedef
void
(*PFN_BSPACV_PREPARE_LOADING)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Write archive to file
 */
typedef
BOOL
(*PFN_BSPACV_SAVE_TO_FILE)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pFileName,
        ULONG                       ulMode
    );

typedef
PUCHAR
(*PFN_BSPACV_EXTRACT_CONTENT)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulContentLen,
        PBOOL                       pbNeedRelease
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  BSP_TEMPLATE_ARCHIVE_CLASS_CONTENT                                             \
    /* duplication of the base object class content */                                  \
    /* the buffer which holds the content of serialized objects */                      \
    PUCHAR                          pStorage;                                           \
                                                                                        \
    /* the length of the buffer */                                                      \
    ULONG                           StorageLen;                                         \
                                                                                        \
    /* available position for next object */                                            \
    ULONG                           CurPos;                                             \
                                                                                        \
    /* content length */                                                                \
    ULONG                           ContentLen;                                         \
                                                                                        \
    /* this flag tells whether or not the storage should be released */                 \
    BOOL                            NeedRelease;                                        \
                                                                                        \
    /* write integer */                                                                 \
    PFN_BSPACV_WRITE_INTEGER        WriteInteger;                                       \
                                                                                        \
    /* write ulong */                                                                   \
    PFN_BSPACV_WRITE_ULONG          WriteUlong;                                         \
                                                                                        \
    /* write double */                                                                  \
    PFN_BSPACV_WRITE_DOUBLE         WriteDouble;                                        \
                                                                                        \
    /* write word (2 bytes) */                                                          \
    PFN_BSPACV_WRITE_WORD           WriteWord;                                          \
                                                                                        \
    /* write boolean */                                                                 \
    PFN_BSPACV_WRITE_BOOLEAN        WriteBoolean;                                       \
                                                                                        \
    /* write a single byte */                                                           \
    PFN_BSPACV_WRITE_BYTE           WriteByte;                                          \
                                                                                        \
    /* write string */                                                                  \
    PFN_BSPACV_WRITE_STRING         WriteString;                                        \
                                                                                        \
    /* write binary stream */                                                           \
    PFN_BSPACV_WRITE_BINARY         WriteBinary;                                        \
                                                                                        \
    /* read integer */                                                                  \
    PFN_BSPACV_READ_INTEGER         ReadInteger;                                        \
                                                                                        \
    /* read ulong */                                                                    \
    PFN_BSPACV_READ_ULONG           ReadUlong;                                          \
                                                                                        \
    /* read double */                                                                   \
    PFN_BSPACV_READ_DOUBLE          ReadDouble;                                         \
                                                                                        \
    /* read word */                                                                     \
    PFN_BSPACV_READ_WORD            ReadWord;                                           \
                                                                                        \
    /* read boolean */                                                                  \
    PFN_BSPACV_READ_BOOLEAN         ReadBoolean;                                        \
                                                                                        \
    /* read a single byte */                                                            \
    PFN_BSPACV_READ_BYTE            ReadByte;                                           \
                                                                                        \
    /* read string */                                                                   \
    PFN_BSPACV_READ_STRING          ReadString;                                         \
                                                                                        \
    /* read binary stream */                                                            \
    PFN_BSPACV_READ_BINARY          ReadBinary;                                         \
                                                                                        \
    /* enlarge the storage */                                                           \
    PFN_BSPACV_ENLARGE_STORAGE      EnlargeStorage;                                     \
                                                                                        \
    /* check if storage is big enough (comparing with given size in bytes) */           \
    PFN_BSPACV_BIG_ENOUGH           BigEnough;                                          \
                                                                                        \
    /* get remained content size */                                                     \
    PFN_BSPACV_GET_REMAIN_SIZE      GetRemainSize;                                      \
                                                                                        \
    /* set up storage */                                                                \
    PFN_BSPACV_SET_STORAGE          SetStorage;                                         \
                                                                                        \
    /* release storage */                                                               \
    PFN_BSPACV_RELEASE_STORAGE      ReleaseStorage;                                     \
                                                                                        \
    /* prepare loading --- set current position to 0 */                                 \
    PFN_BSPACV_PREPARE_LOADING      PrepareLoading;                                     \
                                                                                        \
    /* save to file */                                                                  \
    PFN_BSPACV_SAVE_TO_FILE         SaveToFile;                                         \
    PFN_BSPACV_EXTRACT_CONTENT      ExtractContent;                                     \
    PFN_BSPENG_OBJ_REMOVE           Remove;                                             \

/* content filtering basic message object  */
typedef  struct
_BSP_TEMPLATE_ARCHIVE_OBJECT
{
    BSP_TEMPLATE_ARCHIVE_CLASS_CONTENT
}
BSP_TEMPLATE_ARCHIVE_OBJECT, *PBSP_TEMPLATE_ARCHIVE_OBJECT;



/* -----------------------------------------------------------------------------
 *              Object "_TEMPLATE_OBJECT"
 * -------------------------------------------------------------------------- */

typedef  struct
_BSP_TEMPLATE_NAME_PARAM
{
    /* parameters in one group */
    PBSP_TEMPLATE_VAR_OBJECT        *pParams;       
    /* number of parameters in this group including output parameters */
    ULONG                           ulParams;       
    /* number of output parameters in this group */
    ULONG                           ulOutParams;
}
BSP_TEMPLATE_NAME_PARAM, *PBSP_TEMPLATE_NAME_PARAM;

#define  CREATE_BSP_TEMPLATE_NAME_PARAM(pParam, Count)                          \
    {                                                                           \
        pParam  =                                                               \
            (PBSP_TEMPLATE_NAME_PARAM)AnscAllocateMemory                        \
                (                                                               \
                    sizeof(BSP_TEMPLATE_NAME_PARAM)                             \
                );                                                              \
        if (pParam)                                                             \
        {                                                                       \
            if (Count != 0)                                                     \
            {                                                                   \
                (pParam)->pParams   =                                           \
                    (PBSP_TEMPLATE_VAR_OBJECT *)AnscAllocateMemory              \
                        (                                                       \
                            Count * sizeof(PBSP_TEMPLATE_VAR_OBJECT)            \
                        );                                                      \
                if ((pParam)->pParams)                                          \
                {                                                               \
                    AnscZeroMemory                                              \
                        (                                                       \
                            (pParam)->pParams,                                  \
                            Count * sizeof(PBSP_TEMPLATE_VAR_OBJECT)            \
                        );                                                      \
                }                                                               \
            }                                                                   \
            (pParam)->ulParams      = Count;                                    \
            (pParam)->ulOutParams   = 0;                                        \
        }                                                                       \
    }                                                                           \


#define  REMOVE_BSP_TEMPLATE_NAME_PARAM(p)                                      \
    {                                                                           \
        PBSP_TEMPLATE_VAR_OBJECT    pVar;                                       \
        ULONG                       i;                                          \
        for (i = 0; (p) && i < (p)->ulParams; i ++)                             \
        {                                                                       \
            pVar = ((p)->pParams)[i];                                           \
            /* pVar->Remove((ANSC_HANDLE)pVar); */                              \
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVar);          \
        }                                                                       \
        if ((p)->pParams)                                                       \
        {                                                                       \
            AnscFreeMemory((p)->pParams);                                       \
        }                                                                       \
        AnscFreeMemory((p));                                                    \
    }                                                                           \


typedef  struct
_BSP_TEMPLATE_NAME_PARAM_LIST
{
    PBSP_TEMPLATE_NAME_PARAM        *pGroups;
    ULONG                           ulGroups;
}
BSP_TEMPLATE_NAME_PARAM_LIST, *PBSP_TEMPLATE_NAME_PARAM_LIST;


#define  CREATE_BSP_TEMPLATE_NAME_PARAM_LIST(pList, Count)                      \
    {                                                                           \
        pList   =                                                               \
            (PBSP_TEMPLATE_NAME_PARAM_LIST)AnscAllocateMemory                   \
                (                                                               \
                    sizeof(BSP_TEMPLATE_NAME_PARAM_LIST)                        \
                );                                                              \
        if (pList)                                                              \
        {                                                                       \
            if (Count != 0)                                                     \
            {                                                                   \
                (pList)->pGroups    =                                           \
                    (PBSP_TEMPLATE_NAME_PARAM *)AnscAllocateMemory              \
                        (                                                       \
                            Count * sizeof(PBSP_TEMPLATE_NAME_PARAM)            \
                        );                                                      \
                if ((pList)->pGroups)                                           \
                {                                                               \
                    AnscZeroMemory                                              \
                        (                                                       \
                            (pList)->pGroups,                                   \
                            Count * sizeof(PBSP_TEMPLATE_NAME_PARAM)            \
                        );                                                      \
                }                                                               \
            }                                                                   \
            (pList)->ulGroups   = Count;                                        \
        }                                                                       \
    }                                                                           \


#define  REMOVE_BSP_TEMPLATE_NAME_PARAM_LIST(pList)                             \
    {                                                                           \
        PBSP_TEMPLATE_NAME_PARAM    pParams;                                    \
        ULONG                       i;                                          \
        for (i = 0; (pList) && i < (pList)->ulGroups; i ++)                     \
        {                                                                       \
            pParams = ((pList)->pGroups)[i];                                    \
            REMOVE_BSP_TEMPLATE_NAME_PARAM(pParams);                            \
        }                                                                       \
        if ((pList)->pGroups)                                                   \
        {                                                                       \
            AnscFreeMemory((pList)->pGroups);                                   \
        }                                                                       \
        AnscFreeMemory(pList);                                                  \
    }                                                                           \


#define  BSPENG_COMPONENT_NAME                      "BspEngObj"
#define  BSPENG_COMPONENT_OID                       ANSC_OBJECT_OID_NULL
#define  BSPENG_COMPONENT_TYPE                      ANSC_OBJECT_TYPE_NULL

typedef
const char *
(*PFN_BSPTPO_PARSE)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pStr,
        ULONG                       ulStartLine
    );

typedef
void
(*PFN_BSPTPO_RUN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEnv,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hOutputSet,
        PVOID                       *oID
    );

typedef
const char *
(*PFN_BSPTPO_GET_TMPL_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_BSPTPO_STOP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime
    );

typedef
void
(*PFN_BSPTPO_PRINT_TOKENS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_BSPTPO_PRINT_BRANCH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        BOOL                        bStart
    );

typedef
const char *
(*PFN_BSPTPO_TOKENIZE)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pStr,
        ULONG                       ulStartLine
    );

typedef
BOOL
(*PFN_BSPTPO_PARSE_TEXT)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bIgnore
    );

typedef
BOOL
(*PFN_BSPTPO_PARSE_COMMENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_BSPTPO_PARSE_ID_KEYWORD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_BSPTPO_PARSE_STRING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_BSPTPO_PARSE_NUMBER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_BSPTPO_PARSE_OP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_BSPTPO_IS_NAME_CHAR)
    (
        ANSC_HANDLE                 hThisObject,
        int                         ch
    );

typedef
LONG
(*PFN_BSPTPO_GET_BINARY)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pSrc,
        const char                  **pEnd
    );

typedef
LONG
(*PFN_BSPTPO_GET_OCTAL)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pSrc,
        const char                  **pEnd
    );

typedef
LONG
(*PFN_BSPTPO_GET_HEX)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pSrc,
        const char                  **pEnd
    );

typedef
void
(*PFN_BSPTPO_PRINT_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 pBranchData
    );

typedef
BOOL
(*PFN_BSPTPO_COMPILE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_BSPTPO_COMPILE_PROCEDURES)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_TAKEOUT_PROCEDURE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 *phNextToken,
        ANSC_HANDLE                 *phPrevToken
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_BUILD_TREE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_STATEMENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_BSPTPO_IS_LVALUE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_BINARY_EXP_FUNC)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_BINARY_EXP)
    (
        ANSC_HANDLE                  hThisObject,
        PFN_BSPTPO_BINARY_EXP_FUNC   pFunc,
        const PBSP_TEMPLATE_OPERATOR pOpList
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_ARRAY_EXP_CONDITION)
    (
        ANSC_HANDLE                 hThisObject,
        BSP_TEMPLATE_OPERATOR       TerminalOp
    );

typedef
BOOL
(*PFN_BSPTPO_IS_EXP_PROC_CALL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hNameExp
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_NAME_EXP)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bWithParams
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_IS_TEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BSP_TEMPLATE_OPERATOR
(*PFN_BSPTPO_IS_OP)
    (
        ANSC_HANDLE                 hThisObject,
        BSP_TEMPLATE_OPERATOR       op
    );

typedef
BSP_TEMPLATE_OPERATOR
(*PFN_BSPTPO_IN_OP_LIST)
    (
        ANSC_HANDLE                 hThisObject,
        const BSP_TEMPLATE_OPERATOR Ops[]
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_IS_IDENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_IS_LITERAL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_BSPTPO_IS_END)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_PARSE_ERROR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOrigToken,
        const char                  *pMsg,
        ANSC_HANDLE                 hTemp
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_NO_MATCH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOrigToken
    );

typedef
ULONG
(*PFN_BSPTPO_NEW_SYMBOL)
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_SYMBOL_PRO    pSymbolPro
    );

typedef
ULONG
(*PFN_BSPTPO_FIND_SYMBOL)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        BOOL                        bAddSymbolIfNotFound
    );

typedef
void
(*PFN_BSPTPO_REMOVE_SYMBOLS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_BSPTPO_DO_TEMPLATE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        BOOL                        *pbTerminated
    );

typedef
void
(*PFN_BSPTPO_DO_STATEMENT_LIST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        BOOL                        BIter,
        BOOL                        *pbTerminated
    );

typedef
void
(*PFN_BSPTPO_DO_BRANCH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        *pbTerminated
    );

typedef
BSP_TEMPLATE_API_CODE
(*PFN_BSPTPO_GET_API_CODE)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pApiName
    );

typedef
BOOL
(*PFN_BSPTPO_IS_OUTPUT_API)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_EVAL_EXP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranchData,
        ANSC_HANDLE                 hRuntime,
        BOOL                        *pbTerminated
    );

typedef
const char *
(*PFN_BSPTPO_EVAL_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        BOOL                        bPassRef
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_EVAL_REF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranchData,
        ANSC_HANDLE                 hRuntime
    );

typedef
const char *
(*PFN_BSPTPO_EVAL_NAME_2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hParamList
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_STACK_VAR)
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        nVar,
        ANSC_HANDLE                 hRuntime,
        BOOL                        bParam
    );

typedef
void
(*PFN_BSPTPO_SET_LIST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerList
    );

typedef
ULONG
(*PFN_BSPTPO_GET_ERROR_LINE_NO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
const char *
(*PFN_BSPTPO_GET_ERROR_MSG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ULONG
(*PFN_BSPTPO_GET_LINE_NO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_GET_TOKEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_BSPTPO_COPY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTmpl
    );

typedef
void
(*PFN_BSPTPO_UPDATE_SYM)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTmpl
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_EVAL_ARRAY_ITEM)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranchData,
        ANSC_HANDLE                 hRuntime
    );

typedef
ANSC_STATUS
(*PFN_BSPTPO_SET_CMIF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCmif
    );

typedef
ANSC_STATUS
(*PFN_BSPTPO_INCLUDE_TEMPLATE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        PUCHAR                      pName,
        BOOL                        *pbTerminated
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_GET_BEEP_OBJECT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        bSlapVar,
        const char                  *pName,
        BOOL                        *pbSimpleVar
    );

typedef
ANSC_HANDLE
(*PFN_BSPTPO_DO_OBJECT_ACCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hObj,
        BOOL                        bSimpleVar,
        const char                  *pName,
        ANSC_HANDLE                 hParamList,
        ANSC_HANDLE                 hValueSet,
        BOOL                        *pbTerminated
    );

typedef
void
(*PFN_BSPTPO_ACCESS_VAR_CTYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        bSetContentType,
        ANSC_HANDLE                 hResultVar
    );

typedef
void
(*PFN_BSPTPO_OUTPUT_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pString
    );

typedef
void
(*PFN_BSPTPO_OUTPUT_BYTES)
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pString,
        ULONG                       ulSize
    );

typedef
void
(*PFN_BSPTPO_OUTPUT_NUMBER)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulVal,
        BOOL                        bSigned
    );

typedef
void
(*PFN_BSPTPO_OUTPUT_VAR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  BSP_TEMPLATE_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                  \
    /* Data members */                                                                  \
    const char                      *pName;                                             \
    ANSC_HANDLE                     hOwnerList;                                         \
    ANSC_HANDLE                     hText;                                              \
    const char                      *pTmplPtr;                                          \
    ANSC_HANDLE                     hToken;                                             \
    ANSC_HANDLE                     hPrevToken;                                         \
    ULONG                           ulLineNo;                                           \
    ANSC_HANDLE                     hTree;                                              \
    ULONG                           ulErrLineNo;                                        \
    const char                      *pErrMsg;                                           \
    ANSC_HANDLE                     hSymbol;                                            \
    ANSC_HANDLE                     hArrayMap;                                          \
    ULONG                           ulNumParams;                                        \
    ULONG                           ulInWhile;                                          \
    ULONG                           ulStackBase;                                        \
    ANSC_HANDLE                     hCmif;                                              \
    BOOL                            bVirtual;                                           \
    /* APIs */                                                                          \
    PFN_BSPTPO_SET_LIST             SetList;                                            \
    PFN_BSPTPO_PARSE                Parse;                                              \
    PFN_BSPTPO_RUN                  Run;                                                \
    PFN_BSPTPO_GET_TMPL_NAME        GetTmplName;                                        \
    PFN_BSPTPO_STOP                 Stop;                                               \
    PFN_BSPTPO_PRINT_TOKENS         PrintTokens;                                        \
    PFN_BSPTPO_PRINT_BRANCH         PrintBranch;                                        \
    PFN_BSPTPO_TOKENIZE             Tokenize;                                           \
    PFN_BSPTPO_PARSE_TEXT           ParseText;                                          \
    PFN_BSPTPO_PARSE_COMMENT        ParseComment;                                       \
    PFN_BSPTPO_PARSE_ID_KEYWORD     ParseIdentOrKeyword;                                \
    PFN_BSPTPO_PARSE_STRING         ParseString;                                        \
    PFN_BSPTPO_PARSE_NUMBER         ParseNumber;                                        \
    PFN_BSPTPO_PARSE_OP             ParseOp;                                            \
    PFN_BSPTPO_IS_NAME_CHAR         IsNameChar;                                         \
    PFN_BSPTPO_GET_BINARY           GetBinary;                                          \
    PFN_BSPTPO_GET_OCTAL            GetOctal;                                           \
    PFN_BSPTPO_GET_HEX              GetHex;                                             \
    PFN_BSPTPO_PRINT_DATA           PrintData;                                          \
    /* statement tree */                                                                \
    PFN_BSPTPO_COMPILE              Compile;                                            \
    PFN_BSPTPO_COMPILE_PROCEDURES   CompileProcedures;                                  \
    PFN_BSPTPO_TAKEOUT_PROCEDURE    TakeoutProcedure;                                   \
    PFN_BSPTPO_BUILD_TREE           BuildTree;                                          \
    /* syntax parsing function */                                                       \
    PFN_BSPTPO_STATEMENT            StatementList;                                      \
    PFN_BSPTPO_STATEMENT            Statement;                                          \
    PFN_BSPTPO_STATEMENT            IncludeStatement;                                   \
    PFN_BSPTPO_STATEMENT            IfStatement;                                        \
    PFN_BSPTPO_STATEMENT            IfElse;                                             \
    PFN_BSPTPO_STATEMENT            WhileStatement;                                     \
    PFN_BSPTPO_STATEMENT            SwitchStatement;                                    \
    PFN_BSPTPO_STATEMENT            ExpressionList;                                     \
    PFN_BSPTPO_STATEMENT            ReturnStatement;                                    \
    PFN_BSPTPO_STATEMENT            SetOutputStatement;                                 \
    PFN_BSPTPO_STATEMENT            Expression;                                         \
    PFN_BSPTPO_STATEMENT            ArrayDeclaration;                                   \
    PFN_BSPTPO_STATEMENT            ApiCall;                                            \
    PFN_BSPTPO_STATEMENT            AssignmentExp;                                      \
    PFN_BSPTPO_STATEMENT            LogicalExp;                                         \
    PFN_BSPTPO_STATEMENT            ComparisonExp;                                      \
    PFN_BSPTPO_STATEMENT            AdditiveExp;                                        \
    PFN_BSPTPO_STATEMENT            MultiplicativeExp;                                  \
    PFN_BSPTPO_STATEMENT            StrCatExp;                                          \
    PFN_BSPTPO_STATEMENT            UnaryExp;                                           \
    PFN_BSPTPO_STATEMENT            PostfixExp;                                         \
    PFN_BSPTPO_STATEMENT            ValueExp;                                           \
    PFN_BSPTPO_STATEMENT            ArrayExp;                                           \
    PFN_BSPTPO_STATEMENT            ApiExp;                                             \
    PFN_BSPTPO_STATEMENT            ObjectExp;                                          \
    PFN_BSPTPO_STATEMENT            VariableExp;                                        \
    PFN_BSPTPO_STATEMENT            ProcCallExp;                                        \
    PFN_BSPTPO_NAME_EXP             NameExp;                                            \
    PFN_BSPTPO_BINARY_EXP           BinaryExp;                                          \
    PFN_BSPTPO_IS_LVALUE            IsLValue;                                           \
    PFN_BSPTPO_ARRAY_EXP_CONDITION  ArrayExpCondition;                                  \
    PFN_BSPTPO_IS_EXP_PROC_CALL     IsExpProcCall;                                      \
    /* for tokenization */                                                              \
    PFN_BSPTPO_IS_TEXT              IsText;                                             \
    PFN_BSPTPO_IS_OP                IsOp;                                               \
    PFN_BSPTPO_IN_OP_LIST           InOpList;                                           \
    PFN_BSPTPO_IS_IDENT             IsIdent;                                            \
    PFN_BSPTPO_IS_LITERAL           IsLiteral;                                          \
    PFN_BSPTPO_IS_END               IsEnd;                                              \
    PFN_BSPTPO_PARSE_ERROR          ParseError;                                         \
    PFN_BSPTPO_NO_MATCH             NoMatch;                                            \
    PFN_BSPTPO_NEW_SYMBOL           NewSymbol;                                          \
    PFN_BSPTPO_FIND_SYMBOL          FindSymbol;                                         \
    PFN_BSPTPO_REMOVE_SYMBOLS       RemoveSymbols;                                      \
    /* execution */                                                                     \
    PFN_BSPTPO_DO_TEMPLATE          DoTemplate;                                         \
    PFN_BSPTPO_DO_STATEMENT_LIST    DoStatementList;                                    \
    PFN_BSPTPO_DO_BRANCH            DoBranch;                                           \
    PFN_BSPTPO_GET_API_CODE         GetApiCode;                                         \
    PFN_BSPTPO_IS_OUTPUT_API        IsOutputApi;                                        \
    PFN_BSPTPO_EVAL_EXP             EvalExpression;                                     \
    PFN_BSPTPO_EVAL_NAME            EvalName;                                           \
    PFN_BSPTPO_EVAL_REF             EvalRef;                                            \
    /* EvalName2 doesn't push parameters onto stack */                                  \
    PFN_BSPTPO_EVAL_NAME_2          EvalName2;                                          \
    PFN_BSPTPO_STACK_VAR            StackVar;                                           \
    PFN_BSPTPO_GET_ERROR_LINE_NO    GetErrorLineNo;                                     \
    PFN_BSPTPO_GET_ERROR_MSG        GetErrMsg;                                          \
    PFN_BSPTPO_GET_LINE_NO          GetLineNo;                                          \
    PFN_BSPTPO_GET_TOKEN            GetToken;                                           \
    PFN_BSPTPO_COPY                 Copy;                                               \
    PFN_BSPTPO_UPDATE_SYM           UpdateSymbols;                                      \
    PFN_BSPTPO_EVAL_ARRAY_ITEM      EvalArrayItem;                                      \
    PFN_BSPENG_OBJECT_STORE         Store;                                              \
    PFN_BSPENG_OBJECT_LOAD          Load;                                               \
    PFN_BSPTPO_SET_CMIF             SetCmif;                                            \
    PFN_BSPTPO_INCLUDE_TEMPLATE     IncludeTemplate;                                    \
    PFN_BSPENG_OBJ_REMOVE           Remove;                                             \
    /* BEEP object access */                                                            \
    PFN_BSPTPO_DO_OBJECT_ACCESS     DoObjectAccess;                                     \
    PFN_BSPTPO_GET_BEEP_OBJECT      GetBeepObject;                                      \
    PFN_BSPTPO_OUTPUT_STRING        OutputString;                                       \
    PFN_BSPTPO_OUTPUT_BYTES         OutputBytes;                                        \
    PFN_BSPTPO_OUTPUT_NUMBER        OutputNumber;                                       \
    PFN_BSPTPO_OUTPUT_VAR           OutputVar;                                          \
    PFN_BSPTPO_ACCESS_VAR_CTYPE     AccessVarContentType;                               \
    /* end of object class content */                                                   \


/* content filtering basic message object  */
typedef  struct
_BSP_TEMPLATE_OBJECT
{
    BSP_TEMPLATE_CLASS_CONTENT
}
BSP_TEMPLATE_OBJECT, *PBSP_TEMPLATE_OBJECT;


#define  BSPENG_PREPARE_BINARY_OPERANDS                                             \
                                                                                    \
    bDeleteL = bDeleteR = TRUE;                                                     \
                                                                                    \
    pValL =                                                                         \
        pMyObject->EvalExpression(hThisObject, &iBr->left, hRuntime, &bTerminated); \
                                                                                    \
    /*                                                                              \
    hVal  = pMyObject->EvalArrayItem                                                \
                            (                                                       \
                                hThisObject,                                        \
                                &iBr->left,                                         \
                                hRuntime                                            \
                            );                                                      \
                                                                                    \
    if (hVal)                                                                       \
    {                                                                               \
        pValL->Remove((ANSC_HANDLE)pValL);                                          \
        bDeleteL    = FALSE;                                                        \
        pValL       = (PBSP_TEMPLATE_VAR_OBJECT)hVal;                               \
    }                                                                               \
    */                                                                              \
                                                                                    \
    bDeleteL    = TRUE;                                                             \
                                                                                    \
    pValR =                                                                         \
        pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);\
                                                                                    \
    /*                                                                              \
    hVal  = pMyObject->EvalArrayItem                                                \
                            (                                                       \
                                hThisObject,                                        \
                                &iBr->right,                                        \
                                hRuntime                                            \
                            );                                                      \
                                                                                    \
    if (hVal)                                                                       \
    {                                                                               \
        pValR->Remove((ANSC_HANDLE)pValR);                                          \
        bDeleteR    = FALSE;                                                        \
        pValR       = (PBSP_TEMPLATE_VAR_OBJECT)hVal;                               \
    }                                                                               \
    */                                                                              \
    bDeleteR    = TRUE;                                                             \

#define  BSPENG_PREPARE_BINARY_OPERANDS_N                                           \
                                                                                    \
    {                                                                               \
    ANSC_HANDLE                     hVal;                                           \
                                                                                    \
    bDeleteL = bDeleteR = TRUE;                                                     \
                                                                                    \
    pValL =                                                                         \
        pMyObject->EvalExpression(hThisObject, &iBr->left, hRuntime, &bTerminated); \
                                                                                    \
    hVal  = pMyObject->EvalArrayItem                                                \
                            (                                                       \
                                hThisObject,                                        \
                                &iBr->left,                                         \
                                hRuntime                                            \
                            );                                                      \
                                                                                    \
    if (hVal)                                                                       \
    {                                                                               \
        /* pValL->Remove((ANSC_HANDLE)pValL); */                                    \
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);                 \
        bDeleteL    = FALSE;                                                        \
        pValL       = (PBSP_TEMPLATE_VAR_OBJECT)hVal;                               \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        /* pValL->Remove((ANSC_HANDLE)pValL); */                                    \
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);                 \
        bDeleteL    = FALSE;                                                        \
        pValL       =                                                               \
            pMyObject->StackVar(hThisObject, iBr->left.Value.n, hRuntime, FALSE);   \
    }                                                                               \
                                                                                    \
    pValR =                                                                         \
        pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);\
                                                                                    \
    /*
    hVal  = pMyObject->EvalArrayItem                                                \
                            (                                                       \
                                hThisObject,                                        \
                                (ANSC_HANDLE)pValR,                                 \
                                &iBr->right,                                        \
                                hRuntime                                            \
                            );                                                      \
                                                                                    \
    if (hVal)                                                                       \
    {                                                                               \
        pValR->Remove((ANSC_HANDLE)pValR);                                          \
        bDeleteR    = FALSE;                                                        \
        pValR       = (PBSP_TEMPLATE_VAR_OBJECT)hVal;                               \
    }                                                                               \
    */                                                                              \
    bDeleteR    = TRUE;                                                             \
    }                                                                               \

#define  BSPENG_REMOVE_BINARY_OPERANDS                                              \
    if (bDeleteL)                                                                   \
    {                                                                               \
        /* pValL->Remove((ANSC_HANDLE)pValL); */                                    \
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);                 \
    }                                                                               \
                                                                                    \
    if (bDeleteR)                                                                   \
    {                                                                               \
        /* pValR->Remove((ANSC_HANDLE)pValR); */                                    \
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);                 \
    }                                                                               \


#endif

