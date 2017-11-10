#!/bin/sh
##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2017 RDK Management
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

if [ "$1" = "init" ]; then
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

	if [ "$ENABLEWEBPA" = "true" ];then
		echo "ENABLEWEBPA is true..Intializing WebPA.."
	else
		echo "EnablePa parameter is set to false. Hence not initializng WebPA.."
		exit 1
	fi
elif [ "$1" = "restart" ]; then
	#Actions to complete For Self Healing
	echo "RDKB_PROCESS_CRASHED : WebPA_process is not running, need restart"
	#We'll set the reason only if webpa reconnect is not due to DNS resolve
	syscfg get X_RDKCENTRAL-COM_LastReconnectReason | grep "Dns_Res_webpa_reconnect"
	if [ $? != 0 ]; then
		echo "setting reconnect reason from sysd_task_health_monitor.sh"
		echo "Setting Last reconnect reason"
		syscfg set X_RDKCENTRAL-COM_LastReconnectReason WebPa_crash
		result=`echo $?`
		if [ "$result" != "0" ]
		then
			echo "SET for Reconnect Reason failed"
		fi
		syscfg commit
		result=`echo $?`
		if [ "$result" != "0" ]
		then
			echo "Commit for Reconnect Reason failed"
		fi
		echo "SET succeeded"
	fi
else
	echo "ccspWebpaCheck.sh didn't perform any actions"
fi