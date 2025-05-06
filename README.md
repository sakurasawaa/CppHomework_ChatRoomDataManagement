# CppHomework_ChatRoomDataManagement
## 环境配置(Linux/WSL2: Ubuntu 24.04 LTS)
```sh
bash env_config.sh
```
## 数据库初始化
```sh
bash mysql_init.sh
```
## 二次编辑
VScode安装WSL/ssh扩展，点击右下角蓝条connect to wsl 或connect to host，连接到Linux后打开项目文件夹根据提示安装C++扩展和makefile扩展，F5即可自动构建并运行。
```sh
#项目构建
make
#清理构建
make clean
```
