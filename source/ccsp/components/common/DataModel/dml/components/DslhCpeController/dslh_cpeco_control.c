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

    module: dslh_cpeco_control.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced control functions
        of the Dslh Cpe Controller Object.

        *   DslhCpecoInitDataModel
        *   DslhCpecoRegisterAllParams
        *   DslhCpecoRegisterObject
        *   DslhCpecoRegisterObject2
        *   DslhCpecoLoadExternalDMLibFile
        *   DslhCpecoLoadInternalDMLibFile
        *   DslhCpecoUnloadDMLibFile
        *   DslhCpecoRegisterHiddenObject
        *   DslhCpecoRegisterHiddenObject2
        *   DslhCpecoRegisterDataModel
        *   DslhCpecoRegisterDataModel2

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin     Zhu
        Kang    Quan

    ---------------------------------------------------------------

    revision:

        09/09/05    initial revision.
        03/03/09    Bin add "RegisterHiddenObject"
        11/01/10    Bin added DslhDataModelAgent support
        01/05/11    Bin moved "ValidateHierarchyInterface" from ObjController object;
        
**********************************************************************/

#include "dslh_cpeco_global.h"
#include "ccsp_base_api.h"
#include "safec_lib_common.h"

#define  CCSP_PARAMETER_MAX_COUNT                       2000
#define  CCSP_DUMMY_PARAM_NAME							"CcspDummy"
#define  CCSP_MAX_PARAMETER_PER_REGISTRATION            50
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoInitDataModel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to intialize management model.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhCpecoInitDataModel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject          = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase   = (PDSLH_WMP_DATABASE_OBJECT    )pMyObject->hDslhWmpDatabase;

    /*
     * Parameter names use a hierarchical form similar to a directory tree. The name of a parti-
     * cular Parameter is represented by the concatenation of each successive node in the hierarcy
     * separated with a "." (dot), starting at the trunk of the hierarchy and leading to the leaves.
     * When specifying a partial path, indicating an intermediate node in the hierarchy, the trail-
     * ing "." (dot) is always used as the last character.
     */
    pDslhWmpDatabase->CreateRecordTree ((ANSC_HANDLE)pDslhWmpDatabase);
    pDslhWmpDatabase->ConfigRecordTree ((ANSC_HANDLE)pDslhWmpDatabase);

    /*
     * If this is the first time we talk to this particular ACS server (identified by the
     * "InternetGateway.ManagementServer.URL" parameter), we MUST include the "0 BOOTSTRAP" event
     * in the Inform message. After the initial contact, we must reset a flag so following sessions
     * don't send "0 BOOTSTRAP" event. This is done by adding a new 'invisible' (i.e. invisible to
     * ACS) parameter under the same ManagementServer object:
     * "InternetGateway.ManagementServer.InitialContact" (boolean).
     */

