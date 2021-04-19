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

    module:	bspeng_co_process.c

        For BSP Engine Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the string Component Object.

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


#include "bspeng_co_global.h"
#include "safec_lib_common.h"

const char BspTemplateAPINames[][_BSP_TEMPLATE_API_MAX_NAME_LENGTH] = 
{
    "_atoi", "_atof", "_itoa", "_ftoa",
    "_strstr", "_strcmp", "_strcmpi", "_strncmp", 
    "_strlen", "_strgetchar", "_strsetchar",
    "_strleft", "_strmid", "_strright", "_sleep"
};


#ifdef   _BSP_RICH_FEATURE_SET
/* 
 *  the follow array contains the contents of corresponding name
 *  defined in "_TEMPLATE_OPERATOR" (tmpleng_co_interface.h)
 */
const char* const sBspOpList[BspOp_Term + 1] = {
	"", "", "include",
	"if", "elseif", "else", "endif",
	"while", "endwhile", "", "break", "continue", "return", "exit", 
	"setoutput", "endsetoutput",
	"call", "sys", "env", "", "param", "endproc", "substr", "execute", "dim", "", "", 
    "switch", "endswitch", "case", "default",
    "",
	"=", "++", "--", "+=", "-=", "*=", "/=", "%=",
    "|=", "&=", "<<=", ">>=", "^=", 
    "<<", ">>", 
	"<=", ">=", "!=", "<", ">", "==",
	"||", "&&", "!", "|", "&", "^", "~", 
	"+", "-", "*", "/", "%", "$", 
	"[", "]", ",", "(", ")", ".",
	"\n", "proc", ";", ":", 
    "#>"
};

#else

/* 
 *  the follow array contains the contents of corresponding name
 *  defined in "_TEMPLATE_OPERATOR" (tmpleng_co_interface.h)
 */
const char* const sBspOpList[BspOp_Term + 1] = {
	"", "", "include",
	"if", "elseif", "else", "endif",
	"while", "endwhile", "", "break", "continue", "return", "exit", 
	"setoutput", "endsetoutput",
	"call", "", "", "", "param", "endproc", "substr", "execute", "dim", "", "", 
    "switch", "endswitch", "case", "default",
    "",
	"=", "++", "--", "+=", "-=", "*=", "/=", "%=",
    "|=", "&=", "<<=", ">>=", "^=", 
    "<<", ">>", 
	"<=", ">=", "!=", "<", ">", "==",
	"||", "&&", "!", "|", "&", "^", "~", 
	"+", "-", "*", "/", "%", "$", 
	"[", "]", ",", "(", ")", ".",
	"\n", "proc", ";", ":",
    "#>"
};

#endif

const char BspReservedProcNames[][_BSP_TEMPLATE_API_MAX_NAME_LENGTH] = 
{
    "_atoi", "_atof", "_itoa", "_ftoa", "execute"
};


static BOOL
BspEngIsCharSpace
    (
        char                        ch
    );


static BOOL
BspEngIsCharAlpha
    (
        char                        ch
    );


static BOOL
BspEngIsCharDigit
    (
        char                        ch
    );

/**********************************************************************

    caller:     owner of this object

    prototype:

        const char *
        BspTemplateObjParse
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pStr,
                ULONG                       ulStartLine
            )

    description:

        This function is called to parse given BSP script.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pStr
                BSP script.

                ULONG                       ulStartLine
                Where to start.

    return:     tokens parsed.

**********************************************************************/

const char *
BspTemplateObjParse
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pStr,
        ULONG                       ulStartLine
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PANSC_BYTE_ARRAY_OBJECT         pByteArray;
    const char                      *pNew;

    pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;
    pByteArray->RemoveAll((ANSC_HANDLE)pByteArray);
    pByteArray->SetGrowth((ANSC_HANDLE)pByteArray, BSPENG_TOKENIZER_BA_GROWTH);

#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        AnscTrace("Start tokenizing, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds());
    }
#endif

    pNew = pMyObject->Tokenize(hThisObject, pStr, ulStartLine);

#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        AnscTrace("Tokenizing done, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds());
    }
#endif

    if (pNew)
    {
/*
#ifdef   _DEBUG
        if ( BspEngIsCallFlowTraced() )
        {
            pMyObject->PrintTokens(hThisObject);
        }
#endif
*/

#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        AnscTrace("Start compiling, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds());
    }
#endif

        pMyObject->Compile(hThisObject);

#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        AnscTrace("Compilation done, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds());
    }
#endif

        /* remove tokens */
        BspTemplateEngRemoveToken(pMyObject->hToken);
    }

    return pNew;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjRun
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEnv,
                ANSC_HANDLE                 hWriter,
                ANSC_HANDLE                 hOutputSet,
                PVOID                       *oID
            )

    description:

        This function is called to run a compiled BSP script.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEnv
                BSP execution environment.

                ANSC_HANDLE                 hWriter
                BSP writer object.

                ANSC_HANDLE                 hOutputSet
                BSP output set object.

                PVOID                       *oID

    return:     void.

**********************************************************************/

void
BspTemplateObjRun
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEnv,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hOutputSet,
        PVOID                       *oID
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_LIST_OBJECT       pList       = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;
    PBSPENG_SOA_INTERFACE           pBspSoaIf   = pList->GetBspSoaIf((ANSC_HANDLE)pList);
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt, *poID;
    BOOL                            bTerminated = FALSE;

    if ( !pBspSoaIf )
    {
        return;
    }

    pRt     = (PBSP_TEMPLATE_RUNTIME_OBJECT)CreateBspEngRuntimeComponent(NULL, NULL, NULL);

    /* we got to return immediately */
    if (!pRt)
        return;

    poID    = (PBSP_TEMPLATE_RUNTIME_OBJECT *)oID;

    pRt->LoopLimit  = BSP_TEMPLATE_LOOP_LIMIT;
    pRt->hEnv       = hEnv;
    pRt->hOutputSet = hOutputSet;
    pRt->hWriter    = hWriter;

    if (poID)
        *poID   = pRt;

    pMyObject->DoTemplate(hThisObject, (ANSC_HANDLE)pRt, &bTerminated);

    if (poID)
        *poID   = NULL;

    pRt->Remove((ANSC_HANDLE)pRt);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        const char *
        BspTemplateObjGetTmplName
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get template name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     template name.

**********************************************************************/

const char *
BspTemplateObjGetTmplName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;

    return pMyObject->pName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjStop
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to stop the execution of BSP script.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateObjStop
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    
    if (pRt)
        pRt->Stop   = BspOp_Exit;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        const char *
        BspTemplateObjTokenize
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pStr,
                ULONG                       ulStartLine
            )

    description:

        This function is called to tokenize the given script.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     tokens parsed.

**********************************************************************/

const char *
BspTemplateObjTokenize
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pStr,
        ULONG                       ulStartLine
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    BOOL                            bInText   = TRUE;
/*    BOOL                            bAtStart  = TRUE;*/
    BOOL                            bAtEnd    = FALSE;
    PBSP_TEMPLATE_TOKEN             pFirst = NULL, pToken = NULL, pPrevToken = NULL, pPrev2Token = NULL;
    char                            ch;
    BOOL                            bOK;

    pMyObject->pTmplPtr = pStr;
    pMyObject->ulLineNo = ulStartLine;

    pMyObject->hToken   = AnscAllocateMemory(sizeof(BSP_TEMPLATE_TOKEN));

    if (!pMyObject->hToken)
    {
        return NULL;
    }

    pFirst  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    pMyObject->hPrevToken   = NULL;

    while (*pMyObject->pTmplPtr != '\0')
    {
        if (bInText)
        {
/*  Always try to parse free text
            if (pMyObject->ParseText(hThisObject, bAtStart))
*/
            if (pMyObject->ParseText(hThisObject, FALSE))
            {
                pMyObject->hPrevToken = pMyObject->hToken;
                pMyObject->hToken = AnscAllocateMemory(sizeof(BSP_TEMPLATE_TOKEN));
                if (!pMyObject->hToken)
                {
                    break;
                }
                pToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
                pToken->pNext   = NULL;
                pPrevToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hPrevToken;
                if (pPrevToken)
                    pPrevToken->pNext   = pToken;

                pToken->Type            = BspToken_eOp;
                pToken->Value.op        = BspOp_End;
                pToken->LineNo          = pMyObject->ulLineNo;
                
                pMyObject->hPrevToken   = pMyObject->hToken;
                pMyObject->hToken       = AnscAllocateMemory(sizeof(BSP_TEMPLATE_TOKEN));
                if (!pMyObject->hToken)
                {
                    break;
                }
                pToken                  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
                pToken->pNext           = NULL;

                pPrevToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hPrevToken;
                if (pPrevToken)
                    pPrevToken->pNext   = pToken;
                
                if (bAtEnd)
                    break;
            }

            /*bAtStart = FALSE;*/
	    bInText = FALSE;

            continue;
        }

        ch = *pMyObject->pTmplPtr;
        if (BspEngIsCharSpace(ch))
        {
            if (ch == '\n')
            {
                pMyObject->ulLineNo ++;
                goto NEXT_LINE;
            }

            pMyObject->pTmplPtr ++;

            continue;
        }

NEXT_LINE:
        bOK = FALSE;

        /* if (BspEngIsCharAlpha(ch) || ch == '_') */
        /* allow number to be partial name in object exp */
        if (BspEngIsCharAlpha(ch) || ch == '_' || 
            (BspEngIsCharDigit(ch) && pPrev2Token && pPrev2Token->Type == BspToken_eIdent && pPrevToken->Type == BspToken_eOp && pPrevToken->Value.op == BspOp_Period)) 
            bOK = pMyObject->ParseIdentOrKeyword(hThisObject);
        else
/*
            if (ch == '"' || ch == '\\')
*/
            if (ch == '"')
                bOK = pMyObject->ParseString(hThisObject);
            else
                if (BspEngIsCharDigit(ch))
                    bOK = pMyObject->ParseNumber(hThisObject);
                else
                    if (pMyObject->ParseComment(hThisObject))
                        continue;
                    else
                    {
                        if (pMyObject->ParseOp(hThisObject))
                        {
                            pToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;

                            bOK = bAtEnd ? pToken->Value.op == BspOp_Term : TRUE;

                            if (pToken->Value.op == BspOp_Term)
                            {
                                pToken->Value.op = BspOp_End;
                                bInText = TRUE;
                            }

                            pPrevToken = (PBSP_TEMPLATE_TOKEN)pMyObject->hPrevToken;

                            if (
                                    pToken->Value.op == BspOp_End        && 
                                    pPrevToken                      &&
                                    pPrevToken->Type == BspToken_eOp   &&
                                    pPrevToken->Value.op == BspOp_End
                               )
                            {
                                continue;
                            }
                        }

                        if (!bOK)
                        {
                            pPrevToken = (PBSP_TEMPLATE_TOKEN)pMyObject->hPrevToken;

                            pMyObject->ulErrLineNo = pPrevToken ? pPrevToken->LineNo : ulStartLine;
                            if (!pMyObject->pErrMsg)
                            {
                                pMyObject->pErrMsg     = "missing or invalid characters";
                            }
                            BspTemplateEngRemoveToken((ANSC_HANDLE)pFirst);
                            
                            return NULL;
                        }

                        pMyObject->hPrevToken   = pMyObject->hToken;
                        pMyObject->hToken   = (ANSC_HANDLE)AnscAllocateMemory(sizeof(BSP_TEMPLATE_TOKEN));
                        if (!pMyObject->hToken)
                        {
                            break;
                        }
                        pToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
                        pToken->pNext = NULL;

                        pPrev2Token = pPrevToken;
                        pPrevToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hPrevToken;
                        if (pPrevToken)
                            pPrevToken->pNext   = pToken;

                        if (bAtEnd)
                            break;

                        pPrevToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hPrevToken;

                        /* tokenize everything
                        if (pPrevToken->Type == BspToken_eOp && pPrevToken->Value.op == BspOp_EndProc)
                            bAtEnd  = TRUE;
                    
                        */
                    }
    }

    if (pMyObject->hToken == (ANSC_HANDLE)pFirst)
    {
        /* first BSP must not be empty */
        if (ulStartLine == 1) 
        {
            pMyObject->ulErrLineNo  = ulStartLine;
            pMyObject->pErrMsg      = "missing or invalid characters";
            BspTemplateEngRemoveToken((ANSC_HANDLE)pFirst);
            pMyObject->hToken       = NULL;
            return NULL;
        }
        else
            pFirst  = NULL;
    }

    BspTemplateEngRemoveToken(pMyObject->hToken);

    pPrevToken = (PBSP_TEMPLATE_TOKEN)pMyObject->hPrevToken;

    if (pPrevToken)
        pPrevToken->pNext   = NULL;

    pMyObject->hToken       = (ANSC_HANDLE)pFirst;

    return pMyObject->pTmplPtr;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjParseText
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bIgnore
            )

    description:

        This function is called to parse free text block.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bIgnore
                Whether to add token in list.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjParseText
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bIgnore
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PANSC_BYTE_ARRAY_OBJECT         pByteArray;
    ULONG                           ulSize, ulCount;
    char                            ch;
    PBSP_TEMPLATE_TOKEN             pToken    = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;

    if (*pMyObject->pTmplPtr == '\n')
    {
        pMyObject->pTmplPtr ++;
        pMyObject->ulLineNo ++;
    }
    else
        if (*pMyObject->pTmplPtr == '\r' && *(pMyObject->pTmplPtr + 1) == '\n')
        {
            pMyObject->pTmplPtr += 2;
            pMyObject->ulLineNo ++;
        }

    pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;

    /* skip leading tabs */
    while (*pMyObject->pTmplPtr == '\t')
        pMyObject->pTmplPtr ++;

    ulSize  = pByteArray->GetSize((ANSC_HANDLE)pByteArray);

    while (
              *pMyObject->pTmplPtr != '\0'          &&
              (*pMyObject->pTmplPtr != '<' || *(pMyObject->pTmplPtr + 1) != '#')
          )
    {
        if (!bIgnore)
        {
            pByteArray->Add((ANSC_HANDLE)pByteArray, (ANSC_OBJECT_ARRAY_DATA)(intptr_t)*pMyObject->pTmplPtr);
        }

        if (*pMyObject->pTmplPtr++ == '\n')
            pMyObject->ulLineNo ++;
    }

    if (*pMyObject->pTmplPtr != '\0')
        pMyObject->pTmplPtr += 2;

    /* skip trailing tabs */
    ulCount = pByteArray->GetSize((ANSC_HANDLE)pByteArray);
    while (ulCount != 0)
    {
        ch = (char)(intptr_t)pByteArray->GetAt((ANSC_HANDLE)pByteArray, ulCount - 1);
        if (ch == '\t')
        {
            pByteArray->RemoveAt((ANSC_HANDLE)pByteArray, ulCount - 1, 1);
            ulCount --;
        }
        else
            break;
    }

    ulCount = pByteArray->GetSize((ANSC_HANDLE)pByteArray);
    if (!bIgnore && ulCount != ulSize)
    {
        pToken->Type        = BspToken_eText;
        pToken->Value.str   = ulSize;
        pToken->TextLen     = ulCount -  ulSize;
        pToken->LineNo      = pMyObject->ulLineNo;

        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjParseComment
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to parse comments.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjParseComment
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    char                            *pBuf, *pNext;
    char                            ch, chNext;
    BOOL                            bLineComm = TRUE;
    BOOL                            bFoundCommEnd   = FALSE;

    /* 
     * we support two types of comments like C++ language does.
     * They are block comment and line comment.
     * Be notice that the line comment will be ended at the end
     * of the current line (CR/LF or #> reached).
     */

    pBuf    = (char *)pMyObject->pTmplPtr;

    if (*pBuf != '/')
        return FALSE;
    else
        if (*pBuf == '\0')
            return FALSE;

    ch = *(pBuf+1);

    if (ch == '/')
    {
        bLineComm = TRUE;
    }
    else
        if (ch == '*')
        {
            bLineComm   = FALSE;
        }
        else
            return FALSE;

    /* search the end of comment */
    ch      = (char)0xFF;
    while ((ch = *pMyObject->pTmplPtr) != '\0')
    {
        if (bLineComm)
        {
            if (
                   (ch == '\r' || ch == '\n') ||
                   (ch == '#' && *(pMyObject->pTmplPtr + 1) == '>')
               )
            {
                bFoundCommEnd   = TRUE;
                pMyObject->ulLineNo ++;

                break;
            }
        }
        else
        {
            pNext   = (char *)pMyObject->pTmplPtr + 1;
            chNext  = *pNext;

            if (ch == '*' && chNext == '/')
            {
                pMyObject->pTmplPtr += 2;
                bFoundCommEnd   = TRUE;

                break;
            }
            else if ( ch == '\n' )
            {
                pMyObject->ulLineNo ++;
            }
        }

        pMyObject->pTmplPtr ++;
    }

    if (!bFoundCommEnd)
    {
        pMyObject->pErrMsg     = "comment end expected";
    }

    if (*pMyObject->pTmplPtr == '\0')
        return FALSE;

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateParseIdentOrKeyword
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to parse Identifier or Keyword.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjParseIdentOrKeyword
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PANSC_BYTE_ARRAY_OBJECT         pByteArray;
    ULONG                           ulSize;
    char                            ch;
    ULONG                           i;
    char                            *pBuf;
    PBSP_TEMPLATE_TOKEN             pToken;

    pToken      = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;
    ulSize      = pByteArray->GetSize((ANSC_HANDLE)pByteArray);

    while (TRUE)
    {
        ch = *pMyObject->pTmplPtr;
        if (!pMyObject->IsNameChar(hThisObject, ch))
            break;

        pMyObject->pTmplPtr ++;

#ifdef   _BSP_SCRIPT_CASE_INSENSITIVE
        /* keywords and identifiers must be case-insensitive */
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';
#endif

        pByteArray->Add((ANSC_HANDLE)pByteArray, (ANSC_OBJECT_ARRAY_DATA)(intptr_t)ch);
    }

    pByteArray->Add((ANSC_HANDLE)pByteArray, (ANSC_OBJECT_ARRAY_DATA)'\0');

    pBuf    = pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, ulSize);

    for (i = BspOp_None; i < BspOp_Name; i ++)
    {
        if (strcmp((char *)pBuf, (char *)sBspOpList[i]) == 0 )
            break;
    }

    pToken->LineNo      = pMyObject->ulLineNo;

    if (i < BspOp_Name)
    {
        /*
            pByteArray->SetSize((ANSC_HANDLE)pByteArray, ulSize, (ULONG)-1);
        */
        pToken->Type        = BspToken_eOp;
        pToken->Value.op    = (BSP_TEMPLATE_OPERATOR)i;
    }
    else
    {
        pToken->Type        = BspToken_eIdent;
        pToken->Value.str   = ulSize;
    }

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjParseString
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to parse a string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjParseString
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pToken    = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    char                            ch;
    PANSC_BYTE_ARRAY_OBJECT         pByteArray;
    const char                      *pEnd; 

    pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;

    pToken->Type        = BspToken_eString;
    pToken->LineNo      = pMyObject->ulLineNo;
    pToken->Value.str   = pByteArray->GetSize((ANSC_HANDLE)pByteArray);

/*
    while ((ch = *++pMyObject->pTmplPtr) != '\0' && ch != '"' && ch != '\'')
*/
    while ((ch = *++pMyObject->pTmplPtr) != '\0' && ch != '"')
    {
        if (ch == '\\')
        {
            ch = *++pMyObject->pTmplPtr;
            if (ch == '\0')
                break;

            switch (ch)
            {
            case 't':
                ch = '\t';
                break;

            case 'n':
                ch = '\n';
                break;

            case 'r':
                ch = '\r';
                break;

            case '0':
                
                if ( TRUE )
                {
                    char            chNext  = *(pMyObject->pTmplPtr+1);

                    pEnd    = (char*)pMyObject->pTmplPtr + 2;

                    if ( chNext == 'x' || chNext == 'X' )
                    {
                        ch = (char)pMyObject->GetHex(hThisObject, pMyObject->pTmplPtr + 2,  &pEnd);
                    }
                    else if ( chNext == 'b' || chNext == 'B' )
                    {
                        ch = (char)pMyObject->GetBinary(hThisObject, pMyObject->pTmplPtr + 2,  &pEnd);
                    }
                    else if ( chNext >= '0' && chNext <= '7' )
                    {
                        ch = (char)pMyObject->GetOctal(hThisObject, pMyObject->pTmplPtr,  &pEnd);
                    }
                    else
                    {
                        ch = '\0';
                    }

                    pMyObject->pTmplPtr = pEnd - 1;
                }

                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':

                ch = (char)pMyObject->GetOctal(hThisObject, pMyObject->pTmplPtr,  &pEnd);
                pMyObject->pTmplPtr = pEnd;
                
                break;

            default:
                /* strip off the back slash */
                break;
            }
        }

        pByteArray->Add((ANSC_HANDLE)pByteArray, (ANSC_OBJECT_ARRAY_DATA)(intptr_t)ch);
    }

    pByteArray->Add((ANSC_HANDLE)pByteArray, (ANSC_OBJECT_ARRAY_DATA)'\0');

    pMyObject->pTmplPtr ++;

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjParseNumber
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to parse a number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjParseNumber
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    char                             *pEnd =NULL; 
    LONG                            aLong;
    PBSP_TEMPLATE_TOKEN             pToken    = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;

#ifdef   _BSPENG_NO_DOUBLE
    BSP_TEMPLATE_DOUBLE             aDouble;
#else
    DOUBLE                          aDouble;
#endif

    if (*pMyObject->pTmplPtr == '0')
    {
        switch (*(pMyObject->pTmplPtr+1))
        {
        case 'X':
        case 'x':
            aLong   = pMyObject->GetHex(hThisObject, pMyObject->pTmplPtr + 2,(const char **) &pEnd);
            break;

        case 'b':
        case 'B':
            aLong   = pMyObject->GetBinary(hThisObject, pMyObject->pTmplPtr + 2, (const char **)&pEnd);
            break;

        default:
            aLong   = pMyObject->GetOctal(hThisObject, pMyObject->pTmplPtr + 1, (const char **)&pEnd);
            break;
        }

        pToken->Type        = BspToken_eNumber;
        pToken->Value.num   = aLong;
    }
    else
    {
#ifdef   _BSPENG_NO_DOUBLE
        BOOL                        bHasFrac    = FALSE;
        BOOL                        *pHasFrac    = &bHasFrac;
        char                        **ppEnd = &pEnd;

        STRING_TO_BSP_TEMPLATE_DOUBLE((char *)pMyObject->pTmplPtr, &aDouble, ppEnd, pHasFrac);

        if (bHasFrac)
        {
            aLong       = BSP_TEMPLATE_DOUBLE_GET_INT(aDouble);
        }
        else
        {
            /* 
             * since the number is not real (no fraction), we don't need to
             * consider it as "double", so the number range can be the same
             * as we expect, namely 0 to 2 ^ 32 - 1 (- 2^ 31 ~ 2 ^ 31 - 1).
             */
            pEnd    = (char *)pMyObject->pTmplPtr;

            while (pEnd && *pEnd != 0)
            {
                if (*pEnd < '0' || *pEnd > '9')
                {
                    break;
                }

                pEnd ++;
            }

            aLong       = _ansc_atol((const char *)pMyObject->pTmplPtr);
        }
#else
        ULONG                       ulLen;
        char                        *pDot;

        aDouble     = _ansc_strtod((const char *)pMyObject->pTmplPtr, (char **)&pEnd);
        aLong       = (LONG)aDouble;

        ulLen       = pEnd - pMyObject->pTmplPtr;
        pDot        = _ansc_memchr(pMyObject->pTmplPtr, '.', ulLen);
#endif

#ifdef   _BSPENG_NO_DOUBLE
        if (!bHasFrac)
#else
        if (!pDot)
#endif
        {
            pToken->Type        = BspToken_eNumber;
            pToken->Value.num   = aLong;
        }
        else
        {
            pToken->Type        = BspToken_eReal;
            pToken->Value.real  = aDouble;
        }
    }

    pToken->LineNo      = pMyObject->ulLineNo;
    pMyObject->pTmplPtr = (const char *)pEnd;

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        const char *
        BspTemplateObjParseOp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to parse operator.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjParseOp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    ULONG                           i, ulLen;
    PBSP_TEMPLATE_TOKEN             pToken    = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;

    for (i = BspOp_Assign; i <= BspOp_Term; i ++)
    {
        ulLen   = AnscSizeOfString(sBspOpList[i]);
        if (AnscEqualMemory((char *)pMyObject->pTmplPtr, (char *)sBspOpList[i], ulLen))
            break;
    }

    if (i == BspOp_Assign)
    {
        if (*(char *)(pMyObject->pTmplPtr + 1) == '=')
        {
            i = BspOp_Eq;
        }
    }

    if (i <= BspOp_Term)
    {
        pToken->Type        = BspToken_eOp;
        pToken->Value.op    = (BSP_TEMPLATE_OPERATOR)i;
        pToken->LineNo      = pMyObject->ulLineNo;
        pMyObject->pTmplPtr +=AnscSizeOfString(sBspOpList[i]);

        return TRUE;
    }
    else
        return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjIsNameChar
            (
                ANSC_HANDLE                 hThisObject,
                int                         ch
            )

    description:

        This function is called to determine if given character
        is valid in identifier.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                int                         ch
                character to be determined.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjIsNameChar
    (
        ANSC_HANDLE                 hThisObject,
        int                         ch
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    if (ch == '_')
        return TRUE;

    if (ch >= '0' && ch <= '9')
        return TRUE;

    if (ch >= 'a' && ch <= 'z')
        return TRUE;

    if (ch >= 'A' && ch <= 'Z')
        return TRUE;

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        BspTemplateObjGetBinary
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get binary number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pSrc
                Where to start parsing.

                const char                  **pEnd
                Contains the end of the binary number.

    return:     status of operation.

**********************************************************************/

LONG
BspTemplateObjGetBinary
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pSrc,
        const char                  **pEnd
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
	LONG                            aRet;
	
    for (aRet = 0; *pSrc == '0' || *pSrc == '1'; pSrc ++)
    {
		aRet = (aRet << 1) + (*pSrc - '0');
    }

	*pEnd = pSrc;

	return aRet;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        BspTemplateObjGetOctal
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pSrc,
                const char                  **pEnd
            )

    description:

        This function is called to get octal number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pSrc
                Where to parse octal number.

                const char                  **pEnd
                Where the octal number ends

    return:     the value of octal number.

