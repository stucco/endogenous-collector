# hone collector
This collector reads Hone data from the local host and forwards the data to a RabbitMQ broker. To learn more about Hone see [Hone's GitHub page](https://github.com/HoneProject/Linux-Sensor/wiki).

## Dependencies
1. qmake (sudo apt-get install qt4-qmake)
2. Qt4 dev libraries (sudo apt-get install libqt4-dev)
3. International Components for Unicode (sudo apt-get install libicu48) 

## Install
1. Build and install OpenPcapNgLib:

        mkdir OpenPcapNgLib/build
        cd OpenPcapNgLib/build
        qmake ../PcapNgLib.pro
        make
        sudo make install
        cd ../..

2. Build and install Hone DataIngestor

	mkdir DataIngestor/build
        cd DataIngestor/build
        qmake ../DataIngestor.pro
	make
        sudo make install
        cd ../..

3. Install the Hone Linux sensor:

        ./install-linux-sensor.sh

(Alternatively, you can download the [Hone Linux sensor](https://github.com/HoneProject/Linux-Sensor) and build it on your own.)

4. To download the l2r binary run:

        ../get-l2r.sh

(Alternatively, you can download [l2r](https://github.com/ornl-sava/l2r) and build it on your own.)

## Running the collector
To run the Hone collector with default parameters run:

    sudo ./collect_hone.sh

(The script is run as sudo because, by default, /dev/hone is only readable by root.)

### Scheduling the collector
Once this script is started, it will continue to stream Hone data to RabbitMQ. You can use `cron` to ensure this script is run every time the system starts:

    crontab -e

add:

    @reboot /full/path/to/collect_hone.sh

### Options
    HONE_DEV: path to Hone device (default /dev/hone)
    
    EXCHANGE: RabbitMQ exchange name (default 'stucco')
    ROUTING_KEY: RabbitMQ routing key (default 'stucco.in.hone')
    RMQ_IP: RabbitMQ broker IP (default 127.0.0.1)
    RMQ_PORT: RabbitMQ broker port (default 5672)

To specify some of these options run:

    OPTION1="value1" OPTION2="value2" ./collect_hone.sh