/*
    if ( pProperty->bStartCwmpRightAway )
    {
        returnStatus = pMyObject->StartCWMP((ANSC_HANDLE)pMyObject);
    }
*/
    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoRegisterAllParams
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to register all parameters.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhCpecoRegisterAllParams
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoRegisterObject2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName,
                void*                       pfnObjcoConstructor,
                ANSC_HANDLE                 hParamArray
            );

    description:

        This function is called to register an object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName,
                The objName;

                void*                       pfnObjcoConstructor,
                The creation method;

                ANSC_HANDLE                 hParamArray
                Specifies the array of parameters to be registered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhCpecoRegisterObject2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        void*                       pfnObjcoConstructor,
        ANSC_HANDLE                 hParamArray
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject         = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE          )pMyObject->GetDslhMprIf((ANSC_HANDLE)pMyObject);
    PDSLH_CWMP_OBJECT_DESCR         pObjectDescr      = (PDSLH_CWMP_OBJECT_DESCR      )NULL;
    PDSLH_CWMP_PARAM_DESCR          pParamDescr       = (PDSLH_CWMP_PARAM_DESCR       )NULL;
    PANSC_HANDLE                    pParamDescrArray  = (PANSC_HANDLE)hParamArray;
    ULONG                           ulParamCount      = 0;

    pObjectDescr = (PDSLH_CWMP_OBJECT_DESCR)AnscAllocateMemory(sizeof(DSLH_CWMP_OBJECT_DESCR));

    if ( !pObjectDescr )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pObjectDescr->Name                  = AnscCloneString(pObjName);
        pObjectDescr->Type                  = DSLH_CWMP_OBJECT_TYPE_regular;
        pObjectDescr->bWritable             = FALSE;
        pObjectDescr->bInvisible            = FALSE;
        pObjectDescr->bDynamic              = FALSE;
        pObjectDescr->bGroupCommit          = TRUE;
        pObjectDescr->MaxInstanceNumber     = 1;
        pObjectDescr->pfnObjcoConstructor   = pfnObjcoConstructor;
        pObjectDescr->hParamArray           = hParamArray;
        pObjectDescr->hDslhTr69If           = NULL;

        returnStatus =
            pDslhMprIf->RegObject
                (
                    pDslhMprIf->hOwnerContext,
                    (ANSC_HANDLE)pObjectDescr
                );

       if( returnStatus != ANSC_STATUS_SUCCESS)
       {
           DslhCwmpFreeObjectDescr(pObjectDescr);

           return returnStatus;
       }

        if( pMyObject->uObjCount < CPECONTROLLER_MAX_REG_OBJECT_COUNT)
        {
            pMyObject->hObjDespArray[pMyObject->uObjCount ++] = pObjectDescr;
        }

    }

    if ( !pParamDescrArray )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pParamDescr       = (PDSLH_CWMP_PARAM_DESCR)pParamDescrArray[ulParamCount];

    while (pParamDescr != NULL && pParamDescr->Name != NULL )
    {
        returnStatus =
            pDslhMprIf->RegParameter
                (
                    pDslhMprIf->hOwnerContext,
                    (ANSC_HANDLE)pParamDescr,
                    pObjectDescr->Name
                );

        ulParamCount++;
        pParamDescr       = (PDSLH_CWMP_PARAM_DESCR)pParamDescrArray[ulParamCount];
    }

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoRegisterHiddenObject2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pObjName,
                void*                       pfnObjcoConstructor,
                ANSC_HANDLE                 hParamArray
            );

    description:

        This function is called to register a hidden object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pObjName,
                The objName;

                void*                       pfnObjcoConstructor,
                The creation method;

                ANSC_HANDLE                 hParamArray
                Specifies the array of parameters to be registered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhCpecoRegisterHiddenObject2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName,
        void*                       pfnObjcoConstructor,
        ANSC_HANDLE                 hParamArray
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject         = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE          )pMyObject->GetDslhMprIf((ANSC_HANDLE)pMyObject);
    PDSLH_CWMP_OBJECT_DESCR         pObjectDescr      = (PDSLH_CWMP_OBJECT_DESCR      )NULL;
    PDSLH_CWMP_PARAM_DESCR          pParamDescr       = (PDSLH_CWMP_PARAM_DESCR       )NULL;
    PANSC_HANDLE                    pParamDescrArray  = (PANSC_HANDLE)hParamArray;
    ULONG                           ulParamCount      = 0;

    pObjectDescr = (PDSLH_CWMP_OBJECT_DESCR)AnscAllocateMemory(sizeof(DSLH_CWMP_OBJECT_DESCR));

    if ( !pObjectDescr )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pObjectDescr->Name                  = AnscCloneString(pObjName);
        pObjectDescr->Type                  = DSLH_CWMP_OBJECT_TYPE_regular;
        pObjectDescr->bWritable             = FALSE;
        pObjectDescr->bInvisible            = TRUE;    /* set it as invisible */
        pObjectDescr->bDynamic              = FALSE;
        pObjectDescr->bGroupCommit          = TRUE;
        pObjectDescr->MaxInstanceNumber     = 1;
        pObjectDescr->pfnObjcoConstructor   = pfnObjcoConstructor;
        pObjectDescr->hParamArray           = hParamArray;

        returnStatus =
            pDslhMprIf->RegObject
                (
                    pDslhMprIf->hOwnerContext,
                    (ANSC_HANDLE)pObjectDescr
                );

       if( returnStatus != ANSC_STATUS_SUCCESS)
       {
           DslhCwmpFreeObjectDescr(pObjectDescr);

           return returnStatus;
       }

        if( pMyObject->uObjCount < CPECONTROLLER_MAX_REG_OBJECT_COUNT)
        {
            pMyObject->hObjDespArray[pMyObject->uObjCount ++] = pObjectDescr;
        }
    }

    if ( !pParamDescrArray )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pParamDescr       = (PDSLH_CWMP_PARAM_DESCR)pParamDescrArray[ulParamCount];

    while (pParamDescr != NULL && pParamDescr->Name != NULL )
    {
        returnStatus =
            pDslhMprIf->RegParameter
                (
                    pDslhMprIf->hOwnerContext,
                    (ANSC_HANDLE)pParamDescr,
                    pObjectDescr->Name
                );

        ulParamCount++;
        pParamDescr       = (PDSLH_CWMP_PARAM_DESCR)pParamDescrArray[ulParamCount];
    }

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoRegisterObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjDescr,
                ANSC_HANDLE                 hParamArray
            );

    description:

        This function is called to register an object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjDescr
                Specifies the object descriptor to be registered.

                ANSC_HANDLE                 hParamArray
                Specifies the array of parameters to be registered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhCpecoRegisterObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr,
        ANSC_HANDLE                 hParamArray
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject         = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE          )pMyObject->GetDslhMprIf((ANSC_HANDLE)pMyObject);
    PDSLH_CWMP_OBJECT_DESCR         pObjectDescr      = (PDSLH_CWMP_OBJECT_DESCR      )hObjDescr;
    PANSC_HANDLE                    pParamDescrArray  = (PANSC_HANDLE                 )hParamArray;
    PDSLH_CWMP_PARAM_DESCR          pParamDescr       = (PDSLH_CWMP_PARAM_DESCR       )NULL;
    ULONG                           ulParamCount      = 0;
    name_spaceType_t *              pParameterArray   = pMyObject->hParameterArray;
    char *                          pParameterHolder  = pMyObject->hParameterHolder;
    
    if ( !pObjectDescr )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        pObjectDescr->hParamArray = hParamArray;

        returnStatus =
            pDslhMprIf->RegObject
                (
                    pDslhMprIf->hOwnerContext,
                    (ANSC_HANDLE)pObjectDescr
                );

        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            return returnStatus;
        }
    }

    if ( !pParamDescrArray )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pParamDescr       = (PDSLH_CWMP_PARAM_DESCR)pParamDescrArray[ulParamCount];

    while (pParamDescr != NULL && pParamDescr->Name != NULL )
    {
        returnStatus =
            pDslhMprIf->RegParameter
                (
                    pDslhMprIf->hOwnerContext,
                    (ANSC_HANDLE)pParamDescr,
                    pObjectDescr->Name
                );
        
        ulParamCount++;

        /* We need to collect all full name and type of all parameters. */
        if ( pMyObject->uParameterArrayIndex < pMyObject->uParameterCount )
        {
            if ( pParameterArray && pParameterHolder )
            {
                _ansc_sprintf( &pParameterHolder[pMyObject->uParameterIndex], "%s%s", pObjectDescr->Name, pParamDescr->Name );
                pParameterArray[pMyObject->uParameterArrayIndex].name_space   = &pParameterHolder[pMyObject->uParameterIndex];

                if ( !_ansc_strncmp( pParamDescr->DataType, DSLH_CWMP_DATA_NAME_string, _ansc_strlen(DSLH_CWMP_DATA_NAME_string) ) )
                    pParameterArray[pMyObject->uParameterArrayIndex++].dataType   = ccsp_string;
                else if ( !_ansc_strncmp( pParamDescr->DataType, DSLH_CWMP_DATA_NAME_int, _ansc_strlen(DSLH_CWMP_DATA_NAME_int) ))
                    pParameterArray[pMyObject->uParameterArrayIndex++].dataType   = ccsp_int;
                else if ( !_ansc_strncmp( pParamDescr->DataType, DSLH_CWMP_DATA_NAME_unsignedInt, _ansc_strlen(DSLH_CWMP_DATA_NAME_unsignedInt) ))
                    pParameterArray[pMyObject->uParameterArrayIndex++].dataType   = ccsp_unsignedInt;
                else if ( !_ansc_strncmp( pParamDescr->DataType, DSLH_CWMP_DATA_NAME_boolean, _ansc_strlen(DSLH_CWMP_DATA_NAME_boolean) ))
                    pParameterArray[pMyObject->uParameterArrayIndex++].dataType   = ccsp_boolean;
                else if ( !_ansc_strncmp( pParamDescr->DataType, DSLH_CWMP_DATA_NAME_dateTime, _ansc_strlen(DSLH_CWMP_DATA_NAME_dateTime) ))
                    pParameterArray[pMyObject->uParameterArrayIndex++].dataType   = ccsp_dateTime;
                else if ( !_ansc_strncmp( pParamDescr->DataType, DSLH_CWMP_DATA_NAME_base64, _ansc_strlen(DSLH_CWMP_DATA_NAME_base64) ))
                    pParameterArray[pMyObject->uParameterArrayIndex++].dataType   = ccsp_base64;
                else
                {
                    pParameterArray[pMyObject->uParameterArrayIndex++].dataType   = ccsp_byte;
                }

                pMyObject->uParameterIndex += _ansc_strlen( &pParameterHolder[pMyObject->uParameterIndex] ) + 1;

            }
        }
        else
        {
            AnscTraceWarning(("DslhCpecoRegisterObject -- There are too many parameters and increase the array size.\n"));
        }
        
        pParamDescr       = (PDSLH_CWMP_PARAM_DESCR)pParamDescrArray[ulParamCount];
    }

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoLoadExternalDMLibFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pLibXmlFile,
                BOOL                        bPopulateTree
            );

    description:

        This function is called to load Data Model library from XML file

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pLibXmlFile
                The XML File name;

                BOOL                        bPopulateTree
                Whether to repopulate the object or not;

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhCpecoLoadExternalDMLibFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pLibXmlFile,
        BOOL                        bPopulateTree
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject          = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)pMyObject->hDslhDataModelAgent;
    ANSC_HANDLE                     pFileHandle        = NULL;
    char*                           pXMLContent        = NULL;
    ULONG                           uFileLength        = 0;
    ULONG                           uBufferSize        = 0;

    if( pDslhDataModelAgent == NULL || pLibXmlFile == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    /* load from the file */
    pFileHandle =
        AnscOpenFile
        (
            pLibXmlFile,
            ANSC_FILE_O_BINARY | ANSC_FILE_O_RDONLY,
            ANSC_FILE_S_IREAD
        );

    if( pFileHandle == NULL)
    {
        AnscTraceWarning
            ((
                "DslhCpecoLoadExternalDMLibFile -- failed to open %s\n",
                pLibXmlFile
            ));

        return ANSC_STATUS_FAILURE;
    }

    uFileLength = AnscGetFileSize( pFileHandle);

    pXMLContent = (char*)AnscAllocateMemory( uFileLength + 8);

    if( pXMLContent == NULL)
    {
        AnscCloseFile( pFileHandle ); /*RDKB-5787 , CID-33331; free resource allocated*/
        return ANSC_STATUS_RESOURCES;
    }

    uBufferSize = uFileLength + 8;

    if( AnscReadFile( pFileHandle, pXMLContent, &uBufferSize) != ANSC_STATUS_SUCCESS)
    {
        AnscCloseFile( pFileHandle ); /*RDKB-5787 , CID-33331; free resource allocated*/
        AnscFreeMemory(pXMLContent);

        AnscTraceWarning
            ((
                "DslhCpecoLoadExternalDMLibFile -- failed to read file %s\n",
                pLibXmlFile
            ));

        return ANSC_STATUS_FAILURE;
    }

    if( pFileHandle != NULL)
    {
        AnscCloseFile(pFileHandle);
    }
    returnStatus = pDslhDataModelAgent->LoadDataModelXML(pDslhDataModelAgent, pXMLContent, uBufferSize,TRUE /* external */, bPopulateTree );
    AnscFreeMemory(pXMLContent);
    return returnStatus;
}