**********************************************************************/

LONG
BspTemplateObjGetOctal
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pSrc,
        const char                  **pEnd
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    LONG                            aRet;
    int                             dig = 0;
    int                             nCount    = 0;

    *pEnd   = NULL;

    for (aRet = 0, *pEnd = NULL; nCount < 3 || *pEnd == NULL; pSrc ++)
    {
        switch (*pSrc)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
            dig = *pSrc - '0';
            nCount ++;
            break;

        default:
            *pEnd   = pSrc;
            nCount  = 3;
            break;
        }

        if (*pEnd == NULL)
            aRet    = (aRet << 3) + dig;
    }

    if ( *pEnd == NULL )
    {
        *pEnd = pSrc;
    }

    return aRet;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        BspTemplateObjGetHex
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pSrc,
                const char                  **pEnd
            )

    description:

        This function is called to parse HEX number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pSrc
                Where to start parsing.

                const char                  **pEnd
                Where hex number ends.

    return:     the value of hex number.

**********************************************************************/

LONG
BspTemplateObjGetHex
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pSrc,
        const char                  **pEnd
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    LONG                            aRet;
    int                             dig  = 0;
    int                             nCount    = 0;

    *pEnd   = NULL;

    for (aRet = 0, *pEnd = NULL; nCount < 2 || *pEnd == NULL; pSrc ++)
    {
        switch (*pSrc)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            dig     = *pSrc - '0';
            break;

        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            dig     = *pSrc - 'a' + 10;
            break;

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            dig     = *pSrc - 'A' + 10;
            break;

        default:
            *pEnd   = pSrc;
            nCount  = 2;
            break;
        }

        nCount ++;

        if (*pEnd == NULL)
            aRet    = (aRet << 4) + dig;
    }

    if ( *pEnd == NULL )
    {
        *pEnd   = pSrc;
    }

    return aRet;
}


#ifdef   _DEBUG 

/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjPrintData
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranchData
            )

    description:

        This function is called to print out branch data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranchData
                Branch data to be output.

    return:     void.

**********************************************************************/

void
BspTemplateObjPrintData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranchData
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_DATA       pBrData   = (PBSP_TEMPLATE_BRANCH_DATA)hBranchData;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr;

    if (pBrData->type == BspBranch_eBranch)
    {
        pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)pBrData->Value.b;

        pMyObject->PrintBranch((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pBr, FALSE);

        return;
    }

    switch (pBrData->type)
    {
    case BspBranch_eString:
        AnscTrace("%s", pBrData->Value.s);
        break;

    case BspBranch_eNumber:
        AnscTrace("%ld ", pBrData->Value.n);
        break;

    case BspBranch_eReal:
#ifdef   _BSPENG_NO_DOUBLE
        {
            char                    buf[16];

            BSP_TEMPLATE_DOUBLE_TO_STRING(buf, (int)pBrData->Value.r);
            AnscTrace(buf);
        }
#else
        AnscTrace("%f ", pBrData->Value.r);
#endif
        break;

    case BspBranch_eVar:
        AnscTrace("var %ld ", pBrData->Value.n);
        break;

    case BspBranch_eEmpty:
        break;

    case BspBranch_eArrayDim:
    case BspBranch_eArrayItem:
    case BspBranch_eApiParams:
        {
            PBSP_TEMPLATE_ARRAY_DATA    pData;
            ULONG                       i;
            PBSP_TEMPLATE_BRANCH_OBJECT pBr;

            pData = &pBrData->Value.a;

            AnscTrace("dimension %lu ", pData->Count);

            for (i = 0; i < pData->Count; i ++)
            {
                pBr = pData->pBranch[i];
                pMyObject->PrintBranch(hThisObject, (ANSC_HANDLE)pBr, FALSE);
                if (i != pData->Count - 1)
                    AnscTrace(" , ");
            }
        }
        break;

    default:
        AnscTrace("unknown branch type\r\n");
        break;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjPrintTokens
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to print out all tokens.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateObjPrintTokens
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pToken    = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    BOOL                            bHasToken = (pToken != NULL);

    if (bHasToken)
        AnscTrace("======== Dumping Tokens ===========\r\n");

    while (pToken)
    {
        switch(pToken->Type)
        {
        case BspToken_eText:
            AnscTrace("text ");
            break;

        case BspToken_eString:
            AnscTrace("string ");
            break;

        case BspToken_eIdent:
            AnscTrace("ident ");
            break;

        case BspToken_eNumber:
            AnscTrace("number ");
            break;

        case BspToken_eReal:
            AnscTrace("real ");
            break;

        case BspToken_eOp:
            AnscTrace("%s ", sBspOpList[pToken->Value.op]);
            if (pToken->Value.op == BspOp_End)
                AnscTrace("\r\n");
            break;
            
        default:
            AnscTrace("unknown token type! ");
            break;
        }

        pToken  = pToken->pNext;
    }

    if (bHasToken)
        AnscTrace("\n======== Tokens dumped ===========\r\n\r\n");
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjGetTmplName
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                BOOL                        bStart
            )

    description:

        This function is called to print branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                BOOL                        bStart
                The first branch or not.

    return:     void.

**********************************************************************/

void
BspTemplateObjPrintBranch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        BOOL                        bStart
    )
{
    UNREFERENCED_PARAMETER(hBranch);
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr = NULL; /*RDKB-6004, CID-24596, Initializing the variable*/

    if (bStart)
    {
        if (pMyObject->ulErrLineNo)
        {
            AnscTrace("Line %lu: %s\n", pMyObject->ulErrLineNo, pMyObject->pErrMsg);
        }

        pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)pMyObject->hTree;
    }

    if (!pBr)
        return;

    switch (pBr->op)
    {
    case BspOp_TextBlock:
        AnscTrace("text ");
        break;

    case BspOp_If:
        AnscTrace("If ");
        pMyObject->PrintData(hThisObject, &pBr->right);
        AnscTrace("\r\n");
        break;

    case BspOp_Else:
        pMyObject->PrintData(hThisObject, &pBr->right);
        AnscTrace("else \r\n");
        pMyObject->PrintData(hThisObject, &pBr->left);
        break;

    case BspOp_While:
        AnscTrace("while ");
        pMyObject->PrintData(hThisObject, &pBr->right);
        AnscTrace("\r\n");
        pMyObject->PrintData(hThisObject, &pBr->left);
        break;

	case BspOp_Break: 
    case BspOp_Continue: 
    case BspOp_Return: 
    case BspOp_Exit:
        AnscTrace("%s ", sBspOpList[pBr->op]);
        break;

    case BspOp_Iter:
        AnscTrace("iter ");
        pMyObject->PrintData(hThisObject, &pBr->right);
        AnscTrace("\r\n");
        pMyObject->PrintData(hThisObject, &pBr->left);
        break;

    case BspOp_SetOutput:
        AnscTrace("setoutput ");
        pMyObject->PrintData(hThisObject, &pBr->right);
        AnscTrace("\r\n");
        pMyObject->PrintData(hThisObject, &pBr->left);
        break;

    case BspOp_Call:
    case BspOp_Env:
    case BspOp_Param:
    case BspOp_Execute:
        AnscTrace("%s ", sBspOpList[pBr->op]);
        pMyObject->PrintData(hThisObject, &pBr->right);
        break;

    case BspOp_Api:
        pMyObject->PrintData(hThisObject, &pBr->left);
        AnscTrace(" ( ");
        pMyObject->PrintData(hThisObject, &pBr->right);
        AnscTrace(" ) \r\n");
        break;

    case BspOp_Obj:
        pMyObject->PrintData(hThisObject, &pBr->left);
        pMyObject->PrintData(hThisObject, &pBr->right);
        break;

    case BspOp_Dim:
    case BspOp_ArrayItem:
        pMyObject->PrintData(hThisObject, &pBr->left);
        AnscTrace(" [] ");
        pMyObject->PrintData(hThisObject, &pBr->right);
        break;

    case BspOp_SubStr:
        AnscTrace("%s ", sBspOpList[pBr->op]);
        pMyObject->PrintData(hThisObject, &pBr->left);
        AnscTrace(" , ");
        pMyObject->PrintData(hThisObject, &pBr->right);
        break;

    case BspOp_Name:
        pMyObject->PrintData(hThisObject, &pBr->right);
        pMyObject->PrintData(hThisObject, &pBr->left);
        break;

	case BspOp_Incr: 
    case BspOp_Decr:
    case BspOp_IncrN:
    case BspOp_DecrN:
    case BspOp_MultiplyN:
    case BspOp_DivideN:
    case BspOp_ModuloN:
    case BspOp_BitOrN:
    case BspOp_BitAndN:
    case BspOp_ShiftLeft:
    case BspOp_ShiftRight:
    case BspOp_ShiftLeftN:
    case BspOp_ShiftRightN:
    case BspOp_BitXorN:
	case BspOp_Assign:
	case BspOp_LE: 
    case BspOp_GE: 
    case BspOp_NE: 
    case BspOp_LT: 
    case BspOp_GT: 
    case BspOp_Eq:
	case BspOp_Or: 
    case BspOp_And:
    case BspOp_Not:
    case BspOp_BitOr:
    case BspOp_BitAnd:
    case BspOp_BitXor:
    case BspOp_BitNot:
	case BspOp_Add: 
    case BspOp_Subtract: 
    case BspOp_Multiply: 
    case BspOp_Divide: 
    case BspOp_Modulo: 
    case BspOp_StrCat:
        pMyObject->PrintData(hThisObject, &pBr->left);
        AnscTrace("%s ", sBspOpList[pBr->op]);
        pMyObject->PrintData(hThisObject, &pBr->right);
        break;

    case BspOp_Comma:
    case BspOp_Period:
        pMyObject->PrintData(hThisObject, &pBr->right);
        AnscTrace("%s ", sBspOpList[pBr->op]);
        pMyObject->PrintData(hThisObject, &pBr->left);
        break;

    case BspOp_End:
        pMyObject->PrintData(hThisObject, &pBr->right);
        AnscTrace("\r\n");
        pMyObject->PrintData(hThisObject, &pBr->left);
        break;

    default:
        AnscTrace("unknown operator ");
        break;
    }
}

#endif


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjCompileProcedures
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to compile procedures in the BSP
        script.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjCompileProcedures
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    ANSC_HANDLE                     hToken, hNextToken, hPrevToken;
    PBSP_TEMPLATE_LIST_OBJECT       pList;
    PBSP_TEMPLATE_OBJECT            pBspProc;
    BOOL                            bLoaded;
    PANSC_BYTE_ARRAY_OBJECT         pByteArray, pText;
    char                            *pStream;
    ULONG                           ulStreamSize;

    /* we don't allow nested procedure */
    if (!pMyObject->bVirtual)
    {
        return FALSE;
    }

    hPrevToken      = hNextToken  = (ANSC_HANDLE)NULL;

    pByteArray      = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;
    pStream         = (char *)pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, 0);
    ulStreamSize    = pByteArray->GetSize((ANSC_HANDLE)pByteArray);

    pList           = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;

    while (TRUE)
    {
        hToken  = pMyObject->TakeoutProcedure(hThisObject, &hNextToken, &hPrevToken);

        if (!hToken)
        {
            break;
        }

        /* 
         * we found a BSP procedure --- what we need to do next:
         *  1. create a BSP template object
         *  2. set the token list into the new BSP template
         *  3. copy BSP stream buffer from virtual BSP template
         *  4. build syntax tree of the new BSP template
         *  5. add the new template into BSP template list object
         */

        pBspProc    = (PBSP_TEMPLATE_OBJECT)CreateBspEngComponent(NULL, NULL, NULL);

        if (pBspProc)
        {
            pBspProc->hToken    = hToken;

            pText   = (PANSC_BYTE_ARRAY_OBJECT)pBspProc->hText;
            pText->SetBuffer((ANSC_HANDLE)pText, pStream, ulStreamSize, TRUE);

            pBspProc->SetList((ANSC_HANDLE)pBspProc, pMyObject->hOwnerList);

            if (!pBspProc->Compile((ANSC_HANDLE)pBspProc))
            {
                pBspProc->Remove((ANSC_HANDLE)pBspProc);

                return FALSE;
            }
            else
            {
                BspTemplateEngRemoveToken(hToken);

                bLoaded = pList->IsTemplateLoaded((ANSC_HANDLE)pList, (char *)pBspProc->GetTmplName((ANSC_HANDLE)pBspProc));

                if (bLoaded)
                {
                    return FALSE;
                }
                else
                {
                    pList->AddTemplate((ANSC_HANDLE)pList, (ANSC_HANDLE)pBspProc);
                }
            }
        }
    }

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjTakeoutProcedure
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 *phNextToken,
                ANSC_HANDLE                 *phPrevToken
            )

    description:

        This function is called to take out a procedure from
        BSP script.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 *phNextToken
                'Next' token.

                ANSC_HANDLE                 *phPrevToken
                'Prev' token.

    return:     where a procedure starts.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjTakeoutProcedure
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 *phNextToken,
        ANSC_HANDLE                 *phPrevToken
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    ANSC_HANDLE                     hToken    = (ANSC_HANDLE)NULL;
    PBSP_TEMPLATE_TOKEN             pToken    = NULL;
    PBSP_TEMPLATE_TOKEN             pStart    = NULL;
    PBSP_TEMPLATE_TOKEN             pEnd      = NULL;
    char                            *pStr     = NULL;
    PANSC_BYTE_ARRAY_OBJECT         pByteArray;
    PBSP_TEMPLATE_TOKEN             pPrevToken= NULL;
    PBSP_TEMPLATE_TOKEN             pPrevStart= *(PBSP_TEMPLATE_TOKEN *)phPrevToken;
    PBSP_TEMPLATE_TOKEN             pNext     = NULL;

    pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;

    if (*phNextToken)
    {
        hToken  = *phNextToken;
    }
    else
    {
        hToken  = pMyObject->hToken;
    }

    pToken      = (PBSP_TEMPLATE_TOKEN)hToken;
    pPrevToken  = pPrevStart;

    while (pToken)
    {
        if (!pStart)
        {
            if (pToken->Type == BspToken_eIdent)
            {
                pStr    = (char *)pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, pToken->Value.str);

                if (strcasecmp(pStr, (char *)sBspOpList[BspOp_Proc]) == 0 )
                {
                    pStart      = pToken;
                    pPrevStart  = pPrevToken;
                }
            }
        }
        else
        {
            if (pToken->Type == BspToken_eOp && pToken->Value.op == BspOp_EndProc)
            {
                /* found "endproc" */
                pEnd    = pToken;
                pToken  = pToken->pNext;

                if (pToken->Type == BspToken_eOp && pToken->Value.op == BspOp_End)
                {
                    pEnd    = pToken;
                }

                /* break anyway */
                break;
            }
        }

        pPrevToken  = pToken;
        pToken      = pToken->pNext;
    }

    *phNextToken    = (ANSC_HANDLE)(pEnd?pEnd->pNext:NULL);

    /* take out token list */
    if (pStart)
    {
        pNext       = pEnd?pEnd->pNext:NULL;

        if (!pPrevStart)
        {
            pMyObject->hToken   = (ANSC_HANDLE)pNext;
        }
        else
        {
            pPrevStart->pNext   = pNext;
        }
    }

    if (pEnd)
    {
        pEnd->pNext = NULL;
    }

    if (pPrevStart)
    {
        *phPrevToken    = (ANSC_HANDLE)pPrevStart;
    }
    else
    {
        *phPrevToken    = (ANSC_HANDLE)pNext;
    }

    return (ANSC_HANDLE)pStart;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjCompile
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to compile the BSP script.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of opearation.

**********************************************************************/

BOOL
BspTemplateObjCompile
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    ANSC_HANDLE                     hToken;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr;
    BOOL                            bLoadProc = TRUE;

    hToken  = pMyObject->hToken;

    if (pMyObject->bVirtual)
    {
#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        AnscTrace("Compiling procedures, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds());
    }
#endif

        bLoadProc   = pMyObject->CompileProcedures(hThisObject);

#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        AnscTrace("Procedures compiled, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds());
    }
#endif
    }

    if (!bLoadProc)
    {
        return FALSE;
    }

    if (!pMyObject->hToken)
    {
        return TRUE;
    }

    pMyObject->RemoveSymbols(hThisObject);

    pMyObject->ulNumParams  = 0;
    pMyObject->ulInWhile    = 0;

    hToken  = pMyObject->hToken;

    pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)pMyObject->hTree;
    
    if (pBr)
        BspTemplateBranchCORemove((ANSC_HANDLE)pBr);

#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        AnscTrace("Building syntax tree, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds());
    }
#endif

    pMyObject->hTree    = pMyObject->BuildTree(hThisObject);

#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        if ( pMyObject->hTree != (ANSC_HANDLE)NULL )
        {    AnscTrace("Syntax tree has built, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds()); }
        else
        {    AnscTrace("Syntax error detected or procedure has no statement, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds());}
    }
#endif

    pMyObject->hToken   = hToken;

/*
    return pMyObject->hTree != NULL;
*/

    /* we allow empty procedure */
    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjBuildTree
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build up token tree.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to the root of token tree.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjBuildTree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TOKEN             pToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
    PBSP_TEMPLATE_BRANCH_OBJECT     pSt;
    PANSC_BYTE_ARRAY_OBJECT         pByteArray;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray;
    char                            *pStr;
    ANSC_HANDLE                     hStatementList;

    if (!pMyObject->bVirtual)
    {
        /*
         * we're compiling a BSP procedure, so we need to 
         * process procedure name and parameter list.
         */
        pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;

        if (!pMyObject->IsOp(hThisObject, BspOp_Proc))
        {
            BOOL                        bProc   = FALSE;

            pToken  = pMyObject->IsIdent(hThisObject);

            if (!pToken)
            {
                bProc   = FALSE;
            }
            else
            {
                pStr    = (char *)pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, pToken->Value.str);

                bProc   = (strcasecmp(pStr, (char *)sBspOpList[BspOp_Proc]) == 0 );
            }

            if (!bProc)
            {
                return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "proc is expected",
                                        (ANSC_HANDLE)NULL
                                    );
            }
        }

        pToken  = pMyObject->IsIdent(hThisObject);

        if (!pToken)
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "missing or invalid procedure name",
                                        (ANSC_HANDLE)NULL
                                    );
        }

        pStr        = (char *)pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, pToken->Value.str);
        pMyObject->pName    = (const char *)AnscDupString((PUCHAR)pStr);

        if (pMyObject->IsOp(hThisObject, BspOp_ParenL))
        {
            PBSP_TEMPLATE_SYMBOL_PRO        pSymPro;

            while ((pToken = pMyObject->IsIdent(hThisObject)))
            {
                pSymPro = (PBSP_TEMPLATE_SYMBOL_PRO)AnscAllocateMemory(sizeof(BSP_TEMPLATE_SYMBOL_PRO));

                if (pSymPro)
                {
                    char                *pStr;

                    pStr                = (char *)pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, pToken->Value.str);
                    pSymPro->pName      = (const char*)AnscDupString((PUCHAR)pStr);

                    pMyObject->NewSymbol(hThisObject, pSymPro);
                }

                if (!pMyObject->IsOp(hThisObject, BspOp_Comma))
                    break;
            }

            if (!pMyObject->IsOp(hThisObject, BspOp_ParenR))
            {
                return pMyObject->ParseError
                                        (
                                            hThisObject,
                                            pMyObject->hToken,
                                            "expected ) on parameter list of procedure",
                                            (ANSC_HANDLE)NULL
                                        );
            }

            pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hSymbol;
            pMyObject->ulNumParams  = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);
        }
    }

    pTemp           = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);

    if ( !pTemp )
        return NULL;

#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        AnscTrace("Parsing statement list, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds());
    }
#endif

    hStatementList  = pMyObject->StatementList(hThisObject);
    pSt             = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, hStatementList);

#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        AnscTrace("Statement list created, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds());
    }
#endif

    /* we don't check if pSt is NULL because we allow
     * empty procedure
     */

    if (pMyObject->ulErrLineNo)
    {
        pTemp->Remove((ANSC_HANDLE)pTemp);

        AnscTrace("*** Compilation error: line %lu - %s ***.\n", pMyObject->ulErrLineNo, pMyObject->pErrMsg);
        AnscTrace("*** Note: an error in a proccedure will make it void! ***\r\n");

        return NULL;
    }

    if (!pMyObject->bVirtual)
    {
        if (!pSt)
        {
            /* skip all new lines */
            while (pMyObject->IsOp(hThisObject, BspOp_End));
        }

        /* virtual BSP doesn't need to be terminated by "endproc" */
        if (!pMyObject->IsOp(hThisObject, BspOp_EndProc))
        {
            if (pMyObject->IsOp(hThisObject, BspOp_End))
            {
                return pMyObject->ParseError
                                        (
                                            hThisObject,
                                            pMyObject->hToken,
                                            "procedure end expected",
                                            (ANSC_HANDLE)pTemp
                                        );
            }
            else
            {
                return pMyObject->ParseError
                                        (
                                            hThisObject,
                                            pMyObject->hToken,
                                            "missing procedure end (new line)",
                                            (ANSC_HANDLE)pTemp
                                        );
            }
        }

        if (!pMyObject->IsOp(hThisObject, BspOp_End))
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        pMyObject->hToken,
                                        "statement end expected",
                                        (ANSC_HANDLE)pTemp
                                    );
        }
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return pSt;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjStatementList
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build up statement list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to statement list.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjStatementList
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     pSt, pNext, pPrev, pMain = NULL;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;

    pNext = NULL;

    pTemp = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);

    if ( !pTemp )
        return NULL;

    for (pPrev = NULL; (pSt = pMyObject->Statement(hThisObject)); pPrev = pNext)
    {
        pNext   = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if(!pNext) /*RDKB-6004, CID-24418, NULL check before other operation*/
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        pMyObject->hToken,
                                        "oo resource object creation failed",
                                        (ANSC_HANDLE)pTemp
                                    );
        }
        BspTemplateBranchSetOp((ANSC_HANDLE)pNext, BspOp_End);

        if (!pMain)
            pMain   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pNext);

        BspTemplateBranchSetRight((ANSC_HANDLE)pNext, (ANSC_HANDLE)pSt);

        if (pPrev)
            BspTemplateBranchSetLeft((ANSC_HANDLE)pPrev, (ANSC_HANDLE)pNext);

        if (!pMyObject->IsOp(hThisObject, BspOp_End))
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        pMyObject->hToken,
                                        "statement end expected",
                                        (ANSC_HANDLE)pTemp
                                    );
        }
    }

    if (pMyObject->ulErrLineNo)
    {
        return pMyObject->ParseError
                            (
                                hThisObject,
                                pMyObject->hToken,
                                "error when parsing statement list",
                                (ANSC_HANDLE)pTemp
                            );
    }

    if (pNext)
        BspTemplateBranchSetLeft((ANSC_HANDLE)pNext, (ANSC_HANDLE)NULL);

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return pMain;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjStatement
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build a statement.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to statement.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjStatement
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_BRANCH_OBJECT     pSt;
    ANSC_HANDLE                     hSt;
    const BSP_TEMPLATE_OPERATOR     opList[] = { BspOp_Break, BspOp_Continue, BspOp_Return, BspOp_Exit, BspOp_Term };
    BSP_TEMPLATE_OPERATOR           op;
    PBSP_TEMPLATE_TOKEN             pToken;

    /* remove empty statements */
    while (pMyObject->IsOp(hThisObject, BspOp_End));

    if ((hSt = pMyObject->IncludeStatement(hThisObject)))
        return hSt;

    if ((hSt = pMyObject->IfStatement(hThisObject)))
        return hSt;

    if (pMyObject->ulErrLineNo)
        return NULL;

    if ((hSt = pMyObject->WhileStatement(hThisObject)))
        return hSt;

    if ((hSt = pMyObject->SwitchStatement(hThisObject)))
        return hSt;

    if ((hSt = pMyObject->ReturnStatement(hThisObject)))
        return hSt;

    if (pMyObject->ulErrLineNo)
        return NULL;

#ifdef   _BSP_RICH_FEATURE_SET
    if (hSt = pMyObject->SetOutputStatement(hThisObject))
        return hSt;

    if (pMyObject->ulErrLineNo)
        return NULL;
