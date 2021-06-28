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

    MODULE: pkcs12_utility_export_api.h

        The PKCS12 Utility functions for PKCS12 Handling

    ---------------------------------------------------------------

    DESCRIPTION:

        This interface is for the entity of PKI implementation.

        The entity is responsible for pkcs12 encode/decode implementation.

    ---------------------------------------------------------------

    ENVIRONMENT:

        platform independent

    ---------------------------------------------------------------

    AUTHOR:

        Bin Zhu

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   01/17/2003  initial revision

 **********************************************************************/


#ifndef  _PKCS12_UTILITY_EXPORT_API_H
#define  _PKCS12_UTILITY_EXPORT_API_H

ANSC_HANDLE
AnscCreatePKCS12Utility
    (
        ANSC_HANDLE                 hContainer,
        ANSC_HANDLE                 hReserved
    );

#endif  /*_PKCS12_UTILITY_EXPORT_API_H*/


