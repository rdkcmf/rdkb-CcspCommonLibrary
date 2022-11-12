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

    module: dslh_dmagnt_exported.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the framework's exported functions
        by the Dslh DataModelAgent object;

        *   COSANotifyValueChange
        *   COSANotifyDiagComplete
        *   COSAValidateHierarchyInterface
        *   COSAGetParamValueBool
        *   COSAGetParamValueInt
        *   COSAGetParamValueUlong
        *   COSAGetParamValueString
        *   COSAGetParamValue
        *   COSAGetRegistryRootFolder
        *   COSAGetInstanceNumberByIndex
        *   COSAGetInterfaceByName
        *   COSAGetMessageBusHandle
        *   COSARegisterCallBackAfterInitDml
        *   COSAGetSubsystemPrefix
        *   COSARepopulateTable

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        01/06/2011    initial revision.
        01/11/2011    added SLAP related apis.
        03/21/2011    added api to retrieve instance number by index
        10/25/2011    added api to retrieve subsystem prefix;
        11/08/2011    added api "COSARepopulateTable".

**********************************************************************/


#include "dslh_dmagnt_global.h"
#include "dslh_varro_interface.h"
#include "ccsp_base_api.h"
#include "safec_lib_common.h"

extern  PDSLH_DATAMODEL_AGENT_OBJECT         g_DslhDataModelAgent;

/*static const char* msg_path = "/com/cisco/spvtg/ccsp/EVENT" ;
static const char* msg_interface = "com.cisco.spvtg.ccsp.EVENT" ;
static const char* msg_method = "__send" ;
static const char* Introspect_msg = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
                                    "<node name=\"/com/cisco/ccsp/dbus\">\n"
                                    "  <interface name=\"org.freedesktop.DBus.Introspectable\">\n"
                                    "    <method name=\"Introspect\">\n"
                                    "      <arg name=\"data\" direction=\"out\" type=\"s\"/>\n"
                                    "    </method>\n"
                                    "  </interface>\n"
                                    "  <interface name=\"ccsp.msg\">\n"
                                    "    <method name=\"__send\">\n"
                                    "      <arg type=\"s\" name=\"from\" direction=\"in\" />\n"
                                    "      <arg type=\"s\" name=\"request\" direction=\"in\" />\n"
                                    "      <arg type=\"s\" name=\"response\" direction=\"out\" />\n"
                                    "    </method>\n"
                                    "    <method name=\"__app_request\">\n"
                                    "      <arg type=\"s\" name=\"from\" direction=\"in\" />\n"
                                    "      <arg type=\"s\" name=\"request\" direction=\"in\" />\n"
                                    "      <arg type=\"s\" name=\"argu\" direction=\"in\" />\n"
                                    "      <arg type=\"s\" name=\"response\" direction=\"out\" />\n"
                                    "    </method>\n"
                                    "  </interface>\n"
                                    "</node>\n"
                                    ;
*/

