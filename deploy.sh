#!/bin/bash

# Exit on error
set -e

mkdir -p test_deploy && \
cp build/test_cpp test_deploy/ && \
cp -r include/usearch test_deploy/include/ && \
cp -r fp16/include/fp16 test_deploy/include/ && \
cp -r simsimd/include/simsimd test_deploy/include/ && \
cp -r stringzilla/include/stringzilla test_deploy/include/ && \
echo "Files copied successfully!"
