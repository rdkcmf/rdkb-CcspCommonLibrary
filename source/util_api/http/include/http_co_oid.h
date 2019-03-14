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

    module:	http_co_oid.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object ids for the Http
        Component Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/20/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_CO_OID_
#define  _HTTP_CO_OID_


/***********************************************************
            GENERAL HTTP FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  HTTP_FEATURE_OBJECT_OID_BASE               HTTP_COMPONENT_OID_BASE         + 0x1000
#define  HTTP_GENERAL_FO_OID_BASE                   HTTP_FEATURE_OBJECT_OID_BASE    + 0x0000

#define  HTTP_SIMPLE_CLIENT_OID                     HTTP_GENERAL_FO_OID_BASE        + 0x0001
#define  HTTP_SIMPLE_SERVER_OID                     HTTP_GENERAL_FO_OID_BASE        + 0x0002
#define  HTTP_SIMPLE_PROXY_OID                      HTTP_GENERAL_FO_OID_BASE        + 0x0003
#define  HTTP_PROXY_SESSION_OID                     HTTP_GENERAL_FO_OID_BASE        + 0x0004

#define  HTTP_ADVANCED_PROXY_OID                    HTTP_GENERAL_FO_OID_BASE        + 0x0005
#define  HTTP_PSO_VER2_OID                          HTTP_GENERAL_FO_OID_BASE        + 0x0006
#define  HTTP_TRANS_RECORD_OID                      HTTP_GENERAL_FO_OID_BASE        + 0x0007

#define  HTTP_WEBS_SESSION_OID                      HTTP_GENERAL_FO_OID_BASE        + 0x0008
#define  HTTP_WEBS_TRANS_OID                        HTTP_GENERAL_FO_OID_BASE        + 0x0009
#define  HTTP_WEBC_SESSION_OID                      HTTP_GENERAL_FO_OID_BASE        + 0x000A
#define  HTTP_WEBC_TRANS_OID                        HTTP_GENERAL_FO_OID_BASE        + 0x000B


/***********************************************************
          GENERAL HTTP EXTENSION OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Extension Objects that cannot be categorized. Extension
 * Objects are integral parts of underlying execution environment, and are always created during
 * initialization.
 */
#define  HTTP_EXTENSION_OBJECT_OID_BASE             HTTP_COMPONENT_OID_BASE         + 0x8000
#define  HTTP_GENERAL_EO_OID_BASE                   HTTP_EXTENSION_OBJECT_OID_BASE  + 0x0100

#define  HTTP_BASIC_MESSAGE_OID                     HTTP_GENERAL_EO_OID_BASE        + 0x0001
#define  HTTP_BMO_REP_OID                           HTTP_GENERAL_FO_OID_BASE        + 0x0002
#define  HTTP_BMO_REQ_OID                           HTTP_GENERAL_FO_OID_BASE        + 0x0003

#define  HTTP_FORM_CONTENT_OID                      HTTP_GENERAL_FO_OID_BASE        + 0x0011
#define  HTTP_MESSAGE_BODY_OID                      HTTP_GENERAL_FO_OID_BASE        + 0x0012
#define  HTTP_MBO_IDENTITY_OID                      HTTP_GENERAL_FO_OID_BASE        + 0x0013
#define  HTTP_MBO_CHUNKED_OID                       HTTP_GENERAL_FO_OID_BASE        + 0x0014
#define  HTTP_MBO_GZIPPED_OID                       HTTP_GENERAL_FO_OID_BASE        + 0x0015
#define  HTTP_MBO_COMPRESSED_OID                    HTTP_GENERAL_FO_OID_BASE        + 0x0016
#define  HTTP_MBO_DEFLATED_OID                      HTTP_GENERAL_FO_OID_BASE        + 0x0017
#define  HTTP_MBO_FILE_OID                          HTTP_GENERAL_FO_OID_BASE        + 0x0018

#define  HTTP_ATO_STATUS_OID                        HTTP_GENERAL_FO_OID_BASE        + 0x0020
#define  HTTP_ATO_HEADER_OID                        HTTP_GENERAL_FO_OID_BASE        + 0x0021
#define  HTTP_ATO_METHOD_OID                        HTTP_GENERAL_FO_OID_BASE        + 0x0022
#define  HTTP_ATO_CGIENV_OID                        HTTP_GENERAL_FO_OID_BASE        + 0x0023
#define  HTTP_ATO_MCTYPE_OID                        HTTP_GENERAL_FO_OID_BASE        + 0x0024
#define  HTTP_ATO_SCTYPE_OID                        HTTP_GENERAL_FO_OID_BASE        + 0x0025
#define  HTTP_ATO_FXCAT1_OID                        HTTP_GENERAL_FO_OID_BASE        + 0x0026
#define  HTTP_ATO_FXCAT2_OID                        HTTP_GENERAL_FO_OID_BASE        + 0x0027

#define  HTTP_AUTHO_OID                             HTTP_GENERAL_FO_OID_BASE        + 0x0030
#define  HTTP_AUTHCO_OID                            HTTP_GENERAL_FO_OID_BASE        + 0x0031
#define  HTTP_AUTHSO_OID                            HTTP_GENERAL_FO_OID_BASE        + 0x0032


#endif
