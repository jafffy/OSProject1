#!/bin/sh
rm timestamp*
rm statistics*

./build-process.sh
./build-factory.sh

./benchmark.py

./evaluateStatistics.py
./plotStatistics.py