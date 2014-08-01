#if [ -e /mnt/sysdata0/switch ]; then
#   cd /mnt/appdata0/cc
#   ./cosa_start.sh
#   exit 0
#fi

#if [ "x"$1 = "x" ];then
#    sleep 40
#fi

killall CcspWifiSsp

export LD_LIBRARY_PATH=$PWD:.:$PWD/lib:/lib:/usr/lib:$LD_LIBRARY_PATH

#mkdir -p /nvram/ccsp
#cp ccsp_msg.cfg /nvram/ccsp
cp ccsp_msg.cfg /tmp

if [ "x"$1 = "x" ];then
    sleep 10
fi

# have IP address for dbus config generated
#./DbusCfg

Subsys="eRT."

echo "Elected subsystem is $Subsys"

sleep 1

if [ -e ./wifi ]; then
	cd wifi 
	if [ "x"$Subsys = "x" ];then
    	./CcspWifiSsp &
	else
    	echo "./CcspWifiSsp -subsys $Subsys &"
    	./CcspWifiSsp -subsys $Subsys &
	fi
fi

