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

    module:	slap_dslh_paramto_access.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object-access functions
        of the Slap Dslh ParamTree Object.

        *   SlapDslhParamtoGetChildObjNames
        *   SlapDslhParamtoGetParamNames
        *   SlapDslhParamtoGetParamValue
		*	SlapDslhParamtoGetParamTypeAndValue
        *   SlapDslhParamtoSetParamValue
        *   SlapDslhParamtoSetParamValueString
        *   SlapDslhParamtoSetParamValueInt
        *   SlapDslhParamtoSetParamValueUint
        *   SlapDslhParamtoSetParamValueBool
        *   SlapDslhParamtoApplyChanges
		*   SlapDslhParamtoGetParamInfo
		*   SlapDslhParamtoGetParamInfoShort
        *   SlapDslhParamtoGetHiddenInfo
		*	SlapDslhParamtoGetObjectInfo
        *   SlapDslhParamtoAddObject
        *   SlapDslhParamtoDelObject
        *   SlapDslhParamtoAcqWriteAccess
        *   SlapDslhParamtoRelWriteAccess
        *   SlapDslhParamtoIsParamTreeReadOnly
        *   SlapDslhParamtoIsParameterReadOnly
		*	SlapDslhParamtoEscapeXmlString

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin     Zhu

    ---------------------------------------------------------------

    revision:

        10/20/05    initial revision.
		11/17/06    Bin Zhu added new apis to support GUI;
        03/22/10    Bin added api "GetParamInfoFromCLI" to support CLI;
		10/06/11	* Ported to CCSP Cosa framework.
					* Combined the 2 sets of SlapDslhParamtoSetParamValue..
					* Renamed GetParamInfoFromCLI to GetParamInfoShort.
					* All apis now reqires "subsystem_prefix" to be passed 
					  in as additional parameters..
					* All "Set" apis require "sessionID", "writeID", and "commit"
					  to be passed in as addtional parameters.
					* SlapDslhParamtoGetHiddenInfo() is not implemented due to 
					  lack of d-bus api supports at this time

**********************************************************************/


#include "slap_dslh_paramto_global.h"
#include "safec_lib_common.h"

#define  TYPE_DSLH_PARAMETER_NAME						0
#define  TYPE_DSLH_ENTRY_NAME							1
#define  TYPE_DSLH_OTHER_NAME							2
#define  TYPE_DSLH_OBJECT_NAME		 TYPE_DSLH_OTHER_NAME

#define  COMMIT_YES										1

#define  SESSIOIN_ZERO									0
#define  SESSION_ACQ_FAIL								0

#define  MAXI_VALUE_LENGTH_IN_TABLE                     120

static  char		g_Subsystem[32] = {0};

ULONG
SlapSetSubsystem
    (
        char*           pSubsystem
    )
{
    errno_t  rc = -1;
    rc = STRCPY_S_NOCLOBBER(g_Subsystem, sizeof(g_Subsystem), pSubsystem);
    ERR_CHK(rc);
    return ANSC_STATUS_SUCCESS;
}


static ULONG
GetDslhNameType
	(
        char*                       pName
	)
{
	ULONG							length		= AnscSizeOfString(pName);
	BOOL							bIsEntry	= TRUE;
	char							c			= '.';
	ULONG							j			= 0;

	if( pName[length - 1] != '.')
	{
		return TYPE_DSLH_PARAMETER_NAME;
	}

	bIsEntry = TRUE;
	c		 = pName[length - 2];
	j		 = 0;

	while( c != '.' && j < 6)
	{
		if( c < '0' || c > '9')
		{
			bIsEntry = FALSE;

			break;
		}

		j++;
		c		 = pName[length - 2 - j];					
	}

	if( j >= 6)
	{
		bIsEntry = FALSE;
	}

	if( bIsEntry)
	{
		return TYPE_DSLH_ENTRY_NAME;
	}

	return TYPE_DSLH_OTHER_NAME;
}


ANSC_STATUS
SetParamValueHelper
    (
        ANSC_HANDLE                 MsgBusHandle,
	  	char*						pSubsystemPrefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
		parameterValStruct_t*		strValStruct
)
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_FAILURE;
	parameterValStruct_t **ppParamVal = NULL;
    componentStruct_t ** ppComponents = NULL;
    int ret   = 0;
    int size  = 0;
	char * dst_componentid =  NULL;
	char * dst_dbus_path    =  NULL;
	char * parameterNames[1]	=   {strValStruct[0].parameterName};
	char * pFaultParameter	= NULL;
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

	ret = CcspBaseIf_discComponentSupportingNamespace 
        (
            MsgBusHandle,
            cr_id,
            parameterNames[0],
            pSubsystemPrefix,
            &ppComponents,
            &size
        );

    if ( CCSP_SUCCESS != ret || 1 != size)  // there can be only one owner component of a leaf namespace in a subsystem 
    {
		free_componentStruct_t(MsgBusHandle, size, ppComponents); 
		if (CCSP_CR_ERR_UNSUPPORTED_NAMESPACE == ret ||  CCSP_CR_ERR_INVALID_PARAM	== ret )
			return 1;
		else
        return  returnStatus;
    }

    dst_componentid = AnscCloneString(ppComponents[0]->componentName);
    dst_dbus_path    = AnscCloneString(ppComponents[0]->dbusPath);
	free_componentStruct_t(MsgBusHandle, size, ppComponents);

    ret = CcspBaseIf_getParameterValues(
            MsgBusHandle,
            dst_componentid,
            dst_dbus_path,
            parameterNames,
            1,
            &size,
            &ppParamVal
        );

    if ( CCSP_SUCCESS != ret  || 1 != size )
	{
		free_parameterValStruct_t (MsgBusHandle, size, ppParamVal);    
		return  returnStatus;
	}
	
	if ( strValStruct[0].type != ppParamVal[0]->type)  //datatype miss match
	{
		//printf("Exit on Type Miss Match ..\n");
		free_parameterValStruct_t (MsgBusHandle, size, ppParamVal);    
		return  returnStatus;
	}

	free_parameterValStruct_t (MsgBusHandle, size, ppParamVal);    

	ret = CcspBaseIf_setParameterValues(
            MsgBusHandle,
            dst_componentid,
            dst_dbus_path,
			sessionID, 
			writeID,
			strValStruct,
			1,//set 1 param
			commit,
			&pFaultParameter
	);

	if ( pFaultParameter != NULL )
	{
       AnscFreeMemory(pFaultParameter);
	}

    if(ret != CCSP_SUCCESS )
    {
	    return  returnStatus;
    }
	
	returnStatus =  ANSC_STATUS_SUCCESS;
    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

		ULONG
		SlapDslhParamtoGetChildObjNames
			(
				ANSC_HANDLE					hThisObject,
	  			char*						pSubsystemPrefix,
				char*						pParamName,
				SLAP_STRING_ARRAY**			pNameArray
			)

    description:

        This function is called to get all the decendant object names start from
		the parent object. Leaf nodes are not included.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

				char*						pParamName,
				The parent parameter name. If it's empty, it should be the root;

				SLAP_STRING_ARRAY**			pNameArray,
				The output parameter name array;

    return:     status of operation.

**********************************************************************/

ULONG
SlapDslhParamtoGetChildObjNames
	(
		ANSC_HANDLE					hThisObject,
	  	char*						pSubsystemPrefix,
		char*						pParamName,
		SLAP_STRING_ARRAY**			pNameArray
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_FAILURE;
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	int ret   = 0;
    int size  = 0;
	int size2 = 0;
    parameterInfoStruct_t **ppParamInfos = NULL;
    componentStruct_t ** ppComponents = NULL;
	int i	= 0;
	int j   = 0;
	int prior_sz = 0;
	int count   = 0;
	SLAP_STRING_ARRAY*			pNameArrayCopyFrom = NULL;
	SLAP_STRING_ARRAY*			pNameArrayCopyTo = NULL;
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

	ret = CcspBaseIf_discComponentSupportingNamespace 
            (
                MsgBusHandle,
                cr_id,
                pParamName,
                pSubsystemPrefix,
                &ppComponents,
                &size
            );

    if ( CCSP_SUCCESS != ret || 1 > size) 
    {
		free_componentStruct_t(MsgBusHandle, size, ppComponents); 
		if (CCSP_CR_ERR_UNSUPPORTED_NAMESPACE == ret ||  CCSP_CR_ERR_INVALID_PARAM	== ret )
			return 1;
		else
        return  returnStatus;
    }

	count = 0;
	for (j = 0; j < size; j++)
	{
		ret = CcspBaseIf_getParameterNames
			(
				MsgBusHandle,
				ppComponents[j]->componentName,
				ppComponents[j]->dbusPath,
				pParamName,
				FALSE,
				&size2,
				&ppParamInfos
			);

		if ( CCSP_SUCCESS != ret) 
		{
			free_componentStruct_t(MsgBusHandle, size, ppComponents);
			free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);    
			if (pNameArrayCopyFrom != NULL)
			{
				SlapFreeVarArray(pNameArrayCopyFrom);
			}
			return  returnStatus;
		}
		
		count = 0;
		for( i = 0; i < size2; i ++)
		{
			if( ppParamInfos[i]->parameterName == NULL || AnscSizeOfString(ppParamInfos[i]->parameterName) == 0)
			{
				continue;
			}

			if( (ppParamInfos[i]->parameterName)[AnscSizeOfString(ppParamInfos[i]->parameterName) - 1] != '.')
			{
				continue;
			}

			// only get the none-leaf node 
			count ++;
		}

		if ( pNameArrayCopyFrom == NULL)
		{
			prior_sz = 0;
		}
		else
		{
			prior_sz = pNameArrayCopyFrom->VarCount;
		}

		SlapAllocStringArray2((count+prior_sz), pNameArrayCopyTo);

		for ( i=0; i<prior_sz; i++)
		{
			pNameArrayCopyTo->Array.arrayString[i] = AnscCloneString(pNameArrayCopyFrom->Array.arrayString[i]);
		}

		if (pNameArrayCopyFrom != NULL)
		{
			SlapFreeVarArray(pNameArrayCopyFrom);
		}
		
		count = 0;
		for( i = 0; i < size2; i ++)
		{
			if( ppParamInfos[i]->parameterName == NULL || AnscSizeOfString(ppParamInfos[i]->parameterName) == 0)
			{
				continue;
			}

			if( (ppParamInfos[i]->parameterName)[AnscSizeOfString(ppParamInfos[i]->parameterName) - 1] != '.')
			{
				continue;
			}

			// only get the none-leaf node 
			pNameArrayCopyTo->Array.arrayString[prior_sz+count]  = AnscCloneString(ppParamInfos[i]->parameterName);
			count ++;
		}

		free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);   

		pNameArrayCopyFrom = pNameArrayCopyTo;
		pNameArrayCopyTo = NULL;
	}

	free_componentStruct_t(MsgBusHandle, size, ppComponents); 

	*pNameArray = pNameArrayCopyFrom;
	returnStatus = ANSC_STATUS_SUCCESS;

	return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

		ULONG
		SlapDslhParamtoGetParamNames
			(
				ANSC_HANDLE					hThisObject,
				char*						pParamName,
                ULONG                       uNextLevel,
				SLAP_STRING_ARRAY**			pNameArray
			);

    description:

        This function is called to get all the child parameter names start from
		the parent param. Leaf nodes are included.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       uNextLevel,
                Next Level only or not;

				char*						pParamName,
				The parent parameter name. If it's empty, it should be the root;

				SLAP_STRING_ARRAY**			pNameArray,
				The output parameter name array;

    return:     status of operation.

