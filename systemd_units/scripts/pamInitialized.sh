#!/bin/sh
#Soft Dependency SystemD Synchronization
#It was found during testing placing in ExecStartPre the loop wouldn't break
#SystemD also has built In Service Timeouts. Need to Save Off Loop Counter to RootFS
#to prevent infinite Loop
if [ -s /tmp/pam_boot_counter ]; then
   pamloopcounter=`cat /tmp/pam_boot_counter`
else
   echo "[PAM] /tmp/pam_boot_counter empty"
   pamloopcounter=0
fi

#Look For Initialization File, but give up after 5 Minutes
until [ $pamloopcounter -eq 60 ]; do	
	sleep 5
	pamloopcounter=$((pamloopcounter+1))
	echo "$pamloopcounter" > /tmp/pam_boot_counter
		
	if [ -f /tmp/pam_initialized ]; then
		break
	fi
done
