Cookie-Logger
=============

Le DataLogger officiel du projet Cookie

# À quoi ça sert ?
Récupérer les données télétransmises par le ballon en direct live pour les afficher sur un tableau de bord.
Il permet aussi de les afficher dans des graphiques ainsi que la position du ballon sur une carte grâce aux données GPS.

# Compiler le projet

#### Configurations disponibles
 - [ ] Linux 32bits
 - [x] Linux 64bits
 - [ ] Windows
 - [ ] ~~Max OS~~

## Linux 64 bits

### Installation des dépendances

Commencer par installer les dépendances des dépots officiels.
```shell
# Ubuntu
sudo apt-get install qt4-dev-tools libqjson-dev g++ python3.4 python3.4-dev python-numpy
```

#### Compilation de [qtserialport](https://github.com/qtproject/qtserialport)

```shell
git clone https://github.com/qtproject/qtserialport.git
cd qtserialport
git checkout qt4-dev # Compilation pour qt4
qmake qtserialport.pro -spec linux-g++-32
make && sudo make install
cd ../..
```

#### Compilation de [QWT](http://qwt.sourceforge.net/)
##### Installation via les dépots officiels
Cookie-Logger nécessite une version de qwt supérieure à 6.0.0 et inférieure à 6.1.0. Sur ubuntu 14.04, la version installée semble compatible : `sudo apt-get install libqwt-dev`
##### Compilation des sources
```shell
# Téléchargement de la version 6.0.2
wget http://downloads.sourceforge.net/project/qwt/qwt/6.0.2/qwt-6.0.2.tar.bz2
tar -xv qwt-6.0.2.tar.bz2
cd qwt-6.0.2
qmake-qt4
make && sudo make install
cd ..
# Linkage de la bibliothèque
ln -s /usr/local/qwt-6.0.2/include /usr/include/qwt
ln -s /usr/local/qwt-6.0.2/lib/* /usr/lib/
```
### Compilation de cookie
Normalement toutes les dépendances sont présentes, il ne reste plus qu'à compiler normalement.
```shell
git clone https://github.com/mjc-ermont/Cookie-Logger.git
cd Cookie-Logger
qmake-qt4
make
```