ANSC_STATUS
DslhCpecoLoadExternalDMLibFile2
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pfnXMLLoader,
        BOOL                        bPopulateTree
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject          = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)pMyObject->hDslhDataModelAgent;

    if( pDslhDataModelAgent == NULL || pfnXMLLoader == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }
    returnStatus = pDslhDataModelAgent->LoadDataModelXML2(pDslhDataModelAgent, pfnXMLLoader,TRUE /* external */, bPopulateTree);
    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoLoadInternalDMLibFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pLibXmlFile
            );

    description:

        This function is called to load internal Data Model library from XML file

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pLibXmlFile
                The XML File name;

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhCpecoLoadInternalDMLibFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pLibXmlFile
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject          = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)pMyObject->hDslhDataModelAgent;
    ANSC_HANDLE                     pFileHandle        = NULL;
    char*                           pXMLContent        = NULL;
    ULONG                           uFileLength        = 0;
    ULONG                           uBufferSize        = 0;

    if( pDslhDataModelAgent == NULL || pLibXmlFile == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    /* load from the file */
    pFileHandle =
        AnscOpenFile
        (
            pLibXmlFile,
            ANSC_FILE_O_BINARY | ANSC_FILE_O_RDONLY,
            ANSC_FILE_S_IREAD
        );

    if( pFileHandle == NULL)
    {
        AnscTrace("Failed to load the file : '%s'\n", pLibXmlFile);

        return ANSC_STATUS_FAILURE;
    }

    uFileLength = AnscGetFileSize( pFileHandle);

    pXMLContent = (char*)AnscAllocateMemory( uFileLength + 8);

    if( pXMLContent == NULL)
    {
        AnscCloseFile(pFileHandle); /*RDKB-5787 , CID-33066; free the resource allocated*/
        return ANSC_STATUS_RESOURCES;
    }

    uBufferSize = uFileLength + 8;

    if( AnscReadFile( pFileHandle, pXMLContent, &uBufferSize) != ANSC_STATUS_SUCCESS)
    {
        AnscCloseFile(pFileHandle); /*RDKB-5787 , CID-33066; free the resource allocated*/
        AnscFreeMemory(pXMLContent);

        return ANSC_STATUS_FAILURE;
    }

    AnscCloseFile(pFileHandle);

    returnStatus = pDslhDataModelAgent->LoadDataModelXML(pDslhDataModelAgent, pXMLContent, uBufferSize,FALSE /* internal */, FALSE /* don't populate */ );

    AnscFreeMemory(pXMLContent);


    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoUnloadDMLibFile
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pLibXmlFile
            );

    description:

        This function is called to unload a Data Model library from XML file

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pLibXmlFile
                The XML File name;

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhCpecoUnloadDMLibFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pLibXmlFile
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject          = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)pMyObject->hDslhDataModelAgent;
    ANSC_HANDLE                     pFileHandle        = NULL;
    char*                           pXMLContent        = NULL;
    ULONG                           uFileLength        = 0;
    ULONG                           uBufferSize        = 0;

    if( pDslhDataModelAgent == NULL || pLibXmlFile == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    /* load from the file */
    pFileHandle =
        AnscOpenFile
        (
            pLibXmlFile,
            ANSC_FILE_O_BINARY | ANSC_FILE_O_RDONLY,
            ANSC_FILE_S_IREAD
        );

    if( pFileHandle == NULL)
    {
        AnscTrace("Failed to load the file : '%s'\n", pLibXmlFile);

        return ANSC_STATUS_FAILURE;
    }

    uFileLength = AnscGetFileSize( pFileHandle);

    pXMLContent = (char*)AnscAllocateMemory( uFileLength + 8);

    if( pXMLContent == NULL)
    {
        AnscCloseFile(pFileHandle); /*RDKB-5787 , CID-32896; Free allocated resources*/
        return ANSC_STATUS_RESOURCES;
    }

    uBufferSize = uFileLength + 8;

    if( AnscReadFile( pFileHandle, pXMLContent, &uBufferSize) != ANSC_STATUS_SUCCESS)
    {
        AnscFreeMemory(pXMLContent);  /*RDKB-5787 , CID-32896; Free allocated resources*/
        AnscCloseFile(pFileHandle);

        return ANSC_STATUS_FAILURE;
    }

    AnscCloseFile(pFileHandle);


    returnStatus = pDslhDataModelAgent->UnloadDataModelXML(pDslhDataModelAgent, pXMLContent, uBufferSize);

    AnscFreeMemory(pXMLContent);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoRegisterHiddenObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjDescr,
                ANSC_HANDLE                 hParamArray
            );

    description:

        This function is called to register a hidden object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjDescr
                Specifies the object descriptor to be registered.

                ANSC_HANDLE                 hParamArray
                Specifies the array of parameters to be registered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhCpecoRegisterHiddenObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr,
        ANSC_HANDLE                 hParamArray
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject         = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE          )pMyObject->GetDslhMprIf((ANSC_HANDLE)pMyObject);
    PDSLH_CWMP_OBJECT_DESCR         pObjectDescr      = (PDSLH_CWMP_OBJECT_DESCR      )hObjDescr;
    PANSC_HANDLE                    pParamDescrArray  = (PANSC_HANDLE                 )hParamArray;
    PDSLH_CWMP_PARAM_DESCR          pParamDescr       = (PDSLH_CWMP_PARAM_DESCR       )NULL;
    ULONG                           ulParamCount      = 0;

    if ( !pObjectDescr )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        pObjectDescr->bInvisible  = TRUE;   /* force it to invisible */
        pObjectDescr->hParamArray = hParamArray;
        returnStatus =
            pDslhMprIf->RegObject
                (
                    pDslhMprIf->hOwnerContext,
                    (ANSC_HANDLE)pObjectDescr
                );

        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            return returnStatus;
        }
    }

    if ( !pParamDescrArray )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pParamDescr       = (PDSLH_CWMP_PARAM_DESCR)pParamDescrArray[ulParamCount];

    while (pParamDescr != NULL && pParamDescr->Name != NULL )
    {
        returnStatus =
            pDslhMprIf->RegParameter
                (
                    pDslhMprIf->hOwnerContext,
                    (ANSC_HANDLE)pParamDescr,
                    pObjectDescr->Name
                );

        ulParamCount++;
        pParamDescr       = (PDSLH_CWMP_PARAM_DESCR)pParamDescrArray[ulParamCount];
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoRegisterDataModel
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCR_id,
                char*                       pXMLFile,
                char*                       pCompName,
                ULONG                       version,
                char*                       pDbusPath,
                char*                       pPrefix
            );

    description:

        This function is called to load and register data model.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pCR_id,
                The CR component id;

                char*                       pXMLFile,
                The data model xml file. 
                It can be empty or "" if only CCSP base data model is supported.

                char*                       pCompName,
                The CCSP component name;

                ULONG                       version,
                The version of the component;


                char*                       pDbusPath,
                The component dbus path;

                char*                       pPrefix
                current subsystem prefix;

    return:     status of operation.

