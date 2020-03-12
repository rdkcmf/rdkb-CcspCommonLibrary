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

sleep 60
#set rbus crash only in rbus mode
if [ -e /nvram/rbus ]; then
    check_rbus=`systemctl status rbus.service | cut -d$'\n' -f7 | cut -d "," -f2 | cut -d ")" -f1 | cut -d '=' -f2`
    if [ "$check_rbus" != "TERM" ]; then
       if [ "$BOX_TYPE" = "XB3" ];then
          rpcclient $ARM_ARPING_IP "syscfg set X_RDKCENTRAL-COM_LastRebootReason Rbus_crash && syscfg set X_RDKCENTRAL-COM_LastRebootCounter 1 && syscfg commit && sync"
          sh /lib/rdk/uploadDumps.sh '' 0
          rpcclient $ARM_ARPING_IP "sh /rdklogger/backupLogs.sh false Rbus_crash"         
       else
          syscfg set X_RDKCENTRAL-COM_LastRebootReason Rbus_crash && syscfg set X_RDKCENTRAL-COM_LastRebootCounter 1 && syscfg commit && sync
          sh /lib/rdk/uploadDumps.sh '' 0
          sh /rdklogger/backupLogs.sh "false" "Rbus_crash"
          
       fi
    fi
    reboot
fi
