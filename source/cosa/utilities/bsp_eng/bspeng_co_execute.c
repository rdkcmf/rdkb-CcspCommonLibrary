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

    module:	bspeng_co_execute.c

        For BSP Engine Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the BSP Engine Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/05/03    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "bspeng_co_global.h"
#include "safec_lib_common.h"

#ifdef   _DEBUG
BOOL
BspEngIsCallFlowTraced
    (
    )
{
    static ULONG                    ulLastChkTm = 0;
    static BOOL                     bTrace      = FALSE;
    ULONG                           ulTmNow     = AnscGetTickInSeconds();

    if ( ulLastChkTm == 0 || ulTmNow - ulLastChkTm >= 5 )
    {
        FILE*                       pFile       = fopen("/tmp/bspeng_callflow_on", "r");

        bTrace = (pFile != NULL);
        if ( pFile ) fclose(pFile);

        ulLastChkTm = ulTmNow;
    }

    return  bTrace;
}
#endif


static ANSC_HANDLE
BspEngGetBeepObjectHandle
    (
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hTmplObj,
        ANSC_HANDLE                 hBspSoaIf,
        char                        *pObjName
    );


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoExecute
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute the given branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoExecute
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    UNREFERENCED_PARAMETER(hVar);
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_STACK_OBJECT      pStack    = (PBSP_TEMPLATE_STACK_OBJECT)pRt->GetStack(hRuntime);
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr;
    PBSP_TEMPLATE_VAR_OBJECT        aVar;
    PBSP_TEMPLATE_OBJECT            aTmpl, pTmpl = NULL;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray, pMyArray;
    ULONG                           ulExecSize;
    PBSP_TEMPLATE_TOKEN             aToken;
    ULONG                           ulArgsCount  = 0;
    BOOL                            bTerminated = FALSE;

	if (pRt->InExecute) 
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: execute not allowed within execute.\r\n");

		return;
	}

    aVar = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);

	if (aVar->Type != BspVar_String) 
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: execute paramter is not a string.\r\n");
		
        return;
	}

    pPtrArray = (PANSC_PTR_ARRAY_OBJECT)pRt->hExecute;

    ulExecSize = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);

    if (ulExecSize != 0)
    {
        pTmpl = 
            (PBSP_TEMPLATE_OBJECT)pPtrArray->GetAt
                (
                    (ANSC_HANDLE)pPtrArray, 
                    ulExecSize - 1
                );
    }

	if (!pTmpl || pTmpl->GetTmplName((ANSC_HANDLE)pTmpl) != pMyObject->pName)
    {
        pTmpl = (PBSP_TEMPLATE_OBJECT)CreateBspEngComponent(NULL, NULL, NULL);
        pTmpl->Copy((ANSC_HANDLE)pTmpl, hThisObject);
		pPtrArray->Add((ANSC_HANDLE)pPtrArray, pTmpl);
    }

    ulExecSize = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);
	aTmpl = (PBSP_TEMPLATE_OBJECT)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, ulExecSize - 1);

	if (aTmpl->Tokenize((ANSC_HANDLE)aTmpl, aVar->Value.str, 1) == NULL) 
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: execute: line ");
        pMyObject->OutputNumber((ANSC_HANDLE)pMyObject, aTmpl->ulErrLineNo, FALSE);
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, ": ");
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, aTmpl->pErrMsg);
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, " ");

        /* aVar->Remove((ANSC_HANDLE)aVar); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)aVar);
		
        return;
	}

	aTmpl->ulInWhile = 0;
	aToken = (PBSP_TEMPLATE_TOKEN)aTmpl->hToken;
	aTmpl->hTree = aTmpl->StatementList((ANSC_HANDLE)aTmpl);
	BspTemplateEngRemoveToken(aToken);

	if (aTmpl->ulErrLineNo) 
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: execute: line ");
        pMyObject->OutputNumber((ANSC_HANDLE)pMyObject, aTmpl->ulErrLineNo, FALSE);
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, ": ");
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, aTmpl->pErrMsg);
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, " ");

        /* aVar->Remove((ANSC_HANDLE)aVar); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)aVar);

		return;
	}

    pMyArray    = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hSymbol;
    pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)aTmpl->hSymbol;

	pStack->Add
        (
            (ANSC_HANDLE)pStack, 
            pPtrArray->GetSize((ANSC_HANDLE)pPtrArray) - 
            pMyArray->GetSize((ANSC_HANDLE)pMyArray)
        );

	pRt->InExecute = TRUE;
    ulArgsCount = pRt->ulArgCount   = 0;

	aTmpl->DoStatementList((ANSC_HANDLE)aTmpl, aTmpl->hTree, hRuntime, FALSE, &bTerminated);

	pRt->InExecute  = FALSE;
    pRt->ulArgCount = ulArgsCount;

	if (pRt->Stop == BspOp_Return)
		pRt->Stop = 0;

    pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)aTmpl->hTree;
	BspTemplateBranchCORemove((ANSC_HANDLE)pBr);
	aTmpl->hTree = NULL;

    pMyObject->UpdateSymbols((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)aTmpl);

    /* aVar->Remove((ANSC_HANDLE)aVar); */
    pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)aVar);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoStrCat
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute 'StrCat'.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoStrCat
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    ANSC_HANDLE                     hVal;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    bDeleteL = bDeleteR = TRUE;

    pValL = pMyObject->EvalExpression(hThisObject, &iBr->left, hRuntime, &bTerminated);

    hVal  = pMyObject->EvalArrayItem
                            (
                                hThisObject, 
                                &iBr->left,
                                hRuntime
                            );

    if (hVal)
    {
        /* pValL->Remove((ANSC_HANDLE)pValL); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);

        bDeleteL    = FALSE;
        pValL       = (PBSP_TEMPLATE_VAR_OBJECT)hVal;
    }

    pValR = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);

    hVal  = pMyObject->EvalArrayItem
                            (
                                hThisObject, 
                                &iBr->right,
                                hRuntime
                            );

    if (hVal)
    {
        /* pValR->Remove((ANSC_HANDLE)pValR); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);

        bDeleteR    = FALSE;
        pValR       = (PBSP_TEMPLATE_VAR_OBJECT)hVal;
    }

    aResult->Assign((ANSC_HANDLE)aResult, pValL);
    aResult->StrCat((ANSC_HANDLE)aResult, pValR);
    
    if (bDeleteL)
    {
        /* pValL->Remove((ANSC_HANDLE)pValL); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);
    }

    if (bDeleteR)
    {
        /* pValR->Remove((ANSC_HANDLE)pValR); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoSubStr
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute 'SubStr'.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoSubStr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr;
    BOOL                            bDeleteL, bDeleteR;
    ANSC_HANDLE                     hVal;
    ULONG                           ulVal;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;

	PBSP_TEMPLATE_VAR_OBJECT        aStr;
    PBSP_TEMPLATE_BRANCH_OBJECT     pRight;
    LONG                            aIndex, aChars;
    int                             i;
    BOOL                            bDeleteStr  = TRUE;
    BOOL                            bTerminated = FALSE;

    bDeleteL = bDeleteR = TRUE;

    aStr = pMyObject->EvalExpression(hThisObject, &iBr->left, hRuntime, &bTerminated);

    pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)iBr->left.Value.b;
        
    if (iBr->left.type == BspBranch_eBranch && pBr->op == BspOp_ArrayItem)
    {
        ulVal       = aStr->Value.num;
        /* aStr->Remove((ANSC_HANDLE)aStr); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)aStr);

        aStr        = pMyObject->StackVar(hThisObject, ulVal, hRuntime, FALSE);
        bDeleteStr  = FALSE;
    }

	if (aStr->Type != BspVar_String) 
    {
		BSP_TEMPLATE_VAR_OBJECT aEmpty;
		aEmpty.Type = BspVar_String;
		aEmpty.Value.str = "";
		aStr->StrCat((ANSC_HANDLE)aStr, &aEmpty);
	}

    pRight  = (PBSP_TEMPLATE_BRANCH_OBJECT)iBr->right.Value.b;

    pBr     = (PBSP_TEMPLATE_BRANCH_OBJECT)pRight->right.Value.b;

    pValL   = pMyObject->EvalExpression(hThisObject, &pRight->left, hRuntime, &bTerminated);

    hVal    = pMyObject->EvalArrayItem
                            (
                                hThisObject, 
                                &pRight->left,
                                hRuntime
                            );

    if (hVal)
    {
        /* pValL->Remove((ANSC_HANDLE)pValL); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);

        bDeleteL    = FALSE;
        pValL       = (PBSP_TEMPLATE_VAR_OBJECT)hVal;
    }

    pValR = pMyObject->EvalExpression(hThisObject, &pRight->right, hRuntime, &bTerminated);

    hVal  = pMyObject->EvalArrayItem
                            (
                                hThisObject, 
                                &pRight->right,
                                hRuntime
                            );

    if (hVal)
    {
        /* pValR->Remove((ANSC_HANDLE)pValR); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);

        bDeleteR    = FALSE;
        pValR       = (PBSP_TEMPLATE_VAR_OBJECT)hVal;
    }

	aIndex = pValL->GetLong((ANSC_HANDLE)pValL);
	aChars = pValR->GetLong((ANSC_HANDLE)pValR);

    if (bDeleteL)
    {
        /* pValL->Remove((ANSC_HANDLE)pValL); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);
    }

    if (bDeleteR)
    {
        /* pValR->Remove((ANSC_HANDLE)pValR); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);
    }

	if (aIndex < 0 || aChars < 0) 
    {
        pMyObject->OutputString
            (
                (ANSC_HANDLE)pMyObject, 
                "script error: native number of substr index or items.\n"
            );
		
        return;
	}

	if (aIndex >= (long)AnscSizeOfString(aStr->Value.str)) 
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: substr index larger than string.\r\n");
		
        return;
	}

	for (i = 0; aStr->Value.str[aIndex] != '\0' && aChars-- > 0; ++i)
		aStr->Value.str[i] = aStr->Value.str[aIndex++];

	aStr->Value.str[i] = '\0';

	aResult->Assign((ANSC_HANDLE)aResult, aStr);

    if (bDeleteStr)
    {
        /* aStr->Remove((ANSC_HANDLE)aStr); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)aStr);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoModulo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute modulo.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoModulo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Assign((ANSC_HANDLE)aResult, pValL);
    aResult->Mod((ANSC_HANDLE)aResult, pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoBitXorN
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute bit XOR ^=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoBitXorN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS_N;

    pValL->BitXor((ANSC_HANDLE)pValL, pValR);
    aResult->Assign((ANSC_HANDLE)aResult, (ANSC_HANDLE)pValL);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoShiftRightN
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute shift right <<=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoShiftRightN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS_N;

    pValL->ShiftRight((ANSC_HANDLE)pValL, pValR);
    aResult->Assign((ANSC_HANDLE)aResult, (ANSC_HANDLE)pValL);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoShiftLeftN
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute shift left >>=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoShiftLeftN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS_N;

    pValL->ShiftLeft((ANSC_HANDLE)pValL, pValR);
    aResult->Assign((ANSC_HANDLE)aResult, (ANSC_HANDLE)pValL);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoShiftRight
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute shift right >>.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoShiftRight
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = 
        pValL->GetLong((ANSC_HANDLE)pValL) >> pValR->GetLong((ANSC_HANDLE)pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoShiftLeft
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute shift left <<.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoShiftLeft
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = 
        pValL->GetLong((ANSC_HANDLE)pValL) << pValR->GetLong((ANSC_HANDLE)pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoExecute
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute bit AND &=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoBitAndN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS_N;

    pValL->BitAnd((ANSC_HANDLE)pValL, pValR);
    aResult->Assign((ANSC_HANDLE)aResult, (ANSC_HANDLE)pValL);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoBitOrN
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute bit OR |=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoBitOrN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS_N;

    pValL->BitOr((ANSC_HANDLE)pValL, pValR);
    aResult->Assign((ANSC_HANDLE)aResult, (ANSC_HANDLE)pValL);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoModuloN
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute modulo %=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoModuloN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS_N;

    pValL->Mod((ANSC_HANDLE)pValL, pValR);
    aResult->Assign((ANSC_HANDLE)aResult, (ANSC_HANDLE)pValL);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoDivideN
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute divide /=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoDivideN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS_N;

    pValL->Divide((ANSC_HANDLE)pValL, pValR);
    aResult->Assign((ANSC_HANDLE)aResult, (ANSC_HANDLE)pValL);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoDivide
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute divide /.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoDivide
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Assign((ANSC_HANDLE)aResult, pValL);
    aResult->Divide((ANSC_HANDLE)aResult, pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoMultiplyN
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute multiply *=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoMultiplyN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS_N;

    pValL->Multiply((ANSC_HANDLE)pValL, pValR);
    aResult->Assign((ANSC_HANDLE)aResult, (ANSC_HANDLE)pValL);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoMultiply
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute multiply *.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoMultiply
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Assign((ANSC_HANDLE)aResult, pValL);
    aResult->Multiply((ANSC_HANDLE)aResult, pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoDecrN
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute decrement -=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoDecrN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS_N;

    pValL->Minus((ANSC_HANDLE)pValL, pValR);
    aResult->Assign((ANSC_HANDLE)aResult, (ANSC_HANDLE)pValL);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoSubstract
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute substract -.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoSubtract
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL = TRUE, bDeleteR=TRUE; /*RDKB-5934 , CID-24539, CID-24719, initializing before use */
    ANSC_HANDLE                     hVal;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    pValR = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);

    hVal  = pMyObject->EvalArrayItem
                            (
                                hThisObject, 
                                (ANSC_HANDLE)&iBr->right, 
                                hRuntime
                            );

    if (hVal)
    {
        /* pValR->Remove((ANSC_HANDLE)pValR); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);

        bDeleteR = FALSE;
        pValR = hVal;
    }

	if (iBr->left.type == BspBranch_eEmpty)
    {
        aResult->Assign((ANSC_HANDLE)aResult, pValR);
		aResult->Neg((ANSC_HANDLE)aResult);
    }
	else
    {
        pValL = pMyObject->EvalExpression(hThisObject, &iBr->left, hRuntime, &bTerminated);

        hVal  = pMyObject->EvalArrayItem
                                (
                                    hThisObject, 
                                    (ANSC_HANDLE)&iBr->left, 
                                    hRuntime
                                );

        if (hVal)
        {
            /* pValL->Remove((ANSC_HANDLE)pValL); */
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);

            bDeleteL = FALSE;
            pValL = hVal;
        }

        aResult->Assign((ANSC_HANDLE)aResult, pValL);
		aResult->Minus((ANSC_HANDLE)aResult, pValR);

        if (bDeleteL)
        {
            /* pValL->Remove((ANSC_HANDLE)pValL); */
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);
        }
    }

    if (bDeleteR)
    {
        /* pValR->Remove((ANSC_HANDLE)pValR); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoIncrN
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute increment +=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoIncrN
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS_N;

/*
if (TRUE)
{
    bDeleteL = bDeleteR = TRUE;                                                     
                                                                                    
    pValL =                                                                         
        pMyObject->EvalExpression(hThisObject, &iBr->left, hRuntime, &bTerminated); 
                                                                                    
    hVal  = pMyObject->EvalArrayItem                                                
                            (                                                       
                                hThisObject,                                        
                                &iBr->left,                                         
                                hRuntime                                            
                            );                                                      
                                                                                    
    if (hVal)                                                                       
    {                                                                               
        pValL->Remove((ANSC_HANDLE)pValL);                                          
        bDeleteL    = FALSE;                                                        
        pValL       = (PBSP_TEMPLATE_VAR_OBJECT)hVal;                               
    }                                                                               
    else                                                                            
    {                                                                               
        pValL->Remove((ANSC_HANDLE)pValL);                                          
        bDeleteL    = FALSE;                                                        
        pValL       =                                                               
            pMyObject->StackVar(hThisObject, iBr->left.Value.n, hRuntime, FALSE);   
    }                                                                               
}
*/

    pValL->Plus((ANSC_HANDLE)pValL, pValR);
    aResult->Assign((ANSC_HANDLE)aResult, (ANSC_HANDLE)pValL);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoAdd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute add +.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoAdd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    ANSC_HANDLE                     hVal;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    bDeleteR = bDeleteL = TRUE;

    pValR = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);

    hVal  = pMyObject->EvalArrayItem
                            (
                                hThisObject, 
                                (ANSC_HANDLE)&iBr->right, 
                                hRuntime
                            );

    if (hVal)
    {
        /* pValR->Remove((ANSC_HANDLE)pValR); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);

        bDeleteR = FALSE;
        pValR = hVal;
    }

	if (iBr->left.type == BspBranch_eEmpty)
    {
        aResult->Assign((ANSC_HANDLE)aResult, pValR);
		aResult->Pos((ANSC_HANDLE)aResult);
    }
	else
    {
        pValL = pMyObject->EvalExpression(hThisObject, &iBr->left, hRuntime, &bTerminated);

        hVal  = pMyObject->EvalArrayItem
                                (
                                    hThisObject, 
                                    (ANSC_HANDLE)&iBr->left, 
                                    hRuntime
                                );

        if (hVal)
        {
            /* pValL->Remove((ANSC_HANDLE)pValL); */
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);

            bDeleteL = FALSE;
            pValL = hVal;
        }

        aResult->Assign((ANSC_HANDLE)aResult, pValL);
		aResult->Plus((ANSC_HANDLE)aResult, pValR);

        if (bDeleteL)
        {
            /* pValL->Remove((ANSC_HANDLE)pValL); */
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);
        }
    }

    if (bDeleteR)
    {
        /* pValR->Remove((ANSC_HANDLE)pValR); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoBitXor
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute bit XOR ^.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoBitXor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = 
        pValL->GetLong((ANSC_HANDLE)pValL) ^ pValR->GetLong((ANSC_HANDLE)pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoBitAnd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute bit AND &.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoBitAnd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = 
        pValL->GetLong((ANSC_HANDLE)pValL) & pValR->GetLong((ANSC_HANDLE)pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoBitNot
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute bit NOT ~.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoBitNot
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteR;
    ANSC_HANDLE                     hVal;
    PBSP_TEMPLATE_VAR_OBJECT        pValR;
    BOOL                            bTerminated = FALSE;

    bDeleteR = TRUE;

    pValR = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);

    hVal  = pMyObject->EvalArrayItem
                            (
                                hThisObject, 
                                &iBr->right,
                                hRuntime
                            );

    if (hVal)
    {
        /* pValR->Remove((ANSC_HANDLE)pValR); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);

        bDeleteR    = FALSE;
        pValR       = (PBSP_TEMPLATE_VAR_OBJECT)hVal;
    }

    aResult->Value.num = ~pValR->GetLong((ANSC_HANDLE)pValR);

    if (bDeleteR)
    {
        /* pValR->Remove((ANSC_HANDLE)pValR); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoBitOr
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute bit OR |.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoBitOr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = 
        pValL->GetLong((ANSC_HANDLE)pValL) | pValR->GetLong((ANSC_HANDLE)pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoNot
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute NOT !.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoNot
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteR;
    ANSC_HANDLE                     hVal;
    PBSP_TEMPLATE_VAR_OBJECT        pValR;
    BOOL                            bTerminated = FALSE;

    bDeleteR = TRUE;

    pValR = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);

    hVal  = pMyObject->EvalArrayItem
                            (
                                hThisObject, 
                                &iBr->right,
                                hRuntime
                            );

    if (hVal)
    {
        /* pValR->Remove((ANSC_HANDLE)pValR); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);

        bDeleteR    = FALSE;
        pValR       = (PBSP_TEMPLATE_VAR_OBJECT)hVal;
    }

    aResult->Value.num = !pValR->GetLong((ANSC_HANDLE)pValR);

    if (bDeleteR)
    {
        /* pValR->Remove((ANSC_HANDLE)pValR); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoAnd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute AND &&.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoAnd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = 
        pValL->GetLong((ANSC_HANDLE)pValL) && pValR->GetLong((ANSC_HANDLE)pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoOr
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute OR ||.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoOr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = 
        pValL->GetLong((ANSC_HANDLE)pValL) || pValR->GetLong((ANSC_HANDLE)pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoEq
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute EQ ==.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoEq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = pValL->Equal((ANSC_HANDLE)pValL, pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoGt
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute GREATER >.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoGt
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = pValL->Greater((ANSC_HANDLE)pValL, pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoLt
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute LESS <.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoLt
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = pValR->Greater((ANSC_HANDLE)pValR, pValL);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoNE
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute NOT EQUAL !=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoNe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = !pValL->Equal((ANSC_HANDLE)pValL, pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoGe
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute GREATER or EQUAL >=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoGe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = !pValR->Greater((ANSC_HANDLE)pValR, pValL);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoLe
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute LESS or EQUAL <=.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoLe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BOOL                            bDeleteL, bDeleteR;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

    BSPENG_PREPARE_BINARY_OPERANDS;

    aResult->Value.num = !pValL->Greater((ANSC_HANDLE)pValL, pValR);

    BSPENG_REMOVE_BINARY_OPERANDS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoAssign
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute Assign =.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoAssign
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr;
    PBSP_TEMPLATE_VAR_OBJECT        stVar;
    ULONG                           ulVar     = (ULONG)-1;
    BOOL                            bDelete   = TRUE;
    BOOL                            bTerminated = FALSE;

    pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)iBr->left.Value.b;

    if (iBr->left.type == BspBranch_eBranch && !pBr)
    {
        /* special assignment statement without left operand */
        BOOL                        bArrayItem = FALSE;
        PBSP_TEMPLATE_BRANCH_OBJECT pRight;

        pVal = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);

        if (iBr->right.type == BspBranch_eBranch)
        {
            pRight      = (PBSP_TEMPLATE_BRANCH_OBJECT)iBr->right.Value.b;
            bArrayItem  = (pRight && pRight->op == BspOp_ArrayItem);

            if (bArrayItem)
            {
                stVar  = 
                    (PBSP_TEMPLATE_VAR_OBJECT)pMyObject->EvalArrayItem
                        (
                            hThisObject, 
                            &iBr->right,
                            hRuntime
                        );

                if (!stVar)
                {
                    /* pVal->Remove((ANSC_HANDLE)pVal); */
                    pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);

                    pVal    = NULL;
                }
                else
                {
                    pVal->Assign
                            (
                                (ANSC_HANDLE)pVal, 
                                (ANSC_HANDLE)stVar
                            );
                }
            }
        }

        if (pVal)
        {
            pMyObject->OutputVar((ANSC_HANDLE)pMyObject, pVal);
            /* pVal->Remove((ANSC_HANDLE)pVal); */
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);
        }
        else
        {
            pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: cannot find the variable on stack.\r\n");
        }
    }
    else
    {
        /* normal assignment statement */
        if (iBr->left.type == BspBranch_eBranch && pBr->op == BspOp_ArrayItem)
        {
            stVar  = 
                (PBSP_TEMPLATE_VAR_OBJECT)pMyObject->EvalArrayItem
                    (
                        hThisObject, 
                        &iBr->left,
                        hRuntime
                    );
        }
        else
        {
            ulVar = iBr->left.Value.n;
            stVar = pMyObject->StackVar(hThisObject, ulVar, hRuntime, FALSE);
        }

        pVal = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);

        pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)iBr->right.Value.b;

        bDelete = TRUE;

        if (iBr->bObjSetProperty)
        {
            /* 
             * note here:
             *   The 4th parameter pVal is not the result value, but input one.
             * So the "DoObj" function must not change the value of pVal !!!
             */
            BspTemplateObjDoObj(hThisObject, iBr->left.Value.b, hRuntime, pVal, &bTerminated);

            if (bDelete)
            {
                /* pVal->Remove((ANSC_HANDLE)pVal); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);
            }
        }
        else
        {
            if (stVar)
            {
                stVar->Assign((ANSC_HANDLE)stVar, pVal);
            }
            else
            {
                AnscTrace("No such variable on stack, ulVar = %lu!\n", ulVar);
            }

            if (bDelete)
            {
                /* pVal->Remove((ANSC_HANDLE)pVal); */
                pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);
            }

            if (stVar)
                aResult->Assign((ANSC_HANDLE)aResult, stVar);
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoExecute
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute decrease --.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoDecr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr;
    ULONG                           ulVal;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

	if (iBr->left.Value.n)
    {
        pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)iBr->right.Value.b;

        if (iBr->right.type == BspBranch_eBranch && pBr->op == BspOp_ArrayItem)
        {
            pValL   = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);
            ulVal   = pValL->Value.num;
            /* pValL->Remove((ANSC_HANDLE)pValL); */
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);
        }
        else
        {
            ulVal   = iBr->right.Value.n;
        }

        pVal    = pMyObject->StackVar(hThisObject, ulVal, hRuntime, FALSE);

        pVal->Decrease((ANSC_HANDLE)pVal);
		aResult->Assign
                    (
                        (ANSC_HANDLE)aResult, 
                        pVal
                    );
    }
	else 
    {
        pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)iBr->right.Value.b;

        if (iBr->right.type == BspBranch_eBranch && pBr->op == BspOp_ArrayItem)
        {
            pValR   = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);
            ulVal   = pValR->Value.num;
            /* pValR->Remove((ANSC_HANDLE)pValR); */
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);
        }
        else
        {
            ulVal   = iBr->right.Value.n;
        }

		pVal = pMyObject->StackVar(hThisObject, ulVal, hRuntime, FALSE);

        /* set result before decrement */
        aResult->Assign((ANSC_HANDLE)aResult, (ANSC_HANDLE)pVal);

        pVal->Decrease((ANSC_HANDLE)pVal);
	}
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoIncr
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute INCR ++.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoIncr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr;
    ULONG                           ulVal;
    PBSP_TEMPLATE_VAR_OBJECT        pValL, pValR;
    BOOL                            bTerminated = FALSE;

	if (iBr->left.Value.n)
    {
        pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)iBr->right.Value.b;

        if (iBr->right.type == BspBranch_eBranch && pBr->op == BspOp_ArrayItem)
        {
            pValL   = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);
            ulVal   = pValL->Value.num;
            /* pValL->Remove((ANSC_HANDLE)pValL); */
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);
        }
        else
        {
            ulVal   = iBr->right.Value.n;
        }

        pVal    = pMyObject->StackVar(hThisObject, ulVal, hRuntime, FALSE);

        pVal->Increase((ANSC_HANDLE)pVal);

		aResult->Assign
                    (
                        (ANSC_HANDLE)aResult, 
                        pVal
                    );
    }
	else 
    {
        pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)iBr->right.Value.b;

        if (iBr->right.type == BspBranch_eBranch && pBr->op == BspOp_ArrayItem)
        {
            pValR   = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);

            ulVal   = pValR->Value.num;

            /* pValR->Remove((ANSC_HANDLE)pValR); */
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValR);
        }
        else
        {
            ulVal   = iBr->right.Value.n;
        }

        pVal    = pMyObject->StackVar(hThisObject, ulVal, hRuntime, FALSE);
        
        /* set the result before increment */
        aResult->Assign((ANSC_HANDLE)aResult, (ANSC_HANDLE)pVal);

        pVal->Increase((ANSC_HANDLE)pVal);
	}
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoArrayItem
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to evaluate array item.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoArrayItem
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    ULONG                           ulVar;
    PBSP_TEMPLATE_BRANCH_DATA       pBrData;
    PBSP_TEMPLATE_ARRAY_DATA        pArrayData;
    PBSP_TEMPLATE_ARRAY_ITEMS       pItems;
    BOOL                            bOutBound = FALSE;
    ULONG                           ulIndex   = 0;
    ULONG                           i, ulSize;
    PBSP_TEMPLATE_BRANCH_OBJECT     pDim;
    BOOL                            bTerminated;

    ulVar = iBr->left.Value.n;
    pVal  = pMyObject->StackVar(hThisObject, ulVar, hRuntime, FALSE);

    if (!pVal || pVal->Type != BspVar_Array)
    {
        aResult->Value.num = 0;

        return;
    }

    pItems  = &pVal->Value.arrayItems;

    pBrData = &iBr->right;

    if (pBrData->type == BspBranch_eArrayDim)
    {
        pArrayData  = &pBrData->Value.a;

        if (pArrayData->Count > BSP_TEMPLATE_ARRAY_DIMENSION_LIMIT)
        {
            aResult->Value.num  = 0;
            return;
        }

        for (i = 0; i < pArrayData->Count; i ++)
        {
            pDim    = pArrayData->pBranch[i];
            pVal    = pMyObject->EvalExpression(hThisObject, &pDim->right, hRuntime, &bTerminated);

            /* 
             * protect BSP engine from misbehaving if a script error found that
             * array index is not of type integer or unsigned integer
             */
            if ( pVal->Type != BspVar_Number && pVal->Type != BspVar_UNumber )
            {
                aResult->Value.num  = 0;
                return;
            }

            ulSize  = pVal->Value.num;

            /* pVal->Remove((ANSC_HANDLE)pVal); */
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);

            if (i == 0)
            {
                ulIndex = ulSize;

                bOutBound   |= (ulSize >= pItems->ulSize);
            }
            else
            {
                ulIndex *= pItems->Dims[i];
                ulIndex += ulSize;

                bOutBound   |= (ulSize >= pItems->Dims[i]);
            }
        }

        if (bOutBound)
        {
            aResult->Value.num  = 0;
        }
        else
        {
            if (!pItems->bBulkData)
            {
                PBSP_TEMPLATE_VAR_OBJECT    *pVars, pVar;

                pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pItems->hVars;
                pVar    = pVars[ulIndex];

                aResult->Assign((ANSC_HANDLE)aResult, pVar);
            }
            else
            {
                aResult->GetBulkDataAt((ANSC_HANDLE)aResult, pItems, ulIndex);
            }
        }
    }
    else
    {
        aResult->Value.num  = 0;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoDim
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute to create array items.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoDim
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    UNREFERENCED_PARAMETER(hVar);
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    PBSP_TEMPLATE_ARRAY_DATA        pArrayData;
    ULONG                           nItems = 0;
    ULONG                           i;
    PBSP_TEMPLATE_BRANCH_OBJECT     pDim;
    UINT                            ulSize;
    ULONG                           ulVar;
    BOOL                            bTerminated = FALSE;
    ULONG                           nLength     = 0;
    ULONG                           Dims[BSP_TEMPLATE_ARRAY_DIMENSION_LIMIT];
    ULONG                           numDims     = 0;

    ulVar = iBr->left.Value.n;
    pArrayData      = &(iBr->right.Value.a);

    for (i = 0; i < pArrayData->Count; i ++)
    {
        pDim = pArrayData->pBranch[i];

        pVal = pMyObject->EvalExpression(hThisObject, &pDim->right, hRuntime, &bTerminated);
        ulSize = pVal->Value.num;

        /* pVal->Remove((ANSC_HANDLE)pVal); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);

        Dims[i] = ulSize;

        if ((LONG)ulSize <= 0 && i)
        {
            pMyObject->OutputString
                (
                    (ANSC_HANDLE)pMyObject, 
                    "script error: the dimension of the array (except the first) must be greater than 0.\n"
                );

            return;
        }

        if (i == 0)
        {
            nItems  = ulSize;
            nLength = ulSize;
        }
        else
        {
            nItems *= ulSize;
        }
    }
    
    numDims = pArrayData->Count;

    pVal    = pMyObject->StackVar((ANSC_HANDLE)pMyObject, ulVar, hRuntime, FALSE);
    
    /* this variable should be reset */
    pVal->Reset((ANSC_HANDLE)pVal);

    pVal->Type                      = BspVar_Array;
    pVal->Value.arrayItems.ulSize   = 0;
    pVal->Value.arrayItems.ulLen    = 0;
    pVal->Value.arrayItems.hVars    = NULL;

    for (i = 0; i < pArrayData->Count; i ++)
    {
        pVal->Value.arrayItems.Dims[i]  = Dims[i];
    }

    pVal->Value.arrayItems.numDims  = numDims;

    if (nItems != 0)
    {
        PBSP_TEMPLATE_VAR_OBJECT    *pVars;
        ULONG                       i;

        pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)AnscAllocateMemory(nItems * sizeof(PBSP_TEMPLATE_VAR_OBJECT));

        if (pVars)
        {
            pVal->Value.arrayItems.hVars    = (ANSC_HANDLE)pVars;
            pVal->Value.arrayItems.ulSize   = nLength;
            pVal->Value.arrayItems.ulLen    = nLength;

            for (i = 0; i < nItems; i ++)
            {
                pVars[i]    = (PBSP_TEMPLATE_VAR_OBJECT)CreateBspEngVarComponent(NULL, NULL, NULL);

                /* if array size is too big, the performance degrades */
                /* pVars[i]    = (PBSP_TEMPLATE_VAR_OBJECT)pRt->AcquireVariable((ANSC_HANDLE)pRt); */
            }
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoParam
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute 'param'.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoParam
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    LONG                            aItem;
    BOOL                            bTerminated = FALSE;

    pVal = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);
	aItem = pVal->GetLong((ANSC_HANDLE)pVal);
    /* pVal->Remove((ANSC_HANDLE)pVal); */
    pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);

	if (aItem > (long)pRt->ulArgCount) 
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: param out of range.\r\n");

		return;
	}

	if (aItem == 0)
    {
		aResult->Value.num = pRt->ulArgCount;
    }
	else
    {
		aResult->Assign
                    (
                        (ANSC_HANDLE)aResult, 
                        pMyObject->StackVar(hThisObject, aItem-1, hRuntime, BspOpt_Param)
                    );
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoEnv
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute env command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoEnv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBranch);
    UNREFERENCED_PARAMETER(hRuntime);
    UNREFERENCED_PARAMETER(hVar);