//unused func
#if 0
static DBusHandlerResult path_message_func (DBusConnection  *conn,
                                     DBusMessage     *message,
                                     void            *user_data)
{
    UNREFERENCED_PARAMETER(user_data);
    const char *interface = dbus_message_get_interface(message);
    const char *method   = dbus_message_get_member(message);
    DBusMessage *reply;
    char *resp = "8888";
    char *from = 0;
    char *req = 0;
    char * err_msg  = DBUS_ERROR_NOT_SUPPORTED;

    reply = dbus_message_new_method_return (message);
    if (reply == NULL)
    {
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    if(!strcmp("org.freedesktop.DBus.Introspectable", interface)  && !strcmp(method, "Introspect"))
    {
        printf("Received method Introspect...\n");

        if ( !dbus_message_append_args (reply, DBUS_TYPE_STRING, &Introspect_msg, DBUS_TYPE_INVALID))
            printf ("No memory\n");

        if (!dbus_connection_send (conn, reply, NULL))
            printf ("No memory\n");

        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;

    }

    if (!strcmp(msg_interface, interface) && !strcmp(method, msg_method))
    {
        printf("Received method %s...\n", method);

        if(dbus_message_get_args (message,
                                  NULL,
                                  DBUS_TYPE_STRING, &from,
                                  DBUS_TYPE_STRING, &req,
                                  DBUS_TYPE_INVALID))
        {
            dbus_message_append_args (reply, DBUS_TYPE_STRING, &resp, DBUS_TYPE_INVALID);

            if (!dbus_connection_send (conn, reply, NULL))
                printf ("No memory\n");

            dbus_message_unref (reply);
        }

        return DBUS_HANDLER_RESULT_HANDLED;
    }


    dbus_message_set_error_name (reply, err_msg) ;
    dbus_connection_send (conn, reply, NULL);
    dbus_message_unref (reply);
    return DBUS_HANDLER_RESULT_HANDLED;
}
#endif

/**********************************************************************

    prototype:

        BOOL
        COSANotifyValueChange
            (
                void*                       hDmlAgent,
                char*                       pParamName   
            )

    description:

        This function notifies cosa that one value has been changed back-end.

    argument:   char*                       pParamName
                The name of the changed value.

    return:     Succeeded or not;

**********************************************************************/

ANSC_STATUS
COSANotifyValueChange
   (
       void*                       hDmlAgent,
       char*                       pParamName   
   )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDmlAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase    = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf          = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;
    PDSLH_VAR_RECORD_OBJECT         pVarRecord          = (PDSLH_VAR_RECORD_OBJECT)NULL;

    pVarRecord =
        (PDSLH_VAR_RECORD_OBJECT)pDslhMprIf->GetVarRecord
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pParamName
            );

    if ( !pVarRecord )
    {
        AnscTrace("COSANotifyValueChange: Invalid or unknown parameter name: %s\n", pParamName);

        return ANSC_STATUS_FAILURE;
    }

    pVarRecord->NotifyValueChanged((ANSC_HANDLE)pVarRecord);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    prototype:

        BOOL
        COSANotifyDiagComplete
            (
               void*                       hDmlAgent,
            );

    description:

        This function notifies cosa that diag has finished.

    argument:   

    return:     Succeeded or not;

**********************************************************************/
BOOL
COSANotifyDiagComplete
    (
       void*                       hDmlAgent
    )
{
    UNREFERENCED_PARAMETER(hDmlAgent);
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;

    return  (returnStatus == ANSC_STATUS_SUCCESS);        
}


/**********************************************************************

    prototype:

        BOOL
        COSAValidateHierarchyInterface
            (
                void*                       hDmlAgent,
                PCHAR                       pHierarchName,
                ULONG                       uType
            )

    description:

        This function is called to validate the HierarchName of an interface;

    argument:   
            PCHAR                       pHierarchName,
            The full HierarchName of the interface;

            ULONG                       uType
            The type of interfcace;

    return:     Valid or not;

**********************************************************************/
BOOL
COSAValidateHierarchyInterface
    (
        void*                       hDmlAgent,
        PCHAR                       pHierarchName,
        ULONG                       uType
    )
{
    UNREFERENCED_PARAMETER(hDmlAgent);
    UNREFERENCED_PARAMETER(pHierarchName);
    UNREFERENCED_PARAMETER(uType);
    return TRUE;
}

/**********************************************************************

    prototype:

        BOOL
        COSAGetParamValueBool
            (
                void*                       hDmlAgent,
                char*                       pParamName
            )

    description:

        This function is called to retrieve a Bool value of a parameter;

    argument:   
            char*                       pParamName
            The full name of the parameter;

    return:     the bool value;

**********************************************************************/
BOOL
COSAGetParamValueBool
    (
        void*                       hDmlAgent,
        char*                       pParamName
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDmlAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase   = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;

    return pDslhMprIf->GetParamValueBool(pDslhMprIf->hOwnerContext, pParamName);
}

