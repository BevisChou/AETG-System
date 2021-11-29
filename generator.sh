#!/bin/sh

DATA_DIR=data
INPUT_FILE=$1
OUTPUT_FILE=$2

pushd src
make all
popd

cd build

(java mapper.InputGenerator ../${DATA_DIR}/${INPUT_FILE}; echo $3;) | \
    ./aetg | \
    java mapper.OutputGenerator ../${DATA_DIR}/${INPUT_FILE} > \
    ../${DATA_DIR}/${OUTPUT_FILE}