#!/bin/sh
#These were commands from cosa_start_rem.sh that were run later during initialization

source /etc/device.properties

export LOG4C_RCPATH=/etc

isCloudCapable=1
isCloudCapable=`syscfg get cloud_capable_flag`
if [ $isCloudCapable -eq 0 ]
then
	syscfg set cloud_capable_flag 1
	syscfg commit
fi

echo "Enabling ssh by default"
syscfg set mgmt_wan_sshaccess 1
syscfg commit
