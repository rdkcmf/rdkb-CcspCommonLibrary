#!/bin/sh
####################################################################################
# If not stated otherwise in this file or this component's Licenses.txt file the
# following copyright and licenses apply:

#  Copyright 2018 RDK Management

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

# http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#######################################################################################

. /etc/device.properties
. /etc/include.properties

source /etc/utopia/service.d/log_capture_path.sh
BBHM_CUR_CFG="/nvram/bbhm_cur_cfg.xml"
BINPATH="/usr/bin"
UTOPIA_PATH=/etc/utopia/service.d
if [ "x$BOX_TYPE" = "xXB3" ]; then
cd /fss/gw/usr/ccsp/
else
cd /usr/ccsp/
fi

if [ -f /etc/mount-utils/getConfigFile.sh ];then
   . /etc/mount-utils/getConfigFile.sh
fi

export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket
if [ "x$BOX_TYPE" = "xXB3" ]; then
export LOG4C_RCPATH=/fss/gw/rdklogger
elif [ "x$BOX_TYPE" = "xTCCBR" ]; then
export LOG4C_RCPATH=/rdklogger
else
export LOG4C_RCPATH=/etc
fi

echo_t "Getting value of CMC and CID after PSM Initialization"
grep -irn "X_COMCAST-COM_CID\|X_COMCAST-COM_CMC" $BBHM_CUR_CFG

if [ -f /tmp/cosa_start_rem_triggered ]; then
	echo_t "Already cosa_start_rem.sh script was triggered"
	exit
else
	loop_count=0
	while [ $loop_count -lt 6 ]; do
		wan_event=`sysevent get wan-status`
		if [ "$wan_event" == "started" ]; then
			break
		fi
		echo "wan-status is not started yet. sleeping for 10 sec in cosa_start_rem.sh ..."
		sleep 10
		loop_count=$((loop_count + 1))
	done

	touch  /tmp/cosa_start_rem_triggered
	echo_t "Triggered cosa_start_rem script and created tmp/cosa_start_rem_triggered file"
fi

if [ -f "/etc/AKER_ENABLE" ]; then
    echo "/usr/bin/aker -p $PARODUS_URL -c $AKER_URL -w parcon -d /nvram/pcs.bin -f /nvram/pcs.bin.md5" > /tmp/aker_cmd.cmd
    aker_cmd=`cat /tmp/aker_cmd.cmd`
    $aker_cmd &
    echo_t "aker process should have been started"
else
    echo_t "aker NOT enabled"
fi


isCloudCapable=1
isCloudCapable=`syscfg get cloud_capable_flag`
if [ $isCloudCapable -eq 0 ]
then
	syscfg set cloud_capable_flag 1
	syscfg commit
fi

if [ -f /tmp/cp_subsys_ert ]; then
        Subsys="eRT."
elif [ -e /tmp/cp_subsys_emg ]; then
	        Subsys="eMG."
else
        Subsys=""
fi

echo "Creating MESH vlans"
$UTOPIA_PATH/service_multinet_exec create_mesh_vlan &

if [ "x$BOX_TYPE" != "xTCCBR" ]; then
echo "Starting brlan1 initialization, check whether brlan1 is there or not"
ifconfig | grep brlan1
if [ $? == 1 ]; then
    echo "brlan1 not present go ahead and create it"
    sysevent set multinet-up 2
fi

# Waiting for brlan1 interface creation for 30 sec
iter=0
max_iter=2
while [ ! -f /tmp/brlan1_up ] && [ "$iter" -le $max_iter ]
do
    iter=$((iter+1))
    echo "Sleep Iteration# $iter"
    sleep 10
done

if [ ! -f /tmp/brlan1_up ]; then
    echo "brlan1 is not up after maximum iterations i.e 30 sec go ahead with the execution"
else
    echo "brlan1 is created after interation $iter go ahead with the execution"
fi
fi

