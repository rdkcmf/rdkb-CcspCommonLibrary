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

    module:	dslh_wmpdo_mprif_priv.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Venka Gade

    ---------------------------------------------------------------

    revision:

        05/01/2014    initial revision.

**********************************************************************/

#include "ansc_platform.h"
#include "dslh_definitions.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
		DslhWmpdoMprRegObject_priv_CheckParameterUnderDevice
            (
    			const PDSLH_CWMP_OBJECT_DESCR pObjectDescr
            );

    description:

        This function is called to from DslhWmpdoMprRegObject.

    argument:   PDSLH_CWMP_OBJECT_DESCR			pObjectDescr
                This Object Descriptor.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhWmpdoMprRegObject_priv_CheckParameterUnderDevice
    (
    	PDSLH_CWMP_OBJECT_DESCR pObjectDescr
    )
{
    UNREFERENCED_PARAMETER(pObjectDescr);
	return ANSC_STATUS_BAD_NAME;
}
