#!/bin/bash

cd ChronoReader &&
git pull origin master &&
git submodule init &&
git submodule update &&
cd .. &&

if [ $OS == "LINUX" ];
then
	sudo apt-get install qt4-dev-tools libqjson-dev libqwt-dev &&
	git clone git://gitorious.org/qt/qtserialport.git &&
	cd qtserialport &&
	qmake qtserialport.pro &&
	make &&
	sudo make install &&
	cd ..
	qmake Logger21.pro &&
	make
elif [ $OS == "WIN" ];
then
	sudo apt-get install autopoint intltool gperf cmake scons liblzma-dev &&
	git clone https://github.com/jnovy/pxz && 
	cd pxz &&
	make &&
	sudo make install && 
	cd .. &&
	wget https://dl.dropboxusercontent.com/s/bhhvvbmfzy207rp/mxe.tar.xz &&
	echo "Extracting" &&
	pxz -d mxe.tar.xz &&
	echo "Detarring" &&
	tar -xf mxe.tar &&
	export PATH=/home/travis/mjc-ermont/Cookie-Logger/mxe/usr/bin:$PATH &&
	echo "Building" &&
	/home/travis/mjc-ermont/Cookie-Logger/mxe/usr/i686-pc-mingw32.shared/qt/bin/qmake Logger21.pro &&
	make
fi
