make clean all
PORT_FILE=$(/sbin/ifconfig eth0 | sed -En 's/127.0.0.1//;s/.*inet (addr:)?(([0-9]*\.){3}[0-9]*).*/\2/p')
port=$(cat /local/Fall_2017/vikrant/cse489589_assignment3/vikrant/controllerport/$PORT_FILE.port)
#echo $port
/usr/sbin/lsof -i tcp:$port | awk 'NR!=1 {print $2}' | xargs kill -9;
/local/Fall_2017/vikrant/cse489589_assignment3/vikrant/./assignment3 $port