**********************************************************************/
void
dslhCpeEnumObjEntities
	(
		PDSLH_CPE_CONTROLLER_OBJECT	pMyObject,
		PDSLH_OBJ_ENTITY_OBJECT		pObjEntity
	)
{
    name_spaceType_t *              pParameterArray   = pMyObject->hParameterArray;
    char *                          pParameterHolder  = pMyObject->hParameterHolder;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = (PSINGLE_LINK_ENTRY       )NULL;
	BOOL							bHasChildParam      = TRUE;

    if ( pObjEntity->ObjDescr && (pObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table) )
    {
        pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjEntity->GetObjEntity3((ANSC_HANDLE)pObjEntity);

        /* enumerate the entries */
        dslhCpeEnumObjEntities(pMyObject, pChildObjEntity);

        return;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pObjEntity->VareoTable);
    if( !pSLinkEntry)
    {
		bHasChildParam = FALSE;
    }

    /* enumerate the child object */
    pSLinkEntry = AnscQueueGetFirstEntry(&pObjEntity->ObjeoQueue);

	if( !pSLinkEntry)
	{
                /*CID: 59112 Dereference after null check*/
		if((!bHasChildParam) && (pObjEntity->ObjDescr))
		{
			/* If there's no child param or object, register a dummy param */
            _ansc_sprintf( &pParameterHolder[pMyObject->uParameterIndex], "%s%s", pObjEntity->ObjDescr->Name, CCSP_DUMMY_PARAM_NAME );
            pParameterArray[pMyObject->uParameterArrayIndex].name_space   = &pParameterHolder[pMyObject->uParameterIndex];
            pParameterArray[pMyObject->uParameterArrayIndex++].dataType   = ccsp_int;
            pMyObject->uParameterIndex += _ansc_strlen( &pParameterHolder[pMyObject->uParameterIndex] ) + 1;

			AnscTraceWarning(("Register a dummy param '%s%s'\n", pObjEntity->ObjDescr->Name, CCSP_DUMMY_PARAM_NAME ));
		}
	}
	else
	{
		while ( pSLinkEntry )
		{
			pChildObjEntity  = ACCESS_DSLH_OBJ_ENTITY_OBJECT(pSLinkEntry);
			pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

			dslhCpeEnumObjEntities(pMyObject,pChildObjEntity);
		}
	}
}