HOME_LAN_ISOLATION=`psmcli get dmsb.l2net.HomeNetworkIsolation`
if [ "$HOME_LAN_ISOLATION" == "1" ]; then
echo "Starting brlan10 initialization, check whether brlan10 is there or not"
ifconfig | grep brlan10
if [ $? == 1 ]; then
    echo "brlan10 not present go ahead and create it"
    sysevent set multinet-up 9
fi

# Waiting for brlan10 -MoCA bridge interface creation for 30 sec
iter=0
max_iter=2
while [ ! -f /tmp/MoCABridge_up ] && [ "$iter" -le $max_iter ]
do
    iter=$((iter+1))
    echo "Sleep Iteration# $iter"
    sleep 10
done

if [ ! -f /tmp/MoCABridge_up ]; then
    echo "brlan10 is not up after maximum iterations i.e 30 sec go ahead with the execution"
else
    echo "brlan10 is created after interation $iter go ahead with the execution"
    killall igmpproxy                                   
    killall MRD
    sleep 1 
    #smcroute -f /usr/ccsp/moca/smcroute.conf -d
    sysevent set mcastproxy-restart
    MRD &
    sysevent set firewall-restart

fi

fi
echo_t "Elected subsystem is $Subsys"

if [ -f /lib/rdk/start-upnp-service ]; then
    /lib/rdk/start-upnp-service start &
fi

if [ -e ./pam ]; then
    cd pam
        #double background to detach the script from the tty
        ((sh /etc/email_notification_monitor.sh 12 &) &)
        cd ..
fi
echo_t "Enabling ssh by default"
syscfg set mgmt_wan_sshaccess 1
syscfg commit 
echo_t "PWD is `pwd`"

# Enable XCONF Conf config fetch
if [ -f  /lib/rdk/dcm.service ]; then
    echo_t "DCM SCRIPT: Calling dcm.service"
    /bin/sh /lib/rdk/dcm.service &
fi

if [ -e ./mta ]; then
    cd mta
    if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspMtaAgentSsp
    else
        echo_t "$BINPATH/CcspMtaAgentSsp -subsys $Subsys"
        $BINPATH/CcspMtaAgentSsp -subsys $Subsys
    fi
    cd ..
fi

if [ -e ./moca ]; then
   cd moca
    if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspMoCA
    else
        echo "$BINPATH/CcspMoCA -subsys $Subsys"
       $BINPATH/CcspMoCA -subsys $Subsys
    fi
    cd ..
fi

#if [ -e /nvram/disableCcspRmSsp ]; then
#   echo "****DISABLE CcspRmSsp*****"
#elif [ -f ./cp_subsys_ert ]; then
#        if [ -e ./rm ]; then
#                cd rm
#        echo "$BINPATH/CcspRmSsp -subsys $Subsys"
#                $BINPATH/CcspRmSsp -subsys $Subsys
#        cd ..
#        fi
#fi

if [ "x"$1 = "xpam" ] || [ "x"$2 = "xpam" ]; then
	exit 0
fi

#cd ../avahi
#$PWD/avahi-daemon --file=$PWD/avahi-daemon.conf -D

# Tr069Pa, as well as SecureSoftwareDownload and FirmwareUpgrade

if [ -e ./tr069pa ]; then
	cd tr069pa
	if [ "x"$Subsys = "x" ]; then
		$BINPATH/CcspTr069PaSsp
	else
	echo_t "$BINPATH/CcspTr069PaSsp -subsys $Subsys"
		$BINPATH/CcspTr069PaSsp -subsys $Subsys
	fi
#        sysevent setunique GeneralPurposeFirewallRule " -A INPUT -i erouter0 -p tcp --dport=7547 -j ACCEPT "
#        sysevent setunique GeneralPurposeFirewallRule " -A INPUT ! -i erouter0 -p tcp -m tcp --dport 7547 -j DROP "
	cd ..
fi

#if [ -e ./ssd ]; then
#        cd ssd
#        if [ "x"$Subsys = "x" ];then
#                $BINPATH/CcspSsdSsp
#        else
#                echo "$BINPATH/CcspSsdSsp -subsys $Subsys"
#                $BINPATH/CcspSsdSsp -subsys $Subsys
#        fi
#        cd ..
#fi