#ifdef   _BSP_RICH_FEATURE_SET

    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PBSP_TEMPLATE_WRITER_OBJECT     pWriter   = (PBSP_TEMPLATE_WRITER_OBJECT)pRt->hWriter;
    PBSP_TEMPLATE_STACK_OBJECT      pStack    = (PBSP_TEMPLATE_STACK_OBJECT)pRt->GetStack(hRuntime);
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    PBSP_TEMPLATE_BRANCH_OBJECT     aBr;
    BSP_TEMPLATE_STATE              aState;
	const char                      *aFunc;
    PBSP_TEMPLATE_ACCESS_OBJECT     aObj;

	pStack->GetState((ANSC_HANDLE)pStack, &aState);

	if (!pRt->hEnv) 
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: no environment commands defined.\r\n");
		
        return;
	}

	aObj = (PBSP_TEMPLATE_ACCESS_OBJECT)pRt->hEnv;

	for (aBr = iBr->right.Value.b; aBr->left.Value.b && aObj; aBr = aBr->left.Value.b) 
    {
		aFunc = pMyObject->EvalName(hThisObject, aBr->right.Value.b, hRuntime, FALSE);

        pStack->NewFunction((ANSC_HANDLE)pStack, &aState);

		aObj = aObj->Member
                    (
                        (ANSC_HANDLE)aObj,
                        aFunc, 
                        (ANSC_HANDLE)pWriter, 
                        pStack->GetBufferAt
                            (
                                (ANSC_HANDLE)pStack, 
                                pStack->GetBase((ANSC_HANDLE)pStack)
                            ), 
                        pStack->GetCount((ANSC_HANDLE)pStack)
                    );

        pStack->DeleteFunction((ANSC_HANDLE)pStack, &aState);
	}

	if (!aObj)
    {
        return;
    }

	aFunc = pMyObject->EvalName(hThisObject, aBr->right.Value.b, hRuntime, FALSE);
    pStack->NewFunction((ANSC_HANDLE)pStack, &aState);

    if (iBr->bTopOp)
    {
		aObj->Output
                (
                    (ANSC_HANDLE)aObj,
                    aFunc, 
                    (ANSC_HANDLE)pWriter, 
                    pStack->GetBufferAt
                        (
                            (ANSC_HANDLE)pStack, 
                            pStack->GetBase((ANSC_HANDLE)pStack)
                        ), 
                    pStack->GetCount((ANSC_HANDLE)pStack)
                );
    }
	else
    {
        pVal = aObj->Value
                        (
                            (ANSC_HANDLE)aObj,
                            aFunc, 
                            (ANSC_HANDLE)pWriter, 
                            pStack->GetBufferAt
                                (
                                    (ANSC_HANDLE)pStack, 
                                    pStack->GetBase((ANSC_HANDLE)pStack)
                                ), 
                            pStack->GetCount((ANSC_HANDLE)pStack)
                        );

		aResult->Assign((ANSC_HANDLE)aResult, pVal);
        /* pVal->Remove((ANSC_HANDLE)pVal); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);
    }

	pStack->DeleteFunction((ANSC_HANDLE)pStack, &aState);

#endif
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoObj
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute object expression, it
        could be method call or property retrieval.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoObj
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        *pbTerminated
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject       = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr             = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt             = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult         = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
	const char                      *aFunc          = NULL;
    BOOL                            bExecContext    = FALSE;
    BOOL                            bLeftOperand    = iBr->bObjSetProperty;
    ANSC_HANDLE                     hSlapVar        = (ANSC_HANDLE)NULL;
    PBSP_TEMPLATE_NAME_PARAM_LIST   pParamList      = NULL;
    BOOL                            bTerminated     = FALSE;
    PSLAP_VARIABLE                  pReturnVal      = NULL;
    ANSC_HANDLE                     hBeepObj        = (ANSC_HANDLE)NULL;
    ANSC_HANDLE                     hSlapReturnVal  = (ANSC_HANDLE)NULL;
    BOOL                            bSimpleVar      = FALSE;
    BOOL                            bFreeBeepObj    = FALSE;
    ULONG                           ulIndex = (ULONG)0; /*RDKB-5934 , CID-24800, initializing before use*/
    PBSP_TEMPLATE_BRANCH_OBJECT     pLeft;
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    PBSP_TEMPLATE_BRANCH_OBJECT     aBr;
    BOOL                            bBrokeIn        = FALSE;
    PUCHAR                          pObjName        = NULL;
    PSLAP_VARIABLE                  pSlapVar        = NULL;
    errno_t   rc   = -1;

    pLeft   = (PBSP_TEMPLATE_BRANCH_OBJECT)iBr->left.Value.b;
    if (!pLeft)
    {
        /*
         * This happens when the following expression appears.
         *  
         *      x("size")   = 3     // x is of simple data type
         */
        BspTemplateObjDoObjProperty(hThisObject, hBranch, hRuntime, hVar);
        return;
    }

    /* try to get properties of the first object */
    if (iBr->left.type == BspBranch_eBranch)
    {
        pObjName    = 
            (PUCHAR)pMyObject->EvalName2
                (
                    hThisObject, 
                    iBr->left.Value.b, 
                    hRuntime, 
                    (ANSC_HANDLE)(&pParamList)
                );
    }

    if (pObjName)
    {
        bExecContext    = TRUE;
        hBeepObj        = NULL;
        ulIndex         = (ULONG)-1;
        pVal            = NULL;
        pObjName        = AnscDupString(pObjName);
    }
    else
    {
        /* get object handle */
        pVal    = pMyObject->EvalExpression(hThisObject, &iBr->left, hRuntime, &bTerminated);

        if (pVal->Type == BspVar_Ref || pVal->Type == BspVar_Number || pVal->Type == BspVar_UNumber)
        {
            ulIndex = pVal->Value.num;
        }

        /* pVal->Remove((ANSC_HANDLE)pVal); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);

        if (ulIndex != (ULONG)-1)
        {
            pVal    = pMyObject->StackVar(hThisObject, ulIndex, hRuntime, FALSE);
        }
        else
        {
            pVal    = NULL;
        }
    }

    if (!bExecContext && (!pVal || pVal->Type != BspVar_Object || !pVal->Value.obj))
    {
        /* 
         * simple data type --- if BMW 2.0 supports variable content type
         * and rendering attribute inherited from BMW 1.0, here's the place
         * that we can make a SLAP function call to get/set variable's 
         * content type and rendering attribute
         */
        hSlapVar    = pVal ? pVal->Value.obj : (ANSC_HANDLE)NULL;
	AnscTrace("hSlapVar = %s!\n", (char *)hSlapVar);
        bSimpleVar  = TRUE;
    }

    /* 
     * enumerate methods and parameters and call APIs via Universal 
     * Object Access Interface.
     */

    if (pVal && pVal->Type == BspVar_Object)
    {
        pSlapVar    = (PSLAP_VARIABLE)pVal->Value.obj;

        if (
               pSlapVar     && 
               (
                    SLAP_VAR_SYNTAX_TYPE_handle == pSlapVar->Syntax || 
                    SLAP_VAR_SYNTAX_TYPE_object == pSlapVar->Syntax
                )
           )
        {
            hBeepObj = pSlapVar->Variant.varObject;
        }
    }

    aBr = iBr->right.Value.b;

    /* start object access with a specific object */
    if (hBeepObj && pParamList && pParamList->ulGroups != 0)
    {
        bBrokeIn    = TRUE;

        if (!aFunc)
        {
            if (!bLeftOperand || aBr)
            {
                aFunc   = BSP_METHOD_GET_ITEM;
            }
            else
            {
                aFunc   = BSP_METHOD_SET_ITEM;
            }
        }

        if (aBr)
        {
            goto ACCESS_NEXT;
        }
        else
        {
            goto ACCESS_LAST;
        }
    }
    else
    {
        if (pParamList)
        {
            REMOVE_BSP_TEMPLATE_NAME_PARAM_LIST(pParamList);
            pParamList  = NULL;
        }
    }

    /* enumerate all property names */
/*
	for (aBr = iBr->right.Value.b; aBr && aBr->left.Value.b; aBr = aBr->left.Value.b) 
*/
    while (aBr && aBr->left.Value.b)
    {
		aFunc = 
            pMyObject->EvalName2
                (
                    hThisObject, 
                    aBr->right.Value.b, 
                    hRuntime, 
                    (ANSC_HANDLE)(&pParamList) 
                );

        /* get starting object from "ExecContext" */
        if (bExecContext && !hBeepObj)
        {
            if (!pParamList || pParamList->ulGroups == 0)
            {
                ULONG               ulNewStrSize = 0;

                aBr = aBr->left.Value.b;

                /*CID: 66327 Explicit null dereferenced*/
                if (pObjName)
                    ulNewStrSize    = AnscSizeOfString((const char *)pObjName) + 1 + AnscSizeOfString((const char *)aFunc) + 1;

                rc = sprintf_s((char *)pObjName, ulNewStrSize, ".%s", aFunc);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }

                if (pParamList)
                {
                    REMOVE_BSP_TEMPLATE_NAME_PARAM_LIST(pParamList);
                    pParamList  = NULL;
                }

                continue;
            }

            hBeepObj    = 
                pMyObject->GetBeepObject
                    (
                        hThisObject,
                        hRuntime,
                        (ANSC_HANDLE)NULL,
                        FALSE,
                        (const char *)pObjName,
                        &bSimpleVar
                    );

            AnscFreeMemory(pObjName);
            pObjName    = NULL;
        }

ACCESS_NEXT:
        if (hBeepObj == NULL)
        {
            /* to make engine robust */
            break;
        }

        hSlapReturnVal  = 
            pMyObject->DoObjectAccess
                (
                    hThisObject,
                    hRuntime,
                    hBeepObj,
                    bSimpleVar,
                    aFunc,
                    (ANSC_HANDLE)pParamList,
                    (ANSC_HANDLE)NULL,
                    &bTerminated
                );

        if (hSlapReturnVal)
        {
            /* proceed to next object */
            pReturnVal  = (PSLAP_VARIABLE)hSlapReturnVal;

            if (bFreeBeepObj)
            {
                bFreeBeepObj    = FALSE;
                if (hBeepObj)
                {
                    SlapFreeVariable(((PSLAP_VARIABLE)hBeepObj));
                }
                hBeepObj        = NULL;
            }

            switch (pReturnVal->Syntax)
            {
                case    SLAP_VAR_SYNTAX_TYPE_handle:
                case    SLAP_VAR_SYNTAX_TYPE_object:

                    hBeepObj        = pReturnVal->Variant.varHandle;
                    bFreeBeepObj    = FALSE; 

                    break;

                default:

                    hBeepObj        = hSlapReturnVal;
                    bSimpleVar      = TRUE;
                    bFreeBeepObj    = TRUE;

                    break;
            }

            SlapFreeVariable(((PSLAP_VARIABLE)hSlapReturnVal));
            hSlapReturnVal  = NULL;
            pReturnVal      = NULL;
        }

        if (pParamList)
        {
            REMOVE_BSP_TEMPLATE_NAME_PARAM_LIST(pParamList);
            pParamList  = NULL;
        }

        if (bBrokeIn)
        {
            bBrokeIn    = FALSE;
        }
        else
        {
            aBr = aBr->left.Value.b;
        }
	}

    if (pParamList)
    {
        REMOVE_BSP_TEMPLATE_NAME_PARAM_LIST(pParamList);
        pParamList  = NULL;
    }

    if (NULL == hBeepObj && pObjName)
    {
        hBeepObj    = 
            pMyObject->GetBeepObject
                (
                    hThisObject,
                    hRuntime,
                    (ANSC_HANDLE)NULL,
                    FALSE,
                    (const char *)pObjName,
                    &bSimpleVar
                );

        if (!hBeepObj && !pVal && pObjName)
        {
            ULONG                   ulVal;

            ulVal   = 
                pMyObject->FindSymbol
                    (
                        hThisObject,
                        (const char *)pObjName, 
                        FALSE
                    );

            if (ulVal != (ULONG)-1)
            {
                /* the can be mapped to a local variable */
                pVal    = pMyObject->StackVar(hThisObject, ulVal, hRuntime, FALSE);
            }
        }

        if (hBeepObj)
        {
            AnscFreeMemory(pObjName);
            pObjName    = NULL;
        }
    }

    if (aBr)
    {
        aFunc = 
            pMyObject->EvalName2
                (
                    hThisObject, 
                    aBr->right.Value.b, 
                    hRuntime, 
                    (ANSC_HANDLE)(&pParamList)
                );

        if (hBeepObj)
        {
ACCESS_LAST:
            hSlapReturnVal  = 
                pMyObject->DoObjectAccess
                    (
                        hThisObject,
                        hRuntime,
                        hBeepObj,
                        bSimpleVar,
                        aFunc,
                        (ANSC_HANDLE)pParamList,
                        bLeftOperand?(ANSC_HANDLE)aResult:(ANSC_HANDLE)NULL,
                        &bTerminated
                    );

            /* set up the return value */
            if (hSlapReturnVal && !bLeftOperand && aResult)
            {
                BspEng_VC_Slap2Var((ANSC_HANDLE)hSlapReturnVal, (ANSC_HANDLE)aResult);
            }

            if (hSlapReturnVal)
            {
                SlapFreeVariable(((PSLAP_VARIABLE)hSlapReturnVal));
                hSlapReturnVal  = NULL;
            }
        }
        else
        {
            BOOL                    bBuiltInProperty    = FALSE;

            if (pVal && aFunc)
            {
                if (strcmp((char *)aFunc, BSP_VAR_CONTENT_TYPE) == 0 )
                {
                    /*
                     * set/get variable's content type
                     */

                    pMyObject->AccessVarContentType
                        (
                            hThisObject,
                            hRuntime,
                            (ANSC_HANDLE)pVal,
                            bLeftOperand,
                            (ANSC_HANDLE)aResult
                        );

                    bBuiltInProperty    = TRUE;
                }
                else
                if (strcmp((char *)aFunc, BSP_VAR_SLAP_SYNTAX_TYPE) == 0 )
                {
                    /*
                     * get variable's SLAP syntax
                     */

                    if (!bLeftOperand)
                    {
                        aResult->Reset((ANSC_HANDLE)aResult);

                        aResult->Type       = BspVar_UNumber;
                        aResult->Value.num  = pVal->SlapSyntax;
                    }

                    bBuiltInProperty    = TRUE;
                }
                else
                if (pVal->Type == BspVar_Array)
                {
                    PBSP_TEMPLATE_ARRAY_ITEMS   pItems;

                    pItems  = &pVal->Value.arrayItems;

                    if (strcmp((char *)aFunc, BSP_ARRAY_VAR_TYPE) == 0 )
                    {
                        bBuiltInProperty    = TRUE;

                        if (!bLeftOperand)
                        {
                            /* get array type */
                            aResult->Type       = BspVar_Number;
                            aResult->Value.num  = pItems->Type;
                        }
                        else
                        if (aResult->Type == BspVar_Number || aResult->Type == BspVar_UNumber)
                        {
                            /* set array type */
                            pItems->Type    = aResult->Value.num;
                        }
                        else
                        {
                            bBuiltInProperty    = FALSE;
                        }
                    }
                    else
                    if (strcmp((char *)aFunc, BSP_ARRAY_VAR_LENGTH) == 0 )
                    {
                        bBuiltInProperty    = TRUE;

                        if (!bLeftOperand)
                        {
                            aResult->Type       = BspVar_Number;
                            aResult->Value.num  = pItems->ulLen;
                        }
                        else
                        if (aResult->Type == BspVar_Number || aResult->Type == BspVar_UNumber)
                        {
                            /* set array length */
                            pVal->SetArraySize((ANSC_HANDLE)pVal, (ULONG)aResult->Value.num);
                        }
                        else
                        {
                            bBuiltInProperty    = FALSE;
                        }
                    }
                }
            }
            
            /*
             * try the whole expression as an object --- allows BSP writers to
             * save an object into a local variable and not have to type
             * fully qualified name again and again
             */
            if (!bBuiltInProperty && pObjName)
            {
                ULONG               ulNewStrSize = 0;
                PSLAP_VARIABLE      pSlapObject;

                aBr = aBr->left.Value.b;
                /*CID: 60554 Dereference after null check*/
                if (aFunc)
                    ulNewStrSize    = AnscSizeOfString((const char *)pObjName) + 1 + AnscSizeOfString((const char *)aFunc) + 1;

                rc = sprintf_s((char *)pObjName, ulNewStrSize, ".%s", aFunc);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }

                hBeepObj    = 
                    pMyObject->GetBeepObject
                        (
                            hThisObject,
                            hRuntime,
                            (ANSC_HANDLE)NULL,
                            FALSE,
                            (const char *)pObjName,
                            &bSimpleVar
                        );

                SlapAllocVariable(pSlapObject);
                if (pSlapObject)
                {
                    SlapInitVariable(pSlapObject);

                    pSlapObject->Syntax = SLAP_VAR_SYNTAX_TYPE_object;
                    pSlapObject->Variant.varObject  = (SLAP_OBJECT)hBeepObj;

                    BspEng_VC_Slap2Var((ANSC_HANDLE)pSlapObject, (ANSC_HANDLE)aResult);
                    if (pSlapObject)
                    {
                        SlapFreeVariable(pSlapObject);
                        pSlapObject = NULL;
                    }
                }

#ifdef   _DEBUG
                if (!hBeepObj && BspEngIsCallFlowTraced())
                {
                    AnscTrace("?????????????????????????????????????????????????????????????\r\n");
                    AnscTrace("%s couldn't be called. NULL object is referenced.\r\n", pObjName);
                    AnscTrace("?????????????????????????????????????????????????????????????\r\n");
                }
#endif
            }
        }

        /* make a final SLAP call */
        if (pParamList)
        {
            REMOVE_BSP_TEMPLATE_NAME_PARAM_LIST(pParamList);
            pParamList  = NULL;
        }
    }

    if (pObjName)
    {
        AnscFreeMemory(pObjName);
    }

    if (bFreeBeepObj)
    {
        bFreeBeepObj    = FALSE;
        if (hBeepObj)
        {
            SlapFreeVariable(((PSLAP_VARIABLE)hBeepObj));
            hBeepObj    = NULL;
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
        BspTemplateObjDoObjProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to retrieve object's property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoObjProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject       = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_LIST_OBJECT       pList           = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;
    PBSPENG_SOA_INTERFACE           pBspSoaIf       = pList->GetBspSoaIf((ANSC_HANDLE)pList);
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr             = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt             = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult         = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
	const char                      *pObjName       = NULL;
    BOOL                            bLeftOperand    = iBr->bObjSetProperty;
    PBSP_TEMPLATE_NAME_PARAM_LIST   pParamList      = NULL;
    char                            *pMethodName    = NULL;
    SLAP_PARAMETER_LIST             SlapParamList;
    SLAP_PARAMETER_LIST             *outList        = NULL;
    PSLAP_VARIABLE                  pSlapReturnVal  = NULL;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    ANSC_HANDLE                     hBeepObj        = (ANSC_HANDLE)NULL;
    ULONG                           ulBspSoaStatus;
    char                            *pPropertyName  = NULL;
    BOOL                            bSimpleVar      = FALSE;
    errno_t                         rc              = -1;

    SlapInitParamList((&SlapParamList));

    /* get object handle */
    if (iBr->op == BspOp_Call)
    {
        pObjName = 
            pMyObject->EvalName2
                (
                    hThisObject, 
                    iBr->right.Value.b, 
                    hRuntime, 
                    (ANSC_HANDLE)(&pParamList) 
                );

#if 0
        AnscTrace("Accessing object %s's attribute.\n", pObjName);
#endif

        if (!pParamList || pParamList->ulGroups != 1)
        {
            /* no "property" specified */

            goto EXIT;

        }
        else
        {
            PBSP_TEMPLATE_NAME_PARAM    pParam;
            PBSP_TEMPLATE_VAR_OBJECT    pVar;

            pParam  = pParamList->pGroups[0];

            if (pParam->ulParams != 1)
            {
                goto EXIT;
            }
            else
            {
                pVar    = pParam->pParams[0];

                if (pVar && pVar->Type == BspVar_String)
                {
                    pPropertyName   = pVar->Value.str;
                }
                else
                {
                    goto EXIT;
                }
            }
        }

        if (bLeftOperand)
        {
            /* set property */
            pMethodName = BSP_METHOD_SET_ITEM;
        }
        else
        {
            /* get property */
            pMethodName = BSP_METHOD_GET_ITEM;
        }

        /* make a SLAP call to get/set attribute of the given object */

        hBeepObj    = 
            pMyObject->GetBeepObject
                (
                    hThisObject,
                    hRuntime,
                    (ANSC_HANDLE)NULL,
                    FALSE,
                    pObjName,
                    &bSimpleVar
                );

        BspEng_PreparePropertyParamList
            (
                hRuntime,
                &SlapParamList,
                !bLeftOperand,
                (PUCHAR)pPropertyName,
                hVar
            );

        if (hBeepObj)
        {
#ifdef   _DEBUG
            if ( BspEngIsCallFlowTraced() )
            {
                AnscTrace("    ---> Making slap call %s, time = %lu (ms) ...\n", pMethodName, AnscGetTickInMilliSeconds());

                if (strcasecmp(pMethodName, "SetItem") == 0 || strcasecmp(pMethodName, "GetItem") == 0 )
                {
                    if (SlapParamList.ParamCount != 0)
                    {
                        PSLAP_VARIABLE  pSlapName = (PSLAP_VARIABLE)SlapParamList.ParamArray;

                        if ( pSlapName->Syntax == SLAP_VAR_SYNTAX_string )
                        {
                            AnscTrace("              @@@@ item name is <%s> @@@@\n", pSlapName->Variant.varString);
                        }
                    }
                }
            }
#endif

            status  = 
                pBspSoaIf->InvokeObject
                    (
                        pBspSoaIf->hOwnerContext,
                        hBeepObj,
                        pMethodName,
                        &SlapParamList,
                        &outList,
                        &pSlapReturnVal,
                        &ulBspSoaStatus
                    );

#ifdef   _DEBUG
        if ( BspEngIsCallFlowTraced() )
        {
            AnscTrace("        <--- slap call %s returned, time = %lu.\n", pMethodName, AnscGetTickInMilliSeconds());

            if (status != ANSC_STATUS_SUCCESS)
            {
                AnscTrace("Failed to make slap call '%s'.\n", pMethodName);
            }
            else
            {
                if ( !pSlapReturnVal )
                {
                    AnscTrace("        <--- Return value: NULL.\r\n");
                }
                else
                {
                    PSLAP_VARIABLE  pRetVal = pSlapReturnVal;
                    char            buf[32];

                    switch ( pRetVal->Syntax )
                    {
                        case    SLAP_VAR_SYNTAX_bool:
                        case    SLAP_VAR_SYNTAX_int:
                        case    SLAP_VAR_SYNTAX_uint32:

                                rc = sprintf_s(buf, sizeof(buf), "0x%.8x, signed=%s", (UINT)pRetVal->Variant.varUint32, pRetVal->Syntax==SLAP_VAR_SYNTAX_int?"yes":"no");
                                if(rc < EOK)
                                {
                                    ERR_CHK(rc);
                                }
                                AnscTrace("        <--- Return numberic value: %s.\n", buf);

                                break;

                        case    SLAP_VAR_SYNTAX_string:

                                if ( !pRetVal->Variant.varString )
                                {
                                    AnscTrace("        <--- Return string value: <>.\r\n");
                                }
                                else
                                if ( AnscSizeOfString(pRetVal->Variant.varString) < 960 )
                                {
                                    AnscTrace("        <--- Return string value: <%s>.\n", pRetVal->Variant.varString);
                                }
                                else
                                {
                                    /*ULONG   i = 0;

                                    
                                    AnscTrace("        <--- Return string value: <");
                                    for ( i = 0; i < AnscSizeOfString(pRetVal->Variant.varString); i ++)
                                    {
                                        AnscTrace("%c", pRetVal->Variant.varString[i]);
                                    }
                                    AnscTrace(">\r\n");
                                    */
                                    AnscTrace("        <--- Return string value: <TOO LONG TO DISPLAY>.\r\n");
                                }

                                break;

                        default:

                                AnscTrace("        <--- Return value has complex type.\r\n");

                                break;
                    }
                }
            }
        }
#endif
        }

        if (outList)
        {
            SlapFreeParamList(outList);
        }

        if (pSlapReturnVal)
        {
            /* set up the return value */
            if (!bLeftOperand && aResult)
            {
                BspEng_VC_Slap2Var((ANSC_HANDLE)pSlapReturnVal, (ANSC_HANDLE)aResult);
            }

            if (pSlapReturnVal)
            {
                SlapFreeVariable(pSlapReturnVal);
                pSlapReturnVal  = NULL;
            }
        }

        SlapCleanParamList((&SlapParamList));
        SlapInitParamList((&SlapParamList));
    }
    else
    {
        /* shouldn't reach here */
    }

EXIT:

    if (pParamList)
    {
        REMOVE_BSP_TEMPLATE_NAME_PARAM_LIST(pParamList);
    }

    return;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoCall
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute procedure call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoCall
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        *pbTerminated
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PBSP_TEMPLATE_STACK_OBJECT      pStack    = (PBSP_TEMPLATE_STACK_OBJECT)pRt->GetStack(hRuntime);
    BSP_TEMPLATE_STATE              aState;
    const char                      *aName;
    PBSP_TEMPLATE_OBJECT            aTmpl;
    PBSP_TEMPLATE_LIST_OBJECT       pList;
    BOOL                            bTerminated = FALSE;

	pStack->GetState((ANSC_HANDLE)pStack, &aState);
	aName = pMyObject->EvalName(hThisObject, iBr->right.Value.b, hRuntime, BspOpt_PassRef);

    pList = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;
	aTmpl = pList->FindItem((ANSC_HANDLE)pList, aName);

	if (!aTmpl) 
    {
        /*
         *  if no BSP function can be found, we have to make
         * an assumption that this must be an object access
         * expression with form "obj(p1, p2, ...)"
         */

        /* we don't need to call "DeleteFunction" since no function
         * call is constructed.
         *
    	 *   pStack->DeleteFunction((ANSC_HANDLE)pStack, &aState);
         */

        BspTemplateObjDoObjProperty(hThisObject, hBranch, hRuntime, hVar);

        return;
	}

    /* clear up return value */
    pRt->ClearReturnValue((ANSC_HANDLE)pRt);

    aTmpl->ulStackBase  = aState.base;

	pStack->NewFunction((ANSC_HANDLE)pStack, &aState);
	aTmpl->DoTemplate((ANSC_HANDLE)aTmpl, hRuntime, &bTerminated);
	pStack->DeleteFunction((ANSC_HANDLE)pStack, &aState);

    /* set up return value (we don't care if the procedure really returns one */
    aResult->Assign((ANSC_HANDLE)aResult, pRt->GetReturnValue((ANSC_HANDLE)pRt));

    if (pbTerminated)
    {
        *pbTerminated   = bTerminated;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoSetoutput
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute 'setoutput'.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoSetoutput
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBranch);
    UNREFERENCED_PARAMETER(hRuntime);
    UNREFERENCED_PARAMETER(hVar);
#ifdef   _BSP_RICH_FEATURE_SET

    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PBSP_TEMPLATE_WRITER_OBJECT     pWriter   = (PBSP_TEMPLATE_WRITER_OBJECT)pRt->hWriter;
    PBSP_TEMPLATE_STACK_OBJECT      pStack    = (PBSP_TEMPLATE_STACK_OBJECT)pRt->GetStack(hRuntime);
    BSP_TEMPLATE_STATE              aState;
    const char                      *aName;
    PBSP_TEMPLATE_WRITER_OBJECT     aWriter, aCurr;
    PBSP_TEMPLATE_OUTPUT_SET_OBJECT pOutputset;
    BOOL                            bTerminated = FALSE;

	if (!pRt->hOutputSet) 
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: no alternate outputs specified.\r\n");
		
        return;
	}

	pStack->GetState((ANSC_HANDLE)pStack, &aState);

	aName = pMyObject->EvalName(hThisObject, iBr->right.Value.b, hRuntime, FALSE);
	pStack->NewFunction((ANSC_HANDLE)pStack, &aState);

    pOutputset  = pRt->hOutputSet;
    
	aWriter = pOutputset->Set
                        (
                            (ANSC_HANDLE)pOutputset,
                            aName, 
                            (ANSC_HANDLE)pWriter,
							pStack->GetBufferAt
                                (
                                    (ANSC_HANDLE)pStack, 
                                    pStack->GetBase((ANSC_HANDLE)pStack)
                                ), 
                            pStack->GetCount((ANSC_HANDLE)pStack)
                        );

	pStack->DeleteFunction((ANSC_HANDLE)pStack, &aState);

	if (!pRt->hWriter) 
    {
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "script error: output \"");
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, aName);
        pMyObject->OutputString((ANSC_HANDLE)pMyObject, "\" not known ");
		
        return;
	}

	aCurr = (PBSP_TEMPLATE_WRITER_OBJECT)pRt->hWriter;
	pRt->hWriter = (ANSC_HANDLE)aWriter;
	pMyObject->DoStatementList(hThisObject, iBr->left.Value.b, hRuntime, FALSE, &bTerminated);
	aWriter->Remove((ANSC_HANDLE)aWriter);
	pRt->hWriter = (ANSC_HANDLE)aCurr;

