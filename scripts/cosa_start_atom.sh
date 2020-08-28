#!/bin/sh
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

. /etc/device.properties

MAX_WAIT_TIME=300
killall CcspWifiSsp
killall harvester
killall CcspCrSsp
killall rpcserver

#To set the default value for AP_WPA_GROUP_REKEY
DEVICE_MODEL=`grep "DEVICE_MODEL" /etc/device.properties | cut -d '=' -f2`
MFG_NAME=`grep "MFG_NAME" /etc/device.properties | cut -d '=' -f2`
if [ "$DEVICE_MODEL"=="TCHXB3" ]; then
        AP_WPA_GROUP_REKEY_16=`cfg -e | grep AP_WPA_GROUP_REKEY_16|cut -d '=' -f2`
        if [ "$AP_WPA_GROUP_REKEY_16"=="3600" ]; then
                if [ -f /usr/ccsp/wifi/grp_rekey_flip.sh ]; then
                        sh /usr/ccsp/wifi/grp_rekey_flip.sh
                fi
        fi
fi

# have IP address for dbus config generated
vconfig add eth0 500
ifconfig eth0.500 $ATOM_INTERFACE_IP

# have IP address for Radius config
vconfig add eth0 4090
ifconfig eth0.4090 192.168.251.254 netmask 255.255.255.0 up
ip route add default via 192.168.251.1

# Creating ATOM side vlan for IOT and assigning IP address
vconfig add eth0 106
ifconfig eth0.106 192.168.106.254 netmask 255.255.255.0 up
ip route add default via 192.168.106.1

# RDKB-15951 Creating Mesh Bhaul vlan in atom
vconfig add eth0 1060
ifconfig eth0.1060 192.168.245.254 netmask 255.255.255.0 up
ip route add default via 192.168.245.1

#start dropbear
if [ -f /etc/mount-utils/getConfigFile.sh ];then
     . /etc/mount-utils/getConfigFile.sh
fi
echo "starting dropbear ATOM"
mkdir -p /tmp/.dropbear
DROPBEAR_PARAMS_1="/tmp/.dropbear/dropcfg1$$"
DROPBEAR_PARAMS_2="/tmp/.dropbear/dropcfg2$$"
getConfigFile $DROPBEAR_PARAMS_1
getConfigFile $DROPBEAR_PARAMS_2
dropbear -r $DROPBEAR_PARAMS_1 -r $DROPBEAR_PARAMS_2 -E -s -p $ATOM_INTERFACE_IP:22 > /dev/null 2>&1 &

export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
#####BEGIN: Changes for ARRISXB3-3853
export PATH=$PATH:/etc/ath
echo "PATH="$PATH

LOG_FOLDER="/rdklogs/logs/"
CRONPATH="/tmp/cron/"
CRONFILE=$CRONPATH"root"
CRONFILE_BK="/tmp/cron_tab$$.txt"
MESHAGENT="/usr/bin/meshAgent"
DNSMASQ_FILE="/nvram/dnsmasq.leases"
MODEL_NUM=`grep "MODEL_NUM" /etc/device.properties | cut -d "=" -f2`

mkdir -p $LOG_FOLDER

if [ -e /rdklogger/log_capture_path_atom.sh ]
then
	source /rdklogger/log_capture_path_atom.sh 
else
	echo_t()
	{
        	echo $1
	}
fi

#####END: Changes for ARRISXB3-3853

# starting the rpcserver
echo_t "starting rpcserver atom"
if [ -f /usr/bin/rpcserver ];then
      /usr/bin/rpcserver &
fi

if [ -f /usr/ccsp/wifi/bwg_wifi_setting.sh ];then
      /usr/ccsp/wifi/bwg_wifi_setting.sh
fi

# upgrade the wifi config to suppot vlan 106
# This script is only for ARRISXB3 and Ciscoxb3. All other product do not need this step.
rdk_wifi_upgrade_vlan106

# Upgdate the correct Vlan for Secure SSIDs.
rdk_wifi_upgrade_secureSSID_vlan


#Check for radio swap cases and revert it to right one for Cisco 3941
if [ "$MODEL_NUM" = "DPC3941" ]; then
 if [ -f  /nvram/etc/ath/.configData ]; then
  SWAPPED=`grep "AP_RADIO_ID=" /nvram/etc/ath/.configData | cut -f2 -d"="`
  if [ "$SWAPPED" -eq 1 ]; then
   echo "Radios are found to be swapped from previous config, restoring to right value"
   /usr/ccsp/wifi/correctRadioCfg.sh
  fi
 fi
 echo "Checking and configurig mesh APs properly"
 /usr/ccsp/wifi/meshapcfgCorrection.sh