#if [ -e ./fu ]; then
#        cd fu
#        if [ "x"$Subsys = "x" ];then
#                $BINPATH/CcspFuSsp
#        else
#                echo "$BINPATH/CcspFuSsp -subsys $Subsys"
#                $BINPATH/CcspFuSsp -subsys $Subsys
#        fi
#        cd ..
#fi
       
#if [ -e ./tr069pa ]; then
# add firewall rule to allow incoming packet for port 7547
#sysevent setunique GeneralPurposeFirewallRule " -A INPUT -i erouter0 -p tcp --dport=7547 -j ACCEPT "
#fi

#fi


if [ -e ./tad ]; then
        cd tad
        #delay TaD in order to reduce CPU overload and make PAM ready early
        if [ "x"$Subsys = "x" ];then
                $BINPATH/CcspTandDSsp
        else
        echo_t "$BINPATH/CcspTandDSsp -subsys $Subsys"
                $BINPATH/CcspTandDSsp -subsys $Subsys
        fi
        cd ..
fi

isPeriodicFWCheckEnable=`syscfg get PeriodicFWCheck_Enable`
if [ "$isPeriodicFWCheckEnable" == "false" ]; then
    if [ "x$BOX_TYPE" == "xXB3" ]; then
	       echo_t "XCONF SCRIPT : Calling XCONF Client"
		/etc/xb3_firmwareDwnld.sh &
	fi
fi

#if [ -e ./ccspRecoveryManager ]; then
#        if [ "x"$Subsys = "x" ];then
#                $BINPATH/ccspRecoveryManager &
#        else
#                echo "$BINPATH/ccspRecoveryManager -subsys $Subsys &"
#                $BINPATH/ccspRecoveryManager -subsys $Subsys &
#        fi
#fi

#Lm need initialization after others running
#Sleep 120 is a temporary method
#if [ -e ./lm ]; then
#    sleep 120
#    cd lm
#if [ "x"$Subsys = "x" ];then
#    ./CcspLmSsp
#else
#   echo "./CcspLmSsp -subsys $Subsys"
#   ./CcspLmSsp -subsys $Subsys
#fi
#cd ..
#fi

if [ -e ./xdns ]; then
    cd xdns
    echo_t "$BINPATH/CcspXdnsSsp -subsys $Subsys &"
    $BINPATH/CcspXdnsSsp -subsys $Subsys &
    cd ..
fi

if [ -e ./ethagent ]; then
    cd ethagent
    echo_t "$BINPATH/CcspEthAgent -subsys $Subsys"
    $BINPATH/CcspEthAgent -subsys $Subsys
    cd ..
fi

if [ -e ./lm ]; then
    echo_t "***Starting CcspLMLite****"
    cd lm
    echo_t "$BINPATH/CcspLMLite -subsys $Subsys &"
	if [ "x$BOX_TYPE" == "xXB3" ] && [ "x$DEVICE_MODEL" == "xTCHXB3" ]; then
		    ulimit -s 2048
		    $BINPATH/CcspLMLite -subsys $Subsys &
		    ulimit -s 64
	else
		    $BINPATH/CcspLMLite -subsys $Subsys &
	fi

	cd ..
fi

if [ "x$BOX_TYPE" == "xTCCBR" ]; then
#Added this from cosa_start_atom.sh since in tchxb6 all Ccsp Componets are running on Arm side
if [ -e ./harvester ]; then
       echo_t "****STARTING HARVESTER***"
        cd harvester
        $BINPATH/harvester &
       cd ..
fi

#Added this from cosa_start_atom.sh since in tchxb6 all Ccsp Componets are running on Arm side
if [ -e ./wifi ]; then
       cd wifi
       if [ "x"$Subsys = "x" ];then
        echo_t "****STARTING CCSPWIFI WITHOUT SUBSYS***"
       $BINPATH/CcspWifiSsp &
       else
        echo_t "****STARTING CCSPWIFI WITH SUBSYS***"
       echo_t "$BINPATH/CcspWifiSsp -subsys $Subsys &"
       $BINPATH/CcspWifiSsp -subsys $Subsys &
       fi
	   cd ..
