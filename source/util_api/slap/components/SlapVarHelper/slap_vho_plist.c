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

    module:	slap_vho_plist.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced variable conversion
        functions of the Slap Var Helper Object.

        *   SlapVhoShiftLeftParamList
        *   SlapVhoShiftRightParamList
        *   SlapVhoEqualParamLists
        *   SlapVhoInitParamList
        *   SlapVhoCloneParamList
        *   SlapVhoCleanParamList

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/26/05    initial revision.

**********************************************************************/


#include "slap_vho_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        SlapVhoShiftLeftParamList
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 param_list,
                ULONG                       tbs_num
            );

    description:

        This function is called to shift the parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 param_list
                Specifies the parameter list to be operated on.

                ULONG                       tbs_num
                Specifies how far the parameters should be shifted.

    return:     none.

**********************************************************************/

void
SlapVhoShiftLeftParamList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 param_list,
        ULONG                       tbs_num
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_PARAMETER_LIST            tbs_list  = (PSLAP_PARAMETER_LIST)param_list;
    ULONG                           shift_num = AnscGetMin2(tbs_list->ParamCount, tbs_num);
    ULONG                           ii        = 0;

    for ( ii = 0; ii < shift_num; ii++ )
    {
        SlapCleanVariable((&tbs_list->ParamArray[ii]));
        SlapInitVariable ((&tbs_list->ParamArray[ii]));
    }

    for ( ii = 0; ii < (tbs_list->ParamCount - shift_num); ii++ )
    {
        tbs_list->ParamArray[ii] = tbs_list->ParamArray[ii + shift_num];

        SlapInitVariable((&tbs_list->ParamArray[ii + shift_num]));
    }

    tbs_list->ParamCount -= shift_num;

    return;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        SlapVhoShiftRightParamList
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 param_list,
                ULONG                       tbs_num
            );

    description:

        This function is called to shift the parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 param_list
                Specifies the parameter list to be operated on.

                ULONG                       tbs_num
                Specifies how far the parameters should be shifted.

    return:     none.

**********************************************************************/

void
SlapVhoShiftRightParamList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 param_list,
        ULONG                       tbs_num
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_PARAMETER_LIST            tbs_list  = (PSLAP_PARAMETER_LIST)param_list;
    ULONG                           shift_num = tbs_num;
    ULONG                           ii        = 0;

    for ( ii = (tbs_list->ParamCount + shift_num - 1); ii >= shift_num; ii-- )
    {
        tbs_list->ParamArray[ii] = tbs_list->ParamArray[ii - shift_num];

        SlapInitVariable((&tbs_list->ParamArray[ii - shift_num]));
    }

    for ( ii = 0; ii < shift_num; ii++ )
    {
        SlapCleanVariable((&tbs_list->ParamArray[ii]));
        SlapInitVariable ((&tbs_list->ParamArray[ii]));
    }

    tbs_list->ParamCount += shift_num;

    return;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapVhoEqualParamLists
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 list1,
                ANSC_HANDLE                 list2,
                BOOL                        b_syntax_only
            );

    description:

        This function is called to compare the parameter lists.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 list1
                Specifies the first parameter list to be compared.

                ANSC_HANDLE                 list2
                Specifies the second parameter list to be compared.

                BOOL                        b_syntax_only
                Specifies the comparison mode.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SlapVhoEqualParamLists
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 list1,
        ANSC_HANDLE                 list2,
        BOOL                        b_syntax_only
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_PARAMETER_LIST            param_list1 = (PSLAP_PARAMETER_LIST)list1;
    PSLAP_PARAMETER_LIST            param_list2 = (PSLAP_PARAMETER_LIST)list2;
    BOOL                            b_equal_pl  = TRUE;
    ULONG                           ii          = 0;
    BOOL                            b_same_var  = FALSE;
    SLAP_VARIABLE*                  slap_var1   = NULL;
    SLAP_VARIABLE*                  slap_var2   = NULL;

    do
    {
        if ( param_list1->ParamCount != param_list2->ParamCount )
        {
            b_equal_pl = FALSE;

            break;
        }

        for ( ii = 0; ii < param_list1->ParamCount; ii++ )
        {
            slap_var1 = &param_list1->ParamArray[ii];
            slap_var2 = &param_list2->ParamArray[ii];

            if ( b_syntax_only )
            {
                b_same_var = (slap_var1->Syntax == slap_var2->Syntax);
            }
            else
            {
                SlapEqualVariables(slap_var1, slap_var2, b_same_var);
            }

            if ( !b_same_var )
            {
                b_equal_pl = FALSE;

                break;
            }
        }

        break;
    }
    while ( FALSE );

    return  b_equal_pl;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        SlapVhoInitParamList
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 param_list
            );

    description:

        This function is called to initialie a parameter list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 param_list
                Specifies the parameter list to be initialized.

    return:     none.

**********************************************************************/

void
SlapVhoInitParamList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 param_list
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_PARAMETER_LIST            tbi_list = (PSLAP_PARAMETER_LIST)param_list;
    ULONG                           ii       = 0;

    tbi_list->ParamCount = 0;

    for ( ii = 0; ii < SLAP_MAX_PARAM_ARRAY_SIZE; ii++ )
    {
        SlapInitVariable((&tbi_list->ParamArray[ii]));
    }

    return;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        SlapVhoCloneParamList
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 src_param_list,
                ANSC_HANDLE*                dst_param_list
            );

    description:

        This function is called to clone a parameter list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 src_param_list
                Specifies the source parameter list to be cloned.

                ANSC_HANDLE*                dst_param_list
                Specifies the destination parameter list to be filled.

    return:     none.

**********************************************************************/

void
SlapVhoCloneParamList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 src_param_list,
        ANSC_HANDLE*                dst_param_list
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_PARAMETER_LIST            src_list = (PSLAP_PARAMETER_LIST)src_param_list;
    PSLAP_PARAMETER_LIST            dst_list = (PSLAP_PARAMETER_LIST)*dst_param_list;
    ULONG                           ii       = 0;

    if ( !dst_list )
    {
        dst_list =
            (PSLAP_PARAMETER_LIST)AnscAllocateMemory
                (
                    sizeof(SLAP_PARAMETER_LIST)
                );

        if ( dst_list )
        {
            dst_list->ParamCount = 0;
        }
    }
    else
    {
        SlapCleanParamList(dst_list);
    }

    if ( dst_list )
    {
        dst_list->ParamCount = src_list->ParamCount;

        for ( ii = 0; ii < src_list->ParamCount; ii++ )
        {
            SlapCloneVariable
                (
                    (&src_list->ParamArray[ii]),
                    (&dst_list->ParamArray[ii])
                );
        }
    }

    *dst_param_list = dst_list;

    /*CID:164052,164053 Resource leak in Bmc2ReqcoDoExecute()*/
    if (dst_list)
    {
	SlapFreeParamList(dst_list);
	/* CID 186730 fix */
	(dst_list) = NULL;
    }

    return;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        SlapVhoCleanParamList
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 param_list
            );

    description:

        This function is called to clean a parameter list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 param_list
                Specifies the parameter list to be cleaned.

    return:     none.

**********************************************************************/

void
SlapVhoCleanParamList
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 param_list
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_PARAMETER_LIST            tbc_list = (PSLAP_PARAMETER_LIST)param_list;
    ULONG                           ii       = 0;

    for ( ii = 0; ii < tbc_list->ParamCount; ii++ )
    {
        SlapCleanVariable((&tbc_list->ParamArray[ii]));
    }

    tbc_list->ParamCount = 0;

    return;
}
