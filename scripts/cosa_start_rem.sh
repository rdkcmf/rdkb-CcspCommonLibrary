#!/bin/sh

source /etc/utopia/service.d/log_capture_path.sh

cd /fss/gw/usr/ccsp/

export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket

isCloudCapable=1
isCloudCapable=`syscfg get cloud_capable_flag`
if [ $isCloudCapable -eq 0 ]
then
	syscfg set cloud_capable_flag 1
	syscfg commit
fi

if [ -f ./cp_subsys_ert ]; then
        Subsys="eRT."
elif [ -e ./cp_subsys_emg ]; then
        Subsys="eMG."
else
        Subsys=""
fi

echo "Elected subsystem is $Subsys"

if [ -e ./pam ]; then
    cd pam
        #double background to detach the script from the tty
        ((sh ./email_notification_monitor.sh 12 &) &)
        cd ..
fi
 
echo "PWD is `pwd`"

if [ -e ./mta ]; then
    cd mta
    if [ "x"$Subsys = "x" ];then
        ./CcspMtaAgentSsp
    else
        echo "./CcspMtaAgentSsp -subsys $Subsys"
        ./CcspMtaAgentSsp -subsys $Subsys
    fi
    cd ..
fi

if [ -e ./cm ]; then
        cd cm
        if [ "x"$Subsys = "x" ];then
        ./CcspCMAgentSsp
        else
        echo "./CcspCMAgentSsp -subsys $Subsys"
                ./CcspCMAgentSsp -subsys $Subsys
        fi
        cd ..
fi

if [ -f ./cp_subsys_ert ]; then
        if [ -e ./rm ]; then
                cd rm
        echo "./CcspRmSsp -subsys $Subsys"
                ./CcspRmSsp -subsys $Subsys
        cd ..
        fi
fi

if [ "x"$1 = "xpam" ] || [ "x"$2 = "xpam" ]; then
	exit 0
fi

#cd ../avahi
#$PWD/avahi-daemon --file=$PWD/avahi-daemon.conf -D

# Tr069Pa, as well as SecureSoftwareDownload and FirmwareUpgrade


if [ -e ./tr069pa ]; then
        cd tr069pa
        if [ "x"$Subsys = "x" ]; then
                ./CcspTr069PaSsp
        else
        echo "./CcspTr069PaSsp -subsys $Subsys"
                ./CcspTr069PaSsp -subsys $Subsys
        fi
        cd ..
fi

if [ -e ./ssd ]; then
        cd ssd
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
        if [ "x"$Subsys = "x" ];then
                ./CcspFuSsp
        else
                echo "./CcspFuSsp -subsys $Subsys"
                ./CcspFuSsp -subsys $Subsys
        fi
        cd ..
fi
       
if [ -e ./tr069pa ]; then
# add firewall rule to allow incoming packet for port 7547
sysevent setunique GeneralPurposeFirewallRule " -A INPUT -i erouter0 -p tcp --dport=7547 -j ACCEPT "
fi

#fi

if [ -e ./tad ]; then
        cd tad
        #delay TaD in order to reduce CPU overload and make PAM ready early
        if [ "x"$Subsys = "x" ];then
                ./CcspTandDSsp
        else
        echo "./CcspTandDSsp -subsys $Subsys"
                ./CcspTandDSsp -subsys $Subsys
        fi
        cd ..
fi

#if [ -e ./webpa ]; then
#	cd webpa
#	
#	if [ "x"$Subsys = "x" ];then
#		./webpa
#	else
#		echo "./webpa -subsys $Subsys"
#		./webpa -subsys $Subsys
#	fi
#	cd ..
#fi

if [ -e ./webpa ]; then
	ENABLEWEBPA=`cat /nvram/webpa_cfg.json | grep -r EnablePa | awk '{print $2}' | sed 's|[\"\",]||g'`
	echo "ENABLEWEBPA is $ENABLEWEBPA"
	if [ "$ENABLEWEBPA" = "" ];then
		echo "EnablePa parameter is not available in webpa_cfg.json file. \n Hence adding the parameter to json file and setting it to default: false\n"
		`sed -i '2i \        "EnablePa": "false",' /nvram/webpa_cfg.json`
	elif [ "$ENABLEWEBPA" = "true" ];then
		echo "ENABLEWEBPA is true..Intializing WebPA.."
		cd webpa
		if [ "x"$Subsys = "x" ];then
			./webpa
		else
			echo "./webpa -subsys $Subsys"
			./webpa -subsys $Subsys
		fi
		cd ..
	else
		echo "EnablePa parameter is set to false. Hence not initializng WebPA.."
	fi
fi

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
#if [ "x"$Subsys = "x" ];then
#    ./CcspLmSsp
#else
#   echo "./CcspLmSsp -subsys $Subsys"
#   ./CcspLmSsp -subsys $Subsys
#fi
#cd ..
#fi

if [ -e ./lm ]; then
    cd lm
    ./CcspLMLite &
fi

echo "XCONF SCRIPT : Calling XCONF Client"
cd /fss/etc
./xb3_firmwareDwnld.sh &

echo "Running process monitoring script"
/etc/process_monitor.sh &
