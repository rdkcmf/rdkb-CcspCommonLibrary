##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2015 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
#######################################################################
#   Copyright [2014] [Cisco Systems, Inc.]
# 
#   Licensed under the Apache License, Version 2.0 (the \"License\");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
# 
#       http://www.apache.org/licenses/LICENSE-2.0
# 
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an \"AS IS\" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#######################################################################

BINPATH="/usr/bin"
BBHM_CUR_CFG="/nvram/bbhm_cur_cfg.xml"
source /etc/utopia/service.d/log_env_var.sh
source /etc/utopia/service.d/log_capture_path.sh
source /etc/device.properties

ulimit -c unlimited
if [ "$BUILD_TYPE" != "prod" ]; then
      echo /tmp/%t_core.prog_%e.signal_%s > /proc/sys/kernel/core_pattern
fi

if [ -f /etc/mount-utils/getConfigFile.sh ];then
   . /etc/mount-utils/getConfigFile.sh
fi

if [ "x"$1 = "xkill" ] || [ "x"$2 = "xkill" ]; then
	killall ccspRecoveryManager
	sleep 3
	killall CcspTandDSsp
	killall CcspDnsSsp
#	killall CcspFuSsp
#	killall CcspSsdSsp
	killall CcspPandMSsp
	killall CcspTr069PaSsp
	killall PsmSsp
#	killall CcspRmSsp
	killall CcspCrSsp
#    killall CcspLmSsp
	killall CcspMtaAgentSsp
	killall CcspCMAgentSsp
    killall CcspLMLite
    killall CcspXdnsSsp
	killall webpa
fi

#
#	Allow custom plug-ins
#
if [ -f "$PWD/cosa_start_custom_1.sh" ]; then
	./cosa_start_custom_1.sh
fi

export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket
export LOG4C_RCPATH=/rdklogger

#zqiu: update the bbhm for 2.1s11
if [ "$BOX_TYPE" = "XB3" ]; then
    /usr/ccsp/psm/bbhm_patch.sh -f /nvram/bbhm_cur_cfg.xml
else
    echo "bbhm patch is not required for other platforms"
fi

# Check if bbhm has Notify flag present
NOTIFYPRESENT=`cat /nvram/bbhm_cur_cfg.xml | grep NotifyWiFiChanges`
REDIRCTEXISTS=""

# If Notify flag is not present then we will add it as per the syscfg DB value
if [ "$NOTIFYPRESENT" = "" ]
then
	REDIRECT_VALUE=`syscfg get redirection_flag`
	if [ "$REDIRECT_VALUE" = "" ]
	then
		#Just making sure if syscfg command didn't fail
		REDIRCTEXISTS=`cat /nvram/syscfg.db | grep redirection_flag | cut -f2 -d=`
	fi

	if [ "$REDIRECT_VALUE" = "false" ] || [ "$REDIRCTEXISTS" = "false" ];
	then
		
		echo " Apply Notifywifichanges flse"
		sed '10 a \ \ \ <Record name=\"eRT.com.cisco.spvtg.ccsp.Device.WiFi.NotifyWiFiChanges\" type=\"astr\">false</Record>' /nvram/bbhm_cur_cfg.xml > /var/tmp/bbhm_cur_cfg.xml
		sed '10 a \ \ \ <Record name=\"eRT.com.cisco.spvtg.ccsp.Device.WiFi.NotifyWiFiChanges\" type=\"astr\">false</Record>' /nvram/bbhm_bak_cfg.xml > /var/tmp/bbhm_bak_cfg.xml
		cp /var/tmp/bbhm_cur_cfg.xml /nvram/bbhm_cur_cfg.xml
		cp /var/tmp/bbhm_bak_cfg.xml /nvram/bbhm_bak_cfg.xml
		rm /var/tmp/bbhm_cur_cfg.xml
		rm /var/tmp/bbhm_bak_cfg.xml
	elif [ "$REDIRECT_VALUE" = "true" ] || [ "$REDIRCTEXISTS" = "true" ];
	then
		echo " Apply Notifywifichanges tue"
		sed '10 a \ \ \ <Record name=\"eRT.com.cisco.spvtg.ccsp.Device.WiFi.NotifyWiFiChanges\" type=\"astr\">true</Record>' /nvram/bbhm_cur_cfg.xml > /var/tmp/bbhm_cur_cfg.xml
		sed '10 a \ \ \ <Record name=\"eRT.com.cisco.spvtg.ccsp.Device.WiFi.NotifyWiFiChanges\" type=\"astr\">true</Record>' /nvram/bbhm_bak_cfg.xml > /var/tmp/bbhm_bak_cfg.xml
		cp /var/tmp/bbhm_cur_cfg.xml /nvram/bbhm_cur_cfg.xml
		cp /var/tmp/bbhm_bak_cfg.xml /nvram/bbhm_bak_cfg.xml
		rm /var/tmp/bbhm_cur_cfg.xml
		rm /var/tmp/bbhm_bak_cfg.xml
	fi
fi

if [ ! -f "/nvram/l2net_port_details_cleaned" ] 
then
	echo "Clearing up bridge data from from DB"
	cp /nvram/bbhm_cur_cfg.xml /tmp/b3
	sed -i '/dmsb.l2net.2.Port./d' /tmp/b3
	sed -i '/Provision.COSALibrary.Bridging.NextPortInstanceNum2.NextPortInstanceNum/d' /tmp/b3
        sed -i '/Provision.COSALibrary.BridgingHalfAdded./d' /tmp/b3
	cp /tmp/b3 /nvram/bbhm_cur_cfg.xml
	rm /tmp/b3
	touch "/nvram/l2net_port_details_cleaned"
