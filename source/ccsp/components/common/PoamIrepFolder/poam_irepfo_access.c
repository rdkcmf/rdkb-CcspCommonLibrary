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

    module:	poam_irepfo_access.c

        For Proxy Object Access Model Implementation (POAM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object-access functions
        of the Poam Irep Folder Object.

        *   PoamIrepFoGetFolderName
        *   PoamIrepFoGetFolder
        *   PoamIrepFoGetFolderByIndex
        *   PoamIrepFoAddFolder
        *   PoamIrepFoDelFolder
        *   PoamIrepFoGetFolderCount
        *   PoamIrepFoEnumFolder
        *   PoamIrepFoSortFolder
        *   PoamIrepFoGetRecord
        *   PoamIrepFoGetRecordByIndex
        *   PoamIrepFoGetRecordByIndex2
        *   PoamIrepFoSetRecord
        *   PoamIrepFoAddRecord
        *   PoamIrepFoDelRecord
        *   PoamIrepFoGetRecordCount
        *   PoamIrepFoGetRecordArray
        *   PoamIrepFoEnumRecord
        *   PoamIrepFoSortRecord
        *   PoamIrepFoClear
        *   PoamIrepFoClose
        *   PoamIrepFoEnableFileSync

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/04/03    initial revision.

**********************************************************************/


#include "poam_irepfo_global.h"
#include "safec_lib_common.h"

ANSC_HANDLE     g_MessageBusHandle_Irep = NULL;
ANSC_HANDLE     g_MessageBusHandle_Irep_combine = NULL; /*for combined components, they use this handle*/
char            g_SubSysPrefix_Irep[32] = {0};

/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        PoamIrepFoGetFolderName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the name of the current
        folder object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     folder name.

**********************************************************************/

char*
PoamIrepFoGetFolderName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PPOAM_IREP_FOLDER_OBJECT        pMyObject        = (PPOAM_IREP_FOLDER_OBJECT)hThisObject;
    PCHAR                           pName            = NULL;
    PCHAR                           pName2           = NULL;
    PCHAR                           pName3           = NULL;
    
    if ( pMyObject )
    {
        /* get C from A.B.C. */
        
        pName = (PCHAR)&pMyObject->Name[ AnscSizeOfString((const char *)pMyObject->Name) - 1 ];
        pName2 = pName;
        
        pName--; /* skip last '.' */
        while( ( *pName != '.' ) && ( pName != (CHAR*)&pMyObject->Name[0] ) ) pName--;
        if ( *pName == '.' )
        {
            pName++;
        }

        *pName2   = '\0';
        pName3 = AnscCloneString( pName );
        *pName2   = '.';

        return  pName3;
    }
    else
    {
        return  NULL;
    }
    
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        PoamIrepFoGetFolder
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pSubFolderName
            );

    description:

        This function is called to retrieve the IrepFolder object of
        the specified sub-folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pSubFolderName
                Specifies the name of the sub-folder to be retrieved.

    return:     poam object.

**********************************************************************/

ANSC_HANDLE
PoamIrepFoGetFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSubFolderName
    )
{
    CHAR                            fullName[IREPFO_FULLNAME_LENGTH]    = {0};
    PPOAM_IREP_FOLDER_OBJECT        pMyObject                           = (PPOAM_IREP_FOLDER_OBJECT)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pNewFolder                          = NULL;
    errno_t                         rc                                  = -1;
    
    /* Get the full name */

    rc = sprintf_s( fullName, sizeof(fullName), "%s%s.", pMyObject->Name, pSubFolderName);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return NULL;
    }

    pNewFolder = PoamIrepFoCreate( NULL, NULL, fullName);
    if ( pNewFolder )
    {
        return (ANSC_HANDLE)pNewFolder;
    }
    else
    {
        return NULL;
    }
    
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        PoamIrepFoGetFolderByIndex
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 ulIndex
            );

    description:

        This function is called to retrieve the IrepFolder object of
        the specified sub-folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 ulIndex
                Specifies the index of the sub-folder to be retrieved.

    return:     poam object.

**********************************************************************/

