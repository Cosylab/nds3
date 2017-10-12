#!/bin/bash
# Exit on error
set -e
# Print each line
set -v

# install latest LCOV [1]
wget http://ftp.de.debian.org/debian/pool/main/l/lcov/lcov_1.11.orig.tar.gz
tar xf lcov_1.11.orig.tar.gz
make -C lcov-1.11/ install PREFIX="$HOME/.local"
# install lcov to coveralls conversion + upload tool
gem install coveralls-lcov
