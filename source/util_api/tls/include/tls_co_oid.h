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

    module:	tls_co_oid.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object ids for the TLS
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


#ifndef  _TLS_CO_OID_
#define  _TLS_CO_OID_


/***********************************************************
            GENERAL TLS FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  TLS_FEATURE_OBJECT_OID_BASE                TLS_COMPONENT_OID_BASE         + 0x1000
#define  TLS_GENERAL_FO_OID_BASE                    TLS_FEATURE_OBJECT_OID_BASE    + 0x0000

#define  TLS_ENV_CONTROLLER_OID                     TLS_GENERAL_FO_OID_BASE        + 0x0001
#define  TLS_CRYPTO_PROVIDER_OID                    TLS_GENERAL_FO_OID_BASE        + 0x0002
#define  TLS_SESSION_MANAGER_OID                    TLS_GENERAL_FO_OID_BASE        + 0x0003
#define  TLS_SESSION_ENTRY_OID                      TLS_GENERAL_FO_OID_BASE        + 0x0004
#define  TLS_CONN_CONTROLLER_OID                    TLS_GENERAL_FO_OID_BASE        + 0x0005
#define  TLS_RECORD_KEEPER_OID                      TLS_GENERAL_FO_OID_BASE        + 0x0006
#define  TLS_RECORD_CLIENT_OID                      TLS_GENERAL_FO_OID_BASE        + 0x0007

#define  TLS_HAND_SHAKER_OID                        TLS_GENERAL_FO_OID_BASE        + 0x0011
#define  TLS_ALERT_HANDLER_OID                      TLS_GENERAL_FO_OID_BASE        + 0x0012
#define  TLS_CIPHER_SWITCH_OID                      TLS_GENERAL_FO_OID_BASE        + 0x0013
#define  TLS_APP_CONNECTOR_OID                      TLS_GENERAL_FO_OID_BASE        + 0x0014

#define  TLS_HSO_CLIENT_OID                         TLS_GENERAL_FO_OID_BASE        + 0x0021
#define  TLS_HSO_SERVER_OID                         TLS_GENERAL_FO_OID_BASE        + 0x0022


#endif
