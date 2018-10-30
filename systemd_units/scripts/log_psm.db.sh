#!/bin/sh

if [ ! -f "/tmp/psm_db_logged" ]; then
	cp /nvram/bbhm_cur_cfg.xml /rdklogs/logs/bbhm_cur_cfg_before_psminit.xml
	touch /tmp/psm_db_logged
fi
