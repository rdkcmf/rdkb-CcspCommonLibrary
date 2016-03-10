#!/bin/sh

export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket
/usr/bin/dbus-daemon --config-file=/usr/ccsp/basic.conf --fork