ANSC_STATUS
dslhCpeCheckEmptyObject
	(
		ANSC_HANDLE					hThisObject
	)
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject         = (PDSLH_CPE_CONTROLLER_OBJECT)hThisObject;
    name_spaceType_t *              pParameterArray   = pMyObject->hParameterArray;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE)pMyObject->GetDslhMprIf(pMyObject);
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord         = NULL;

	if( !pParameterArray || !pParameterArray || !pDslhMprIf)
	{
		return ANSC_STATUS_SUCCESS;
	}

    pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)
        pDslhMprIf->GetObjRecord
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                DM_ROOTNAME
            );

    if( !pObjRecord || pObjRecord->GetObjRecordCount(pObjRecord) == 0)
    {
        return ANSC_STATUS_SUCCESS;
    }

	AnscTraceWarning(("Search for empty object in 'dslhCpeCheckEmptyObject'\n"));

    dslhCpeEnumObjEntities(pMyObject, pObjRecord->hDslhObjEntity);       

	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
DslhCpecoRegisterDataModelInternal
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCR_id,
        char*                       pXMLFile,
        void*                       pfnXMLLoader,
        char*                       pCompName,
        ULONG                       version,
        char*                       pDbusPath,
        char*                       pPrefix
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject         = (PDSLH_CPE_CONTROLLER_OBJECT)hThisObject;
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)pMyObject->hDslhDataModelAgent;
    name_spaceType_t              * pParameterArray   = NULL;
    char                          * pParameterHolder  = NULL;
    ULONG                           uCount            = 0;
    char                            pTmpCRName[128]   = { 0 };
    char                            pTmpCompName[128] = { 0 };
    errno_t                         rc                = -1;

    if( pCompName == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pParameterArray  = (name_spaceType_t *)AnscAllocateMemory(CCSP_PARAMETER_MAX_COUNT * sizeof(name_spaceType_t) );
    pParameterHolder = (char *)AnscAllocateMemory(CCSP_PARAMETER_MAX_COUNT * 90 * sizeof(char) );
    if ( !pParameterArray || !pParameterHolder )
        goto EXIT;

    /* Set return paramter into Cpe to collect full name of all parameters . */
    pMyObject->SetParameterArray((ANSC_HANDLE)pMyObject, pParameterArray, pParameterHolder,  CCSP_PARAMETER_MAX_COUNT );

    pDslhDataModelAgent->pPrefix = pPrefix;

    returnStatus = 
        pDslhDataModelAgent->RegBaseDataModel(pDslhDataModelAgent, pCompName);

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTrace("!!! failed to register CCSP base data model for '%s'\n", pCompName);

        pMyObject->SetParameterArray((ANSC_HANDLE)pMyObject, NULL, NULL, 0 );

        goto EXIT;
    }

    /* will be registered together with stardard data model */
    /*
    returnStatus =
        CcspBaseIf_registerBase
            (
                pMyObject->hDbusHandle,
                pCR_id,
                pCompName,
                version,
                pDbusPath,
                pPrefix
            );

    AnscTrace("!!! registerBase returns %d !!!\n", returnStatus);

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }
    */
    AnscSListInitializeHeader(&pMyObject->DmlCallbackList);    

    if( pXMLFile != NULL && AnscSizeOfString(pXMLFile) > 4)
    {
        /* load COSA plugin library */
        int ret = 0;
        ret = pMyObject->LoadExternalDMLibFile(pMyObject, pXMLFile, FALSE);
        if (ret)
            AnscTraceWarning(("DslhCpecoLoadExternalDMLibFile failed!!! ret: %d\n", ret));
    }
    else if( pfnXMLLoader != NULL )
    {
        int ret = 0;
        ret = pMyObject->LoadExternalDMLibFile2(pMyObject, pfnXMLLoader, FALSE);
        if (ret)
            AnscTraceWarning(("DslhCpecoLoadExternalDMLibFile failed!!! ret: %d\n", ret));
    }

    /* load DSLH parameter tree */
    returnStatus = pMyObject->InitDataModel((ANSC_HANDLE)pMyObject);

    AnscTrace("Status of InitDataModel = %lu\n", returnStatus);

    if (TRUE)
    {
        PSINGLE_LINK_ENTRY              pLink = NULL;
        PDSLHDMAGNT_CALLBACK            pEntry = NULL;

        pLink = AnscSListGetFirstEntry(&pMyObject->DmlCallbackList);

        while ( pLink )
        {
            pEntry = ACCESS_DSLHDMAGNT_CALLBACK(pLink);

            if (pEntry)
                pEntry->func(pMyObject);

            pLink  = AnscSListGetNextEntry(pLink);

            AnscFreeMemory(pEntry);
        }
    }
    
    /* This is just for test */
    uCount = 0;
    while( pParameterArray[uCount].name_space )
    {
        /*AnscTrace("%4d %s type:%d.\n", uCount, pParameterArray[uCount].name_space, pParameterArray[uCount].dataType );*/
        uCount++;
    } 

    /* add prefix to both CR and compName if needed */
    if( pPrefix != NULL && AnscSizeOfString(pPrefix) > 0)
    {
        if( _ansc_strstr(pCR_id, pPrefix) == pCR_id)
        {
            rc = strcpy_s(pTmpCRName, sizeof(pTmpCRName), pCR_id);
            ERR_CHK(rc);
        }
        else
        {
            rc = sprintf_s(pTmpCRName, sizeof(pTmpCRName), "%s%s", pPrefix, pCR_id);
            if(rc < EOK)
            {
               ERR_CHK(rc);
            }
        }

        if( _ansc_strstr(pCompName, pPrefix) == pCR_id)
        {
            rc = strcpy_s(pTmpCompName, sizeof(pTmpCompName), pCompName);
            ERR_CHK(rc);
        }
        else
        {
            rc = sprintf_s(pTmpCompName, sizeof(pTmpCompName), "%s%s", pPrefix, pCompName);
            if(rc < EOK)
            {
               ERR_CHK(rc);
            }
        }
    }
    else
    {
        rc = strcpy_s(pTmpCRName, sizeof(pTmpCRName), pCR_id);
        ERR_CHK(rc);
        rc = strcpy_s(pTmpCompName, sizeof(pTmpCompName), pCompName);
        ERR_CHK(rc);
    }

	/* There's a test case with an empty object (No child param, No child object).
	 * Since there's no param registered for this object, PA doesn't have any idea to distribute
	 * the request, hence we will fail this case
	 */
	/* Bin added here to register a dummy param for such object to pass the test case. */
    dslhCpeCheckEmptyObject(hThisObject);

    AnscTrace("!!! Ready to register capabilities of '%s' to '%s'...!!!\n", pTmpCompName, pTmpCRName);

