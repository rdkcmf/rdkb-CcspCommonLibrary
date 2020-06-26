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

#operating rbus based on new naming nomenclature
if [ -e /nvram/rbus_support_on_pending ]; then
    touch /nvram/rbus_support
    rm /nvram/rbus_support_on_pending
elif [ -e /nvram/rbus_support_off_pending ]; then
    if [ -e /nvram/rbus_support ]; then
        rm /nvram/rbus_support
    fi
    rm /nvram/rbus_support_off_pending
fi

#removing older nomenclature files
if [ -e /nvram/rbus_on_pending ]; then
    rm /nvram/rbus_on_pending
fi
if [ -e /nvram/rbus_off_pending ]; then
    rm /nvram/rbus_off_pending
fi
if [ -e /nvram/rbus ]; then
    rm /nvram/rbus
fi
