/*
* If not stated otherwise in this file or this component's Licenses.txt file the
* following copyright and licenses apply:
*
* Copyright 2019 RDK Management
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

        For Dbus base library Implementation,
        Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    environment:

            platform dependent

     ---------------------------------------------------------------

     author:

            Harikrishnan Raman

     ---------------------------------------------------------------

     revision:

            11/22/2019    initial revision.

**********************************************************************/
#ifndef CCSP_RBUS_INTERNAL_H
#define CCSP_RBUS_INTERNAL_H
#include <rbus/rbuscore.h>
#include <rbus/rbus_filter.h>
#include <rbus/rbus.h>
#include <rbus/rbus_value.h>

#define RBUS_RETURN_CODE_SUCCESS 0
#define RBUS_RETURN_CODE_FAILURE 1

typedef enum dataType_e* pdataType_e;

int CcspBaseIf_evt_callback_rbus(const char * object_name,  const char * event_name, rbusMessage message, void * user_data);
void ccsp_handle_rbus_component_reply (void* bus_handle, rbusMessage msg, rbusValueType_t typeVal, pdataType_e pType, char** pStringValue);
unsigned int get_writeid(const char *str);
char* writeid_to_string(unsigned int writeid);
unsigned int string_to_writeid(const char *str);
#endif /* CCSP_RBUS_INTERNAL_H */
