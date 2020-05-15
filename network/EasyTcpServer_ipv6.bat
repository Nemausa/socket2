@echo off

set cmd="strIP=any"

set cmd=%cmd% nPort=4567

set cmd=%cmd% nThread=1

set cmd=%cmd% nMaxClient=10000

set cmd=%cmd% nSendBuffSize=20480

set cmd=%cmd% nRecvBuffSize=20480

set cmd=%cmd% -sendback

set cmd=%cmd% -sendfull

set cmd=%cmd% -checkMsgID

set cmd=%cmd% -p

set cmd=%cmd% -ipv6

EasyServer %cmd%

pause