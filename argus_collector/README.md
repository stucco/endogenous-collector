# argus collector
This collector reads network flow data from an argus server and forwards the data to a RabbitMQ broker. The argus client `ra` is used to read from the argus server, and `l2r` is used to send the data to a RabbitMQ broker.

## Install
1. To download and build the argus clients run:

        ./get-argus-clients.sh

2. To download the l2r binary run:

        ../get-l2r.sh

3. Alternatively, you can download [argus](http://qosient.com/argus), and [l2r](https://github.com/ornl-sava/l2r) and build them on your own.

## argus setup
This argus collector script utilizes the argus `ra` client. This script does not install or start an argus server. An argus server needs to be installed and started before `ra` can forward data. (See [argus' website](http://qosient.com/argus) for argus-server documentation.)

Once argus server is installed, as a quick test you can run:

    sudo argus -P 561 -d &
    ra -S localhost:561

If everything is working, then `ra` will output network-flow data.

## Running the collector
To run the argus collector with default parameters run:

    ./collect_argus.sh

### Scheduling the collector
Once this script is started, it will continue to stream argus data to RabbitMQ. You can use `cron` to ensure this script is run every time the system starts:

    crontab -e

add:

    @reboot /full/path/to/collect_argus.sh

### Options
    ARGUS_IP: argus server IP address (default 127.0.0.1)
    ARGUS_PORT: argus server port (default 561)
    
    EXCHANGE: RabbitMQ exchange name (default 'stucco')
    ROUTING_KEY: RabbitMQ routing key (default 'stucco.in.argus')
    RMQ_IP: RabbitMQ broker IP (default 127.0.0.1)
    RMQ_PORT: RabbitMQ broker port (default 5672)

To specify some of these options run:

    OPTION1="value1" OPTION2="value2" ./collect_argus.sh
