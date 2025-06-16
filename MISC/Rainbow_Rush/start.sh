#!/bin/sh

IMGTAG="misc-rainbow-rush"
IMGNAME="misc-rainbow-rush-container"
PORT=4001

# Build the docker image
echo "BUILD docker $IMGTAG"
docker build -t $IMGTAG .

# Kill potential previous docker
echo "KILL docker $IMGNAME"
docker kill $IMGNAME 2>/dev/null || true
docker rm $IMGNAME 2>/dev/null || true

# Run the docker image
# - forward port of the docker 4007 to machine 4007
echo "RUN docker tag:$IMGTAG name:$IMGNAME"
docker run --name $IMGNAME -p $PORT:$PORT -d $IMGTAG

echo "Container started on port $PORT"
echo "Connect with: nc localhost $PORT"
