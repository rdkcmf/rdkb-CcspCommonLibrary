#!/bin/sh

UTOPIA_PATH="/etc/utopia/service.d"
TAD_PATH="/usr/ccsp/tad"
source $TAD_PATH/corrective_action.sh
source $UTOPIA_PATH/log_env_var.sh
exec 3>&1 4>&2 >>$SELFHEALFILE 2>&1

	killall PsmSsp
        killall self_heal_connectivity_test.sh
        killall resource_monitor.sh
        killall log_agent
        killall CcspPandMSsp
        killall CcspWecbController
        killall notify_comp
        killall CcspHomeSecurity
        killall CcspCMAgentSsp
	killall CcspMoCA
        killall CcspTr069PaSsp
        killall CcspTandDSsp
        killall webpa
        killall CcspSafeNAT
        killall CcspLMLite
        killall CcspMtaAgentSsp
        killall CcspHotspot


export LD_LIBRARY_PATH=$PWD:.:$PWD/../../lib:$PWD/../../.:/lib:/usr/lib:$LD_LIBRARY_PATH
export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket
BINPATH="/usr/bin"
source /etc/utopia/service.d/log_env_var.sh                                 
source /etc/device.properties                                               
export LOG4C_RCPATH=/rdklogger
                                                                      
ulimit -c unlimited 

	if [ -f /tmp/cp_subsys_ert ]; then
        	Subsys="eRT."
	elif [ -e ./cp_subsys_emg ]; then
        	Subsys="eMG."
	else
        	Subsys=""
	fi
	keepalive_args="-n `sysevent get wan_ifname` -e 1"

	echo "RDKB_SELFHEAL : Resetting process PsmSsp on atom reset"
	 if [ "x"$Subsys = "x" ];then
                $BINPATH/PsmSsp
        else
                echo "$BINPATH/PsmSsp -subsys $Subsys"
                $BINPATH/PsmSsp -subsys $Subsys
        fi

        sleep 20

	echo "RDKB_SELFHEAL : Resetting process log_agent on atom reset"

        cd /usr/ccsp/logagent

        if [ "x"$Subsys = "x" ];then
                $BINPATH/log_agent
        else
                echo "$BINPATH/log_agent -subsys $Subsys"
                $BINPATH/log_agent -subsys $Subsys
        fi
	
	#notify-comp
        echo "RDKB_SELFHEAL : Resetting process notify-comp on atom reset"

	cd /usr/ccsp/notify-comp
	if [ "x"$Subsys = "x" ];then
                $BINPATH/notify_comp
        else
                $BINPATH/notify_comp -subsys $Subsys
        fi

	# PandM restart
	echo "RDKB_SELFHEAL : Resetting process CcspPandMSsp on atom reset"
	cd /usr/ccsp/pam/
	 if [ "x"$Subsys = "x" ];then
                $BINPATH/CcspPandMSsp
        else
		 $BINPATH/CcspPandMSsp -subsys $Subsys
	fi
	# We need to check whether to enable captive portal flag
	checkCaptivePortal
        sleep 3
       
	#CcspCMAgentSsp
	echo "RDKB_SELFHEAL : Resetting process CcspCMAgentSsp on atom reset"
	 cd /usr/ccsp/cm
	if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspCMAgentSsp
        else
             $BINPATH/CcspCMAgentSsp -subsys $Subsys
        fi

	echo "RDKB_SELFHEAL : Resetting process CcspMoCA on atom reset"

	cd /usr/ccsp/moca
	if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspMoCA
        else
	$BINPATH/CcspMoCA -subsys $Subsys
	fi

	
	#mta
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspMtaAgentSsp on atom reset"
	cd /usr/ccsp/mta
	if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspMtaAgentSsp
   	 else
	$BINPATH/CcspMtaAgentSsp -subsys $Subsys
        fi


	sleep 3
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process webpa on atom reset"
	cd /usr/ccsp/webpa
	if [ "x"$Subsys = "x" ];then
        $BINPATH/webpa
	 else
        $BINPATH/webpa -subsys $Subsys
    	fi


	 #td
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspTandDSsp on atom reset"
	cd /usr/ccsp/tad
	if [ "x"$Subsys = "x" ];then
                $BINPATH/CcspTandDSsp
        else
                $BINPATH/CcspTandDSsp -subsys $Subsys
        fi

	 #safenet
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspSafeNAT on atom reset"
	cd /usr/ccsp/ccsp-safenat-broadband
	if [ "x"$Subsys = "x" ];then
                $BINPATH/CcspSafeNAT
        else
                $BINPATH/CcspSafeNAT -subsys $Subsys
        fi


	#CcspWiecbController
        echo "RDKB_SELFHEAL : Resetting process CcspWecbController on atom reset"
	cd /usr/ccsp/wecb
	if [ "x"$Subsys = "x" ];then
        	$BINPATH/CcspWecbController
	else
       		$BINPATH/CcspWecbController -subsys $Subsys
    	fi

	 #CcspLMLite
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspLMLite on atom reset"
	cd /usr/ccsp/lm
	$BINPATH/CcspLMLite -subsys $Subsys

	
	xfinityenable=`psmcli get dmsb.hotspot.enable`

	if [ $xfinityenable -eq 1 ];then
	 #CcspHotspot
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspHotspot on atom reset"
	cd /usr/ccsp/hotspot
	$BINPATH/CcspHotspot -subsys $Subsys > /dev/null &
	cd -
	fi

	#CcspHomeSecurity
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspHomeSecurity on atom reset"
	CcspHomeSecurity 8081&
	sleep 5

	#CcspTr069PaSsp
        echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspTr069PaSsp on atom reset"
	cd /usr/ccsp/tr069pa
        if [ "x"$Subsys = "x" ]; then
                $BINPATH/CcspTr069PaSsp
        else
		$BINPATH/CcspTr069PaSsp -subsys $Subsys
        fi
	
