#!/usr/bin/env bash
# build web version

set -eu -o pipefail

BUILD_DIR=build-web

# TODO add docker support
emsdk_run() {
    podman run --rm \
               --security-opt=label=disable\
               -v $(pwd):/src:rw \
               -u $(id -u):$(id -g) \
               --userns keep-id \
               emscripten/emsdk \
               "$@"
}

if [[ ! -d "./$BUILD_DIR" ]] || [[ "${1:-}" == "--configure" ]]; then
    echo "Configuring for web using cmake"
    emsdk_run emcmake cmake -S . -B "/src/$BUILD_DIR"
fi

echo "Building for web"
emsdk_run cmake --build "/src/$BUILD_DIR"
