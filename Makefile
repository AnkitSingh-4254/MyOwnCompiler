BUILD_TYPE ?= Release

.PHONY: all configure build run clean

all: build

configure:
	cmake -S cpp-compiler -B cpp-compiler/build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build: configure
	cmake --build cpp-compiler/build -- -j

run: build
	./cpp-compiler/build/mycompiler

clean:
	rm -rf cpp-compiler/build