/**********************************************************************

    prototype:

        int
        COSAGetParamValueInt
            (
                void*                       hDmlAgent,
                char*                       pParamName
            )

    description:

        This function is called to retrieve an integer value of a parameter;

    argument:   
            char*                       pParamName
            The full name of the parameter;

    return:     the integer value;

**********************************************************************/
int
COSAGetParamValueInt
    (
        void*                       hDmlAgent,
        char*                       pParamName
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDmlAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase   = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;

    return pDslhMprIf->GetParamValueInt(pDslhMprIf->hOwnerContext, pParamName);
}

/**********************************************************************

    prototype:

        ULONG
        COSAGetParamValueUlong
            (
                void*                       hDmlAgent,
                char*                       pParamName
            )

    description:

        This function is called to retrieve a UONG value of a parameter;

    argument:   
            char*                       pParamName
            The full name of the parameter;

    return:     the ULONG value;

**********************************************************************/
ULONG
COSAGetParamValueUlong
    (
        void*                       hDmlAgent,
        char*                       pParamName
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDmlAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase   = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;

    return pDslhMprIf->GetParamValueUint(pDslhMprIf->hOwnerContext, pParamName);
}

/**********************************************************************

    prototype:

        int
        COSAGetParamValueString
            (
                void*                       hDmlAgent,
                char*                       pParamName,
                char*                       pBuffer,
                PULONG                      pulSize
            )

    description:

        This function is called to retrieve a string value of a parameter;

    argument:   
            char*                       pParamName
            The full name of the parameter;

            char*                       pBuffer,
            The buffer for the value;

            PULONG                      pulSize
            The buffer of size;

    return:     0 = SUCCESS; -1 = FAILURE; 1 = NEW_SIZE;

**********************************************************************/
int
COSAGetParamValueString
    (
        void*                       hDmlAgent,
        char*                       pParamName,
        char*                       pBuffer,
        PULONG                      pulSize
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDmlAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase    = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf          = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;
    char*                           pString             = NULL;
    int result;
    unsigned long len;

    pString = pDslhMprIf->GetParamValueString(pDslhMprIf->hOwnerContext, pParamName);

    if (pString == NULL)
    {
        return -1;
    }

    len = strlen(pString);

    if (len >= *pulSize)
    {
        // AnscTraceWarning(("COSAGetParamValueString: output buffer too small '%s'\n", pParamName));
        result = 1;
    }
    else
    {
        memcpy(pBuffer, pString, len + 1);
        result = 0;
    }

    AnscFreeMemory(pString);

    /*
       Warning: inconsisent API. The passed in value is a buffer size, the
       returned value is a string length (ie one less then the required
       buffer size).
    */
    *pulSize = len;

    return result;
}

/**********************************************************************

    prototype:

        void*
        COSAGetParamValue
            (
                void*                       hDmlAgent,
                char*                       pParamName
            )

    description:

        This function is called to retrieve a parameter value;

    argument:   
            char*                       pParamName
            The full name of the parameter;

    return:     the parameter value handle;

**********************************************************************/
void*
COSAGetParamValue
    (
        void*                       hDmlAgent,
        char*                       pParamName
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDmlAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase   = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;

    return (void*)pDslhMprIf->GetParamValue(pDslhMprIf->hOwnerContext, pParamName);
}



/**********************************************************************

    prototype:

        BOOL
        COSASetParamValueBool
            (
                char*                       pParamName,
                BOOL                        bParamValue
            )

    description:

        this interface support recursely call.

    argument:   
            char*                       pParamName
            The full name of the parameter;

            BOOL                        bParamValue
            This is the value for this type.
            
    return:     ANSC_STATUS

**********************************************************************/
ANSC_STATUS
COSASetParamValueBool
    (
        char*                       pParamName,
        BOOL                        bParamValue
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)g_DslhDataModelAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase   = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;

    printf("Call in COSASetParamValueBool.\n");

    return pDslhMprIf->SetParamValueBool(pDslhMprIf->hOwnerContext, DSLH_MPA_ENTITY_Subscriber/*DSLH_MPA_ENTITY_ACS*/, pParamName, bParamValue);
}


