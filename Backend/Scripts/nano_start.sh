#!/bin/bash

cd "$(dirname "$0")/../build"

echo "Flushing Redis..."
redis-cli FLUSHDB

echo "Starting Backend Server..."
./my_app