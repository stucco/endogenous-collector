#!/bin/bash

function parse_sshd() {
	sshd_regex="(^[A-Za-z]{3}\s+[0-9]{1,2} [0-9]{2}:[0-9]{2}:[0-9]{2}) (\S+) (sshd)\[[0-9]+\]: (Failed|Accepted) password for (invalid user ){0,1}(\S+) from (\S+) port"
	while read line
	do 
		if [[ $line =~ $sshd_regex ]]; then
			event_date="${BASH_REMATCH[1]}"
			local_host="${BASH_REMATCH[2]}"
			prog="${BASH_REMATCH[3]}"
			login_status="${BASH_REMATCH[4]}"
			# skip BASH_REMATCH[5]
			user_name="${BASH_REMATCH[6]}"
			remote_host="${BASH_REMATCH[7]}"
			echo $event_date,$local_host,$prog,$login_status,$user_name,$remote_host
		fi
	done < ${AUTHLOG_PATH}
}

AUTHLOG_PATH=${AUTHLOG_PATH:-'/var/log/auth.log'}

# l2r arguments
EXCHANGE=${EXCHANGE:-'stucco'}
ROUTING_KEY=${ROUTING_KEY:-'stucco.in.login_events'}
RMQ_IP=${RMQ_IP:-'127.0.0.1'}
RMQ_PORT=${RMQ_PORT:-5672}

RMQ_HEADERS="-header-key HasContent -header-value true"
URI="amqp://guest:guest@${RMQ_IP}:${RMQ_PORT}/"
L2R_ARGS="-exchange=$EXCHANGE -msg-mode=2 -routing-key=$ROUTING_KEY -uri=$URI $RMQ_HEADERS"

# Pipe login events to RabbitMQ
parse_sshd | l2r $L2R_ARGS


