#!/bin/sh
##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2022 RDK Management
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
while true 
do 
	var1=`sysevent get wan-status | grep started`
	var2=`sysevent get current_wan_ifname | grep erouter0`
	var3=`sysevent get mesh_wan_linkstatus | grep up`
	
	if [ "$var1" != "" ]; then # normal mode with erouter0 interface
		break		
	else 
                echo "Parodus - wan-status is not started, waiting.."
		if [ "$var2" == "" ]; then  # LTE backup mode with LTE interface
                        echo "Parodus - current_wan_ifname is not erouter0"
			if [ "$var3" != "" ]; then # LTE backup wan up
                        	break
                        else
                                echo "Parodus - mesh_wan_linkstatus is not up, waiting.."
                 	fi	
                 else 
                        echo "Parodus - current_wan_ifname is erouter0"
		fi	
	fi
	sleep 5 
done 
