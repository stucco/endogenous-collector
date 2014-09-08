# login event collector
This collector forwards login events to a RabbitMQ broker. This collector parses `auth.log` for failed and successful login events (currently, only `ssh` logins). `l2r` is used to send the data to RabbitMQ.

## Dependencies
1. To download the l2r binary run:

        ../get-l2r.sh

2. Alternatively, you can download [l2r](https://github.com/ornl-sava/l2r) and build it on your own.

## Running the collector
To run this collector with default parameters run:

    ./collect_login_events.sh

### Scheduling the collector
This script a 'one-shot' collector, that is, it collects the data once and exits). You can use `cron` to schedule this script to run periodically. For example, to run this script once a day:

    crontab -e

add:

    @daily /full/path/to/collect_installed_pkg_list.sh

### Options
    AUTHLOG_PATH: Full path to auth.log (default '/var/log/auth.log')

    EXCHANGE: RabbitMQ exchange name (default 'stucco')
    ROUTING_KEY: RabbitMQ routing key (default 'stucco.in.login_events')
    RMQ_IP: RabbitMQ broker IP (default 127.0.0.1)
    RMQ_PORT: RabbitMQ broker port (default 5672)

To specify some of these options run:

    OPTION1="value1" OPTION2="value2" ./collect_installed_pkg_list.sh
