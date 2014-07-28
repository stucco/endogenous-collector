# endogenous collectors
This repo contains scripts and agents that collect host and network data, i.e., "endogenous" data.

## l2r
Multiple endogenous collectors use `l2r` to send data to a RabbitMQ broker.

1. To download the l2r binary run:

        ./get-l2r.sh

2. Alternatively, you can download [l2r](https://github.com/ornl-sava/l2r) and build it on your own.

3. Add l2r to your PATH (You could simply copy l2r to /usr/local/bin/)
