//#include "ssp_global.h"
#include "stdlib.h"
#include "ccsp_dm_api.h"
#include "ccsp_base_api.h"
#include "ansc_platform.h"
#include "slap_definitions.h"
//void WriteLog(char *msg)
void WriteLog(char *msg, ANSC_HANDLE bus_handle, char *Subsytem, char *pParameterName)
{
parameterValStruct_t        val1[1] = {0};
parameterValStruct_t        *val = val1;
char*                       faultParam = NULL;
int                         nResult = CCSP_SUCCESS;
char   dst_pathname_cr[64]  =  {0};
/*extern ANSC_HANDLE bus_handle;*/
char                            l_Subsystem[32]         = {0};
int                         ret;
int                         size = 0;
componentStruct_t **        ppComponents = NULL;
char log[300] = {0};
char paramName[100] = {0};
strcpy(l_Subsystem, Subsytem);
strcpy(paramName, pParameterName);
sprintf(dst_pathname_cr, "%s%s", l_Subsystem, CCSP_DBUS_INTERFACE_CR);

       /* ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
                    dst_pathname_cr,
                    "Device.Wifilog.",
                    l_Subsystem,        
                    &ppComponents,
                    &size);*/
		ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
                    dst_pathname_cr,
                    paramName,
                    l_Subsystem,        /* prefix */
                    &ppComponents,
                    &size);

        if ( ret == CCSP_SUCCESS && size == 1)
        {
	
			parameterValStruct_t val[1] = { { paramName, "hello", ccsp_string} }; 
					strcpy(log,msg);
					printf("Log = %s\n",log);
					val[0].parameterValue = log;
            ret = CcspBaseIf_setParameterValues
				(
					bus_handle, 
					ppComponents[0]->componentName, 
					ppComponents[0]->dbusPath,
					0, 0x0,   /* session id and write id */
					&val, 
					1, 
					TRUE,   /* no commit */
					&faultParam
				);	
                
                if (ret != CCSP_SUCCESS && faultParam)
                {
                    AnscTraceError(("Error:Failed to SetValue for param '%s'\n", faultParam));
                //    bus_handle->freefunc(faultParam);
                }

            free_componentStruct_t(bus_handle, size, ppComponents);
        }

}
int GetLogInfo(ANSC_HANDLE bus_handle, char *Subsytem, char *pParameterName)
{
parameterValStruct_t        val1[1] = {0};
parameterValStruct_t        *val = val1;
char*                       faultParam = NULL;
int                         nResult = CCSP_SUCCESS;
char   dst_pathname_cr[64]  =  {0};
/*extern ANSC_HANDLE bus_handle;*/
char                            l_Subsystem[32]         = {0};
int                         ret;
int                         size = 0;
int                         out = 0;
componentStruct_t **        ppComponents = NULL;
char log[300] = {0};
char paramName[100] = {0};
char *p = &paramName;
strcpy(l_Subsystem, Subsytem);
strcpy(paramName, pParameterName);
sprintf(dst_pathname_cr, "%s%s", l_Subsystem, CCSP_DBUS_INTERFACE_CR);

		ret = CcspBaseIf_discComponentSupportingNamespace(bus_handle,
                    dst_pathname_cr,
                    paramName,
                    l_Subsystem,       
                    &ppComponents,
                    &size);

        if ( ret == CCSP_SUCCESS && size == 1)
        {
	    char *parameterNames[1];

	int val_size = 0;
	parameterValStruct_t **parameterval = NULL;

	parameterNames[0] = p;

	
            ret = CcspBaseIf_getParameterValues
				(
					bus_handle, 
					ppComponents[0]->componentName, 
					ppComponents[0]->dbusPath,
					parameterNames,//paramName,
					1,
					&val_size, 
					&parameterval
				);	

                if (ret != CCSP_SUCCESS)
                {
					printf("Error:Failed to SetValue for param");

                }
			if(val_size > 0)
			{
				if(!strcmp(parameterval[0]->parameterValue,"true"))
				{
					out = TRUE;
				}
				else if(!strcmp(parameterval[0]->parameterValue,"false"))
				{
					out = FALSE;
				}
				else
				{
					out = atoi(parameterval[0]->parameterValue);
				}
		
			free_parameterValStruct_t (bus_handle, val_size, parameterval);
			}


            free_componentStruct_t(bus_handle, size, ppComponents);
        }

		return out;
}
