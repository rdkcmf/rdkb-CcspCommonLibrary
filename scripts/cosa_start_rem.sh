#!/bin/sh

source /etc/utopia/service.d/log_capture_path.sh

BINPATH="/usr/bin"

cd /fss/gw/usr/ccsp/


export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket
export LOG4C_RCPATH=/fss/gw/rdklogger

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

echo "Elected subsystem is $Subsys"

if [ -e ./pam ]; then
    cd pam
        #double background to detach the script from the tty
        ((sh /etc/email_notification_monitor.sh 12 &) &)
        cd ..
fi
echo "Enabling ssh by default"
syscfg set mgmt_wan_sshaccess 1
syscfg commit 
echo "PWD is `pwd`"

if [ -e /nvram/disableCcspMtaAgentSsp ]; then
   echo "****DISABLE MTAAGENTSSP*****"
elif [ -e ./mta ]; then
    cd mta
    if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspMtaAgentSsp
    else
        echo "$BINPATH/CcspMtaAgentSsp -subsys $Subsys"
        $BINPATH/CcspMtaAgentSsp -subsys $Subsys
    fi
    cd ..
fi

if [ -e /nvram/disableCcspCMAgentSsp ]; then
   echo "****DISABLE CcspCMAgentSsp*****"
elif [ -e ./cm ]; then
        cd cm
        if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspCMAgentSsp
        else
        echo "$BINPATH/CcspCMAgentSsp -subsys $Subsys"
                $BINPATH/CcspCMAgentSsp -subsys $Subsys
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

if [ -e /nvram/disableCcspTr069PaSsp ]; then
   echo "****DISABLE CcspTr069PaSsp*****"
elif [ -e ./tr069pa ]; then
        cd tr069pa
        if [ "x"$Subsys = "x" ]; then
                $BINPATH/CcspTr069PaSsp
        else
        echo "$BINPATH/CcspTr069PaSsp -subsys $Subsys"
                $BINPATH/CcspTr069PaSsp -subsys $Subsys
        fi
        sysevent setunique GeneralPurposeFirewallRule " -A INPUT -i erouter0 -p tcp --dport=7547 -j ACCEPT "
        cd ..
fi

#if [ -e /nvram/disableCcspSsdSsp ]; then
#   echo "****DISABLE CcspSsdSsp*****"
#elif [ -e ./ssd ]; then
#        cd ssd
#        if [ "x"$Subsys = "x" ];then
#                $BINPATH/CcspSsdSsp
#        else
#                echo "$BINPATH/CcspSsdSsp -subsys $Subsys"
#                $BINPATH/CcspSsdSsp -subsys $Subsys
#        fi
#        cd ..
#fi

#if [ -e /nvram/disableCcspFuSsp ]; then
#   echo "****DISABLE CcspFuSsp*****"
#elif [ -e ./fu ]; then
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

if [ -e /nvram/disableCcspTandDSsp ]; then
   echo "****DISABLE CcspTandDSsp*****"
elif [ -e ./tad ]; then
        cd tad
        #delay TaD in order to reduce CPU overload and make PAM ready early
        if [ "x"$Subsys = "x" ];then
                $BINPATH/CcspTandDSsp
        else
        echo "$BINPATH/CcspTandDSsp -subsys $Subsys"
                $BINPATH/CcspTandDSsp -subsys $Subsys
        fi
        cd ..
fi

        
if [ -e /nvram/webpa_cfg.json ]; then
	echo "webpa_cfg.json exists in nvram"
 else
	echo "webpa_cfg.json not found in nvram"
	cp /etc/webpa_cfg.json /nvram/webpa_cfg.json
	echo "webpa_cfg.json file does not exist. Hence copying the factory default file.."
fi
        
WEBPAVER=`cat /nvram/webpa_cfg.json | grep "file-version" | awk '{print $2}' | sed 's|[\"\",]||g'`
echo "WEBPAVER is $WEBPAVER"
if [ "$WEBPAVER" = "" ];then
	cp /etc/webpa_cfg.json /nvram/webpa_cfg.json
	echo "Copying factory default file as webpa file-version does not exist in current cfg file.."
fi
	
ENABLEWEBPA=`cat /nvram/webpa_cfg.json | grep EnablePa | awk '{print $2}' | sed 's|[\"\",]||g'`
echo "ENABLEWEBPA is $ENABLEWEBPA"

if [ -e /nvram/disablewebpa ]; then
	echo "***Disabling webpa*****"
elif [ "$ENABLEWEBPA" = "true" ];then
	echo "ENABLEWEBPA is true..Intializing WebPA.."
	if [ "x"$Subsys = "x" ];then
		$BINPATH/webpa
	else
		echo "./webpa -subsys $Subsys"
		$BINPATH/webpa -subsys $Subsys
	fi
else
	echo "EnablePa parameter is set to false. Hence not initializng WebPA.."
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

if [ -e /nvram/disableCcspLMLite ]; then
	echo "***Disabling CcspLMLite*****"
elif [ -e ./lm ]; then
    cd lm
    echo "$BINPATH/CcspLMLite -subsys $Subsys &"
    $BINPATH/CcspLMLite -subsys $Subsys &
fi

echo "XCONF SCRIPT : Calling XCONF Client"
cd /fss/gw/etc
./xb3_firmwareDwnld.sh &

SELFHEAL_ENABLE=`syscfg get selfheal_enable`
if [ "$SELFHEAL_ENABLE" == "false" ]
then
	echo "Running process monitoring script"
	/etc/process_monitor.sh &
fi

# starting the minidump watcher & uploader
if [ -f /usr/bin/inotify-minidump-watcher ];then
      /usr/bin/inotify-minidump-watcher /minidumps "*.dmp" &
fi

/etc/utopia/service.d/service_sshd.sh sshd-start &

# Enable XCONF Conf config fetch
if [ -f  /lib/rdk/dcm.service ]; then
   /bin/sh /lib/rdk/dcm.service &
fi
