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

    module:	bspeng_co_internal_api.h

        For BSP Engine Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the string component
        object.

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

#ifndef  _BSPENG_CO_INTERNAL_API
#define  _BSPENG_CO_INTERNAL_API

/* ============== BSP Template Reader Object APIs ================== */
ULONG
BspTemplateReaderRead
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );

ANSC_STATUS
BspTemplateReaderClose
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateReaderOpenExternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName
    );

ANSC_STATUS
BspTemplateReaderOpenInternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        char                        *pContent,
        ULONG                       ulContentLen
    );

char *
BspTemplateReaderGetTemplateName
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_STATUS
BspTemplateReaderSetExtPath
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pExtPath
    );


/* ============== Template Access Object APIs ================== */
void
BspTemplateAccessOutput
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hArgs,
        ULONG                       ulArgs
    );

ANSC_HANDLE
BspTemplateAccessMember
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hArgs,
        ULONG                       ulArgs
    );

ANSC_HANDLE
BspTemplateAccessValue
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hArgs,
        ULONG                       ulArgs
    );


/* ============== Template List Object APIs ================== */

ANSC_HANDLE
BspTemplateListAddItem
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pText
    );

ANSC_HANDLE
BspTemplateListUpdateItem
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pOrigText,
        const char                  *pText
    );

ANSC_HANDLE
BspTemplateListFindItem
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName
    );

BOOL
BspTemplateListAddGroup
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReader,
        BOOL                        bRemoveAll,
        const char                  *pVirtualName
    );

BOOL
BspTemplateListStore
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    );

BOOL
BspTemplateListLoad
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    );

BOOL
BspTemplateListIsTemplateLoaded
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pTemplateName
    );

ANSC_STATUS
BspTemplateListSetTemplateLoaded
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pTemplateName
    );

ANSC_STATUS
BspTemplateListRegisterTemplate
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        char                        *pContent,
        ULONG                       ulLen
    );

char *
BspTemplateListGetRegisteredTemplate
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        PULONG                      pulLen
    );

void
BspTemplateListSetTemplateSource
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bFileBase
    );

void
BspTemplateListUseArchive
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bArchive
    );

void
BspTemplateListAddTemplate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTemplate
    );

ANSC_HANDLE
BspTemplateListGetTemplate
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

void
BspTemplateListImportTemplates
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pTemplateName,
        ANSC_HANDLE                 hImportList
    );

void
BspTemplateListClearTemplates
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateListSetCmif
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCmif
    );

ANSC_HANDLE
BspTemplateListGetCmif
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateListCacheTemplates
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName
    );

ANSC_STATUS
BspTemplateListLoadCacheTemplates
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCacheEntry
    );

ANSC_STATUS
BspTemplateListSetBspSoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCmif
    );

ANSC_HANDLE
BspTemplateListGetBspSoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

/* ============== Template Output Set Object APIs ================== */

ANSC_HANDLE
BspTemplateOutputSet
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hArgs,
        ULONG                       ulArgs
    );


/* ============== Template Var Object APIs ================== */

ANSC_HANDLE
BspTemplateVarIncrease
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateVarDecrease
    (
        ANSC_HANDLE                 hThisObject
    );


LONG
BspTemplateVarGetLong
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateVarAssign
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


BOOL
BspTemplateVarGreater
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


BOOL
BspTemplateVarEqual
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


ANSC_HANDLE
BspTemplateVarPlus
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


ANSC_HANDLE
BspTemplateVarMinus
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


ANSC_HANDLE
BspTemplateVarMultiply
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


ANSC_HANDLE
BspTemplateVarDivide
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


ANSC_HANDLE
BspTemplateVarMod
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


ANSC_HANDLE
BspTemplateVarStrCat
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


ANSC_HANDLE
BspTemplateVarBitOr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


ANSC_HANDLE
BspTemplateVarBitAnd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


ANSC_HANDLE
BspTemplateVarBitXor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


ANSC_HANDLE
BspTemplateVarShiftLeft
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


ANSC_HANDLE
BspTemplateVarShiftRight
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateVarPos
    (
        ANSC_HANDLE                 hThisObject
    );


void
BspTemplateVarNeg
    (
        ANSC_HANDLE                 hThisObject
    );