#if 0

    for (uIndex = 0; uIndex < uCount; uIndex += CCSP_MAX_PARAMETER_PER_REGISTRATION)
    {
        ULONG uRegCount = uCount > uIndex + CCSP_MAX_PARAMETER_PER_REGISTRATION ?
                          CCSP_MAX_PARAMETER_PER_REGISTRATION : uCount - uIndex;

        ULONG uWait = 10; /* 10 seconds */
        do
        {
            returnStatus = 
              CcspBaseIf_registerCapabilities
                (
                    pMyObject->hDbusHandle,
                    pTmpCRName,                      /* Component id for CR */
                    pTmpCompName,
                    version,
                    pDbusPath,
                    pPrefix,
                    pParameterArray + uIndex,
                    uRegCount
                );

            /*
            AnscTrace("!!! registerCapabilities returns %d !!!\n", returnStatus);

            if (returnStatus != CCSP_SUCCESS) break;
            */

            /* When return value is CCSP_CR_ERR_NAMESPACE_OVERLAP(201), it's high possible this component is restarted.*/
            if ( ( CCSP_SUCCESS != returnStatus ) && ( CCSP_CR_ERR_NAMESPACE_OVERLAP != returnStatus ) )
             {
                AnscTraceWarning(("!!! %s registerCapabilities failed with code %d! Waiting for %d seconds to retry...!!! \n", pComponentName, returnStatus, uWait));
                AnscSleep(uWait * 1000);
                uWait *=2;
            }
            else
            {
                AnscTrace("!!! %s registerCapabilities returns %d !!!\n", pComponentName, returnStatus);

                break;
            }
        } while ( TRUE );

    }

