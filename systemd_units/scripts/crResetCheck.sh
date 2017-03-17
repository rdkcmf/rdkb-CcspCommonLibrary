#!/bin/sh
# Component Register can safely restart if no other RDKB components have initialized

source /etc/device.properties
  
if [ -f /tmp/pam_initialized ] || [ -f /tmp/psm_initialized ]; then
	#This will be launched even during the happy path. For those cases give time for system to shutdown
	sleep 10
	echo "`date`: CcspCrSsp Restarted Rebooting" >> ${PROCESS_RESTART_LOG}
	sync
	source /rdklogger/logfiles.sh;syncLogs_nvram2
	reboot
else
	echo "`date`: Stopping/Restarting CcspCrSsp" >> ${PROCESS_RESTART_LOG}
	rm -f /tmp/CcspCrSsp_Restarted
fi