**********************************************************************/

ULONG
SlapDslhParamtoGetParamNames
	(
		ANSC_HANDLE					hThisObject,
	  	char*						pSubsystemPrefix,
		char*						pParamName,
        ULONG                       uNextLevel,
		SLAP_STRING_ARRAY**			pNameArray
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_FAILURE;
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	int ret   = 0;
    int size  = 0;
	int size2 = 0;
    parameterInfoStruct_t **ppParamInfos = NULL;
    componentStruct_t ** ppComponents = NULL;
	int i	= 0;
	int j   = 0;
	int prior_sz = 0;
	SLAP_STRING_ARRAY*			pNameArrayCopyFrom = NULL;
	SLAP_STRING_ARRAY*			pNameArrayCopyTo = NULL;
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

	ret = CcspBaseIf_discComponentSupportingNamespace 
            (
                MsgBusHandle,
                cr_id,
                pParamName,
                pSubsystemPrefix,
                &ppComponents,
                &size
            );

    if ( CCSP_SUCCESS != ret || 1 > size) 
    {
		free_componentStruct_t(MsgBusHandle, size, ppComponents); 
		if (CCSP_CR_ERR_UNSUPPORTED_NAMESPACE == ret ||  CCSP_CR_ERR_INVALID_PARAM	== ret )
			return 1;
		else
        return  returnStatus;
    }		
	
	for (j = 0; j < size; j++)
	{
		ret = CcspBaseIf_getParameterNames
			(
				MsgBusHandle,
				ppComponents[j]->componentName,
				ppComponents[j]->dbusPath,
				pParamName,
				uNextLevel,
				&size2,
				&ppParamInfos
			);

		if ( CCSP_SUCCESS != ret) 
		{
			free_componentStruct_t(MsgBusHandle, size, ppComponents);
			free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);    
			if (pNameArrayCopyFrom != NULL)
			{
				SlapFreeVarArray(pNameArrayCopyFrom);
			}
			return  returnStatus;
		}

		if ( pNameArrayCopyFrom == NULL)
		{
			prior_sz = 0;
		}
		else
		{
			prior_sz = pNameArrayCopyFrom->VarCount;
		}

		SlapAllocStringArray2((size2+prior_sz), pNameArrayCopyTo);

		for ( i=0; i<prior_sz; i++)
		{
			pNameArrayCopyTo->Array.arrayString[i] = AnscCloneString(pNameArrayCopyFrom->Array.arrayString[i]);
		}

		if (pNameArrayCopyFrom != NULL)
		{
			SlapFreeVarArray(pNameArrayCopyFrom);
		}

		for( i = 0; i < size2; i ++)
		{
			pNameArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString(ppParamInfos[i]->parameterName);
		}

		free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);   

		pNameArrayCopyFrom = pNameArrayCopyTo;
		pNameArrayCopyTo = NULL;
	}

	free_componentStruct_t(MsgBusHandle, size, ppComponents); 

	*pNameArray = pNameArrayCopyFrom;

	returnStatus = ANSC_STATUS_SUCCESS;

	return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        SlapDslhParamtoGetParamValue
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called to retrieve a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter whose value is to be retrieved.

    return:     parameter value.

**********************************************************************/

SLAP_VARIABLE*
SlapDslhParamtoGetParamValue
    (
        ANSC_HANDLE                 hThisObject,
	  	char*				pSubsystemPrefix,
        char*                       pParamName
    )
{
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
    SLAP_VARIABLE *                  pRetSlapVal           = NULL;
    int ret   = 0;
    int size  = 0;
    parameterValStruct_t **ppParamVal = NULL;
    componentStruct_t ** ppComponents = NULL;
	char * dst_componentid =  NULL;
	char * dst_dbus_path    =  NULL;
	char * parameterNames[1]	=   {pParamName};
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }


	if (TYPE_DSLH_PARAMETER_NAME != GetDslhNameType(pParamName))
	{
		return NULL;  //object or instance name 
	}

	ret = CcspBaseIf_discComponentSupportingNamespace 
            (
                MsgBusHandle,
                cr_id,
                pParamName,
                pSubsystemPrefix,
                &ppComponents,
                &size
            );

    if ( CCSP_SUCCESS == ret && 1 == size)  /* there can be only one owner of a leaf namespace in a subsystem */
    {
        dst_componentid = AnscCloneString(ppComponents[0]->componentName);
        dst_dbus_path    = AnscCloneString(ppComponents[0]->dbusPath);
		free_componentStruct_t(MsgBusHandle, size, ppComponents);
    }
    else
    {
		free_componentStruct_t(MsgBusHandle, size, ppComponents); 
        return  NULL;
    }

    ret = CcspBaseIf_getParameterValues(
                MsgBusHandle,
                dst_componentid,
                dst_dbus_path,
                parameterNames,
                1,
                &size,
                &ppParamVal
            );

    if ( CCSP_SUCCESS == ret  && 1 == size && ccsp_none != (ppParamVal[0]->type) )
    {
		pRetSlapVal = AnscAllocateMemory(sizeof(SLAP_VARIABLE));
        SlapInitVariable((pRetSlapVal));

		switch( (ppParamVal[0]->type))
		{
			case ccsp_int:
			case ccsp_long:
				pRetSlapVal->Syntax         = SLAP_VAR_SYNTAX_int;
				pRetSlapVal->Variant.varInt = SlapVcoStringToInt(NULL, ppParamVal[0]->parameterValue);
					break;

			case ccsp_unsignedInt:
			case ccsp_unsignedLong:
				pRetSlapVal->Syntax            = SLAP_VAR_SYNTAX_uint32;
				pRetSlapVal->Variant.varUint32 = SlapVcoStringToUint32(NULL, ppParamVal[0]->parameterValue);
					break;

			case ccsp_boolean:
				pRetSlapVal->Syntax          = SLAP_VAR_SYNTAX_bool;
				pRetSlapVal->Variant.varBool = SlapVcoStringToBool(NULL, ppParamVal[0]->parameterValue); 
					break;

			default:	/*This includes following ccsp types that has no match in SLAP_VAR_SYNTAX types 
						case ccsp_string:    
						case ccsp_dateTime:
						case ccsp_base64:
						case ccsp_float:
						case ccsp_double:
						case ccsp_byte:*/
				pRetSlapVal->Syntax            = SLAP_VAR_SYNTAX_string;
				pRetSlapVal->Variant.varString = AnscCloneString(ppParamVal[0]->parameterValue);
				break;
		}

    }
	free_parameterValStruct_t(MsgBusHandle, size, ppParamVal);
	return pRetSlapVal;      
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        SlapDslhParamtoGetParamTypeAndValue
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName,
				char**						ppType,
				char**						ppValue
            );

    description:

        This function is called to retrieve a parameter type and value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter whose value is to be retrieved.

  				char**						ppType,
				The output of parameter type;

				char**						ppValue
				The output of parameter value;

    return:     none

