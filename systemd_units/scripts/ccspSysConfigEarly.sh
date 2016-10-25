#!/bin/sh
#These were commands from cosa_start.sh that were run early during initialization

source /etc/device.properties

export LOG4C_RCPATH=/etc

# have IP address for Radius config
vconfig add eth0 4090
ifconfig eth0.4090 192.168.251.254 netmask 255.255.255.0 up
ip route add default via 192.168.251.1

ulimit -c unlimited
if [ "$BUILD_TYPE" != "prod" ]; then
      echo /tmp/%t_core.prog_%e.signal_%s > /proc/sys/kernel/core_pattern
fi

#
#	Allow custom plug-ins
#
if [ -f "$PWD/cosa_start_custom_1.sh" ]; then
	./cosa_start_custom_1.sh
fi

#update the bbhm for 2.1s11
/usr/ccsp/psm/bbhm_patch.sh -f /nvram/bbhm_cur_cfg.xml

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
		REDIRCTEXISTS=`cat /nvram/syscfg.db | grep redirection_flag | cut -f2 -d=`
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

# Start coredump
if [ -f "$PWD/core_compr" ]; then
	if ! [ -e "/var/core" ]; then
		mkdir -p /var/core/
	fi
	echo "|$PWD/core_compr /var/core %p %e" >/proc/sys/kernel/core_pattern
	ulimit -c unlimited

	./core_report.sh &
fi

cp ccsp_msg.cfg /tmp

touch /tmp/cp_subsys_ert

if [ "$BOX_TYPE" == "XB6" ]; then
	#RDK-7248: Temp Workaround
	#Service Files are generic for all devices
	sleep 60
fi
