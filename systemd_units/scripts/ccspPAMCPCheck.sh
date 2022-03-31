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
# During Some Cases Like Factory Reset P&M is coming up before Utopia is Fully Initialized

# This function will check if captive portal needs to be enabled or not.
source /etc/device.properties

checkCaptivePortal()
{

# Get all flags from DBs
isWiFiConfigured=`syscfg get redirection_flag`
psmNotificationCP=`psmcli get eRT.com.cisco.spvtg.ccsp.Device.WiFi.NotifyWiFiChanges`

#Read the http response value
networkResponse=`cat /var/tmp/networkresponse.txt`

iter=0
max_iter=2
PandM_wait_timeout=600
PandM_wait_count=0
while [ "$psmNotificationCP" = "" ] && [ "$iter" -le $max_iter ]
do
	iter=$((iter+1))
	echo "$iter"
	psmNotificationCP=`psmcli get eRT.com.cisco.spvtg.ccsp.Device.WiFi.NotifyWiFiChanges`
done

echo "RDKB_SELFHEAL : NotifyWiFiChanges is $psmNotificationCP"
echo "RDKB_SELFHEAL : redirection_flag val is $isWiFiConfigured"

if [ "$isWiFiConfigured" = "true" ]
then
	if [ "$networkResponse" = "204" ] && [ "$psmNotificationCP" = "true" ]
	then
		# Check if P&M is up and able to find the captive portal parameter
		while : ; do
			echo "RDKB_SELFHEAL : Waiting for PandM to initalize completely to set ConfigureWiFi flag"
			CHECK_PAM_INITIALIZED=`find /tmp/ -name "pam_initialized"`
			echo "RDKB_SELFHEAL : CHECK_PAM_INITIALIZED is $CHECK_PAM_INITIALIZED"
			if [ "$CHECK_PAM_INITIALIZED" != "" ]
			then
				echo "RDKB_SELFHEAL : WiFi is not configured, setting ConfigureWiFi to true"
				output=`dmcli eRT setvalues Device.DeviceInfo.X_RDKCENTRAL-COM_ConfigureWiFi bool TRUE`
				check_success=`echo $output | grep  "Execution succeed."`
				if [ "$check_success" != "" ]
				then
					echo "RDKB_SELFHEAL : Setting ConfigureWiFi to true is success"
				else
					echo "$output"
				fi
				break
			fi
			PandM_wait_count=$(($PandM_wait_count+2))
			if [ "$PandM_wait_count" -gt "$PandM_wait_timeout" ]	
			then
				echo "RDKB_SELFHEAL : PandM_wait_count reached timeout value, exiting from checkCaptivePortal function"
				break
			fi				
			sleep 2
		done
	else
		echo "RDKB_SELFHEAL : We have not received a 204 response or PSM valus is not in sync"
	fi
else
	echo "RDKB_SELFHEAL : Syscfg DB value is : $isWiFiConfigured"
fi	

}

if [ "$IS_BCI" != "yes" ]; then
	checkCaptivePortal
fi
# moved the code from ccspSysConfigLate.sh
echo "Enabling ssh by default"
syscfg set mgmt_wan_sshaccess 1
syscfg commit