**********************************************************************/
void
SlapDslhParamtoGetParamTypeAndValue
	(
		ANSC_HANDLE					hThisObject,
	  	char*				pSubsystemPrefix,
		char*						pParamName,
		char**						ppType,
		char**						ppValue
	)
{
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
    int ret   = 0;
    int size  = 0;
    parameterValStruct_t **ppParamVal = NULL;
    componentStruct_t ** ppComponents = NULL;
	char * dst_componentid =  NULL;
	char * dst_dbus_path    =  NULL;
	char * parameterNames[1]	=   {pParamName};
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

	*ppType = AnscCloneString("unknown");
	*ppValue = AnscCloneString("unknown");

	if (TYPE_DSLH_PARAMETER_NAME != GetDslhNameType(pParamName))
	{
		return ;  //object or instance name 
	}

	ret = CcspBaseIf_discComponentSupportingNamespace 
            (
                MsgBusHandle,
                cr_id,
                pParamName,
                pSubsystemPrefix,
                &ppComponents,
                &size
            );

		ret = CcspBaseIf_discComponentSupportingNamespace 
            (
                MsgBusHandle,
                cr_id,
                pParamName,
                "",
                &ppComponents,
                &size
            );


    if ( CCSP_SUCCESS == ret && 1 == size)  // there can be only one owner of a leaf namespace in a subsystem 
    {
        dst_componentid = AnscCloneString(ppComponents[0]->componentName);
        dst_dbus_path    = AnscCloneString(ppComponents[0]->dbusPath);
		free_componentStruct_t(MsgBusHandle, size, ppComponents);
    }
    else
    {
		free_componentStruct_t(MsgBusHandle, size, ppComponents); 
        return ;
    }

    ret = CcspBaseIf_getParameterValues(
                MsgBusHandle,
                dst_componentid,
                dst_dbus_path,
                parameterNames,
                1,
                &size,
                &ppParamVal
            );

    if ( CCSP_SUCCESS == ret  && 1 == size && ccsp_none != (ppParamVal[0]->type) )
    {
		switch( (ppParamVal[0]->type))
		{
			case ccsp_string: 
				*ppType = AnscCloneString("string");
				break;

			case ccsp_int:
			case ccsp_long:
				*ppType = AnscCloneString("int");
				break;

			case ccsp_unsignedInt:
			case ccsp_unsignedLong:
				*ppType = AnscCloneString("unsignedInt");
				break;

			case ccsp_boolean:
				*ppType = AnscCloneString("boolean");
				break;

			case ccsp_dateTime: 
				*ppType = AnscCloneString("dateTime");
				break;

			case ccsp_base64: 
				*ppType = AnscCloneString("base64");
				break;

			case ccsp_float: 
				*ppType = AnscCloneString("float");
				break;

			case ccsp_double: 
				*ppType = AnscCloneString("double");
				break;

			case ccsp_byte: 
				*ppType = AnscCloneString("byte");
				break;
			default:	//"unknown" 
				break;
		}
		*ppValue = AnscCloneString((ppParamVal[0]->parameterValue));
    }
	free_parameterValStruct_t(MsgBusHandle, size, ppParamVal);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapDslhParamtoSetParamValue
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName,
                SLAP_VARIABLE*              pParamValue
            );

    description:

        This function is called to configure a parameter value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter whose value is to be configure.

                SLAP_VARIABLE*              pParamValue
                Specifies the parameter value to be set.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapDslhParamtoSetParamValue
    (
        ANSC_HANDLE                 hThisObject,
	  	char*				pSubsystemPrefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_FAILURE;
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	parameterValStruct_t strValStruct[1] = {{0}};
	char buf[32] = {'\0'};
    errno_t rc = -1;

	if (TYPE_DSLH_PARAMETER_NAME != GetDslhNameType(pParamName))
	{
		return 1;  //object or instance name 
	}
    
    strValStruct[0].parameterName  = AnscCloneString(pParamName);

    switch( (pParamValue->Syntax))
    {
        case SLAP_VAR_SYNTAX_int:
            strValStruct[0].type         = ccsp_int;
            rc = sprintf_s(buf, sizeof(buf), "%d", pParamValue->Variant.varInt);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
            strValStruct[0].parameterValue = AnscCloneString(buf);
            break;

        case SLAP_VAR_SYNTAX_uint32:
            strValStruct[0].type            = ccsp_unsignedInt;
            rc = sprintf_s(buf, sizeof(buf), "%d", (int)pParamValue->Variant.varUint32);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
            strValStruct[0].parameterValue = AnscCloneString(buf);
            break;

		case SLAP_VAR_SYNTAX_bool:
			strValStruct[0].type          = ccsp_boolean;
			if ( pParamValue->Variant.varBool == TRUE )
			{
				strValStruct[0].parameterValue = AnscCloneString("true");
			}
			else
			{
				strValStruct[0].parameterValue = AnscCloneString("false");
			}
				break;

		case SLAP_VAR_SYNTAX_string:
			strValStruct[0].type          = ccsp_string;
			if ( NULL == pParamValue->Variant.varString )
			{
			   strValStruct[0].parameterValue = AnscCloneString("");
			}
			else
			{
			   strValStruct[0].parameterValue = AnscCloneString(pParamValue->Variant.varString);
			}
				break;

		default:	/*CCSP datatypes has no match for other SLAP_VAR_SYNTAX...*/
			return returnStatus;
			break;
	}

	return SetParamValueHelper(
							MsgBusHandle, 
							pSubsystemPrefix, 
							sessionID, 
							writeID, 
							commit, 
							strValStruct);

}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapDslhParamtoSetParamValueString
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName,
                char*                       strValue
            );

    description:

        This function is called to configure a parameter value of string type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter whose value is to be configure.

                char*                       strValue
                the new string value

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapDslhParamtoSetParamValueString
    (
        ANSC_HANDLE                 hThisObject,
	  	char*						pSubsystemPrefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        char*                       strValue
    )
{
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	parameterValStruct_t strValStruct[1] = {{0}};

	if (TYPE_DSLH_PARAMETER_NAME != GetDslhNameType(pParamName))
	{
		return 1;  //object or instance name 
	}
    
	strValStruct[0].parameterName  = AnscCloneString(pParamName);
	strValStruct[0].type  = ccsp_string;

	if ( NULL == strValue )
	{
       strValStruct[0].parameterValue = AnscCloneString("");
	}
	else
	{
       strValStruct[0].parameterValue = AnscCloneString(strValue);
	}

	return SetParamValueHelper(
							MsgBusHandle, 
							pSubsystemPrefix, 
							sessionID, 
							writeID, 
							commit, 
							strValStruct);

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapDslhParamtoSetParamValueInt
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName,
                int                         intValue
            );

    description:

        This function is called to configure a parameter value of int type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter whose value is to be configure.

                int                         intValue
                the new value

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapDslhParamtoSetParamValueInt
    (
        ANSC_HANDLE                 hThisObject,
	  	char*				pSubsystemPrefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        int                         intValue
    )
{
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	parameterValStruct_t strValStruct[1] = {{0}};
	char   buf[32] = {'\0'};
    errno_t rc = -1;

	if (TYPE_DSLH_PARAMETER_NAME != GetDslhNameType(pParamName))
	{
		return 1;  //object or instance name 
	}
    
	strValStruct[0].parameterName  = AnscCloneString(pParamName);
	strValStruct[0].type  = ccsp_int;
	rc = sprintf_s(buf, sizeof(buf), "%d", intValue);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    strValStruct[0].parameterValue = AnscCloneString(buf);

	return SetParamValueHelper(
							MsgBusHandle, 
							pSubsystemPrefix, 
							sessionID, 
							writeID, 
							commit, 
							strValStruct);

}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapDslhParamtoSetParamValueUint
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName,
                ULONG                       newValue
            );

    description:

        This function is called to configure a parameter value of uint type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter whose value is to be configure.

                ULONG                       newValue
                the new value

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapDslhParamtoSetParamValueUint
    (
        ANSC_HANDLE                 hThisObject,
	  	char*				pSubsystemPrefix,
		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        ULONG                       newValue
    )
{
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	parameterValStruct_t strValStruct[1] = {{0}};
	char   buf[32] = {'\0'};;
    errno_t rc = -1;

	if (TYPE_DSLH_PARAMETER_NAME != GetDslhNameType(pParamName))
	{
		return 1;  //object or instance name 
	}
    
	strValStruct[0].parameterName  = AnscCloneString(pParamName);
	strValStruct[0].type  = ccsp_unsignedInt;
	rc = sprintf_s(buf, sizeof(buf), "%u", (UINT)newValue);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    strValStruct[0].parameterValue = AnscCloneString(buf);

	return SetParamValueHelper(
							MsgBusHandle, 
							pSubsystemPrefix, 
							sessionID, 
							writeID, 
							commit, 
							strValStruct);

}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapDslhParamtoSetParamValueBool
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName,
                SLAP_BOOL                   newValue
            );

    description:

        This function is called to configure a parameter value of 
		boolean type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter whose value is to be configure.

                SLAP_BOOL                   newValue
                the new value

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapDslhParamtoSetParamValueBool
    (
        ANSC_HANDLE                 hThisObject,
	  	char*						pSubsystemPrefix,
 		int							sessionID,
		unsigned int				writeID,
		BOOL						commit,
        char*                       pParamName,
        SLAP_BOOL                   newValue
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_FAILURE;
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	parameterValStruct_t strValStruct[1] = {{0}};

	if (TYPE_DSLH_PARAMETER_NAME != GetDslhNameType(pParamName))
	{
		return 1;  //object or instance name 
	}
    
	strValStruct[0].parameterName  = AnscCloneString(pParamName);
	strValStruct[0].type  = ccsp_boolean;

	if ( TRUE == newValue )
	{
       strValStruct[0].parameterValue = AnscCloneString("true");
	}
	else if ( FALSE == newValue )
	{
       strValStruct[0].parameterValue = AnscCloneString("false");
	}
	else
	{
		return	returnStatus ;
	}

	return SetParamValueHelper(
							MsgBusHandle, 
							pSubsystemPrefix, 
							sessionID, 
							writeID, 
							commit, 
							strValStruct);

}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapDslhParamtoApplyChanges
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName
            );

    description:

        This function is called to apply all previous changes made to
        the parameters under the specified object. 
		Caller must call SlapDslhParamtoAcqWriteAccess to lock an 
		Active Session prior to Apply Changes.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                Specifies the object whose changes to be applied.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapDslhParamtoApplyChanges
    (
        ANSC_HANDLE                 hThisObject,
	  	char*						pSubsystemPrefix,
		int							sessionID,
		unsigned int				writeID,
        char*                       pObjName
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_FAILURE;
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	int ret   = 0;
    int size  = 0;
    componentStruct_t ** ppComponents = NULL;
	int i	= 0;
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

	ret = CcspBaseIf_discComponentSupportingNamespace 
            (
                MsgBusHandle,
                cr_id,
                pObjName,
                pSubsystemPrefix,
                &ppComponents,
                &size
            );

    if ( CCSP_SUCCESS != ret || 1 > size) 
    {
		free_componentStruct_t(MsgBusHandle, size, ppComponents); 
		if (CCSP_CR_ERR_UNSUPPORTED_NAMESPACE == ret ||  CCSP_CR_ERR_INVALID_PARAM	== ret )
			return ANSC_STATUS_BAD_NAME;
		else
        return  returnStatus;
    }

	for (i = 0; i < size; i++)
	{
		ret = CcspBaseIf_setCommit
			(
				MsgBusHandle,
				ppComponents[i]->componentName,
				ppComponents[i]->dbusPath,
				sessionID,
				writeID,
				COMMIT_YES
			);

		if ( CCSP_SUCCESS != ret) 
		{
			//printf("Commit Failded on Component: %s \n",ppComponents[i]->componentName);
			return  returnStatus;
		}
	}

	free_componentStruct_t(MsgBusHandle, size, ppComponents); 

	returnStatus = ANSC_STATUS_SUCCESS;
    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

		ULONG
		SlapDslhParamtoGetParamInfo
			(
				ANSC_HANDLE					hThisObject,
				char*						pParamName,
				ULONG                       uNextLevel,
				SLAP_STRING_ARRAY**			pNameArray,
				SLAP_INT_ARRAY**			pAccessArray,
				SLAP_STRING_ARRAY**			pTypeArray,
				SLAP_STRING_ARRAY**			pValueArray
				SLAP_STRING_ARRAY**			pAttrArray
			);

    description:

        This function is called to get all the parameter information start from
		the parent param.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

				char*						pParamName,
				The parent parameter name. If it's empty, it should be the root;

				ULONG                       uNextLevel,
				The next level is true or not;

				SLAP_STRING_ARRAY**			pNameArray,
				The output parameter name array;

				SLAP_INT_ARRAY**			pAccessArray
				The output access array;

				SLAP_STRING_ARRAY**			pTypeArray,
				The output Type Array;

				SLAP_STRING_ARRAY**			pValueArray
				The output Value Array;

				SLAP_STRING_ARRAY**			pAttrArray
				The output attribute array;

    return:     status of operation.

**********************************************************************/

ULONG
SlapDslhParamtoGetParamInfo
	(
		ANSC_HANDLE					hThisObject,
	  	char*				pSubsystemPrefix,
		char*						pParamName,
		ULONG                       uNextLevel,
		SLAP_STRING_ARRAY**			pNameArray,
		SLAP_INT_ARRAY**			pAccessArray,
		SLAP_STRING_ARRAY**			pTypeArray,
		SLAP_STRING_ARRAY**			pValueArray,
		SLAP_STRING_ARRAY**			pAttrArray
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_FAILURE;
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	int ret   = 0;
    int size  = 0;
	int size2 = 0;
	int size3 = 0;
	int size4 = 0;
    parameterValStruct_t **ppParamVal = NULL;
    parameterInfoStruct_t **ppParamInfos = NULL;
    componentStruct_t ** ppComponents = NULL;
	parameterAttributeStruct_t **ppParamAttri = NULL;
	char * parameterNames[1024];
	char	pValueBuf[1024]  = { '\0' };
	int j	= 0;
	int i	= 0;
	int valueLen = 0;
	int leaf_cnt = 0;
	int prior_sz = 0;
	SLAP_STRING_ARRAY*			pNameArrayCopyFrom = NULL;
    SLAP_STRING_ARRAY*			pTypeArrayCopyFrom = NULL;
	SLAP_STRING_ARRAY*			pValueArrayCopyFrom = NULL;
	SLAP_STRING_ARRAY*			pAttrArrayCopyFrom = NULL;
	SLAP_INT_ARRAY*				pAccessArrayCopyFrom = NULL;
	SLAP_STRING_ARRAY*			pNameArrayCopyTo = NULL;
    SLAP_STRING_ARRAY*			pTypeArrayCopyTo = NULL;
	SLAP_STRING_ARRAY*			pValueArrayCopyTo = NULL;
	SLAP_STRING_ARRAY*			pAttrArrayCopyTo = NULL;
	SLAP_INT_ARRAY*				pAccessArrayCopyTo = NULL;
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
	
	if( pParamName == NULL || strcmp(pParamName, "") == 0 )
	{
		pParamName = AnscCloneString("Device.");
	}


	ret = CcspBaseIf_discComponentSupportingNamespace 
            (
                MsgBusHandle,
                cr_id,
                pParamName,
                pSubsystemPrefix,
                &ppComponents,
                &size
            );

    if ( CCSP_SUCCESS != ret || 1 > size) 
    {
		free_componentStruct_t(MsgBusHandle, size, ppComponents); 
		if (CCSP_CR_ERR_UNSUPPORTED_NAMESPACE == ret ||  CCSP_CR_ERR_INVALID_PARAM	== ret )
			return 1;
		else
        return  returnStatus;
    } 

	for (j = 0; j < size; j++)
	{
		ret = CcspBaseIf_getParameterNames
			(
				MsgBusHandle,
				ppComponents[j]->componentName,
				ppComponents[j]->dbusPath,
				pParamName,
				TRUE,//only need next level names. CcspBaseIf_getParameterValues() can find all decendants.
				&size2,
				&ppParamInfos
			);

		if ( CCSP_SUCCESS != ret ) 
		{
			free_componentStruct_t(MsgBusHandle, size, ppComponents);
			free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);    
			if (pNameArrayCopyFrom != NULL)
			{
				SlapFreeVarArray(pNameArrayCopyFrom);
				SlapFreeVarArray(pTypeArrayCopyFrom);
				SlapFreeVarArray(pValueArrayCopyFrom);
				SlapFreeVarArray(pAttrArrayCopyFrom);
				SlapFreeVarArray(pAccessArrayCopyFrom);
			}
			return  returnStatus;
		}

		if ( size2 < 1)
		{
			free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);    
			continue;
		}

		leaf_cnt = 0;

		for ( i = 0; i < size2; i++ )
		{
			if ( 1 == uNextLevel )//only pass leaf-node names to CcspBaseIf_getParameterValues()
			{
				if( TYPE_DSLH_PARAMETER_NAME == GetDslhNameType(ppParamInfos[i]->parameterName)) 
				{
					parameterNames[leaf_cnt++] = ppParamInfos[i]->parameterName;
				}
			}
			else//pass all names to CcspBaseIf_getParameterValues()
			{
					parameterNames[leaf_cnt++] = ppParamInfos[i]->parameterName;
			}
		}

		if ( 0 != leaf_cnt)
		{
			ret = CcspBaseIf_getParameterValues(
						MsgBusHandle,
						ppComponents[j]->componentName,
						ppComponents[j]->dbusPath,
						parameterNames,
						leaf_cnt,
						&size3,
						&ppParamVal
					);

			if ( CCSP_SUCCESS != ret || ( 1 == uNextLevel && size3 != leaf_cnt ) )
			{
				free_componentStruct_t(MsgBusHandle, size, ppComponents);
				free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);  
				free_parameterValStruct_t(MsgBusHandle, size3, ppParamVal);
				if (pNameArrayCopyFrom != NULL)
				{
					SlapFreeVarArray(pNameArrayCopyFrom);
					SlapFreeVarArray(pTypeArrayCopyFrom);
					SlapFreeVarArray(pValueArrayCopyFrom);
					SlapFreeVarArray(pAttrArrayCopyFrom);
					SlapFreeVarArray(pAccessArrayCopyFrom);
				}
				return  returnStatus;
			}

			ret = CcspBaseIf_getParameterAttributes(
						MsgBusHandle,
						ppComponents[j]->componentName,
						ppComponents[j]->dbusPath,
						parameterNames,
						leaf_cnt,
						&size4,
						&ppParamAttri

					);

			if ( CCSP_SUCCESS != ret || size3 != size4 || ( 1 == uNextLevel && size4 != leaf_cnt ) )
			{
				free_componentStruct_t(MsgBusHandle, size, ppComponents);
				free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);  
				free_parameterValStruct_t(MsgBusHandle, size3, ppParamVal);
				free_parameterAttributeStruct_t(MsgBusHandle, size4, ppParamAttri);
				if (pNameArrayCopyFrom != NULL)
				{
					SlapFreeVarArray(pNameArrayCopyFrom);
					SlapFreeVarArray(pTypeArrayCopyFrom);
					SlapFreeVarArray(pValueArrayCopyFrom);
					SlapFreeVarArray(pAttrArrayCopyFrom);
					SlapFreeVarArray(pAccessArrayCopyFrom);
				}
				return  returnStatus;
			}
		}
		else
		{
			size3 = 0;
			size4 = 0;
		}
		
		if ( 0 == uNextLevel )
		{	//re-get ppParamInfos to to include param/obj/entry from all levels
			free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);  
			ret = CcspBaseIf_getParameterNames
				(
					MsgBusHandle,
					ppComponents[j]->componentName,
					ppComponents[j]->dbusPath,
					pParamName,
					FALSE,
					&size2,
					&ppParamInfos
				);
			if ( CCSP_SUCCESS != ret ) 
			{
				free_componentStruct_t(MsgBusHandle, size, ppComponents);
				free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos); 
				free_parameterValStruct_t(MsgBusHandle, size3, ppParamVal);
				free_parameterAttributeStruct_t(MsgBusHandle, size4, ppParamAttri);
				if (pNameArrayCopyFrom != NULL)
				{
					SlapFreeVarArray(pNameArrayCopyFrom);
					SlapFreeVarArray(pTypeArrayCopyFrom);
					SlapFreeVarArray(pValueArrayCopyFrom);
					SlapFreeVarArray(pAttrArrayCopyFrom);
					SlapFreeVarArray(pAccessArrayCopyFrom);
				}
				return  returnStatus;
			}

			if ( size2 < 1)
			{
				free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos); 
				free_parameterValStruct_t(MsgBusHandle, size3, ppParamVal);
				free_parameterAttributeStruct_t(MsgBusHandle, size4, ppParamAttri);
				continue;
			}
		}

		leaf_cnt = 0;

		if ( pNameArrayCopyFrom == NULL)
		{
			prior_sz = 0;
		}
		else
		{
			prior_sz = pNameArrayCopyFrom->VarCount;
		}

		SlapAllocStringArray2((size2+prior_sz), pNameArrayCopyTo);
		SlapAllocStringArray2((size2+prior_sz), pTypeArrayCopyTo);
		SlapAllocStringArray2((size2+prior_sz), pValueArrayCopyTo);
		SlapAllocStringArray2((size2+prior_sz), pAttrArrayCopyTo);
		SlapAllocIntArray(0, sizeof(SLAP_INT) * (size2+prior_sz), pAccessArrayCopyTo);

		for ( i=0; i<prior_sz; i++)
		{
			pNameArrayCopyTo->Array.arrayString[i] = AnscCloneString(pNameArrayCopyFrom->Array.arrayString[i]);
			pTypeArrayCopyTo->Array.arrayString[i] = AnscCloneString(pTypeArrayCopyFrom->Array.arrayString[i]);
			pValueArrayCopyTo->Array.arrayString[i] = AnscCloneString(pValueArrayCopyFrom->Array.arrayString[i]);
			pAttrArrayCopyTo->Array.arrayString[i] = AnscCloneString(pAttrArrayCopyFrom->Array.arrayString[i]);
			pAccessArrayCopyTo->Array.arrayInt[i] = pAccessArrayCopyFrom->Array.arrayInt[i];
		}

		if (pNameArrayCopyFrom != NULL)
		{
			SlapFreeVarArray(pNameArrayCopyFrom);
			SlapFreeVarArray(pTypeArrayCopyFrom);
			SlapFreeVarArray(pValueArrayCopyFrom);
			SlapFreeVarArray(pAttrArrayCopyFrom);
			SlapFreeVarArray(pAccessArrayCopyFrom);
		}

		for ( i = 0; i < size2; i++ )
		{
			// clone parameter name without the  parent-prefix portion  
			pNameArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString(ppParamInfos[i]->parameterName);

			if( true == ppParamInfos[i]->writable)
			{

				if( TYPE_DSLH_PARAMETER_NAME == GetDslhNameType(ppParamInfos[i]->parameterName)) //writable prameter
				{
					pAccessArrayCopyTo->Array.arrayInt[prior_sz+i] = 1;
				}
				else if( TYPE_DSLH_ENTRY_NAME == GetDslhNameType(ppParamInfos[i]->parameterName))//writable(deletable) entry
				{
					pAccessArrayCopyTo->Array.arrayInt[prior_sz+i] = 2;
				}
				else  //writable(addable) object
				{
					pAccessArrayCopyTo->Array.arrayInt[prior_sz+i] = 3;
				}
			}
			else
			{
				pAccessArrayCopyTo->Array.arrayInt[prior_sz+i] = 0;
			}

			if( 0 < size3 && leaf_cnt < size3 && strcmp(ppParamInfos[i]->parameterName, ppParamVal[leaf_cnt]->parameterName) == 0 )
			{

				if ( true == ppParamAttri[leaf_cnt]->notificationChanged )
				{	
					if ( true == ppParamAttri[leaf_cnt]->notification )
					{
						pAttrArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("11");
					}
					else
					{
						pAttrArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("10");
					}
				}
				else
				{
					if ( true == ppParamAttri[leaf_cnt]->notification )
					{
						pAttrArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("01");
					}
					else
					{
						pAttrArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("00");
					}
				}

				switch( (ppParamVal[leaf_cnt]->type))
				{
					case ccsp_string: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("string");
						break;

					case ccsp_int:
					case ccsp_long:
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("int");
						break;

					case ccsp_unsignedInt:
					case ccsp_unsignedLong:
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("unsignedInt");
						break;

					case ccsp_boolean:
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("boolean");
						break;

					case ccsp_dateTime: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("dateTime");
						break;

					case ccsp_base64: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("base64");
						break;

					case ccsp_float: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("float");
						break;

					case ccsp_double: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("double");
						break;

					case ccsp_byte: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("byte");
						break;

					case ccsp_none: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("object");
						break;

					default:	//"unknown" 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("unknown");
						break;
				}

				if( ppParamVal[leaf_cnt]->parameterValue == NULL)
				{
					pValueArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("(EMPTY)");
				}
				else
				{
					valueLen = AnscSizeOfString(ppParamVal[leaf_cnt]->parameterValue);

					if( valueLen == 0)
					{
						pValueArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("(EMPTY)");
					}
					else if( valueLen <= MAXI_VALUE_LENGTH_IN_TABLE)
					{
						pValueArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString(ppParamVal[leaf_cnt]->parameterValue);
					}
					else
					{
						AnscZeroMemory(pValueBuf, MAXI_VALUE_LENGTH_IN_TABLE + 8);

						AnscCopyMemory(pValueBuf, ppParamVal[leaf_cnt]->parameterValue, MAXI_VALUE_LENGTH_IN_TABLE);
						pValueBuf[MAXI_VALUE_LENGTH_IN_TABLE] = '.';
						pValueBuf[MAXI_VALUE_LENGTH_IN_TABLE + 1] = '.';
						pValueBuf[MAXI_VALUE_LENGTH_IN_TABLE + 2] = '.';

						pValueArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString(pValueBuf);
					}
				}

				leaf_cnt++;
			}
			else
			{
				if (TYPE_DSLH_ENTRY_NAME == GetDslhNameType(ppParamInfos[i]->parameterName))
				{
					pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("entry");
				}
				else
				{
					pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("object");
				}

				pValueArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("");
				pAttrArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("");
			}
		}

		free_parameterValStruct_t(MsgBusHandle, size3, ppParamVal);
		free_parameterAttributeStruct_t(MsgBusHandle, size4, ppParamAttri);
		free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);  

		pNameArrayCopyFrom = pNameArrayCopyTo;
		pTypeArrayCopyFrom = pTypeArrayCopyTo;
		pValueArrayCopyFrom = pValueArrayCopyTo;
		pAttrArrayCopyFrom = pAttrArrayCopyTo;
		pAccessArrayCopyFrom = pAccessArrayCopyTo;

		pNameArrayCopyTo = NULL;
		pTypeArrayCopyTo = NULL;
		pValueArrayCopyTo = NULL;
		pAttrArrayCopyTo = NULL;
		pAccessArrayCopyTo = NULL;
	}

	free_componentStruct_t(MsgBusHandle, size, ppComponents); 

	*pNameArray = pNameArrayCopyFrom;
	*pTypeArray = pTypeArrayCopyFrom;
	*pValueArray = pValueArrayCopyFrom;
	*pAttrArray = pAttrArrayCopyFrom;
	*pAccessArray = pAccessArrayCopyFrom;

	returnStatus = ANSC_STATUS_SUCCESS;

    return  returnStatus;

}
/**********************************************************************

    caller:     owner of this object

    prototype:

		ULONG
		SlapDslhParamtoGetParamInfoShort
			(
				ANSC_HANDLE					hThisObject,
				char*						pParamName,
				SLAP_STRING_ARRAY**			pNameArray,
				SLAP_STRING_ARRAY**			pTypeArray,
				SLAP_STRING_ARRAY**			pValueArray
			);

    description:

        This function is called to get next-levely-only objects (and their 
		types and values if it is a leaf node) of a given parent. The parent 
		name portion is stripped off before the names are returned. 

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

				char*						pParamName,
				The parent parameter name. If it's empty, it should be the root;

				SLAP_STRING_ARRAY**			pNameArray,
				The output parameter name array;

				SLAP_STRING_ARRAY**			pTypeArray,
                The output parameter type array;

				SLAP_STRING_ARRAY**			pValueArray
				The output Value Array;

    return:     status of operation.

**********************************************************************/

