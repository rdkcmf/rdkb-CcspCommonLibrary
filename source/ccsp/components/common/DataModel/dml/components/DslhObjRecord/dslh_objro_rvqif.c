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

    module:	dslh_objro_rvqif.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Dslh Obj Record Object.

        *   DslhObjroIsParamUpdated
        *   DslhObjroClearParamUpdated
        *   DslhObjroRvqGetParamValue
        *   DslhObjroRvqGetParamValueString
        *   DslhObjroRvqGetParamValueInt
        *   DslhObjroRvqGetParamValueUint32
        *   DslhObjroRvqGetParamValueBool
        *   DslhObjroRvqGetParamValueUint32Array
        *   DslhObjroRvqGetParamValueUcharArray
        *   DslhObjroRvqGetParamValueHandle

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/11/05    initial revision.

**********************************************************************/


#include "dslh_objro_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhObjroRvqIsParamUpdated
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called by the ObjController to determine
        whether the specified parameter was updated in the previous
        SetParameterValue() call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the name of the parameter to be examined.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
DslhObjroRvqIsParamUpdated
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord = (PDSLH_VAR_RECORD_OBJECT)NULL;

    pChildVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pMyObject->GetVarRecord
            (
                (ANSC_HANDLE)pMyObject,
                pParamName
            );

    if ( !pChildVarRecord )
    {
        return  FALSE;
    }
    else if ( !pChildVarRecord->TempParamValue )
    {
        return  FALSE;
    }
    else if ( pChildVarRecord->bAlreadySet )
    {
        return FALSE;
    }

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjroRvqClearParamUpdated
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called by the ObjController to clear the
        Temp value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the name of the parameter to be examined.

    return:     TRUE or FALSE.

**********************************************************************/

ANSC_STATUS
DslhObjroRvqClearParamUpdated
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord = (PDSLH_VAR_RECORD_OBJECT)NULL;

    pChildVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pMyObject->GetVarRecord
            (
                (ANSC_HANDLE)pMyObject,
                pParamName
            );

    if ( !pChildVarRecord )
    {
        return  ANSC_STATUS_FAILURE;
    }

    pChildVarRecord->bAlreadySet = TRUE;

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        DslhObjroRvqGetParamValue
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called by the ObjController to retrieve the
        modified value of the specified parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the name of the parameter to be matched.

    return:     parameter value.

**********************************************************************/

SLAP_VARIABLE*
DslhObjroRvqGetParamValue
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PDSLH_VAR_RECORD_OBJECT         pChildVarRecord = (PDSLH_VAR_RECORD_OBJECT)NULL;

    pChildVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pMyObject->GetVarRecord
            (
                (ANSC_HANDLE)pMyObject,
                pParamName
            );

    if ( !pChildVarRecord )
    {
        return  NULL;
    }

    return  pChildVarRecord->TempParamValueTrans? pChildVarRecord->TempParamValueTrans : pChildVarRecord->TempParamValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        DslhObjroRvqGetParamValueString
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called by the ObjController to retrieve the
        modified value of the specified parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the name of the parameter to be matched.

    return:     parameter value.

**********************************************************************/

char*
DslhObjroRvqGetParamValueString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PSLAP_VARIABLE                  pParamValue     = (PSLAP_VARIABLE         )NULL;
	PDSLH_RVQ_INTERFACE             pDslhRvqIf      = (PDSLH_RVQ_INTERFACE    )pMyObject->hDslhRvqIf;

    pParamValue =
        (PSLAP_VARIABLE)pDslhRvqIf->GetParamValue
            (
                (ANSC_HANDLE)pMyObject,
                pParamName
            );

    if ( !pParamValue )
    {
        return  NULL;
    }

    return  pParamValue->Variant.varString;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        int
        DslhObjroRvqGetParamValueInt
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called by the ObjController to retrieve the
        modified value of the specified parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the name of the parameter to be matched.

    return:     parameter value.

**********************************************************************/

int
DslhObjroRvqGetParamValueInt
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PSLAP_VARIABLE                  pParamValue     = (PSLAP_VARIABLE         )NULL;
	PDSLH_RVQ_INTERFACE             pDslhRvqIf      = (PDSLH_RVQ_INTERFACE    )pMyObject->hDslhRvqIf;

    pParamValue =
        (PSLAP_VARIABLE)pDslhRvqIf->GetParamValue
            (
                (ANSC_HANDLE)pMyObject,
                pParamName
            );

    if ( !pParamValue )
    {
        return  0;
    }

    return  pParamValue->Variant.varInt;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DslhObjroRvqGetParamValueUint32
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called by the ObjController to retrieve the
        modified value of the specified parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the name of the parameter to be matched.

    return:     parameter value.