ULONG
BspTemplateVarStrToNum
    (
        ANSC_HANDLE                 hThisObject
    );

void
BspTemplateVarReset
    (
        ANSC_HANDLE                 hThisObject
    );

void
BspTemplateVarSetContentType
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pContentType
    );

void
BspTemplateVarSetContentType2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulContentType
    );

char *
BspTemplateVarGetContentType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateVarGetBulkDataAt
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArrayItem,
        ULONG                       ulIndex
    );

ANSC_STATUS
BspTemplateVarSetArraySize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize
    );

/* ============== Template Branch Object APIs ================== */

void
BspTemplateBranchSetLeft
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildBranch
    );

void
BspTemplateBranchSetRight
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildBranch
    );

void
BspTemplateBranchSetOp
    (
        ANSC_HANDLE                 hThisObject,
        BSP_TEMPLATE_OPERATOR       op
    );

BOOL
BspTemplateBranchStore
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    );

BOOL
BspTemplateBranchLoad
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    );

BOOL
BspTemplateBranchStoreBranchData
    (
        ANSC_HANDLE                 hThisObject,
        PSLIST_HEADER               pSListHeader,
        ANSC_HANDLE                 hArchive,
        ANSC_HANDLE                 hBranchData
    );

BOOL
BspTemplateBranchLoadBranchData
    (
        ANSC_HANDLE                 hThisObject,
        PSLIST_HEADER               pListHeader,
        ANSC_HANDLE                 hArchive,
        ANSC_HANDLE                 hBranchData
    );


/* ============== Template Temp Object APIs ================== */


void
BspTemplateTempClear
    (
        ANSC_HANDLE                 hThisObject
    );


void
BspTemplateTempRelease
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateTempSet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch
    );


/* ============== Template Stack Object APIs ================== */

ANSC_HANDLE
BspTemplateStackAdd
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );


ANSC_HANDLE
BspTemplateStackGetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );


ULONG
BspTemplateStackGetCount
    (
        ANSC_HANDLE                 hThisObject
    );


ULONG
BspTemplateStackGetBase
    (
        ANSC_HANDLE                 hThisObject
    );


ULONG
BspTemplateStackGetTop
    (
        ANSC_HANDLE                 hThisObject
    );


void
BspTemplateStackGetState
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_STATE         pState
    );


void
BspTemplateStackNewFunction
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_STATE         pState
    );


void
BspTemplateStackDeleteFunction
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_STATE         pState
    );

ANSC_HANDLE
BspTemplateStackGetBufferAt

    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

void
BspTemplateStackDump
    (
        ANSC_HANDLE                 hThisObject
    );

/* ============== Template Stack Object APIs ================== */


void
BspTemplateRuntimeSetEnv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEnv
    );


void
BspTemplateRuntimeSetWriter
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWriter
    );


void
BspTemplateRuntimeSetOutputset
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOutputset
    );


ANSC_HANDLE
BspTemplateRuntimeGetStack
    (
        ANSC_HANDLE                 hThisObject
    );


void
BspTemplateRuntimeClearReturnValue
    (
        ANSC_HANDLE                 hThisObject
    );


BOOL
BspTemplateRuntimeSetReturnValue
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hValue
    );


ANSC_HANDLE
BspTemplateRuntimeGetReturnValue
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
BspTemplateRuntimeAcquireVariable
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateRuntimeReleaseVariable
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

ANSC_STATUS
BspTemplateRuntimeSetSlapParamRef
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE*                pParamRef,
        ULONG                       ulParamCount
    );

ANSC_HANDLE
BspTemplateRuntimeGetSlapParamRef
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      ulParamRefCount
    );


/* ============== Template Engine Object APIs ================== */

const char *
BspTemplateObjParse
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pStr,
        ULONG                       ulStartLine
    );


void
BspTemplateObjRun
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEnv,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hOutputSet,
        PVOID                       *oID
    );


const char *
BspTemplateObjGetTmplName
    (
        ANSC_HANDLE                 hThisObject
    );


void
BspTemplateObjStop
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime
    );


void
BspTemplateObjPrintTokens
    (
        ANSC_HANDLE                 hThisObject
    );


void
BspTemplateObjPrintBranch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        BOOL                        bStart
    );