#endif
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoControl
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute control statement.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoControl
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    UNREFERENCED_PARAMETER(hVar);
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    BOOL                            bTerminated = FALSE;

    if (iBr->op == BspOp_Return)
    {

        pVal = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);

/*  EvalExpression() has already taken care of array item, no further check is necessary
        if (iBr->right.type == BspBranch_eBranch)
        {
            pRight      = (PBSP_TEMPLATE_BRANCH_OBJECT)iBr->right.Value.b;
            bArrayItem  = (pRight && pRight->op == BspOp_ArrayItem);

            if (bArrayItem)
            {
                pVal->Assign
                        (
                            (ANSC_HANDLE)pVal, 
                            pMyObject->StackVar(hThisObject, pVal->Value.num, hRuntime, FALSE)
                        );
            }
        }
*/

        pRt->SetReturnValue((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);

        /* pVal->Remove((ANSC_HANDLE)pVal); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);
    }

    pRt->Stop = iBr->op;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoWhile
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute while statement block.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoWhile
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    UNREFERENCED_PARAMETER(hVar);
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr, pIter;
    long                            lValue;
	int                             i = pRt->LoopLimit;
    BOOL                            bTerminated = FALSE;

    /* initialization expression list */
    if (iBr->right.Value.b)
    {
		pMyObject->DoStatementList(hThisObject, iBr->right.Value.b, hRuntime, FALSE, &bTerminated);

        if (bTerminated)
        {
            return;
        }
    }

    /* body of while statement */
    pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)iBr->left.Value.b;

    while (i > 0)
    {
        if (pBr->right.type == BspBranch_eBranch && !pBr->right.Value.b)
        {
            /* empty condition, statement list will be always executed */
        }
        else
        {
            pVal    = pMyObject->EvalExpression(hThisObject, &pBr->right, hRuntime, &bTerminated);
            lValue  = pVal->GetLong((ANSC_HANDLE)pVal);
            /* pVal->Remove((ANSC_HANDLE)pVal); */
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);

            if (!lValue)
            {
                return;
            }
        }

		pMyObject->DoStatementList(hThisObject, pBr->left.Value.b, hRuntime, FALSE, &bTerminated);

		if (pRt->Stop == BspOp_Return ||pRt->Stop == BspOp_Exit)
        {
			return;
        }

		if (pRt->Stop == BspOp_Break) 
        {
			pRt->Stop = 0;
			return;
		}

		if (pRt->Stop == BspOp_Continue) 
        {
			pRt->Stop = 0;

            pIter = (PBSP_TEMPLATE_BRANCH_OBJECT)pBr->left.Value.b;

            if(pIter) /*RDKB-5934 , CID-24262, Null check before use*/
            {

                for (; pIter->op != BspOp_Iter && pIter->left.Value.b; )
                {
                    pIter = (PBSP_TEMPLATE_BRANCH_OBJECT)pIter->left.Value.b;
                }

                if (pIter)
                {

                    pMyObject->DoStatementList
                        (
                            hThisObject, 
                            pIter, 
                            hRuntime, 
                            BspOpt_Iter,
                            &bTerminated
                        );

                    if (bTerminated)
                    {
                        return;
                    }

                }

            }

        }

        --i;
    }
    
	if (i == 0)
    {
        pMyObject->OutputString
            (
                (ANSC_HANDLE)pMyObject, 
                "script error: while statement iteration max reached.\n"
            );
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoSwitch
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute switch statement block.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoSwitch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    UNREFERENCED_PARAMETER(hVar);
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        pVal, pExp;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr, pCase;
    BOOL                            bTerminated = FALSE;
    BOOL                            bDone       = FALSE;

    pExp = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);

    pBr  = iBr->left.Value.b;

    while (pBr)
    {
        if (pBr->op == BspOp_Else)
        {
            pCase = pBr->right.Value.b;
            pVal = pMyObject->EvalExpression(hThisObject, &pCase->right, hRuntime, &bTerminated);
            if (BspTemplateVarEqual((ANSC_HANDLE)pExp, pVal))
            {
        		pMyObject->DoStatementList(hThisObject, pCase->left.Value.b, hRuntime, FALSE, &bTerminated);
                bDone = TRUE;
            }
            pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);

            if (bDone)
            {
                break;
            }
        }
        else if (pBr->op == BspOp_Default)
        {
        	pMyObject->DoStatementList(hThisObject, pBr->left.Value.b, hRuntime, FALSE, &bTerminated);
        }

        /* next case */
        pBr = pBr->left.Value.b;
    }

    pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pExp);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoElse
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute 'else' branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoElse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    UNREFERENCED_PARAMETER(hVar);
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    PBSP_TEMPLATE_BRANCH_OBJECT     aBr, pBr;
    LONG                            lValue;
    BOOL                            bTerminated = FALSE;

    aBr = iBr;

    while (TRUE)
    {
        pVal = pMyObject->EvalExpression(hThisObject, &aBr->right, hRuntime, &bTerminated);

        lValue = pVal->GetLong((ANSC_HANDLE)pVal);
        /* pVal->Remove((ANSC_HANDLE)pVal); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);

        if (lValue)
        {
            return;
        }

		if (!aBr->left.Value.b)
        {
			return;
        }

        pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)aBr->left.Value.b;

		if (pBr->op == BspOp_End) 
        {
			pMyObject->DoStatementList(hThisObject, aBr->left.Value.b, hRuntime, FALSE, &bTerminated);

			return;
		}

        aBr = aBr->left.Value.b;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute 'if' branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        aResult   = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PBSP_TEMPLATE_VAR_OBJECT        pVal;
    LONG                            lValue;
    BOOL                            bTerminated = FALSE;

    pVal = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);
    lValue = pVal->GetLong((ANSC_HANDLE)pVal);
    /* pVal->Remove((ANSC_HANDLE)pVal); */
    pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVal);

	if (lValue) 
    {
		pMyObject->DoStatementList(hThisObject, iBr->left.Value.b, hRuntime, FALSE, &bTerminated);
		aResult->Value.num = 1;
	}
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoTextblock
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to output free text block.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoTextblock
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar
    )
{
    UNREFERENCED_PARAMETER(hRuntime);
    UNREFERENCED_PARAMETER(hVar);
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;

    pMyObject->OutputBytes((ANSC_HANDLE)pMyObject, iBr->right.Value.s, iBr->left.Value.n);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateObjDoInclude
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to execute 'include' statement.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                Branch object.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateObjDoInclude
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        *pbTerminated
    )
{
    UNREFERENCED_PARAMETER(hVar);
    PBSP_TEMPLATE_OBJECT            pMyObject = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     iBr       = (PBSP_TEMPLATE_BRANCH_OBJECT)hBranch;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        pValL;
    char                            *pName;
    BOOL                            bTerminated = FALSE;

    pValL   = pMyObject->EvalExpression(hThisObject, &iBr->right, hRuntime, &bTerminated);

    if (pValL->Type == BspVar_String)
    {
        pName   = pValL->Value.str;

        if (pName)
        {
            pMyObject->IncludeTemplate((ANSC_HANDLE)pMyObject, hRuntime, (PUCHAR)pName, &bTerminated);

#ifdef   _DEBUG
            if ( BspEngIsCallFlowTraced() )
            {
                AnscTrace("BSP page %s loaded, time = %lu (ms).\n", pName, AnscGetTickInMilliSeconds());
            }
#endif
        }
        else
        {
            AnscTrace("Can't inlcude BSP without name!\r\n");
        }
    }

    /* pValL->Remove((ANSC_HANDLE)pValL); */
    pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pValL);

    if (pbTerminated)
    {
        *pbTerminated   = bTerminated;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateEngGetBeepObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar,
                BOOL                        bSlapVar,
                const char                  *pName,
                BOOL                        *pbSimpleVar
            )

    description:

        This function is called to execute the given branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRuntime
                Runtime object.

                ANSC_HANDLE                 hVar
                Variable object.

                BOOL                        bSlapVar
                Indicates to use hVar.

                const char                  *pName
                BEEP object name.

                BOOL                        *pbSimpleVar
                contains whether the BEEP object name is actually
                refering to a simple variable.

    return:     handle to BEEP object.

