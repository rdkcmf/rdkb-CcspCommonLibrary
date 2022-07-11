#!/bin/sh
####################################################################################
# If not stated otherwise in this file or this component's Licenses.txt file the
# following copyright and licenses apply:

#  Copyright 2019 RDK Management

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

source /etc/device.properties

#set rbus crash only in rbus mode
check_rbus=`systemctl status rbus.service | cut -d$'\n' -f7 | cut -d "," -f2 | cut -d ")" -f1 | cut -d '=' -f2`
echo "`date`: RBus IPC Process status is $check_rbus"
if [ "$check_rbus" != "TERM" ]; then

    echo "`date`: RBus crash handler is in Sleep/Idle for few sec to give time for the crashuploader"
    sleep 50

    if [ "$BOX_TYPE" = "XB3" ];then
       echo "`date`: RBus crash handler updating reboot reason for XB3"
       rpcclient $ARM_ARPING_IP "syscfg set X_RDKCENTRAL-COM_LastRebootReason Rbus_crash && syscfg set X_RDKCENTRAL-COM_LastRebootCounter 1 && syscfg commit && sync"
       rpcclient $ARM_ARPING_IP "sh /rdklogger/backupLogs.sh false Rbus_crash"
       echo "REBOOTING THE SYSTEM....."
       rpcclient $ARM_ARPING_IP "reboot"
    else
       echo "`date`: RBus crash handler updating reboot reason for NON-XB3"

       syscfg set X_RDKCENTRAL-COM_LastRebootReason Rbus_crash
       syscfg set X_RDKCENTRAL-COM_LastRebootCounter "1"
       syscfg commit
       sync

       echo "`date`: RBus IPC Process Crashed Rebooting" >> ${PROCESS_RESTART_LOG}
       source /rdklogger/logfiles.sh;syncLogs_nvram2
       echo "REBOOTING THE SYSTEM....."
       reboot
    fi

else
    echo "`date`: RBus IPC Process stopped"
fi
