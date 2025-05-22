#!/bin/bash

HOSTNAMES=(172.27.201.232 192.168.4.1)


for host in ${HOSTNAMES[@]}; do
    echo "Trying $host ..."
    if ping $host -q -t 1 -o &>/dev/null; then 
        for file in $(ls -1 data); do
            echo "Uploading $file"
            curl -F "data=@data/$file" http://${host}/upload
        done
    else
        echo "Nope."
    fi
done
