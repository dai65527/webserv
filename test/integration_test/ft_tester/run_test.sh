#!/bin/bash
make re -C ../../../
../../../webserv ./test.conf&

sleep 1

echo "\n\n\n" | ./tester http://localhost:8888

if [ $? -ne 0 ]; then
    kill $!
    exit 1
else
    kill $!
fi
