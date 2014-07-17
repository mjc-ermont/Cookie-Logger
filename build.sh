#!/bin/bash

pwd
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
	qmake Logger21.pro CONFIG+=release &&
	make
	mv Logger21 Logger21-$OS-$ARCH
elif [ $OS == "WIN" ];
then
	sudo apt-get install autopoint intltool gperf cmake scons liblzma-dev &&
	git clone https://github.com/jnovy/pxz && 
	cd pxz &&
	make &&
	sudo make install && 
	cd .. &&

	if [ $ARCH == "i686" ];
	then
		wget https://dl.dropboxusercontent.com/s/do0ufqeziw8d2qo/mxe_i686.tar.xz
	elif [ $ARCH == "x86_64" ];
	then
		wget https://dl.dropboxusercontent.com/s/r2frnt3b68heh80/mxe_x86_64.tar.xz
	fi
	echo "Extracting" &&
	pxz -d mxe_$ARCH.tar.xz &&
	echo "Detarring" &&
	tar -xf mxe_$ARCH.tar &&
	sudo cp -r mxe /opt
	sudo mkdir /opt/mxe/usr/$ARCH-w64-mingw32.shared/qt/include/qwt && 
	sudo mv /opt/mxe/usr/$ARCH-w64-mingw32.shared/qt/include/{qw*.h,qwt/}
	export PATH=/opt/mxe/usr/bin:$PATH &&
	echo "Building" &&
	/opt/mxe/usr/$ARCH-w64-mingw32.shared/qt/bin/qmake Logger21.pro CONFIG+=release &&
	make
	mv release/Logger21.exe Logger21-$OS-$ARCH.exe
fi

