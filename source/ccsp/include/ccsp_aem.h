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

/***********************************************************
  Meta information on Application Environment (AE)
  Execution Environment (EE) is alternative name for AE.
  Note AE and EE will be used interchangeably in scope of
  CCSP application environment manager.
***********************************************************/
#define  CCSP_AEM_EE_INFO_MAX_NAME_LEN                 32
#define  CCSP_AEM_EE_INFO_MAX_ALIAS_LEN                64
#define  CCSP_AEM_EE_INFO_TYPE_MAX_LEN                 64
#define  CCSP_AEM_EE_INFO_MAX_VENDOR_LEN               128
#define  CCSP_AEM_EE_INFO_MAX_VERSION_LEN              32
#define  CCSP_AEM_EE_INFO_IMG_SUF_MAX_LEN              64

typedef  struct
_CCSP_AEM_EE_INFO
{
/*----------------------------------------------------------------------------
 * The following information is provided and maintianed by each Application 
 * Environment (AE).
 ----------------------------------------------------------------------------*/

    /* Name - A Name provided by the CPE that adequately distinguishes this 
     * ExecEnv from all other ExecEnv instances. 
     */
    char                       Name      [CCSP_AEM_EE_INFO_MAX_NAME_LEN    +1];
    /* Type - Indicates the complete type and specification version of this ExecEnv.
     */
    char                       Type      [CCSP_AEM_EE_INFO_TYPE_MAX_LEN    +1];
    /* Vendor - The vendor that produced this ExecEnv.
     */
    char                       Vendor    [CCSP_AEM_EE_INFO_MAX_VENDOR_LEN  +1];
    /* Version - The Version of this ExecEnv as specified by the Vendor that 
     * implemented this ExecEnv, not the version of the specification.
     */
    char                       Version   [CCSP_AEM_EE_INFO_MAX_VERSION_LEN +1];
    /* ParentName - the name of parent execution environment. 
     */
    char                       ParentName[CCSP_AEM_EE_INFO_MAX_NAME_LEN    +1];
    /* image file extensions that this EE may recognize
     */
    char                       ImgExts   [CCSP_AEM_EE_INFO_IMG_SUF_MAX_LEN +1];

/*----------------------------------------------------------------------------
 * The following information is provided and maintained by AEM
 ----------------------------------------------------------------------------*/

    /* 
     * Alias provides a mechanism for an ACS to label this instance for future reference. 
     * Alias can be changed by cloud server e.g. ACS, AEM is responsible to 
     * associate it with specific EE. Required by TR-069 spec, the alias has to 
     * be persistent over reboot.
     */
    char                       Alias     [CCSP_AEM_EE_INFO_MAX_ALIAS_LEN   +1];
}
CCSP_AEM_EE_INFO, *PCCSP_AEM_EE_INFO;


/***********************************************************
  Deployment Unit (DU) meta info
***********************************************************/

#define  CCSP_AEM_DU_META_INFO_UUID_MAX_LEN         36
#define  CCSP_AEM_DU_META_INFO_DUID_MAX_LEN         64
#define  CCSP_AEM_DU_META_INFO_MAX_NAME_LEN         64
#define  CCSP_AEM_DU_META_INFO_MAX_ALIAS_LEN        64
#define  CCSP_AEM_DU_META_INFO_URL_MAX_LEN          1024
#define  CCSP_AEM_DU_META_INFO_DESC_MAX_LEN         256
#define  CCSP_AEM_DU_META_INFO_MAX_VENDOR_LEN       128
#define  CCSP_AEM_DU_META_INFO_MAX_VERSION_LEN      32

typedef  struct
_CCSP_AEM_DU_META_INFO
{
/*----------------------------------------------------------------------------
 * The following information is provided and maintianed by each Application 
 * Environment (AE) on the specified DeploymentUnit (DU)
 ----------------------------------------------------------------------------*/

    /* UUID - A Universally Unique Identifier either provided by the ACS, or generated 
     * by the CPE, at the time of Deployment Unit Installation. The format of this value 
     * is defined by [RFC4122] Version 3 (Name-Based) and [Annex H/TR-069a3].
     * This value MUST NOT be altered when the DeploymentUnit is updated.
     */
     char                      UUID      [CCSP_AEM_DU_META_INFO_UUID_MAX_LEN    +1];
    /* DUID - Deployment Unit Identifier chosen by the targeted ExecEnv. 
     * The format of this value is Execution Environment specific.
     */
    char                       DUID      [CCSP_AEM_DU_META_INFO_DUID_MAX_LEN    +1];
    /* Name - Indicates the Name of this DeploymentUnit, which is chosen by 
     * the author of the Deployment Unit. 
     */
    char                       Name      [CCSP_AEM_DU_META_INFO_MAX_NAME_LEN    +1];
    /* URL - Contains the URL used by the most recent ChangeDUState RPC to either 
     * Install or Update this DeploymentUnit. 
     */
    char                       URL       [CCSP_AEM_DU_META_INFO_URL_MAX_LEN    +1];   
    /* Desc - Description of this DeploymentUnit */
    char                       Desc      [CCSP_AEM_DU_META_INFO_DESC_MAX_LEN    +1];
    /* Vendor - the author of this DeploymentUnit formatted as a domain name.
     * The value of this parameter is used in the generation of the UUID based 
     * on the rules defined in [Annex H/TR-069a3].
     */
    char                       Vendor    [CCSP_AEM_DU_META_INFO_MAX_VENDOR_LEN  +1];
    /* Version - Version of this DeploymentUnit. The format of this value is 
     * Execution Environment specific. 
     */
    char                       Version   [CCSP_AEM_DU_META_INFO_MAX_VERSION_LEN +1];
    /* EeName - the name of application environment on which the package was installed */
    char                       EeName    [CCSP_AEM_DU_META_INFO_MAX_NAME_LEN    +1];   


/*----------------------------------------------------------------------------
 * The following information is provided and maintained by AEM
 ----------------------------------------------------------------------------*/

    /* 
     * Alias provides a mechanism for an ACS to label this instance for future reference. 
     * Alias can be changed by cloud server e.g. ACS, AEM is responsible to 
     * associate it with UUID/DUID. Required by TR-069 spec, the alias has to 
     * be persistent over reboot. Application environment adaptor does not need to
     * be aware of alias.
     */
    char                       Alias     [CCSP_AEM_DU_META_INFO_MAX_ALIAS_LEN   +1];
}
CCSP_AEM_DU_META_INFO, *PCCSP_AEM_DU_META_INFO;

