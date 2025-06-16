#!/bin/sh

IMGTAG="misc-2x2-furious"
IMGNAME="misc-2x2-furious-container"
PORT=4002

# Build the docker image
echo "BUILD docker $IMGTAG"
docker build -t $IMGTAG .

# Kill potential previous docker
echo "KILL docker $IMGNAME"
docker kill $IMGNAME 2>/dev/null || true
docker rm $IMGNAME 2>/dev/null || true

# Run the docker image
#   - forward port of the docker 4002 to machine 4002
echo "RUN docker tag:$IMGTAG   name:$IMGNAME"
docker run --name $IMGNAME -p $PORT:$PORT -d $IMGTAG

echo "Container started on port $PORT"
echo "Connect with: nc localhost $PORT"