/**********************************************************************

    prototype:

        ANSC_STATUS
        COSASetParamValueInt
            (
                char*                       pParamName,
                int                         iParamValue
            )

    description:

        This function is called to set an integer value of a parameter;

    argument:   
            char*                       pParamName
            The full name of the parameter;

            int                         iParamValue
            This is the value for this type.
            
    return:    ANSC_STATUS

**********************************************************************/
ANSC_STATUS
COSASetParamValueInt
    (
        char*                       pParamName,
        int                         iParamValue
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)g_DslhDataModelAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase   = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;

    return pDslhMprIf->SetParamValueInt(pDslhMprIf->hOwnerContext,  DSLH_MPA_ENTITY_Subscriber/*DSLH_MPA_ENTITY_ACS*/, pParamName, iParamValue);
}


/**********************************************************************

    prototype:

        ANSC_STATUS
        COSASetParamValueUlong
            (
                char*                       pParamName,
                ULONG                       ulParamValue
            )

    description:

        This function is called to set a UONG value of a parameter;

    argument:   
            char*                       pParamName
            The full name of the parameter;

            ULONG                       ulParamValue
            This is the value for this type.
            
    return:     ANSC_STATUS

**********************************************************************/
ANSC_STATUS
COSASetParamValueUlong
    (
        char*                       pParamName,
        ULONG                       ulParamValue
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)g_DslhDataModelAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase   = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;

    return pDslhMprIf->SetParamValueUint(pDslhMprIf->hOwnerContext, DSLH_MPA_ENTITY_Subscriber/*DSLH_MPA_ENTITY_ACS*/, pParamName, ulParamValue);
}


/**********************************************************************

    prototype:

        ANSC_STATUS
        COSASetParamValueString
            (
                char*                       pParamName,
                char*                       pParamValue
            )

    description:

        This function is called to set a string value of a parameter;

    argument:   
            char*                       pParamName
            The full name of the parameter;

            char*                       pParamValue
            This is the value for this string type.
            
    return:    ANSC_STATUS type ;

**********************************************************************/
ANSC_STATUS
COSASetParamValueString
    (
        char*                       pParamName,
        char*                       pParamValue
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)g_DslhDataModelAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase    = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf          = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;

    return pDslhMprIf->SetParamValueString(pDslhMprIf->hOwnerContext, DSLH_MPA_ENTITY_Subscriber/*DSLH_MPA_ENTITY_ACS*/, pParamName, pParamValue);
}


/**********************************************************************

    prototype:

        ANSC_STATUS
        COSASetParamValue
            (
                char*                       pParamName,
                SLAP_VARIABLE*        pParamValue
            )

    description:

        This function is called to retrieve a parameter value;

    argument:   
            char*                       pParamName
            The full name of the parameter;

            SLAP_VARIABLE*        pParamValue
            This is a slap variable.
            
    return:    ANSC_STATUS type ;

**********************************************************************/
ANSC_STATUS
COSASetParamValue
    (
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)g_DslhDataModelAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase   = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;

    return pDslhMprIf->SetParamValue(pDslhMprIf->hOwnerContext, DSLH_MPA_ENTITY_Subscriber/*DSLH_MPA_ENTITY_ACS*/, pParamName, pParamValue);
}


/**********************************************************************

    prototype:

        void*
        COSAGetRegistryRootFolder
            (
               void*                       hDmlAgent,
            )

    description:

        This function is called to retrieve the root folder of System Registry 
        in COSA;

    argument:   
            
    return:     the handle of the folder;

**********************************************************************/
void*
COSAGetRegistryRootFolder
    (
        void*                       hDmlAgent
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    return (void*)pDslhDmlAgent->hIrepFolderCOSA;
}


