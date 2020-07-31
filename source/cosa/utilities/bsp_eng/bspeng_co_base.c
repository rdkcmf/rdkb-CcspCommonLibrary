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

    module:	bspeng_co_base.c

        For BSP Engine Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic component object functions
        of the string Component Object.

        *   BspEngCOCreate
        *   BspEngCORemove

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/14/03        initial revision.

**********************************************************************/


#include "bspeng_co_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        BspEngCOCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the String Component object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created string component object.

**********************************************************************/

ANSC_HANDLE
BspEngCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    UNREFERENCED_PARAMETER(hOwnerContext);
    UNREFERENCED_PARAMETER(hAnscReserved);
    PBSP_TEMPLATE_OBJECT            pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBSP_TEMPLATE_OBJECT)AnscAllocateMemory(sizeof(BSP_TEMPLATE_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->SetList                      = BspTemplateObjSetList;
    pMyObject->Parse                        = BspTemplateObjParse;
    pMyObject->Run                          = BspTemplateObjRun;
    pMyObject->GetTmplName                  = BspTemplateObjGetTmplName;
    pMyObject->Stop                         = BspTemplateObjStop;
    pMyObject->Tokenize                     = BspTemplateObjTokenize;
    pMyObject->ParseText                    = BspTemplateObjParseText;
    pMyObject->ParseComment                 = BspTemplateObjParseComment;
    pMyObject->ParseIdentOrKeyword          = BspTemplateObjParseIdentOrKeyword;
    pMyObject->ParseString                  = BspTemplateObjParseString;
    pMyObject->ParseNumber                  = BspTemplateObjParseNumber;
    pMyObject->ParseOp                      = BspTemplateObjParseOp;
    pMyObject->IsNameChar                   = BspTemplateObjIsNameChar;
    pMyObject->GetBinary                    = BspTemplateObjGetBinary;
    pMyObject->GetOctal                     = BspTemplateObjGetOctal;
    pMyObject->GetHex                       = BspTemplateObjGetHex;
#ifdef   _DEBUG
    pMyObject->PrintData                    = BspTemplateObjPrintData;
    pMyObject->PrintTokens                  = BspTemplateObjPrintTokens;
    pMyObject->PrintBranch                  = BspTemplateObjPrintBranch;
#endif
    pMyObject->Compile                      = BspTemplateObjCompile;
    pMyObject->CompileProcedures            = BspTemplateObjCompileProcedures;
    pMyObject->TakeoutProcedure             = BspTemplateObjTakeoutProcedure;
    pMyObject->BuildTree                    = BspTemplateObjBuildTree;
    pMyObject->StatementList                = BspTemplateObjStatementList;
    pMyObject->Statement                    = BspTemplateObjStatement;
    pMyObject->IncludeStatement             = BspTemplateObjIncludeStatement;
    pMyObject->IfStatement                  = BspTemplateObjIfStatement;
    pMyObject->IfElse                       = BspTemplateObjIfElse;
    pMyObject->WhileStatement               = BspTemplateObjWhileStatement;
    pMyObject->SwitchStatement              = BspTemplateObjSwitchStatement;
    pMyObject->ExpressionList               = BspTemplateObjExpressionList;
    pMyObject->ReturnStatement              = BspTemplateObjReturnStatement;
    pMyObject->SetOutputStatement           = BspTemplateObjSetouputStatement;
    pMyObject->Expression                   = BspTemplateObjExpression;
    pMyObject->ArrayDeclaration             = BspTemplateObjArrayDeclaration;
    pMyObject->ApiCall                      = BspTemplateObjApiCall;
    pMyObject->IsLValue                     = BspTemplateObjIsLValue;
    pMyObject->AssignmentExp                = BspTemplateObjAssignmentExp;
    pMyObject->LogicalExp                   = BspTemplateObjLogicalExp;
    pMyObject->ComparisonExp                = BspTemplateObjComparisonExp;
    pMyObject->AdditiveExp                  = BspTemplateObjAdditiveExp;
    pMyObject->MultiplicativeExp            = BspTemplateObjMultiplicativeExp;
    pMyObject->StrCatExp                    = BspTemplateObjStrCatExp;
    pMyObject->BinaryExp                    = BspTemplateObjBinaryExp;
    pMyObject->UnaryExp                     = BspTemplateObjUnaryExp;
    pMyObject->PostfixExp                   = BspTemplateObjPostfixExp;
    pMyObject->ValueExp                     = BspTemplateObjValueExp;
    pMyObject->ArrayExp                     = BspTemplateObjArrayExp;
    pMyObject->ArrayExpCondition            = BspTemplateObjArrayExpCondition;
    pMyObject->IsExpProcCall                = BspTemplateObjIsExpProcCall;
    pMyObject->ApiExp                       = BspTemplateObjApiExp;
    pMyObject->ObjectExp                    = BspTemplateObjObjectExp;
    pMyObject->VariableExp                  = BspTemplateObjVariableExp;
    pMyObject->ProcCallExp                  = BspTemplateObjProcCallExp;
    pMyObject->NameExp                      = BspTemplateObjNameExp;
    pMyObject->IsText                       = BspTemplateObjIsText;
    pMyObject->IsOp                         = BspTemplateObjIsOp;
    pMyObject->InOpList                     = BspTemplateObjInOpList;
    pMyObject->IsIdent                      = BspTemplateObjIsIdent;
    pMyObject->IsLiteral                    = BspTemplateObjIsLiteral;
    pMyObject->IsEnd                        = BspTemplateObjIsEnd;
    pMyObject->ParseError                   = BspTemplateObjParseError;
    pMyObject->NoMatch                      = BspTemplateObjNoMatch;
    pMyObject->NewSymbol                    = BspTemplateObjNewSymbol;
    pMyObject->FindSymbol                   = BspTemplateObjFindSymbol;
    pMyObject->RemoveSymbols                = BspTemplateObjRemoveSymbols;
    pMyObject->DoTemplate                   = BspTemplateObjDoTemplate;
    pMyObject->DoStatementList              = BspTemplateObjDoStatementList;
    pMyObject->DoBranch                     = BspTemplateObjDoBranch;
    pMyObject->GetApiCode                   = BspTemplateObjGetApiCode;
    pMyObject->IsOutputApi                  = BspTemplateObjIsOutputApi;
    pMyObject->EvalExpression               = BspTemplateObjEvalExp;
    pMyObject->EvalName                     = BspTemplateObjEvalName;
    pMyObject->EvalName2                    = BspTemplateObjEvalName2;
    pMyObject->EvalRef                      = BspTemplateObjEvalRef;
    pMyObject->StackVar                     = BspTemplateObjStackVar;
    pMyObject->GetErrorLineNo               = BspTemplateObjGetErrorLineNo;
    pMyObject->GetErrMsg                    = BspTemplateObjGetErrorMsg;
    pMyObject->GetLineNo                    = BspTemplateObjGetLineNo;
    pMyObject->GetToken                     = BspTemplateObjGetToken;
    pMyObject->Copy                         = BspTemplateObjCopy;
    pMyObject->UpdateSymbols                = BspTemplateObjUpdateSymbols;
    pMyObject->EvalArrayItem                = BspTemplateObjEvalArrayItem;
    pMyObject->Store                        = BspTemplateObjStore;
    pMyObject->Load                         = BspTemplateObjLoad;
    pMyObject->SetCmif                      = BspTemplateObjSetCmif;
    pMyObject->IncludeTemplate              = BspTemplateObjIncludeTemplate;

    pMyObject->GetBeepObject                = BspTemplateEngGetBeepObject;
    pMyObject->DoObjectAccess               = BspTemplateEngDoObjectAccess;

    pMyObject->OutputString                 = BspTemplateEngOutputString;
    pMyObject->OutputBytes                  = BspTemplateEngOutputBytes;
    pMyObject->OutputNumber                 = BspTemplateEngOutputNumber;
    pMyObject->OutputVar                    = BspTemplateEngOutputVar;
    pMyObject->AccessVarContentType         = BspTemplateEngAccessVarContentType;

    pMyObject->Remove                       = BspEngCORemove;

    pMyObject->hText        = AnscCreateByteArrayComponent(NULL, NULL, NULL);
    pMyObject->hSymbol      = AnscCreatePtrArrayComponent(NULL, NULL, NULL);

    pMyObject->ulStackBase  = 0;

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspEngCORemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspEngCORemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_OBJECT            pMyObject    = (PBSP_TEMPLATE_OBJECT)hThisObject;
    PANSC_BYTE_ARRAY_OBJECT         pText        = (PANSC_BYTE_ARRAY_OBJECT)pMyObject->hText;
    PANSC_PTR_ARRAY_OBJECT          pSymbol      = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hSymbol;
    PBSP_TEMPLATE_BRANCH_OBJECT     pTree        = (PBSP_TEMPLATE_BRANCH_OBJECT)pMyObject->hTree;

    if (pMyObject->pName)
        AnscFreeMemory((char *)pMyObject->pName);

    if (pTree)
        BspTemplateBranchCORemove((ANSC_HANDLE)pTree);

    pMyObject->RemoveSymbols(hThisObject);

    pText->Remove((ANSC_HANDLE)pText);
    pSymbol->Remove((ANSC_HANDLE)pSymbol);

    AnscFreeMemory(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
