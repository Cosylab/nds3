#/bin/bash
# Exit on error
set -e
# Print each line
set -v

mkdir build
(cd build
	cmake -DCMAKE_BUILD_TYPE=Debug ../CMake
	make)

mkdir tests/build
(cd tests/build
	cmake -DCMAKE_BUILD_TYPE=Debug ../CMake -DLIBRARY_LOCATION=../../build
	make
	./nds3tests)

mkdir doc/examples/build
(cd doc/examples/build
	cmake -DCMAKE_BUILD_TYPE=Debug ../CMake -DLIBRARY_LOCATION=../../../build
	make)

lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info '/usr/*' 'googletest-src/*' 'tests/*' --output-file coverage.info
lcov --list coverage.info
coveralls-lcov --repo-token "${COVERALLS_TOKEN}" coverage.info