#endif

    if ((hSt = pMyObject->ArrayDeclaration(hThisObject)))
        return hSt;

    if (pMyObject->ulErrLineNo)
        return NULL;

    if ((hSt = pMyObject->Expression(hThisObject)))
    {
        pSt = (PBSP_TEMPLATE_BRANCH_OBJECT)hSt;

        if (pSt->op == BspOp_Env || pSt->op == BspOp_Obj)
            pSt->bTopOp = TRUE;

        return hSt;
    }

    if ((hSt = pMyObject->ApiCall(hThisObject)))
        return hSt;

    if (pMyObject->ulErrLineNo)
        return NULL;

    if (pMyObject->IsOp(hThisObject, BspOp_Execute))
    {
        PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
        PBSP_TEMPLATE_BRANCH_OBJECT     pExp;

        if (!pMyObject->IsOp(hThisObject, BspOp_ParenL))
        {
            return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "execute lacks (",
                                    (ANSC_HANDLE)NULL
                                );
        }

        pTemp = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
        if ( !pTemp )
            return NULL;
        pExp  = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->Expression(hThisObject));

        if (!pExp)
        {
            return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "execute: invalid expression",
                                    (ANSC_HANDLE)pTemp
                                );
        }

        if (!pMyObject->IsOp(hThisObject, BspOp_ParenR))
        {
            return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "execute lacks )",
                                    (ANSC_HANDLE)pTemp
                                );
        }

        pSt = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( pSt )
        {
            BspTemplateBranchSetOp((ANSC_HANDLE)pSt, BspOp_Execute);
            BspTemplateBranchSetRight((ANSC_HANDLE)pSt, (ANSC_HANDLE)pExp);
        }

        pTemp->Remove((ANSC_HANDLE)pTemp);

        return (ANSC_HANDLE)pSt;
    }

    op = pMyObject->InOpList(hThisObject, opList);
    if (op)
    {
        if ((op == BspOp_Continue || op == BspOp_Break) && !pMyObject->ulInWhile)
        {
            return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "break/continue requires enclosing while",
                                    (ANSC_HANDLE)NULL
                                );
        }

        pSt = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( pSt )
            BspTemplateBranchSetOp((ANSC_HANDLE)pSt, op);

        return (ANSC_HANDLE)pSt;
    }

    pToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->IsText(hThisObject);

    if (pToken)
    {
        PANSC_BYTE_ARRAY_OBJECT     pByteArray;
        char                        *pBuf;

        pSt = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( pSt )
        {
            BspTemplateBranchSetOp((ANSC_HANDLE)pSt, BspOp_TextBlock);
        
            pByteArray = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;

            pSt->right.type     = BspBranch_eString;
            pBuf                = (char *)pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, pToken->Value.str);
            pSt->right.Value.s  = (PCHAR)AnscDupString((PUCHAR)pBuf);

            pSt->left.type      = BspBranch_eNumber;
            pSt->left.Value.n   = pToken->TextLen;
        }

        return (ANSC_HANDLE)pSt;
    }

    return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjIncludeStatement
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build include statement.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to include statement.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjIncludeStatement
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
    PBSP_TEMPLATE_BRANCH_OBJECT     pInclude, pExp;

    if (!pMyObject->IsOp(hThisObject, BspOp_Include))
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

    pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
        return NULL;
    pExp    = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->Expression(hThisObject));

    if (!pExp)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "inc statement expects reference BSP page",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    pInclude = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( pInclude )
    {
        BspTemplateBranchSetOp((ANSC_HANDLE)pInclude, BspOp_Include);
        BspTemplateBranchSetRight((ANSC_HANDLE)pInclude, (ANSC_HANDLE)pExp);
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return pInclude;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjIfStatement
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build if statement.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to if statement.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjIfStatement
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
    PBSP_TEMPLATE_BRANCH_OBJECT     pExp, pList, pElse, pIf;

    if (!pMyObject->IsOp(hThisObject, BspOp_If))
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

    pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
        return NULL;

    pExp    = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->Expression(hThisObject));

    if (!pExp)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "if statement lacks expression",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    if (!pMyObject->IsOp(hThisObject, BspOp_End))
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "')' expected",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    pList   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->StatementList(hThisObject));

    if (!pList && pMyObject->ulErrLineNo)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "statement list parsing error",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    pElse   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->IfElse(hThisObject));

    if (!pElse && pMyObject->ulErrLineNo)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "failed to parse if/else statement block",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    if (!pMyObject->IsOp(hThisObject, BspOp_Endif))
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "endif expected",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    pIf = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( pIf )
    {
        BspTemplateBranchSetOp((ANSC_HANDLE)pIf, BspOp_If);
        BspTemplateBranchSetRight((ANSC_HANDLE)pIf, (ANSC_HANDLE)pExp);
        BspTemplateBranchSetLeft((ANSC_HANDLE)pIf, (ANSC_HANDLE)pList);

        if (pElse)
        {
            BspTemplateBranchSetRight((ANSC_HANDLE)pElse, (ANSC_HANDLE)pIf);

            pTemp->Remove((ANSC_HANDLE)pTemp);

            return pElse;
        }
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return pIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjIfStatement
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build if/else statement.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to if/else statement.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjIfElse
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp       = NULL;
    const BSP_TEMPLATE_OPERATOR     opList[]    = { BspOp_ElseIf, BspOp_Else, BspOp_Term };

    switch (pMyObject->InOpList(hThisObject, opList))
    {
    case BspOp_ElseIf:
        {
            PBSP_TEMPLATE_BRANCH_OBJECT pList, pIf, pElseIf = NULL, pElse, pExp; /*RDKB-6004, CID-24767, initializing pElseIf*/

            pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
            if ( !pTemp )
                return NULL;
            pExp    = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->Expression(hThisObject));

            if (!pExp)
            {
                return pMyObject->ParseError
                                        (
                                            hThisObject,
                                            (ANSC_HANDLE)pOrigToken,
                                            "elseif lacks expression",
                                            (ANSC_HANDLE)pTemp
                                        );
            }

            if (!pMyObject->IsOp(hThisObject, BspOp_End))
            {
                return pMyObject->ParseError
                                        (
                                            hThisObject,
                                            (ANSC_HANDLE)pOrigToken,
                                            "statement end expected",
                                            (ANSC_HANDLE)pTemp
                                        );
            }

            pList   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->StatementList(hThisObject));

            if (!pList && pMyObject->ulErrLineNo)
            {
                return pMyObject->ParseError
                                        (
                                            hThisObject,
                                            (ANSC_HANDLE)pOrigToken,
                                            "statement list parsing error",
                                            (ANSC_HANDLE)pTemp
                                        );
            }

            pElse   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->IfElse(hThisObject));
            if (!pElse && pMyObject->ulErrLineNo)
            {
                return pMyObject->ParseError
                                        (
                                            hThisObject,
                                            (ANSC_HANDLE)pOrigToken,
                                            "statement list parsing error",
                                            (ANSC_HANDLE)pTemp
                                        );
            }

            pIf = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
            if ( pIf )
            {
                BspTemplateBranchSetOp((ANSC_HANDLE)pIf, BspOp_If);

                BspTemplateBranchSetRight((ANSC_HANDLE)pIf, (ANSC_HANDLE)pExp);
                BspTemplateBranchSetLeft((ANSC_HANDLE)pIf, (ANSC_HANDLE)pList);

                pElseIf = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
                if ( pElseIf )
                {
                    BspTemplateBranchSetOp((ANSC_HANDLE)pElseIf, BspOp_Else);

                    /*
                    pElseIf->SetLeft((ANSC_HANDLE)pElseIf, (ANSC_HANDLE)pIf);
                    */

                    if (pElse)
                    {
                        BspTemplateBranchSetLeft((ANSC_HANDLE)pElseIf, (ANSC_HANDLE)pElse);
                        BspTemplateBranchSetRight((ANSC_HANDLE)pElse, (ANSC_HANDLE)pIf);
                    }
                    else
                    {
                        pElse = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
                        if(pElse)/*RDKB-6004, CID-33486, Null check for further action*/
                        {
                            BspTemplateBranchSetOp((ANSC_HANDLE)pElse, BspOp_Else);
                            BspTemplateBranchSetRight((ANSC_HANDLE)pElse, (ANSC_HANDLE)pIf);
                            BspTemplateBranchSetLeft((ANSC_HANDLE)pElseIf, (ANSC_HANDLE)pElse);
                        }
                    }
                }
            }

            pTemp->Remove((ANSC_HANDLE)pTemp);

            if ( !pElseIf )
            {
                if ( pElse )
                {
                    BspTemplateBranchCORemove((ANSC_HANDLE)pElse);
                }

                if ( pIf )
                {
                    BspTemplateBranchCORemove((ANSC_HANDLE)pIf);
                }
            }
            else
            {
                if ( !pElse && pIf )
                {
                    BspTemplateBranchCORemove((ANSC_HANDLE)pIf);
                }
            }


            return pElseIf;
        }
        break;

    case BspOp_Else:
        {
            PBSP_TEMPLATE_BRANCH_OBJECT pList, pElse;

            pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
            if ( !pTemp )
                return NULL;

            if (!pMyObject->IsOp(hThisObject, BspOp_End))
            {
                return pMyObject->ParseError
                                        (
                                            hThisObject,
                                            (ANSC_HANDLE)pOrigToken,
                                            "statement end expected",
                                            (ANSC_HANDLE)pTemp
                                        );
            }

            pList   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->StatementList(hThisObject));

/* We allow no statement (comments are not counted as real statements) in else branch
            if (!pList)
            {
                return pMyObject->ParseError
                                        (
                                            hThisObject,
                                            (ANSC_HANDLE)NULL,
                                            "No statement",
                                            (ANSC_HANDLE)pTemp
                                        );
            }
*/

            pElse   = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
            if ( pElse )
            {
                BspTemplateBranchSetOp((ANSC_HANDLE)pElse, BspOp_Else);
                BspTemplateBranchSetLeft((ANSC_HANDLE)pElse, (ANSC_HANDLE)pList);
            }

            pTemp->Remove((ANSC_HANDLE)pTemp);

            return pElse;
        }
        break;

    default:
            return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjWhileStatement
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build while statement.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to while statement.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjWhileStatement
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp       = NULL;
    PBSP_TEMPLATE_BRANCH_OBJECT     pInit, pExp, pExp2, pList, pWhile, pBr, pBody;

    if (!pMyObject->IsOp(hThisObject, BspOp_While))
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

    if (++pMyObject->ulInWhile > BSP_TEMPLATE_WHILE_NEST_LIMIT)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "nested while statement limit exceded",
                                    (ANSC_HANDLE)NULL
                                );
    }

    pInit   = (PBSP_TEMPLATE_BRANCH_OBJECT)pMyObject->ExpressionList((ANSC_HANDLE)pMyObject);

    if (!pMyObject->IsOp(hThisObject, BspOp_SemiColon))
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "semicolon ';' expected",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
    {
        BspTemplateBranchCORemove((ANSC_HANDLE)pInit);
        return NULL;
    }
    pExp    = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->Expression(hThisObject));

    /* pExp could be NULL */

    pExp2   = NULL;

    if (!pMyObject->IsOp(hThisObject, BspOp_SemiColon))
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "semicolon ';' expected",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    pExp2   = (PBSP_TEMPLATE_BRANCH_OBJECT)pMyObject->ExpressionList((ANSC_HANDLE)pMyObject);
    /* pExp2 could be NULL */

    if (!pMyObject->IsOp(hThisObject, BspOp_End))
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "statement end '\n' expected",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    pList   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->StatementList(hThisObject));

/* We allow empty while loop --- no statement block in the body
    if (!pList && pMyObject->ulErrLineNo)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "no statements in while block",
                                    (ANSC_HANDLE)pTemp
                                );
    }
*/

    if (!pMyObject->IsOp(hThisObject, BspOp_EndWhile))
    {
        if (pMyObject->IsEnd(hThisObject))
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "while lacks endwhile",
                                        (ANSC_HANDLE)pTemp
                                    );
        }
        else
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "unrecognized statement",
                                        (ANSC_HANDLE)pTemp
                                    );
        }
    }

    pMyObject->ulInWhile --;

    pWhile  = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( pWhile )
    {
        BspTemplateBranchSetOp((ANSC_HANDLE)pWhile, BspOp_While);

        pBody   = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if(pBody) /*RDKB-6004, CID-33179, NULL check before working on the object container object*/
        {
            BspTemplateBranchSetOp((ANSC_HANDLE)pBody, BspOp_Iter);
            BspTemplateBranchSetRight((ANSC_HANDLE)pBody, pExp);
            BspTemplateBranchSetLeft((ANSC_HANDLE)pBody, pList);

            BspTemplateBranchSetRight((ANSC_HANDLE)pWhile, pInit);
            BspTemplateBranchSetLeft((ANSC_HANDLE)pWhile, pBody);

            if (pExp2)
            {
                for (pBr = pBody; pBr->left.Value.b; pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)pBr->left.Value.b);

                BspTemplateBranchSetLeft((ANSC_HANDLE)pBr, (ANSC_HANDLE)pExp2);
            }
        }
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    if ( !pWhile )
    {
        BspTemplateBranchCORemove((ANSC_HANDLE)pInit);
        if ( pExp2 )
            BspTemplateBranchCORemove((ANSC_HANDLE)pExp2);
    }

    return pWhile;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjSwitchStatement
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build switch statement.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to while statement.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjSwitchStatement
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp       = NULL;
    PBSP_TEMPLATE_BRANCH_OBJECT     pExp, pSwitch, pCaseExp, pCase, pDefault, pElse;
    PBSP_TEMPLATE_BRANCH_OBJECT*    pCases      = NULL;
    ULONG                           ulNumCases  = 0, ulMaxCases = 16;
    PBSP_TEMPLATE_BRANCH_OBJECT*    pElses      = NULL;
    ULONG                           i;

    if (!pMyObject->IsOp(hThisObject, BspOp_Switch))
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

    pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
    {
        return NULL;
    }

    pExp    = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->Expression(hThisObject));

    if (!pExp)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "switch statement lacks expression",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    pCases = (PBSP_TEMPLATE_BRANCH_OBJECT*)AnscAllocateMemory(sizeof(PBSP_TEMPLATE_BRANCH_OBJECT) * ulMaxCases);

    if (!pCases || !pMyObject->IsOp(hThisObject, BspOp_End))
    {
        /*RDKB-6004, CID-24396, free memory*/
        if(pCases)
        {
            AnscFreeMemory(pCases);
            pCases =NULL;
        }

        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "out of resources",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    while (pMyObject->IsOp(hThisObject, BspOp_Case))
    {
        pCaseExp = (PBSP_TEMPLATE_BRANCH_OBJECT)pMyObject->Expression(hThisObject);

        if (!pCaseExp)
        {
            if (pCases)
            {
                AnscFreeMemory(pCases);
            }
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "case statement lacks expression",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        if (ulNumCases >= ulMaxCases)
        {
            PBSP_TEMPLATE_BRANCH_OBJECT*    pNewCases = NULL;

            pNewCases = (PBSP_TEMPLATE_BRANCH_OBJECT*)AnscAllocateMemory(sizeof(PBSP_TEMPLATE_BRANCH_OBJECT) * ulMaxCases * 2);
            
            if (pNewCases)
            {
                for (i = 0; i < ulMaxCases; i ++)
                {
                    pNewCases[i] = pCases[i];
                }
                ulMaxCases *= 2;

                AnscFreeMemory(pCases);
                pCases = pNewCases;
            }
        }

        if (ulNumCases < ulMaxCases)
        {
            pCase = pCases[ulNumCases] = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        }
        else
        {
            pCase = NULL;
        }

        if (!pCase)
        {
            if (pCases)
            {
                AnscFreeMemory(pCases);
            }
            BspTemplateBranchCORemove((ANSC_HANDLE)pCaseExp);

            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "out of resources when parsing switch-case statement",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        BspTemplateBranchSetOp   (pCase, BspOp_Case);
        BspTemplateBranchSetRight(pCase, pCaseExp);
        BspTemplateBranchSetLeft (pCase, pMyObject->StatementList(hThisObject));

        pTemp->Set((ANSC_HANDLE)pTemp, pCase);

        ulNumCases++;
    }

    pDefault = NULL;

    if (pMyObject->IsOp(hThisObject, BspOp_Default))
    {
        pDefault = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);

        if (!pDefault)
        {
            if (pCases)
            {
                AnscFreeMemory(pCases);
            }

            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "out of resources when parsing switch-default statement",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        pTemp->Set((ANSC_HANDLE)pTemp, pDefault);

        BspTemplateBranchSetOp  (pDefault, BspOp_Default);
        BspTemplateBranchSetLeft(pDefault, pMyObject->StatementList(hThisObject));
    }

    if (!pMyObject->IsOp(hThisObject, BspOp_EndSwitch))
    {
        if (pCases)
        {
            AnscFreeMemory(pCases);
        }

        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "endswitch is expected",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    if (ulNumCases)
    {
        pElses = (PBSP_TEMPLATE_BRANCH_OBJECT*)AnscAllocateMemory(sizeof(PBSP_TEMPLATE_BRANCH_OBJECT) * ulNumCases);

        for (i = 0; i < ulNumCases; i ++)
        {
            pElse = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
            if (!pElse)
            {
                if (pCases)
                {
                    AnscFreeMemory(pCases);
                }
                if(pElses) /*RDKB-6004, CID-33398, freeing the memory*/
                {
                    AnscFreeMemory(pElses);
                }

                return pMyObject->ParseError
                                        (
                                            hThisObject,
                                            (ANSC_HANDLE)pOrigToken,
                                            "out of resource while parsing switch statement",
                                            (ANSC_HANDLE)pTemp
                                        );
            }
            pElses[i] = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pElse);
            BspTemplateBranchSetOp((ANSC_HANDLE)pElse, BspOp_Else);
        }
    }

    pSwitch  = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( pSwitch )
    {
        BspTemplateBranchSetOp((ANSC_HANDLE)pSwitch, BspOp_Switch);

        BspTemplateBranchSetRight((ANSC_HANDLE)pSwitch, pExp);

        if (pElses && pElses[0])
        {
            for (i = ulNumCases; i !=0; i --)
            {
                BspTemplateBranchSetRight(pElses[i-1], pCases[i-1]);

                if (i >= ulNumCases)
                {
                    if (pDefault)
                    {
                        BspTemplateBranchSetLeft(pElses[i-1], pDefault);
                    }
                }
                else
                {
                    BspTemplateBranchSetLeft(pElses[i-1], pElses[i]);
                }
            }

            BspTemplateBranchSetLeft((ANSC_HANDLE)pSwitch, pElses[0]);
        }
        else if (pDefault)
        {
            BspTemplateBranchSetLeft((ANSC_HANDLE)pSwitch, pDefault);
        }
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    if (pElses)
    {
        AnscFreeMemory(pElses);
    }

    if (pCases)
    {
        AnscFreeMemory(pCases);
    }

    return pSwitch;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjExpressionList
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build expression list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to expression list.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjExpressionList
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp       = NULL;
    PBSP_TEMPLATE_BRANCH_OBJECT     pExpList, pExp, pCurExpList, pNewExpList;

    pExpList    = NULL;
    pCurExpList = NULL;

    pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
        return NULL;
    pExp    = 
        (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set
            (
                (ANSC_HANDLE)pTemp, 
                pMyObject->Expression(hThisObject)
            );

    if (!pExp)
    {
        pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
    }
    else
    {
        pExpList    = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( !pExpList )
            return NULL;
        BspTemplateBranchSetOp((ANSC_HANDLE)pExpList, BspOp_Iter);
        BspTemplateBranchSetRight((ANSC_HANDLE)pExpList, pExp);
        pCurExpList = pExpList;

        pExp    = NULL;

        while (pMyObject->IsOp(hThisObject, BspOp_Comma))
        {
            pExp        = 
                (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set
                    (
                        (ANSC_HANDLE)pTemp, 
                        pMyObject->Expression(hThisObject)
                    );

            pNewExpList = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);

            if (!pNewExpList)
            {
                BspTemplateBranchCORemove((ANSC_HANDLE)pExpList);
                pExpList    = NULL;

                pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

                break;
            }
            else
            {
                BspTemplateBranchSetOp((ANSC_HANDLE)pNewExpList, BspOp_Iter);
                BspTemplateBranchSetRight((ANSC_HANDLE)pNewExpList, pExp);

                BspTemplateBranchSetLeft((ANSC_HANDLE)pCurExpList, (ANSC_HANDLE)pNewExpList);
                pCurExpList = pNewExpList;
            }
        }
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return pExpList;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjReturnStatement
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build return statement.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to return statement.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjReturnStatement
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
    PBSP_TEMPLATE_BRANCH_OBJECT     pExp, pReturn;

    if (!pMyObject->IsOp(hThisObject, BspOp_Return))
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

    pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
        return NULL;
    pExp    = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->Expression(hThisObject));

    pReturn = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( pReturn )
    {
        BspTemplateBranchSetOp((ANSC_HANDLE)pReturn, BspOp_Return);
        BspTemplateBranchSetRight((ANSC_HANDLE)pReturn, (ANSC_HANDLE)pExp);
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return pReturn;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjSetoutputStatement
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build setoutput statement.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to setoutput statement.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjSetouputStatement
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
#ifndef  _BSP_RICH_FEATURE_SET

    return (ANSC_HANDLE)NULL;

#else

    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
    PBSP_TEMPLATE_BRANCH_OBJECT     pSet, pName, pList;

    if (!pMyObject->IsOp(hThisObject, BspOp_SetOutput))
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

    pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
        return NULL;

    pName   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->NameExp(hThisObject, FALSE));

    if (!pName)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "setoutput lacks name",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    if (!pMyObject->IsOp(hThisObject, BspOp_End))
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "statement end expected",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    pList   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->StatementList(hThisObject));

    if (!pList && pMyObject->ulErrLineNo)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "freetemp",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    if (!pMyObject->IsOp(hThisObject, BspOp_EndSetOutput))
    {
        if (pMyObject->IsEnd(hThisObject))
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "setoutput lacks endsetoutput",
                                        (ANSC_HANDLE)pTemp
                                    );
        }
        else
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "unrecognized statement",
                                        (ANSC_HANDLE)pTemp
                                    );
        }
    }

    pSet    = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( pSet )
    {
        BspTemplateBranchSetOp((ANSC_HANDLE)pSet, BspOp_SetOutput);

        BspTemplateBranchSetRight((ANSC_HANDLE)pSet, (ANSC_HANDLE)pName);
        BspTemplateBranchSetLeft((ANSC_HANDLE)pSet, (ANSC_HANDLE)pList);
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return pSet;
#endif
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjObjExpression
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build object access expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to object access expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjExpression
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;

    return pMyObject->AssignmentExp(hThisObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjArrayDeclaration
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build array declaration statement.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to array declaration statement.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjArrayDeclaration
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
    PBSP_TEMPLATE_BRANCH_OBJECT     pName, dimBranch[BSP_TEMPLATE_ARRAY_DIMENSION_LIMIT];
    ULONG                           ulDim       = 0;
    PBSP_TEMPLATE_BRANCH_OBJECT     pExp, pDim, pRight=NULL, *pBranches =NULL, pBr; /*RDK- , CID-24763, Initializing pBranches to NULL */
    PBSP_TEMPLATE_TOKEN             pEnd;
    ULONG                           i;

    if (!pMyObject->IsOp(hThisObject, BspOp_Dim))
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

    pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);

    if (!pTemp)
    {
        return (ANSC_HANDLE)NULL;
    }

    pName   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->VariableExp(hThisObject));

    if (!pName)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "array name is expected after dim",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    if (!pMyObject->IsLValue(hThisObject, pName))
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "illegal array name",
                                    (ANSC_HANDLE)pTemp
                                );
    }


    while (TRUE)
    {
        if (!pMyObject->IsOp(hThisObject, BspOp_ArrayL))
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "[ is expected after array name",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        pExp    = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->Expression(hThisObject));

        if (!pExp && ulDim != 0)
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "illegal array dimension",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        if (!pMyObject->IsOp(hThisObject, BspOp_ArrayR))
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "] is expected after array dimension",
                                        (ANSC_HANDLE)pTemp
                                    );
        }


        ulDim   ++;

        if (ulDim > BSP_TEMPLATE_ARRAY_DIMENSION_LIMIT)
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "array has too many dimensions",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        dimBranch[ulDim - 1]    = pExp;

        pEnd    = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;

        if (pMyObject->IsOp(hThisObject, BspOp_End))
        {
            pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pEnd);
            break;
        }
    }

    pDim    = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( pDim )
    {
        BspTemplateBranchSetOp((ANSC_HANDLE)pDim, BspOp_Dim);

        BspTemplateBranchSetLeft((ANSC_HANDLE)pDim, (ANSC_HANDLE)pName);

        pRight  = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);

        if(pRight) /*RDKB-6004, CID-33262, Null check before proceed futher*/
        {
            pBranches   = (PBSP_TEMPLATE_BRANCH_OBJECT *)AnscAllocateMemory(sizeof(PBSP_TEMPLATE_BRANCH_OBJECT) * ulDim);
            if (pBranches)
            {
                for (i = 0; i < ulDim; i ++)
                {
                    pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
                    if (pBr)
                    {
                        BspTemplateBranchSetRight((ANSC_HANDLE)pBr, dimBranch[i]);
                    }
                    pBranches[i] = pBr;
                }
            }

            pRight->right.type              = BspBranch_eArrayDim;
            pRight->right.Value.a.Count     = ulDim;
            pRight->right.Value.a.pBranch   = (void**)pBranches;

            BspTemplateBranchSetRight((ANSC_HANDLE)pDim, (ANSC_HANDLE)pRight);
        }
    }

    if (!pBranches)
    {
        BspTemplateBranchCORemove((ANSC_HANDLE)pDim);
        pDim    = NULL;
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return (ANSC_HANDLE)pDim;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjApiCall
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build API call statement.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to API call statement.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjApiCall
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
    ULONG                           ulParams    = 0;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TOKEN             aName;
    PBSP_TEMPLATE_BRANCH_OBJECT     pExp, pParams[BSP_TEMPLATE_API_PARAM_LIMIT];
    PBSP_TEMPLATE_BRANCH_OBJECT     pApi, pVar, pRight, *pBranches, pBr;
    PANSC_BYTE_ARRAY_OBJECT         pByteArray;
    ULONG                           i;
    char                            *pBuf;

    aName   = (PBSP_TEMPLATE_TOKEN)pMyObject->IsIdent(hThisObject);

    if (!aName)
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

    if (!pMyObject->IsOp(hThisObject, BspOp_ParenL))
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

    pTemp = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
        return NULL;

    while (TRUE)
    {
        pExp    = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pMyObject->Expression(hThisObject));

        if (!pExp)
        {
            pTemp->Release((ANSC_HANDLE)pTemp);
            pTemp->Remove((ANSC_HANDLE)pTemp);

            return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
        }

        pParams[ulParams ++]    = pExp;


        if (pMyObject->IsOp(hThisObject, BspOp_ParenR))
            break;

        if (!pMyObject->IsOp(hThisObject, BspOp_Comma))
        {
            pTemp->Release((ANSC_HANDLE)pTemp);
            pTemp->Remove((ANSC_HANDLE)pTemp);

            return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
        }
    }

    if (pMyObject->IsOp(hThisObject, BspOp_Period))
    {
        pTemp->Release((ANSC_HANDLE)pTemp);
        pTemp->Remove((ANSC_HANDLE)pTemp);

        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
    }

    pApi    = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( pApi )
    {
        BspTemplateBranchSetOp((ANSC_HANDLE)pApi, BspOp_Api);

        pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;

        pVar    = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( pVar )
        {
            pVar->right.type    = BspBranch_eString;
            pBuf                = pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, aName->Value.str);
            pVar->right.Value.s = (PCHAR)AnscDupString((PUCHAR)pBuf);
        }

        pBranches   = 
            (PBSP_TEMPLATE_BRANCH_OBJECT *)AnscAllocateMemory
                (
                    sizeof(PBSP_TEMPLATE_BRANCH_OBJECT) * ulParams
                );

        if (pBranches)
        {
            for (i = 0; i < ulParams; i ++)
            {
                pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
                if (pBr)
                {
                    BspTemplateBranchSetRight((ANSC_HANDLE)pBr, (ANSC_HANDLE)pParams[i]);
                }
                pBranches[i] = pBr;
            }
        }

        pRight  = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( pRight )
        {
            pRight->right.type              = BspBranch_eApiParams;
            pRight->right.Value.a.Count     = ulParams;
            pRight->right.Value.a.pBranch   = (void**)pBranches;
        }
        else /*RDKB-6004, CID-24226, free memory if not used */
        {
            if(pBranches)
            {
                for (i = 0; i < ulParams; i ++)
                {
                    if(!pBranches[i])
                    {
                        BspTemplateBranchCORemove((ANSC_HANDLE)pBranches[i]);
                        pBranches[i] = NULL;
                    }
                }
                AnscFreeMemory(pBranches);
                /* pBranches = NULL;  
                ** [QNS] Do we need to set NULL here? this will remove "pApi" object.
                ** Keeping current implementation as is, freeing memory allocated in case not used.
                */
            }
        }

        BspTemplateBranchSetLeft((ANSC_HANDLE)pApi, (ANSC_HANDLE)pVar);
        BspTemplateBranchSetRight((ANSC_HANDLE)pApi, (ANSC_HANDLE)pRight);

        if (!pBranches)
        {
            BspTemplateBranchCORemove((ANSC_HANDLE)pApi);
            pApi    = NULL;
        }
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return (ANSC_HANDLE)pApi;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjIsLValue
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch
            )

    description:

        This function is called to check if the given branch is
        L-Value or not.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object to be examined.

    return:     TRUE it's L-Value, FALSE otherwise.

