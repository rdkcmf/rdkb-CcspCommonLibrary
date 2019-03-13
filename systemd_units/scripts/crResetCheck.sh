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
# Component Register can safely restart if no other RDKB components have initialized

source /etc/device.properties
if [ -f /tmp/pam_initialized ] || [ -f /tmp/psm_initialized ]; then
	#This will be launched even during the happy path. For those cases give time for system to shutdown
	sleep 10
	if [ -e "/usr/bin/onboarding_log" ]; then
	    /usr/bin/onboarding_log "Device reboot due to reason CR_crash"
	fi
	syscfg set X_RDKCENTRAL-COM_LastRebootReason CR_crash
	syscfg set X_RDKCENTRAL-COM_LastRebootCounter "1"
	syscfg commit
	sync
	source /rdklogger/logfiles.sh;syncLogs_nvram2
	reboot
else
	echo "`date`: Stopping/Restarting CcspCrSsp" >> ${PROCESS_RESTART_LOG}
	rm -f /tmp/CcspCrSsp_Restarted
fi