/**********************************************************************

    prototype:

        ULONG
        COSAGetInstanceNumberByIndex
            (
                void*                       hDmlAgent,
                char*                      pObjName,
                ULONG                      ulIndex
            );

    description:

        This function is called to retrieve the instance number specified by index;

    argument:   char*                      pObjName,
                The full object name;

                ULONG                      ulIndex
                The index specified;
            
    return:     the instance number;

**********************************************************************/
ULONG
COSAGetInstanceNumberByIndex
    (
        void*                       hDmlAgent,
        char*                      pObjName,
        ULONG                      ulIndex
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDmlAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase    = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf          = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord          = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity          = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord     = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           i                   = 0;

    if( pObjName == NULL)
    {
        return 0;
    }

    pObjRecord =
        (PDSLH_OBJ_RECORD_OBJECT)pDslhMprIf->GetObjRecord
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pObjName
            );

    if( pObjRecord == NULL)
    {
        return 0;
    }

    pObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecord->hDslhObjEntity;

    if( pObjEntity == NULL || pObjEntity->ObjDescr->Type != DSLH_CWMP_OBJECT_TYPE_table)
    {
        return 0;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pObjRecord->ObjroQueue);

    while ( pSLinkEntry )
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);

        if ( pChildObjRecord != NULL && i == ulIndex)
        {
            return  _ansc_atoi(pChildObjRecord->LastName);
        }

        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);
        i ++;
    }

    return 0;
}

/**********************************************************************

    prototype:

        void*
        COSAGetInterfaceByName
            (
                void*                       hDmlAgent,
                char*                   pIfName
            )

    description:

        This function is called to retrieve the interface specified by name;

    argument:   char*                   pIfName
                The interface name
            
    return:     the interface handle;

**********************************************************************/
void*
COSAGetInterfaceByName
    (
        void*                       hDmlAgent,
        char*                           pIfName
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDmlAgent->hDslhCpeController;

    return pDslhCpeController->GetInterfaceByName(pDslhCpeController, pIfName);
}
/**********************************************************************

    prototype:

        void*
        COSAGetMessageBusHandle
            (
               void*                       hDmlAgent,
            )

    description:

        This function is called to retrieve the message bus handle

    argument:   
            
    return:     the handle of the folder;

**********************************************************************/
void*
COSAGetMessageBusHandle
    (
        void*                       hDmlAgent
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    PCCSP_FC_CONTEXT         pPnmFcContext       =   (PCCSP_FC_CONTEXT)pDslhDmlAgent->GetFcContext((ANSC_HANDLE)pDslhDmlAgent);

    if ( pPnmFcContext )
    {
        return pPnmFcContext->hMessageBus;
    }
    else
    {
        return pDslhDmlAgent->hDbusHandle;
    }
}


/**********************************************************************

    prototype:

        void*
        COSARegisterCallBackAfterInitDml
            (
                void*                       hDmlAgent,
                ANSC_HANDLE                 handle
            )

    description:

        This function is called to register a callback function.

    argument:   
                ANSC_HANDLE                 handle,
                The callback function;
            
    return:     none

**********************************************************************/
void *
COSARegisterCallBackAfterInitDml
    (
        void*                       hDmlAgent,
        ANSC_HANDLE                 handle
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    DSLHDMAGNT_CALLBACK *           pEntry = handle; 
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDmlAgent->hDslhCpeController;

    AnscSListPushEntryAtBack(&pDslhCpeController->DmlCallbackList, &pEntry->Linkage);                        
                
    return handle;
}

