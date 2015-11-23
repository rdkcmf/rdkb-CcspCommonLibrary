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

if [ "x"$1 = "xkill" ] || [ "x"$2 = "xkill" ]; then
	killall ccspRecoveryManager
	sleep 3
	killall CcspTandDSsp
	killall CcspDnsSsp
	killall CcspFuSsp
	killall CcspSsdSsp
	killall CcspPandMSsp
	killall CcspTr069PaSsp
	killall PsmSsp
	killall CcspRmSsp
	killall CcspCrSsp
#    killall CcspLmSsp
	killall CcspMtaAgentSsp
	killall CcspCMAgentSsp
    killall CcspLMLite
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
export LOG4C_RCPATH=/fss/gw/rdklogger

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
$BINPATH/dbus-daemon --config-file=./basic.conf --fork

mkdir -p /var/tmp/logs/


if [ -f "/fss/gw/rdklogger/rdkbLogMonitor.sh" ]
then
	/fss/gw/rdklogger/rdkbLogMonitor.sh &
fi

if [ -f "/fss/gw/rdklogger/fileUploadRandom.sh" ]
then
	/fss/gw/rdklogger/fileUploadRandom.sh &
fi
touch /tmp/cp_subsys_ert

if [ -f /tmp/cp_subsys_ert ]; then
	Subsys="eRT."
elif [ -e ./cp_subsys_emg ]; then
	Subsys="eMG."
else
	Subsys=""
fi

echo "Elected subsystem is $Subsys"

if [ "x"$Subsys = "x" ];then
	$BINPATH/CcspCrSsp
else
	echo "$BINPATH/CcspCrSsp -subsys $Subsys"
	$BINPATH/CcspCrSsp -subsys $Subsys
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

if [ "x"$Subsys = "x" ];then
	$BINPATH/PsmSsp
else
    echo "$BINPATH/PsmSsp -subsys $Subsys"
	$BINPATH/PsmSsp -subsys $Subsys
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

if [ -e ./wecb ]; then                                                                                                                                                                                                                    
    cd wecb                                                                                                                
    if [ "x"$Subsys = "x" ];then                                                                                           
        $BINPATH/CcspWecbController                                                                                               
    else                                                                                                                   
        echo "$BINPATH/CcspWecbController -subsys $Subsys"                                                                        
       $BINPATH/CcspWecbController -subsys $Subsys                                                                               
    fi                                                                                                                     
    cd ..                                                                                                                  
fi  