**********************************************************************/

ANSC_HANDLE
BspTemplateEngGetBeepObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        bSlapVar,
        const char                  *pName,
        BOOL                        *pbSimpleVar
    )
{
    /*
     *  This function is called to access underlying BEEP object via
     * BEEP PEC interface. The return value is the handle to a BEEP
     * object
     */

    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_LIST_OBJECT       pList       = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;
    ANSC_HANDLE                     hObj        = (ANSC_HANDLE)NULL;
    PBSPENG_SOA_INTERFACE           pBspSoaIf   = pList->GetBspSoaIf((ANSC_HANDLE)pList);
    BOOL                            bSimpleVar  = FALSE;

    if (!pBspSoaIf)
    {
        return (ANSC_HANDLE)NULL;
    }

    if (!hVar)
    {
        /* 
         * the scenario is like the following form.
         *
         *      ExecContext.xxx
         */
        hObj    = BspEngGetBeepObjectHandle(hRuntime, hThisObject, (ANSC_HANDLE)pBspSoaIf, (char *)pName);

        if (!hObj)
        {
            /* try to map the name to local variable */
            ULONG                   ulVal;

            ulVal   = 
                pMyObject->FindSymbol
                    (
                        hThisObject,
                        pName, 
                        FALSE
                    );

            if (ulVal != (ULONG)-1)
            {
                /* the can be mapped to a local variable */
                PBSP_TEMPLATE_VAR_OBJECT    pVar;

                pVar    = pMyObject->StackVar(hThisObject, ulVal, hRuntime, FALSE);

                if (pVar->Type == BspVar_Object && pVar->Value.obj)
                {
                    PSLAP_VARIABLE  pSlapVar    = (PSLAP_VARIABLE)pVar->Value.obj;

                    switch (pSlapVar->Syntax)
                    {
                        case    SLAP_VAR_SYNTAX_TYPE_handle:
                        case    SLAP_VAR_SYNTAX_TYPE_object:

                            hObj    = pSlapVar->Variant.varObject;

                            break;
                    }
                }
            }
        }
    }
    else
    {
        if (!bSlapVar)
        {
            /*
             *  This covers the scenario like this:
             *
             *      obj.xxx     
             *      obj(...)
             *
             *      and obj is a BSP script variable
             */
            PBSP_TEMPLATE_VAR_OBJECT    pVar        = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

            if (pVar->Type == BspVar_Object)
            {
                hObj    = pVar->Value.obj;
            }
            else
            {
                hObj        = NULL;
                bSimpleVar  = TRUE;
            }
        }
        else
        {
            /*
             *  This covers scenario like this:
             *
             *      obj.xxx
             *      obj(...)
             *
             *      and obj is a SLAP variable returned by previous
             *      slap call
             */

            PSLAP_VARIABLE          pVar    = (PSLAP_VARIABLE)hVar;

            switch (pVar->Syntax)
            {
                case    SLAP_VAR_SYNTAX_TYPE_handle:
                    
                    hObj    = pVar->Variant.varHandle;
                    
                    break;

                case    SLAP_VAR_SYNTAX_TYPE_object:
                    
                    hObj    = pVar->Variant.varObject;
                    
                    break;

                default:

                    hObj        = hVar;
                    bSimpleVar  = TRUE;

                    break;
            }
        }
    }

    if (pbSimpleVar)
    {
        *pbSimpleVar    = bSimpleVar;
    }

    return hObj;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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
            )

    description:

        This function is called to access object's property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRuntime
                Handle to runtime object.

                ANSC_HANDLE                 hObj
                Handle to Slap object to be accessed.

                BOOL                        bSimpleVar
                If the variable is simple format.

                const char                  *pName
                Name of property.

                ANSC_HANDLE                 hParamList
                Parameters specified.

                ANSC_HANDLE                 hValueSet
                Property's value to be set if any.

                BOOL                        *pbTerminated
                To tell engine to terminate BSP page generation if 
                it is TRUE on return.

    return:     Slap variable returned.

