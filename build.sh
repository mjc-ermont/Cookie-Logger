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
	sudo apt-get install autopoint intltool gperf cmake scons &&
	wget https://dl.dropboxusercontent.com/content_link/mz7Ei4ao7lqEJg8pUrGyinv7GQuZvWE5EYWLYMFu7p9aFmkOL6JKwH0ErToR8rUj?dl=1 -O mxe.tar.xz
	pxz mxe.tar.xz
	tar zf mxe.tar
	export PATH=$TRAVIS_BUILD_DIR/mxe/usr/bin:$PATH &&
	mxe/usr/i686-w64-mingw32.shared/qt/bin/qmake Logger21.pro &&
	make
fi