fi
fi

echo "bringing up XfinityWifi interfaces after all CCSP processes are up"
xfinityenable=`psmcli get dmsb.hotspot.enable`
if [ $xfinityenable -eq 1 ];then
    echo "Xfinitywifi is enabled bring up brlan2 and brlan3 if not already present"
    sysevent set hotspot-start
else
    echo "Xfinitywifi is not enabled no need to bring up brlan2 and brlan3 interfaces"
fi

sleep 5

SELFHEAL_ENABLE=`syscfg get selfheal_enable`
if [ "$SELFHEAL_ENABLE" == "false" ]
then
	echo_t "Running process monitoring script"
	/etc/process_monitor.sh &
fi

# starting the minidump watcher & uploader
if [ -f /usr/bin/inotify-minidump-watcher ];then
      /usr/bin/inotify-minidump-watcher /minidumps /lib/rdk/uploadDumps.sh  "\"\" 0" "*.dmp" &
fi

# Upload minidumps generated before the inotify call
if [ -f /lib/rdk/uploadDumps.sh ];then
     sh /lib/rdk/uploadDumps.sh  "" 0 &
fi


#/etc/utopia/service.d/service_sshd.sh sshd-start &

# Enable SSH between processors for devices having multiple processors alone
if [ "x$MULTI_CORE" == "xyes" ]; then
    rm -rf /telemetry/*.cmd
    /usr/bin/inotify-minidump-watcher /telemetry /lib/rdk/telemetryEventListener.sh 0 "*.cmd" &
fi

echo "Enable RFC feature"
# Enable RFC feature
if [ -f  /lib/rdk/rfc.service ]; then
   /bin/sh /lib/rdk/rfc.service &
fi

# XF3 enables zram using systemd
# Check zram and enable based on RFC
if [ -f  /lib/rdk/rdkzram.service ]; then
   /bin/sh /lib/rdk/rdkzram.service &
fi

#RDKB-7535
if [ "x$BOX_TYPE" != "xTCCBR" ] && [ "x$WAN_TYPE" != "xEPON" ]; then
	if [ -f "/rdklogger/rdkbLogMonitor.sh" ]
	then
		/rdklogger/rdkbLogMonitor.sh &
	fi
fi

if [ -f "/rdklogger/fileUploadRandom.sh" ]
then
	/rdklogger/fileUploadRandom.sh &
fi

isPeriodicFWCheckEnabled=`syscfg get PeriodicFWCheck_Enable`
if [ "$isPeriodicFWCheckEnabled" == "true" ]; then
	echo "XCONF SCRIPT : Calling XCONF Client"
	if [ "x$BOX_TYPE" != "xTCCBR" ]; then
		/etc/firmwareSched.sh &
	fi
fi

if [ "x$ENABLE_SNMPv3" == "xtrue" ]; then
    if [ -f /lib/rdk/run_snmpv3_master.sh ]; then
        /lib/rdk/run_snmpv3_master.sh &
    fi
fi

ADVSEC_LAUNCH_SCRIPT=/usr/ccsp/pam/launch_adv_security.sh
fingerprintenable=`syscfg get Advsecurity_DeviceFingerPrint`
rabidEnabled=`syscfg get Advsecurity_RabidEnable`
if [ "$fingerprintenable" = "1" ] || [ "$rabidEnabled" = "1" ]; then
    echo "Device_Finger_Printing_enabled:true"
    if [ -f $ADVSEC_LAUNCH_SCRIPT ]; then
        echo_t "$ADVSEC_LAUNCH_SCRIPT"
        $ADVSEC_LAUNCH_SCRIPT -start &
    fi
else
    echo "Device_Finger_Printing_enabled:false"
fi

#if [ "x$BOX_TYPE" == "xTCCBR" ]; then
#	echo_t "starting apshealth.sh"
#	/usr/ccsp/wifi/apshealth.sh &
#
#	echo_t "starting stahealth_log.sh"
#	/usr/ccsp/wifi/stahealth_log.sh &
#fi

rm -rf /tmp/.dropbear