**********************************************************************/

BOOL
BspTemplateObjIsLValue
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;

    /* 
     * The following expression could appear at the left side of assignment:
     *
     *  1. variable
     *  2. array variable
     *  3. object
     *  4. "function call" --- it's kind of ambiguous! Actually we want
     *     here is to support setting object's property. Here's an example.
     *          <#
     *              x = Application.GetObj("xxx")
     *              x("MaxUsers") = 10          // it's not a function call!
     *          #>
     */

    return 
        (
            (pBr->op == BspOp_None && pBr->right.type == BspBranch_eVar)        ||
            (pBr->op == BspOp_ArrayItem && pBr->left.type == BspBranch_eVar)    ||
            (pBr->op == BspOp_Obj)                                              ||
            (pBr->op == BspOp_Call)
        );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjAssignmentExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build assignment expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to assignment expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjAssignmentExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp       = NULL;
    PBSP_TEMPLATE_BRANCH_OBJECT     pExp, pVal, pAssign;

    pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
        return NULL;
    pExp    = 
        (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set
            (
                (ANSC_HANDLE)pTemp, 
                pMyObject->LogicalExp(hThisObject)
            );

    if (!pExp)
    {
        pTemp->Release((ANSC_HANDLE)pTemp);
        pTemp->Remove((ANSC_HANDLE)pTemp);
        pTemp   = NULL;

        /* this means to output expression result */
    }

    if (!pMyObject->IsOp(hThisObject, BspOp_Assign))
    {
        if (pTemp)
        {
            pTemp->Remove((ANSC_HANDLE)pTemp);
        }
        
        return (ANSC_HANDLE)pExp;
    }

    if (pExp && !pMyObject->IsLValue(hThisObject, (ANSC_HANDLE)pExp))
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "variable required for assignment",
                                    (ANSC_HANDLE)pTemp
                                );
    }
    else
    {
        if (pExp && (pExp->op == BspOp_Obj || pExp->op == BspOp_Call))
        {
            pExp->bObjSetProperty   = TRUE;
        }
    }

    if (!pTemp)
    {
        pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
        if ( !pTemp )
            return NULL;
    }

    pVal    = 
        (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set
            (
                (ANSC_HANDLE)pTemp, 
                pMyObject->AssignmentExp(hThisObject)
            );

    if (!pVal)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    pMyObject->hToken,
                                    "invalid expression",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    pAssign = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);

    if (pAssign)
    {
        /* set "bObjSetProperty" flag if the left operand is specified as object access expression */
        if (pExp)
        {
            pAssign->bObjSetProperty    = pExp->bObjSetProperty;
        }

        BspTemplateBranchSetOp((ANSC_HANDLE)pAssign, BspOp_Assign);
        BspTemplateBranchSetLeft((ANSC_HANDLE)pAssign, (ANSC_HANDLE)pExp);
        BspTemplateBranchSetRight((ANSC_HANDLE)pAssign, (ANSC_HANDLE)pVal);
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return (ANSC_HANDLE)pAssign;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjLogicalExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build logical expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to logical expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjLogicalExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
	const BSP_TEMPLATE_OPERATOR     opList[] = { BspOp_And, BspOp_Or, BspOp_Term };

    return 
        pMyObject->BinaryExp
            (
                hThisObject, 
                pMyObject->ComparisonExp, 
                (const PBSP_TEMPLATE_OPERATOR)opList
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjComparisonExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build comparison expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to comparison expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjComparisonExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
	const BSP_TEMPLATE_OPERATOR     opList[] = { BspOp_LT, BspOp_GT, BspOp_LE, BspOp_GE, BspOp_Eq, BspOp_NE, BspOp_Term };

    return 
        pMyObject->BinaryExp
            (
                hThisObject, 
                pMyObject->AdditiveExp, 
                (const PBSP_TEMPLATE_OPERATOR)opList
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjAdditiveExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build additive expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to additive expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjAdditiveExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
	const BSP_TEMPLATE_OPERATOR     opList[] = 
    { 
        BspOp_Add, BspOp_Subtract, BspOp_IncrN, BspOp_DecrN, BspOp_MultiplyN, BspOp_DivideN, BspOp_ModuloN,
        BspOp_BitOrN, BspOp_BitAndN, BspOp_ShiftLeft, BspOp_ShiftRight, BspOp_ShiftLeftN, BspOp_ShiftRightN,
        BspOp_BitXorN, BspOp_BitOr, BspOp_BitAnd, BspOp_BitXor, BspOp_Term  
    };

    return 
        pMyObject->BinaryExp
            (
                hThisObject, 
                pMyObject->MultiplicativeExp, 
                (const PBSP_TEMPLATE_OPERATOR)opList
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjMultiplicativeExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build multiplicative expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to multiplicative expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjMultiplicativeExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
	const BSP_TEMPLATE_OPERATOR     opList[] = { BspOp_Multiply, BspOp_Divide, BspOp_Modulo, BspOp_Term };

    return 
        pMyObject->BinaryExp
            (
                hThisObject, 
                pMyObject->StrCatExp, 
                (const PBSP_TEMPLATE_OPERATOR)opList
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjStrCatExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build StrCat expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to StrCat expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjStrCatExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    const BSP_TEMPLATE_OPERATOR     opList[] = { BspOp_StrCat, BspOp_Term };
    
    return 
        pMyObject->BinaryExp
            (
                hThisObject, 
                pMyObject->UnaryExp, 
                (const PBSP_TEMPLATE_OPERATOR)opList
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjBinaryExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build binary expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to binary expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjBinaryExp
    (
        ANSC_HANDLE                  hThisObject,
        PFN_BSPTPO_BINARY_EXP_FUNC   pFunc,
        const PBSP_TEMPLATE_OPERATOR pOpList
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     pExp, pNext;
    PBSP_TEMPLATE_BRANCH_OBJECT     pRight = NULL;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
    BSP_TEMPLATE_OPERATOR           op;

    pExp = (*pFunc)(hThisObject);
    if (!pExp)
        return NULL;

    while ((op = pMyObject->InOpList(hThisObject, pOpList)))
    {
        pRight  = (PBSP_TEMPLATE_BRANCH_OBJECT)(*pFunc)(hThisObject);
        if (!pRight)
            break;

        pNext   = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( !pNext )
        {
            if ( pExp )
            {
                BspTemplateBranchCORemove((ANSC_HANDLE)pExp);
                pExp = NULL;
            }
            goto EXIT;
        }

        BspTemplateBranchSetOp((ANSC_HANDLE)pNext, op);
        BspTemplateBranchSetRight((ANSC_HANDLE)pNext, (ANSC_HANDLE)pRight);
        BspTemplateBranchSetLeft((ANSC_HANDLE)pNext, (ANSC_HANDLE)pExp);
        pExp    = pNext;
    }

    if (op && !pRight)
    {
        pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
        if ( !pTemp )
        {
            BspTemplateBranchCORemove((ANSC_HANDLE)pExp);
            return NULL;
        }

        pTemp->Set((ANSC_HANDLE)pTemp, (ANSC_HANDLE)pExp);

        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    pMyObject->hToken,
                                    "invalid operand",
                                    (ANSC_HANDLE)pTemp
                                );
    }

EXIT:
    return (ANSC_HANDLE)pExp;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjUnaryExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build unary expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to unary expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjUnaryExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     pExp;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
	const BSP_TEMPLATE_OPERATOR     opList[] = 
        { 
            BspOp_Subtract, BspOp_Add, BspOp_IncrN, BspOp_DecrN, BspOp_MultiplyN, BspOp_DivideN, BspOp_ModuloN,
            BspOp_BitOrN, BspOp_BitAndN, BspOp_ShiftLeft, BspOp_ShiftRight, BspOp_ShiftLeftN, BspOp_ShiftRightN,
            BspOp_Not, BspOp_Incr, BspOp_Decr, BspOp_BitNot, BspOp_Term 
        };
    BSP_TEMPLATE_OPERATOR           op;
    PBSP_TEMPLATE_BRANCH_OBJECT     pRight;

    pExp    = pMyObject->PostfixExp(hThisObject);
    if (pExp)
        return (ANSC_HANDLE)pExp;

    if (pMyObject->ulErrLineNo)
        return NULL;

    op  = pMyObject->InOpList(hThisObject, opList);

    if (!op)
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

    pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
    {
        if ( pExp )
            BspTemplateBranchCORemove((ANSC_HANDLE)pExp);

        return NULL;
    }
    pRight  = 
        (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set
            (
                (ANSC_HANDLE)pTemp, 
                pMyObject->UnaryExp(hThisObject)
            );

    if (!pRight)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    pMyObject->hToken,
                                    "invalid operand",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    if (
           (op == BspOp_Decr || op == BspOp_Incr) && 
           !pMyObject->IsLValue(hThisObject, (ANSC_HANDLE)pRight)
       )
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    pMyObject->hToken,
                                    "variable required for ++/--",
                                    (ANSC_HANDLE)pTemp
                                );
    }

    pExp    = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( pExp )
    {
        BspTemplateBranchSetOp((ANSC_HANDLE)pExp, op);

        BspTemplateBranchSetRight((ANSC_HANDLE)pExp, (ANSC_HANDLE)pRight);

        if (op == BspOp_Decr || op == BspOp_Incr)
        {
            pExp->left.type     = BspBranch_eNumber;
            pExp->left.Value.n  = 1;
        }
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return (ANSC_HANDLE)pExp;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjPostfixExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build postfix expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to postfix expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjPostfixExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_BRANCH_OBJECT     pExp, pNext;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
	const BSP_TEMPLATE_OPERATOR     opList[] = { BspOp_Incr, BspOp_Decr, BspOp_Term };
    BSP_TEMPLATE_OPERATOR           op;

    pExp    = pMyObject->ValueExp(hThisObject);
    if (!pExp)
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

    pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
    {
        BspTemplateBranchCORemove((ANSC_HANDLE)pExp);
        return NULL;
    }

    while ((op = pMyObject->InOpList(hThisObject, opList)))
    {
        if ((op == BspOp_Decr || op == BspOp_Incr) && !pMyObject->IsLValue(hThisObject, pExp))
        {
            pTemp->Set((ANSC_HANDLE)pTemp, pExp);
            return pMyObject->ParseError
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pOrigToken,
                                        "variable required for ++/--",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        pNext   = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( !pNext )
        {
            BspTemplateBranchCORemove((ANSC_HANDLE)pExp);
            break;
        }
        BspTemplateBranchSetOp((ANSC_HANDLE)pNext, op);

        BspTemplateBranchSetRight((ANSC_HANDLE)pNext, pExp);

        if (op == BspOp_Decr || op == BspOp_Incr)
        {
            pNext->left.type    = BspBranch_eNumber;
            pNext->left.Value.n = 0;
        }

        pExp    = pNext;
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return (ANSC_HANDLE)pExp;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjValueExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build value expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to value expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjValueExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pToken;
    PBSP_TEMPLATE_BRANCH_OBJECT     pLiteral, pExp;
    PANSC_BYTE_ARRAY_OBJECT         pByteArray;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
    ANSC_HANDLE                     hBr;

    pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;

    pToken = (PBSP_TEMPLATE_TOKEN)pMyObject->IsLiteral(hThisObject);
    
    if (pToken)
    {
        pLiteral = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( !pLiteral )
            return NULL;

        switch (pToken->Type)
        {
        case BspToken_eString:
            {
                char                *pStr;

                pLiteral->right.type    = BspBranch_eString;
                pStr = (char *)pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, pToken->Value.str);
                pLiteral->right.Value.s = (PCHAR)AnscDupString((PUCHAR)pStr);
            }
            break;

        case BspToken_eNumber:
            pLiteral->right.type    = BspBranch_eNumber;
            pLiteral->right.Value.n = pToken->Value.num;
            break;

        case BspToken_eReal:
            pLiteral->right.type    = BspBranch_eReal;
            pLiteral->right.Value.r = pToken->Value.real;
            break;
        default:
            break;
        }

        return pLiteral;
    }

    /*pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;*/

    if (pMyObject->IsOp(hThisObject, BspOp_ParenL))
    {
        pTemp   = CreateBspEngTempComponent(NULL, NULL, NULL);
        if ( !pTemp )
            return NULL;

        pExp    = 
            (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set
                (
                    (ANSC_HANDLE)pTemp, 
                    pMyObject->Expression(hThisObject)
                );

        if (!pExp)
        {
            return pMyObject->ParseError     
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pMyObject->hToken,
                                        "invalid expression",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        if (!pMyObject->IsOp(hThisObject, BspOp_ParenR))
        {
            return pMyObject->ParseError     
                                    (
                                        hThisObject,
                                        (ANSC_HANDLE)pMyObject->hToken,
                                        "expected ) on expression",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        pTemp->Remove((ANSC_HANDLE)pTemp);

        return pExp;
    }

    if (pMyObject->IsOp(hThisObject, BspOp_SubStr))
    {
        PBSP_TEMPLATE_BRANCH_OBJECT pStr, pStart, pChars;
        PBSP_TEMPLATE_BRANCH_OBJECT pPair, pSub;

        if (!pMyObject->IsOp(hThisObject, BspOp_ParenL))
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject, 
                                        pMyObject->hToken,
                                        "expected ( after substr",
                                        NULL
                                    );
        }

        pTemp   = CreateBspEngTempComponent(NULL, NULL, NULL);
        if ( !pTemp )
            return NULL;

        pStr    = 
            (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set
                (
                    (ANSC_HANDLE)pTemp, 
                    pMyObject->Expression(hThisObject)
                );

        if (!pStr)
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject, 
                                        pMyObject->hToken,
                                        "invalid string expression",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        if (!pMyObject->IsOp(hThisObject, BspOp_Comma))
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject, 
                                        pMyObject->hToken,
                                        "expected , after string expression",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        pStart  = 
            (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set
                (
                    (ANSC_HANDLE)pTemp, 
                    pMyObject->Expression(hThisObject)
                );

        if (!pStart)
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject, 
                                        pMyObject->hToken,
                                        "invalid expression",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        if (!pMyObject->IsOp(hThisObject, BspOp_Comma))
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject, 
                                        pMyObject->hToken,
                                        "expected , after string expression",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        pChars  = 
            (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set
                (
                    (ANSC_HANDLE)pTemp, 
                    pMyObject->Expression(hThisObject)
                );

        if (!pChars)
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject, 
                                        pMyObject->hToken,
                                        "invalid expression",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        if (!pMyObject->IsOp(hThisObject, BspOp_ParenR))
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject, 
                                        pMyObject->hToken,
                                        "expected ) after substr",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        pPair   = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( pPair )
        {
            BspTemplateBranchSetOp((ANSC_HANDLE)pPair, BspOp_Comma);
            BspTemplateBranchSetLeft((ANSC_HANDLE)pPair, (ANSC_HANDLE)pStart);
            BspTemplateBranchSetRight((ANSC_HANDLE)pPair, (ANSC_HANDLE)pChars);
        }

        pSub    = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( pSub )
        {
            BspTemplateBranchSetOp((ANSC_HANDLE)pSub, BspOp_SubStr);
            BspTemplateBranchSetLeft((ANSC_HANDLE)pSub, pStr);
            BspTemplateBranchSetRight((ANSC_HANDLE)pSub, pPair);
        }

        pTemp->Remove((ANSC_HANDLE)pTemp);

        return pSub;
    }

    hBr = pMyObject->ProcCallExp(hThisObject);
    if (hBr)
        return hBr;

    hBr = pMyObject->ObjectExp(hThisObject);
    if (hBr)
        return hBr;

    hBr = pMyObject->ArrayExp(hThisObject);
    if (hBr)
        return hBr;

    hBr = pMyObject->ApiExp(hThisObject);
    if (hBr)
        return hBr;

    return pMyObject->VariableExp(hThisObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjArrayExpCondition
            (
                ANSC_HANDLE                 hThisObject,
                BSP_TEMPLATE_OPERATOR       TerminalOp
            )

    description:

        This function is called to build array expression condition.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to array expression condition.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjArrayExpCondition
    (
        ANSC_HANDLE                 hThisObject,
        BSP_TEMPLATE_OPERATOR       TerminalOp
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp       = NULL;
    PBSP_TEMPLATE_BRANCH_OBJECT     pNameExp;
    PBSP_TEMPLATE_TOKEN             pEnd;
    PBSP_TEMPLATE_BRANCH_OBJECT     pExp;
    ULONG                           ulDimCount  = 0;
    PBSP_TEMPLATE_BRANCH_OBJECT     dimBranch[BSP_TEMPLATE_ARRAY_DIMENSION_LIMIT];
    PBSP_TEMPLATE_BRANCH_OBJECT     pArrayItem;
    PBSP_TEMPLATE_BRANCH_OBJECT     *pBranches, pRight, pBr;
    ULONG                           i;

    pTemp       = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
        return NULL;

    pNameExp    = 
        (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set
            (
                (ANSC_HANDLE)pTemp, 
                pMyObject->VariableExp(hThisObject)
            );

    if (!pNameExp)
    {
        pTemp->Release((ANSC_HANDLE)pTemp);
        pTemp->Remove((ANSC_HANDLE)pTemp);

        return pMyObject->NoMatch(hThisObject, pOrigToken);
    }

    while (TRUE)
    {
        if (!pMyObject->IsOp(hThisObject, BspOp_ArrayL))
        {
            if (ulDimCount != 0)
                break;
            else
            {
                pTemp->Release((ANSC_HANDLE)pTemp);
                pTemp->Remove((ANSC_HANDLE)pTemp);

                return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
            }
        }

        pExp    = 
            (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set
                (
                    (ANSC_HANDLE)pTemp, 
                    pMyObject->Expression(hThisObject)
                );

        if (!pExp)
        {
            pTemp->Release((ANSC_HANDLE)pTemp);
            pTemp->Remove((ANSC_HANDLE)pTemp);

            return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
        }

        /*pLast = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;*/

        if (!pMyObject->IsOp(hThisObject, BspOp_ArrayR))
        {
            pTemp->Release((ANSC_HANDLE)pTemp);
            pTemp->Remove((ANSC_HANDLE)pTemp);

            return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
        }

        dimBranch[ulDimCount ++] = pExp;

        pEnd = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;

        if (pMyObject->IsOp(hThisObject, TerminalOp))
        {
            pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pEnd);
            break;
        }
    }

    if (ulDimCount >BSP_TEMPLATE_ARRAY_DIMENSION_LIMIT)
    {
        return pMyObject->ParseError
                               (
                                   hThisObject,
                                   (ANSC_HANDLE)pOrigToken,
                                   "array over max dimensions",
                                   (ANSC_HANDLE)pTemp
                               );
    }

    pArrayItem  = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( pArrayItem )
    {
        BspTemplateBranchSetOp((ANSC_HANDLE)pArrayItem, BspOp_ArrayItem);
        BspTemplateBranchSetLeft((ANSC_HANDLE)pArrayItem, (ANSC_HANDLE)pNameExp);
        pRight      = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if(pRight) /*RDKB-6004, CID-33022  Null check before proceed*/
        {
            pBranches   = 
                (PBSP_TEMPLATE_BRANCH_OBJECT *)
                    AnscAllocateMemory(sizeof(PBSP_TEMPLATE_BRANCH_OBJECT) * ulDimCount);

            if (pBranches)
            {
                for (i = 0; i < ulDimCount; i ++)
                {
                    pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
                    if (pBr)
                    {
                        BspTemplateBranchSetRight((ANSC_HANDLE)pBr, dimBranch[i]);
                    }
                    pBranches[i] = pBr;
                }
            }

            pRight->right.Value.a.pBranch   = (void**)pBranches;
            pRight->right.Value.a.Count     = ulDimCount;
            pRight->right.type              = BspBranch_eArrayDim;

            BspTemplateBranchSetRight((ANSC_HANDLE)pArrayItem, (ANSC_HANDLE)pRight);

            if (!pBranches)
            {
                BspTemplateBranchCORemove((ANSC_HANDLE)pArrayItem);
                pArrayItem  = NULL;
            }
        }
        else
        {
            BspTemplateBranchCORemove((ANSC_HANDLE)pArrayItem);
            pArrayItem  = NULL;

            return pMyObject->ParseError
                    (
                        hThisObject,
                        (ANSC_HANDLE)pOrigToken,
                        "out of resource while building array",
                        (ANSC_HANDLE)pTemp
                    );
        }
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return (ANSC_HANDLE)pArrayItem;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjIsExpProcCall
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hNameExp
            )

    description:

        This function is called to examine if the given expression
        is a procedure call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hNameExp
                Expression to be examined.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjIsExpProcCall
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hNameExp
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PBSP_TEMPLATE_BRANCH_OBJECT     pNameExp        = (PBSP_TEMPLATE_BRANCH_OBJECT)hNameExp;
    ULONG                           ulParamGroups   = 0;
    PBSP_TEMPLATE_BRANCH_OBJECT     aBr;

    if (pNameExp->op != BspOp_Name)
    {
        return FALSE;
    }

    /* 
     * check how many parameter groups it has ---
     * a procedure call must have only one parameter group for the first
     * object
     */

    aBr = pNameExp->right.Value.b;
    while (aBr)
    {
        ulParamGroups ++;
        aBr = aBr->left.Value.b;
    }

    return (ulParamGroups == 1);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjArrayExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build array expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to array expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjArrayExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;

    return pMyObject->ArrayExpCondition(hThisObject, BspOp_End);
}
 

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjApiExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build API expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to API expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjApiExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;

    return pMyObject->ApiCall(hThisObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjObjectExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build object expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to object expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjObjectExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    /* 
     *  To support sth. like list("next").getAddr() or list[i]("next").getAddr()
     */

    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_LIST_OBJECT       pList       = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;
    PBSPENG_SOA_INTERFACE           pBspSoaIf   = pList->GetBspSoaIf((ANSC_HANDLE)pList);
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp;
    PBSP_TEMPLATE_BRANCH_OBJECT     pName=NULL, pNext, pPrev, pMain  = NULL, pObj=NULL, pObjName; /*RDKB-6004, CID-24633, pObj is used without initializing in conditional code flow*/
    PBSP_TEMPLATE_TOKEN             pToken;
    PANSC_BYTE_ARRAY_OBJECT         pByteArray;
    PBSP_TEMPLATE_BRANCH_OBJECT     pObjArray   = NULL;
    PBSP_TEMPLATE_BRANCH_OBJECT     pProp       = NULL;
    PBSP_TEMPLATE_BRANCH_OBJECT     pParam, pPrevProp, pNextProp;
    BOOL                            bParseProp  = FALSE;
    
#ifdef   _BSP_RICH_FEATURE_SET
    if (pMyObject->IsOp(hThisObject, BspOp_Env))
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
#endif

    pToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->IsIdent(hThisObject);

    if (!pToken)
        return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
    else
    {
        char                        *pStr;

        pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;
        pStr    = (char *)pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, pToken->Value.str);

        if (BspTemplateObjGetApiCode(hThisObject, pStr) != BspApi_unknown)
        {
            return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
        }
    }

    pTemp   = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
        return NULL;

    pPrevProp   = NULL;
    pNext       = NULL;

    /* after the object name, the token should be either "." or array dimensions */
    if (!pMyObject->IsOp(hThisObject, BspOp_Period))
    {
        /* 
         * the object is associated with a property, we don't introduce
         * here any restrictions such as how many parameters (properties)
         * allowed. But in practice, if property is specified, the number
         * should be 1. We'll resolve the problem when the statement is
         * interpreted.
         */
        if (pMyObject->IsOp(hThisObject, BspOp_ParenL))
        {
            bParseProp  = TRUE;

            goto PARSE_PROP;
        }
        else
        {
            if (pMyObject->IsOp(hThisObject, BspOp_ArrayL))
            {
                pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);

                pObjArray   = 
                    (PBSP_TEMPLATE_BRANCH_OBJECT)pMyObject->ArrayExpCondition
                        (
                            hThisObject, 
                            BspOp_Period
                        );

                pTemp->Set((ANSC_HANDLE)pTemp, (ANSC_HANDLE)pObjArray);

                if (pMyObject->IsOp(hThisObject, BspOp_ParenL))
                {
                    bParseProp  = TRUE;

                    goto PARSE_PROP;
                }
            }

            if (!pObjArray)
            {
                pTemp->Release((ANSC_HANDLE)pTemp);
                pTemp->Remove((ANSC_HANDLE)pTemp);

                return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
            }
            else
            {
                if (!pMyObject->IsOp(hThisObject, BspOp_Period))
                {
                    pTemp->Release((ANSC_HANDLE)pTemp);
                    pTemp->Remove((ANSC_HANDLE)pTemp);

                    return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
                }
            }
        }
    }

PARSE_PROP:
    if (bParseProp)
    {
        PBSP_TEMPLATE_BRANCH_OBJECT     pNextParam, pPrevParam, pExp;
        BOOL                            bLastOpIsColon      = FALSE;
        ULONG                           ulOpColonCount      = 0;

        pNextParam = NULL;

        pNextProp   = CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( !pNextProp )
        {
            goto EXIT;
        }
        BspTemplateBranchSetOp((ANSC_HANDLE)pNextProp, BspOp_SemiColon);

        if (!pProp)
        {
            pProp   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pNextProp);
        }
    
        pParam  = NULL;

        /* 
         * if the first item is BspOp_Colon ":" in the parameter list, that means
         * all parameters will be output ones. 
         */
        if (pMyObject->IsOp(hThisObject, BspOp_Colon))
        {
            bLastOpIsColon  = TRUE;
            ulOpColonCount  ++;
        }

        for (pPrevParam = NULL; (pExp = pMyObject->Expression(hThisObject)); pPrevParam = pNextParam)
        {
            pNextParam = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
            if ( !pNextParam )
            {
                goto EXIT;
            }
            if (!bLastOpIsColon || ulOpColonCount > 1)
            {
                BspTemplateBranchSetOp((ANSC_HANDLE)pNextParam, BspOp_Comma);
            }
            else
            {
                BspTemplateBranchSetOp((ANSC_HANDLE)pNextParam, BspOp_Colon);
            }

            if (!pParam)
            {
                pParam = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pNextParam);
            }

            BspTemplateBranchSetRight((ANSC_HANDLE)pNextParam, pExp);

            if (pPrevParam)
            {
                BspTemplateBranchSetLeft((ANSC_HANDLE)pPrevParam, pNextParam);
            }

            if (pMyObject->IsOp(hThisObject, BspOp_Colon))
            {
                bLastOpIsColon  = TRUE;
                ulOpColonCount ++;
            }
            else
            if (pMyObject->IsOp(hThisObject, BspOp_Comma))
            {
                bLastOpIsColon  = FALSE;
            }
            else
            {
                break;
            }
        }

        if (!pExp || !pMyObject->IsOp(hThisObject, BspOp_ParenR))
        {
            pTemp->Release((ANSC_HANDLE)pTemp);
            pTemp->Remove((ANSC_HANDLE)pTemp);
            BspTemplateBranchCORemove((ANSC_HANDLE)pNextProp); /*RDKB-6004, CID-33372; releasing handle as not used*/
            return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
        }

        BspTemplateBranchSetRight((ANSC_HANDLE)pNextProp, (ANSC_HANDLE)pParam);

        if (pPrevProp)
        {
            BspTemplateBranchSetLeft((ANSC_HANDLE)pPrevProp, (ANSC_HANDLE)pNextProp);
        }
        
        pPrevProp   = pNextProp;

        if (pMyObject->IsOp(hThisObject, BspOp_ParenL))
        {
            goto PARSE_PROP;
        }

        if (!pMyObject->IsOp(hThisObject, BspOp_Period))
        {
            /*
            pTemp->Release((ANSC_HANDLE)pTemp);
            pTemp->Remove((ANSC_HANDLE)pTemp);

            return pMyObject->NoMatch(hThisObject, (ANSC_HANDLE)pOrigToken);
            */

            goto OBJECT_ONLY;
        }
    }

    for ((pPrev = NULL);( pName = pMyObject->NameExp(hThisObject, FALSE)); pPrev = pNext)
    {
        pNext   = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( !pNext )
        {
            goto EXIT;
        }
        BspTemplateBranchSetOp((ANSC_HANDLE)pNext, BspOp_Period);

        if (!pMain)
            pMain   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pNext);

        BspTemplateBranchSetRight((ANSC_HANDLE)pNext, (ANSC_HANDLE)pName);

        if (pPrev)
            BspTemplateBranchSetLeft((ANSC_HANDLE)pPrev, (ANSC_HANDLE)pNext);

        if (!pMyObject->IsOp(hThisObject, BspOp_Period))
            break;
    }

OBJECT_ONLY:

    if (!pName)
    {
        return pMyObject->ParseError
                                (
                                    hThisObject,
                                    pMyObject->hToken,
                                    "missing or invalid env name",
                                    pTemp
                                );
    }

    if (pNext)
    {
        BspTemplateBranchSetLeft((ANSC_HANDLE)pNext, (ANSC_HANDLE)NULL);
    }

    pObj    = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( !pObj )
    {
        goto EXIT;
    }
    BspTemplateBranchSetOp((ANSC_HANDLE)pObj, BspOp_Obj);

    pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;

    pObjName  = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( !pObjName )
    {
        BspTemplateBranchCORemove((ANSC_HANDLE)pObj);
        pObj = NULL;
        goto EXIT;
    }
    BspTemplateBranchSetOp((ANSC_HANDLE)pObjName, BspOp_Name);

    if (!pObjArray)
    {
        char                        *pStr;
        BOOL                        bAddVarIfNotExist   = FALSE;
        BOOL                        bBuiltInObj         = FALSE;

        /* check object name */
        pStr                = (char *)pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, pToken->Value.str);
        bBuiltInObj         = pBspSoaIf->IsBuiltInObj(pBspSoaIf->hOwnerContext, pStr);
        bAddVarIfNotExist   = !bBuiltInObj && (NULL == pProp);

            pMyObject->FindSymbol
                (
                    hThisObject, 
                    pStr, 
                    bAddVarIfNotExist
                );
        /* save the name, no matter if the name has been added into symbol table */
        pObjName->left.type        = BspBranch_eString;
        pObjName->left.Value.s     = (PCHAR)AnscDupString((PUCHAR)pStr);

        BspTemplateBranchSetLeft((ANSC_HANDLE)pObj, (ANSC_HANDLE)pObjName);
    }
    else
    {
        BspTemplateBranchSetLeft((ANSC_HANDLE)pObjName, (ANSC_HANDLE)pObjArray);
        BspTemplateBranchSetLeft((ANSC_HANDLE)pObj, (ANSC_HANDLE)pObjName);
    }

    BspTemplateBranchSetRight((ANSC_HANDLE)pObjName, (ANSC_HANDLE)pProp);

    BspTemplateBranchSetRight((ANSC_HANDLE)pObj, (ANSC_HANDLE)pMain);

EXIT:
    if ( pTemp )
        pTemp->Remove((ANSC_HANDLE)pTemp);

    return (ANSC_HANDLE)pObj;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjVariableExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build variable expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to variable expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjVariableExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp       = NULL;
    PBSP_TEMPLATE_BRANCH_OBJECT     pParam;
    PBSP_TEMPLATE_BRANCH_OBJECT     pExp;
    PBSP_TEMPLATE_TOKEN             pToken;
    PBSP_TEMPLATE_BRANCH_OBJECT     pVariable;

#ifdef   _BSP_RICH_FEATURE_SET
    PBSP_TEMPLATE_BRANCH_OBJECT     pName, pNext, pPrev, pMain;
    PBSP_TEMPLATE_BRANCH_OBJECT     pEnv;

    /* assuming coming statement is env command */
    if (pMyObject->IsOp(hThisObject, BspOp_Env))
    {
        if (!pMyObject->IsOp(hThisObject, BspOp_Period))
        {
            return pMyObject->ParseError
                                (
                                    hThisObject, 
                                    (ANSC_HANDLE)pOrigToken, 
                                    "expected . after env", 
                                    (ANSC_HANDLE)NULL
                                );
        }
        
        pMain = NULL;

        pTemp = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
        if ( !pTemp )
            return NULL;

        for (pPrev = NULL; pName = pMyObject->NameExp(hThisObject, FALSE); pPrev = pNext)
        {
            pNext   = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
            if ( !pNext )
            {
                goto EXIT;
            }
            BspTemplateBranchSetOp((ANSC_HANDLE)pNext, BspOp_Period);

            if (!pMain)
                pMain   = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pNext);

            BspTemplateBranchSetRight((ANSC_HANDLE)pNext, pName);

            if (pPrev)
                BspTemplateBranchSetLeft((ANSC_HANDLE)pPrev, pNext);

            if (!pMyObject->IsOp(hThisObject, BspOp_Period))
                break;
        }

        if (!pName)
        {
            return pMyObject->ParseError
                                (
                                    hThisObject,
                                    pMyObject->hToken,
                                    "missing or invalid env name",
                                    (ANSC_HANDLE)pTemp
                                );
        }

        BspTemplateBranchSetLeft((ANSC_HANDLE)pNext, NULL);

        pEnv = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( pEnv )
        {
            BspTemplateBranchSetOp((ANSC_HANDLE)pEnv, BspOp_Env);
            BspTemplateBranchSetRight((ANSC_HANDLE)pEnv, pMain);
        }

EXIT:
        pTemp->Remove((ANSC_HANDLE)pTemp);

        return (ANSC_HANDLE)pEnv;
    }
#endif

    /* assuming coming statement is parameter */
    if (pMyObject->IsOp(hThisObject, BspOp_Param))
    {
        if (!pMyObject->IsOp(hThisObject, BspOp_ArrayL))
        {
            return pMyObject->ParseError
                                (
                                    hThisObject,
                                    (ANSC_HANDLE)pOrigToken,
                                    "expected [ after param",
                                    (ANSC_HANDLE)NULL
                                );
        }

        pTemp = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
        /*RDKB-6004, CID-24408, return if Creation of new object container object*/
        if ( !pTemp )
        {
            return NULL;
        }

        pExp  = 
            (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set
                (
                    (ANSC_HANDLE)pTemp, 
                    pMyObject->Expression(hThisObject)
                );

        if (!pExp)
        {
            ANSC_HANDLE             hReturn;

            hReturn = pMyObject->ParseError
                                    (
                                        hThisObject, 
                                        (ANSC_HANDLE)pMyObject->hToken,
                                        "invalid param expression",
                                        (ANSC_HANDLE)pTemp
                                    );

            return hReturn;
        }

        if (!pMyObject->IsOp(hThisObject, BspOp_ArrayR))
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject, 
                                        (ANSC_HANDLE)pOrigToken,
                                        "expected ] on param expression",
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        pParam = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if (pParam )
        {
            BspTemplateBranchSetOp((ANSC_HANDLE)pParam, BspOp_Param);
            BspTemplateBranchSetRight((ANSC_HANDLE)pParam, pExp);
        }

        pTemp->Remove((ANSC_HANDLE)pTemp);

        return pParam;
    }

    /* trying variable name */
    pToken = (PBSP_TEMPLATE_TOKEN)pMyObject->IsIdent(hThisObject);

    if (pToken)
    {
        ULONG                       ulVar;
        const char                  *pVarName;
        PANSC_BYTE_ARRAY_OBJECT     pByteArray;

        pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;
        pVariable   = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);

        if ( pVariable )
        {
            pVarName    = (const char *)pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, pToken->Value.str);

            pVariable->right.type       = BspBranch_eVar;
            ulVar   = 
                pMyObject->FindSymbol
                    (
                        hThisObject, 
                        pVarName, 
                        TRUE
                    );
            pVariable->right.Value.n    = ulVar; 
        }

        return pVariable;
    }

    return pMyObject->NoMatch(hThisObject, pOrigToken);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjProcCallExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build procedure call expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to procedure call expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjProcCallExp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp       = NULL;
    PBSP_TEMPLATE_BRANCH_OBJECT     pName;
    PBSP_TEMPLATE_BRANCH_OBJECT     pSt;
    BOOL                            bCallExist  = FALSE;
    BOOL                            bReserved   = FALSE;

    if (pMyObject->IsOp(hThisObject, BspOp_Call))
    {
        bCallExist  = TRUE;
    }

    pTemp = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
    if ( !pTemp )
        return NULL;

    pName   = (PBSP_TEMPLATE_BRANCH_OBJECT)pMyObject->NameExp(hThisObject, TRUE);

    pTemp->Set((ANSC_HANDLE)pTemp, (ANSC_HANDLE)pName);

    if (!pName || !pMyObject->IsExpProcCall(hThisObject, pName))
    {
        pTemp->Release((ANSC_HANDLE)pTemp);
        pTemp->Remove((ANSC_HANDLE)pTemp);

        return pMyObject->NoMatch(hThisObject, pOrigToken);
    }

    if (pMyObject->IsOp(hThisObject, BspOp_Period))
    {
        if (!bCallExist)
        {
            pTemp->Release((ANSC_HANDLE)pTemp);
            pTemp->Remove((ANSC_HANDLE)pTemp);

            return pMyObject->NoMatch(hThisObject, pOrigToken);
        }
        else
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject, 
                                        (ANSC_HANDLE)pOrigToken, 
                                        "unsupported object access format after procedure call", 
                                        (ANSC_HANDLE)pTemp
                                    );
        }
    }

    /* check if the procedure name is in reserved list */
    /* reserved list includes "execute", and all supported APIs */
    if (!bCallExist)
    {
        PBSP_TEMPLATE_BRANCH_DATA   pLeft;
        char                        *pProcName;
        ULONG                       i, ulSize;

        pLeft       = &pName->left;
        pProcName   = pLeft->Value.s;

        if (pMyObject->GetApiCode(hThisObject, pProcName) != BspApi_unknown)
        {
            pTemp->Release((ANSC_HANDLE)pTemp);
            pTemp->Remove((ANSC_HANDLE)pTemp);

            return pMyObject->NoMatch(hThisObject, pOrigToken);
        }

        ulSize      = sizeof(BspReservedProcNames)/sizeof(BspReservedProcNames[0]);

        for (i = 0; i < ulSize; i ++)
        {
            if (strcasecmp(pProcName, (char *)BspReservedProcNames[i]) == 0 )
            {
                bReserved   = TRUE;

                break;
            }
        }

        if (bReserved)
        {
            pTemp->Release((ANSC_HANDLE)pTemp);
            pTemp->Remove((ANSC_HANDLE)pTemp);

            return pMyObject->NoMatch(hThisObject, pOrigToken);
        }
    }

    pSt = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( pSt )
    {
        BspTemplateBranchSetOp((ANSC_HANDLE)pSt, BspOp_Call);
        BspTemplateBranchSetRight((ANSC_HANDLE)pSt, (ANSC_HANDLE)pName);
    }

    pTemp->Remove((ANSC_HANDLE)pTemp);

    return (ANSC_HANDLE)pSt;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjNameExp
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bWithParams
            )

    description:

        This function is called to build name expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bWithParams

    return:     handle to name expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjNameExp
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bWithParams
    )
{
    /*
     * Note:
     * 
     *   what we call an expression "Name Expression", we mean the expression
     * may have the following format.
     *
     *      name(p11, p12, ..., p1n) *(p21, p22, ..., p2n)
     *
     *  For examples:
     *
     *      getProperty("next")
     *      getProperty("next")("size")
     *      getItem("name", 1)("size")
     */

    /*
     *  Internal representation will look like this.
     *
     *                  BspOp_Name
     *                    /    \
     *                   /      \
     *                  /        \
     *                 /          \
     *               Name      BspOp_SemiColon
     *                             /         \
     *                            /           \
     *                           /             \
     *                  BspOp_SemiColon      BspOp_Comma
     *                      /                   /     \       
     *                     /                   /       \
     *                    /               BspOp_Comma  Parameter 1
     *                  ...                  ...   \
     *                  /                           \
     *            BspOp_Semicolon               Parameter 2
     *                      \
     *                       \
     *                    BspOp_Comma
     *                       /     \
     *                     ...     ...
     */

    PBSP_TEMPLATE_OBJECT            pMyObject       = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pOrigToken      = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TOKEN             pIdent;
    PBSP_TEMPLATE_BRANCH_OBJECT     pMain, pExp, pNext, pPrev, pParamList, pNextList, pPrevList;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp           = NULL;
    PBSP_TEMPLATE_BRANCH_OBJECT     pNameExp = NULL; /*RDKB-6004, CID-24638, initlizing the variable*/
    PANSC_BYTE_ARRAY_OBJECT         pByteArray      = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;
    char                            *pBuf;
    BOOL                            bSucc           = FALSE;
    BOOL                            bLastOpIsColon  = FALSE;
    ULONG                           ulOpColonCount  = 0;

    pIdent = (PBSP_TEMPLATE_TOKEN)pMyObject->IsIdent(hThisObject);

    if (!pIdent)
        return pMyObject->NoMatch(hThisObject, pOrigToken);

    pParamList  = pPrevList = NULL;
    pOrigToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;

PARAM_AGAIN:
    pMain = NULL;

    if (!pMyObject->IsOp(hThisObject, BspOp_ParenL))
    {
        if (bWithParams && !bSucc)
        {
            return pMyObject->NoMatch(hThisObject, pOrigToken);
        }
    }
    else
    {
        /* 
         * if the first item is BspOp_Colon ":" in the parameter list, that means
         * all parameters will be output ones. 
         */
        if (pMyObject->IsOp(hThisObject, BspOp_Colon))
        {
            bLastOpIsColon  = TRUE;
            ulOpColonCount  ++;
        }

        pNext = NULL;
        pTemp = (PBSP_TEMPLATE_TEMP_OBJECT)CreateBspEngTempComponent(NULL, NULL, NULL);
        if ( !pTemp )
            return NULL;

        for (pPrev = NULL; (pExp = pMyObject->Expression(hThisObject)); pPrev = pNext)
        {
            pNext   = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
            if ( !pNext )
            {
                goto EXIT;
            }

            if (!bLastOpIsColon || ulOpColonCount > 1)
            {
                BspTemplateBranchSetOp((ANSC_HANDLE)pNext, BspOp_Comma);
            }
            else
            {
                BspTemplateBranchSetOp((ANSC_HANDLE)pNext, BspOp_Colon);
            }

            if (!pMain)
            {
                pMain = (PBSP_TEMPLATE_BRANCH_OBJECT)pTemp->Set((ANSC_HANDLE)pTemp, pNext);
            }

            BspTemplateBranchSetRight((ANSC_HANDLE)pNext, pExp);

            if (pPrev)
            {
                BspTemplateBranchSetLeft((ANSC_HANDLE)pPrev, pNext);
            }

            if (pMyObject->IsOp(hThisObject, BspOp_Colon))
            {
                bLastOpIsColon  = TRUE;
                ulOpColonCount ++;
            }
            else
            if (pMyObject->IsOp(hThisObject, BspOp_Comma))
            {
                bLastOpIsColon  = FALSE;
            }
            else
            {
                break;
            }
        }

        /*  Allow empty parameter list */

        if (!pMyObject->IsOp(hThisObject, BspOp_ParenR))
        {
            return pMyObject->ParseError
                                    (
                                        hThisObject, 
                                        (ANSC_HANDLE)pOrigToken, 
                                        "expected ) on parameter list", 
                                        (ANSC_HANDLE)pTemp
                                    );
        }

        if (pNext)
        {
            BspTemplateBranchSetLeft((ANSC_HANDLE)pNext, NULL);
        }

        bSucc   = TRUE;

        pNextList   = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
        if ( !pNextList )
        {
            goto EXIT;
        }

        BspTemplateBranchSetOp((ANSC_HANDLE)pNextList, BspOp_SemiColon);

        BspTemplateBranchSetRight((ANSC_HANDLE)pNextList, (ANSC_HANDLE)pMain);
        BspTemplateBranchSetLeft((ANSC_HANDLE)pNextList, (ANSC_HANDLE)NULL);

        if (!pParamList)
        {
            pParamList  = pTemp->Set((ANSC_HANDLE)pTemp, pNextList);
        }
        
        if (pPrevList)
        {
            BspTemplateBranchSetLeft((ANSC_HANDLE)pPrevList, (ANSC_HANDLE)pNextList);
        }

        pPrevList   = pNextList;

        goto PARAM_AGAIN;
    }

    pNameExp    = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( pNameExp )
    {
        BspTemplateBranchSetOp((ANSC_HANDLE)pNameExp, BspOp_Name);

        pNameExp->left.type     = BspBranch_eString;
        pBuf                    = (char *)pByteArray->GetBuffer((ANSC_HANDLE)pByteArray, pIdent->Value.str);
        pNameExp->left.Value.s  = (PCHAR)AnscDupString((PUCHAR)pBuf);
        BspTemplateBranchSetRight((ANSC_HANDLE)pNameExp, (ANSC_HANDLE)pParamList);
    }

EXIT:
    if (pTemp)
    {
        pTemp->Remove((ANSC_HANDLE)pTemp);
    }

    return pNameExp;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjIsText
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build free text block expression.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to free text block expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjIsText
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pToken    = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TOKEN             pRet;

    if (!pToken)
        return NULL;

    if (pToken->Type != BspToken_eText)
       return NULL;

    pRet = pToken;
    pMyObject->hToken = (ANSC_HANDLE)pToken->pNext;

    return pRet;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BSP_TEMPLATE_OPERATOR
        BspTemplateObjIsOp
            (
                ANSC_HANDLE                 hThisObject,
                BSP_TEMPLATE_OPERATOR       op
            )

    description:

        This function is called to check if the next token is
        the same as given one.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BSP_TEMPLATE_OPERATOR       op
                operator to be examined.

    return:     operator.

**********************************************************************/

BSP_TEMPLATE_OPERATOR
BspTemplateObjIsOp
    (
        ANSC_HANDLE                 hThisObject,
        BSP_TEMPLATE_OPERATOR       op
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pToken    = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;

    if (!pToken)
        return BspOp_None;
    
    if (pToken->Type == BspToken_eOp && pToken->Value.op == op)
    {
        pMyObject->hToken   = (ANSC_HANDLE)pToken->pNext;
        
        return op;
    }

    return BspOp_None;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BSP_TEMPLATE_OPERATOR
        BspTemplateObjInOpList
            (
                ANSC_HANDLE                 hThisObject,
                const BSP_TEMPLATE_OPERATOR Ops[]
            )

    description:

        This function is called to see if the next token
        is in given operator array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const BSP_TEMPLATE_OPERATOR Ops[]
                The operator array.

    return:     handle to object expression.

**********************************************************************/

BSP_TEMPLATE_OPERATOR
BspTemplateObjInOpList
    (
        ANSC_HANDLE                 hThisObject,
        const BSP_TEMPLATE_OPERATOR Ops[]
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pToken    = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    BSP_TEMPLATE_OPERATOR           op;
    ULONG                           i;

    if (!pToken || pToken->Type != BspToken_eOp)
        return BspOp_None;

    op = pToken->Value.op;

    for (i = 0; Ops[i] != BspOp_Term; i ++)
    {
        if (op == Ops[i])
            break;
    }

    if (Ops[i] != BspOp_Term)
    {
        pMyObject->hToken   = (ANSC_HANDLE)pToken->pNext;
        return op;
    }

    return BspOp_None;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjIsIdent
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build identifier.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to identifier.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjIsIdent
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pToken    = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TOKEN             pRet;

    if (!pToken)
        return NULL;

    if (pToken->Type != BspToken_eIdent)
       return NULL;

    pRet = pToken;
    pMyObject->hToken = (ANSC_HANDLE)pToken->pNext;

    return pRet;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjIsLiteral
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to build literal string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to starting token.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjIsLiteral
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TOKEN             pToken    = (PBSP_TEMPLATE_TOKEN)pMyObject->hToken;
    PBSP_TEMPLATE_TOKEN             pRet;

    if (!pToken)
        return NULL;

    if (
           pToken->Type != BspToken_eString   &&
           pToken->Type != BspToken_eReal     &&
           pToken->Type != BspToken_eNumber
       )
       return NULL;

    pRet = pToken;
    pMyObject->hToken = (ANSC_HANDLE)pToken->pNext;

    return pRet;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjIsEnd
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to see if no token available.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjIsEnd
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;

    return !pMyObject->hToken;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjParseError
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOrigToken,
                const char                  *pMsg,
                ANSC_HANDLE                 hTemp
            )

    description:

        This function is called when a parsing error encounted.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOrigToken
                Where error occurred.

                const char                  *pMsg
                Error message.

                ANSC_HANDLE                 hTemp
                Temp object to be released.

    return:     handle to object expression.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjParseError
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOrigToken,
        const char                  *pMsg,
        ANSC_HANDLE                 hTemp
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_TEMP_OBJECT       pTemp       = (PBSP_TEMPLATE_TEMP_OBJECT)hTemp;
    PBSP_TEMPLATE_TOKEN             pToken      = (PBSP_TEMPLATE_TOKEN)hOrigToken;
    PBSP_TEMPLATE_TOKEN             pPrevToken  = (PBSP_TEMPLATE_TOKEN)pMyObject->hPrevToken;

    if (pTemp)
    {
        pTemp->Release((ANSC_HANDLE)pTemp);
        pTemp->Remove((ANSC_HANDLE)pTemp);
    }

    if (!pMyObject->ulErrLineNo)
    {
        pMyObject->ulErrLineNo  = pToken ? pToken->LineNo : pPrevToken->LineNo;
        pMyObject->pErrMsg      = pMsg;
    }

    return NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjNoMatch
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOrigToken
            )

    description:

        This function is called to restore to original token.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     NULL.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjNoMatch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOrigToken
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;

    pMyObject->hToken   = hOrigToken;

    return NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BspTemplateObjnewSymbol
            (
                ANSC_HANDLE                 hThisObject,
                PBSP_TEMPLATE_SYMBOL_PRO    pSymbolPro
            )

    description:

        This function is called to add the given symbol in
        symbol list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PBSP_TEMPLATE_SYMBOL_PRO    pSymbolPro
                Symbol to be added.

    return:     the variable ID allocated for the given symbol.

**********************************************************************/

ULONG
BspTemplateObjNewSymbol
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_SYMBOL_PRO    pSymbolPro
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray;
    ULONG                           ulSize;

    pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hSymbol;
    pPtrArray->Add((ANSC_HANDLE)pPtrArray, pSymbolPro);
    ulSize      = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);

    return ulSize - 1;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BspTemplateObjObjectExp
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pName,
                BOOL                        bAddSymbolIfNotFound
            )

    description:

        This function is called to find given symbol by name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pName
                Symbol name.

                BOOL                        bAddSymbolIfNotFound
                Indication whether or not to add the symbol into
                symbol table if it cannot be found.

    return:     variable ID.

**********************************************************************/

ULONG
BspTemplateObjFindSymbol
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        BOOL                        bAddSymbolIfNotFound
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray;
    ULONG                           ulSize, i;
    PBSP_TEMPLATE_SYMBOL_PRO        pSymPro;

    pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hSymbol;
    ulSize      = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);

    for (i = 0; i < ulSize; i ++)
    {
        pSymPro = (PBSP_TEMPLATE_SYMBOL_PRO)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i);

        if (strcmp((char *)pName, (char *)pSymPro->pName) == 0 )
        {
#if 0   /* 11/05/2003, dynamic array support */
            if (pArrayPro)
            {
                *pArrayPro  = pSymPro->pDimSize;
            }
#endif

            return i;
        }
    }

    if (bAddSymbolIfNotFound)
    {
        pSymPro = (PBSP_TEMPLATE_SYMBOL_PRO)AnscAllocateMemory(sizeof(BSP_TEMPLATE_SYMBOL_PRO));
        if (pSymPro)
        {
            /* 
             * make a copy of variable name because
             * the memory reallocation might change
             * the address of mText buffer.
             */
            pSymPro->pName  = (PCHAR)AnscDupString((PUCHAR)pName);

            return pMyObject->NewSymbol(hThisObject, pSymPro);
        }
    }

    return (ULONG)-1;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjObjectExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to remove all symbols.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateObjRemoveSymbols
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray;
    ULONG                           ulSize, i;
    PBSP_TEMPLATE_SYMBOL_PRO        pSymPro;

    pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hSymbol;
    ulSize      = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);

    for (i = 0; i < ulSize; i ++)
    {
        pSymPro = (PBSP_TEMPLATE_SYMBOL_PRO)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i);

        if (pSymPro->pName)
            AnscFreeMemory((char *)pSymPro->pName);

        AnscFreeMemory(pSymPro);
    }

    pPtrArray->RemoveAll((ANSC_HANDLE)pPtrArray);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoTemplate
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRuntime,
                BOOL                        *pbTerminated
            )

    description:

        This function is called to execute a template.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRuntime
                Runtime environment.

                BOOL                        *pbTerminated
                Specifies if BSP engine should terminate current
                execution and returns control flow to caller.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoTemplate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        BOOL                        *pbTerminated
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_STACK_OBJECT      pStack    = (PBSP_TEMPLATE_STACK_OBJECT)pRt->hStack;
    LONG                            nLoops;
    BOOL                            bExecute;
    ULONG                           ulArgCount;
    BOOL                            bTerminated = FALSE;

    if (pMyObject->ulErrLineNo)
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: BSP ");
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, pMyObject->pName);
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, ", line ");
        pMyObject->OutputNumber((ANSC_HANDLE)pMyObject, pMyObject->ulErrLineNo, FALSE);
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, ": ");
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, pMyObject->pErrMsg);

		return;
    }

    if (pRt->ulNestCount > BSP_TEMPLATE_RECURSIVE_LIMIT)
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: too many nested procedure calls ");
        pRt->Stop = BspOp_Exit;
        
        return;
    }

    pRt->ulNestCount ++;

    nLoops      = pRt->LoopLimit;
    bExecute    = pRt->InExecute;
    ulArgCount  = pRt->ulArgCount;
    pRt->ulArgCount = pStack->GetCount((ANSC_HANDLE)pStack);

    if (pRt->ulArgCount < pMyObject->ulNumParams)
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: BSP ");
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, pMyObject->pName);
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, " call missing params.\r\n");
    }
    else
    {
        PBSP_TEMPLATE_OBJECT        pTmpl;
        PANSC_PTR_ARRAY_OBJECT      pSymbolArray, pExecuteArray;
        ULONG                       ulSymbols, ulExecute;

        pSymbolArray = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hSymbol;
        ulSymbols    = pSymbolArray->GetSize((ANSC_HANDLE)pSymbolArray);  

        pStack->Add((ANSC_HANDLE)pStack, ulSymbols - pMyObject->ulNumParams);

        pMyObject->DoStatementList(hThisObject, pMyObject->hTree, hRuntime, FALSE, &bTerminated);

        /* if the operator is BspOp_Return, we reset the stop flag to continue execution */
        if (pRt->Stop == BspOp_Return)
            pRt->Stop = 0;

        pExecuteArray = (PANSC_PTR_ARRAY_OBJECT)pRt->hExecute;
        ulExecute     = pExecuteArray->GetSize((ANSC_HANDLE)pExecuteArray);

        if (ulExecute != 0)
        {
            pTmpl = 
                (PBSP_TEMPLATE_OBJECT)pExecuteArray->GetAt
                    (
                        (ANSC_HANDLE)pExecuteArray, 
                        ulExecute - 1
                    );

            if (pTmpl && pTmpl->pName == pMyObject->pName)
            {
                pTmpl->Remove((ANSC_HANDLE)pTmpl);
                pExecuteArray->RemoveAt((ANSC_HANDLE)pExecuteArray, ulExecute - 1, 1);
            }
        }
    }

    pRt->ulArgCount = ulArgCount;
    pRt->InExecute  = bExecute;
    pRt->LoopLimit  = nLoops;
    pRt->ulNestCount --;

    if (pbTerminated)
    {
        *pbTerminated   = bTerminated;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoStatementList
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                BOOL                        BIter,
                BOOL                        *pbTerminated
            )

    description:

        This function is called to execute statement list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Where to execute.

                ANSC_HANDLE                 hRuntime
                Runtime environment object.

                BOOL                        BIter
                Unused.

                BOOL                        *pbTerminated
                Execution termination flag.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoStatementList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        BOOL                        BIter,
        BOOL                        *pbTerminated
    )
{
    UNREFERENCED_PARAMETER(BIter);
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_LIST_OBJECT       pList       = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;
    PBSPENG_SOA_INTERFACE           pBspSoaIf   = pList->GetBspSoaIf((ANSC_HANDLE)pList);
    PBSP_TEMPLATE_BRANCH_OBJECT     pBranch     = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr;
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt;
    BOOL                            bTerminated = FALSE;

    pRt     = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;

    for (pBr = pBranch; pBr; pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)pBr->left.Value.b)
    {
        if (pRt->Stop)
        {
            return;
        }

        pVal = pMyObject->EvalExpression(hThisObject, &pBr->right, hRuntime, &bTerminated);

        /*  
         * BSP engine doesn't behave like template engine here. 
         * Nothing will be output unless new syntax <# = expression #> is used.
         * Or BSP response's write method is invoked.
         */
        /* pVal->Remove((ANSC_HANDLE)pVal); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);

        if (bTerminated)
        {
            pRt->Stop   = BspOp_Exit;

            break;
        }

        if ( pBspSoaIf->IsInterrupted && pBspSoaIf->IsInterrupted(pBspSoaIf->hOwnerContext) )
        {
            pRt->Stop   = BspOp_Exit;

            break;
        }
    }

    if (pbTerminated)
    {
        *pbTerminated   = bTerminated;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoBranch
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar,
                BOOL                        *pbTerminated
            )

    description:

        This function is called to do a branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime environment object.

                ANSC_HANDLE                 hVar
                Used to contained returned value

                BOOL                        *pbTerminated
                Execution termination flag.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoBranch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        *pbTerminated
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr         = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_VAR_OBJECT        aResult     = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bTerminated = FALSE;

    if (!iBr)
        return;

    switch (iBr->op)
    {
	case BspOp_TextBlock: 
        BspTemplateObjDoTextblock(hThisObject, hBranch, hRuntime, hVar);
        break;

	case BspOp_If:
        BspTemplateObjDoIf(hThisObject, hBranch, hRuntime, hVar);
        break;

	case BspOp_Else: 
        BspTemplateObjDoElse(hThisObject, hBranch, hRuntime, hVar);
        break;

	case BspOp_While: 
        BspTemplateObjDoWhile(hThisObject, hBranch, hRuntime, hVar);
        break;

	case BspOp_Switch: 
        BspTemplateObjDoSwitch(hThisObject, hBranch, hRuntime, hVar);
        break;

	case BspOp_Break:
	case BspOp_Continue:
	case BspOp_Return:
	case BspOp_Exit:
        BspTemplateObjDoControl(hThisObject, hBranch, hRuntime, hVar);
		break;

#ifdef   _BSP_RICH_FEATURE_SET
	case BspOp_SetOutput: 
        BspTemplateObjDoSetoutput(hThisObject, hBranch, hRuntime, hVar);
        break;

    case BspOp_Env: 
        BspTemplateObjDoEnv(hThisObject, hBranch, hRuntime, hVar);
        break;
#endif

	case BspOp_Call: 
        BspTemplateObjDoCall(hThisObject, hBranch, hRuntime, hVar, &bTerminated);
        break;

	case BspOp_Obj: 
        BspTemplateObjDoObj(hThisObject, hBranch, hRuntime, hVar, &bTerminated);
        break;

	case BspOp_Param: 
        BspTemplateObjDoParam(hThisObject, hBranch, hRuntime, hVar);
        break;

    case BspOp_Dim:
        BspTemplateObjDoDim(hThisObject, hBranch, hRuntime, hVar);
        break;

    case BspOp_ArrayItem:
        BspTemplateObjDoArrayItem(hThisObject, hBranch, hRuntime, hVar);
        break;

	case BspOp_Incr:
        BspTemplateObjDoIncr(hThisObject, hBranch, hRuntime, hVar);
		break;

	case BspOp_Decr:
        BspTemplateObjDoDecr(hThisObject, hBranch, hRuntime, hVar);
		break;

	case BspOp_Assign:
        BspTemplateObjDoAssign(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_LE:
        BspTemplateObjDoLe(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_GE:
        BspTemplateObjDoGe(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_NE:
        BspTemplateObjDoNe(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_LT:
        BspTemplateObjDoLt(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_GT:
        BspTemplateObjDoGt(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_Eq:
        BspTemplateObjDoEq(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_Or:
        BspTemplateObjDoOr(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_And:
        BspTemplateObjDoAnd(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_Not:
        BspTemplateObjDoNot(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_BitNot:
        BspTemplateObjDoBitNot(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_BitOr:
        BspTemplateObjDoBitOr(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_BitAnd:
        BspTemplateObjDoBitAnd(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_BitXor:
        BspTemplateObjDoBitXor(hThisObject, hBranch, hRuntime, hVar);
        break;

	case BspOp_Add:
        BspTemplateObjDoAdd(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_IncrN:
        BspTemplateObjDoIncrN(hThisObject, hBranch, hRuntime, hVar);
        break;

	case BspOp_Subtract:
        BspTemplateObjDoSubtract(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_DecrN:
        BspTemplateObjDoDecrN(hThisObject, hBranch, hRuntime, hVar);
        break;

    case BspOp_Multiply:
        BspTemplateObjDoMultiply(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_MultiplyN:
        BspTemplateObjDoMultiplyN(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_Divide:
        BspTemplateObjDoDivide(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_DivideN:
        BspTemplateObjDoDivideN(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_ModuloN:
        BspTemplateObjDoModuloN(hThisObject, hBranch, hRuntime, hVar);
        break;

    case BspOp_BitOrN:
        BspTemplateObjDoBitOrN(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_BitAndN:
        BspTemplateObjDoBitAndN(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_ShiftLeft:
        BspTemplateObjDoShiftLeft(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_ShiftRight:
        BspTemplateObjDoShiftRight(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_ShiftLeftN:
        BspTemplateObjDoShiftLeftN(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_ShiftRightN:
        BspTemplateObjDoShiftRightN(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_BitXorN:
        BspTemplateObjDoBitXorN(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_Modulo:
        BspTemplateObjDoModulo(hThisObject, hBranch, hRuntime, hVar);
		break;

    case BspOp_StrCat:
        BspTemplateObjDoStrCat(hThisObject, hBranch, hRuntime, hVar);
		break;

	case BspOp_SubStr: 
        BspTemplateObjDoSubStr(hThisObject, hBranch, hRuntime, hVar);
        break;

	case BspOp_Execute: 
        BspTemplateObjDoExecute(hThisObject, hBranch, hRuntime, hVar);
        break;

    case BspOp_Api:
        BspTemplateObjDoApi(hThisObject, iBr, hRuntime, hVar);
        break;

    case BspOp_Include:
        BspTemplateObjDoInclude(hThisObject, hBranch, hRuntime, hVar, &bTerminated);
        break;

    case BspOp_Name:
        if (iBr->left.type  == BspBranch_eBranch)
        {
            pMyObject->DoBranch(hThisObject, iBr->left.Value.b, hRuntime, hVar, &bTerminated);
        }
        else
        {
            aResult->Type       = BspVar_Number;
            aResult->Value.num  = iBr->left.Value.n;
        }
        break;

	default:
        {
            pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: unexpected branch operator.\r\n");
        }
    }

    if (pbTerminated)
    {
        *pbTerminated   = bTerminated;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjDoApi
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute API.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime environment object.

                ANSC_HANDLE                 hVar
                Contains returned value.

    return:     handle to object expression.

**********************************************************************/

void
BspTemplateObjDoApi
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        pResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BSP_TEMPLATE_API_CODE           code;
    PBSP_TEMPLATE_BRANCH_OBJECT     aBr;
    PBSP_TEMPLATE_VAR_OBJECT        pVar;
    BOOL                            bDelete   = TRUE;
    BOOL                            bTerminated = FALSE;

    code = pMyObject->GetApiCode(hThisObject, pBr->left.Value.s);

    if (code == BspApi_unknown)
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: unknown API call\r\n");

        return;
    }

    switch (code)
    {
    case BspApi_atoi:
        {
            int numParams = pBr->right.Value.a.Count;

            if (numParams != 1)
            {
                pMyObject->OutputString
                    (
                        (ANSC_HANDLE)pMyObject, 
                        "script error: API _atoi can only accept one parameter.\n"
                    );

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];

            pVar = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            switch (pVar->Type)
            {
            case BspVar_String:
                pResult->Value.num = pVar->Value.str?atoi(pVar->Value.str):0;
                break;

            case BspVar_Number:
                pResult->Value.num = pVar->Value.num;
                break;

            case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
                pResult->Value.num= BSP_TEMPLATE_DOUBLE_GET_INT(pVar->Value.real);
#else
                pResult->Value.num= (long)pVar->Value.real;
#endif
                break;

            default:
                pResult->Value.num = 0;
                pMyObject->OutputString
                    (
                        (ANSC_HANDLE)pMyObject, 
                        "script error: API _atoi can't handle the given parameter\n"
                    );
                break;
            }

            if (bDelete)
            {
                /* pVar->Remove((ANSC_HANDLE)pVar); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVar);
            }
        }
        break;

    case BspApi_atof:
        {
            int numParams = pBr->right.Value.a.Count;

            if (numParams != 1)
            {
                pMyObject->OutputString
                    (
                        (ANSC_HANDLE)pMyObject, 
                        "script error: API _atof can only accept one parameter "
                    );
                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];

            pVar = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            pResult->Type = BspVar_Real;

            switch (pVar->Type)
            {
            case BspVar_String:
#ifdef   _BSPENG_NO_DOUBLE
                {
                    char            *pEnd =NULL;
                    BOOL            bHasFrac =FALSE;
                    BOOL            *pHasFrac    = &bHasFrac;
                    char            **ppEnd = &pEnd;

                    STRING_TO_BSP_TEMPLATE_DOUBLE
                        (
                            pVar->Value.str, 
                            &pResult->Value.real, 
                            ppEnd, 
                            pHasFrac
                        );
                }
#else
                pResult->Value.real = atof(pVar->Value.str);
#endif
                break;

            case BspVar_Number:
#ifdef   _BSPENG_NO_DOUBLE
                BSP_TEMPATE_DOUBLE_SET
                    (
                        pResult->Value.real, 
                        pVar->Value.num * BSP_TEMPLATE_DOUBLE_SCALE
                    );
#else
                pResult->Value.real = pVar->Value.num;
#endif
                break;

            case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
                pResult->Value.real = pVar->Value.real;
#else
                pResult->Value.real = (long)pVar->Value.real;
#endif
                break;

            default:
#ifdef   _BSPENG_NO_DOUBLE
                BSP_TEMPATE_DOUBLE_SET(pResult->Value.real, 0);
#else
                pResult->Value.real = 0;
#endif

                pMyObject->OutputString
                    (
                        (ANSC_HANDLE)pMyObject, 
                        "script error: API _atof can't handle the given parameter.\n"
                    );
                break;
            }

            if (bDelete)
            {
                /* pVar->Remove((ANSC_HANDLE)pVar); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVar);
            }
        }
        break;
    
    case BspApi_itoa:
        {
            int                         numParams   = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    num, radix, buf;
            int                         nRadix      = 10;
            char                        *pBuf;

            if (numParams < 2 || numParams > 3)
            {
                pMyObject->OutputString
                    (
                        (ANSC_HANDLE)pMyObject, 
                        "script error: API _itoa can't handle given parameters.\n"
                    );

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];

            num = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            if (numParams == 3)
            {
                aBr = pBr->right.Value.a.pBranch[2];
                radix = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);
                nRadix = radix->Value.num;

                /* radix->Remove((ANSC_HANDLE)radix); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)radix);
            }

            aBr = pBr->right.Value.a.pBranch[1];

            /*
            buf = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);
            */
            if (TRUE)
            {
                PBSP_TEMPLATE_BRANCH_OBJECT pBranch = NULL;

                if (aBr->right.type == BspBranch_eBranch)
                {
                    pBranch = (PBSP_TEMPLATE_BRANCH_OBJECT)aBr->right.Value.b;
                }

                if (pBranch && pBranch->op == BspOp_ArrayItem)
                {
                    buf  = 
                        (PBSP_TEMPLATE_VAR_OBJECT)pMyObject->EvalArrayItem
                            (
                                hThisObject, 
                                &aBr->right,
                                hRuntime
                            );
                }
                else
                {
                    ULONG               ulVar = aBr->right.Value.n;

                    buf = pMyObject->StackVar(hThisObject, ulVar, hRuntime, FALSE);
                }
            }

            pBuf = (char *)AnscAllocateMemory(32);

            if (pBuf)
            {
                switch (num->Type)
                {
                    case    BspVar_Number:

                            _ansc_itoa(num->Value.num, pBuf, nRadix);
                            pResult->Value.str = pBuf;
                            pResult->Type = BspVar_String;
                            pResult->Size = AnscSizeOfString(pBuf);
                            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pResult);
                            if (buf)
                            {
                                buf->Assign((ANSC_HANDLE)buf, pResult);
                            }

                            break;

                    case    BspVar_Real:

#ifdef   _BSPENG_NO_DOUBLE
                            _ansc_itoa((int)BSP_TEMPLATE_DOUBLE_GET_INT(num->Value.real), pBuf, nRadix);
#else
                            _ansc_itoa((int)num->Value.real, pBuf, nRadix);
#endif
                            pResult->Value.str = pBuf;
                            pResult->Type = BspVar_String;
                            pResult->Size = AnscSizeOfString(pBuf);
                            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pResult);

                            if (buf)
                            {
                                buf->Assign((ANSC_HANDLE)buf, pResult);
                            }

                            break;

                    case    BspVar_String:

                            AnscFreeMemory(pBuf);
                            pResult->Assign((ANSC_HANDLE)pResult, num);
                            pResult->Type = BspVar_String;
                            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pResult);

                            if (buf)
                            {
                                buf->Assign((ANSC_HANDLE)buf, pResult);
                            }

                            break;

                    default:

                            AnscFreeMemory(pBuf);
                            pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _itoa can't handle given parameter.\r\n");

                            break;
                }
            }

            if (bDelete)
            {
                /* num->Remove((ANSC_HANDLE)num); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)num);
            }
        }
        break;

    case BspApi_ftoa:
        {
            int                         numParams = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    num, buf  = NULL;
            char                        *pBuf;

            if (numParams != 2)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _ftoa expects two parameters.\r\n");

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];
            num = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            aBr = pBr->right.Value.a.pBranch[1];
            /*
            buf = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);
            */
            if (TRUE)
            {
                PBSP_TEMPLATE_BRANCH_OBJECT pBranch = NULL;

                if (aBr->right.type == BspBranch_eBranch)
                {
                    pBranch = (PBSP_TEMPLATE_BRANCH_OBJECT)aBr->right.Value.b;
                }

                if (pBranch && pBranch->op == BspOp_ArrayItem)
                {
                    buf  = 
                        (PBSP_TEMPLATE_VAR_OBJECT)pMyObject->EvalArrayItem
                            (
                                hThisObject, 
                                &aBr->right,
                                hRuntime
                            );
                }
                else
                {
                    ULONG               ulVar = aBr->right.Value.n;

                    buf = pMyObject->StackVar(hThisObject, ulVar, hRuntime, FALSE);
                }
            }

            pBuf = (char *)AnscAllocateMemory(32);

            if (pBuf)
            {
                switch (num->Type)
                {
                    case    BspVar_Number:

                            _ansc_itoa(num->Value.num, pBuf, 10);
                            pResult->Value.str = pBuf;
                            pResult->Type = BspVar_String;
                            pResult->Size = AnscSizeOfString(pBuf);
                            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pResult);

                            if (buf)
                            {
                                buf->Assign((ANSC_HANDLE)buf, pResult);
                            }

                        break;

                    case    BspVar_Real:

#ifdef   _BSPENG_NO_DOUBLE
                            BSP_TEMPLATE_DOUBLE_TO_STRING(pBuf, (int)num->Value.real);
#else
                            errno_t rc = -1;
                            rc =sprintf_s(pBuf,32, "%f", num->Value.real);
                            if(rc < EOK){
                                ERR_CHK(rc);
                            }
#endif
                            pResult->Value.str = pBuf;
                            pResult->Type = BspVar_String;
                            pResult->Size = AnscSizeOfString(pBuf);
                            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pResult);

                            if (buf)
                            {
                                buf->Assign((ANSC_HANDLE)buf, pResult);
                            }

                            break;

                    case    BspVar_String:

                            AnscFreeMemory(pBuf);
                            pResult->Assign((ANSC_HANDLE)pResult, num);
                            pResult->Type = BspVar_String;
                            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pResult);

                            if (buf)
                            {
                                buf->Assign((ANSC_HANDLE)buf, pResult);
                            }

                            break;

                    default:

                            AnscFreeMemory(pBuf);
                            pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _ftoa can't handle given parameter.\r\n");

                            break;
                }
            }

            if (bDelete)
            {
                /* num->Remove((ANSC_HANDLE)num); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)num);
            }
        }
        break;

    case BspApi_strstr:
        {
            int                         numParams = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    s1, s2;
            BOOL                        bDeleteSub= TRUE;
            char                        *pStrSrc, *pStrSub;
            char                        *pStrFound = NULL;
            int                         nPos       = -1;

            if (numParams != 2)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _strstr expects two parameters.\r\n");

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];
            s1  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            aBr = pBr->right.Value.a.pBranch[1];
            s2  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            pStrFound   = NULL;

            if (s1 && s2)
            {
                pStrSrc     = pStrSub   = NULL;

                if (s1->Type == BspVar_String)
                {
                    pStrSrc = s1->Value.str;
                }

                if (s2->Type == BspVar_String)
                {
                    pStrSub = s2->Value.str;
                }

                if (pStrSrc && pStrSub)
                {
                    pStrFound   = _ansc_strstr(pStrSrc, pStrSub);
                    
                    if (pStrFound)
                    {
                        nPos    = pStrFound - pStrSrc;
                    }
                }
            }

            pResult->Type       = BspVar_Number;
            pResult->Value.num  = nPos;

            if (bDelete)
            {
                /* s1->Remove((ANSC_HANDLE)s1); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s1);
            }

            if (bDeleteSub)
            {
                /* s2->Remove((ANSC_HANDLE)s2); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s2);
            }
        }
        break;

    case BspApi_strcmp:
        {
            int                         numParams   = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    s1, s2;
            BOOL                        bDelete2    = TRUE;
            char                        *pStr1, *pStr2;
            int                         nCmpResult  = 0;

            if (numParams != 2)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _strcmp expects two parameters.\r\n");

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];
            s1  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            aBr = pBr->right.Value.a.pBranch[1];
            s2  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            if (s1 && s2)
            {
                pStr1   = pStr2 = NULL;

                if (s1->Type == BspVar_String)
                {
                    pStr1   = s1->Value.str;
                }

                if (s2->Type == BspVar_String)
                {
                    pStr2   = s2->Value.str;
                }

                if (pStr1 && pStr2)
                {
                    nCmpResult  = _ansc_strcmp(pStr1, pStr2);
                }
                else if (!pStr1 && !pStr2)
                {
                    nCmpResult  = 0;
                }
                else if (!pStr1 && pStr2)
                {
                    nCmpResult  = -1;
                }
                else
                {
                    nCmpResult  = 1;
                }
            }

            pResult->Type       = BspVar_Number;
            pResult->Value.num  = nCmpResult;

            if (bDelete)
            {
                /* s1->Remove((ANSC_HANDLE)s1); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s1);
            }

            if (bDelete2)
            {
                /* s2->Remove((ANSC_HANDLE)s2); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s2);
            }
        }
        break;

    case BspApi_strcmpi:
        {
            int                         numParams   = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    s1, s2;
            BOOL                        bDelete2    = TRUE;
            char                        *pStr1, *pStr2;
            int                         nCmpResult  = 0;

            if (numParams != 2)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _strcmpi expects two parameters.\r\n");

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];
            s1  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            aBr = pBr->right.Value.a.pBranch[1];
            s2  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            if (s1 && s2)
            {
                pStr1   = pStr2 = NULL;

                if (s1->Type == BspVar_String)
                {
                    pStr1   = s1->Value.str;
                }

                if (s2->Type == BspVar_String)
                {
                    pStr2   = s2->Value.str;
                }

                if (pStr1 && pStr2)
                {
                    nCmpResult  = _ansc_stricmp(pStr1, pStr2);
                }
                else if (!pStr1 && !pStr2)
                {
                    nCmpResult  = 0;
                }
                else if (!pStr1 && pStr2)
                {
                    nCmpResult  = -1;
                }
                else
                {
                    nCmpResult  = 1;
                }
            }

            pResult->Type       = BspVar_Number;
            pResult->Value.num  = nCmpResult;

            if (bDelete)
            {
                /* s1->Remove((ANSC_HANDLE)s1); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s1);
            }

            if (bDelete2)
            {
                /* s2->Remove((ANSC_HANDLE)s2); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s2);
            }
        }
        break;

    case BspApi_strncmp:
        {
            int                         numParams   = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    s1, s2, num;
            BOOL                        bDelete2    = TRUE;
            BOOL                        bDeleteNum  = TRUE;
            char                        *pStr1, *pStr2;
            int                         nCmpResult  = 0;
            int                         cmpCount    = 0;

            if (numParams != 3)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _strncmp expects two parameters.\r\n");

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];
            s1  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            aBr = pBr->right.Value.a.pBranch[1];
            s2  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            aBr = pBr->right.Value.a.pBranch[2];

            num = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            if (num->Type == BspVar_Number || num->Type == BspVar_UNumber)
            {
                cmpCount    = num->Value.num;
            }

            if (s1 && s2)
            {
                pStr1   = pStr2 = NULL;

                if (s1->Type == BspVar_String)
                {
                    pStr1   = s1->Value.str;
                }

                if (s2->Type == BspVar_String)
                {
                    pStr2   = s2->Value.str;
                }

                if (pStr1 && pStr2)
                {
                    nCmpResult  = _ansc_strncmp(pStr1, pStr2, cmpCount);
                }
                else if (!pStr1 && !pStr2)
                {
                    nCmpResult  = 0;
                }
                else if (!pStr1 && pStr2)
                {
                    nCmpResult  = -1;
                }
                else
                {
                    nCmpResult  = 1;
                }
            }

            pResult->Type       = BspVar_Number;
            pResult->Value.num  = nCmpResult;

            if (bDelete)
            {
                /* s1->Remove((ANSC_HANDLE)s1); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s1);
            }

            if (bDelete2)
            {
                /* s2->Remove((ANSC_HANDLE)s2); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s2);
            }

            if (bDeleteNum)
            {
                /* num->Remove((ANSC_HANDLE)num); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)num);
            }
        }
        break;

    case BspApi_strlen:
        {
            int                         numParams   = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    s1;
            char                        *pStr;
            int                         str_len     = 0;

            if (numParams != 1)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _strlen expects one parameters.\r\n");

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];
            s1  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            if (s1)
            {
                pStr    = NULL;

                if (s1->Type == BspVar_String)
                {
                    pStr    = s1->Value.str;
                }

                if (pStr)
                {
                    str_len = AnscSizeOfString(pStr);
                }
            }

            pResult->Type       = BspVar_Number;
            pResult->Value.num  = str_len;

            if (bDelete)
            {
                /* s1->Remove((ANSC_HANDLE)s1); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s1);
            }
        }
        break;

    case BspApi_strgetchar:
        {
            int                         numParams   = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    s1, num;
            BOOL                        bDeleteNum  = TRUE;
            char                        *pStr;
            int                         index       = 0;
            char                        c[2];

            c[0] = c[1] = 0;

            if (numParams != 2)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _strgetchar expects two parameters.\r\n");

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];
            s1  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            aBr = pBr->right.Value.a.pBranch[1];
            num = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            if (s1 && num)
            {
                pStr    = NULL;

                if (s1->Type == BspVar_String)
                {
                    pStr    = s1->Value.str;
                }

                if (num->Type == BspVar_Number || num->Type == BspVar_UNumber)
                {
                    index   = num->Value.num;

                    if (pStr)
                    {
                        int         str_len = AnscSizeOfString(pStr);

                        if (index >= 0 && index < str_len)
                        {
                            c[0]    = *(pStr + index);
                        }
                    }
                }
            }

            pResult->Type       = BspVar_String;
            pResult->Value.str  = AnscCloneString(c);
            pResult->Size       = AnscSizeOfString(pResult->Value.str);
            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pResult);

            if (bDelete)
            {
                /* s1->Remove((ANSC_HANDLE)s1); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s1);
            }

            if (bDeleteNum)
            {
                /* num->Remove((ANSC_HANDLE)num); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)num);
            }
        }
        break;

    case BspApi_strsetchar:
        {
            int                         numParams   = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    s1 = NULL, s2 = NULL, num = NULL; /*RDKB-6004, CID-24615 , Initializing */
            BOOL                        bDeleteNum  = TRUE;
            BOOL                        bDeleteS2   = TRUE;
            char                        *pStr;
            int                         index       = 0;
            char                        c = '\0';

            if (numParams != 3)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _strsetchar expects three parameters.\r\n");

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];

            if (aBr->right.type == BspBranch_eBranch)
            {
                PBSP_TEMPLATE_BRANCH_OBJECT pRight;

                pRight = (PBSP_TEMPLATE_BRANCH_OBJECT)aBr->right.Value.b;

                if (pRight->op == BspOp_ArrayItem)
                {
                    s1  = 
                        (PBSP_TEMPLATE_VAR_OBJECT)pMyObject->EvalArrayItem
                            (
                                hThisObject, 
                                &aBr->right,
                                hRuntime
                            );
                }
            }
            else
            {
                ULONG               ulVar;

                ulVar = aBr->right.Value.n;
                s1    = pMyObject->StackVar(hThisObject, ulVar, hRuntime, FALSE);
            }

            bDelete = FALSE;

            aBr = pBr->right.Value.a.pBranch[1];
            num = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            aBr = pBr->right.Value.a.pBranch[2];
            s2  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            if (s1 && num && s2)
            {
                pStr    = NULL;

                if (s1->Type == BspVar_String)
                {
                    pStr    = s1->Value.str;
                }

                if (num->Type == BspVar_Number || num->Type == BspVar_UNumber)
                {
                    BOOL            bCharValid  = TRUE;

                    index   = num->Value.num;

                    if (s2->Type == BspVar_String && s2->Value.str)
                    {
                        c   = s2->Value.str[0];
                    }
                    else if (s2->Type == BspVar_Number || s2->Type == BspVar_UNumber)
                    {
                        c   = (char)(s2->Value.num);
                    }
                    else
                    {
                        bCharValid  = FALSE;
                    }

                    if (pStr && bCharValid)
                    {
                        int         str_len = AnscSizeOfString(pStr);

                        if (index >= 0 && index < str_len)
                        {
                            pStr[index] = c;
                        }
                    }
                }
            }

            if (bDelete)
            {
                /* s1->Remove((ANSC_HANDLE)s1); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s1);
            }

            if (bDeleteNum)
            {
                /* num->Remove((ANSC_HANDLE)num); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)num);
            }

            if (bDeleteS2)
            {
                /* s2->Remove((ANSC_HANDLE)s2); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s2);
            }
        }
        break;

    case BspApi_strleft:
        {
            int                         numParams   = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    s1, num;
            BOOL                        bDeleteNum  = TRUE;
            char                        *pStr       = NULL;
            int                         length      = 0;
            char                        *pLeft      = NULL;
            int                         nLeftLen    = 0;

            if (numParams != 2)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _strleft expects two parameters.\r\n");

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];
            s1  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            aBr = pBr->right.Value.a.pBranch[1];
            num = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            if (s1 && num)
            {
                pStr    = NULL;

                if (s1->Type == BspVar_String)
                {
                    pStr    = s1->Value.str;
                }

                if (num->Type == BspVar_Number || num->Type == BspVar_UNumber)
                {
                    length  = num->Value.num;

                    if (pStr)
                    {
                        int         str_len = AnscSizeOfString(pStr);

                        if (length <= str_len)
                        {
                            nLeftLen    = length;
                        }
                        else
                        {
                            nLeftLen    = str_len;
                        }
                    }
                }
            }

            if (nLeftLen > 0 && pStr)
            {
                pLeft   = (char *)AnscAllocateMemory(nLeftLen + 1);
                if (pLeft)
                {
                    AnscCopyMemory(pLeft, pStr, nLeftLen);
                    pLeft[nLeftLen] = 0;
                }
            }

            pResult->Type       = BspVar_String;
            pResult->Value.str  = pLeft;
            pResult->Size       = pLeft?AnscSizeOfString(pLeft):0;
            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pResult);

            if (bDelete)
            {
                /* s1->Remove((ANSC_HANDLE)s1); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s1);
            }

            if (bDeleteNum)
            {
                /* num->Remove((ANSC_HANDLE)num); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)num);
            }
        }
        break;

    case BspApi_strmid:
        {
            int                         numParams   = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    s1, num, len;
            BOOL                        bDeleteNum  = TRUE;
            BOOL                        bDeleteLen  = TRUE;
            char                        *pStr       = NULL;
            int                         length      = 0;
            int                         start       = 0;
            char                        *pMid       = NULL;
            int                         nMidLen     = 0;

            if (numParams != 3)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _strmid expects three parameters.\r\n");

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];
            s1  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            aBr = pBr->right.Value.a.pBranch[1];
            num = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            aBr = pBr->right.Value.a.pBranch[2];
            len = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            if (s1 && num && len)
            {
                if (num->Type == BspVar_Number || num->Type == BspVar_UNumber)
                {
                    start  = num->Value.num;

                    if (len->Type == BspVar_Number || len->Type == BspVar_UNumber)
                    {
                        length  = len->Value.num;

                        if (start >= 0 && length >= 0 && s1->Type == BspVar_String)
                        {
                            pStr    = s1->Value.str;
                        }

                        if (pStr)
                        {
                            int         str_len = AnscSizeOfString(pStr);

                            if (start >= str_len)
                            {
                                pStr    = NULL;
                            }
                            else
                            {
                                pStr    += start;

                                if (start + length >= str_len)
                                {
                                    nMidLen = str_len - start;
                                }
                                else
                                {
                                    nMidLen = length;
                                }
                            }
                        }
                    }
                }
            }

            if (nMidLen > 0 && pStr)
            {
                pMid   = (char *)AnscAllocateMemory(nMidLen + 1);
                if (pMid)
                {
                    AnscCopyMemory(pMid, pStr, nMidLen);
                    pMid[nMidLen] = 0;
                }
            }

            pResult->Type       = BspVar_String;
            pResult->Value.str  = pMid;
            pResult->Size       = pMid?AnscSizeOfString(pMid):0;
            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pResult);

            if (bDelete)
            {
                /* s1->Remove((ANSC_HANDLE)s1); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s1);
            }

            if (bDeleteNum)
            {
                /* num->Remove((ANSC_HANDLE)num); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)num);
            }

            if (bDeleteLen)
            {
                /* len->Remove((ANSC_HANDLE)len); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)len);
            }
        }
        break;

    case BspApi_strright:
        {
            int                         numParams   = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    s1, num;
            BOOL                        bDeleteNum  = TRUE;
            char                        *pStr       = NULL;
            int                         length      = 0;
            char                        *pRight     = NULL;
            int                         nRightLen   = 0;

            if (numParams != 2)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: API _strright expects two parameters.\r\n");

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];
            s1  = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            aBr = pBr->right.Value.a.pBranch[1];
            num = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            if (s1 && num)
            {
                pStr    = NULL;

                if (s1->Type == BspVar_String)
                {
                    pStr    = s1->Value.str;
                }

                if (num->Type == BspVar_Number || num->Type == BspVar_UNumber)
                {
                    length  = num->Value.num;

                    if (pStr)
                    {
                        int         str_len = AnscSizeOfString(pStr);

                        if (length <= str_len)
                        {
                            nRightLen    = length;
                        }
                        else
                        {
                            nRightLen    = str_len;
                        }
                    }
                }
            }

            if (nRightLen > 0 && pStr)
            {
                pRight   = (char *)AnscAllocateMemory(nRightLen + 1);
                if (pRight)
                {
                    pStr    = pStr + AnscSizeOfString(pStr) - nRightLen;
                    AnscCopyMemory(pRight, pStr, nRightLen);
                    pRight[nRightLen] = 0;
                }
            }

            pResult->Type       = BspVar_String;
            pResult->Value.str  = pRight;
            pResult->Size       = pRight?AnscSizeOfString(pRight):0;
            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pResult);

            if (bDelete)
            {
                /* s1->Remove((ANSC_HANDLE)s1); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)s1);
            }

            if (bDeleteNum)
            {
                /* num->Remove((ANSC_HANDLE)num); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)num);
            }
        }
        break;

    case    BspApi_sleep:
        {
            int                         numParams   = pBr->right.Value.a.Count;
            PBSP_TEMPLATE_VAR_OBJECT    num;
            int                         nSleepTime  = 0;

            if (numParams != 1)
            {
                pMyObject->OutputString
                    (
                        (ANSC_HANDLE)pMyObject, 
                        "script error: API _sleep only takes one parameter.\n"
                    );

                return;
            }

            aBr = pBr->right.Value.a.pBranch[0];

            num = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

            switch (num->Type)
            {
                case BspVar_Number:
                case BspVar_UNumber:

                    nSleepTime  = (int)num->Value.num;

                    break;
        	default:
            	    break;
            }

            if (nSleepTime > 0)
            {
                AnscSleep(nSleepTime);
            }

            if (bDelete)
            {
                /* num->Remove((ANSC_HANDLE)num); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)num);
            }
        }
        break;
    default:
        break;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BSP_TEMPLATE_API_CODE
        BspTemplateObjGetApiCode
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pApiName
            )

    description:

        This function is called to get API code.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pApiName
                The name of API.

    return:     API code.

**********************************************************************/

BSP_TEMPLATE_API_CODE
BspTemplateObjGetApiCode
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pApiName
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           i, apiCount;

    apiCount = sizeof(BspTemplateAPINames)/sizeof(BspTemplateAPINames[0]);
    for (i = 0; i < apiCount; i ++)
    {
        if ( strcmp((char *)pApiName, (char *)BspTemplateAPINames[i]) == 0 )
        {
            return (BSP_TEMPLATE_API_CODE)i;
        }
    }

    return BspApi_unknown;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjIsOutputApi
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch
            )

    description:

        This function is called to see if given branch is an 
        output API.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjIsOutputApi
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_BRANCH_OBJECT     pRBr;
    BSP_TEMPLATE_API_CODE           ac;

    if (pBr->right.type != BspBranch_eBranch)
        return FALSE;

    pRBr = (PBSP_TEMPLATE_BRANCH_OBJECT)pBr->right.Value.b;

    if (pRBr->op != BspOp_Api)
        return FALSE;

    ac = pMyObject->GetApiCode(hThisObject, pRBr->left.Value.s);

    switch (ac)
    {
    case BspApi_atoi:
    case BspApi_atof:
        return TRUE;
    default:
            break;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjEvalExp
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranchData,
                ANSC_HANDLE                 hRuntime,
                BOOL                        *pbTerminated
            )

    description:

        This function is called to evaluate given branch data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranchData
                Branch data object.

                ANSC_HANDLE                 hRuntime
                Runtime environment object.

                BOOL                        *pbTerminated
                Execution termination flag.

    return:     handle to variable.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjEvalExp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranchData,
        ANSC_HANDLE                 hRuntime,
        BOOL                        *pbTerminated
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_BRANCH_DATA       pBrData   = (PBSP_TEMPLATE_BRANCH_DATA)hBranchData;
    PBSP_TEMPLATE_VAR_OBJECT        pResult;
    BOOL                            bTerminated = FALSE;
    int                             type = pBrData->type;

    /* pResult = CreateBspEngVarComponent(NULL, NULL, NULL); */
    pResult = (PBSP_TEMPLATE_VAR_OBJECT)pRt->AcquireVariable((ANSC_HANDLE)pRt);

    switch (type)
    {
    case BspBranch_eBranch:
        pMyObject->DoBranch(hThisObject, pBrData->Value.b, hRuntime, (ANSC_HANDLE)pResult, &bTerminated);
        break;

    case BspBranch_eString:
        pResult->Type   = BspVar_String;
        if (pRt->InExecute)
        {
            pResult->Size       = AnscSizeOfString(pBrData->Value.s) + 1;
            pResult->Value.str  = (PCHAR)AnscDupString((PUCHAR)pBrData->Value.s);
            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pResult);
        }
        else
        {
            pResult->Value.str  = pBrData->Value.s;
        }
        break;

    case BspBranch_eNumber:
        pResult->Type       = BspVar_Number;
        pResult->Value.num  = pBrData->Value.n;
        break;

    case BspBranch_eReal:
        pResult->Type       = BspVar_Real;
        pResult->Value.real = pBrData->Value.r;
        break;

    case BspBranch_eVar:
        {
            PBSP_TEMPLATE_VAR_OBJECT    pVar;

            pVar = pMyObject->StackVar((ANSC_HANDLE)pMyObject, pBrData->Value.n, hRuntime, FALSE);
            pResult->Assign((ANSC_HANDLE)pResult, pVar);
        }
        break;

    case BspOp_Name:
        pResult->Type       = BspVar_Number;
        pResult->Value.num  = pBrData->Value.n;
        break;

    default:
        pMyObject->OutputString
            (
                (ANSC_HANDLE)pMyObject, 
                "script error: cannot evaluate given branch, type not correct.\n"
            );
        break;
    }

    if (pbTerminated)
    {
        *pbTerminated   = bTerminated;
    }

    return pResult;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        const char *
        BspTemplateObjEvalName
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                BOOL                        bPassRef
            )

    description:

        This function is called to evaluate the branch to get
        the name of object access or API call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime environment object.

                BOOL                        bPassRef
                To pass variable reference or not.

    return:     name of evaluated branch.

