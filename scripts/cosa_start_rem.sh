#!/bin/sh

. /etc/device.properties
. /etc/include.properties

source /etc/utopia/service.d/log_capture_path.sh


BINPATH="/usr/bin"
UTOPIA_PATH=/etc/utopia/service.d

cd /fss/gw/usr/ccsp/


export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket
export LOG4C_RCPATH=/fss/gw/rdklogger

if [ -f /tmp/cosa_start_rem_triggered ]; then
	echo_t "Already cosa_start_rem.sh script was triggered"
	exit
else
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


if [ "x$ENABLE_SNMPv3" == "xtrue" ]; then
    if [ -f /lib/rdk/run_snmpv3_master.sh ]; then
        /lib/rdk/run_snmpv3_master.sh &
    fi
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

HOME_LAN_ISOLATION=`psmcli get dmsb.l2net.HomeNetworkIsolation`
if [ "$HOME_LAN_ISOLATION" = "1" ]; then
echo "Starting brlan10 initialization, check whether brlan10 is there or not"
ifconfig | grep brlan10
if [ $? == 1 ]; then
    echo "brlan10 not present go ahead and create it"
    if [ "$BOX_TYPE" = "XF3" ]; then
        sh /usr/ccsp/lan_handler.sh home_lan_isolation_enable
    else
        sysevent set multinet-up 9
    fi
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
    if [ "$BOX_TYPE" = "XF3" ]; then
    	sh /etc/utopia/service.d/service_mcastproxy.sh mcastproxy-restart
    else 
        sysevent set mcastproxy-restart
    fi
    MRD &
    sysevent set firewall-restart

fi

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

# Enable XCONF Conf config fetch
if [ -f  /lib/rdk/dcm.service ]; then
    echo_t "DCM SCRIPT: Calling dcm.service"
    /bin/sh /lib/rdk/dcm.service &
fi

if [ "x$BOX_TYPE" = "xXB3" ] || [ "x$BOX_TYPE" = "xTCCBR" ]; then
    if [ -f "/etc/PARODUS_ENABLE" ]; then
        echo_t "Parodus is enabled"
    else
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

    if [ "$ENABLEWEBPA" = "true" ]; then
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
    fi
fi

isPeriodicFWCheckEnable=`syscfg get PeriodicFWCheck_Enable`
if [ "$isPeriodicFWCheckEnable" == "false" ]; then
	echo_t "XCONF SCRIPT : Calling XCONF Client"
	/etc/xb3_firmwareDwnld.sh &
fi

sleep 5 

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

sleep 5 

#if [ -e ./wecb ];
#then
#    cd wecb
#   if [ "x"$Subsys = "x" ];then
#        $BINPATH/CcspWecbController
#    else
#       echo "$BINPATH/CcspWecbController -subsys $Subsys"
#        $BINPATH/CcspWecbController -subsys $Subsys
#    fi
 #   cd ..
#fi

sleep 5 

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

sleep 5 

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

#<Disabled for now- TBD for later implementation>#Adapter for SNMP via WebPA
#if [ -f "/etc/PARODUS_ENABLE" ]; then
#    echo_t "Starting Parodus2Snmp in background.. "
#    if [ -f "$BINPATH/parodus2snmp" ]; then
#        echo_t "$BINPATH/parodus2snmp"
#        $BINPATH/parodus2snmp &
#    else
#        echo_t "Could not start Parodus2Snmp !!!"
#    fi
#fi


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

#sleep 5 

#echo_t "*** Start CcspSafeNAT ***"
#if [ -e ./ccsp-safenat-broadband ]; then
#        cd ccsp-safenat-broadband
#
#        if [ "x"$Subsys = "x" ];then
#                $BINPATH/CcspSafeNAT
#        else
#                echo_t "$BINPATH/CcspSafeNAT -subsys $Subsys"
#                $BINPATH/CcspSafeNAT -subsys $Subsys
#        fi
#        cd ..
#fi

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

sleep 5 

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
    $BINPATH/CcspLMLite -subsys $Subsys &
    cd ..
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

#Changes done for RDKB-8751
#SELFHEAL_ENABLE=`syscfg get selfheal_enable`
#if [ "$SELFHEAL_ENABLE" == "false" ]
#then
#	echo_t "Running process monitoring script"
#	/etc/process_monitor.sh &
#fi

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
    echo "starting drop bear"
    dropbear -E -s -p $ARM_INTERFACE_IP:22 -P /var/run/dropbear_ipc.pid > /dev/null 2>&1 &
    /usr/bin/inotify-minidump-watcher /telemetry /lib/rdk/telemetryEventListener.sh 0 "*.cmd" &
fi

echo "Enable DCM RFC feature"
# Enable DCM RFC feature RDKB-8798
if [ -f  /lib/rdk/dcmrfc.service ]; then
   /bin/sh /lib/rdk/dcmrfc.service &
fi

# Check zram and enable based on RFC
if [ -f  /lib/rdk/rdkzram.service ]; then
   /bin/sh /lib/rdk/rdkzram.service &
fi

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

sleep 5 

if [ -e ./xdns ]; then
    cd xdns
    echo_t "$BINPATH/CcspXdnsSsp -subsys $Subsys &"
    $BINPATH/CcspXdnsSsp -subsys $Subsys &
    cd ..
fi

if [ -e ./advsec ]; then
    cd advsec
    echo_t "$BINPATH/CcspAdvSecuritySsp -subsys $Subsys &"
    $BINPATH/CcspAdvSecuritySsp -subsys $Subsys &
    cd ..
fi


if [ -e ./CcspArmMdc ]; then
    # make sure MDC firewall rules are disabled at boot
    syscfg set MDC-ENABLED 0
    cd CcspArmMdc
    echo "Starting ARM MDC"
    echo "$BINPATH/Arm_Mdc -subsys $Subsys"
    $BINPATH/Arm_Mdc -subsys $Subsys
    cd ..
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

#waiting for DCM service to complete
sleep 5

isPeriodicFWCheckEnabled=`syscfg get PeriodicFWCheck_Enable`
if [ "$isPeriodicFWCheckEnabled" == "true" ]; then
	echo "XCONF SCRIPT : Calling XCONF Client"
	/etc/firmwareSched.sh &
fi

if [ "x$BOX_TYPE" = "xXB3" ] && [ "x$PARTNER_ID" = "xcox" ]; then
echo_t "starting wan_ssh script"              
sh /lib/rdk/wan_ssh.sh &    
fi 

if [ [[ "x$BOX_TYPE" = "xXB3" ] || [ "x$BOX_TYPE" = "xTCCBR" ]]  && [-f "/etc/PARODUS_ENABLE" ]]; then
    # Checking parodus and parodusStart PID
    PARODUS_PID=`pidof parodus`
    PARODUSSTART_PID=`pidof parodusStart`
    if [ "$PARODUS_PID" = "" ] && ["$PARODUSSTART_PID" = ""]; then 
     	echo_t "wan-status ready event is not received.. Starting parodus in background "
     	$BINPATH/parodusStart &
    else
    	echo_t "Parodus is started or start up is in progress"
    fi
fi