ANSC_HANDLE
PoamIrepFoGetFolderByIndex
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex
    )
{
    PPOAM_IREP_FOLDER_OBJECT        pMyObject                           = (PPOAM_IREP_FOLDER_OBJECT)hThisObject;
    PPOAM_IREP_FOLDER_OBJECT        pNewFolder                          = NULL;
    PCHAR                           pName                               = NULL;
    CHAR                            fullName[IREPFO_FULLNAME_LENGTH]    = {0};
    errno_t                         rc                                  = -1;

    /* Get the full name */
    pName = PoamIrepFoEnumFolder
            (
                hThisObject,
                ulIndex
            );
    
    if ( pName != NULL)
    {
        rc = sprintf_s( fullName, sizeof(fullName), "%s%s.", pMyObject->Name, pName);
        if(rc < EOK)
        {
           ERR_CHK(rc);
           return NULL;
        }

        pNewFolder = PoamIrepFoCreate( NULL, NULL, fullName );
        AnscFreeMemory(pName);
        
        if ( pNewFolder )
        {
            return (ANSC_HANDLE)pNewFolder;
        }
    }
    

    return NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        PoamIrepFoAddFolder
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pSubFolderName,
                SLAP_UINT32                 ulReserved
            );

    description:

        This function is called to create an IrepFolder object under
        the current folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pSubFolderName
                Specifies the name of the sub-folder to be created.

                SLAP_UINT32                 ulReserved
                Specifies the user-specific context value.

    return:     poam object.

**********************************************************************/

ANSC_HANDLE
PoamIrepFoAddFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSubFolderName,
        SLAP_UINT32                 ulReserved
    )
{
    UNREFERENCED_PARAMETER(ulReserved);
    return PoamIrepFoGetFolder( hThisObject, pSubFolderName);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        PoamIrepFoDelFolder
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pSubFolderName
            );

    description:

        This function is called to delete the specified sub-folder
        under the current folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pSubFolderName
                Specifies the name of the sub-folder to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
PoamIrepFoDelFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pSubFolderName
    )
{
    PPOAM_IREP_FOLDER_OBJECT        pMyObject                           = (PPOAM_IREP_FOLDER_OBJECT)hThisObject;
    ANSC_STATUS                     returnStatus                        = ANSC_STATUS_SUCCESS;
    CHAR                            fullName[IREPFO_FULLNAME_LENGTH]    = {0};
    ANSC_HANDLE                     hIrep                               = (g_MessageBusHandle_Irep_combine)?g_MessageBusHandle_Irep_combine:g_MessageBusHandle_Irep;
    errno_t                         rc                                  = -1;

    /* Del all sub record */
    rc = sprintf_s( fullName, sizeof(fullName), "%s%s.", pMyObject->Name, pSubFolderName);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }

    returnStatus = PSM_Del_Record( hIrep, g_SubSysPrefix_Irep, fullName );
    
    return returnStatus;
    
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PoamIrepFoGetFolderCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to number of sub-folders.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     folder count.

**********************************************************************/

ULONG
PoamIrepFoGetFolderCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    INT                             ret                                 = CCSP_SUCCESS;
    PPOAM_IREP_FOLDER_OBJECT        pMyObject                           = (PPOAM_IREP_FOLDER_OBJECT)hThisObject;
    PCCSP_BASE_RECORD               pRecordArray                        = NULL;
    UINT                            NumOfRecord                         = 0;
    UINT                            Count                               = 0;
    UINT                            Index                               = 0;
    ANSC_HANDLE                     hIrep                               = (g_MessageBusHandle_Irep_combine)?g_MessageBusHandle_Irep_combine:g_MessageBusHandle_Irep;        
    /* Get the full name */
    ret = PsmEnumRecords
            (
                hIrep,
                g_SubSysPrefix_Irep,
                (const char * const)pMyObject->Name,
                1,
                &NumOfRecord,
                &pRecordArray
            );

    if ( ret == CCSP_SUCCESS )
    {
        while ( Count < NumOfRecord )
        {
            if ( pRecordArray[Count].RecordType == CCSP_BASE_OBJECT )
            {
                Index++;
            }
            else if ( pRecordArray[Count].RecordType == CCSP_BASE_INSTANCE )
            {
                Index++;
            }

            Count++;
        }  
    }

    if ( pRecordArray )
        AnscFreeMemory(pRecordArray);
    
    return Index;        
    
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        PoamIrepFoEnumFolder
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 ulIndex
            );

    description:

        This function is called to retrieve the name of the specified
        sub-folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 ulIndex
                Specifies the index of the sub-folder to be retrieved.

    return:     folder name.

