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

killall CcspWifiSsp
killall harvester
killall CcspCrSsp
killall rpcserver
# have IP address for dbus config generated
vconfig add eth0 500
ifconfig eth0.500 192.168.101.3

# have IP address for Radius config
vconfig add eth0 4090
ifconfig eth0.4090 192.168.251.254 netmask 255.255.255.0 up
ip route add default via 192.168.251.1

# Creating ATOM side vlan for IOT and assigning IP address
vconfig add eth0 106
ifconfig eth0.106 192.168.106.254 netmask 255.255.255.0 up
ip route add default via 192.168.106.1

#start dropbear
dropbear -E -B -p 192.168.101.3:22 -b /etc/sshbanner.txt&

export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
#####BEGIN: Changes for ARRISXB3-3853
export PATH=$PATH:/etc/ath
echo "PATH="$PATH
export LOG4C_RCPATH=/rdklogger

LOG_FOLDER="/rdklogs/logs/"

mkdir -p $LOG_FOLDER

if [ "$CR_IN_PEER" = "yes" ]
then
	ATOMCONSOLELOGFILE="$LOG_FOLDER/AtomConsolelog.txt.0"
	exec 3>&1 4>&2 >>$ATOMCONSOLELOGFILE 2>&1
fi

#####END: Changes for ARRISXB3-3853

# upgrade the wifi config to suppot vlan 106
# This script is only for ARRISXB3 and Ciscoxb3. All other product do not need this step.
rdk_wifi_upgrade_vlan106

# Config vlan 106 Secure ssid (phrase 2): RDKB-6974
/usr/ccsp/wifi/rdk_wifi_configure_lnf_ap.sh

# enable core files on atom
ulimit -c unlimited
echo "/tmp/core.%e.%p" > /proc/sys/kernel/core_pattern

cp -f ccsp_msg.cfg /tmp

if [ "x"$1 = "x" ];then
    sleep 10
fi

Subsys="eRT."

echo "Elected subsystem is $Subsys"

sleep 1


echo "Starting telnet"
/usr/sbin/telnetd -b 192.168.101.3

echo "Starting inotify watcher for telemetry"
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

if [ -e ./harvester ]; then
	echo "****STARTING HARVESTER***"
        cd harvester
        $BINPATH/harvester &
	cd ..
fi

if [ -e ./ccsp-servicemanager-broadband ]; then
        echo "****STARTING SERVICEMANAGER***"
        cd ccsp-servicemanager-broadband
        if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspServiceManager &
        else
        echo "$BINPATH/CcspServiceManager -subsys $Subsys &"
        $BINPATH/CcspServiceManager -subsys $Subsys &
        fi
        cd ..
fi

if [ -e ./wifi ]; then
	cd wifi 
	if [ "x"$Subsys = "x" ];then
        echo "****STARTING CCSPWIFI WITHOUT SUBSYS***"
    	$BINPATH/CcspWifiSsp &
	else
        echo "****STARTING CCSPWIFI WITH SUBSYS***"
    	echo "$BINPATH/CcspWifiSsp -subsys $Subsys &"
    	$BINPATH/CcspWifiSsp -subsys $Subsys &
	fi
fi

echo "starting process monitor script"
sh /usr/ccsp/wifi/process_monitor_atom.sh &

echo "Monitor ATOM log folder size"
sh /rdklogger/atom_log_monitor.sh &


if [ -f "/usr/ccsp/tdk_start.sh" ]
then
    if [ -f /version.txt ]
    then
        echo "version.txt exists"
        imgname=`cat /version.txt | grep -i 'imagename'`
        echo $imgname
        flag=`echo $imgname|awk '{print match($0,"TDK")}'`;
        if [ $flag -gt 0 ];then
            echo "Found TDK Image"
            echo "Invoking TDK start up script"
            sh /usr/ccsp/tdk_start.sh &
        fi
    fi
fi

# starting the rpcserver
echo "starting rpcserver atom"
if [ -f /usr/bin/rpcserver ];then
      /usr/bin/rpcserver &
fi

if [ "$BOX_TYPE" = "XB3" ] && [ -f "/etc/webgui_atom.sh" ]
then
   echo "Starting web gui"
   sh /etc/webgui_atom.sh &
fi

