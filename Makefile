.PHONY: build check config release

calibrate:
	./out/win1x-release/src/tastyhedge.exe calibrate --mds=./mds mds/tape_TSLA_20230501.exchange.csv mds/tape_TSLA_20230501.broker.csv

release: config build check

build:
	cmake --build --preset linux-release --target tastyhedge
build-all:
	cmake --build --preset linux-release
config:
	cmake --preset linux-release
check:
	./out/linux-release/test/tastyhedge_test
