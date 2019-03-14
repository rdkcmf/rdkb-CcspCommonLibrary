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

    module:	tls_co_name.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object names for the TLS
        Component Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/16/03    initial revision.

**********************************************************************/


#ifndef  _TLS_CO_NAME_
#define  _TLS_CO_NAME_


/***********************************************************
            GENERAL TLS FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  TLS_ENV_CONTROLLER_NAME                    "envController"
#define  TLS_CRYPTO_PROVIDER_NAME                   "cryptoProvider"
#define  TLS_SESSION_MANAGER_NAME                   "sessionManager"
#define  TLS_SESSION_ENTRY_NAME                     "sessionEntry"
#define  TLS_CONN_CONTROLLER_NAME                   "connController"
#define  TLS_RECORD_KEEPER_NAME                     "recordKeeper"
#define  TLS_RECORD_CLIENT_NAME                     "recordClient"

#define  TLS_HAND_SHAKER_NAME                       "handShaker"
#define  TLS_ALERT_HANDLER_NAME                     "alertHandler"
#define  TLS_CIPHER_SWITCH_NAME                     "cipherSwitch"
#define  TLS_APP_CONNECTOR_NAME                     "appConnector"

#define  TLS_HSO_CLIENT_NAME                        "hsoClient"
#define  TLS_HSO_SERVER_NAME                        "hsoServer"


#endif