/***********************************************************
  Execution Unit (DU) meta info
***********************************************************/
#define  CCSP_AEM_EU_META_INFO_EUID_MAX_LEN         64
#define  CCSP_AEM_EU_META_INFO_MAX_NAME_LEN         32
#define  CCSP_AEM_EU_META_INFO_MAX_LABEL_LEN        64
#define  CCSP_AEM_EU_META_INFO_MAX_ALIAS_LEN        64
#define  CCSP_AEM_EU_META_INFO_MAX_VENDOR_LEN       128
#define  CCSP_AEM_EU_META_INFO_MAX_VERSION_LEN      32
#define  CCSP_AEM_EU_META_INFO_DESC_MAX_LEN         256

typedef  struct
_CCSP_AEM_EU_META_INFO
{
/*----------------------------------------------------------------------------
 * The following information is provided and maintianed by each Application 
 * Environment (AE).
 ----------------------------------------------------------------------------*/

    /* EUID - Execution Unit Identifier chosen by the ExecEnv during installation 
     * of the associated DeploymentUnit. The format of this value is Execution 
     * Environment specific, but it MUST be unique across ExecEnv instances. 
     * Thus, it is recommended that this be a combination of the Execution Environment
     * Name and an Execution Environment local unique value.
     */
    char                       EUID      [CCSP_AEM_EU_META_INFO_EUID_MAX_LEN    +1];
    /* Name - The name of this ExecutionUnit as it pertains to its associated 
     * DeploymentUnit, which SHOULD be unique across all ExecutionUnit instances 
     * contained within its associated DeploymentUnit.     
     */
    char                       Name      [CCSP_AEM_EU_META_INFO_MAX_NAME_LEN    +1];
    /* EeLabel - The name of this ExecutionUnit as provided by the ExecEnv, which 
     * SHOULD be unique across all ExecutionUnit instances contained within a specific 
     * ExecEnv.
     */
    char                       EeLabel   [CCSP_AEM_EU_META_INFO_MAX_LABEL_LEN   +1];
    /* Vendor - Vendor of this ExecutionUnit. */
    char                       Vendor    [CCSP_AEM_EU_META_INFO_MAX_VENDOR_LEN  +1];
    /* Version - Version of the ExecutionUnit. The format of this value is 
     * Execution Environment specific.
     */
    char                       Version   [CCSP_AEM_EU_META_INFO_MAX_VERSION_LEN +1];
    /* Desc - Textual description of this ExecutionUnit. The format of this value is 
     * Execution Environment specific.
     */
    char                       Desc      [CCSP_AEM_EU_META_INFO_DESC_MAX_LEN    +1];
    /* EeName - the name of EE on which the EU is running or supposedly to run */
    char                       EeName    [CCSP_AEM_EU_META_INFO_MAX_NAME_LEN    +1];   

/*----------------------------------------------------------------------------
 * The following information is provided and maintained by AEM
 ----------------------------------------------------------------------------*/

    /* 
     * Alias provides a mechanism for an ACS to label this instance for future reference. 
     * Alias can be changed by cloud server e.g. ACS, AEM is responsible to 
     * associate it with specific EU. Required by TR-069 spec, the alias has to 
     * be persistent over reboot.
     */
    char                       Alias     [CCSP_AEM_EU_META_INFO_MAX_ALIAS_LEN   +1];
}
CCSP_AEM_EU_META_INFO, *PCCSP_AEM_EU_META_INFO;

/***********************************************************
  EE state
***********************************************************/

#define  CCSP_AEM_EE_STATUS_UP                      1       /* EE is up running */
#define  CCSP_AEM_EE_STATUS_ERROR                   2       /* EE is running in error state */
#define  CCSP_AEM_EE_STATUS_DISABLED                3       /* EE is disabled */