**********************************************************************/

char*
PoamIrepFoEnumFolder
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex
    )
{
    INT                             ret                                 = CCSP_SUCCESS;
    PPOAM_IREP_FOLDER_OBJECT        pMyObject                           = (PPOAM_IREP_FOLDER_OBJECT)hThisObject;
    CHAR                            fullName[IREPFO_FULLNAME_LENGTH]    = {0};
    PCCSP_BASE_RECORD               pRecordArray                        = NULL;
    UINT                            NumOfRecord                         = 0;
    UINT                            Count                               = 0;
    UINT                            Index                               = 0;
    ANSC_HANDLE                     hIrep                               = (g_MessageBusHandle_Irep_combine)?g_MessageBusHandle_Irep_combine:g_MessageBusHandle_Irep;
    errno_t                         rc                                  = -1;

    /* Get the full name */
    ret = PsmEnumRecords
            (
                hIrep,
                g_SubSysPrefix_Irep,
                (const char * const)pMyObject->Name,
                1,
                &NumOfRecord,
                &pRecordArray
            );

    if ( ret == CCSP_SUCCESS )
    {
        while ( Count < NumOfRecord )
        {
            if ( pRecordArray[Count].RecordType == CCSP_BASE_OBJECT )
            {
                if( Index == ulIndex )
                {
                    rc = sprintf_s(fullName, sizeof(fullName), "%s", pRecordArray[Count].Instance.Name);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }
                    break;
                }

                Index++;
            }
            else if ( pRecordArray[Count].RecordType == CCSP_BASE_INSTANCE )
            {
                if( Index == ulIndex )
                {
                    rc = sprintf_s(fullName, sizeof(fullName), "%d", pRecordArray[Count].Instance.InstanceNumber);
                    if(rc < EOK)
                    {
                        ERR_CHK(rc);
                    }
                    break;
                }

                Index++;
            }

            Count++;
            
        }
    }

    if ( pRecordArray )
        AnscFreeMemory(pRecordArray);

    if ( fullName[0] != '\0' )
    {
        return AnscCloneString(fullName);
    }

    return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        PoamIrepFoSortFolder
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSortingType,
                BOOL                        bAscending
            );

    description:

        This function is called to sort all the sub-folder objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSortingType
                Specifies how the sorting should be done.

                BOOL                        bAscending
                Specifies how the sorting should be done.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
PoamIrepFoSortFolder
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulSortingType);
    UNREFERENCED_PARAMETER(bAscending);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        PoamIrepFoGetRecord
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pRecordName,
                SLAP_UINT32*                pulRecordType
            );

    description:

        This function is called to retrieve the specified record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pRecordName
                Specifies the name of the record to be retrieved.

                SLAP_UINT32*                pulRecordType
                Specifies the type of the record to be returned.

    return:     record variable.

**********************************************************************/