/**********************************************************************

    prototype:

        char*
        COSAGetSubsystemPrefix
            (
               void*                       hDmlAgent,
            )

    description:

        This function is called to retrieve the subsystem prefix string value;

    argument:   
            
    return:     the prefix string

**********************************************************************/
char*
COSAGetSubsystemPrefix
    (
        void*                       hDmlAgent
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    return pDslhDmlAgent->pPrefix;
}


/**********************************************************************

    prototype:

        char*
        COSAGetSubsystemPrefix2
            (
            )

    description:

        This function is called to retrieve the subsystem prefix string value;

    argument:   
            
    return:     the prefix string

**********************************************************************/
char*
COSAGetSubsystemPrefix2
    (
    )
{
    return g_DslhDataModelAgent->pPrefix;
}

/**********************************************************************

    prototype:

        BOOL
        COSARepopulateTable
            (
                void*                       hDmlAgent,
                char*        pTabName
            )

    description:

        This function is called to re-populate a table object.

    argument:   
                char*       pTabName
                The full object name of a table. e.g. "Device.WANConnection.1.WINIPConnection.".
                There should no "{i}" included.
            
    return:     the status of the operation.

**********************************************************************/
BOOL
COSARepopulateTable
    (
        void*                       hDmlAgent,
        char*        pTabName
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDmlAgent       = (PDSLH_DATAMODEL_AGENT_OBJECT)hDmlAgent;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDmlAgent->hDslhCpeController;
    PDSLH_MPR_INTERFACE             pDslhMprIf          = (PDSLH_MPR_INTERFACE        )pDslhCpeController->GetDslhMprIf(pDslhCpeController);
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord          = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity          = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;

    if( !pTabName || !pDslhMprIf)
    {
        return FALSE;
    }

    /* increase the request counter by one */
    pDslhMprIf->IncReqCounter(pDslhMprIf->hOwnerContext);

    pObjRecord =
        (PDSLH_OBJ_RECORD_OBJECT)pDslhMprIf->GetObjRecord
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pTabName
            );

    if ( !pObjRecord )
    {
        AnscTrace("Invalid table object name '%s' in COSARepopulateTable.\n", pTabName);

        return FALSE;
    }

    pObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecord->hDslhObjEntity;

    if( pObjEntity)
    {
        if(pObjEntity->ObjDescr->Type != DSLH_CWMP_OBJECT_TYPE_table)
        {
            AnscTrace("It's not a table : '%s'\n", pTabName);

            return FALSE;
        }

        if( !pObjEntity->ObjDescr->bDynamic)
        {
            pObjRecord->DelAllObjRecords  ((ANSC_HANDLE)pObjRecord);
            pObjRecord->PopulateObjRecords((ANSC_HANDLE)pObjRecord);
            pObjRecord->PopulateVarRecords((ANSC_HANDLE)pObjRecord);
        }
    }

    return TRUE;
}

