编译步骤
================================

Getting Lightyear code
---------------------------------
1. `git clone https://code.csdn.net/ZhuBC/lightyear.git`
2. 


编译boost
--------------------------------
1. 下载最新的boost到D盘，解压到目录D:\boost\_1\_55\_0
2. 打开Visual Studio Command Prompt
3. 进入解压后的目录
2. 运行bootstrap.bat
3. b2  --with-system --with-date_time --with-regex --with-thread --with-program_options --with-locale --build-type=complete


编译protobuf
-------------------------------
Change RuntimeLibray option


编译cef
-------------------------------



编译Messenger
--------------------------------

* NOTE: To create release mode binary, Messenger/res/skin.zip should be recreated. 
* Make sure `git` is in the `$PATH`, to get the git revision when building binary.

数据库设置
----------------------------------
1. 安装MySQL
2. TBD

服务器端编译
------------------------------------
1. 安装最新的golang编译器
2. 设置相应的GOPATH，比如 d:\gopath;D:\Code\Lightyear\proto\gopath
2. 安装mercurial，在.hgrc中配置hostfingerprints如果出现连接错误，
3. [hostfingerprints]
code.google.com     = 83:ed:f0:fc:00:76:6f:2c:84:62:1e:3f:e2:cf:e4:60:92:c6:4d:5c

2. 在git bash中执行如下命令，以安装google protobuffer： go get code.google.com/p/goprotobuf/{proto,protoc-gen-go}
3. cd Lightyear\server
4. go build
5. run server.exe

NOTE: 聊天信息保存在lightyear.chatmessage表中


更新状态图
----------------------------------
2. Online version is [here](http://www.planttext.com/planttext)
1. Install [Graphviz](http://www.graphviz.org/pub/graphviz/stable/windows/graphviz-2.28.0.msi)

2. Install [PlantUML](http://heanet.dl.sourceforge.net/project/plantuml/plantuml.jar)

3. Install [PlantUMLQeditor](http://downloads.sourceforge.net/project/plantumlqeditor/plantuml-qeditor-1.1.0-win32.exe?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Fplantumlqeditor%2F&ts=1397451778&use_mirror=softlayer-ams)

4. 使用QEditor编辑 Messenger/sc/StateChart.txt

[使用命令行？](http://plantuml.sourceforge.net/graphvizdot.html)

other
-----------------------------------
1. [Chrome release link](http://src.chromium.org/viewvc/chrome/releases/)
2. [WebRTC branch](http://webrtc.googlecode.com/svn/branches/)
