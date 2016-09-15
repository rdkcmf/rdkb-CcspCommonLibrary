#!/bin/sh

. /etc/device.properties
. /etc/include.properties

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

echo_t "Elected subsystem is $Subsys"

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

if [ -e /nvram/disableCcspCMAgentSsp ]; then
   echo_t "****DISABLE CcspCMAgentSsp*****"
elif [ -e ./cm ]; then
        cd cm
        if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspCMAgentSsp
        else
        echo_t "$BINPATH/CcspCMAgentSsp -subsys $Subsys"
                $BINPATH/CcspCMAgentSsp -subsys $Subsys
        fi
        cd ..
fi

if [ -e /nvram/webpa_cfg.json ]; then
    echo_t "webpa_cfg.json exists in nvram"
 else
    echo_t "webpa_cfg.json not found in nvram"
    cp /etc/webpa_cfg.json /nvram/webpa_cfg.json
    echo_t "webpa_cfg.json file does not exist. Hence copying the factory default file.."
fi
    
WEBPAVER=`cat /nvram/webpa_cfg.json | grep "file-version" | awk '{print $2}' | sed 's|[\"\",]||g'`
echo_t "WEBPAVER is $WEBPAVER"
if [ "$WEBPAVER" = "" ];then
    cp /etc/webpa_cfg.json /nvram/webpa_cfg.json
    echo_t "Copying factory default file as webpa file-version does not exist in current cfg file.."
fi
    
ENABLEWEBPA=`cat /nvram/webpa_cfg.json | grep EnablePa | awk '{print $2}' | sed 's|[\"\",]||g'`
echo_t "ENABLEWEBPA is $ENABLEWEBPA"

if [ -e /nvram/disablewebpa ]; then
    echo_t "***Disabling webpa*****"
elif [ "$ENABLEWEBPA" = "true" ];then
    echo_t "ENABLEWEBPA is true..Intializing WebPA.."
    if [ "x"$Subsys = "x" ];then
        $BINPATH/webpa
    else
        echo_t "./webpa -subsys $Subsys"
        $BINPATH/webpa -subsys $Subsys
    fi
else
    echo_t "EnablePa parameter is set to false. Hence not initializng WebPA.."
fi

if [ -e /nvram/disableCcspMtaAgentSsp ]; then
   echo_t "****DISABLE MTAAGENTSSP*****"
elif [ -e ./mta ]; then
    cd mta
    if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspMtaAgentSsp
    else
        echo_t "$BINPATH/CcspMtaAgentSsp -subsys $Subsys"
        $BINPATH/CcspMtaAgentSsp -subsys $Subsys
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
   echo_t "****DISABLE CcspTr069PaSsp*****"
elif [ -e ./tr069pa ]; then
        cd tr069pa
        if [ "x"$Subsys = "x" ]; then
                $BINPATH/CcspTr069PaSsp
        else
        echo_t "$BINPATH/CcspTr069PaSsp -subsys $Subsys"
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
   echo_t "****DISABLE CcspTandDSsp*****"
elif [ -e ./tad ]; then
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

echo_t "*** Start CcspSafeNAT ***"
if [ -e ./ccsp-safenat-broadband ]; then
        cd ccsp-safenat-broadband

        if [ "x"$Subsys = "x" ];then
                $BINPATH/CcspSafeNAT
        else
                echo_t "$BINPATH/CcspSafeNAT -subsys $Subsys"
                $BINPATH/CcspSafeNAT -subsys $Subsys
        fi
        cd ..
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
	echo_t "***Disabling CcspLMLite*****"
elif [ -e ./lm ]; then
    cd lm
    echo_t "$BINPATH/CcspLMLite -subsys $Subsys &"
    $BINPATH/CcspLMLite -subsys $Subsys &
fi

echo_t "XCONF SCRIPT : Calling XCONF Client"
cd /fss/gw/etc
./xb3_firmwareDwnld.sh &

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

echo_t "starting rpcserver from arm"
# starting the rpcserver
if [ -f /usr/bin/rpcserver ];then
      /usr/bin/rpcserver &
fi

/etc/utopia/service.d/service_sshd.sh sshd-start &

# Enable SSH between processors for devices having multiple processors alone
if [ "x$MULTI_CORE" == "xyes" ]; then
    dropbear -E -B -p $ARM_INTERFACE_IP:22 &
    /usr/bin/inotify-minidump-watcher /telemetry /lib/rdk/telemetryEventListener.sh 0 "*.cmd" &
fi

# Enable XCONF Conf config fetch
if [ -f  /lib/rdk/dcm.service ]; then
   /bin/sh /lib/rdk/dcm.service &
fi

#RDKB-7535

if [ -f "/rdklogger/rdkbLogMonitor.sh" ]
then
	/rdklogger/rdkbLogMonitor.sh &
fi

if [ -f "/rdklogger/fileUploadRandom.sh" ]
then
	/rdklogger/fileUploadRandom.sh &
fi