SLAP_VARIABLE*
PoamIrepFoGetRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        SLAP_UINT32*                pulRecordType
    )
{
    UNREFERENCED_PARAMETER(pulRecordType);
    INT                             ret                                 = CCSP_SUCCESS;
    CHAR                            fullName[IREPFO_FULLNAME_LENGTH]    = {0};
    PPOAM_IREP_FOLDER_OBJECT        pMyObject                           = (PPOAM_IREP_FOLDER_OBJECT)hThisObject;
    UINT                            Type                                = 0;
    SLAP_VARIABLE                   Value                               = {0};
    PSLAP_VARIABLE                  pValue                              = NULL;
    ANSC_HANDLE                     hIrep                               = (g_MessageBusHandle_Irep_combine)?g_MessageBusHandle_Irep_combine:g_MessageBusHandle_Irep;
    errno_t                         rc                                  = -1;

    /* Get the full name */
    rc = sprintf_s(fullName, sizeof(fullName), "%s%s", pMyObject->Name, pRecordName);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return NULL;
    }

    ret = PSM_Get_Record_Value
            (
                hIrep,
                g_SubSysPrefix_Irep,
                fullName,
                &Type,
                &Value
            );
    
    if ( ret == CCSP_SUCCESS )
    {
        pValue = AnscAllocateMemory(sizeof(SLAP_VARIABLE));
        SlapInitVariable((pValue));

        SlapCloneVariable(&Value,pValue);
        SlapCleanVariable(&Value);
    
        return pValue;
    }
    else
    {
        return NULL;
    }
    
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        PoamIrepFoGetRecordByIndex
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 ulIndex,
                SLAP_UINT32*                pulRecordType
            );

    description:

        This function is called to retrieve the specified record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 ulIndex
                Specifies the index of the record to be retrieved.

                SLAP_UINT32*                pulRecordType
                Specifies the type of the record to be returned.

    return:     record variable.

**********************************************************************/

SLAP_VARIABLE*
PoamIrepFoGetRecordByIndex
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex,
        SLAP_UINT32*                pulRecordType
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(pulRecordType);
    return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_VARIABLE*
        PoamIrepFoGetRecordByIndex2
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 ulIndex,
                SLAP_UINT32*                pulRecordType,
                char**                      ppRecordName
            );

    description:

        This function is called to retrieve the specified record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 ulIndex
                Specifies the index of the record to be retrieved.

                SLAP_UINT32*                pulRecordType
                Specifies the type of the record to be returned.

                char**                      ppRecordName
                Specifies the record name to be returned.

    return:     record variable.

**********************************************************************/

SLAP_VARIABLE*
PoamIrepFoGetRecordByIndex2
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex,
        SLAP_UINT32*                pulRecordType,
        char**                      ppRecordName
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(pulRecordType);
    UNREFERENCED_PARAMETER(ppRecordName);
    return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        PoamIrepFoSetRecord
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pRecordName,
                SLAP_UINT32                 ulRecordType,
                SLAP_UINT32                 ulContentType,
                SLAP_VARIABLE*              pRecordVar
            );

    description:

        This function is called to configure the specified record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pRecordName
                Specifies the name of the record to be configured.

                SLAP_UINT32                 ulRecordType
                Specifies the type of the record to be configured.

                SLAP_UINT32                 ulContentType
                Specifies the type of the content to be configured.

                SLAP_VARIABLE*              pRecordVar
                Specifies the record value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
PoamIrepFoSetRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        SLAP_UINT32                 ulRecordType,
        SLAP_UINT32                 ulContentType,
        SLAP_VARIABLE*              pRecordVar
    )
{
    return PoamIrepFoAddRecord
            (
                hThisObject, 
                pRecordName, 
                ulRecordType, 
                ulContentType, 
                pRecordVar,
                (intptr_t)NULL
             );
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        PoamIrepFoAddRecord
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pRecordName,
                SLAP_UINT32                 ulRecordType,
                SLAP_UINT32                 ulContentType,
                SLAP_VARIABLE*              pRecordVar,
                ULONG                       ulReserved
            );

    description:

        This function is called to create the specified record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pRecordName
                Specifies the name of the record to be created.

                SLAP_UINT32                 ulRecordType
                Specifies the type of the record to be created.

                SLAP_UINT32                 ulContentType
                Specifies the type of the content to be configured.

                SLAP_VARIABLE*              pRecordVar
                Specifies the record value to be configured.

                ULONG                       ulReserved
                Specifies the user-specific record context value.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
PoamIrepFoAddRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        SLAP_UINT32                 ulRecordType,
        SLAP_UINT32                 ulContentType,
        SLAP_VARIABLE*              pRecordVar,
        ULONG                       ulReserved
    )
{
    UNREFERENCED_PARAMETER(ulContentType);
    UNREFERENCED_PARAMETER(ulReserved);
    ANSC_STATUS                     returnStatus                        = ANSC_STATUS_SUCCESS;
    INT                             ret                                 = CCSP_SUCCESS;
    CHAR                            fullName[IREPFO_FULLNAME_LENGTH]    = {0};
    PPOAM_IREP_FOLDER_OBJECT        pMyObject                           = (PPOAM_IREP_FOLDER_OBJECT)hThisObject;
    ANSC_HANDLE                     hIrep                               = (g_MessageBusHandle_Irep_combine)?g_MessageBusHandle_Irep_combine:g_MessageBusHandle_Irep;
    errno_t                         rc                                  = -1;

    /* Get the full name */
    rc = sprintf_s(fullName, sizeof(fullName), "%s%s", pMyObject->Name, pRecordName);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    
    ret = PSM_Set_Record_Value
        (
            hIrep,
            g_SubSysPrefix_Irep,
            fullName,
            ulRecordType,
            pRecordVar
        );
    
    RETURN_STATUS_PSM_TO_ANSC(ret, returnStatus);

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        PoamIrepFoDelRecord
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pRecordName
            );

    description:

        This function is called to delete the specified record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pRecordName
                Specifies the name of the record to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
PoamIrepFoDelRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    )
{
    ANSC_STATUS                     returnStatus                        = ANSC_STATUS_SUCCESS;
    INT                             ret                                 = CCSP_SUCCESS;
    CHAR                            fullName[IREPFO_FULLNAME_LENGTH]    = {0};
    PPOAM_IREP_FOLDER_OBJECT        pMyObject                           = (PPOAM_IREP_FOLDER_OBJECT)hThisObject;
    ANSC_HANDLE                     hIrep                               = (g_MessageBusHandle_Irep_combine)?g_MessageBusHandle_Irep_combine:g_MessageBusHandle_Irep;    
    errno_t                         rc                                  = -1;

    /* Get the full name */
    rc = sprintf_s(fullName, sizeof(fullName), "%s%s", pMyObject->Name, pRecordName);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }

    ret = PSM_Del_Record
            (
                hIrep,
                g_SubSysPrefix_Irep,
                fullName
            );
    
    RETURN_STATUS_PSM_TO_ANSC(ret, returnStatus);

    return returnStatus;
    
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        PoamIrepFoGetRecordCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the number of records.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     record number.

**********************************************************************/

ULONG
PoamIrepFoGetRecordCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UCHAR_ARRAY*
        PoamIrepFoGetRecordArray
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the array of records.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     record array.

**********************************************************************/

SLAP_UCHAR_ARRAY*
PoamIrepFoGetRecordArray
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        PoamIrepFoEnumRecord
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 ulIndex,
                SLAP_UINT32*                pulRecordType,
                SLAP_UINT32*                pulRecordSize
            );

    description:

        This function is called to retrieve the name of the specified
        record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 ulIndex
                Specifies the index of the record to be retrieved.

                SLAP_UINT32*                pulRecordType
                Specifies the type of the record to be returned.

                SLAP_UINT32*                pulRecordSize
                Specifies the size of the record to be returned.

    return:     folder name.

**********************************************************************/

char*
PoamIrepFoEnumRecord
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 ulIndex,
        SLAP_UINT32*                pulRecordType,
        SLAP_UINT32*                pulRecordSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(pulRecordType);
    UNREFERENCED_PARAMETER(pulRecordSize);
    return NULL;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        PoamIrepFoSortRecord
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSortingType,
                BOOL                        bAscending
            );

    description:

        This function is called to sort all the record objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSortingType
                Specifies how the sorting should be done.

                BOOL                        bAscending
                Specifies how the sorting should be done.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