**********************************************************************/

/*
 *  This function is called to access underlying BEEP object via
 * BEEP PEC interface. The return value is the handle to a SLAP
 * variable.
 */
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
    )
{
    /*
     *  Notes:
     *
     *  hObj --- MUST NOT be NULL, it points to a BEEP object or
     *           a SLAP variable (for accessing simple data's property)
     *           depending on bSimpleVar   
     *
     *  bSimpleVar --- hObj points to a BEEP object if this value is TRUE
     *                 otherwise hObj points to a SLAP variable object
     *
     *  pName --- the method or property name. It might be NULL if
     *            xxx(...)(...) is encounted. In this case, hObj
     *            is retreived from variable xxx and the first parameter
     *            in the first group will be treated as property name.
     *            Generally speaking, there should be only one 
     *            parameter to specify the property name.
     *            But if bSimpleVar is TRUE, the name MUST NOT be NULL
     *            or an empty string.
     *
     *  hParamList --- Might be NULL. It points to a structure
     *                 _BSP_TEMPLATE_NAME_PARAM_LIST.
     *
     *  hValueSet  --- It's not NULL and points to a local script
     *                 variable if object's property is specified.
     *                 Otherwise, method call will be called or
     *                 property will be retreived. If it's not,
     *                 the right-most property's value will be
     *                 set to this value.
     *
     *  pbTerminated [OUT] --- to specify whether or not BSP engine
     *                         should continue execute the current
     *                         BSP page. TRUE means this execution
     *                         should be terminated.
     *
     *  Return value --- SLAP variable handle, caller is responsible
     *                   for releasing it by calling macro 
     *                   SlapFreeVariable.
     */

    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_LIST_OBJECT       pList       = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt         = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    PBSPENG_SOA_INTERFACE           pBspSoaIf   = pList->GetBspSoaIf((ANSC_HANDLE)pList);
    BOOL                            bTerminated = FALSE;
    ANSC_HANDLE                     hBeepObj    = hObj;
    PBSP_TEMPLATE_NAME_PARAM_LIST   pParamList  = (PBSP_TEMPLATE_NAME_PARAM_LIST)hParamList;
    SLAP_PARAMETER_LIST             SlapParamList;
    SLAP_PARAMETER_LIST             *outList    = NULL;
    PSLAP_VARIABLE                  pSlapReturnVal;
    SLAP_VARIABLE                   SlapReturnVal;
    char                            *pMethodName = NULL; /*RDKB-5934, CID-24729, Initialize before use*/
    PBSP_TEMPLATE_NAME_PARAM        pParam;
    ULONG                           i           = 0;
    PBSP_TEMPLATE_VAR_OBJECT        pVar;
    BOOL                            bMethodCall = FALSE;
    BOOL                            bSetProp    = TRUE;
    ANSC_STATUS                     status;
    PSLAP_VARIABLE                  pReturnVal  = NULL;
    ULONG                           ulBspSoaStatus;
    errno_t                         rc = -1;

    if (!pBspSoaIf || !hObj || (!pName && !hParamList))
    {
        AnscTrace("No BEEP/SLAP object is specified!\r\n");
        return (ANSC_HANDLE)NULL;
    }

    SlapInitVariable((&SlapReturnVal));
    SlapInitParamList((&SlapParamList));

    while (TRUE)
    {
        pSlapReturnVal  = NULL;

        if (bSimpleVar)
        {
            /* simple variable */

            AnscTrace("Simple type data's property access is not supported yet.\r\n");
        }
        else
        {
            /* normal BEEP object */
            if (!pName)
            {
                /* property access in form "obj(p1)(p2)...(pN) [=xxx]" */
                if (!pParamList->pGroups)
                {
                    AnscTrace("No property specified.\r\n");
                    break;
                }

                pParam  = pParamList->pGroups[i ++];

                if (pParam->ulParams != 1)
                {
                    AnscTrace("Only one parameter is expected!\r\n");
                    break;
                }
                else
                {
                    pVar    = pParam->pParams[0];

                    if (pVar->Type != BspVar_String)
                    {
                        AnscTrace("Property name is expected!\r\n");
                        break;
                    }
                    else
                    {
                        pMethodName = pVar->Value.str;
                    }
                }

                /* prepare parameter list */
                BspEng_VC_PrepareSlapParamList
                    (
                        (ANSC_HANDLE)(&SlapParamList), 
                        (ANSC_HANDLE)pParam->pParams, 
                        pParam->ulParams - pParam->ulOutParams
                    );
            }
            else
            {
                /* method or property */
                pMethodName = (char *)pName;

                /* check if it's a property */
                bMethodCall =  (pParamList && pParamList->ulGroups != 0);

                if (bMethodCall)
                {
                    pParam  = pParamList->pGroups[i ++];

                    /* prepare parameter list */
                    BspEng_VC_PrepareSlapParamList
                        (
                            (ANSC_HANDLE)(&SlapParamList), 
                            (ANSC_HANDLE)pParam->pParams, 
                            pParam->ulParams - pParam->ulOutParams
                        );
                }
                else
                {
                    /* obj.name */
                }
            }
        }

        if (!bMethodCall)
        {
            /* determine whether or not to "set" this property's value */
            /*CID: 56026 Dereference after null check*/
            if (  pParamList )
                  bSetProp    = (hValueSet && (i >= pParamList->ulGroups));

            BspEng_PreparePropertyParamList
                (
                    hRuntime,
                    &SlapParamList,
                    !bSetProp,
                    (PUCHAR)pMethodName,
                    hValueSet
                );

            if (bSetProp)
            {
                pMethodName = BSP_METHOD_SET_PROPERTY;
            }
            else
            {
                pMethodName = BSP_METHOD_GET_PROPERTY;
            }
        }

        if (hBeepObj)
        {
#ifdef   _DEBUG
            if ( BspEngIsCallFlowTraced() )
            {
                AnscTrace("    ---> Making slap call %s, time = %lu (ms) ...\n", pMethodName, AnscGetTickInMilliSeconds());

                if (strcasecmp(pMethodName, "SetItem") == 0 || strcasecmp(pMethodName, "GetItem") == 0 )
                {
                    if (SlapParamList.ParamCount != 0)
                    {
                        PSLAP_VARIABLE  pSlapName = (PSLAP_VARIABLE)SlapParamList.ParamArray;

                        if ( pSlapName->Syntax == SLAP_VAR_SYNTAX_string )
                        {
                            AnscTrace("              @@@@ item name is <%s> @@@@\n", pSlapName->Variant.varString);
                        }
                    }
                }
            }
#endif

            /* we should exclude output parameters being pushed in SlapParamList - input parameter list */

            status  = 
                pBspSoaIf->InvokeObject
                    (
                        pBspSoaIf->hOwnerContext,
                        hBeepObj,
                        pMethodName,
                        &SlapParamList,
                        &outList,
                        &pSlapReturnVal,
                        &ulBspSoaStatus
                    );

#ifdef   _DEBUG
        if ( BspEngIsCallFlowTraced() )
        {
            AnscTrace("        <--- slap call %s returned, time = %lu.\n", pMethodName, AnscGetTickInMilliSeconds());

            if (status != ANSC_STATUS_SUCCESS)
            {
                AnscTrace("Failed to make slap call '%s'.\n", pMethodName);
            }
            else
            {
                if ( !pSlapReturnVal )
                {
                    AnscTrace("        <--- Return value: NULL.\r\n");
                }
                else
                {
                    PSLAP_VARIABLE  pRetVal = pSlapReturnVal;
                    char            buf[32];

                    switch ( pRetVal->Syntax )
                    {
                        case    SLAP_VAR_SYNTAX_bool:
                        case    SLAP_VAR_SYNTAX_int:
                        case    SLAP_VAR_SYNTAX_uint32:

                                rc = sprintf_s(buf, sizeof(buf), "0x%.8x, signed=%s", (UINT)pRetVal->Variant.varUint32, pRetVal->Syntax==SLAP_VAR_SYNTAX_int?"yes":"no");
                                if(rc < EOK)
                                {
                                    ERR_CHK(rc);
                                }
                                AnscTrace("        <--- Return numeric value: %s.\n", buf);

                                break;

                        case    SLAP_VAR_SYNTAX_string:

                                if ( !pRetVal->Variant.varString )
                                {
                                    AnscTrace("        <--- Return string value: <>.\r\n");
                                }
                                else
                                if ( AnscSizeOfString(pRetVal->Variant.varString) < 960 )
                                {
                                    AnscTrace("        <--- Return string value: <%s>.\n", pRetVal->Variant.varString);
                                }
                                else
                                {
                                    /*ULONG   i = 0;

                                    
                                    AnscTrace("        <--- Return string value: <");
                                    for ( i = 0; i < AnscSizeOfString(pRetVal->Variant.varString); i ++)
                                    {
                                        AnscTrace("%c", pRetVal->Variant.varString[i]);
                                    }
                                    AnscTrace(">\r\n");
                                    */
                                    AnscTrace("        <--- Return string value: <TOO LONG TO DISPLAY>.\r\n");
                                }

                                break;

                        default:

                                AnscTrace("        <--- Return value has complex type.\r\n");

                                break;
                    }
                }
            }
        }
#endif
        }

        bTerminated = (ulBspSoaStatus == BSPENG_SOA_STATUS_terminate);

        if (outList)
        {
            /* update output parameters */
            ULONG                   ulOParamCount   = outList->ParamCount;

            if (ulOParamCount != 0)
            {
                ULONG                       i;
                ULONG                       SlapParamRefCount;
                PBSP_TEMPLATE_VAR_OBJECT    *pStackVars;
                PBSP_TEMPLATE_VAR_OBJECT    pRefVar;
            
                pStackVars = 
                    (PBSP_TEMPLATE_VAR_OBJECT *)pRt->GetSlapParamRef
                        (
                            (ANSC_HANDLE)pRt, 
                            &SlapParamRefCount
                        );

                for (i = 0; (i < SlapParamRefCount) && (i < ulOParamCount); i ++)
                {
                    pRefVar = pStackVars[i];

                    if (pRefVar)
                    {
                        BspEng_VC_Slap2Var((ANSC_HANDLE)&outList->ParamArray[i], (ANSC_HANDLE)pRefVar);
                    }
                }
            }

            SlapFreeParamList(outList);
        }

        /* if there's more paramter group(s) that implies property access(es) followed */
        if (pParamList && i < pParamList->ulGroups)
        {
            pName   = NULL;     /* enforce property access */
        }

        /* modify hBeepObj according to the return value */
        if ((bMethodCall || !bSetProp) && pSlapReturnVal)
        {
            switch (pSlapReturnVal->Syntax)
            {
                case    SLAP_VAR_SYNTAX_TYPE_handle:
                case    SLAP_VAR_SYNTAX_TYPE_object:

                    hBeepObj    = pSlapReturnVal->Variant.varHandle;

                    break;

                default:

                    hBeepObj    = (ANSC_HANDLE)NULL;

                    break;
            }
        }

        if (pSlapReturnVal)
        {
            SlapCloneVariable(pSlapReturnVal, (&SlapReturnVal));

            if (pSlapReturnVal)
            {
                SlapFreeVariable(pSlapReturnVal);
                pSlapReturnVal  = NULL;
            }
        }

        SlapCleanParamList((&SlapParamList));
        SlapInitParamList((&SlapParamList));

        if (!pParamList || i >= pParamList->ulGroups || !hBeepObj)
        {
            break;
        }
    
        if (ulBspSoaStatus != BSPENG_SOA_STATUS_continue)
        {
            /* previous BEEP object call failed */
            break;
        }
    }

    SlapCleanParamList((&SlapParamList));
    SlapInitParamList((&SlapParamList));

    if (pbTerminated)
    {
        *pbTerminated   = bTerminated;
    }

    SlapAllocVariable(pReturnVal);

    if (pReturnVal)
    {
        *pReturnVal = SlapReturnVal;
    }

    return (ANSC_HANDLE)pReturnVal;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateEngOutputString
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pString
            )

    description:

        This function is called to output given string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pString
                String to be output.

    return:     void.

