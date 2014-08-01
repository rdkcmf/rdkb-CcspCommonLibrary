1.If config_file is null, default will use "ccsp_msg.cfg".
  the first line in ccsp_msg.cfg wil be the default daemon address, all message will send to this daemon if it's alive. In general it should be your local daemon.

2. CCSP_Message_Bus_Register_Path2 or CCSP_Message_Bus_Register_Path can register multiple path, but one connection can only have one path_function
  Every connection to dbus daemon will has its own main loop. If there are multiple connections shared path_function, you MUST handle the concurrency issues.

3.CCSP_Message_Bus_Init() can be called multiple times in 1 process with different component name

4.component_id match with bus well-known name, must contain ".",
  event_name match method, must not contain "."
  interface name must contain "."
  path must contain "/"

5.In the message header, only contain sender's bus unique name. If you need know sender's well-known name, my understanding, you have to pack it in the message by yourself.

6.In the ccsp_message_mus_test.c, you can find the template how to use these api. In general you only need modify message pack/unpack to use them. To compile:
  gcc -o tmsg ccsp_message_bus_test.c ccsp_message_bus.c  -g -I. -I../learn/pcdbus-1.4.14 -L../learn/pcdbus-1.4.14/dbus/.libs -ldbus-1 -ldbus-internal -Wall
        Start daemon like this: ./dbus-daemon --config-file=basic.conf --nofork
        in 1 shell run,  ./tmsg a.b b.a ,   another shell run ./tmsg b.a a.b 100 1000
        you may change the ip address in basic.conf to match yours.
  ccsp_base_test.c contain template to use base api.
  gcc -o amsg ccsp_base_api.c ccsp_base_api_srv.c ccsp_message_bus.c ccsp_base_test.c  -g -I. -I../learn/pcdbus-1.4.14 -L../learn/pcdbus-1.4.14/dbus/.libs -ldbus-1 -ldbus-internal -Wall  -I../work/ControlPlane/source/cosa/debug_api/include -I../work/ControlPlane/source/cosa/util_api/include -I../work/ControlPlane/source/cosa/debug_api/include/linux

7. this is tested with dbus 1.4.14
        ./configure --disable-tests --enable-verbose-mode  --disable-selinux  --disable-checks --with-x=no 
        --disable-checks is a must, since recursive pthread lock is used.

8.if you don't need slap variable, remove #define PSM_SLAP_VAR in ccsp_abase_api.h
9.during dubs request process or event callback, you can't make dbus call to yourself, but you can make dbus call to other components.

cross-compile for android:
1. download android ndk to /opt/android-ndk-r5b
2. compile dbus daemon and library:
export CC='/opt/android-ndk-r5b/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-gcc --sysroot=/opt/android-ndk-r5b/platforms/android-9/arch-arm'
export LD='/home/qtu/tool/android-toolchain/bin/arm-linux-androideabi-ld CCDEPMODE = depmode=gcc3'
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
cp /opt/android-ndk-r5b/platforms/android-9/arch-arm/usr/include/fcntl.h /opt/android-ndk-r5b/platforms/android-9/arch-arm/usr/include/sys

3.
cross compile expat and copy to sysroot "/opt/android-ndk-r5b/platforms/android-9/arch-arm"
cp ./lib/expat*.h  /opt/android-ndk-r5b/platforms/android-9/arch-arm/usr/include/
cp -a ./libs/*  /opt/android-ndk-r5b/platforms/android-9/arch-arm/usr/lib/

./configure -host=i686-pc-linux-gnu -target=arm-linux-gnu --enable-verbose-mode  --disable-selinux --disable-checks --with-x=no
remove #define HAVE_POSIX_GETPWNAM_R 1 in config.h
#remove all dbus_daemon_LDFLAGS, CFLAGS in 3 makefiles
clear dbus_daemon_LDFLAGS in bus/Makefile , remove all CFLAGS in 3 makefiles
make

4.
/home/qtu/learn/android-ndk-r5b/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-gcc --sysroot=/home/qtu/learn/android-ndk-r5b/platforms/android-9/arch-arm -o tmsg CCSP_Message_Bus_Test.c CCSP_Message_Bus.c  -g -I. -I/home/qtu/learn/tool/dbus_android_bak -L/home/qtu/learn/tool/dbus_android_bak/dbus/.libs -ldbus-1 -ldbus-internal -Wall
5.
adb push tmsg /data/tmp
adb push ccsp_msg.cfg  /data/tmp
adb push libdbus-1.so.3 /data/tmp
adb shell
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
cd /data/tmp
chmod 755 tmsg
./tmsg a.b b.a

cross-compile for cns:
export CC='/opt/cnstools/arm-2009q1/bin/arm-linux-gcc'
export LD='/opt/cnstools/arm-2009q1/bin/arm-linux-ld'
./configure -host=i686-pc-linux-gnu -target=arm-linux-gnu --enable-verbose-mode  --disable-selinux --disable-checks --with-x=no 
/opt/cnstools/arm-2009q1/bin/arm-linux-gcc -o tmsg ccsp_message_bus_test.c ccsp_message_bus.c  -g -I. -I../cnsdbus-1.4.14 -L../cnsdbus-1.4.14/dbus/.libs -ldbus-1 -ldbus-internal -Wall -lpthread -lrt

cross-compile for tpg:
cross compile expat and copy
sudo cp -a .libs/* /opt/cisco/CCSP/mips-linux-toolchain-1.2.0/mips-linux/rootfs/usr/lib
sudo cp lib/expat*.h /opt/cisco/CCSP/mips-linux-toolchain-1.2.0/mips-linux/rootfs/usr/include
export CC='/opt/cisco/CCSP/mips-linux-toolchain-1.2.0/bin/mips-linux-gcc'
export LD='/opt/cisco/CCSP/mips-linux-toolchain-1.2.0/bin/mips-linux-gcc'
export CFLAGS=-fPIC
./configure -host=i686-pc-linux-gnu -target=mips-linux-gnu --enable-verbose-mode  --disable-selinux --disable-checks --with-x=no 
/opt/cisco/CCSP/mips-linux-toolchain-1.2.0/bin/mips-linux-gcc -o tmsg ccsp_message_bus_test.c ccsp_message_bus.c  -g -I. -I../tpgdbus-1.4.14 -L../tpgdbus-1.4.14/dbus/.libs -ldbus-1 -ldbus-internal -Wall -lpthread -lrt



major function comments:
use this to init bus layer:
    CCSP_Message_Bus_Init(argv[1], "ccsp_msg.cfg", &bus_handle, Ansc_AllocateMemory_Callback, Ansc_FreeMemory_Callback);
    
If you export api, use this to register server side process function:
    CCSP_Message_Bus_Register_Path(bus_handle,msg_path, path_message_func,0);
    
Register interested event.    
    CCSP_Message_Bus_Register_Event(bus_handle, 0, msg_path, msg_interface,"abc");
    
register event call back.    
    CCSP_Message_Bus_Set_Event_Callback(bus_handle, evt_callback,0);


In the CCSP_Message_Bus_Test.c, has example to send event:
	CCSP_Message_Bus_Send_Event
to send message: 
	CCSP_Message_Bus_Send
Since you may send event/msg with different arguments, you may change to pack/unpack part of those example function.	
