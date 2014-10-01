#!/bin/bash

# hone arguments
HONE_DEV=${HONE_DEV:-'/dev/hone'}

# l2r arguments
EXCHANGE=${EXCHANGE:-'stucco'}
ROUTING_KEY=${ROUTING_KEY:-'stucco.in.structured.hone'}
RMQ_IP=${RMQ_IP:-'127.0.0.1'}
RMQ_PORT=${RMQ_PORT:-5672}

RMQ_HEADERS="-header-key HasContent -header-value true"
URI="amqp://guest:guest@${RMQ_IP}:${RMQ_PORT}/"
L2R_ARGS="-exchange=$EXCHANGE -msg-mode=2 -routing-key=$ROUTING_KEY -uri=$URI -skip-first $RMQ_HEADERS"

# Pipe argus client to RabbitMQ
DataIngestor -i $HONE_DEV -o - | l2r $L2R_ARGS