typedef  struct
_CCSP_AEM_EE_STATE
{
    /* Enabled - if EE is enabled or not
     */
    BOOL                   Enabled;
    /* Status - one of CCSP_AEM_EE_STATUS_XXX 
     */
    LONG                       Status;                     
    /* InitialRunLevel - The run level that this ExecEnv will be in upon 
     * startup (whether that is caused by a CPE Boot or the Execution 
     * Environment starting). Run levels dictate which Execution Units 
     * will be started. Execution Units will be started if CurrentRunLevel 
     * is greater than or equal to ExecutionUnit.{i}.RunLevel and 
     * ExecutionUnit.{i}.AutoStart is true. If the value of CurrentRunLevel 
     * is -1, then the value of this parameter is irrelevant when read and 
     * setting its value has no impact on the Run Level of this ExecEnv.
     */
    ULONG                       InitialRunLevel;        /* 0 ~ 65535 */
    /* CurrentRunLevel - The run level that this ExecEnv is currently 
     * operating in. This value is altered by changing the RequestedRunLevel 
     * parameter. Upon startup (whether that is caused by a CPE Boot or 
     * the Execution Environment starting) CurrentRunLevel will be equal 
     * to InitialRunLevel, unless Run Levels are not supported by this 
     * ExecEnv in which case CurrentRunLevel will be -1. Run levels 
     * dictate which Execution Units will be started. Execution Units 
     * will be started if CurrentRunLevel is greater than or equal to 
     * ExecutionUnit.{i}.RunLevel and ExecutionUnit.{i}.AutoStart is true. 
     * If CurrentRunLevel is -1 then Run Levels are not supported by this 
     * ExecEnv and setting InitialRunLevel or RequestedRunLevel will not 
     * impact the Run Level of this ExecEnv.
     */
    LONG                        CurrentRunLevel;        /* -1 ~ 65535 */
}
CCSP_AEM_EE_STATE, *PCCSP_AEM_EE_STATE;


/***********************************************************
  EE resource usage
***********************************************************/

typedef  struct
_CCSP_AEM_EE_RES_USAGE
{
    /* AllocatedDiskSpace - The amount of disk space measured in kilobytes 
     * allocated to this ExecEnv. A value of -1 MUST be used for ExecEnv 
     * instances where this parameter is not applicable.
     */
    LONG                        AllocatedDiskSpace; 
    /* AvailableDiskSpace - The amount of disk space measured in kilobytes 
     * currently available to this ExecEnv. This value changes as the 
     * ExecutionUnit instances associated with this ExecEnv consumes 
     * disk space. A value of -1 MUST be used for ExecEnv instances 
     * where this parameter is not applicable.
     */
    LONG                        AvailableDiskSpace;
    /* AllocatedMemory - The amount of physical RAM measured in kilobytes 
     * allocated to this ExecEnv. A value of -1 MUST be used for ExecEnv 
     * instances where this parameter is not applicable.
     */
    LONG                        AllocatedMemory;
    /* AvailableMemory - The amount of physical RAM measured in kilobytes 
     * currently available to this ExecEnv. This value changes as the 
     * ExecutionUnit instances associated with this ExecEnv are 
     * started/stopped and consume the physical RAM. A value of -1 MUST 
     * be used for ExecEnv instances where this parameter is not applicable.
     */
    LONG                        AvailableMemory;
}
CCSP_AEM_EE_RES_USAGE, *PCCSP_AEM_EE_RES_USAGE;


/***********************************************************
 * EE info request types
 *
 * Request type that is going to be used as to get or 
 * set properties of EE where applicable. A single IOCTL
 * like Get/Set API is defined. The request type is used
 * to do data type casting. For example, if request type
 * is CCSP_AEM_EE_REQ_TYPE_METAINFO, AEA needs to convert
 * given value handle to PCCSP_AEM_EE_STATE.
***********************************************************/

/* meta information retrieval, value handle must be cast to PCCSP_AEM_EE_INFO 
 */
#define  CCSP_AEM_EE_REQ_TYPE_METAINFO              1       

/* state retrieval, value handle must be cast to PCCSP_AEM_EE_STATE 
 */
#define  CCSP_AEM_EE_REQ_TYPE_STATE                 2      
 
/* resource usage retrieval, value handle must be cast to PCCSP_AEM_EE_RES_USAGE 
 */
#define  CCSP_AEM_EE_REQ_TYPE_RES_USAGE             3     
  
/* active EU retrieval, value handle must be cast to char **.
 * On successful return, it contains comma-separated string that
 * each represents the name of an active EU.
 */
#define  CCSP_AEM_EE_REQ_TYPE_ACTIVE_EU             4    
   
/* processor reference retrieval, value handle must be cast to char **.
 * On successful return, it contains comma-separated string that
 * each represents the name of architecture of processor on which EE and EUs
 * it manages run.
 */
#define  CCSP_AEM_EE_REQ_TYPE_PROCESSOR_REF         5    
   
/* Enable - only used to change the value. Value handle must be cast
 * to BOOL*. Retrieval this value (false always) should use 
 * CCSP_AEM_EE_REQ_TYPE_STATE instead.
 * 
 * Indicates whether or not this ExecEnv is enabled.
 * Disabling an enabled Execution Environment stops it, while enabling a 
 * disabled Execution Environment starts it. When an Execution Environment 
 * is disabled, Deployment Units installed to that Execution Environment 
 * will be unaffected, but any Execution Units currently running on that 
 * Execution Environment will automatically transition to Idle. If a 
 * ChangeDUState is attempted on a DeploymentUnit that is to be applied 
 * against a disabled ExecEnv, that ChangeDUState operation fails and 
 * the associated DUStateChangeComplete RPC will contain a FaultStruct 
 * for that operation. If a SetParameterValues is attempted against 
 * the ExecutionUnit.{i}.RequestedState for an ExecutionUnit that is 
 * associated with a disabled ExecEnv a CWMP Fault will be issued in 
 * response. Disabling an Execution Environment could place the device 
 * in a non-manageable state. For example, if the operating system itself 
 * was modeled as an ExecEnv and the ACS disabled it, the CWMP management 
 * agent might be terminated leaving the device unmanageable.
 */