**********************************************************************/

void
BspTemplateEngOutputString
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pString
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_LIST_OBJECT       pList       = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;
    PBSPENG_SOA_INTERFACE           pBspSoaIf   = pList->GetBspSoaIf((ANSC_HANDLE)pList);

    if ( !pString || AnscSizeOfString(pString) == 0 )
    {
        return;
    }

    if (!pBspSoaIf)
    {
        return;
    }

    pBspSoaIf->WriteAString
        (
            pBspSoaIf->hOwnerContext,
            (char *)pString
        );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateEngOutputBytes
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pStream,
                ULONG                       ulSize
            )

    description:

        This function is called to output a binary stream.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pStream
                Stream buffer.

                ULONG                       ulSize
                The size of stream buffer in bytes.

    return:     void.

**********************************************************************/

void
BspTemplateEngOutputBytes
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pStream,
        ULONG                       ulSize
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_LIST_OBJECT       pList       = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;
    PBSPENG_SOA_INTERFACE           pBspSoaIf   = pList->GetBspSoaIf((ANSC_HANDLE)pList);

    if (!pBspSoaIf || !pStream || ulSize == 0)
    {
        return;
    }

    pBspSoaIf->WriteBString
        (
            pBspSoaIf->hOwnerContext,
            (PUCHAR)pStream,
            ulSize
        );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateEngOutputNumber
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulVal,
                BOOL                        bSigned
            )

    description:

        This function is called to output a number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulVal
                The absolute value of the number

                BOOL                        bSigned
                If TRUE, the output number is unsigned value, 
                otherwise, a minus value will be output.

    return:     void.