#endif

        // register everything in one scoop, or the last DMs in the PandM array will not be sync'ed with TR069PA in time.  RTian 2013/11/27
        // this has higher memory usage than previous implementation, but hardware has improved so memory is no longer a concern.
        ULONG uWait = 10; /* 10 seconds */
        do
        {
            returnStatus = 
                CcspBaseIf_registerCapabilities
                (
                    pMyObject->hDbusHandle,
                    pTmpCRName,                      /* Component id for CR */
                    pTmpCompName,
                    version,
                    pDbusPath,
                    pPrefix,
                    pParameterArray,
                    uCount
                );


            if ( ( CCSP_SUCCESS != returnStatus ) && ( CCSP_CR_ERR_NAMESPACE_OVERLAP != returnStatus ) )
             {
                AnscTraceWarning(("!!! %s registerCapabilities failed with code %lu! Waiting for %lu seconds to retry...!!! \n", pComponentName, returnStatus, uWait));
                AnscSleep(uWait * 1000);
                uWait *=2;
            }
            else
            {
                AnscTrace("!!! %s registerCapabilities %lu items returns %lu !!!\n", pComponentName, uCount, returnStatus);
                break;
            }
        } while ( TRUE );


    /* unset the return paramter */
    if (uCount == 0)
        pMyObject->SetParameterArray((ANSC_HANDLE)pMyObject, NULL, NULL, 0 );

    /* generate the DT data model XML string */
    /*@jihe2 GenerateDTXmlString when get the Datamodel to save the startup time*/
    /*pDslhDataModelAgent->GenerateDTXmlString(pDslhDataModelAgent);*/

