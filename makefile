.PHONY: build
build:
	cmake -B build
	cmake --build build

.PHONY: run
run:
	./build/CompilerPrinciples

.PHONY: clean
clean:
	rm -rf build cmake-build-debug