fi

# Config mesh backhal SSID
/usr/ccsp/wifi/meshapcfg.sh

# Remove existing dnsmasq.leases file from Atom, since mesh uses new design of direct notification 
#from dnsmasq
if [ $MODEL_NUM == "DPC3941" ] || [ $MODEL_NUM == "TG1682G" ] || [ $MODEL_NUM == "TG1682" ] || [ $MODEL_NUM == "DPC3939" ]; then
 rm -rf $DNSMASQ_FILE
fi

# Config check in wifi cfg database
if [ -f /usr/sbin/wifi_inconsistent_config_check.sh ];then
	/usr/sbin/wifi_inconsistent_config_check.sh
fi

# enable core files on atom
ulimit -c unlimited
echo "/tmp/core.%e" > /proc/sys/kernel/core_pattern
echo "0" > /proc/sys/kernel/core_uses_pid
 
cp -f ccsp_msg.cfg /tmp

if [ "x"$1 = "x" ];then
    sleep 10
fi

Subsys="eRT."

echo_t "Elected subsystem is $Subsys"

sleep 1

echo_t "Starting telnet"
if [ -e /usr/sbin/telnetd ]; then
    /usr/sbin/telnetd -b $ATOM_INTERFACE_IP
fi
echo_t "Starting inotify watcher for telemetry"
/usr/bin/inotify-minidump-watcher /telemetry /lib/rdk/telemetryEventListener.sh 0 "*.cmd" &

#####BEGIN: Changes for ARRISXB3-3853
#/etc/ath/fast_down.sh 
#sleep 5
#####END: Changes for ARRISXB3-3853
if [ "$CR_IN_PEER" = "yes" ]
then
	if [ "x"$Subsys = "x" ];then
		$BINPATH/CcspCrSsp
	else
		echo "$BINPATH/CcspCrSsp -subsys $Subsys"
		$BINPATH/CcspCrSsp -subsys $Subsys
	fi
fi
sleep 3

if [ -f "/usr/ccsp/uptime_compare.sh" ]
then
sh /usr/ccsp/uptime_compare.sh &
sleep 10
fi

if [ -e ./wifi ]; then
	cd wifi 
	if [ "x"$Subsys = "x" ];then
        echo_t "****STARTING CCSPWIFI WITHOUT SUBSYS***"
    	$BINPATH/CcspWifiSsp &
	else
        echo_t "****STARTING CCSPWIFI WITH SUBSYS***"
    	echo "$BINPATH/CcspWifiSsp -subsys $Subsys &"
    	$BINPATH/CcspWifiSsp -subsys $Subsys &
	fi
	cd ..
fi



if [ "$BOX_TYPE" = "XB3" ]
then
    echo 2 > /proc/irq/45/smp_affinity
fi

sleep 60 
timer=0

while :
do

if [ -f "/tmp/wifi_initialized" ] && [ -f "/tmp/pam_initialized" ] || [ $timer -ge $MAX_WAIT_TIME ];then


	if [ "$BOX_TYPE" = "XB3" ] && [ -f "/etc/webgui_atom.sh" ]
	then
	   echo_t "Starting web gui"
	   sh /etc/webgui_atom.sh &
	fi

	##### webpa #####
	if [ -f /usr/bin/webpa ];then
		echo "Starting webpa from ATOM side"
	     	/usr/bin/webpa &
		echo "Started webpa on atom"
	fi

	if [ -e ./harvester ]; then
        	echo_t "****STARTING HARVESTER***"
	        cd harvester
        	$BINPATH/harvester &
	        cd ..
	fi

	break
else
	echo "Waiting for Wifi init completion to start Harvester"
	sleep 10
	timer=`expr $timer + 10`
	if [ $timer -eq $MAX_WAIT_TIME ]; then
	    echo "Waited for 5 min, wifi/pam is still not up. Start webpa, harvester, service manager process and break the loop"
	fi
fi

done

echo_t "Monitor ATOM log folder size"
sh /rdklogger/atom_log_monitor.sh &

if [ "$MFG_NAME" = "Cisco" ]; then
    echo_t "starting wifi_default_radius_port_script"
    /usr/ccsp/wifi/wifi_default_radius_port.sh
fi


