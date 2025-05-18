#!/bin/bash

HOSTNAME=172.27.201.232

for file in $(ls -1 data); do
    echo "Uploading $file"
    curl -F "data=@data/$file" http://$HOSTNAME/upload
done