/**********************************************************************

    prototype:

        void*
        COSAGetParamValue
            (
                parameterValStruct_t       val,
                ULONG                      *parameterValueLength
            )

    description:

        This function is called to retrieve a parameter value;

    argument:   
            char*                       pParamName
            The full name of the parameter;

    return:     the parameter value handle;

**********************************************************************/
ANSC_STATUS
COSAGetParamValueByPathName
    (
        void*                       bus_handle,
        parameterValStruct_t        *val,
        ULONG                       *parameterValueLength
    )
{
    char *pSubSysPrefixList[] = {"eRT.", "eMG.", "", NULL};
    char cr_id[256];
    char * dst_componentid =  NULL;
    char * dst_pathname    =  NULL;
    char * pSubsystem = COSAGetSubsystemPrefix2();
    int same_component=0;
    int i, size1, size2;
    int ret = 0;

    componentStruct_t ** ppComponents = NULL;
    parameterValStruct_t **parameterVal = NULL;


    /*ANSC_HANDLE bus_handle;*/

    /*CCSP_Message_Bus_Init("ccsp.sbclient", CCSP_MSG_BUS_CFG, &bus_handle, Ansc_AllocateMemory_Callback, Ansc_FreeMemory_Callback);*/
    /*CCSP_Msg_SleepInMilliSeconds(1000);*/
    /*CCSP_Message_Bus_Register_Path(bus_handle, msg_path, path_message_func, 0);*/
	 /* Ticket RDKB-39090 purpose */
     _ansc_sprintf(cr_id, "%s%s", pSubsystem, CCSP_DBUS_INTERFACE_CR);

    for (i = 0; pSubSysPrefixList[i] != NULL; i++)
    {
        ret = CcspBaseIf_discComponentSupportingNamespace 
            (
                bus_handle,
                cr_id,
                val->parameterName,
                pSubSysPrefixList[i],
                &ppComponents,
                &size1
            );

        if(ret == CCSP_SUCCESS )
        {
            break;
        }
    }

    if ( ret == CCSP_SUCCESS )
    {
        dst_componentid = ppComponents[0]->componentName;
        dst_pathname    = ppComponents[0]->dbusPath;
    }
    else
    {
        goto EXIT1;
    }
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(!strcmp(bus_info->component_id,dst_componentid))
    {
        same_component=1;
        goto EXIT1;
    }
    ret = CcspBaseIf_getParameterValues(
        bus_handle,
        dst_componentid,
        dst_pathname,
        &(val->parameterName),
        1,
        &size2,
        &parameterVal
    );

    if ( ret == CCSP_SUCCESS  && size2 >= 1)
    {
        unsigned long len = strlen(parameterVal[0]->parameterValue);

        if (len >= *parameterValueLength)
        {
            // AnscTraceWarning(("COSAGetParamValueByPathName: buf size error '%s'\n", val->parameterName));
            memcpy(val->parameterValue, parameterVal[0]->parameterValue, *parameterValueLength - 1);
            val->parameterValue[*parameterValueLength - 1] = 0;
        }
        else
        {
            memcpy(val->parameterValue, parameterVal[0]->parameterValue, len + 1);
        }

        *parameterValueLength = len;
    }

EXIT1:
    if(ppComponents)
    free_componentStruct_t(bus_handle, size1, ppComponents); 
    if(parameterVal)
    free_parameterValStruct_t(bus_handle, size2, parameterVal);

    /*CCSP_Message_Bus_Exit(bus_handle);*/
    if(same_component)
    {
       CcspTraceDebug(("%s:both source and destination are same component\n",__FUNCTION__));
       return ANSC_STATUS_DISCARD;
    }
    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    prototype:

        void*
        COSASetParamValue
            (
                parameterValStruct_t       val,
            )

    description:

        This function is called to retrieve a parameter value;

    argument:   
            char*                       pParamName
            The full name of the parameter;

    return:     the parameter value handle;

**********************************************************************/
ANSC_STATUS
COSASetParamValueByPathName
    (
        void*                      bus_handle,
        parameterValStruct_t       *val
    )
{
    char *pSubSysPrefixList[] = {"eRT.", "eMG.", "", NULL};
    char cr_id[256];
    char * dst_componentid =  NULL;
    char * dst_pathname    =  NULL;
    char * pSubsystem = COSAGetSubsystemPrefix2();
    char * pFaultParameter = NULL;

    int i, size;
    int ret = 0;
    errno_t rc = -1;

    componentStruct_t ** ppComponents = NULL;

    ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;

    /*ANSC_HANDLE bus_handle;*/

    /*CCSP_Message_Bus_Init("ccsp.sbclient", CCSP_MSG_BUS_CFG, &bus_handle, Ansc_AllocateMemory_Callback, Ansc_FreeMemory_Callback);*/
    /*CCSP_Msg_SleepInMilliSeconds(1000);*/
    /*CCSP_Message_Bus_Register_Path(bus_handle, msg_path, path_message_func, 0);*/

    rc = sprintf_s(cr_id, sizeof(cr_id), "%s%s", pSubsystem, CCSP_DBUS_INTERFACE_CR);
    if(rc < EOK)
    {
       ERR_CHK(rc);
    }

    for (i = 0; pSubSysPrefixList[i] != NULL; i++)
    {
        ret = CcspBaseIf_discComponentSupportingNamespace 
            (
                bus_handle,
                cr_id,
                val->parameterName,
                pSubSysPrefixList[i],
                &ppComponents,
                &size
            );

        if(ret == CCSP_SUCCESS )
        {
            AnscTraceWarning(("DiscoverComponent to support '%s'\n", val->parameterName));
            break;
        }
    }

    if ( ret == CCSP_SUCCESS )
    {
        dst_componentid = ppComponents[0]->componentName;
        dst_pathname    = ppComponents[0]->dbusPath;
    }
    else
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto EXIT1;
    }

    ret = CcspBaseIf_setParameterValues(
                bus_handle,
                dst_componentid,
                dst_pathname,
                0,
                0xFFFFFFFF,
                val,
                1,
                1,
                &pFaultParameter
            );

    if ( CCSP_SUCCESS != ret )
    {
        AnscTraceWarning(("SetValue failed with %d\n", ret));
        returnStatus = ANSC_STATUS_BAD_NAME;
    }

EXIT1:
    if (ppComponents)
    {
    free_componentStruct_t(bus_handle, size, ppComponents); 
    }

    /*CCSP_Message_Bus_Exit(bus_handle);*/

    return  returnStatus;
}

