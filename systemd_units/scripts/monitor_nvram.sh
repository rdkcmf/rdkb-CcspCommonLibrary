#!/bin/sh

#
# If not stated otherwise in this file or this component's Licenses.txt file the
# following copyright and licenses apply:
#
# Copyright 2014 RDK Management
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
#

# Shell script to monitor nvaram disk space
# -------------------------------------------------------------------------

. /etc/device.properties

WORK_PATH=/nvram/log*
PARTITION=/nvram
# flag to indicate the threshold value to check
THRESHOLD_SIZE=$1

LOG_FILE=/tmp/disk_cleanup.log
usep=0
count=1

# Usage check
if [ $# -ne 1 ]; then
    echo -e "USAGE:\n    $0 <THRESHOLD_SIZE>"
    exit 0
fi


Timestamp()
{
    date +"%Y-%m-%d %T"
}

echo -e "\n`Timestamp` ---Received call to $0 ----" >> $LOG_FILE

if [ -f /var/tmp/DiskCheck.pid ]
then
   pid=`cat /var/tmp/DiskCheck.pid`
   if [ -d /proc/$pid ]
   then
      echo "`Timestamp` An instance of disk_threshold_check.sh with pid $pid is already running.." >> $LOG_FILE 
      echo "`Timestamp` Exiting script" >> $LOG_FILE
      exit 0
   fi
fi

echo $$ > /var/tmp/DiskCheck.pid

# If nvram monitor log is going beyond 1MG, overwite contents.
file_size=$( stat -c %s $LOG_FILE )
if [ $file_size -gt 1000000 ]
then
    echo -e "`Timestamp` === log size limit 1MB reached=== older contents are wiped \n">$LOG_FILE
fi

# Function to check Usage vs Threshold. If usage is less then exiting
disk_size_check()
{

   usep=`df -kh $PARTITION | grep -v "Filesystem" |awk '{print $5}'|sed 's/%//g'`
   if [ $usep -ge $THRESHOLD_SIZE ]; then
         echo "`Timestamp` Running out of space \"$PARTITION ($usep%)\"" >> $LOG_FILE
   else
       echo "`Timestamp` \"$PARTITION usage ($usep%)\" is less than threshold ($THRESHOLD_SIZE%). Exit monitoring..." >> $LOG_FILE
       exit 0
   fi
}

# find and delete large file in given WORKPATH
deleteMaxFile()
{
    find $WORK_PATH -type f | xargs ls -S > /tmp/deletionList.txt
    maxFile=`head -n 1 /tmp/deletionList.txt`
    echo "`Timestamp` Max File: $maxFile" >> $LOG_FILE
    if [ -f $maxFile ]; then
    	echo "`Timestamp` Deleting the file due to size issue `ls -l $maxFile`" >> $LOG_FILE
    	rm -rf $maxFile
    fi
}

# Scheduling a cronjob for continous monitoring
scheduleCron()
{

    # Check whether any cron jobs are existing or not
    existing_cron_check=`crontab -l -c $CRON_SPOOL|grep -c monitor_nvram_xb6`
    # If no cron job exists, create one
    if [ "$existing_cron_check" -eq 0 ]; then
	(crontab -l 2>/dev/null; echo "*/10 * * * *  /lib/rdk/monitor_nvram_xb6.sh $THRESHOLD_SIZE")| crontab -
        echo "`Timestamp` cron scheduled successfully">> $LOG_FILE
    else
        echo "`Timestamp` cron already exist">> $LOG_FILE
    fi
}

# Cron Scheduling
#scheduleCron

# Execution Steps for DISK cleanup

if [ -d $WORK_PATH/lost+found ]; then
    echo "`Timestamp` Clearing $WORK_PATH/lost+found folder" >> $LOG_FILE
    rm -rf $WORK_PATH/lost+found
fi

#counter variable
check=0

#count is equal to total number of files in log* dirs
count=`find $WORK_PATH -type f|wc -l`

#find and delete large log files one by one. This will check usage vs threshold in each iteration
while [ $check -le $count ]
do
    disk_size_check
    deleteMaxFile
    check=`expr $check + 1`
done

#if control reaches here, nvram usage is still high after deleting all logs under nvram and need to check the partition
echo -e "`Timestamp` CRITICAL ERROR, please check $WORK_PATH folder..!" >> $LOG_FILE
exit 0