**********************************************************************/

const char *
BspTemplateObjEvalName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        BOOL                        bPassRef
    )
{
    /*
     * Please refer to the comments on function BspTemplateObjNameExp in this file first.
     * This function only tries to evaluate the first group of parameters. The others
     * if exist will be ignored.
     */

    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_BRANCH_OBJECT     pTopBr    = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_BRANCH_OBJECT     aBr;
    PBSP_TEMPLATE_VAR_OBJECT        pVar, pEvalVar;
    PBSP_TEMPLATE_STACK_OBJECT      pStack;
/*    PBSP_TEMPLATE_LIST_OBJECT       pList;*/
    BOOL                            bTerminated = FALSE;

    if (pBr->op != BspOp_Name)
        return NULL;

    pBr         = pBr->right.Value.b;

    pStack      = (PBSP_TEMPLATE_STACK_OBJECT)pRt->GetStack((ANSC_HANDLE)pRt);
    /*pList       = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;*/

    for (aBr = pBr->right.Value.b; aBr; aBr = aBr->left.Value.b)
    {
        pVar = pStack->Add((ANSC_HANDLE)pStack, 1);

        if (bPassRef && aBr->right.type == BspBranch_eVar)
        {
            /* variable reference */
            pVar->Type          = BspVar_Ref;
            pVar->Value.num     = aBr->right.Value.n + pStack->GetBase((ANSC_HANDLE)pStack);
        }
        else
        {
            /*PBSP_TEMPLATE_BRANCH_OBJECT pRight;

            pRight = (PBSP_TEMPLATE_BRANCH_OBJECT)aBr->right.Value.b;*/

            pEvalVar = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

/*
            if (aBr->right.type == BspBranch_eBranch && pRight->op == BspOp_ArrayItem)
            {
                PBSP_TEMPLATE_VAR_OBJECT    pArrayVal;

                pArrayVal = 
                    pMyObject->EvalArrayItem
                        (
                            (ANSC_HANDLE)pMyObject, 
                            &aBr->right,
                            hRuntime
                        );

                if (!pArrayVal)
                {
                    return NULL;
                }
                else
                {
                    pVar->Assign((ANSC_HANDLE)pVar, pArrayVal);
                }
            }
            else
*/
            {
                pVar->Assign((ANSC_HANDLE)pVar, pEvalVar);
                /* pEvalVar->Remove((ANSC_HANDLE)pEvalVar); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pEvalVar);
            }
        }
    }

    return pTopBr->left.Value.s;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        const char *
        BspTemplateObjEvalName2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hParamList
            )

    description:

        This function is called to evaluate branch to get the
        name and parameters of object access or procedure call etc.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hParamList
                Parameter list.

    return:     name.

**********************************************************************/

const char *
BspTemplateObjEvalName2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hParamList
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr         = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_BRANCH_OBJECT     pTopBr      = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt         = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_BRANCH_OBJECT     aBr, bBr;
    PBSP_TEMPLATE_VAR_OBJECT        pVar, pEvalVar=NULL; /*RDKB-6004, CID-24772, initializing pEvalVar*/
/*    PBSP_TEMPLATE_STACK_OBJECT      pStack;
    PBSP_TEMPLATE_LIST_OBJECT       pList;*/
    ULONG                           i, j, ulCount  = 0;
    PBSP_TEMPLATE_VAR_OBJECT        *pParams    = NULL;
    ULONG                           ulParamList = 0;
    PBSP_TEMPLATE_NAME_PARAM_LIST   pParamList  = NULL;
    PBSP_TEMPLATE_NAME_PARAM_LIST   *ppParamList= (PBSP_TEMPLATE_NAME_PARAM_LIST *)hParamList;
    BOOL                            bTerminated = FALSE;
    PBSP_TEMPLATE_VAR_OBJECT        SlapParamRef[BSPENG_RUNTIME_MAX_SLAP_PARAMETER_REF];
    ULONG                           SlapParamRefCount   = 0;
    ULONG                           ulOutParamCount     = 0;
    BOOL                            bOutputListStarted  = FALSE;
    PBSP_TEMPLATE_BRANCH_OBJECT     pRight;

    if (pBr->op != BspOp_Name)
        return NULL;

    /*pStack      = (PBSP_TEMPLATE_STACK_OBJECT)pRt->GetStack((ANSC_HANDLE)pRt);
    pList       = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;*/

    /* get number of parameters */
    for (aBr = pBr->right.Value.b; aBr; aBr = aBr->left.Value.b)
    {
        ulParamList ++;
    }

    /* create parameter list */
    CREATE_BSP_TEMPLATE_NAME_PARAM_LIST(pParamList, ulParamList);

    for (i = 0, bBr = pBr->right.Value.b; bBr; bBr = bBr->left.Value.b, i++)
    {
        ulCount = 0;
        for (aBr = bBr->right.Value.b; aBr; aBr = aBr->left.Value.b)
        {
            ulCount ++;
        }

        CREATE_BSP_TEMPLATE_NAME_PARAM(pParamList->pGroups[i], ulCount);

        pParams = pParamList->pGroups[i]->pParams;

        ulOutParamCount     = 0;
        bOutputListStarted  = FALSE;

        for (j = 0, aBr = bBr->right.Value.b; aBr; aBr = aBr->left.Value.b, j++)
        {
            /* pVar = CreateBspEngVarComponent(NULL, NULL, NULL); */
            pVar    = (PBSP_TEMPLATE_VAR_OBJECT)pRt->AcquireVariable((ANSC_HANDLE)pRt);

            pParams[j] = pVar;

            pRight = (PBSP_TEMPLATE_BRANCH_OBJECT)aBr->right.Value.b;

            if (aBr->op == BspOp_Comma || aBr->op == BspOp_Colon)
            {
                if (aBr->op == BspOp_Colon)
                {
                    bOutputListStarted  = TRUE;
                }

                if (bOutputListStarted)
                {
                    ulOutParamCount ++;
                }

                pEvalVar = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

                if (bOutputListStarted && SlapParamRefCount < BSPENG_RUNTIME_MAX_SLAP_PARAMETER_REF)
                {
                    SlapParamRef[SlapParamRefCount ++]  = 
                        pMyObject->EvalRef
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)&aBr->right,
                                (ANSC_HANDLE)pRt
                            );
                }
            }
            else
                if (pRight && (pRight->op == BspOp_Comma || pRight->op == BspOp_Colon))
                {
                    if (pRight->op == BspOp_Colon)
                    {
                        bOutputListStarted  = TRUE;
                    }

                    if (bOutputListStarted)
                    {
                        ulOutParamCount ++;
                    }

                    pEvalVar = pMyObject->EvalExpression(hThisObject, &pRight->right, hRuntime, &bTerminated);

                    if (bOutputListStarted && SlapParamRefCount < BSPENG_RUNTIME_MAX_SLAP_PARAMETER_REF)
                    {
                        SlapParamRef[SlapParamRefCount ++]  = 
                            pMyObject->EvalRef
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    (ANSC_HANDLE)&pRight->right,
                                    (ANSC_HANDLE)pRt
                                );
                    }
                }

            pVar->Assign((ANSC_HANDLE)pVar, pEvalVar);
            /* pEvalVar->Remove((ANSC_HANDLE)pEvalVar); */
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pEvalVar);
        }

        /* set the number of output parameters */
        pParamList->pGroups[i]->ulOutParams = ulOutParamCount;
    }

    *ppParamList    = pParamList;

    pRt->SetSlapParamRef((ANSC_HANDLE)pRt, (ANSC_HANDLE*)SlapParamRef, SlapParamRefCount);

    return (pTopBr->left.type == BspBranch_eString)?pTopBr->left.Value.s : NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjEvalRef
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime
            )

    description:

        This function is called to evaluate the branch to get
        the parameter reference on the stack.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime environment object.

    return:     variable on stack, NULL means not a valid
                parameter for output.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjEvalRef
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranchData,
        ANSC_HANDLE                 hRuntime
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_BRANCH_DATA       pBrData   = (PBSP_TEMPLATE_BRANCH_DATA)hBranchData;
    PBSP_TEMPLATE_STACK_OBJECT      pStack;
    PBSP_TEMPLATE_VAR_OBJECT        pVar;
    ULONG                           ulIndex;
    PBSP_TEMPLATE_VAR_OBJECT        pRefVar   = NULL;


    /* only variables, array items are qualified to be output parameters */

    pStack  = (PBSP_TEMPLATE_STACK_OBJECT)pRt->GetStack((ANSC_HANDLE)pRt);

    switch (pBrData->type)
    {
    case BspBranch_eBranch:

        if (pBrData->Value.b)
        {
            PBSP_TEMPLATE_BRANCH_OBJECT iBr = (PBSP_TEMPLATE_BRANCH_OBJECT)pBrData->Value.b;

            if (iBr->op == BspOp_ArrayItem)
            {
                pVar    = 
                    (PBSP_TEMPLATE_VAR_OBJECT)pMyObject->EvalArrayItem
                        (
                            (ANSC_HANDLE)pMyObject,
                            pBrData,
                            hRuntime
                        );

                if (pVar)
                {
                    pRefVar = pVar;
                }
            }
        }
        break;

    case BspBranch_eVar:
        {
            LONG                    nPos    = pBrData->Value.n;

            ulIndex = pStack->GetBase((ANSC_HANDLE)pStack);
            ulIndex += nPos;

            pVar    = (PBSP_TEMPLATE_VAR_OBJECT)pStack->GetAt((ANSC_HANDLE)pStack, ulIndex);
            pRefVar = pVar;

            while (pVar && pVar->Type == BspVar_Ref)
            {
                nPos    = pVar->Value.num;
                pVar = (PBSP_TEMPLATE_VAR_OBJECT)pStack->GetAt((ANSC_HANDLE)pStack, pVar->Value.num);

                if (pVar)
                {
                    pRefVar = pVar;
                }
            }
        }
        break;
        default:
            break;
    }

    return pRefVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjStackVar
            (
                ANSC_HANDLE                 hThisObject,
                LONG                        nVar,
                ANSC_HANDLE                 hRuntime,
                BOOL                        bParam
            )

    description:

        This function is called to return the variable on stack
        as required.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                LONG                        nVar
                Variable ID.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                BOOL                        bParam
                Whether to consider arguments.

    return:     handle to variable object.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjStackVar
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        nVar,
        ANSC_HANDLE                 hRuntime,
        BOOL                        bParam
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_STACK_OBJECT      pStack;
    PBSP_TEMPLATE_VAR_OBJECT        pVar;
    ULONG                           ulIndex;

    pStack  = (PBSP_TEMPLATE_STACK_OBJECT)pRt->GetStack((ANSC_HANDLE)pRt);
    ulIndex = pStack->GetBase((ANSC_HANDLE)pStack);
    ulIndex += nVar;

    if (!bParam && pRt->ulArgCount > pMyObject->ulNumParams)
        ulIndex += pRt->ulArgCount - pMyObject->ulNumParams;

    pVar = (PBSP_TEMPLATE_VAR_OBJECT)pStack->GetAt((ANSC_HANDLE)pStack, ulIndex);

    while (pVar && pVar->Type == BspVar_Ref)
        pVar = (PBSP_TEMPLATE_VAR_OBJECT)pStack->GetAt((ANSC_HANDLE)pStack, pVar->Value.num);

    return (ANSC_HANDLE)pVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjSetList
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOwnerList
            )

    description:

        This function is called to set owner list object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOwnerList
                List object which owns the template object.

    return:     void.

