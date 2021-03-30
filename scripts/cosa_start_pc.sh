#!/bin/sh
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

ln -s /usr/lib/libupnp.so.3 /usr/lib/libupnp.so
ln -s /usr/lib/libixml.so.2 /usr/lib/libixml.so

if [ "x"$1 = "x" ];then
    sleep 3
fi

killall bbhm
killall ccspRecoveryManager
sleep 1
killall CcspTandDSsp
killall CcspDnsSsp
killall CcspFuSsp
killall CcspSsdSsp
killall CcspPandMSsp
killall CcspTr069PaSsp
killall PsmSsp
killall CcspRmSsp
killall CcspCrSsp
killall CcspLmSsp
killall CcspMtaAgentSsp
killall CcspCMAgentSsp
killall cherokee-worker
killall cherokee
killall CcspLMLite
ps aux|grep "dbus-daemon --config-file="|awk '{print$2}'|xargs kill

export PATH=$PWD/../../:$PATH
export LD_LIBRARY_PATH=$PWD:.:$PWD/lib:$PWD/../../lib:/usr/lib:$LD_LIBRARY_PATH
export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket

cp ccsp_msg.cfg /mnt/sysdata0
cp ccsp_msg.cfg /tmp

if [ "x"$1 = "x" ];then
    sleep 10
fi

# have IP address for dbus config generated
#./DbusCfg

if [ -e /mnt/sysdata0/basic.conf ]; then
    if [ -e ./dbus-daemon ]; then
        dbus-daemon --config-file=/mnt/sysdata0/basic.conf &
	else
	    /usr/bin/dbus-daemon --config-file=/mnt/sysdata0/basic.conf &
	fi
else
    if [ -e ./dbus-daemon ]; then
	    ./dbus-daemon --config-file=./basic.conf &
	else
		dbus-daemon --config-file=./basic.conf &
	fi
fi

if [ -f ./cp_subsys_ert ]; then
    Subsys="eRT."
elif [ -e ./cp_subsys_emg ]; then
    Subsys="eMG."
else
    Subsys=""
fi

echo "Elected subsystem is $Subsys"

sleep 1

if [ "x"$Subsys = "x" ];then
    ./CcspCrSsp &
else
    echo "./CcspCrSsp -subsys $Subsys &"
    ./CcspCrSsp -subsys $Subsys &
fi

if [ -e ./rm ]; then
    sleep 1
    cd rm
    ./CcspRmSsp -subsys $Subsys &
    cd ../
fi

sleep 1
if [ "x"$Subsys = "x" ];then
    ./PsmSsp &
else
    ./PsmSsp -subsys $Subsys &
fi

if [ -e ./pam ]; then
    cd pam
    sleep 1
    if [ "x"$Subsys = "x" ];then
        ./CcspPandMSsp &
    else
        echo "./CcspPandMSsp -subsys $Subsys &"
        ./CcspPandMSsp -subsys $Subsys &
    fi
    cd ..
fi

if [ -e ./cherokee ]; then
    cp ./cherokee/icons/yes.gif /usr/share/cherokee/icons
    cp ./cherokee/icons/add.gif /usr/share/cherokee/icons
    cp ./cherokee/icons/delete.gif /usr/share/cherokee/icons
    cherokee-worker -C ./cherokee/conf/cherokee.conf &
fi


if [ -f ./CcspSampleComp ]; then
    sleep 1
    if [ "x"$Subsys = "x" ];then
        ./CcspSampleComp &
    else
        echo "./CcspSampleComp -subsys $Subsys &"
        ./CcspSampleComp -subsys $Subsys &
    fi
fi

if [ "x"$1 = "x""pam" ]; then
  exit 0
fi


sleep 2
if [ -e ./tr069pa  ]; then
    cd tr069pa
    if [ "x"$Subsys = "x" ];then
        ./CcspTr069PaSsp &
    else
        ./CcspTr069PaSsp -subsys $Subsys &
    fi
    cd ..
fi

if [ -e ./ssd ]; then
    cd ssd
    if [ "x"$Subsys = "x" ];then
        ./CcspSsdSsp &
    else
        echo "./CcspSsdSsp -subsys $Subsys &"
        ./CcspSsdSsp -subsys $Subsys &
    fi
    cd ..
fi

if [ -e ./fu ]; then
    sleep 1
    cd fu
    if [ "x"$Subsys = "x" ];then
            echo "./CcspFuSsp &"
        ./CcspFuSsp &
    else
        echo "./CcspFuSsp -subsys $Subsys &"
        ./CcspFuSsp -subsys $Subsys &
    fi
    cd ..
fi

if [ -f ./CcspLmSsp ]; then
    sleep 1
    if [ "x"$Subsys = "x" ];then
        ./CcspLmSsp &
    else
        echo "./CcspLmSsp -subsys $Subsys &"
        ./CcspLmSsp -subsys $Subsys &
    fi
fi

if [ -e ./tad ]; then
    cd tad
    if [ "x"$Subsys = "x" ];then
        ./CcspTandDSsp &
    else
        ./CcspTandDSsp -subsys $Subsys &
    fi
    cd ..
fi

if [ -f ./ccspRecoveryManager ]; then
    sleep 1
    if [ "x"$Subsys = "x" ];then
        ./ccspRecoveryManager &
    else
        echo "./ccspRecoveryManager -subsys $Subsys &"
        ./ccspRecoveryManager -subsys $Subsys &
    fi
fi

if [ -e ./mta ]; then
    cd mta
    sleep 1
    if [ "x"$Subsys = "x" ];then
        ./CcspMtaAgentSsp &
    else
        echo "./CcspMtaAgentSsp -subsys $Subsys &"
        ./CcspMtaAgentSsp -subsys $Subsys &
    fi
    cd ..
fi

if [ -e ./cm ]; then
    cd cm
    sleep 1
    if [ "x"$Subsys = "x" ];then
        ./CcspCMAgentSsp &
    else
        echo "./CcspCMAgentSsp -subsys $Subsys &"
        ./CcspCM -subsys $Subsys &
    fi
    cd ..
fi

if [ -e ./lm ]; then
    cd lm
    sleep 1
    ./CcspLMLite &
fi