#define  CCSP_AEM_EE_REQ_TYPE_ENABLE                6

/* Reset -  this can only be set. value handle must be cast to BOOL*.
 *
 * Setting this parameter to true causes this ExecEnv to revert back to the 
 * state it was in when the device last issued a "0 BOOTSTRAP" Inform event. 
 * The following requirements dictate what MUST happen for the reset to be 
 * complete:
 *   All Deployment Units that were installed after the last "0 BOOTSTRAP" 
 *     Inform event MUST be removed 
 *   All persistent storage, configuration files, and log files that were 
 *     associated with the removed Deployment Units MUST be removed 
 *   Any Deployment Unit that is still installed against the Execution 
 *     Environment MUST be restored to the version present when the last "0 BOOTSTRAP" 
 *     Inform event was issued 
 *   Any Deployment Unit that was present when the last "0 BOOTSTRAP" Inform 
 *     event was issued, but was subsequently uninstalled and is now not present, 
 *     MUST be installed with the version that was present when the last 
 *     "0 BOOTSTRAP" Inform event was issued 
 *   The Execution Environment MUST be restored to the version and configuration 
 *     present when the last "0 BOOTSTRAP" Inform event was issued 
 *   The Execution Environment MUST be restarted after all other restoration 
 *     requirements have been met 
 *   The value of this parameter is not part of the device configuration and 
 *     is always false when read.
 */
#define  CCSP_AEM_EE_REQ_TYPE_RESET                 7

/* InitialRunLevel - only used to change the value. Use CCSP_AEM_EE_REQ_TYPE_STATE
 * to retrieve this value from AEA. Value handle must be cast to ULONG*.
 */
#define  CCSP_AEM_EE_REQ_TYPE_INITIAL_RUNLEVE       8

/* RequestedRunLevel - only used to change the value. Value handle must be cast 
 * to LONG*. When read, this value should always be -1.
 *
 * Provides a mechanism to remotely manipulate the run level of this ExecEnv, 
 * meaning that altering this parameter's value will change the value of the 
 * CurrentRunLevel. Run levels dictate which Execution Units will be started. 
 * Execution Units will be started if CurrentRunLevel is greater than or equal 
 * to ExecutionUnit.{i}.RunLevel and ExecutionUnit.{i}.AutoStart is true.
 * Setting this value when CurrentRunLevel is -1 has no impact to the Run Level 
 * of this ExecEnv. The value of this parameter is not part of the device 
 * configuration and is always -1 when read.
 */
#define  CCSP_AEM_EE_REQ_TYPE_REQUESTED_RUNLEVE     9

/* Parent EE retrieval, value handle must be cast to char **.
 * It will be empty if this is the primary EE.
 */
#define  CCSP_AEM_EE_REQ_TYPE_PARENT_EE             10    


/***********************************************************
  DU state
***********************************************************/

#define  CCSP_AEM_DU_STATUS_INSTALLING              1       /* DU is being installed */
#define  CCSP_AEM_DU_STATUS_INSTALLED               2       /* DU has been installed */
#define  CCSP_AEM_DU_STATUS_UPDATING                3       /* DU is being updated */
#define  CCSP_AEM_DU_STATUS_UNINSTALLING            4       /* DU is being uninstalled */
#define  CCSP_AEM_DU_STATUS_UNINSTALLED             5       /* DU has been uninstalled */

typedef  struct
_CCSP_AEM_DU_STATE
{
    /* Status - one of CCSP_AEM_DU_STATUS_XXX 
     */
    LONG                        Status;                     
    /* Resolved - Indicates whether or not this DeploymentUnit 
     * has resolved all of its dependencies.
     */
    BOOL                       Resolved;
}
CCSP_AEM_DU_STATE, *PCCSP_AEM_DU_STATE;


/***********************************************************
 * DU info request types
 *
 * Request type that is going to be used as to get or 
 * set properties of DU where applicable. A single IOCTL
 * like Get/Set API is defined. The request type is used
 * to do data type casting. 
***********************************************************/

/* meta information retrieval, value handle must be cast to PCCSP_AEM_DU_INFO 
 */
#define  CCSP_AEM_DU_REQ_TYPE_METAINFO              1       

/* state retrieval, value handle must be cast to PCCSP_AEM_DU_STATE 
 */
#define  CCSP_AEM_DU_REQ_TYPE_STATE                 2      
 
/* VendorLogList retrieval, value handle must be cast to char **.
 * 
 * On successful return, value contains a comma-separated list, and each
 * list item is the Alias of vendor log file when CPE creates the
 * corresponding table entry under "Device.DeviceInfo.VendorLogFile.{i}.".
 */
#define  CCSP_AEM_DU_REQ_TYPE_VENDOR_LOGLIST        3     