**********************************************************************/

void
BspTemplateObjSetList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerList
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;

    pMyObject->hOwnerList   = hOwnerList;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BspTemplateObjGetErrorLineNo
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get error line number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     error line number.

**********************************************************************/

ULONG
BspTemplateObjGetErrorLineNo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;

    return pMyObject->ulErrLineNo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        const char *
        BspTemplateObjObjectExp
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get error message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     error message.

**********************************************************************/

const char *
BspTemplateObjGetErrorMsg
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;

    return pMyObject->pErrMsg;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BspTemplateObjGetLineNo
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get line number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     line number.

**********************************************************************/

ULONG
BspTemplateObjGetLineNo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;

    return pMyObject->ulLineNo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjGetToken
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get next token from token list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to next token.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjGetToken
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT                pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;

    return pMyObject->hToken;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjCopy
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTmpl
            )

    description:

        This function is called to copy given template.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTmpl
                The template to copy from.

    return:     void.

**********************************************************************/

void
BspTemplateObjCopy
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTmpl
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_OBJECT            pTmpl     = (PBSP_TEMPLATE_OBJECT)hTmpl;
    ULONG                           ulSize, i;
    PBSP_TEMPLATE_SYMBOL_PRO        pSymPro, pPro;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray, pMyArray;

    pMyObject->pName        = (PCHAR)AnscDupString((PUCHAR)pTmpl->pName);
    pMyObject->hOwnerList   = pTmpl->hOwnerList;
    pMyObject->ulErrLineNo  = 0;
    pMyObject->ulNumParams  = 0;
    pMyObject->hTree        = NULL;

    pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pTmpl->hSymbol;
    pMyArray    = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hSymbol;

    ulSize = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);

    for (i = 0; i < ulSize; i ++)
    {
        pSymPro = (PBSP_TEMPLATE_SYMBOL_PRO)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i);

        pPro    = (PBSP_TEMPLATE_SYMBOL_PRO)AnscAllocateMemory(sizeof(BSP_TEMPLATE_SYMBOL_PRO));

        if (pPro)
        {
            pPro->pName = (PCHAR)AnscDupString((PUCHAR)pSymPro->pName);
            pMyArray->Add((ANSC_HANDLE)pMyArray, pPro);
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjUpdateSymbols
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTmpl
            )

    description:

        This function is called to update symbols from
        the given template, it should only be called after
        'execute' statement is done.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTmpl
                The template to update symbols from.

    return:     void.

