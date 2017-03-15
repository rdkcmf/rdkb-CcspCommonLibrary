#!/bin/sh
# During Some Cases Like Factory Reset P&M is coming up before Utopia is Fully Initialized

loopBreakCounter=0

while [ ! -f /tmp/utopia_inited ]
do
  
  if [ "$loopBreakCounter" -ne 5 ]; then
  	echo "`date`: Utopia is not ready yet..."
  	loopBreakCounter=$((loopBreakCounter+1))
  	sleep 5
  else
  	break
  fi
done
