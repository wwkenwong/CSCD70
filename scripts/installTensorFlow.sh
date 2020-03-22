#!/bin/bash -e

cd $(cd $(dirname $0)/.. && pwd)

PY_VERSION=3.6

virtualenv --no-site-packages -p python${PY_VERSION} tensorflow-dev

# activate the virtual environment
source tensorflow-dev/bin/activate

pip3 install tf-nightly
