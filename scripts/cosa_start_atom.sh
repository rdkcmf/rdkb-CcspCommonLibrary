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
ifconfig eth0.500 $ATOM_INTERFACE_IP

# have IP address for Radius config
vconfig add eth0 4090
ifconfig eth0.4090 192.168.251.254 netmask 255.255.255.0 up
ip route add default via 192.168.251.1

# Creating ATOM side vlan for IOT and assigning IP address
vconfig add eth0 106
ifconfig eth0.106 192.168.106.254 netmask 255.255.255.0 up
ip route add default via 192.168.106.1

#start dropbear
echo "starting dropbear ATOM"
dropbear -E -B -p $ATOM_INTERFACE_IP:22 > /dev/null 2>&1 &

export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
#####BEGIN: Changes for ARRISXB3-3853
export PATH=$PATH:/etc/ath
echo "PATH="$PATH
export LOG4C_RCPATH=/rdklogger

LOG_FOLDER="/rdklogs/logs/"
CRONPATH="/tmp/cron/"
CRONFILE=$CRONPATH"root"
CRONFILE_BK="/tmp/cron_tab.txt"

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

# upgrade the wifi config to suppot vlan 106
# This script is only for ARRISXB3 and Ciscoxb3. All other product do not need this step.
rdk_wifi_upgrade_vlan106

# Config vlan 106 Secure ssid (phrase 2): RDKB-6974
/lib/rdk/rdk_wifi_configure_lnf_ap.sh

# enable core files on atom
ulimit -c unlimited
echo "/tmp/core.%e.%p" > /proc/sys/kernel/core_pattern

cp -f ccsp_msg.cfg /tmp

if [ "x"$1 = "x" ];then
    sleep 10
fi

Subsys="eRT."

echo_t "Elected subsystem is $Subsys"

sleep 1


echo_t "Starting telnet"
/usr/sbin/telnetd -b $ATOM_INTERFACE_IP

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
sleep 3
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

echo_t "starting process monitor script"
sh /usr/ccsp/wifi/process_monitor_atom.sh &

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
echo_t "starting rpcserver atom"
if [ -f /usr/bin/rpcserver ];then
      /usr/bin/rpcserver &
fi

if [ "$BOX_TYPE" = "XB3" ] && [ -f "/etc/webgui_atom.sh" ]
then
   echo_t "Starting web gui"
   sh /etc/webgui_atom.sh &
fi

sleep 60 
while :
do

if [ -f "/tmp/wifi_initialized" ]
then
	if [ -e ./harvester ]; then
        	echo_t "****STARTING HARVESTER***"
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
	break
else
	echo "Waiting for Wifi init completion to start Harvester"
	sleep 10
fi

done

echo_t "Monitor ATOM log folder size"
sh /rdklogger/atom_log_monitor.sh &

if [ "$CR_IN_PEER" = "yes" ]
then
	echo_t "Start monitoring system statistics on the atom side"
	CRON_PID=`pidof crond`
	if [ "$CRON_PID" = "" ]
	then
		if [ -f $CRONFILE ]
		then
			sed -i '/log_mem_cpu_info_atom.sh/d' $CRONFILE
			echo "54 * * * * /rdklogger/log_mem_cpu_info_atom.sh" >> $CRONFILE
                        echo "3 * * * *  /usr/ccsp/wifi/radiohealth_log.sh" >> $CRONFILE
			echo "1 * * * *  /usr/ccsp/wifi/aphealth_log.sh" >> $CRONFILE
			echo "2 * * * *  /usr/ccsp/wifi/stahealth_log.sh" >> $CRONFILE
			echo "5,35 * * * *  /usr/ccsp/wifi/l2shealth_log.sh" >> $CRONFILE
			echo "0 * * * *  /usr/ccsp/wifi/bandsteering_log.sh" >> $CRONFILE
		else
			if [ ! -d $CRONPATH ]
			then
				mkdir $CRONPATH
			fi
			touch $CRONFILE
			echo "54 * * * * /rdklogger/log_mem_cpu_info_atom.sh" > $CRONFILE
                        echo "3 * * * *  /usr/ccsp/wifi/radiohealth_log.sh" >> $CRONFILE
                        echo "1 * * * *  /usr/ccsp/wifi/aphealth_log.sh" >> $CRONFILE
                        echo "2 * * * *  /usr/ccsp/wifi/stahealth_log.sh" >> $CRONFILE
			echo "5,35 * * * *  /usr/ccsp/wifi/l2shealth_log.sh" >> $CRONFILE
			echo "0 * * * *  /usr/ccsp/wifi/bandsteering_log.sh" >> $CRONFILE
		fi
		crond -c $CRONPATH -l 9
	else
		crontab -l -c $CRONPATH > $CRONFILE_BK
		sed -i '/log_mem_cpu_info_atom.sh/d' $CRONFILE_BK
		echo "54 * * * * /rdklogger/log_mem_cpu_info_atom.sh" >> $CRONFILE_BK
                echo "3 * * * *  /usr/ccsp/wifi/radiohealth_log.sh" >> $CRONFILE_BK
		echo "1 * * * *  /usr/ccsp/wifi/aphealth_log.sh" >> $CRONFILE_BK
		echo "2 * * * *  /usr/ccsp/wifi/stahealth_log.sh" >> $CRONFILE_BK
		echo "5,35 * * * *  /usr/ccsp/wifi/l2shealth_log.sh" >> $CRONFILE_BK
		echo "0 * * * *  /usr/ccsp/wifi/bandsteering_log.sh" >> $CRONFILE_BK
		crontab $CRONFILE_BK -c $CRONPATH
		rm -rf $CRONFILE_BK
	fi
fi
