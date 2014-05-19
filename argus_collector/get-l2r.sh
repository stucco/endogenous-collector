#!/bin/bash

BITSIZE="amd64" # other options: "386"
BRANCH="master"
PLATFORM="linux" # other options: "windows" and "darwin"

echo "Downloading l2r"
curl -L http://gobuild.io/github.com/ornl-sava/l2r/${BRANCH}/${PLATFORM}/${BITSIZE} -o l2r.zip
unzip l2r.zip && rm l2r.zip

