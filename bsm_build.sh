#!/bin/bash

ROOT=${PWD}
BUILD_DIR=${ROOT}/build
TST_DIR=${ROOT}/tst
TST_BUILD=${BUILD_DIR}/tst

NAME="black_scholes"

function usage() {
  echo "
    -d | --install-deps   Install conan dependencies before building
    -c | --clean          Clean build artefacts
    -r | --rebuild        Clean build artefacts before fully rebuilding
    -h | --help           Display this help message
  "
}

function clean() {
  rm -rf ${BUILD_DIR}
}

function install_deps() {
  if [[ ! -d ${BUILD_DIR} ]]; then
    mkdir -p ${BUILD_DIR}
  fi

  pushd ${BUILD_DIR} > /dev/null
  conan install ${ROOT}/conanfile.txt
  popd > /dev/null
}

function set_compile_link() {
  if [[ ! -L ${ROOT}/compile_commands.json ]]; then
    # Symlink compile_commands.json artefact to project ROOT
    # This sets LSP definitions for clangd.
    ln -s ${BUILD_DIR}/compile_commands.json ${ROOT}
  fi
}

function build() {
  if [[ ! -f ${BUILD_DIR}/conanbuildinfo.txt ]]; then
    echo "-- No conan packages found. Installing now... --"
    install_deps
  fi

  cmake -S ${ROOT} -B ${BUILD_DIR}
  cmake --build ${BUILD_DIR}

  set_compile_link
}

function parse_args() {
  if [[ $# -gt 0 ]]; then
    case $1 in
      -d|--install-deps)
        echo "-- Installing conan dependencies... --"
        install_deps
        exit 0
        ;;
      -c|--clean)
        echo "-- Cleaning ${NAME} build artefacts... --"
        clean
        exit 0
        ;;
      -r|--rebuild)
        echo "-- Fully rebuilding ${NAME}... --"
        clean
        install_deps
        ;;
      -h|--help|*)
        usage
        exit 1
        ;;
    esac
  fi
}

parse_args $@
build
