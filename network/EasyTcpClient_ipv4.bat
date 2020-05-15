@echo off
:::::::::::::::::::::
::server ip
set cmd="strIP=127.0.0.1"
::server port
set cmd=%cmd% nPort=4567
::num of work threads
set cmd=%cmd% nThread=1
::num of clients
set cmd=%cmd% nClient=10
set cmd=%cmd% nMsg=1
set cmd=%cmd% nSendSleep=10
set cmd=%cmd% nSendBuffSize=10240
set cmd=%cmd% nRecvBuffSize=10240
set cmd=%cmd% -checkMsgID
set cmd=%cmd% -checkSendBack

EasyClient %cmd%

pause