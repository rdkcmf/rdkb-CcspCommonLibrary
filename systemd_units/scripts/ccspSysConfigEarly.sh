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
#These were commands from cosa_start.sh that were run early during initialization

source /etc/device.properties

SECURE_SYSCFG=`syscfg get UpdateNvram`
SYSCFG_DB_FILE="/nvram/syscfg.db"
if [ "$SECURE_SYSCFG" = "false" ]; then
      SYSCFG_DB_FILE="/opt/secure/data/syscfg.db"
fi

export LOG4C_RCPATH=/etc

cp /usr/ccsp/ccsp_msg.cfg /tmp

if [ "$BOX_TYPE" = "HUB4" ]; then
    #Disable core dump generation
    ulimit -c 0
else
    ulimit -c unlimited
fi

if [ "$BUILD_TYPE" != "prod" ]; then
    if [ "$BOX_TYPE" = "HUB4" ]; then
        #Disable core dump generation
        echo /dev/null > /proc/sys/kernel/core_pattern
    else
        echo /tmp/%t_core.prog_%e.signal_%s > /proc/sys/kernel/core_pattern
    fi
fi

#
#	Allow custom plug-ins
#
if [ -f "$PWD/cosa_start_custom_1.sh" ]; then
	./cosa_start_custom_1.sh
fi

#update the bbhm for 2.1s11
if [ "$MFG_NAME" = "Arris" ]; then
	if [ "$MODEL_NUM" = "TG4482A" ] ; then
		/usr/ccsp/psm/bbhm_patch.sh -f /nvram/bbhm_cur_cfg.xml
	fi
    # Check if bbhm has Notify flag present
    NOTIFYPRESENT=`cat /nvram/bbhm_cur_cfg.xml | grep NotifyWiFiChanges`
    REDIRCTEXISTS=""

    # If Notify flag is not present then we will add it as per the syscfg DB value
    if [ "$NOTIFYPRESENT" = "" ]
    then
            REDIRECT_VALUE=`syscfg get redirection_flag`
            if [ "$REDIRECT_VALUE" = "" ]
            then
                    #Just making sure if syscfg command didn't fail
                    REDIRCTEXISTS=`cat $SYSCFG_DB_FILE  | grep redirection_flag | cut -f2 -d=`
            fi

            if [ "$REDIRECT_VALUE" = "false" ] || [ "$REDIRCTEXISTS" = "false" ];
            then

                    echo " Apply Notifywifichanges flse"
                    sed '10 a \ \ \ <Record name=\"eRT.com.cisco.spvtg.ccsp.Device.WiFi.NotifyWiFiChanges\" type=\"astr\">false</Record>' /nvram/bbhm_cur_cfg.xml > /var/tmp/bbhm_cur_cfg.xml
                    sed '10 a \ \ \ <Record name=\"eRT.com.cisco.spvtg.ccsp.Device.WiFi.NotifyWiFiChanges\" type=\"astr\">false</Record>' /nvram/bbhm_bak_cfg.xml > /var/tmp/bbhm_bak_cfg.xml
                    cp /var/tmp/bbhm_cur_cfg.xml /nvram/bbhm_cur_cfg.xml
                    cp /var/tmp/bbhm_bak_cfg.xml /nvram/bbhm_bak_cfg.xml
                    rm /var/tmp/bbhm_cur_cfg.xml
                    rm /var/tmp/bbhm_bak_cfg.xml
            elif [ "$REDIRECT_VALUE" = "true" ] || [ "$REDIRCTEXISTS" = "true" ];
            then
                    echo " Apply Notifywifichanges tue"
                    sed '10 a \ \ \ <Record name=\"eRT.com.cisco.spvtg.ccsp.Device.WiFi.NotifyWiFiChanges\" type=\"astr\">true</Record>' /nvram/bbhm_cur_cfg.xml > /var/tmp/bbhm_cur_cfg.xml
                    sed '10 a \ \ \ <Record name=\"eRT.com.cisco.spvtg.ccsp.Device.WiFi.NotifyWiFiChanges\" type=\"astr\">true</Record>' /nvram/bbhm_bak_cfg.xml > /var/tmp/bbhm_bak_cfg.xml
                    cp /var/tmp/bbhm_cur_cfg.xml /nvram/bbhm_cur_cfg.xml
                    cp /var/tmp/bbhm_bak_cfg.xml /nvram/bbhm_bak_cfg.xml
                    rm /var/tmp/bbhm_cur_cfg.xml
                    rm /var/tmp/bbhm_bak_cfg.xml
            fi
    fi
