#!/bin/bash

# argus arguments
RA_CMD=${RA_CMD:-'./ra'}
ARGUS_IP=${ARGUS_IP:-'127.0.0.1'}
ARGUS_PORT=${ARGS_PORT:-561}

FIELDS="seq stime ltime saddr sport dir daddr dport proto pkts bytes"
ARG_FIELDS=""
for field in $FIELDS; do ARG_FIELDS="$ARG_FIELDS -s $field"; done
RA_ARGS="-S ${ARGUS_IP}:${ARGUS_PORT} -c , $ARG_FIELDS -nnn - tcp or udp"

# l2r arguments
L2R_CMD=${L2R_CMD:-'./l2r'}
EXCHANGE=${EXCHANGE:-'stucco'}
ROUTING_KEY=${ROUTING_KEY:-'stucco.in.argus'}
RMQ_IP=${RMQ_IP:-'127.0.0.1'}
RMQ_PORT=${RMQ_PORT:-5672}

URI="amqp://guest:guest@${RMQ_IP}:${RMQ_PORT}/"
L2R_ARGS="-exchange=$EXCHANGE -msg-mode=2 -routing-key=$ROUTING_KEY -uri=$URI"

# Pipe argus client to RabbitMQ
echo "$RA_CMD $RA_ARGS | $L2R_CMD $L2R_ARGS"
"$RA_CMD" $RA_ARGS | "$L2R_CMD" $L2R_ARGS