/* VendorConfigList retrieval, value handle must be cast to char **.
 * 
 * On successful return, value contains a comma-separated list, and each
 * list item is the Name of vendor config file when CPE creates the
 * corresponding table entry under "Device.DeviceInfo.VendorConfigFile.{i}.".
 */
#define  CCSP_AEM_DU_REQ_TYPE_VENDOR_CONFIGLIST     4     
  
  
/* EU list retrieval, value handle must be cast to char **.
 * On successful return, it contains comma-separated string that
 * each represents the name of an EU instance associated with
 * requested DU.
 */
#define  CCSP_AEM_DU_REQ_TYPE_EU_LIST               5
   

/***********************************************************
  EU state
***********************************************************/

#define  CCSP_AEM_EU_STATUS_IDLE                          1       /* EU is idle */
#define  CCSP_AEM_EU_STATUS_STARTING                      2       /* EU is starting */
#define  CCSP_AEM_EU_STATUS_ACTIVE                        3       /* EU is up running */
#define  CCSP_AEM_EU_STATUS_STOPPING                      4       /* EU is stopping */

#define  CCSP_AEM_EU_REQUEST_IDLE                         1       /* Request EU to be idle */
#define  CCSP_AEM_EU_REQUEST_ACTIVE                       2       /* Request EU to be active */

#define  CCSP_AEM_EU_FAULTCODE_NO_FAULT                   0       /* EU is working fine */
#define  CCSP_AEM_EU_FAULTCODE_FAILED_START               1       /* EU failed on start */
#define  CCSP_AEM_EU_FAULTCODE_FAILED_AUTOSTART           2       /* EU failed on auto start */
#define  CCSP_AEM_EU_FAULTCODE_FAILED_STOP                3       /* EU failed to stop */
#define  CCSP_AEM_EU_FAULTCODE_FAILURE_ACTIVE             4       /* EU failed while active */
#define  CCSP_AEM_EU_FAULTCODE_DEPENDENCY_ERROR           5       /* EU has dependency error */
#define  CCSP_AEM_EU_FAULTCODE_UNSTARTABLE                6       /* EU is unstartable */

#define  CCSP_AEM_EU_FAULT_MSG_MAX_LEN                    256

typedef  struct
_CCSP_AEM_EU_STATE
{
    /* Status - one of CCSP_AEM_EU_STATUS_XXX 
     */
    LONG                        Status;                     
    /* ExecutionFaultCode - If while running or transitioning between 
     * states this ExecutionUnit identifies a fault this parameter 
     * embodies the problem. It must be one of CCSP_AEM_EU_FAULTCODE_XXX.
     */
    LONG                        ExecutionFaultCode;
    /* ExecutionFaultMessage - If while running or transitioning between 
     * states this ExecutionUnit identifies a fault this parameter provides 
     * a more detailed explanation of the problem.
     */
    char                       ExecutionFaultMessage[CCSP_AEM_EU_FAULT_MSG_MAX_LEN+1];
    /* AutoStart - If true and the RunLevel verification is also met, then 
     * this ExecutionUnit will be automatically started by the device after 
     * its ExecEnv is either rebooted or restarted. If false this ExecutionUnit 
     * will not be started after its ExecEnv is either rebooted or restarted 
     * until it is explicitly commanded to do so by either the ACS or another 
     * Execution Unit.
     */
    BOOL                       AutoStart;
    /* RunLevel - Determines when this ExecutionUnit will be started.
     * If AutoStart is true and ExecEnv.{i}.CurrentRunLevel is greater 
     * than or equal to RunLevel, then this ExecutionUnit will be started.
     * If the value of ExecEnv.{i}.CurrentRunLevel is -1, then the associated 
     * ExecEnv. doesn't support Run Levels, thus the value of this parameter 
     * is irrelevant when read and setting its value has no impact to the Run 
     * Level of this ExecutionUnit.
     */
    ULONG                       RunLevel;
}
CCSP_AEM_EU_STATE, *PCCSP_AEM_EU_STATE;


/***********************************************************
  EU resource usage
***********************************************************/

typedef  struct
_CCSP_AEM_EU_RES_USAGE
{
    /* DiskSpaceInUse - The amount of disk space measured in kilobytes 
     * currently being used by this ExecutionUnit. A value of -1 MUST be 
     * used for ExecutionUnit instances where this parameter is not 
     * applicable.
     */
    LONG                        DiskSpaceInUse;                     
    /* MemoryInUse - The amount of physical RAM measured in kilobytes 
     * currently being used by this ExecutionUnit. A value of -1 MUST be 
     * used for ExecutionUnit instances where this parameter is not 
     * applicable.
     */
    LONG                        MemoryInUse;                     
}
CCSP_AEM_EU_RES_USAGE, *PCCSP_AEM_EU_RES_USAGE;


/***********************************************************
 * EU info request types
 *
 * Request type that is going to be used as to get or 
 * set properties of EU where applicable. A single IOCTL
 * like Get/Set API is defined. The request type is used
 * to do data type casting. 
***********************************************************/

/* meta information retrieval, value handle must be cast to PCCSP_AEM_EU_META_INFO 
 */
#define  CCSP_AEM_EU_REQ_TYPE_METAINFO              1

/* state retrieval, value handle must be cast to PCCSP_AEM_EU_STATE 
 */
