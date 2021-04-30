#!/bin/bash -e
make re -C ./unit_test
cd integration_test/my_tester && ./run_test.sh