/**********************************************************************

    prototype:

        ANSC_STATUS
        COSAGetInstanceNumbers
            (
                char*                      pObjName,
                ULONG*                     pInsList,
                ULONG*                     pInsCnt
            );

    description:

        This function is called to retrieve the instance numbers for an object

    argument:   char*                      pObjName,
                The full object name;
                ULONG*                     pInsList,
                For output, instance number list.
                ULONG*                     pInsCnt,
                [IN/OUT] list size as input, and real instance cnt as output.

    return:     ANSC_STATUS_SUCCESS if ok, and ANSC_STATUS_FAILURE on error

**********************************************************************/
ANSC_STATUS
COSAGetInstanceNumbers
    (
        char*                      pObjName,
        ULONG*                     pInsList,
        ULONG*                     pInsCnt
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController  = (PDSLH_CPE_CONTROLLER_OBJECT)g_DslhDataModelAgent->hDslhCpeController;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase    = (PDSLH_WMP_DATABASE_OBJECT  )pDslhCpeController->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf          = (PDSLH_MPR_INTERFACE        )pDslhWmpDatabase->hDslhMprIf;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord          = (PDSLH_OBJ_RECORD_OBJECT    )NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity          = (PDSLH_OBJ_ENTITY_OBJECT    )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pChildObjRecord     = (PDSLH_OBJ_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = (PSINGLE_LINK_ENTRY     )NULL;
    ULONG                           i                   = 0;

    if( pObjName == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pObjRecord =
        (PDSLH_OBJ_RECORD_OBJECT)pDslhMprIf->GetObjRecord
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                pObjName
            );

    if( pObjRecord == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjRecord->hDslhObjEntity;

    if( pObjEntity == NULL || pObjEntity->ObjDescr->Type != DSLH_CWMP_OBJECT_TYPE_table)
    {
        return ANSC_STATUS_FAILURE;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pObjRecord->ObjroQueue);

    while ( pSLinkEntry && i < *pInsCnt)
    {
        pChildObjRecord = ACCESS_DSLH_OBJ_RECORD_OBJECT(pSLinkEntry);

        if ( pChildObjRecord != NULL)
        {
            pInsList[i] = _ansc_atoi(pChildObjRecord->LastName);
            i ++;
        }

        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);
    }

    *pInsCnt = i;

    return ANSC_STATUS_SUCCESS;
}