#define  CCSP_AEM_EU_REQ_TYPE_STATE                 2      
 
/* resource usage retrieval, value handle must be cast to PCCSP_AEM_EU_RES_USAGE
 */
#define  CCSP_AEM_EU_REQ_TYPE_RES_USAGE             3
 
/* References retrieval, value handle must be cast to char **.
 * 
 * Comma-separated list of strings. Each list item MUST be the path 
 * name of a table row. If the referenced object is deleted, the 
 * corresponding item MUST be removed from the list. Represents 
 * the instances of multi-instanced objects that are directly 
 * controlled by, and have come into existence because of, this 
 * ExecutionUnit. See [Appendix II.3.2/TR-157a3] for more 
 * description and some examples. NOTE: All other objects and 
 * parameters (i.e. not multi-instanced objects) that this 
 * ExecutionUnit has caused to come into existence can be 
 * discovered via the DeviceInfo.SupportedDataModel.{i}. table. 
 */
#define  CCSP_AEM_EU_REQ_TYPE_REFERENCES            4
   
/* AssociatedProcessList retrieval, value handle must be cast to char **.
 * 
 * Comma-separated list of strings. Each list item is PID that is defined
 * in "Device.DeviceInfo.ProcessStatus.Process.{i}."
 */
#define  CCSP_AEM_EU_REQ_TYPE_PROCESSLIST           5

/* VendorLogList retrieval, value handle must be cast to char **.
 * 
 * On successful return, value contains a comma-separated list, and each
 * list item is the Alias of vendor log file when CPE creates the
 * corresponding table entry under "Device.DeviceInfo.VendorLogFile.{i}.".
 */
#define  CCSP_AEM_EU_REQ_TYPE_VENDOR_LOGLIST        6

/* VendorConfigList retrieval, value handle must be cast to char **.
 * 
 * On successful return, value contains a comma-separated list, and each
 * list item is the Name of vendor config file when CPE creates the
 * corresponding table entry under "Device.DeviceInfo.VendorConfigFile.{i}.".
 */
#define  CCSP_AEM_EU_REQ_TYPE_VENDOR_CONFIGLIST     7

/* Set "AutoStart", value handle must be cast to BOOL*.
 * 
 */
#define  CCSP_AEM_EU_REQ_TYPE_AUTO_START            8

/* Set "Run Level", value handle must be cast to LONG*.
 * 
 */
#define  CCSP_AEM_EU_REQ_TYPE_RUN_LEVEL             9

/* Set "RequestState", value handle must be cast to LONG*.
 * 
 */
#define  CCSP_AEM_EU_REQ_TYPE_REQUEST_STATE         10


#define  CCSP_AEMI_DU_STATE_INSTALLED               1
#define  CCSP_AEMI_DU_STATE_UNINSTALLED             2
#define  CCSP_AEMI_DU_STATE_FAILED                  3

/*
 * Description:
 *  PFN_CCSP_AEMI_DSCC_NOTIFY() is callback function that
 *  will be invoked by AEM plug-in to notify a previous requested
 *  package management operation has finished.
 *
 * Arguments:
 *  hEeHandle - EE that has performed DU operation.
 *  OperID - operation ID, passed in by AEM. NULL means
 *           this operation was not triggered by PA, or
 *           in another word, it is an autonomous package
 *           operation.
 *  UUID - UUID of DU
 *  DuRef - Demployment unit referenced that was installed
 *          updated, or removed.
 *  Version - the version of DU affected.
 *  CurrentState - one of CCSP_AEMI_DU_STATE_xxx
 *  Resolved - indicates whether or not DU operation resolved
 *           all dependencies.
 *  EuReList - a comma-separated list of EUs related to 
 *           affected DU. Each item is name of EU.
 *  StartTime - DU operation started in UTC.
 *  CompleteTime - DU operation fully completed in UTC.
 *  ErrorCode - CCSP error code
 *  ErrorString - error string.
 */
typedef
LONG
(*PFN_CCSP_AEMI_DSCC_NOTIFY)
    (
        ANSC_HANDLE           hEeHandle,
        char *                OperID,
        char *                UUID,
        char *                DuRef,
        char *                Version,
        ULONG                 CurrentState,
        BOOL                  Resolved,
        char *                EuRefList,
        char *                StartTime,
        char *                CompleteTime,
        ULONG                 ErrorCode,
        char *                ErrorString
    );

/*
 * Description:
 *  AEM plug-in initialization 
 * 
 * Arguments:
 *  DsccNotifyCB - callback function that AE plugin invokes
 *  to notify AEM that a package operation request has
 *  finished.
 *
 * Return Value:
 *  CCSP status code, 0 means success, other value indicates
 *  error occurs during plugin intialization. AEM may  
 *  simply log the event and will not make any further attempts.
 *
 * Note:
 *  AEA can be responsible for one or more EEs. It's up
 *  to AEA's policy how it wants to manage EEs, e.g. how many
 *  EE instances it wants to manage and when to load them.
 */
typedef  
LONG
(*PFN_CCSP_AEMI_INIT_AE)
    (
        PFN_CCSP_AEMI_DSCC_NOTIFY   DsccNotifyCB
    );


