#!/bin/sh

UTOPIA_PATH="/etc/utopia/service.d"
TAD_PATH="/usr/ccsp/tad"
source $TAD_PATH/corrective_action.sh
source $UTOPIA_PATH/log_env_var.sh
exec 3>&1 4>&2 >>$SELFHEALFILE 2>&1

	killall PsmSsp
        killall self_heal_connectivity_test.sh
        killall resource_monitor.sh

	if [ -f "/tmp/logagent_initialized" ]
	then
		rm -rf /tmp/logagent_initialized
	fi
        killall log_agent

	if [ -f "/tmp/pam_initialized" ]
	then
		rm -rf /tmp/pam_initialized
	fi
        killall CcspPandMSsp

	if [ -f "/tmp/wecb_initialized" ]
	then
		rm -rf /tmp/wecb_initialized
	fi
        killall CcspWecbController

	if [ -f "/tmp/Notify_initialized" ]
	then
		rm -rf /tmp/Notify_initialized
	fi
        killall notify_comp
        killall CcspHomeSecurity

	if [ -f "/tmp/cm_initialized" ]
	then
		rm -rf /tmp/cm_initialized
	fi
        killall CcspCMAgentSsp

	if [ -f "/tmp/moca_initialized" ]
	then
		rm -rf /tmp/moca_initialized
	fi
	killall CcspMoCA
        killall CcspTr069PaSsp
        killall CcspTandDSsp

	if [ -f "/tmp/webpa_initialized" ]
	then
		rm -rf /tmp/webpa_initialized
	fi
        killall webpa
        killall CcspSafeNAT

	if [ -f "/tmp/lmlite_initialized" ]
	then
		rm -rf /tmp/lmlite_initialized
	fi
        killall CcspLMLite
        killall CcspXdnsSsp
	if [ -f "/tmp/mta_initialized" ]
	then
		rm -rf /tmp/mta_initialized
	fi
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

        sleep 15
	count=0
	MAX_COUNT=20
	while :
	do
	if [ -f "/tmp/pam_initialized" ]
	then
		echo "PandM initialization complete after atom reset, initializing other CCSP processes"
		break
	else
		echo "Waiting for PandM initialization to complete"
		count=$((count+1))
		sleep 10
		if [ "$count" -ge "$MAX_COUNT" ]
		then
			echo "PandM initialization reaches MAX time out, breaking the loop"
			break
		fi	
		

	fi
	done
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

	sleep 3
	echo "RDKB_SELFHEAL : Resetting process CcspMoCA on atom reset"

	cd /usr/ccsp/moca
	if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspMoCA
        else
	$BINPATH/CcspMoCA -subsys $Subsys
	fi

	sleep 3
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

	sleep 3
	 #td
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspTandDSsp on atom reset"
	cd /usr/ccsp/tad
	if [ "x"$Subsys = "x" ];then
                $BINPATH/CcspTandDSsp
        else
                $BINPATH/CcspTandDSsp -subsys $Subsys
        fi

	sleep 3
	 #safenet
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspSafeNAT on atom reset"
	cd /usr/ccsp/ccsp-safenat-broadband
	if [ "x"$Subsys = "x" ];then
                $BINPATH/CcspSafeNAT
        else
                $BINPATH/CcspSafeNAT -subsys $Subsys
        fi

	sleep 3
	#CcspWiecbController
        echo "RDKB_SELFHEAL : Resetting process CcspWecbController on atom reset"
	cd /usr/ccsp/wecb
	if [ "x"$Subsys = "x" ];then
        	$BINPATH/CcspWecbController
	else
       		$BINPATH/CcspWecbController -subsys $Subsys
    	fi

	sleep 3
	 #CcspLMLite
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspLMLite on atom reset"
	cd /usr/ccsp/lm
	$BINPATH/CcspLMLite -subsys $Subsys

	sleep 2

 #CcspXdnsSsp
    echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspXdnsSsp on atom reset"
    cd /usr/ccsp/xdns
    $BINPATH/CcspXdnsSsp -subsys $Subsys

	xfinityenable=`psmcli get dmsb.hotspot.enable`

	if [ $xfinityenable -eq 1 ];then
	 #CcspHotspot
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspHotspot on atom reset"
	cd /usr/ccsp/hotspot
	$BINPATH/CcspHotspot -subsys $Subsys > /dev/null &
	cd -
	fi

	sleep 2
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

	sleep 2
	echo "RDKB_SELFHEAL : Resetting process log_agent on atom reset"

        cd /usr/ccsp/logagent

        if [ "x"$Subsys = "x" ];then
                $BINPATH/log_agent
        else
                echo "$BINPATH/log_agent -subsys $Subsys"
                $BINPATH/log_agent -subsys $Subsys
        fi
