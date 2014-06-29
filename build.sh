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
<<<<<<< HEAD
	wget https://dl.dropboxusercontent.com/content_link/mz7Ei4ao7lqEJg8pUrGyinv7GQuZvWE5EYWLYMFu7p9aFmkOL6JKwH0ErToR8rUj?dl=1 -O mxe.tar.xz
	pxz mxe.tar.xz
	tar zf mxe.tar
=======
	git clone https://github.com/mxe/mxe.git &&
	cd mxe &&
	curl http://arthurtoussaint.free.fr/qtserialport.patch | patch -p1
	curl http://arthurtoussaint.free.fr/qwt.patch | patch -p1
	make qt qwt_qt4 qjson qtserialport &&
	cd .. &&
>>>>>>> d34fffb0fe95e74bb787de1527f9f70619960824
	export PATH=$TRAVIS_BUILD_DIR/mxe/usr/bin:$PATH &&
	mxe/usr/i686-w64-mingw32.shared/qt/bin/qmake Logger21.pro &&
	make
fi