const char *
BspTemplateObjTokenize
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pStr,
        ULONG                       ulStartLine
    );


BOOL
BspTemplateObjParseText
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bIgnore
    );


BOOL
BspTemplateObjParseComment
    (
        ANSC_HANDLE                 hThisObject
    );


BOOL
BspTemplateObjParseIdentOrKeyword
    (
        ANSC_HANDLE                 hThisObject
    );


BOOL
BspTemplateObjParseString
    (
        ANSC_HANDLE                 hThisObject
    );


BOOL
BspTemplateObjParseNumber
    (
        ANSC_HANDLE                 hThisObject
    );


BOOL
BspTemplateObjParseOp
    (
        ANSC_HANDLE                 hThisObject
    );


BOOL
BspTemplateObjIsNameChar
    (
        ANSC_HANDLE                 hThisObject,
        int                         ch
    );


LONG
BspTemplateObjGetBinary
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pSrc,
        const char                  **pEnd
    );


LONG
BspTemplateObjGetOctal
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pSrc,
        const char                  **pEnd
    );


LONG
BspTemplateObjGetHex
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pSrc,
        const char                  **pEnd
    );


void
BspTemplateObjPrintData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 pBranchData
    );


BOOL
BspTemplateObjCompile
    (
        ANSC_HANDLE                 hThisObject
    );


BOOL
BspTemplateObjCompileProcedures
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjTakeoutProcedure
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 *phNextToken,
        ANSC_HANDLE                 *phPrevToken
    );


ANSC_HANDLE
BspTemplateObjBuildTree
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjStatementList
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjStatement
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
BspTemplateObjIncludeStatement
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjIfStatement
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjIfElse
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjWhileStatement
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
BspTemplateObjSwitchStatement
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
BspTemplateObjExpressionList
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjReturnStatement
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjSetouputStatement
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjExpression
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjArrayDeclaration
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjApiCall
    (
        ANSC_HANDLE                 hThisObject
    );


BOOL
BspTemplateObjIsLValue
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch
    );


ANSC_HANDLE
BspTemplateObjAssignmentExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjLogicalExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjComparisonExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjAdditiveExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjMultiplicativeExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjStrCatExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjBinaryExp
    (
        ANSC_HANDLE                  hThisObject,
        PFN_BSPTPO_BINARY_EXP_FUNC   pFunc,
        const PBSP_TEMPLATE_OPERATOR pOpList
    );


ANSC_HANDLE
BspTemplateObjUnaryExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjPostfixExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjValueExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjArrayExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjArrayExpCondition
    (
        ANSC_HANDLE                 hThisObject,
        BSP_TEMPLATE_OPERATOR       TerminalOp
    );


BOOL
BspTemplateObjIsExpProcCall
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hNameExp
    );


ANSC_HANDLE
BspTemplateObjApiExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjObjectExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjVariableExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjProcCallExp
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjNameExp
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bWithParams
    );


ANSC_HANDLE
BspTemplateObjIsText
    (
        ANSC_HANDLE                 hThisObject
    );


BSP_TEMPLATE_OPERATOR
BspTemplateObjIsOp
    (
        ANSC_HANDLE                 hThisObject,
        BSP_TEMPLATE_OPERATOR       op
    );


BSP_TEMPLATE_OPERATOR
BspTemplateObjInOpList
    (
        ANSC_HANDLE                 hThisObject,
        const BSP_TEMPLATE_OPERATOR Ops[]
    );


ANSC_HANDLE
BspTemplateObjIsIdent
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjIsLiteral
    (
        ANSC_HANDLE                 hThisObject
    );


BOOL
BspTemplateObjIsEnd
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjParseError
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOrigToken,
        const char                  *pMsg,
        ANSC_HANDLE                 hTemp
    );


ANSC_HANDLE
BspTemplateObjNoMatch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOrigToken
    );


ULONG
BspTemplateObjNewSymbol
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_SYMBOL_PRO    pSymbolPro
    );


ULONG
BspTemplateObjFindSymbol
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        BOOL                        bAddSymbolIfNotFound
    );


void
BspTemplateObjRemoveSymbols
    (
        ANSC_HANDLE                 hThisObject
    );


void
BspTemplateObjDoTemplate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        BOOL                        *pbTerminated
    );


