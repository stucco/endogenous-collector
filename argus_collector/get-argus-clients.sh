#!/bin/bash

VERSION="3.0.6"
BASE_NAME="argus-clients-${VERSION}"
FILE="${BASE_NAME}.tar.gz"

if [ -d $BASE_NAME ]; then
	echo "$BASE_NAME already exists. Skipping download."
else
	echo "Downloading Argus $VERSION"
	curl -L http://qosient.com/argus/src/${FILE} -o "$FILE"
	tar xzf "$FILE" && rm "$FILE"
fi

echo "Building $BASE_NAME"
cd "$BASE_NAME"
./configure && make && ln -s ${BASE_NAME}/bin/ra ../ra

echo "Done"