ULONG
SlapDslhParamtoGetParamInfoShort
	(
		ANSC_HANDLE					hThisObject,
	  	char*						pSubsystemPrefix,
		char*						pParamName,
		SLAP_STRING_ARRAY**			pNameArray,
        SLAP_STRING_ARRAY**			pTypeArray,
		SLAP_STRING_ARRAY**			pValueArray
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_FAILURE;
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	int ret   = 0;
    int size  = 0;
	int size2 = 0;
	int size3 = 0;
    parameterValStruct_t **ppParamVal = NULL;
    parameterInfoStruct_t **ppParamInfos = NULL;
    componentStruct_t ** ppComponents = NULL;
	char * parameterNames[1024];

	char	pValueBuf[1024]  = { '\0' };
	int j	= 0;
	int i	= 0;
	int uParentLength = 0;
	int valueLen = 0;
	int leaf_cnt = 0;
	int prior_sz = 0;

	SLAP_STRING_ARRAY*			pNameArrayCopyFrom = NULL;
    SLAP_STRING_ARRAY*			pTypeArrayCopyFrom = NULL;
	SLAP_STRING_ARRAY*			pValueArrayCopyFrom = NULL;
	SLAP_STRING_ARRAY*			pNameArrayCopyTo = NULL;
    SLAP_STRING_ARRAY*			pTypeArrayCopyTo = NULL;
	SLAP_STRING_ARRAY*			pValueArrayCopyTo = NULL;

    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

	if( pParamName == NULL || strcmp(pParamName, "") == 0 )
	{
		pParamName = AnscCloneString("Device.");
	}

	uParentLength = AnscSizeOfString(pParamName);

	ret = CcspBaseIf_discComponentSupportingNamespace 
            (
                MsgBusHandle,
                cr_id,
                pParamName,
                pSubsystemPrefix,
                &ppComponents,
                &size
            );

    if ( CCSP_SUCCESS != ret || 1 > size) 
    {
		free_componentStruct_t(MsgBusHandle, size, ppComponents); 
		if (CCSP_CR_ERR_UNSUPPORTED_NAMESPACE == ret ||  CCSP_CR_ERR_INVALID_PARAM	== ret )
		return 1;
		else
		return  returnStatus;
    } 

	for (j = 0; j < size; j++)
	{
		ret = CcspBaseIf_getParameterNames
			(
				MsgBusHandle,
				ppComponents[j]->componentName,
				ppComponents[j]->dbusPath,
				pParamName,
				TRUE,
				&size2,
				&ppParamInfos
			);

		if ( CCSP_SUCCESS != ret ) 
		{
			free_componentStruct_t(MsgBusHandle, size, ppComponents);
			free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);    
			if (pNameArrayCopyFrom != NULL)
			{
				SlapFreeVarArray(pNameArrayCopyFrom);
				SlapFreeVarArray(pTypeArrayCopyFrom);
				SlapFreeVarArray(pValueArrayCopyFrom);
			}
			return  returnStatus;
		}

		if ( size2 < 1)
		{
			free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);    
			continue;
		}

		leaf_cnt = 0;

		for ( i = 0; i < size2; i++ )//only pass leaf-node names to CcspBaseIf_getParameterValues()
		{
			if( TYPE_DSLH_PARAMETER_NAME == GetDslhNameType(ppParamInfos[i]->parameterName)) 
			{
				parameterNames[leaf_cnt++] = ppParamInfos[i]->parameterName;
			}
		}
		
		if ( 0 != leaf_cnt )
			{
			ret = CcspBaseIf_getParameterValues(
						MsgBusHandle,
						ppComponents[j]->componentName,
						ppComponents[j]->dbusPath,
						parameterNames,
						leaf_cnt,
						&size3,
						&ppParamVal
					);

			if ( CCSP_SUCCESS != ret || size3 != leaf_cnt )
			{
				free_componentStruct_t(MsgBusHandle, size, ppComponents);
				free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);  
				free_parameterValStruct_t(MsgBusHandle, size3, ppParamVal);
				if (pNameArrayCopyFrom != NULL)
				{
					SlapFreeVarArray(pNameArrayCopyFrom);
					SlapFreeVarArray(pTypeArrayCopyFrom);
					SlapFreeVarArray(pValueArrayCopyFrom);
				}
				return  returnStatus;
			}
		}
		else
		{	//mark empty ppParamVal
			size3 = 0; 
		}
		
		leaf_cnt = 0;

		if ( pNameArrayCopyFrom == NULL)
		{
			prior_sz = 0;
		}
		else
		{
			prior_sz = pNameArrayCopyFrom->VarCount;
		}

		SlapAllocStringArray2((size2+prior_sz), pNameArrayCopyTo);
		SlapAllocStringArray2((size2+prior_sz), pTypeArrayCopyTo);
		SlapAllocStringArray2((size2+prior_sz), pValueArrayCopyTo);

		for ( i=0; i<prior_sz; i++)
		{
			pNameArrayCopyTo->Array.arrayString[i] = AnscCloneString(pNameArrayCopyFrom->Array.arrayString[i]);
			pTypeArrayCopyTo->Array.arrayString[i] = AnscCloneString(pTypeArrayCopyFrom->Array.arrayString[i]);
			pValueArrayCopyTo->Array.arrayString[i] = AnscCloneString(pValueArrayCopyFrom->Array.arrayString[i]);
		}

		if (pNameArrayCopyFrom != NULL)
		{
			SlapFreeVarArray(pNameArrayCopyFrom);
			SlapFreeVarArray(pTypeArrayCopyFrom);
			SlapFreeVarArray(pValueArrayCopyFrom);
		}

		for ( i = 0; i < size2; i++ )
		{
			// clone parameter name without the  parent-prefix portion  
			pNameArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString(ppParamInfos[i]->parameterName + uParentLength);

			if( 0 < size3 && leaf_cnt < size3 && strcmp(ppParamInfos[i]->parameterName, ppParamVal[leaf_cnt]->parameterName) == 0 )
			{
				switch( (ppParamVal[leaf_cnt]->type))
				{
					case ccsp_string: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("string");
						break;

					case ccsp_int:
					case ccsp_long:
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("int");
						break;

					case ccsp_unsignedInt:
					case ccsp_unsignedLong:
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("unsignedInt");
						break;

					case ccsp_boolean:
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("boolean");
						break;

					case ccsp_dateTime: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("dateTime");
						break;

					case ccsp_base64: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("base64");
						break;

					case ccsp_float: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("float");
						break;

					case ccsp_double: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("double");
						break;

					case ccsp_byte: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("byte");
						break;

					case ccsp_none: 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("object");
						break;

					default:	//"unknown" 
						pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("unknown");
						break;
				}

				if( ppParamVal[leaf_cnt]->parameterValue == NULL)
				{
					pValueArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("(EMPTY)");
				}
				else
				{
					valueLen = AnscSizeOfString(ppParamVal[leaf_cnt]->parameterValue);

					if( valueLen == 0)
					{
						pValueArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("(EMPTY)");
					}
					else if( valueLen <= MAXI_VALUE_LENGTH_IN_TABLE)
					{
						pValueArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString(ppParamVal[leaf_cnt]->parameterValue);
					}
					else
					{
						AnscZeroMemory(pValueBuf, MAXI_VALUE_LENGTH_IN_TABLE + 8);

						AnscCopyMemory(pValueBuf, ppParamVal[leaf_cnt]->parameterValue, MAXI_VALUE_LENGTH_IN_TABLE);
						pValueBuf[MAXI_VALUE_LENGTH_IN_TABLE] = '.';
						pValueBuf[MAXI_VALUE_LENGTH_IN_TABLE + 1] = '.';
						pValueBuf[MAXI_VALUE_LENGTH_IN_TABLE + 2] = '.';

						pValueArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString(pValueBuf);
					}
				}

				leaf_cnt++;
			}
			else
			{
				if (TYPE_DSLH_ENTRY_NAME == GetDslhNameType(ppParamInfos[i]->parameterName))
				{
					pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("entry");
				}
				else
				{
					pTypeArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("object");
				}

				pValueArrayCopyTo->Array.arrayString[prior_sz+i]  = AnscCloneString("");
			}
		}

		free_parameterValStruct_t(MsgBusHandle, size3, ppParamVal);
		free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);  

		pNameArrayCopyFrom = pNameArrayCopyTo;
		pTypeArrayCopyFrom = pTypeArrayCopyTo;
		pValueArrayCopyFrom = pValueArrayCopyTo;

		pNameArrayCopyTo = NULL;
		pTypeArrayCopyTo = NULL;
		pValueArrayCopyTo = NULL;
	}

	free_componentStruct_t(MsgBusHandle, size, ppComponents); 

	*pNameArray = pNameArrayCopyFrom;
	*pTypeArray = pTypeArrayCopyFrom;
	*pValueArray = pValueArrayCopyFrom;

	returnStatus = ANSC_STATUS_SUCCESS;
    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

		ULONG
		SlapDslhParamtoGetHiddenInfo
			(
				ANSC_HANDLE					hThisObject,
				SLAP_STRING_ARRAY**			pNameArray,
				SLAP_STRING_ARRAY**			pValueArray
			);

    description:

        This function is called to get all the hidden objects and parameters

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

				SLAP_STRING_ARRAY**			pNameArray,
				The output parameter name array;

				SLAP_STRING_ARRAY**			pValueArray
				The output Value Array;

    return:     status of operation.

