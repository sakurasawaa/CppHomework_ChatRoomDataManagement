#!/bin/bash
sudo apt update -y
sudo apt install git gcc g++ gdb make mysql-server libmysqlcppconn-dev -y
mkdir bin build
echo -n "Initialize the database? [y/n] : "
read choice
choice=$(echo "$choice" | tr '[:upper:]' '[:lower:]')
if [ "$choice" == 'y' ]; then
    echo "Initialize the database..."
    sudo bash mysql_init.sh
    echo "Finish"
else
    echo "Finish"
fi