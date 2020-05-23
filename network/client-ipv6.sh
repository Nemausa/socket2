cd `dirname $0`
nOpenFile=`ulimit -n`
if [ $nOpenFile -lt 204800 ];then
	echo "reset max size"
	ulimit -n 204800
fi
echo "max size"
ulimit -n
cmd='strIP=::1'
cmd=$cmd' nPort=4567'
cmd=$cmd" nThread=1"
cmd=$cmd" nClient=10"
cmd=$cmd" nMsg=1"
cmd=$cmd" nSendSleep=10"
cmd=$cmd" nSendBuffSize=10240"
cmd=$cmd" nRecvBuffSize=10240"
cmd=$cmd" -checkMsgID"
cmd=$cmd" ipv6"


./EasyTcpClient $cmd
#
read -p "..press any key to exit.." var