**********************************************************************/


ULONG
SlapDslhParamtoGetHiddenInfo
	(
		ANSC_HANDLE					hThisObject,
	  	char*				pSubsystemPrefix,
		SLAP_STRING_ARRAY**			pNameArray,
		SLAP_STRING_ARRAY**			pValueArray
    )
{
    UNREFERENCED_PARAMETER(pNameArray);
    UNREFERENCED_PARAMETER(pSubsystemPrefix);
    UNREFERENCED_PARAMETER(pValueArray);
    UNREFERENCED_PARAMETER(hThisObject);
	/* To be implemented in the future.
	   No d-bus API available to support 
	   this feature as of 10/07/2011*/


    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

		ULONG
		SlapDslhParamtoGetObjectInfo
			(
				ANSC_HANDLE					hThisObject,
				SLAP_STRING_ARRAY**			pAddableArray,
				SLAP_STRING_ARRAY**			pDeletableAttrArray
			);

    description:

        This function is called to get all the object information
		( Addable objects and Deletable instances)

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

				SLAP_STRING_ARRAY**			pAddableArray,
				The output Addable object name array;

				SLAP_STRING_ARRAY**			pDeletableArray,
				The output Deletable object name array;

    return:     status

**********************************************************************/

ULONG
SlapDslhParamtoGetObjectInfo
	(
		ANSC_HANDLE					hThisObject,
	  	char*				pSubsystemPrefix,
		SLAP_STRING_ARRAY**			pAddableArray,
		SLAP_STRING_ARRAY**			pDeletableArray
	)
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_FAILURE;
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	int ret   = 0;
    int size  = 0;
	int size2 = 0;
    parameterInfoStruct_t **ppParamInfos = NULL;
    componentStruct_t ** ppComponents = NULL;
	int i	= 0;
	int j   = 0;
	int add_count = 0;
	int del_count = 0;
	int prior_add_sz = 0;
	int prior_del_sz = 0;
	int nameType = 0;
	char  * pParamName = "Device.";
	SLAP_STRING_ARRAY*			pAddableArrayCopyFrom = NULL;
	SLAP_STRING_ARRAY*			pAddableArrayCopyTo = NULL;
	SLAP_STRING_ARRAY*			pDeletableArrayCopyFrom = NULL;
	SLAP_STRING_ARRAY*			pDeletableArrayCopyTo = NULL;
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

	ret = CcspBaseIf_discComponentSupportingNamespace 
            (
                MsgBusHandle,
                cr_id,
                pParamName,
                pSubsystemPrefix,
                &ppComponents,
                &size
            );

    if ( CCSP_SUCCESS != ret || 1 > size) 
    {
		free_componentStruct_t(MsgBusHandle, size, ppComponents); 
		if (CCSP_CR_ERR_UNSUPPORTED_NAMESPACE == ret ||  CCSP_CR_ERR_INVALID_PARAM	== ret )
			return 1;
		else
        return  returnStatus;
    }

	for (j = 0; j < size; j++)
	{
		ret = CcspBaseIf_getParameterNames
			(
				MsgBusHandle,
				ppComponents[j]->componentName,
				ppComponents[j]->dbusPath,
				pParamName,
				FALSE,
				&size2,
				&ppParamInfos
			);

		if ( CCSP_SUCCESS != ret) 
		{
			free_componentStruct_t(MsgBusHandle, size, ppComponents);
			free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);    
			if (pDeletableArrayCopyFrom != NULL)
			{
				SlapFreeVarArray(pDeletableArrayCopyFrom);
			}
			if (pAddableArrayCopyFrom != NULL)
			{
				SlapFreeVarArray(pAddableArrayCopyFrom);
			}
			return  returnStatus;
		}

		add_count = 0;
		del_count = 0;
		for( i = 0; i < size2; i ++)
		{
			if( ppParamInfos[i]->writable)
			{
				nameType = GetDslhNameType(ppParamInfos[i]->parameterName);

				if( TYPE_DSLH_PARAMETER_NAME == nameType) //writable prameter
				{
					continue;
				}
				else if( TYPE_DSLH_ENTRY_NAME == nameType)//writable(deletable) entry
				{
					del_count ++;
				}
				else  //writable(addable) object
				{
					add_count ++;
				}
			}
		}

		if ( pAddableArrayCopyFrom == NULL)
		{
			prior_add_sz = 0;
		}
		else
		{
			prior_add_sz = pAddableArrayCopyFrom->VarCount;
		}

		SlapAllocStringArray2((add_count+prior_add_sz), pAddableArrayCopyTo);

		for ( i=0; i<prior_add_sz; i++)
		{
			pAddableArrayCopyTo->Array.arrayString[i] = AnscCloneString(pAddableArrayCopyFrom->Array.arrayString[i]);
		}
		
		if (pAddableArrayCopyFrom != NULL)
		{
			SlapFreeVarArray(pAddableArrayCopyFrom);
		}
		
		if ( pDeletableArrayCopyFrom == NULL)
		{
			prior_del_sz = 0;
		}
		else
		{
			prior_del_sz = pDeletableArrayCopyFrom->VarCount;
		}

		SlapAllocStringArray2((del_count+prior_del_sz), pDeletableArrayCopyTo);

		for ( i=0; i<prior_del_sz; i++)
		{
			pDeletableArrayCopyTo->Array.arrayString[i] = AnscCloneString(pDeletableArrayCopyFrom->Array.arrayString[i]);
		}

		if (pDeletableArrayCopyFrom != NULL)
		{
			SlapFreeVarArray(pDeletableArrayCopyFrom);
		}

		add_count = 0;
		del_count = 0;
		for( i = 0; i < size2; i ++)
		{
			if( ppParamInfos[i]->writable)
			{
				nameType = GetDslhNameType(ppParamInfos[i]->parameterName);

				if( TYPE_DSLH_PARAMETER_NAME == nameType) //writable prameter
				{
					continue;
				}
				else if( TYPE_DSLH_ENTRY_NAME == nameType)//writable(deletable) entry
				{
					pDeletableArrayCopyTo->Array.arrayString[del_count+prior_del_sz]  = AnscCloneString(ppParamInfos[i]->parameterName);
					del_count ++;
				}
				else  //writable(addable) object
				{
					pAddableArrayCopyTo->Array.arrayString[add_count+prior_add_sz]  = AnscCloneString(ppParamInfos[i]->parameterName);
					add_count ++;
				}
			}
		}

		free_parameterInfoStruct_t (MsgBusHandle, size2, ppParamInfos);   

		pDeletableArrayCopyFrom = pDeletableArrayCopyTo;
		pAddableArrayCopyFrom = pAddableArrayCopyTo;

		pDeletableArrayCopyTo = NULL;
		pAddableArrayCopyTo = NULL;

	}

	free_componentStruct_t(MsgBusHandle, size, ppComponents); 

	*pAddableArray	= pAddableArrayCopyFrom;
	*pDeletableArray = pDeletableArrayCopyFrom;

	returnStatus = ANSC_STATUS_SUCCESS;

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapDslhParamtoAddObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName
            );

    description:

        This function is called to add a new instance under an object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                Specifies the object under which a new object is to be
                added.

    return:     status of operation.