**********************************************************************/

void
BspTemplateObjUpdateSymbols
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTmpl
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_OBJECT            pTmpl     = (PBSP_TEMPLATE_OBJECT)hTmpl;
    ULONG                           ulSize, i;
    PBSP_TEMPLATE_SYMBOL_PRO        pSymPro, pPro;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray, pMyArray;
    ULONG                           ulNewSize;

    pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pTmpl->hSymbol;
    pMyArray    = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hSymbol;

    ulNewSize   = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);
    ulSize      = pMyArray->GetSize((ANSC_HANDLE)pMyArray);

    if (ulNewSize > ulSize)
    {
        for (i = ulSize; i < ulNewSize ; i ++)
        {
            pSymPro = (PBSP_TEMPLATE_SYMBOL_PRO)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i);

            pPro    = (PBSP_TEMPLATE_SYMBOL_PRO)AnscAllocateMemory(sizeof(BSP_TEMPLATE_SYMBOL_PRO));

            if (pPro)
            {
                pPro->pName = (PCHAR)AnscDupString((PUCHAR)pSymPro->pName);
                pMyArray->Add((ANSC_HANDLE)pMyArray, pPro);
            }
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjEvalArrayItem
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranchData,
                ANSC_HANDLE                 hRuntime
            )

    description:

        This function is called to evaluate array item..

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranchData
                Branch data object.

                ANSC_HANDLE                 hRuntime
                Runtime environment object.

    return:     handle to variable object.