void
BspTemplateObjDoStatementList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        BOOL                        BIter,
        BOOL                        *pbTerminated
    );


void
BspTemplateObjDoBranch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        *pbTerminated
    );


BSP_TEMPLATE_API_CODE
BspTemplateObjGetApiCode
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pApiName
    );


BOOL
BspTemplateObjIsOutputApi
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch
    );


ANSC_HANDLE
BspTemplateObjEvalExp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranchData,
        ANSC_HANDLE                 hRuntime,
        BOOL                        *pbTerminated
    );


const char *
BspTemplateObjEvalName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        BOOL                        bPassRef
    );


ANSC_HANDLE
BspTemplateObjEvalRef
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranchData,
        ANSC_HANDLE                 hRuntime
    );


const char *
BspTemplateObjEvalName2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hParamList
    );


ANSC_HANDLE
BspTemplateObjStackVar
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        nVar,
        ANSC_HANDLE                 hRuntime,
        BOOL                        bParam
    );


void
BspTemplateObjSetList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerList
    );


ULONG
BspTemplateObjGetErrorLineNo
    (
        ANSC_HANDLE                 hThisObject
    );


const char *
BspTemplateObjGetErrorMsg
    (
        ANSC_HANDLE                 hThisObject
    );


ULONG
BspTemplateObjGetLineNo
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_HANDLE
BspTemplateObjGetToken
    (
        ANSC_HANDLE                 hThisObject
    );

void
BspTemplateObjCopy
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTmpl
    );

void
BspTemplateObjUpdateSymbols
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTmpl
    );

ANSC_HANDLE
BspTemplateObjEvalArrayItem
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranchData,
        ANSC_HANDLE                 hRuntime
    );

BOOL
BspTemplateObjStore
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    );

BOOL
BspTemplateObjLoad
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    );

ANSC_STATUS
BspTemplateObjSetCmif
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCmif
    );

ANSC_STATUS
BspTemplateObjIncludeTemplate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        PUCHAR                      pName,
        BOOL                        *pbTerminated
    );

void
BspTemplateObjDoApi
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoInclude
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        *pbTerminated
    );


void
BspTemplateObjDoExecute
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoStrCat
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoSubStr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoModulo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoBitXorN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoShiftRightN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoShiftLeftN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoShiftRight
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoShiftLeft
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoBitAndN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoBitOrN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoModuloN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoDivideN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoDivide
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoMultiplyN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoMultiply
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoDecrN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoSubtract
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoIncrN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoAdd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoBitXor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoBitAnd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoBitNot
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoBitOr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoNot
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoAnd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoOr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoEq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoGt
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoLt
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoNe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoGe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoLe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoAssign
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoDecr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoIncr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoArrayItem
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoDim
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoParam
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoEnv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoObj
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        *pbTerminated
    );


void
BspTemplateObjDoObjProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoCall
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        *pbTerminated
    );


void
BspTemplateObjDoSetoutput
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoControl
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoWhile
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoSwitch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoElse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


void
BspTemplateObjDoTextblock
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    );


ANSC_HANDLE
BspTemplateEngGetBeepObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        bSlapVar,
        const char                  *pName,
        BOOL                        *pbSimpleVar
    );


ANSC_HANDLE
BspTemplateEngDoObjectAccess
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


void
BspTemplateEngRemoveToken
    (
        ANSC_HANDLE                 hToken
    );


void
BspTemplateEngOutputString
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pString
    );


void
BspTemplateEngOutputBytes
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pStream,
        ULONG                       ulSize
    );


void
BspTemplateEngOutputNumber
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulVal,
        BOOL                        bSigned
    );


void
BspTemplateEngOutputVar
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );


char *
BspTemplateEngGetSlapObjectString
    (
        ANSC_HANDLE                 hSlapVar
    );


void
BspTemplateEngAccessVarContentType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        bSetContentType,
        ANSC_HANDLE                 hResultVar
    );


/* ============== Template Writer Object APIs ================== */
 
BOOL
BspTemplateWriterWriteBytes
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pBuf,
        PULONG                      pCount
    );


BOOL
BspTemplateWriterWriteString
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pString
    );