EXIT:
    if (uCount == 0)
    {
        pMyObject->SetParameterArray((ANSC_HANDLE)pMyObject, NULL, NULL, 0 );
        if ( pParameterArray )
            AnscFreeMemory(pParameterArray);
        if ( pParameterHolder )
            AnscFreeMemory(pParameterHolder);
    }

    if(ANSC_STATUS_SUCCESS == returnStatus || CCSP_SUCCESS == returnStatus)
        return ANSC_STATUS_SUCCESS;
    else
        return returnStatus;

}

ANSC_STATUS
DslhCpecoRegisterDataModel
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCR_id,
        char*                       pXMLFile,
        char*                       pCompName,
        ULONG                       version,
        char*                       pDbusPath,
        char*                       pPrefix
    )
{
    return DslhCpecoRegisterDataModelInternal
          (
              hThisObject,
              pCR_id,
              pXMLFile,
              NULL,
              pCompName,
              version,
              pDbusPath,
              pPrefix
          );
}

ANSC_STATUS
DslhCpecoRegisterDataModel2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCR_id,
        void*                       pfnXmlLoader,
        char*                       pCompName,
        ULONG                       version,
        char*                       pDbusPath,
        char*                       pPrefix
    )
{
    return DslhCpecoRegisterDataModelInternal
          (
              hThisObject,
              pCR_id,
              NULL,
              pfnXmlLoader,
              pCompName,
              version,
              pDbusPath,
              pPrefix
          );
}