**********************************************************************/

ULONG
SlapDslhParamtoAddObject
    (
        ANSC_HANDLE                 hThisObject,
	  	char*				pSubsystemPrefix,
		int							sessionID,
        char*                       pObjName
    )
{
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
    componentStruct_t ** ppComponents = NULL;
    int ret   = 0;
    int size  = 0;
	char * dst_componentid =  NULL;
	char * dst_dbus_path    =  NULL;
    unsigned int Nums	=	0;
    unsigned int *NumArray	= NULL;
	int InstanceNumber = 0;
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

	if (TYPE_DSLH_OBJECT_NAME != GetDslhNameType(pObjName))
	{
		return ANSC_STATUS_FAILURE;  //parameter or instance name 
	}
    
	ret = CcspBaseIf_discComponentSupportingNamespace 
        (
            MsgBusHandle,
            cr_id,
            pObjName,
            pSubsystemPrefix,
            &ppComponents,
            &size
        );

    if ( CCSP_SUCCESS != ret || 1 != size)  // there can be only one owner component of a leaf namespace in a subsystem 
    {
		free_componentStruct_t(MsgBusHandle, size, ppComponents); 
        return  ANSC_STATUS_FAILURE;
    }

    dst_componentid = AnscCloneString(ppComponents[0]->componentName);
    dst_dbus_path    = AnscCloneString(ppComponents[0]->dbusPath);
	free_componentStruct_t(MsgBusHandle, size, ppComponents);

	ret = CcspBaseIf_GetNextLevelInstances
		(
            MsgBusHandle,
            dst_componentid,
            dst_dbus_path,
			pObjName,
			&Nums,
			&NumArray
		);

	if ( CCSP_SUCCESS != ret ) 
	{
		if ( NumArray != NULL )
		{
		   AnscFreeMemory(NumArray);
		}
		return  ANSC_STATUS_FAILURE;
	}

	InstanceNumber = NumArray[Nums - 1] + 1;
	AnscFreeMemory(NumArray);

	ret = CcspBaseIf_AddTblRow(
            MsgBusHandle,
            dst_componentid,
            dst_dbus_path,
			sessionID,
			pObjName,
			&InstanceNumber
		);

    if(ret != CCSP_SUCCESS )
    {
	    return  ANSC_STATUS_FAILURE;
    }
	
    return  InstanceNumber;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapDslhParamtoDelObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName
            );

    description:

        This function is called to delet a object instance.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName
                Specifies the object under which a new object is to be
                added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapDslhParamtoDelObject
    (
        ANSC_HANDLE                 hThisObject,
	  	char*				pSubsystemPrefix,
		int							sessionID,
        char*                       pObjName
    )
{
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
    componentStruct_t ** ppComponents = NULL;
    int ret   = 0;
    int size  = 0;
	char * dst_componentid =  NULL;
	char * dst_dbus_path    =  NULL;
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

	if (TYPE_DSLH_ENTRY_NAME != GetDslhNameType(pObjName))
	{
		return ANSC_STATUS_BAD_NAME;  //parameter or instance name 
	}
    
	ret = CcspBaseIf_discComponentSupportingNamespace 
        (
            MsgBusHandle,
            cr_id,
            pObjName,
            pSubsystemPrefix,
            &ppComponents,
            &size
        );

    if ( CCSP_SUCCESS != ret || 1 != size)  // there can be only one owner component of a leaf namespace in a subsystem 
    {
		free_componentStruct_t(MsgBusHandle, size, ppComponents); 
		if (CCSP_CR_ERR_UNSUPPORTED_NAMESPACE == ret ||  CCSP_CR_ERR_INVALID_PARAM	== ret )
			return ANSC_STATUS_BAD_NAME;
		else
        return  ANSC_STATUS_FAILURE;
    }

    dst_componentid = AnscCloneString(ppComponents[0]->componentName);
    dst_dbus_path    = AnscCloneString(ppComponents[0]->dbusPath);
	free_componentStruct_t(MsgBusHandle, size, ppComponents);

	ret = CcspBaseIf_DeleteTblRow(
            MsgBusHandle,
            dst_componentid,
            dst_dbus_path,
			sessionID,
			pObjName
		);

    if(ret != CCSP_SUCCESS )
    {
	    return  ANSC_STATUS_FAILURE;
    }
	
    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapDslhParamtoAcqWriteAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire enter an active session for
		exclusively accsssing the parameter tree. 

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.
				int							priority
				component priority
    return:     Acquired SessionID or 0 if failed

**********************************************************************/

INT
SlapDslhParamtoAcqWriteAccess
    (
        ANSC_HANDLE                 hThisObject,
	  	char*				pSubsystemPrefix,
		int							priority
    )
{
    UNREFERENCED_PARAMETER(pSubsystemPrefix);
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	int ret = 0;
	int sessionID = 0;
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

	ret = CcspBaseIf_requestSessionID
        (
            MsgBusHandle,
            cr_id,
            priority,
            &sessionID
        );

    if ( CCSP_SUCCESS != ret)  
    {
		return SESSION_ACQ_FAIL;
    }

    return  sessionID;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapDslhParamtoRelWriteAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to end  caller's active session of 
		exclusively accessing the parameter tree.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.
				int							sessionID
				Session to be terminated

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapDslhParamtoRelWriteAccess
    (
        ANSC_HANDLE                 hThisObject,
	  	char*						pSubsystemPrefix,
		int							sessionID
    )
{
    UNREFERENCED_PARAMETER(pSubsystemPrefix);
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_FAILURE;
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
	int ret = 0;
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

	ret = CcspBaseIf_informEndOfSession
        (
            MsgBusHandle,
            cr_id,
            sessionID
        );

    if ( CCSP_SUCCESS == ret)  
    {
		returnStatus = ANSC_STATUS_SUCCESS;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapDslhParamtoIsParamTreeReadOnly
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to check whether the parameter tree is
        read-only (clocked by an active session)

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SlapDslhParamtoIsParamTreeReadOnly
    (
        ANSC_HANDLE                 hThisObject,
	 	char*				pSubsystemPrefix
    )
{
    UNREFERENCED_PARAMETER(pSubsystemPrefix);
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;

	int sessionID = 0;
	int priority = 0;
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

    /*CID: 52904 Arguments in wrong order*/
    CcspBaseIf_getCurrentSessionID
        (
            MsgBusHandle,
            cr_id,
	    &priority,
            &sessionID
        );

	if ( SESSIOIN_ZERO != sessionID) //There is an active session
		return TRUE;
	else
		return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapDslhParamtoIsParameterReadOnly
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pParamName
            );

    description:

        This function is called to check whether the specified
        parameter is read-only.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pParamName
                Specifies the parameter to be checked.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SlapDslhParamtoIsParameterReadOnly
    (
        ANSC_HANDLE                 hThisObject,
		char*				pSubsystemPrefix,
        char*                       pParamName
    )
{
    PSLAP_DSLH_PARAMTREE_OBJECT     pMyObject        = (PSLAP_DSLH_PARAMTREE_OBJECT)hThisObject;
    ANSC_HANDLE                     MsgBusHandle     = (ANSC_HANDLE                )pMyObject->hInsContext;
    BOOL                            bReadOnly        = FALSE;
    componentStruct_t ** ppComponents = NULL;
	parameterInfoStruct_t **ppParamInfos = NULL;
    int ret   = 0;
    int size  = 0;
	char * dst_componentid =  NULL;
	char * dst_dbus_path    =  NULL;
    char                            cr_id[256];
    errno_t rc = -1;

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", g_Subsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }


	if (TYPE_DSLH_PARAMETER_NAME != GetDslhNameType(pParamName))
	{
		return bReadOnly;  //object or instance name 
	}

	ret = CcspBaseIf_discComponentSupportingNamespace 
        (
            MsgBusHandle,
            cr_id,
            pParamName,
            pSubsystemPrefix,
            &ppComponents,
            &size
        );

    if ( CCSP_SUCCESS != ret || 1 != size)  // there can be only one owner component of a leaf namespace in a subsystem 
    {
		free_componentStruct_t(MsgBusHandle, size, ppComponents); 
		bReadOnly = TRUE;   //what do I return when param name is not found?
		return bReadOnly;  
    }

    dst_componentid = AnscCloneString(ppComponents[0]->componentName);
    dst_dbus_path    = AnscCloneString(ppComponents[0]->dbusPath);
	free_componentStruct_t(MsgBusHandle, size, ppComponents);
   
    ret = CcspBaseIf_getParameterNames(
        MsgBusHandle,
        dst_componentid,
        dst_dbus_path,
        pParamName,
        0,
        &size,
        &ppParamInfos
    );

	if ( CCSP_SUCCESS != ret || size != 1 ) 
	{
		free_parameterInfoStruct_t (MsgBusHandle, size, ppParamInfos);    
		return bReadOnly;  
	}

	if ( TRUE == ppParamInfos[0]->writable)  
 	{
		free_parameterInfoStruct_t (MsgBusHandle, size, ppParamInfos);    
		return  bReadOnly;
	}

	free_parameterInfoStruct_t (MsgBusHandle, size, ppParamInfos);    

	bReadOnly =  TRUE;
    return  bReadOnly;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_STRING
        SlapDslhParamtoEscapeXmlString
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_STRING                 pInputString
            );

    description:

        This function is called to escape XML characters below:

            '<'  ==> "&lt;"
            '>'  ==> "&gt;"
            '&'  ==> "&amp;"
            '"'  ==> "&quot;"
            '\'' ==> "&apos;"


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_STRING                 pInputString
                the input string

    return:     the output string with xml escaped chars

**********************************************************************/

#define  MAX_LEN_PREDEFINED_ENTITIES                10
typedef  struct
_ALXML_PREDEFINED_ENTITIES
{
    UCHAR                           uCharacter;
    UCHAR                           Encode[MAX_LEN_PREDEFINED_ENTITIES];
}
ALXML_PREDEFINED_ENTITIES;

static ULONG Predefined_Count = 5;
static char  Xml_Chars[]      = "<>&'\"";

static const 
ALXML_PREDEFINED_ENTITIES
Predefined_XML_Entities[] =
{
    {'&',    "&amp;"},
    {'<',    "&lt;"},
    {'>',    "&gt;"},
    {'\'',    "&apos;"},
    {'"',    "&quot;"}
};

SLAP_STRING
SlapDslhParamtoEscapeXmlString
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_STRING                 pInputString
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                 length     = 0;
    ULONG                 charCount  = 0;
    PCHAR                 pOutString = NULL;
    ULONG                 pos        = 0;
    ULONG                 i          = 0;
    ULONG                 j          = 0;
    char                  c          = '0';
    errno_t               rc         = -1;

    if( pInputString == NULL)
    {
        return NULL;
    }

    length = AnscSizeOfString(pInputString);

    if( length == 0)
    {
        return NULL;
    }

    /* check how many such chars */
    for( i = 0; i < length; i ++)
    {
        c = pInputString[i];

        if( _ansc_strchr(Xml_Chars, c) != NULL)
        {
            charCount ++;
        }
    }

    if( charCount == 0)
    {
        return AnscCloneString(pInputString);
    }

    /* allocate the new memeory with enough spaces */
    pOutString = (PCHAR)AnscAllocateMemory( length + charCount * 4 + 16);

    if( pOutString == NULL)
    {
        return NULL;
    }

    for( i = 0 ; i < length; i ++)
    {
        c = pInputString[i];

        for ( j = 0; j < Predefined_Count; j ++)
        {
            if( c == Predefined_XML_Entities[j].uCharacter)
            {
                break;
            }
        }

        if( j == Predefined_Count)
        {
            pOutString[pos++] = c;
        }
        else
        {
            rc = strcpy_s
                (
                    (PCHAR)(pOutString + pos),
                    (length + charCount * 4 + 16 - pos),
                    (PCHAR)Predefined_XML_Entities[j].Encode
                );
            if(rc != EOK){
                ERR_CHK(rc);
            }
            pos += AnscSizeOfString((const char *)Predefined_XML_Entities[j].Encode);
        }
    }

    return pOutString;
}


