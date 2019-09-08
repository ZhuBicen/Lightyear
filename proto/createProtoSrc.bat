@echo off
rem should install goprotobuff first
rem go get -u code.google.com/p/goprotobuf/{proto,protoc-gen-go}
rem if certifcate issue met, please add the fingerprint to %USERPROFILE%/.hgrc
@echo on
echo Please ensure protoc-gen-go.exe in PATH env
..\tools\protoc-2.6.1-win32\protoc.exe --go_out=gopath\src\lightyear\proto\core     --cpp_out=..\client\Messenger\proto\src core.proto
..\tools\protoc-2.6.1-win32\protoc.exe --go_out=gopath\src\lightyear\proto\im       --cpp_out=..\client\Messenger\proto\src im.proto
..\tools\protoc-2.6.1-win32\protoc.exe --go_out=gopath\src\lightyear\proto\localdb  --cpp_out=..\client\Messenger\proto\src localdb.proto
..\tools\protoc-2.6.1-win32\protoc.exe --go_out=gopath\src\lightyear\proto\p2p      --cpp_out=..\client\Messenger\proto\src p2p.proto
..\tools\protoc-2.6.1-win32\protoc.exe                                              --cpp_out=..\client\Messenger\proto\src localdb.proto
pause > nul