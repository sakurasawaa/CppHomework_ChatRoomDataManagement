#!/bin/bash
# sudo apt update
# sudo apt install unzip gcc g++ make libmysqlclient-dev -y
# wget --no-check-certificate https://qwrt.sakurasawa.com:5243/d/share/chatroom_data_manage-stu.zip?sign=vrL7xZEL2djIV_49e65cXgKLcJpt0h4aAGeEB0T8hgQ=:0
# mkdir chatroom_data_manage-stu
# unzip chatroom_data_manage-stu.zip -d ./chatroom_data_manage-stu
 CREATE TABLE users (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(50),  password VARCHAR(64), online TINYINT, create_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP );
 CREATE TABLE `groups` (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(50), create_by INT, create_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP );
 CREATE TABLE message (target INT, string VARCHAR(1024), is_group tinyint, time TIMESTAMP DEFAULT CURRENT_TIMESTAMP );