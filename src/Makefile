AETG_DIR = aetg
MAPPER_DIR = mapper

OUT_DIR = ../build

CPP11 = g++-11 -std=c++11
OPT = -pthread

.PHONY: all, aetg, mapper, clean

all: aetg mapper

aetg: ${AETG_DIR}/* init
	${CPP11} ${OPT} ${AETG_DIR}/main.cpp ${AETG_DIR}/AETG.cpp ${AETG_DIR}/TestCase.cpp -o ${OUT_DIR}/aetg

mapper: ${MAPPER_DIR}/* init
	javac ${MAPPER_DIR}/InputGenerator.java ${MAPPER_DIR}/OutputGenerator.java -d ${OUT_DIR}

init:
	$(shell mkdir ${OUT_DIR})

clean:
	$(shell rm -rf ${OUT_DIR})