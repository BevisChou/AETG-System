CPP11 = g++-11 -std=c++11
OPT = -pthread

BASE_DIR = src

OUT = main

.PHONY: default, clean

default: ${BASE_DIR}/*
	${CPP11} ${OPT} ${BASE_DIR}/main.cpp ${BASE_DIR}/AETG.cpp ${BASE_DIR}/TestCase.cpp -o ${OUT}

test: ${BASE_DIR}/*
	${CPP11} ${OPT} ${BASE_DIR}/test.cpp ${BASE_DIR}/AETG.cpp ${BASE_DIR}/TestCase.cpp -o ${OUT}

clean:
	$(shell rm ${OUT})