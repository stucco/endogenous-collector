#!/bin/sh

# clone hone repo
git clone https://github.com/HoneProject/Linux-Sensor.git

# build and install hone kernel module
cd Linux-Sensor/src
#sudo install -m 0644 udev.rules /etc/udev/rules.d/60-hone.rules
sudo make
sudo make install
sudo /sbin/depmod -a
sudo modprobe honeevent