ANSC_HANDLE
BspTemplateWriterWriteChar
    (
        ANSC_HANDLE                 hThisObject,
        char                        ch
    );


ANSC_HANDLE
BspTemplateWriterWriteLong
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        lNum
    );


ANSC_HANDLE
BspTemplateWriterWriteUlong
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNum
    );


ANSC_HANDLE
BspTemplateWriterWriteInt
    (
        ANSC_HANDLE                 hThisObject,
        int                         nNum
    );


ANSC_HANDLE
BspTemplateWriterWriteUint
    (
        ANSC_HANDLE                 hThisObject,
        UINT                        uNum
    );


ANSC_HANDLE
BspTemplateWriterWriteDouble
    (
        ANSC_HANDLE                 hThisObject,
#ifdef   _BSPENG_NO_DOUBLE
        BSP_TEMPLATE_DOUBLE         Value
#else
        DOUBLE                      Value
#endif
    );


ANSC_HANDLE
BspTemplateWriterWriteHex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNum
    );


ANSC_HANDLE
BspTemplateWriterWrite
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    );

ANSC_STATUS
BspTemplateWriterOpenExternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName
    );

ANSC_STATUS
BspTemplateWriterOpenInternal
    (
        ANSC_HANDLE                 hThisObject
    );

void
BspTemplateWriterClose
    (
        ANSC_HANDLE                 hThisObject
    );

char *
BspTemplateWriterGetContent
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulContentSize
    );


/* ============== Template Archive Set Object APIs ================== */

BOOL
BspTemplateArchiveWriteInteger
    (
        ANSC_HANDLE                 hThisObject,
        int                         Value
    );

BOOL
BspTemplateArchiveWriteUlong
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Value
    );

BOOL
BspTemplateArchiveWriteDouble
    (
        ANSC_HANDLE                 hThisObject,
#ifdef   _BSPENG_NO_DOUBLE
        BSP_TEMPLATE_DOUBLE         Value
#else
        DOUBLE                      Value
#endif
    );
    
BOOL
BspTemplateArchiveWriteWord
    (
        ANSC_HANDLE                 hThisObject,
        int                         Value
    );

BOOL
BspTemplateArchiveWriteBoolean
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        Value
    );

BOOL
BspTemplateArchiveWriteByte
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       Value
    );

BOOL
BspTemplateArchiveWriteString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      Value
    );

BOOL
BspTemplateArchiveWriteBinary
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pStream,
        ULONG                       StreamLen
    );

BOOL
BspTemplateArchiveReadInteger
    (
        ANSC_HANDLE                 hThisObject,
        int                         *pValue
    );

BOOL
BspTemplateArchiveReadUlong
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       *pValue
    );

BOOL
BspTemplateArchiveReadDouble
    (
        ANSC_HANDLE                 hThisObject,
#ifdef   _BSPENG_NO_DOUBLE
        BSP_TEMPLATE_DOUBLE         *pValue
#else
        DOUBLE                      *pValue
#endif
    );

BOOL
BspTemplateArchiveReadWord
    (
        ANSC_HANDLE                 hThisObject,
        int                         *pValue
    );

BOOL
BspTemplateArchiveReadBoolean
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        *pValue
    );

BOOL
BspTemplateArchiveReadByte
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       *pValue
    );

BOOL
BspTemplateArchiveReadString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      *pValue
    );

BOOL
BspTemplateArchiveReadBinary
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pStream,
        ULONG                       StreamLen
    );

void
BspTemplateArchiveEnlargeStorage
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIncAtLeast
    );

BOOL
BspTemplateArchiveBigEnough
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Size
    );

ULONG
BspTemplateArchiveGetRemainSize
    (
        ANSC_HANDLE                 hThisObject
    );

void
BspTemplateArchiveSetStorage
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pStorage,
        ULONG                       StorageSize,
        BOOL                        NeedRelease     /* indicating whether this buffer needs to release */
    );

void
BspTemplateArchiveReleaseStorage
    (
        ANSC_HANDLE                 hThisObject
    );

void
BspTemplateArchivePrepareLoading
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
BspTemplateArchiveSaveToFile
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pFileName,
        ULONG                       ulMode
    );

PUCHAR
BspTemplateArchiveExtractContent
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulContentLen,
        PBOOL                       pbNeedRelease
    );


#endif

