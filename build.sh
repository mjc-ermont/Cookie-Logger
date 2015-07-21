#!/bin/bash

pwd

if [ $OS == "LINUX" ];
then
	if [ $ARCH == "i686" ];
	then
		sudo apt-get update && sudo apt-get install autopoint qemu qemu-system qemu-kvm build-essential
		wget http://cloud-images.ubuntu.com/releases/14.04/release/ubuntu-14.04-server-cloudimg-armhf-disk1.img
		qemu-system-i386 -no-kvm -hda ubuntu-14.04-server-cloudimg-armhf-disk1.img -m 1024 -nographic -no-reboot -redir tcp:5555::22 &
		sleep 15
		echo "Connection au ssh"
		ssh root@localhost -p 5555 "
			sudo apt-get install qt4-dev-tools libqjson-dev libqwt-dev python3-dev python3-numpy python-numpy-dev &&
			git clone git://code.qt.io/qt/qtserialport.git &&
			cd qtserialport &&
			git checkout qt4-dev
			qmake qtserialport.pro &&
			make &&
			sudo make install &&
			cd ..
			exit
		"
		echo "Déconnection du ssh"
	elif [ $ARCH == "x86_64" ];
	then
		sudo apt-get install qt4-dev-tools libqjson-dev libqwt-dev python3-dev python3-numpy python-numpy-dev &&
		git clone git://code.qt.io/qt/qtserialport.git &&
		cd qtserialport &&
		git checkout qt4-dev
		qmake qtserialport.pro &&
		make &&
		sudo make install &&
		cd .. &&
		qmake Logger21.pro -spec linux-g++-64
	fi
	make &&
	mv Logger21 Logger21-$OS-$ARCH
elif [ $OS == "WIN" ];
then
	sudo apt-get install autopoint intltool gperf cmake scons liblzma-dev python3-dev python3-numpy python-numpy-dev &&
	git clone https://github.com/jnovy/pxz && 
	cd pxz &&
	make &&
	sudo make install && 
	cd .. &&
	wget http://arthurtoussaint.free.fr/mxe_$ARCH.tar.xz &&
	echo "Extracting" &&
	pxz -d mxe_$ARCH.tar.xz &&
	echo "Detarring" &&
	tar -xf mxe_$ARCH.tar &&
	sudo cp -r mxe /opt &&
	sudo mkdir /opt/mxe/usr/$ARCH-w64-mingw32.shared/qt/include/qwt && 
	sudo mv /opt/mxe/usr/$ARCH-w64-mingw32.shared/qt/include/{qw*.h,qwt/}
	export PATH=/opt/mxe/usr/bin:$PATH &&
	echo "Building" &&
	/opt/mxe/usr/$ARCH-w64-mingw32.shared/qt/bin/qmake Logger21.pro &&
	make &&
	mkdir build &&
	cd build &&
	wget http://arthurtoussaint.free.fr/libgcc_s_sjlj-1.dll &&
	cd .. &&
	cp release/Logger21.exe /opt/mxe/usr/$ARCH-w64-mingw32.shared/bin/{libqjson,zlib1,libstdc++-6,libpng16-16,libsqlite3-0,libeay32,ssleay32}.dll /opt/mxe/usr/$ARCH-w64-mingw32.shared/qt/lib/{qwt,QtCore4,QtGui4,QtSql4,QtSerialPort,QtWebKit4,QtNetwork4,QtSvg4}.dll  build/ &&
	zip -r Logger21-$OS-$ARCH.zip build/
fi