PoamIrepFoSortRecord
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulSortingType);
    UNREFERENCED_PARAMETER(bAscending);
    return ANSC_STATUS_FAILURE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        PoamIrepFoClear
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clear the folder content.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
PoamIrepFoClear
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PPOAM_IREP_FOLDER_OBJECT        pMyObject                           = (PPOAM_IREP_FOLDER_OBJECT)hThisObject;
    CHAR                            fullName[IREPFO_FULLNAME_LENGTH]    = {0};
    ANSC_HANDLE                     hIrep                               = (g_MessageBusHandle_Irep_combine)?g_MessageBusHandle_Irep_combine:g_MessageBusHandle_Irep;
    errno_t                         rc                                  = -1;

    /* Del all sub record */
    rc = sprintf_s(fullName, sizeof(fullName), "%s", pMyObject->Name);
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return ANSC_STATUS_FAILURE;
    }
    /*
      returnStatus = pMyObject->DelRecordRecursive(hThisObject, fullName);
      */
    
    PSM_Del_Record( hIrep, g_SubSysPrefix_Irep, fullName );

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        PoamIrepFoClose
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to close the current folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
PoamIrepFoClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        PoamIrepFoEnableFileSync
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bEnabled
            );

    description:

        This function is called to enable/disable file sync.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bEnabled
                Specifies whether file sync should be enabled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
PoamIrepFoEnableFileSync
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(bEnabled);

    return ANSC_STATUS_SUCCESS;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        PoamIrepFoDelRecordRecursive
            (
                ANSC_HANDLE                 hThisObject,
                char*                               pRecordName
            );

    description:

        This function is called to del all sub couples in this sub record

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                         pRecordName
                Specifies the sub record to del

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
PoamIrepFoDelRecordRecursive
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pRecordName
    )
{
    ANSC_STATUS                     returnStatus                        = ANSC_STATUS_SUCCESS;
    INT                             ret                                 = CCSP_SUCCESS;
    PPOAM_IREP_FOLDER_OBJECT        pMyObject                           = (PPOAM_IREP_FOLDER_OBJECT)hThisObject;
    CHAR                            fullName[IREPFO_FULLNAME_LENGTH]    = {0};
    PCCSP_BASE_RECORD               pRecordArray                        = NULL;
    UINT                            NumOfRecord                         = 0;
    UINT                             Index                               = 0;
    ANSC_HANDLE                     hIrep                               = (g_MessageBusHandle_Irep_combine)?g_MessageBusHandle_Irep_combine:g_MessageBusHandle_Irep;
    errno_t                         rc                                  = -1;

    /* Get the full name */
    ret = PsmEnumRecords
            (
                hIrep,
                g_SubSysPrefix_Irep,
                pRecordName,
                1,
                &NumOfRecord,
                &pRecordArray
            );

    if ( ret == CCSP_SUCCESS )
    {
        for( Index=0; Index<NumOfRecord; Index++ )
        {
            if ( pRecordArray[Index].RecordType == CCSP_BASE_OBJECT )
            {
                rc = sprintf_s(fullName, sizeof(fullName), "%s%s.", pRecordName, pRecordArray[Index].Instance.Name);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
                returnStatus = pMyObject->DelRecordRecursive(hThisObject, fullName);
                if ( returnStatus != ANSC_STATUS_SUCCESS)
                {
                    break;
                }
                
                continue;
            }
            else if ( pRecordArray[Index].RecordType == CCSP_BASE_INSTANCE )
            {
                rc = sprintf_s(fullName, sizeof(fullName), "%s%d.", pRecordName, pRecordArray[Index].Instance.InstanceNumber);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
                returnStatus = pMyObject->DelRecordRecursive(hThisObject, fullName);
                if ( returnStatus != ANSC_STATUS_SUCCESS)
                {
                    break;
                }
                
                continue;
            }
            else if ( pRecordArray[Index].RecordType == CCSP_BASE_PARAM )
            {
                rc = sprintf_s(fullName, sizeof(fullName), "%s%s", pRecordName, pRecordArray[Index].Instance.Name);
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
                PSM_Del_Record( hIrep, g_SubSysPrefix_Irep, fullName );
                
                continue;    
            }
            else
            {
                returnStatus = ANSC_STATUS_FAILURE;
                
                break;
            }
            

        }
    }

    if ( pRecordArray )
        AnscFreeMemory(pRecordArray);

    return returnStatus;
}

