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
    killall CcspWecbController   
fi

export LD_LIBRARY_PATH=$PWD:.:$PWD/lib:/lib:/usr/lib:$LD_LIBRARY_PATH
export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket

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
echo "Starting Config Recover Daemon ..."
conf_rec &

cp ccsp_msg.cfg /tmp

# have IP address for dbus config generated
#./DbusCfg
dbus-daemon --config-file=./basic.conf --fork

touch cp_subsys_ert

if [ -f ./cp_subsys_ert ]; then
    Subsys="eRT."
elif [ -e ./cp_subsys_emg ]; then
    Subsys="eMG."
else
    Subsys=""
fi

echo "Elected subsystem is $Subsys"

if [ "x"$Subsys = "x" ];then
    ./CcspCrSsp
else
    echo "./CcspCrSsp -subsys $Subsys"
    ./CcspCrSsp -subsys $Subsys
fi

#sleep 3
if [ "x"$Subsys = "x" ];then
    ./PsmSsp
else
    echo "./PsmSsp -subsys $Subsys"
    ./PsmSsp -subsys $Subsys
fi

if [ -e ./pam ]; then
    sleep 1
	cd pam
	
    if [ "x"$Subsys = "x" ];then
		./CcspPandMSsp
	else
		echo "./CcspPandMSsp -subsys $Subsys"
		./CcspPandMSsp -subsys $Subsys
	fi

    # double background to detach the script from the tty
	((sh ./email_notification_monitor.sh 12 &) &)

	cd ..
fi

if [ -f ./cp_subsys_ert ]; then
    #sleep 3
	if [ -e ./rm ]; then
	cd rm
        echo "./CcspRmSsp -subsys $Subsys"
		./CcspRmSsp -subsys $Subsys
	cd ..
	fi
fi

#cp ./cherokee/icons/yes.gif /usr/share/cherokee/icons
#cp ./cherokee/icons/add.gif /usr/share/cherokee/icons
#cp ./cherokee/icons/delete.gif /usr/share/cherokee/icons
#cherokee-worker -C ./cherokee/conf/cherokee.conf &

if [ "x"$1 = "xpam" ] || [ "x"$2 = "xpam" ]; then
  exit 0
fi

#cd ../avahi
#$PWD/avahi-daemon --file=$PWD/avahi-daemon.conf -D


# Tr069Pa, as well as SecureSoftwareDownload and FirmwareUpgrade

if [ -e ./wecb ]; then
    sleep 5
    cd wecb
    if [ "x"$Subsys = "x" ];then
        ./CcspWecbController 
    else
        echo "./CcspWecbController -subsys $Subsys"
        ./CcspWecbController -subsys $Subsys 
    fi
    cd ..
fi

if [ -e ./tr069pa ]; then
    sleep 30
    cd tr069pa
    if [ "x"$Subsys = "x" ]; then
        ./CcspTr069PaSsp
    else
        echo "./CcspTr069PaSsp -subsys $Subsys"
        ./CcspTr069PaSsp -subsys $Subsys
    fi
    cd ..
fi

if [ -e ./tr069pa ]; then
    # add firewall rule to allow incoming packet for port 7547
    sysevent setunique GeneralPurposeFirewallRule " -A INPUT -i erouter0 -p tcp --dport=7547 -j ACCEPT "
fi

if [ -e ./cm ]; then
    sleep 5
    cd cm
    if [ "x"$Subsys = "x" ];then
        ./CcspCMAgentSsp 
    else
        echo "./CcspCMAgentSsp -subsys $Subsys"
        ./CcspCMAgentSsp -subsys $Subsys 
    fi
    cd ..
fi

if [ -e ./mta ]; then
    sleep 30
    cd mta
    if [ "x"$Subsys = "x" ];then
        ./CcspMtaAgentSsp 
    else
        echo "./CcspMtaAgentSsp -subsys $Subsys"
        ./CcspMtaAgentSsp -subsys $Subsys 
    fi
    cd ..
fi

if [ -e ./ssd ]; then
    cd ssd
    sleep 1
    if [ "x"$Subsys = "x" ];then
        ./CcspSsdSsp
    else
        echo "./CcspSsdSsp -subsys $Subsys"
        ./CcspSsdSsp -subsys $Subsys
    fi
    cd ..
fi

if [ -e ./fu ]; then
    cd fu
    sleep 1
    if [ "x"$Subsys = "x" ];then
        ./CcspFuSsp
    else
        echo "./CcspFuSsp -subsys $Subsys"
        ./CcspFuSsp -subsys $Subsys
    fi
    cd ..
fi

if [ -e ./tad ]; then
	cd tad
	#delay TaD in order to reduce CPU overload and make PAM ready early
	sleep 3
	if [ "x"$Subsys = "x" ];then
		./CcspTandDSsp
	else
        echo "./CcspTandDSsp -subsys $Subsys"
		./CcspTandDSsp -subsys $Subsys
	fi
	cd ..
fi

sleep 1

if [ -e ./ccspRecoveryManager ]; then
    if [ "x"$Subsys = "x" ];then
        ./ccspRecoveryManager &
    else
        echo "./ccspRecoveryManager -subsys $Subsys &"
        ./ccspRecoveryManager -subsys $Subsys &
    fi
fi

#Lm need initialization after others running
#Sleep 120 is a temporary method
#if [ -e ./lm ]; then
#    sleep 120
#    cd lm
#    if [ "x"$Subsys = "x" ];then
#        ./CcspLmSsp
#    else
#        echo "./CcspLmSsp -subsys $Subsys"
#        ./CcspLmSsp -subsys $Subsys
#    fi
#    cd ..
#fi

if [ -e ./lm ]; then
    cd lm
    sleep 5
    ./CcspLMLite &
fi