/*
 * Description:
 *  AEM plug-in shut-down
 * 
 * Arguments:
 *
 * Return Value:
 *  CCSP status code, 0 means success, other value indicates
 *  error occurs during plugin intialization. AEM may  
 *  simply log the event and will not make any further attempts.
 *  It's up to the EE to do whatever necessary in case error 
 *  occurs.
 *
 * Note:
 *  Since AEA can be responsible for one or more EEs, it needs
 *  to shut down (unload) all EEs it manages.
 */
typedef  
LONG
(*PFN_CCSP_AEMI_SHUTDOWN_AE)
    (
    );


/***********************************************************
  Application Environment service APIs
***********************************************************/

/*
 * Description:
 *  Retrieves the number of EEs AES is managing.
 * 
 * Arguments:
 *
 * Return Value:
 *  Number of EE instances.
 */
typedef
LONG
(*PFN_CCSP_AEMI_GET_EE_COUNT)
    (
    );

/*
 * Description:
 *  Retrieves EE instance by index which is ZERO based.
 * 
 * Arguments:
 *  Index - the index to EE instance to be retrieved.
 *  phEeHandle [out] - on successful return, it contains
 *  the handle to the requested EE instance.
 *
 * Return Value:
 *  CCSP status code. 
 */
typedef
LONG
(*PFN_CCSP_AEMI_GET_EE)
    (
        LONG                        index,
        ANSC_HANDLE*                phEeHandle
    );


/*
 * Description:
 *  Get EE's information
 * 
 * Arguments:
 *  hEeHandle - indicate the handle to EE that is being requested.
 *  ReqType - indicate which type of request, one of CCSP_AEM_EE_REQ_TYPE_XXX.
 *  hValue [in/out] - on success, it contains information
 *  about the specified EE. The value handle must be cast to what each
 *  type specifies.
 *
 * Return Value:
 *  CCSP status code, 0 means success, other value indicates
 *  error occurs.
 */
typedef
LONG
(*PFN_CCSP_AEMI_GET_EE_INFO)
    (
        ANSC_HANDLE                 hEeHandle,
        ULONG                       ReqType,
        ANSC_HANDLE                 hValue
    );


/*
 * Description:
 *  Set EE's information
 * 
 * Arguments:
 *  hEeHandle - indicate the handle to EE that is being requested.
 *  ReqType - indicate which type of request, one of CCSP_AEM_EE_REQ_TYPE_XXX.
 *  hValue [in/out] - on success, it contains information
 *  about the specified EE. The value handle must be cast to what each
 *  type specifies.
 *
 * Return Value:
 *  CCSP status code, 0 means success, other value indicates
 *  error occurs.
 */
typedef
LONG
(*PFN_CCSP_AEMI_SET_EE_INFO)
    (
        ANSC_HANDLE                 hEeHandle,
        ULONG                       ReqType,
        ANSC_HANDLE                 hValue
    );



/*
 * Description:
 *  Install a deployment unit.
 *
 * Arguments:
 *  hEeHandle - handle of EE that is requested to
 *              perform DU operation.
 *  OperID - operation ID
 *  URL - location of DU to be installed.
 *  UUID - UUID of DU to be installed, if NULL, EE
 *        must generate UUID.
 *  Username - username used by CPE to authenticate 
 *        with file server.
 *  Password - password used to authentication with
 *        file server.
 */
typedef
LONG
(*PFN_CCSP_AEMI_INSTALL)
    (
        ANSC_HANDLE            hEeHandle,
        char *                 OperID,
        char *                 URL,
        char *                 UUID,
        char *                 Username,
        char *                 Password
    );


/*
 * Description:
 *  Update a deployment unit.
 *
 * Arguments:
 *  hEeHandle - handle of EE that is requested to
 *              perform DU operation.
 *  OperID - operation ID
 *  UUID - UUID of DU to be installed, if NULL, EE
 *        must generate UUID.
 *  Version - the version of DU to be updated.
 *  URL - location of DU to be installed.
 *  Username - username used by CPE to authenticate 
 *        with file server.
 *  Password - password used to authentication with
 *        file server.
 */
typedef
LONG
(*PFN_CCSP_AEMI_UPDATE)
    (
        ANSC_HANDLE            hEeHandle,
        char *                 OperID,
        char *                 UUID,
        char *                 Version,
        char *                 URL,
        char *                 Username,
        char *                 Password
    );


/*
 * Description:
 *  Update a deployment unit.
 *
 * Arguments:
 *  hEeHandle - handle of EE that is requested to
 *              perform DU operation.
 *  OperID - operation ID
 *  UUID - UUID of DU to be installed, if NULL, EE
 *        must generate UUID.
 *  Version - the version of DU to be updated.
 */
typedef
LONG
(*PFN_CCSP_AEMI_REMOVE)
    (
        ANSC_HANDLE            hEeHandle,
        char *                 OperID,
        char *                 UUID,
        char *                 Version
    );


/***********************************************************
  Deployment Unit (DU) service APIs
***********************************************************/

/*
 * Description:
 *  Retrieves the number of DUs that have been installed
 *  in the specified EE.
 * 
 * Arguments:
 *  hEeHandle - the handle to EE instance.
 *
 * Return Value:
 *  Number of DU instances.
 */
typedef
LONG
(*PFN_CCSP_AEMI_GET_DU_COUNT)
    (
        ANSC_HANDLE            hEeHandle
    );

