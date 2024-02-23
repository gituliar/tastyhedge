.PHONY: build build-all calibrate check config release

release: config build-all check

config:
	cmake --preset linux-release
build:
	cmake --build --preset linux-release --target tastyhedge
build-all:
	cmake --build --preset linux-release
check:
	./out/linux-release/test/tastyhedge_test

calibrate:
	./out/linux-release/src/tastyhedge calibrate --mds=./mds mds/tape_TSLA_20230501.exchange.csv mds/tape_TSLA_20230501.broker.csv
