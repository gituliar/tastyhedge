.PHONY: build check config release

KWINTO = ./out/linux-release/src/kwinto

serve:
	python3 py/kwinto.py


release: config build check


build:
	cmake --build --preset linux-release --target kwinto
build-all:
	cmake --build --preset linux-release
config:
	cmake --preset linux-release
check:
	./out/linux-release/test/kwinto_test

gui:
	${KWINTO} gui --src=/mnt/c/dh/min
options:
	${KWINTO} make --options --src /mnt/c/dh
stocks:
	${KWINTO} make --stocks --src /mnt/c/dh
tape-copy:
	${KWINTO} tape copy --src /mnt/c/dh --dst=/mnt/c/dh -t AAPL,AMD,MSFT,SPY,TSLA



install-kwinto:
	rm -f bin/kwinto
	upx -o bin/kwinto out/linux-release/src/kwinto
install-benchmark:
	cp out/linux-release/test/kwinto_benchmark bin/
install-py:
	cp out/linux-release/py/kwinto.cpython-38-x86_64-linux-gnu.so .