/*
 * Description:
 *  Retrieves EE instance by index which is ZERO based.
 * 
 * Arguments:
 *  hEeHandle - the handle to EE instance.
 *  Index - the index to DU instance to be retrieved.
 *  phDuHandle [out] - on successful return, it contains
 *  the handle to the requested DU instance.
 *
 * Return Value:
 *  CCSP status code. 
 */
typedef
LONG
(*PFN_CCSP_AEMI_GET_DU)
    (
        ANSC_HANDLE             hEeHandle,
        LONG                    index,
        ANSC_HANDLE*            phDuHandle
    );


/*
 * Description:
 *  Get DU's information
 * 
 * Arguments:
 *  hEeHandle - indicate the handle to EE by which requested DU is managed.
 *  hDuHandle - indicate the handle to DU that is being requested
 *  ReqType - indicate which type of request, one of CCSP_AEM_DU_REQ_TYPE_XXX.
 *  hValue [in/out] - on success, it contains information
 *  about the specified DU. The value handle must be cast to what each
 *  type specifies.
 *
 * Return Value:
 *  CCSP status code, 0 means success, other value indicates
 *  error occurs.
 */
typedef
LONG
(*PFN_CCSP_AEMI_GET_DU_INFO)
    (
        ANSC_HANDLE             hEeHandle,
        ANSC_HANDLE             hDuHandle,
        ULONG                   ReqType,
        ANSC_HANDLE             hValue
    );


/*
 * Description:
 *  Set DU's information
 * 
 * Arguments:
 *  hEeHandle - indicate the handle to EE by which requested DU is managed.
 *  hDuHandle - indicate the handle to DU that is being requested
 *  ReqType - indicate which type of request, one of CCSP_AEM_DU_REQ_TYPE_XXX.
 *  hValue [in/out] - on success, it contains information
 *  about the specified DU. The value handle must be cast to what each
 *  type specifies.
 *
 * Return Value:
 *  CCSP status code, 0 means success, other value indicates
 *  error occurs.
 */
typedef
LONG
(*PFN_CCSP_AEMI_SET_DU_INFO)
    (
        ANSC_HANDLE                 hEeHandle,
        ANSC_HANDLE                 hDuHandle,
        ULONG                       ReqType,
        ANSC_HANDLE                 hValue
    );



/***********************************************************
  Execution Unit (EU) service APIs
***********************************************************/

/*
 * Description:
 *  Retrieves the number of EUs that is included in the
 *  specified DU installed on specified EE.
 * 
 * Arguments:
 *
 * Return Value:
 *  Number of EU instances.
 */
typedef
LONG
(*PFN_CCSP_AEMI_GET_EU_COUNT)
    (
        ANSC_HANDLE                 hEeHandle,
        ANSC_HANDLE                 hDuHandle
    );

/*
 * Description:
 *  Retrieves EE instance by index which is ZERO based.
 * 
 * Arguments:
 *  hEeHandle - the handle to EE instance.
 *  Index - the index to DU instance to be retrieved.
 *  phDuHandle [out] - on successful return, it contains
 *  the handle to the requested DU instance.
 *
 * Return Value:
 *  CCSP status code. 
 */
typedef
LONG
(*PFN_CCSP_AEMI_GET_EU)
    (
        ANSC_HANDLE                 hEeHandle,
        ANSC_HANDLE                 hDuHandle,
        LONG                        index,
        ANSC_HANDLE*                phEuHandle
    );


/*
 * Description:
 *  Get EU's information
 * 
 * Arguments:
 *  hEeHandle - indicate the handle to EE by which requested DU is managed.
 *  hDuHandle - indicate the handle to DU that includes the requested EU
 *  hEuHandle - indicate the handle to EU that is being requested
 *  ReqType - indicate which type of request, one of CCSP_AEM_EU_REQ_TYPE_XXX.
 *  hValue [in/out] - on success, it contains information
 *  about the specified EU. The value handle must be cast to what each
 *  type specifies.
 *
 * Return Value:
 *  CCSP status code, 0 means success, other value indicates
 *  error occurs.
 */
typedef
LONG
(*PFN_CCSP_AEMI_GET_EU_INFO)
    (
        ANSC_HANDLE                 hEeHandle,
        ANSC_HANDLE                 hDuHandle,
        ANSC_HANDLE                 hEuHandle,
        ULONG                       ReqType,
        ANSC_HANDLE                 hValue
    );


/*
 * Description:
 *  Set EU's information
 * 
 * Arguments:
 *  hEeHandle - indicate the handle to EE by which requested DU is managed.
 *  hDuHandle - indicate the handle to DU that includes the requested EU
 *  hEuHandle - indicate the handle to EU that is being requested
 *  ReqType - indicate which type of request, one of CCSP_AEM_EU_REQ_TYPE_XXX.
 *  hValue [in/out] - on success, it contains information
 *  about the specified EU. The value handle must be cast to what each
 *  type specifies.
 *
 * Return Value:
 *  CCSP status code, 0 means success, other value indicates
 *  error occurs.
 */
typedef
LONG
(*PFN_CCSP_AEMI_SET_EU_INFO)
    (
        ANSC_HANDLE                 hEeHandle,
        ANSC_HANDLE                 hDuHandle,
        ANSC_HANDLE                 hEuHandle,
        ULONG                       ReqType,
        ANSC_HANDLE                 hValue
    );

