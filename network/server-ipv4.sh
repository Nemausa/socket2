cd `dirname $0`
ulimit -n
ulimit -n 81920
ulimit -n
cmd='strIP=any'
cmd=$cmd' nPort=4567'
cmd=$cmd" nThread=1"
cmd=$cmd" nMaxClient=81920"
cmd=$cmd" nSendBuffSize=10240"
cmd=$cmd" nRecvBuffSize=20140"
cmd=$cmd" -sendback"
cmd=$cmd" -sendfull"
cmd=$cmd" checkMsgID"
cmd=$cmd" -p"
cmd=$cmd" wwwroot=D:/dev/www"
cmd=$cmd" indexpag=index.html"
./EasyTcpServer $cmd
#
read -p "..press any key to exit.." var