else
    echo "bbhm patch is not required"
fi

# Check if Hotspot Max Num of STA is updated.
if [ "$IS_BCI" = "yes" ]; then
	grep "<Record name=\"dmsb.hotspot.max_num_sta_set\" type=\"astr\">1<\/Record>" /nvram/bbhm_cur_cfg.xml
	if [ "$?" == "1" ];then
		cp /nvram/bbhm_cur_cfg.xml /tmp/b1
		cat /tmp/b1 | sed s/"<Record name=\"dmsb.hotspot.max_num_sta_set\" type=\"astr\">0<\/Record>"/"<Record name=\"dmsb.hotspot.max_num_sta_set\" type=\"astr\">1<\/Record>"/ >/tmp/b2
		cp /tmp/b2 /nvram/bbhm_cur_cfg.xml
		rm /tmp/b1
		rm /tmp/b2

		grep "<Record name=\"eRT.com.cisco.spvtg.ccsp.tr181pa.Device.WiFi.AccessPoint.5.BssMaxNumSta\" type=\"astr\">15<\/Record>" /nvram/bbhm_cur_cfg.xml
		if [  "$?" == "1" ] ; then
			cp /nvram/bbhm_cur_cfg.xml /tmp/b1
			cat /tmp/b1 | sed s/"<Record name=\"eRT.com.cisco.spvtg.ccsp.tr181pa.Device.WiFi.AccessPoint.5.BssMaxNumSta\" type=\"astr\">5<\/Record>"/"<Record name=\"eRT.com.cisco.spvtg.ccsp.tr181pa.Device.WiFi.AccessPoint.5.BssMaxNumSta\" type=\"astr\">15<\/Record>"/ >/tmp/b2
			cat /tmp/b2 | sed s/"<Record name=\"eRT.com.cisco.spvtg.ccsp.tr181pa.Device.WiFi.AccessPoint.6.BssMaxNumSta\" type=\"astr\">5<\/Record>"/"<Record name=\"eRT.com.cisco.spvtg.ccsp.tr181pa.Device.WiFi.AccessPoint.6.BssMaxNumSta\" type=\"astr\">15<\/Record>"/ >/tmp/b1
			cat /tmp/b1 | sed s/"<Record name=\"eRT.com.cisco.spvtg.ccsp.tr181pa.Device.WiFi.AccessPoint.9.BssMaxNumSta\" type=\"astr\">30<\/Record>"/"<Record name=\"eRT.com.cisco.spvtg.ccsp.tr181pa.Device.WiFi.AccessPoint.9.BssMaxNumSta\" type=\"astr\">15<\/Record>"/ >/tmp/b2
			cat /tmp/b2 | sed s/"<Record name=\"eRT.com.cisco.spvtg.ccsp.tr181pa.Device.WiFi.AccessPoint.10.BssMaxNumSta\" type=\"astr\">30<\/Record>"/"<Record name=\"eRT.com.cisco.spvtg.ccsp.tr181pa.Device.WiFi.AccessPoint.10.BssMaxNumSta\" type=\"astr\">15<\/Record>"/ >/tmp/b1
			cp /tmp/b1 /nvram/bbhm_cur_cfg.xml
			rm /tmp/b1
			rm /tmp/b2
		fi
	fi
fi

# Start coredump
if [ -f "$PWD/core_compr" ]; then
	if ! [ -e "/var/core" ]; then
		mkdir -p /var/core/
	fi
	echo "|$PWD/core_compr /var/core %p %e" >/proc/sys/kernel/core_pattern
	ulimit -c unlimited

	./core_report.sh &
fi

touch /tmp/cp_subsys_ert


