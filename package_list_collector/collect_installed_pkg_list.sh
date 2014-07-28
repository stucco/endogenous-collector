#!/bin/bash

HOSTNAME=`hostname`

# l2r arguments
EXCHANGE=${EXCHANGE:-'stucco'}
ROUTING_KEY=${ROUTING_KEY:-'stucco.in.installed_package'}
RMQ_IP=${RMQ_IP:-'127.0.0.1'}
RMQ_PORT=${RMQ_PORT:-5672}

RMQ_HEADERS="-header-key HasContent -header-value true"
URI="amqp://guest:guest@${RMQ_IP}:${RMQ_PORT}/"
L2R_ARGS="-exchange=$EXCHANGE -msg-mode=2 -routing-key=$ROUTING_KEY -uri=$URI $RMQ_HEADERS"

# Pipe package list to RabbitMQ
dpkg-query -W -f '${status} ${package} ${version}\n' | grep -i "^install ok installed" | awk "{print \"$HOSTNAME,\"\$4\",\"\$5}" | l2r $L2R_ARGS
