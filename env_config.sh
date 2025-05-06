#!/bin/bash
DB_NAME="ChatRoom"
DB_USER="chatroom"
DB_PASS="chatroom"
# ROOT_PASS="root_pass"

sudo apt update -y
sudo apt install git gcc g++ gdb make mysql-server libmysqlcppconn-dev -y

sudo mysql << EOF

CREATE DATABASE IF NOT EXISTS \`${DB_NAME}\` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

CREATE USER IF NOT EXISTS '${DB_USER}'@'localhost' IDENTIFIED BY '${DB_PASS}';
GRANT ALL PRIVILEGES ON \`${DB_NAME}\`.* TO '${DB_USER}'@'localhost';

USE \`${DB_NAME}\`;
CREATE TABLE users (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(50),  password VARCHAR(64), online TINYINT, create_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP );
CREATE TABLE `groups` (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(50), create_by INT, create_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP );
CREATE TABLE message (target INT, string VARCHAR(1024), from INT, is_group tinyint, time TIMESTAMP DEFAULT CURRENT_TIMESTAMP );

EOF

# -- (可选) 设置 root 用户用密码登录而不是 unix_socket
# ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '${ROOT_PASS}';
# FLUSH PRIVILEGES;