fi

# Start coredump
if [ -f "$PWD/core_compr" ]; then
	if ! [ -e "/var/core" ]; then
		mkdir -p /var/core/
	fi
	echo "|$PWD/core_compr /var/core %p %e" >/proc/sys/kernel/core_pattern
	ulimit -c unlimited

	./core_report.sh &
fi

# Start Config Recover

#conf_rec &

cp ccsp_msg.cfg /tmp

# have IP address for dbus config generated
#./DbusCfg
if [ "$MANUFACTURE" != "Technicolor" ]; then
	$BINPATH/dbus-daemon --config-file=./basic.conf --fork
fi

mkdir -p $LOG_PATH

touch /tmp/cp_subsys_ert

if [ -f /tmp/cp_subsys_ert ]; then
	Subsys="eRT."
elif [ -e ./cp_subsys_emg ]; then
	Subsys="eMG."
else
	Subsys=""
fi

# Remove all disable flags
if [ -e /nvram/disablewecb ]; then
	echo "***Removing all disable flags*****"
        rm -rf /nvram/disable*
fi

echo "Elected subsystem is $Subsys"

if [ "$CR_IN_PEER" != "yes" ]
then
	if [ "x"$Subsys = "x" ];then
		$BINPATH/CcspCrSsp
	else
		echo "$BINPATH/CcspCrSsp -subsys $Subsys"
		$BINPATH/CcspCrSsp -subsys $Subsys
	fi
fi

if [ -e ./logagent ]; then
	cd logagent

	if [ "x"$Subsys = "x" ];then
		$BINPATH/log_agent
	else
		echo "$BINPATH/log_agent -subsys $Subsys"
		$BINPATH/log_agent -subsys $Subsys
	fi
	cd ..
fi

echo_t "Getting value of CMC and CID before PSM Initialization"
grep -irn "X_COMCAST-COM_CID\|X_COMCAST-COM_CMC" $BBHM_CUR_CFG


#if [ -e /nvram/disablePsmSSP ]; then
#	echo "***disabling PsmSSP****"
if [ "x"$Subsys = "x" ];then
	$BINPATH/PsmSsp
else
    echo "$BINPATH/PsmSsp -subsys $Subsys"
	$BINPATH/PsmSsp -subsys $Subsys
fi

sleep 5

if [ -e ./notify-comp ]; then
        cd notify-comp

        if [ "x"$Subsys = "x" ];then
                $BINPATH/notify_comp
        else
                echo "$BINPATH/notify_comp -subsys $Subsys"
                $BINPATH/notify_comp -subsys $Subsys
        fi
        cd ..
fi

if [ -e ./cm ]; then
        cd cm
        if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspCMAgentSsp
        else
        echo_t "$BINPATH/CcspCMAgentSsp -subsys $Subsys"
                $BINPATH/CcspCMAgentSsp -subsys $Subsys
        fi
        cd ..
fi

ADVSEC_MIGRATION_SCRIPT=/usr/ccsp/pam/advsec_migrate_psm_to_syscfg.sh
if [ -f $ADVSEC_MIGRATION_SCRIPT ]; then
#This script should be removed once migration from PSM to SysCfg DB is complete for AdvSec Finger Print service.
        $ADVSEC_MIGRATION_SCRIPT load_syscfg_setting
fi

if [ -e ./pam ]; then
	cd pam

	if [ "x"$Subsys = "x" ];then
		$BINPATH/CcspPandMSsp
	else
		echo "$BINPATH/CcspPandMSsp -subsys $Subsys"
		$BINPATH/CcspPandMSsp -subsys $Subsys
	fi
	cd ..
fi


#if [ -e ./wecb ];
#then                                                                                                                                                                                                                   
#    cd wecb                                                                                                                
#    if [ "x"$Subsys = "x" ];then                                                                                           
#        $BINPATH/CcspWecbController                                                                                               
#    else                                                                                                                   
#        echo "$BINPATH/CcspWecbController -subsys $Subsys"                                                                        
#       $BINPATH/CcspWecbController -subsys $Subsys                                                                               
#    fi                                                                                                                     
#    cd ..                                                                                                                  
#fi  

#Mesh-596: We need to start dropbear a bit earlier so that lease sync happens
#for plume
if [ "x$MULTI_CORE" == "xyes" ]; then
    echo "starting dropbear"
        mkdir -p /tmp/.dropbear
    DROPBEAR_PARAMS_1="/tmp/.dropbear/dropcfg1$$"
    DROPBEAR_PARAMS_2="/tmp/.dropbear/dropcfg2$$"
    getConfigFile $DROPBEAR_PARAMS_1
    getConfigFile $DROPBEAR_PARAMS_2
    dropbear -r $DROPBEAR_PARAMS_1 -r $DROPBEAR_PARAMS_2 -E -s -p $ARM_INTERFACE_IP:22 -P /var/run/dropbear_ipc.pid > /dev/null 2>&1 &

fi

if [ -f /version.txt ]
then
   echo "version.txt exists"
   imgname=`cat /version.txt | grep -i 'imagename'`
   echo $imgname
   flag=`echo $imgname|awk '{print match($0,"TDK")}'`;
   if [ $flag -gt 0 ];then
       if [ -f "/usr/ccsp/tdk/StartTDK.sh" ]
       then
           echo "Found TDK Image"
           echo "Invoking TDK start up script"
           sh /usr/ccsp/tdk/StartTDK.sh &
       else
           echo "Found RDM enabled TDK Image"
           echo "Invoking tdk_download script"
           sh /usr/sbin/tdkb_download.sh &
       fi
   fi
fi