if [ "$CR_IN_PEER" = "yes" ]
then
	echo_t "Start monitoring system statistics on the atom side"
	CRON_PID=`pidof crond`
	if [ "$CRON_PID" = "" ]
	then
		if [ -f $CRONFILE ]
		then
			sed -i '/log_mem_cpu_info_atom.sh/d' $CRONFILE
			echo "20 * * * * /rdklogger/log_mem_cpu_info_atom.sh" >> $CRONFILE
			echo "0 * * * *  /usr/ccsp/wifi/l2shealth_log.sh" >> $CRONFILE
			echo "35 * * * *  /usr/ccsp/wifi/aphealth_log.sh" >> $CRONFILE
			echo "47 2 * * *  /usr/ccsp/wifi/dailystats_log.sh" >> $CRONFILE
			echo "30 * * * * /rdklogger/log_cpu_info_atom.sh"  >> $CRONFILE
			echo "*/15 * * * * sh /usr/ccsp/wifi/radiohealth.sh" >> $CRONFILE
			echo "1 */12 * * *  /usr/ccsp/wifi/mesh_status.sh" >> $CRONFILE
		else
			if [ ! -d $CRONPATH ]
			then
				mkdir $CRONPATH
			fi
			touch $CRONFILE
			echo "20 * * * * /rdklogger/log_mem_cpu_info_atom.sh" > $CRONFILE
			echo "0 * * * *  /usr/ccsp/wifi/l2shealth_log.sh" >> $CRONFILE
			echo "35 * * * *  /usr/ccsp/wifi/aphealth_log.sh" >> $CRONFILE
			echo "47 2 * * *  /usr/ccsp/wifi/dailystats_log.sh" >> $CRONFILE
			echo "30 * * * * /rdklogger/log_cpu_info_atom.sh"  >> $CRONFILE
			echo "*/15 * * * * sh /usr/ccsp/wifi/radiohealth.sh" >> $CRONFILE
			echo "1 */12 * * *  /usr/ccsp/wifi/mesh_status.sh" >> $CRONFILE
		fi
		crond -c $CRONPATH -l 9
	else
		crontab -l -c $CRONPATH > $CRONFILE_BK
		sed -i '/log_mem_cpu_info_atom.sh/d' $CRONFILE_BK
		echo "20 * * * * /rdklogger/log_mem_cpu_info_atom.sh" >> $CRONFILE_BK
		echo "0 * * * *  /usr/ccsp/wifi/l2shealth_log.sh" >> $CRONFILE_BK
		echo "35 * * * *  /usr/ccsp/wifi/aphealth_log.sh" >> $CRONFILE_BK
		echo "47 2 * * *  /usr/ccsp/wifi/dailystats_log.sh" >> $CRONFILE
		echo "30 * * * * /rdklogger/log_cpu_info_atom.sh" >> $CRONFILE_BK
		echo "*/15 * * * * sh /usr/ccsp/wifi/radiohealth.sh" >> $CRONFILE_BK
		echo "1 */12 * * *  /usr/ccsp/wifi/mesh_status.sh" >> $CRONFILE_BK
		crontab $CRONFILE_BK -c $CRONPATH
		rm -rf $CRONFILE_BK
	fi
fi

if [ "$BOX_TYPE" = "XB3" ]; then
	NVRAM_MOUNT=$(cat /proc/mounts | grep -i nvram | cut -d ' ' -f 2)
	MODE=$(cat /proc/mounts | grep nvram | cut -d ' ' -f 4 | cut -d ',' -f 1)
	if [ "$NVRAM_MOUNT" = "" ]; then
		echo "NVRAM is not mounted" >>$ATOMCONSOLELOGFILE
	fi
	if [ "$MODE" = "ro" ]; then
		echo "NVRAM is mounted as read only" >>$ATOMCONSOLELOGFILE
	fi
fi

if [ "$DEVICE_MODEL"=="TCHXB3" ]; then
    if [ -e "/tmp/.atomnvmnt.log" ]; then
        cat /tmp/.atomnvmnt.log >> /rdklogs/logs/AtomConsolelog.txt.0
        rm -rf /tmp/.atomnvmnt.log
        touch /nvram/.atomnvmnt.log
    fi
fi

rm -rf /tmp/.dropbear

if [ -f "/usr/sbin/tdkb_launcher.sh" ]
then
    echo "Found TDK-B launcher script!!!"
    echo "Invoking TDK-B launcher script!!!"
    sh /usr/sbin/tdkb_launcher.sh &
fi

#Start ocsp
if [ -f /lib/rdk/ocsp-support.sh ]; then
    echo_t "Starting ocsp script"
    sh /lib/rdk/ocsp-support.sh &
fi

if [ "$MODEL_NUM" = "DPC3939B" ] || [ "$MODEL_NUM" = "DPC3941B" ]; then
   if [ -f /usr/ccsp/wifi/disable_lnf.sh ];then
      /usr/ccsp/wifi/disable_lnf.sh
   fi
fi
