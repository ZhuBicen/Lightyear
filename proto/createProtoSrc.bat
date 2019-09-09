@echo off
rem should install goprotobuff first
rem go get -u code.google.com/p/goprotobuf/{proto,protoc-gen-go}
rem if certifcate issue met, please add the fingerprint to %USERPROFILE%/.hgrc
@echo on
echo Please ensure protoc-gen-go.exe in PATH env
..\tools\protoc-3.10.0-rc-1-win64\bin\protoc.exe  --go_out=gopath\src  --cpp_out=..\client\Messenger\proto\src core.proto
..\tools\protoc-3.10.0-rc-1-win64\bin\protoc.exe  --go_out=gopath\src  --cpp_out=..\client\Messenger\proto\src im.proto
..\tools\protoc-3.10.0-rc-1-win64\bin\protoc.exe  --go_out=gopath\src  --cpp_out=..\client\Messenger\proto\src localdb.proto
..\tools\protoc-3.10.0-rc-1-win64\bin\protoc.exe  --go_out=gopath\src  --cpp_out=..\client\Messenger\proto\src p2p.proto
pause > nul

