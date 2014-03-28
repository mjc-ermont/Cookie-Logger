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
	sudo apt-get install autopoint libtool
	git clone https://github.com/mxe/mxe.git &&
	cd mxe &&
	make qt &&
	make qwt &&
	make qjson &&
	make qtserialport &&	
	cd .. &&
	export PATH=$TRAVIS_BUILD_DIR/mxe/usr/bin:$PATH &&
	mxe/usr/i686-pc-mingw32/qt/bin/qmake Logger21.pro &&
	make
fi