**********************************************************************/

ANSC_HANDLE
BspTemplateObjEvalArrayItem
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranchData,
        ANSC_HANDLE                 hRuntime
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_DATA       pBrData   = (PBSP_TEMPLATE_BRANCH_DATA)hBranchData;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr;
    PBSP_TEMPLATE_VAR_OBJECT        pVal, pVar;
    ULONG                           ulVar;
    PBSP_TEMPLATE_ARRAY_DATA        pArrayData;
    PBSP_TEMPLATE_ARRAY_ITEMS       pItems;
    BOOL                            bOutBound = FALSE;
    BOOL                            bNeedGrow = FALSE;
    ULONG                           ulIndex   = 0;
    ULONG                           i, ulSize;
    PBSP_TEMPLATE_BRANCH_OBJECT     pDim;
    BOOL                            bTerminated;
    ULONG                           ulNewItems= 0;
    ULONG                           ulNewLen  = 0;
    ANSC_STATUS                     status    = ANSC_STATUS_SUCCESS;

    if (pBrData->type != BspBranch_eBranch)
    {
        return (ANSC_HANDLE)NULL;
    }

    iBr = (PBSP_TEMPLATE_BRANCH_OBJECT)pBrData->Value.b;

    if (iBr->op != BspOp_ArrayItem)
    {
        return (ANSC_HANDLE)NULL;
    }
    else
    {
        ulVar = iBr->left.Value.n;
        pVal  = pMyObject->StackVar(hThisObject, ulVar, hRuntime, FALSE);

        if (!pVal || pVal->Type != BspVar_Array)
        {
            return (ANSC_HANDLE)NULL;
        }

        pItems  = &pVal->Value.arrayItems;

        pBrData = &iBr->right;

        if (pBrData->type == BspBranch_eArrayDim)
        {
            pArrayData  = &pBrData->Value.a;

            if (pArrayData->Count > BSP_TEMPLATE_ARRAY_DIMENSION_LIMIT)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: too many indices specified to an array.\r\n");

                return (ANSC_HANDLE)NULL;
            }

            for (i = 0; i < pArrayData->Count; i ++)
            {
                pDim    = pArrayData->pBranch[i];
                pVar    = pMyObject->EvalExpression(hThisObject, &pDim->right, hRuntime, &bTerminated);

                /* 
                 * protect BSP engine from misbehaving if a script error found that
                 * array index is not of type integer or unsigned integer
                 */
                if ( pVar->Type != BspVar_Number && pVar->Type != BspVar_UNumber )
                {
                    AnscTrace("script error: numeric value is expected as array index!\r\n");
                    bOutBound = TRUE;
                    break;
                }

                ulSize  = pVar->Value.num;

                /* pVar->Remove((ANSC_HANDLE)pVar); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVar);

                if (i == 0)
                {
                    ulIndex     = ulSize;
                    ulNewItems  = ulSize + 1;
                    ulNewLen    = ulNewItems;
                    bNeedGrow   = (ulIndex >= pItems->ulSize);
                }
                else
                {
                    ulIndex     *= pItems->Dims[i];
                    ulIndex     += ulSize;
                    bOutBound   |= (ulSize >= pItems->Dims[i]);

                    ulNewItems  *= pItems->Dims[i];
                }
            }

            if (bOutBound)
            {
                pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: too many or incorrect indices specified to an array.\r\n");

                return (ANSC_HANDLE)NULL;
            }
            else
            {
                PBSP_TEMPLATE_VAR_OBJECT    *pVars, pVar;

                if (bNeedGrow)
                {
                    status  = pVal->SetArraySize((ANSC_HANDLE)pVal, ulNewItems);

                    if (status != ANSC_STATUS_SUCCESS || pVal->Value.arrayItems.bBulkData)
                    {
                        return (ANSC_HANDLE)NULL;
                    }
                }
                else
                {
                    if (pItems->ulLen < ulNewLen)
                    {
                        pItems->ulLen   = ulNewLen;
                    }
                }

                if (pItems->bBulkData)
                {
                    return (ANSC_HANDLE)NULL;
                }
                else
                {
                    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pItems->hVars;
                    pVar    = pVars[ulIndex];

                    return (ANSC_HANDLE)pVar;
                }
            }
        }
    
        return (ANSC_HANDLE)NULL;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjStore
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hArchive
            )

    description:

        This function is called to save the template object into
        archive.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hArchive
                Archive object.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjStore
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive  = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hArchive;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBranch   = (PBSP_TEMPLATE_BRANCH_OBJECT)pMyObject->hTree;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hSymbol;
    PBSP_TEMPLATE_SYMBOL_PRO        pSymPro;
    BOOL                            bSucc;
    ULONG                           i, ulSize;
    UCHAR                           ucHasBranch;

    /* write name into archive */
    bSucc = pArchive->WriteString(hArchive, (PUCHAR)pMyObject->pName);
    if (!bSucc)
        return FALSE;

    /* write a flag whether or not the procedure has statement */
    ucHasBranch = (UCHAR)(pBranch?1:0);

    bSucc = pArchive->WriteByte(hArchive, ucHasBranch);
    if (!bSucc)
        return FALSE;

    if (ucHasBranch == 0)
        return TRUE;

    /* write syntax tree into archive */
    bSucc = BspTemplateBranchStore((ANSC_HANDLE)pBranch, hArchive);
    if (!bSucc)
        return FALSE;

    /* write number of parameters */
    bSucc = pArchive->WriteUlong(hArchive, pMyObject->ulNumParams);

    /* write symbols into archive */
    ulSize  = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);

    /* write number of symbols into archive */
    pArchive->WriteUlong(hArchive, ulSize);

    for (i = 0; i < ulSize; i ++)
    {
        pSymPro = (PBSP_TEMPLATE_SYMBOL_PRO)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i);

        /* write symbol name into archive */
        bSucc = pArchive->WriteString(hArchive, (PUCHAR)pSymPro->pName);
        if (!bSucc)
            return FALSE;
    }

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateObjLoad
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hArchive
            )

    description:

        This function is called to load template object from archive.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hArchive
                Archive object.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateObjLoad
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive  = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hArchive;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBranch;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hSymbol;
    PBSP_TEMPLATE_SYMBOL_PRO        pSymPro;
    ULONG                           i, ulSize;
    BOOL                            bSucc;
    UCHAR                           ucHasBranch;

    /* read BSP name */
    bSucc = pArchive->ReadString(hArchive, (PUCHAR *)&pMyObject->pName);
    if (!bSucc)
        return FALSE;

    bSucc = pArchive->ReadByte(hArchive, &ucHasBranch);
    if (!bSucc)
        return FALSE;

    if (ucHasBranch == 0)
        return TRUE;

    /* read syntax tree */
    pBranch = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
    if ( !pBranch )
        return FALSE;
    bSucc   = BspTemplateBranchLoad((ANSC_HANDLE)pBranch, hArchive);
    if (!bSucc)
    {
        BspTemplateBranchCORemove((ANSC_HANDLE)pBranch);
        return FALSE;
    }

    pMyObject->hTree    = (ANSC_HANDLE)pBranch;

    /* read number of parameters */
    bSucc   = pArchive->ReadUlong(hArchive, &pMyObject->ulNumParams);

    /* read the amount of symbols */
    bSucc   = pArchive->ReadUlong(hArchive, &ulSize);
    if (!bSucc)
        return FALSE;

    for (i = 0; i < ulSize; i ++)
    {
        pSymPro = (PBSP_TEMPLATE_SYMBOL_PRO)AnscAllocateMemory(sizeof(BSP_TEMPLATE_SYMBOL_PRO));

        if (!pSymPro)
        {
            return FALSE;
        }

        /* read symbol name */
        bSucc   = pArchive->ReadString(hArchive, (PUCHAR *)&pSymPro->pName);
        if (!bSucc)
        {
            /*RDKB-6004, CID-24224; freeing Allocated memory in case of error*/
            AnscFreeMemory(pSymPro);
            if(i)
            {
                i--;
                for (; (int)i >= 0; i--)
                {
                     pSymPro = (PBSP_TEMPLATE_SYMBOL_PRO)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i);
                     pPtrArray->RemoveAt((ANSC_HANDLE)pPtrArray, i, 1);
                     if(pSymPro)
                     {
                         AnscFreeMemory(pSymPro);
                     }
                }
            }
            pSymPro = NULL;
            return FALSE;
        }

        pPtrArray->Add((ANSC_HANDLE)pPtrArray, pSymPro);
    }

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjObjectExp
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCmif
            )

    description:

        This function is called to set CM interface.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCmIf
                Cache management interface.

    return:     handle to object expression.

**********************************************************************/

ANSC_STATUS
BspTemplateObjSetCmif
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCmif
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;

    pMyObject->hCmif    = hCmif;

    return ANSC_STATUS_SUCCESS;
}


static void
BspEng_CalcPagePath
    (
        char                        *pPagePath, 
        char                        *pName
    )
{
    int                             i, Len;
    char                            *pLoc; 
    char                            *pTail;
    errno_t                         rc = -1;

    Len = AnscSizeOfString(pPagePath);
    for (i = Len - 1; i >= 0; i --)
    {
        if (pPagePath[i] == '/' || pPagePath[i] == '\\')
        {
            break;
        }
    }

    if (i < 0)
    {
        /* Size of pPagePath is passed as 512 bytes*/
        rc = strcpy_s(pPagePath, 512, pName);
        ERR_CHK(rc);
    }
    else
    {
        i ++;
        /* Size of pPagePath is passed as 512 bytes*/
        rc = strcpy_s((pPagePath + i), (unsigned int)(512 - i), pName);
        ERR_CHK(rc);
    }

    /* remove relative path such as ".." from the path name */
    while ( TRUE )
    {
        pLoc    = _ansc_strstr(pPagePath, "../");

        if ( !pLoc )
        {
            break;
        }

        pTail = pLoc + AnscSizeOfString("../");

        if ( pLoc > pPagePath && *(pLoc-1) == '/' )
        {
            pLoc -= 2;
            while ( pLoc >= pPagePath )
            {
                if ( *pLoc == '/' )
                {
                    pLoc ++;
                    break;
                }

                pLoc --;
            }

            if ( pLoc < pPagePath )
            {
                pLoc = pPagePath;
            }

            Len = AnscSizeOfString(pTail);

            for (i = 0; i < Len; i ++)
            {
                *pLoc   = *pTail;
                pLoc ++;
                pTail ++;
            }

            *pLoc   = '\0';
        }
        else
        {
            /* !!! illegal reference to parent directory of root path !!! */
            *pPagePath  = '\0';
            break;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateObjIncludeTemplate
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRuntime,
                PUCHAR                      pName,
                BOOL                        *pbTerminated
            )

    description:

        This function is called to include the given template into
        current one.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRuntime
                Runtime environment object.

                PUCHAR                      pName
                Name of included template.

                BOOL                        *pbTerminated
                Execution termination flag.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateObjIncludeTemplate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        PUCHAR                      pName,
        BOOL                        *pbTerminated
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject       = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt             = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_STACK_OBJECT      pStack          = (PBSP_TEMPLATE_STACK_OBJECT)pRt->hStack;
    PBSP_TEMPLATE_LIST_OBJECT       pList           = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;
    PBSPENG_SOA_INTERFACE           pBspSoaIf       = pList->GetBspSoaIf((ANSC_HANDLE)pList);
    BOOL                            bTerminated     = FALSE;
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive        = NULL;
    BOOL                            bSucc           = FALSE;
    PBSP_TEMPLATE_LIST_OBJECT       pImportList     = NULL;
    char                            *pPagePath      = NULL;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    ANSC_HANDLE                     hCookedPage     = (ANSC_HANDLE)NULL;
    errno_t                         rc              = -1;

    /*
     *  The path of the new BSP page should be the relative
     *  to the current one.
     */

    pPagePath   = (char *)AnscAllocateMemory(512);
    
    if (!pPagePath)
    {
        status  = ANSC_STATUS_FAILURE;

        goto OUT_OF_RESOURCES;
    }

    rc = strcpy_s(pPagePath,512, (char *)pMyObject->pName);
    ERR_CHK(rc);
    BspEng_CalcPagePath(pPagePath,(char *) pName);

    if (!pList->IsTemplateLoaded((ANSC_HANDLE)pList, pPagePath))
    {
        PUCHAR                      pStream     = NULL;
        ULONG                       ulStreamLen = 0;

        hCookedPage =
            pBspSoaIf->GetCookedPage
                (
                    pBspSoaIf->hOwnerContext,
                    pPagePath
                );

#ifdef   _DEBUG
        if ( BspEngIsCallFlowTraced() )
        {
            AnscTrace("Loading included BSP : %s ...\n", pPagePath);
        }
#endif

        if ( hCookedPage == (ANSC_HANDLE)NULL )
        {
#ifdef   _DEBUG
            if ( BspEngIsCallFlowTraced() )
            {
                AnscTrace("Failed to load BSP page - %s.\n", pPagePath);
            }
#endif

            status  = ANSC_STATUS_FAILURE;

            goto OUT_OF_RESOURCES;
        }

        pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)CreateBspEngArchiveComponent(NULL, NULL, NULL);
        pImportList = (PBSP_TEMPLATE_LIST_OBJECT)CreateBspEngListComponent(NULL, NULL, NULL);

        if (!pArchive || !pImportList)
        {
            status  = ANSC_STATUS_RESOURCES;

            goto OUT_OF_RESOURCES;
        }

        pImportList->UseArchive((ANSC_HANDLE)pImportList, TRUE);

        pBspSoaIf->GetCookedPageData
            (
                pBspSoaIf->hOwnerContext,
                hCookedPage,
                &pStream,
                &ulStreamLen
            );

        pArchive->SetStorage
            (
                (ANSC_HANDLE)pArchive, 
                pStream, 
                ulStreamLen, 
                FALSE
            );

        /* load "compiled" template into list object */
        bSucc = pImportList->Load((ANSC_HANDLE)pImportList, (ANSC_HANDLE)pArchive);
        pArchive->Remove((ANSC_HANDLE)pArchive);

        if (bSucc)
        {
            pList->ImportTemplates((ANSC_HANDLE)pList, (PUCHAR)pName, (ANSC_HANDLE)pImportList);
            pImportList->ClearTemplates((ANSC_HANDLE)pImportList);
        }

        pImportList->Remove((ANSC_HANDLE)pImportList);
    }

    if (TRUE)
    {
        BSP_TEMPLATE_STATE          aState;
        PBSP_TEMPLATE_OBJECT        aTmpl;
        PBSP_TEMPLATE_LIST_OBJECT   pList;

		pStack->GetState((ANSC_HANDLE)pStack, &aState);

        pList = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;
		aTmpl = pList->FindItem((ANSC_HANDLE)pList, pPagePath);

		if (!aTmpl) 
        {
            AnscTrace("Cannot find procedure %s.\n", pPagePath);
		}
        else
        {
            aTmpl->ulStackBase  = aState.base;

		    pStack->NewFunction((ANSC_HANDLE)pStack, &aState);
		    aTmpl->DoTemplate((ANSC_HANDLE)aTmpl, hRuntime, &bTerminated);
		    pStack->DeleteFunction((ANSC_HANDLE)pStack, &aState);
        }
    }

    if (pbTerminated)
    {
        *pbTerminated   = bTerminated;
    }

    goto NO_PROBLEM;

OUT_OF_RESOURCES:
    
    if (pArchive)
    {
        pArchive->Remove((ANSC_HANDLE)pArchive);
    }

    if (pImportList)
    {
        pImportList->Remove((ANSC_HANDLE)pImportList);
    }

NO_PROBLEM:

    if (hCookedPage)
    {
        pBspSoaIf->RetCookedPage(pBspSoaIf->hOwnerContext, hCookedPage);
    }

    if (pPagePath)
    {
        AnscFreeMemory(pPagePath);
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateObjKickoutArrayMap
            (
                ANSC_HANDLE                 hThisObject,
                PBSP_TEMPLATE_STATE         pState
            )

    description:

        This function is called to remove array map.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to object expression.

**********************************************************************/

void
BspTemplateObjKickoutArrayMap
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_STATE         pState
    )
{
    UNREFERENCED_PARAMETER(pState);
    UNREFERENCED_PARAMETER(hThisObject);
#if 0   /* not used in new version */
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray;
    int                             ulSize, i;
    PBSP_TEMPLATE_ARRAY_MAP         pMap;

    if (pState)
    {
        /* 
         * remove all arrays that have "nBase" greater or equal to the
         * "base" in given state
         */
        pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hArrayMap;
        ulSize      = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);

        for (i = ulSize - 1; i >= 0; i --)
        {
            pMap    = (PBSP_TEMPLATE_ARRAY_MAP)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i);
            
            if (pMap->nBase >= pState->base)
            {
                pPtrArray->RemoveAt((ANSC_HANDLE)pPtrArray, i, 1);
                AnscFreeMemory(pMap);
            }
        }
    }
#endif
}


/* =====================================================================================
                Some static utilities
 * ================================================================================== */

/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateEngRemoveToken
            (
                ANSC_HANDLE                 hToken
            )

    description:

        This function is called to release token tree.

    argument:   ANSC_HANDLE                 hToken
                The token to be removed.

    return:     void.

**********************************************************************/

void
BspTemplateEngRemoveToken
    (
        ANSC_HANDLE                 hToken
    )
{
    PBSP_TEMPLATE_TOKEN             pToken = (PBSP_TEMPLATE_TOKEN)hToken;
    PBSP_TEMPLATE_TOKEN             pNext;

    while (pToken)
    {
        pNext   = pToken->pNext;

        AnscFreeMemory(pToken);

        pToken  = pNext;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspEngIsCharSpace
            (
                char                ch
            )

    description:

        This function is call to examine if given character is 'space'.

    argument:   char                ch
                character to be examined.

    return:     status of operation.

**********************************************************************/

static BOOL
BspEngIsCharSpace
    (
        char                        ch
    )
{
    return(ch == ' ' || (ch >= 0x09 && ch <= 0x0D));
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspEngIsCharSpace
            (
                char                ch
            )

    description:

        This function is call to examine if given character is letter.

    argument:   char                ch
                character to be examined.

    return:     status of operation.

**********************************************************************/

static BOOL
BspEngIsCharAlpha
    (
        char                        ch
    )
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspEngIsCharSpace
            (
                char                ch
            )

    description:

        This function is call to examine if given character is digit.

    argument:   char                ch
                character to be examined.

    return:     status of operation.

**********************************************************************/

static BOOL
BspEngIsCharDigit
    (
        char                        ch
    )
{
    return (ch >= '0' && ch <= '9');
}
