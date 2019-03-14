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

    module:	sce_broker_type.h

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object types for the Sce
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


#ifndef  _SCE_BROKER_TYPE_
#define  _SCE_BROKER_TYPE_


/***********************************************************
          GENERAL SCE-BASED BROKER OBJECT DEFINITION
***********************************************************/

/*
 * Define the object types for all the broker objects, each of them provides the provisioning,
 * configuration and monitoring interface on behalf of the underlying Slee Object. While not
 * every slee object should be exposed to the provisioning server, we list all of them here for
 * completeness.
 */
#define  SCE_BROKER_NSM_TYPE                        "nsm"
#define  SCE_BROKER_SLEE_CONTAINER_TYPE             "sleeContainer"

#define  SCE_BROKER_SLEE_LCO_TYPE                   "sleeLco"
#define  SCE_BROKER_SLEE_CEN_LCO_TYPE               "sleeCenLco"
#define  SCE_BROKER_SLEE_DIS_LCO_TYPE               "sleeDisLco"

#define  SCE_BROKER_SLEE_SMO_TYPE                   "sleeSmo"
#define  SCE_BROKER_SLEE_SMORO_TYPE                 "sleeSmoRo"
#define  SCE_BROKER_SLEE_SMOPO_TYPE                 "sleeSmoPo"

#define  SCE_BROKER_SLEE_SSO_TYPE                   "sleeSso"
#define  SCE_BROKER_SLEE_SLO_TYPE                   "sleeSlo"
#define  SCE_BROKER_SLEE_SPO_TYPE                   "sleeSpo"
#define  SCE_BROKER_SLEE_SBO_TYPE                   "sleeSbo"
#define  SCE_BROKER_SLEE_SFO_TYPE                   "sleeSfo"
#define  SCE_BROKER_SLEE_PCO_TYPE                   "sleePco"
#define  SCE_BROKER_SLEE_SOO_TYPE                   "sleeSoo"


#endif
