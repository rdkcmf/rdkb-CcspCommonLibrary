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

source /etc/log_timestamp.sh

if [ -e /nvram/rbus ]
then
         echo_t "[RDKB_SELFHEAL] : Device is in RBUS mode" >> /rdklogs/logs/SelfHeal.txt.0
else
         echo_t "[RDKB_SELFHEAL] : Device is in DBUS mode" >> /rdklogs/logs/SelfHeal.txt.0
fi

if [ -e /nvram/rbus_off_pending ]
then
         echo_t "[RDKB_SELFHEAL] : RFC applied for DBUS mode and will take effect in next reboot" >> /rdklogs/logs/SelfHeal.txt.0
elif [ -e /nvram/rbus_on_pending ]
then
         echo_t "[RDKB_SELFHEAL] : RFC applied for RBUS mode and will take effect in next reboot" >> /rdklogs/logs/SelfHeal.txt.0
fi