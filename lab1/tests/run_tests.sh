#!/bin/bash

BIN_DIR="$(dirname "$0")"

echo "Running unit tests..."
"$BIN_DIR/salesman_tests"
if [ $? -ne 0 ]; then 
    echo "Unit tests failed!"
    exit 1
fi

echo "Running app on matrix4..."
"$BIN_DIR/salesman" "$BIN_DIR/data/matrix4.txt" "$BIN_DIR/out_matrix4.txt"
if [ $? -ne 0 ]; then 
    echo "Failed on matrix4!"
    exit 1
fi

echo "Running app on matrix10..."
"$BIN_DIR/salesman" "$BIN_DIR/data/matrix10.txt" "$BIN_DIR/out_matrix10.txt"
if [ $? -ne 0 ]; then 
    echo "Failed on matrix10!"
    exit 1
fi

echo "Running app on matrix..."
"$BIN_DIR/salesman" "$BIN_DIR/data/matrix.txt" "$BIN_DIR/out_matrix.txt"
if [ $? -ne 0 ]; then 
    echo "Failed on matrix!"
    exit 1
fi

echo "OK"