**********************************************************************/

void
BspTemplateEngOutputNumber
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulVal,
        BOOL                        bSigned
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    char                            buf[32];
    errno_t                         rc              = -1;

    if (bSigned) {
        rc = sprintf_s(buf, sizeof(buf), "%d", (int)ulVal);
    }
    else {
        rc = sprintf_s(buf, sizeof(buf), "%u", (UINT)ulVal);
    }

    if(rc < EOK)
    {
       ERR_CHK(rc);
    }

    pMyObject->OutputString(hThisObject, (const char *)buf);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateEngOutputVar
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to output a variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                Variable object.

    return:     void.

**********************************************************************/

void
BspTemplateEngOutputVar
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar        = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    if (!pVar)
    {
        return;
    }

    switch (pVar->Type)
    {
        case    BspVar_Number:

            pMyObject->OutputNumber(hThisObject, pVar->Value.num, TRUE);

            break;

        case    BspVar_UNumber:

            pMyObject->OutputNumber(hThisObject, pVar->Value.num, FALSE);

            break;

        case    BspVar_String:
            
            pMyObject->OutputString(hThisObject, pVar->Value.str);

            break;

        case    BspVar_Object:

            {
                char                *pObjectString;

                pObjectString   = BspTemplateEngGetSlapObjectString(pVar->Value.obj);

                if (pObjectString)
                {
                    pMyObject->OutputString(hThisObject, pObjectString);

                    AnscFreeMemory(pObjectString);
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

        char *
        BspTemplateEngGetSlapObjectString
            (
                ANSC_HANDLE                 hSlapVar,
            )

    description:

        This function is called to output slap object's pointer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     string.

**********************************************************************/

char *
BspTemplateEngGetSlapObjectString
    (
        ANSC_HANDLE                 hSlapVar
    )
{
    PSLAP_VARIABLE                  pSlapVar    = (PSLAP_VARIABLE)hSlapVar;
    char                            *pString    = NULL;
    ULONG                           ulSize      = 0;
    PSLAP_VAR_ARRAY                 pArray      = NULL;
    ULONG                           i;
    errno_t                         rc          = -1;

    if (!pSlapVar)
    {
        pString = (char *)AnscAllocateMemory(2);
        
        if (pString)
        {
            rc = strcpy_s(pString, 2, "0");
            ERR_CHK(rc);
        }

        return pString;
    }

    switch (pSlapVar->Syntax)
    {
        case    SLAP_VAR_SYNTAX_TYPE_handle:
        case    SLAP_VAR_SYNTAX_TYPE_object:

            ulSize  = 12;

            pString = (char *)AnscAllocateMemory(ulSize);

            if (pString)
            {
#ifdef _64BIT_ARCH_SUPPORT_
                rc = sprintf_s(pString, ulSize, "%.08X", (UINT)(ULONG)pSlapVar->Variant.varHandle);
#else
		rc = sprintf_s(pString, ulSize, "%.08X", (UINT)pSlapVar->Variant.varHandle);
#endif		
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
            }

            break;

        case    SLAP_VAR_SYNTAX_TYPE_boolArray:

            /* 
             *  the string will be in form of "true.false.false.true..."
             */
            
            pArray  = pSlapVar->Variant.varBoolArray;

            if (pArray && pArray->VarCount)
            {
                ulSize  = 6 * pArray->VarCount;

                pString = (char *)AnscAllocateMemory(ulSize);

                if (pString)
                {
                    for (i = 0; i < pArray->VarCount; i ++)
                    {
                        rc = sprintf_s
                            (
                                pString + AnscSizeOfString(pString),
                                ulSize + 1,
                                (i == 0)? "%s" : ".%s",
                                pArray->Array.arrayBool[i] ? "true" : "false"
                            );
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
                    }
                }
            }

            break;

        case    SLAP_VAR_SYNTAX_TYPE_charArray:

            pArray  = pSlapVar->Variant.varCharArray;

            if (pArray && pArray->VarCount)
            {
                ulSize  = pArray->VarCount + 1;

                pString = (char *)AnscAllocateMemory(ulSize);

                if (pString)
                {
                    AnscCopyMemory(pString, pArray->Array.arrayChar, pArray->VarCount);
                    pString[ulSize - 1] = 0;
                }
            }

            break;

        case    SLAP_VAR_SYNTAX_TYPE_ucharArray:

            pArray  = pSlapVar->Variant.varUcharArray;

            if (pArray && pArray->VarCount)
            {
                ulSize  = pArray->VarCount + 1;

                pString = (char *)AnscAllocateMemory(ulSize);

                if (pString)
                {
                    AnscCopyMemory(pString, pArray->Array.arrayUchar, pArray->VarCount);
                    pString[ulSize - 1] = 0;
                }
            }

            break;

        case    SLAP_VAR_SYNTAX_TYPE_intArray:

            /* 
             *  the string will be in form of "xx.yy.zz"
             */
            
            pArray  = pSlapVar->Variant.varIntArray;

            if (pArray && pArray->VarCount)
            {
                ulSize  = 10 * pArray->VarCount;

                pString = (char *)AnscAllocateMemory(ulSize);

                if (pString)
                {
                    for (i = 0; i < pArray->VarCount; i ++)
                    {
                        rc = sprintf_s
                            (
                                pString + AnscSizeOfString(pString),
                                ulSize+1,
                                (i == 0)? "%d" : ".%d",
                                pArray->Array.arrayInt[i]
                            );
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
                    }
                }
            }

            break;

        case    SLAP_VAR_SYNTAX_TYPE_stringArray:

            /* 
             *  the string will be in form of "xx~yy~zz"
             */
            
            pArray  = pSlapVar->Variant.varStringArray;

            if (pArray && pArray->VarCount)
            {
                ulSize  = 0;

                for (i = 0; i < pArray->VarCount; i ++)
                {
                    ulSize  += AnscSizeOfString(pArray->Array.arrayString[i]) + 1;
                }
                
                if (ulSize != 0)
                {
                    pString = (char *)AnscAllocateMemory(ulSize);

                    if (pString)
                    {
                        for (i = 0; i < pArray->VarCount; i ++)
                        {
                            rc = sprintf_s
                                (
                                    pString + AnscSizeOfString(pString),
                                    ulSize+1,
                                    (i == 0)? "%s" : "~%s",
                                    pArray->Array.arrayString[i]
                                );
                            if(rc < EOK)
                            {
                                ERR_CHK(rc);
                            }
                        }
                    }
                }
            }

            break;

        case    SLAP_VAR_SYNTAX_TYPE_uint32Array:

            /* 
             *  the string will be in form of "xx.yy.zz"
             */
            
            pArray  = pSlapVar->Variant.varUint32Array;

            if (pArray && pArray->VarCount)
            {
                ulSize  = 10 * pArray->VarCount;

                pString = (char *)AnscAllocateMemory(ulSize);

                if (pString)
                {
                    for (i = 0; i < pArray->VarCount; i ++)
                    {
                        rc = sprintf_s
                            (
                                pString + AnscSizeOfString(pString),
                                ulSize+1,
                                (i == 0)? "%u" : ".%u",
                                (UINT)pArray->Array.arrayUint32[i]
                            );
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
                    }
                }
            }

            break;

        case    SLAP_VAR_SYNTAX_TYPE_handleArray:
        case    SLAP_VAR_SYNTAX_TYPE_objectArray:

            /* 
             *  the string will be in form of "xx.yy.zz"
             */
            
            pArray  = pSlapVar->Variant.varHandleArray;

            if (pArray && pArray->VarCount)
            {
                ulSize  = 11 * pArray->VarCount;

                pString = (char *)AnscAllocateMemory(ulSize);

                if (pString)
                {
                    for (i = 0; i < pArray->VarCount; i ++)
                    {
                        rc = sprintf_s
                            (
                                pString + AnscSizeOfString(pString),
                                ulSize+1,
                                (i == 0)? "%.08X" : ".%.08X",
#ifdef _64BIT_ARCH_SUPPORT_
                                (UINT)(ULONG)pArray->Array.arrayHandle[i]
#else
                                (UINT)pArray->Array.arrayHandle[i]
#endif
                            );
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
                    }
                }
            }

            break;

        default:

            break;
    }

    return pString;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspEngGetBeepObjectHandle
            (
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hTmplObj,
                ANSC_HANDLE                 hBspSoaIf,
                char                        *pObjName
            )

    description:

        This function is called to get SLAP object handle.

    argument:   ANSC_HANDLE                 hRuntime
                Handle to runtime object.

                ANSC_HANDLE                 hTmpObj
                Handle to BSP template object.
        
                ANSC_HANDLE                 hBspSoaIf
                The handle to BEEP PEC interface.

                char                        *pObjName
                SLAP object name.

    return:     handle to SLAP object.

**********************************************************************/

static ANSC_HANDLE
BspEngGetBeepObjectHandle
    (
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hTmplObj,
        ANSC_HANDLE                 hBspSoaIf,
        char                        *pObjName
    )
{
    /* 
     * The name is in form of xxx [ . xxx ], for example, "Services.Mib2.Database".
     * There're two possibilities that we need to deal with.
     *
     *  1. BEEP PEC interface can resolve this name and return a BEEP object handle.
     *  2. BEEP PEC can't resolve the name, For example, the name is "X.Y.Z".
     *     What we should do next is as follows.
     *      2.1 Check if "X.Y" is a BEEP object, if so, try to get property Z
     *      2.2 Otherwise, one name backward, namely checking if X is a BEEP object,
     *          if so, try to get properties one by one.
     *      2.3 If the left-most name is reached and it's not a BEEP object, we
     *          can say there's no matching BEEP object found.
     */

    PBSP_TEMPLATE_OBJECT            pTemplateObj    = (PBSP_TEMPLATE_OBJECT)hTmplObj;
    PBSPENG_SOA_INTERFACE           pBspSoaIf       = (PBSPENG_SOA_INTERFACE)hBspSoaIf;
    ANSC_HANDLE                     hBeepObj        = (ANSC_HANDLE)NULL;
    ULONG                           ulVar           = (ULONG)-1;
    PUCHAR                          pPos;

    /* check if the "object name" is a BSP variable */
    pPos    = (PUCHAR)_ansc_strchr(pObjName, '.');
    if (!pPos)
    {
        ulVar   = pTemplateObj->FindSymbol((ANSC_HANDLE)pTemplateObj, pObjName, FALSE);
        if (ulVar != (ULONG)-1)
        {
            /* this should be a BSP variable */
            return (ANSC_HANDLE)NULL;
        }
    }

    hBeepObj    = pBspSoaIf->GetSlapObject(pBspSoaIf->hOwnerContext, pObjName);

    if (NULL == hBeepObj)
    {
        char                        *pName          = (char *)AnscDupString((PUCHAR)pObjName);
        char                        *pNameOrg       = pName;
        char                        *pNext          = NULL;
        SLAP_PARAMETER_LIST         SlapParamList;
        SLAP_PARAMETER_LIST         *outList;
        ANSC_STATUS                 status;
        PSLAP_VARIABLE              pSlapReturnVal;
        ULONG                       ulBspSoaStatus;
        char                        *pNames[BSPOBJ_NAME_MAX_LEVEL];
        int                         i, j, nNumNames;
        char                        *pCurName       = (char *)AnscDupString((PUCHAR)pObjName);
        char                        *pCurNameEnd    = NULL;

        SlapInitParamList((&SlapParamList));

        nNumNames   = 0;

        while (pName && *pName != '\0')
        {
            pNext   = _ansc_strchr(pName, '.');

            if (pNext)
            {
                *pNext  = '\0';
                ++ pNext;
            }
            else
            {
                pNext   = NULL;
            }

            pNames[nNumNames ++]    = pName;

            pName   = pNext;
        }

        i = nNumNames - 2;

        while (i >= 0)
        {
            pCurNameEnd = pCurName + AnscSizeOfString(pCurName) - 1;
            while (pCurNameEnd > pCurName && *pCurNameEnd != '.')
            {
                pCurNameEnd --;
            }

            *pCurNameEnd    = '\0';

            hBeepObj    = pBspSoaIf->GetSlapObject(pBspSoaIf->hOwnerContext, pCurName);

            if (!hBeepObj)
            {
                /* move backwards */
                i --;
                continue;
            }

            j = i + 1;

            while (j < nNumNames)
            {
                /* the current pName should be a property of current BEEP object */

                BspEng_PreparePropertyParamList
                    (
                        hRuntime,
                        &SlapParamList,
                        TRUE,
                        (PUCHAR)pNames[j],
                        NULL
                    );

                pSlapReturnVal  = NULL;
                outList         = NULL;

                status  = 
                    pBspSoaIf->InvokeObject
                        (
                            pBspSoaIf->hOwnerContext,
                            hBeepObj,
                            BSP_METHOD_GET_PROPERTY,
                            &SlapParamList,
                            &outList,
                            &pSlapReturnVal,
                            &ulBspSoaStatus
                        );


#ifdef   _DEBUG
            if ( BspEngIsCallFlowTraced() )
            {
                AnscTrace("Slap property '%s' is accessed, time = %lu.\n", pNames[j], AnscGetTickInMilliSeconds());

                if (status != ANSC_STATUS_SUCCESS)
                {
                    AnscTrace("Failed to get property '%s'.\n", pNames[j]);
                }
            }
#endif

                SlapCleanParamList((&SlapParamList));
                SlapInitParamList((&SlapParamList));

                if (outList)
                {
                    SlapFreeParamList(outList);
                }

                if (ulBspSoaStatus != BSPENG_SOA_STATUS_continue)
                {
                    break;
                }

                if (!pSlapReturnVal)
                {
                    hBeepObj    = (ANSC_HANDLE)NULL;
                }
                else
                {
                    switch (pSlapReturnVal->Syntax)
                    {
                        case    SLAP_VAR_SYNTAX_TYPE_handle:
                        case    SLAP_VAR_SYNTAX_TYPE_object:

                            hBeepObj    = pSlapReturnVal->Variant.varHandle;

                            break;

                        default:

                            hBeepObj    = (ANSC_HANDLE)NULL;

                            break;
                    }

                    if (pSlapReturnVal)
                    {
                        SlapFreeVariable(pSlapReturnVal);
                        pSlapReturnVal  = NULL;
                    }
                }

                if (NULL == hBeepObj)
                {
                    /* couldn't get property */
                    break;
                }

                j ++;
            }
        
            if (j >= nNumNames && hBeepObj)
            {
                /* found the object */
                break;
            }
        
            i --;
        }

        AnscFreeMemory(pCurName);
        AnscFreeMemory(pNameOrg);
    }

    return hBeepObj;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateEngAccessVarContentType
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hVar,
                BOOL                        bSetContentType,
                ANSC_HANDLE                 hResultVar
            )

    description:

        This function is called to get/set content type of a 
        BSP variable.

    argument:   ANSC_HANDLE                 hTmpObj
                Handle to BSP template object.
        
                ANSC_HANDLE                 hBspSoaIf
                The handle to BEEP PEC interface.

                char                        *pObjName
                SLAP object name.

    return:     void.

**********************************************************************/

void
BspTemplateEngAccessVarContentType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hVar,
        BOOL                        bSetContentType,
        ANSC_HANDLE                 hResultVar
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject   = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PBSP_TEMPLATE_LIST_OBJECT       pList       = (PBSP_TEMPLATE_LIST_OBJECT)pMyObject->hOwnerList;
    PBSPENG_SOA_INTERFACE           pBspSoaIf   = pList->GetBspSoaIf((ANSC_HANDLE)pList);
    PBSP_TEMPLATE_VAR_OBJECT        pVar        = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PBSP_TEMPLATE_VAR_OBJECT        pResult     = (PBSP_TEMPLATE_VAR_OBJECT)hResultVar;
    SLAP_PARAMETER_LIST             SlapParamList;
    SLAP_PARAMETER_LIST             *outList    = NULL;
    PSLAP_VARIABLE                  pParam;
    PSLAP_VARIABLE                  pRetVal     = NULL;
    ULONG                           ulBspSoaStatus;
    BOOL                            bSimpleVar  = TRUE;
    ANSC_HANDLE                     hBeepObj    = (ANSC_HANDLE)NULL;

    if (!pVar)
    {
        return;
    }

    SlapInitParamList((&SlapParamList));

    hBeepObj    = 
        pMyObject->GetBeepObject
            (
                hThisObject,
                hRuntime,
                NULL,
                FALSE,
                SLAP_UTILITY_VAR_ENTITY,
                &bSimpleVar
            );

    if (bSetContentType)
    {
        BOOL                        bTypeName   = FALSE;
        BOOL                        bTypeId     = FALSE;

        if (pResult) /*RDKB-5934, CID-24385; null check before use*/
        {
            bTypeName   = (pResult->Type == BspVar_String && pResult->Value.str != NULL);
            bTypeId     = (pResult->Type == BspVar_Number || pResult->Type == BspVar_UNumber);

            if (bTypeName || bTypeId)
            {
                pVar->SetContentType((ANSC_HANDLE)pVar, pResult->Value.str);

                if (pVar->Type == BspVar_Object && hBeepObj)
                {
                    /* also set content type of SLAP variable */
                    SlapParamList.ParamCount    = 3;

                    pParam  = &SlapParamList.ParamArray[0];
                    pParam->Syntax  = SLAP_VAR_SYNTAX_TYPE_string;
                    pParam->Variant.varString   = AnscCloneString(BSP_VAR_CONTENT_TYPE);

                    pParam  = &SlapParamList.ParamArray[1];
                    SlapCloneVariable(pParam, ((PSLAP_VARIABLE)pVar->Value.obj));

                    pParam  = &SlapParamList.ParamArray[2];

                    if (bTypeName)
                    {
                        pParam->Syntax              = SLAP_VAR_SYNTAX_TYPE_string;
                        pParam->Variant.varString   = AnscCloneString(pResult->Value.str);
                    }
                    else
                    {
                        pParam->Syntax              = SLAP_VAR_SYNTAX_TYPE_uint32;
                        pParam->Variant.varUint32   = (ULONG)pVar->Value.num;
                    }

                        pBspSoaIf->InvokeObject
                            (
                                pBspSoaIf->hOwnerContext,
                                hBeepObj,
                                BSP_METHOD_GET_PROPERTY,
                                &SlapParamList,
                                &outList,
                                &pRetVal,
                                &ulBspSoaStatus
                            );
                    SlapCleanParamList((&SlapParamList));
                    SlapInitParamList((&SlapParamList));

                    if (outList)
                    {
                        SlapFreeParamList(outList);
                    }

                    if (pRetVal)
                    {
                        SlapFreeVariable(pRetVal);
                        pRetVal = NULL;
                    }
                }
            }
        }
    }
    else    /* get content type */
    {
        char                        *pType  = SLAP_CONTENT_NAME_UNSPECIFIED;
        ULONG                       ulType;

        if (pVar->Type == BspVar_Object && hBeepObj)
        {
            /* get SLAP variable's content type */
            SlapParamList.ParamCount    = 2;

            pParam  = &SlapParamList.ParamArray[0];
            pParam->Syntax  = SLAP_VAR_SYNTAX_TYPE_string;
            pParam->Variant.varString   = AnscCloneString(BSP_VAR_CONTENT_TYPE);

            pParam  = &SlapParamList.ParamArray[1];
            SlapCloneVariable(pParam, ((PSLAP_VARIABLE)pVar->Value.obj));

                pBspSoaIf->InvokeObject
                    (
                        pBspSoaIf->hOwnerContext,
                        hBeepObj,
                        BSP_METHOD_GET_PROPERTY,
                        &SlapParamList,
                        &outList,
                        &pRetVal,
                        &ulBspSoaStatus
                    );
            SlapCleanParamList((&SlapParamList));
            SlapInitParamList((&SlapParamList));

            if (outList)
            {
                SlapFreeParamList(outList);
            }

            if (pRetVal)
            {
                switch (pRetVal->Syntax)
                {
                    case    SLAP_VAR_SYNTAX_TYPE_string:

                        pType   = pRetVal->Variant.varString;

                        pVar->SetContentType((ANSC_HANDLE)pVar, pType);

                        break;

                    case    SLAP_VAR_SYNTAX_TYPE_uint32:

                        ulType  = pRetVal->Variant.varUint32;

                        pVar->SetContentType2((ANSC_HANDLE)pVar, ulType);

                        break;

                    default:

                        /* reset to unspecified */
                        pVar->SetContentType2((ANSC_HANDLE)pVar, 0);

                        break;
                }
            }
            else
            {
                /* reset to unspecified */
                pVar->SetContentType2((ANSC_HANDLE)pVar, 0);
            }

            if (pRetVal)
            {
                SlapFreeVariable(pRetVal);
                pRetVal = NULL;
            }
        }

        /* not associated with a SLAP object yet */
        pType   = pVar->GetContentType((ANSC_HANDLE)pVar);

        pResult->Reset((ANSC_HANDLE)pResult);

        pResult->Type       = BspVar_String;
        pResult->Size       = 0;
        pResult->bTemp      = TRUE;
        pResult->Value.str  = pType;
    }
}


