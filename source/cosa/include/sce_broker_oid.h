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

    module:	sce_broker_oid.h

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object ids for the Sce
        Broker Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/20/01    initial revision.

**********************************************************************/


#ifndef  _SCE_BROKER_OID_
#define  _SCE_BROKER_OID_


/***********************************************************
          GENERAL SCE-BASED BROKER OBJECT DEFINITION
***********************************************************/

/*
 * Define the object oids for all the broker objects, each of them provides the provisioning,
 * configuration and monitoring interface on behalf of the underlying Slee Object. While not
 * every slee object should be exposed to the provisioning server, we list all of them here for
 * completeness.
 */
#define  SCE_BROKER_OBJECT_OID_BASE                 SCE_COMPONENT_OID_BASE          + 0xF000
#define  SCE_BROKER_NSM_OID                         SCE_BROKER_OBJECT_OID_BASE      + 0x0001
#define  SCE_BROKER_SLEE_CONTAINER_OID              SCE_BROKER_OBJECT_OID_BASE      + 0x0002

#define  SCE_BROKER_SLEE_LCO_OID                    SCE_BROKER_OBJECT_OID_BASE      + 0x0003
#define  SCE_BROKER_SLEE_CEN_LCO_OID                SCE_BROKER_OBJECT_OID_BASE      + 0x0004
#define  SCE_BROKER_SLEE_DIS_LCO_OID                SCE_BROKER_OBJECT_OID_BASE      + 0x0005

#define  SCE_BROKER_SLEE_SMO_OID                    SCE_BROKER_OBJECT_OID_BASE      + 0x0006
#define  SCE_BROKER_SLEE_SMORO_OID                  SCE_BROKER_OBJECT_OID_BASE      + 0x0007
#define  SCE_BROKER_SLEE_SMOPO_OID                  SCE_BROKER_OBJECT_OID_BASE      + 0x0008

#define  SCE_BROKER_SLEE_SSO_OID                    SCE_BROKER_OBJECT_OID_BASE      + 0x0009
#define  SCE_BROKER_SLEE_SLO_OID                    SCE_BROKER_OBJECT_OID_BASE      + 0x000A
#define  SCE_BROKER_SLEE_SPO_OID                    SCE_BROKER_OBJECT_OID_BASE      + 0x000B
#define  SCE_BROKER_SLEE_SBO_OID                    SCE_BROKER_OBJECT_OID_BASE      + 0x000C
#define  SCE_BROKER_SLEE_SFO_OID                    SCE_BROKER_OBJECT_OID_BASE      + 0x000D
#define  SCE_BROKER_SLEE_PCO_OID                    SCE_BROKER_OBJECT_OID_BASE      + 0x000E
#define  SCE_BROKER_SLEE_SOO_OID                    SCE_BROKER_OBJECT_OID_BASE      + 0x000F


#endif