**********************************************************************/

ULONG
DslhObjroRvqGetParamValueUint32
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PSLAP_VARIABLE                  pParamValue     = (PSLAP_VARIABLE         )NULL;
	PDSLH_RVQ_INTERFACE             pDslhRvqIf      = (PDSLH_RVQ_INTERFACE    )pMyObject->hDslhRvqIf;

    pParamValue =
        (PSLAP_VARIABLE)pDslhRvqIf->GetParamValue
            (
                (ANSC_HANDLE)pMyObject,
                pParamName
            );

    if ( !pParamValue )
    {
        return  0;
    }

    return  pParamValue->Variant.varUint32;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhObjroRvqGetParamValueBool
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called by the ObjController to retrieve the
        modified value of the specified parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the name of the parameter to be matched.

    return:     parameter value.

**********************************************************************/

BOOL
DslhObjroRvqGetParamValueBool
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject       = (PDSLH_OBJ_RECORD_OBJECT)hThisObject;
    PSLAP_VARIABLE                  pParamValue     = (PSLAP_VARIABLE         )NULL;
	PDSLH_RVQ_INTERFACE             pDslhRvqIf      = (PDSLH_RVQ_INTERFACE    )pMyObject->hDslhRvqIf;

    pParamValue =
        (PSLAP_VARIABLE)pDslhRvqIf->GetParamValue
            (
                (ANSC_HANDLE)pMyObject,
                pParamName
            );

    if ( !pParamValue )
    {
        return  FALSE;
    }

    return  (pParamValue->Variant.varBool != 0)? TRUE : FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UINT32_ARRAY*
        DslhObjroRvqGetParamValueUint32Array
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called by the ObjController to retrieve the
        modified value of the specified parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the name of the parameter to be matched.

    return:     parameter value.

**********************************************************************/

SLAP_UINT32_ARRAY*
DslhObjroRvqGetParamValueUint32Array
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject          = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PSLAP_VARIABLE                  pParamValue        = (PSLAP_VARIABLE             )NULL;
	PDSLH_RVQ_INTERFACE             pDslhRvqIf         = (PDSLH_RVQ_INTERFACE    )pMyObject->hDslhRvqIf;

    pParamValue =
        (PSLAP_VARIABLE)pDslhRvqIf->GetParamValue
            (
                (ANSC_HANDLE)pMyObject,
                pParamName
            );

    if ( !pParamValue )
    {
        return  0;
    }

    return  pParamValue->Variant.varUint32Array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UCHAR_ARRAY*
        DslhObjroRvqGetParamValueUcharArray
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called by the ObjController to retrieve the
        modified value of the specified parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the name of the parameter to be matched.

    return:     parameter value.

**********************************************************************/

SLAP_UCHAR_ARRAY*
DslhObjroRvqGetParamValueUcharArray
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject          = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PSLAP_VARIABLE                  pParamValue        = (PSLAP_VARIABLE             )NULL;
	PDSLH_RVQ_INTERFACE             pDslhRvqIf         = (PDSLH_RVQ_INTERFACE    )pMyObject->hDslhRvqIf;

    pParamValue =
        (PSLAP_VARIABLE)pDslhRvqIf->GetParamValue
            (
                (ANSC_HANDLE)pMyObject,
                pParamName
            );

    if ( !pParamValue )
    {
        return  NULL;
    }

    return  pParamValue->Variant.varUcharArray;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhObjroRvqGetParamValueHandle
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called by the ObjController to retrieve the
        modified value of the specified parameter.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the name of the parameter to be matched.

    return:     parameter value.

**********************************************************************/

ANSC_HANDLE
DslhObjroRvqGetParamValueHandle
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    )
{
    PDSLH_OBJ_RECORD_OBJECT         pMyObject          = (PDSLH_OBJ_RECORD_OBJECT    )hThisObject;
    PSLAP_VARIABLE                  pParamValue        = (PSLAP_VARIABLE             )NULL;
	PDSLH_RVQ_INTERFACE             pDslhRvqIf         = (PDSLH_RVQ_INTERFACE    )pMyObject->hDslhRvqIf;

    pParamValue =
        (PSLAP_VARIABLE)pDslhRvqIf->GetParamValue
            (
                (ANSC_HANDLE)pMyObject,
                pParamName
            );

    if ( !pParamValue )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  pParamValue->Variant.varHandle;
}
