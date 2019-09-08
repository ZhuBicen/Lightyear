# Lighyear使用说明
## 服务端的安装与设置
### 数据库
1. 数据库服务器IP： 
2. 数据库名：passport
2. 用户信息表：sso_user

### 启动服务
```
$ ./server.exe -help
Usage of D:\Project\Lightyear\server\server.exe:
  -alsologtostderr=false: log to standard error as well as files
  -log_backtrace_at=:0: when logging hits line file:N, emit a stack trace
  -log_dir="": If non-empty, write log files in this directory
  -logtostderr=false: log to standard error instead of files
  -stderrthreshold=0: logs at or above this threshold go to stderr
  -v=0: log level for V logs
  -vmodule=: comma-separated list of pattern=N settings for file-filtered logging
```

### 收集日志文件
请参看`-log_dir`选项
