#!/bin/sh
####################################################################################
# If not stated otherwise in this file or this component's Licenses.txt file the
# following copyright and licenses apply:

#  Copyright 2018 RDK Management

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

# http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#######################################################################################

UTOPIA_PATH="/etc/utopia/service.d"
TAD_PATH="/usr/ccsp/tad"
source $TAD_PATH/corrective_action.sh
source $UTOPIA_PATH/log_env_var.sh
exec 3>&1 4>&2 >>$SELFHEALFILE 2>&1

	killall PsmSsp
    killall self_heal_connectivity_test.sh
    killall resource_monitor.sh

	if [ -f "/tmp/pam_initialized" ]
	then
		rm -rf /tmp/pam_initialized
	fi
        killall CcspPandMSsp

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
	if [ "$MODEL_NUM" = "DPC3939B" ] || [ "$MODEL_NUM" = "DPC3941B" ]; then
		echo_t "Disabling MTA for BWG "
	else
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspMtaAgentSsp on atom reset"
	cd /usr/ccsp/mta
	if [ "x"$Subsys = "x" ];then
        $BINPATH/CcspMtaAgentSsp
   	 else
	$BINPATH/CcspMtaAgentSsp -subsys $Subsys
        fi
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

	 #CcspLMLite
	echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspLMLite on atom reset"
	cd /usr/ccsp/lm
	$BINPATH/CcspLMLite -subsys $Subsys

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
        if [ "$MODEL_NUM" = "DPC3939B" ] || [ "$MODEL_NUM" = "DPC3941B" ]; then 
         echo_t "Disabling CcpsHomeSecurity for BWG "
        else
        echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspHomeSecurity on atom reset"
	CcspHomeSecurity 8081&
        sleep 5
        fi

	#CcspTr069PaSsp
	if [ "$MODEL_NUM" = "DPC3939B" ] || [ "$MODEL_NUM" = "DPC3941B" ]; then
        echo_t "Disabling TR069Pa for BWG "
        else
	enable_TR69_Binary=`syscfg get EnableTR69Binary`
	if [ "" = "$enable_TR69_Binary" ] || [ "true" = "$enable_TR69_Binary" ]; then
            echo "[`getDateTime`] RDKB_SELFHEAL : Resetting process CcspTr069PaSsp on atom reset"
	    cd /usr/ccsp/tr069pa
	    if [ "x"$Subsys = "x" ]; then
                $BINPATH/CcspTr069PaSsp
		else
		$BINPATH/CcspTr069PaSsp -subsys $Subsys
            fi
	fi
	fi

	sleep 2

    echo "Enable RFC feature from ccsp restart"
    # Enable RFC feature
    if [ -f  /lib/rdk/rfc.service ]; then
            /bin/sh /lib/rdk/rfc.service &
    fi
