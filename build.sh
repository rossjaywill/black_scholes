#!/bin/bash

ROOT=${PWD}
BUILD_DIR=${ROOT}/build
TST_DIR=${ROOT}/tst
TST_BUILD=${BUILD_DIR}/tst

NAME="black_scholes"

function usage() {
  echo "
    -d | --debug          Debug build
    -t | --toolchain      Specify which compiler toolchain to use (gcc/clang)
    -i | --install-deps   Install conan dependencies before building
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

  local build_type="Release"
  if [[ "${BUILD_TYPE,,}" == "debug" ]]; then
    build_type="Debug"
  fi

  # CXX=${CXX} cmake -S ${ROOT} -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE="${build_type}"
  # CXX=${CXX} cmake --build ${BUILD_DIR}
  cmake -S ${ROOT} -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE="${build_type}"
  cmake --build ${BUILD_DIR}

  set_compile_link
}

function parse_args() {
  while test $#  -gt 0; do
  case "$1" in
      -d|--debug)
        echo "-- Creating debug build... --"
        export BUILD_TYPE="debug"
        shift
        ;;
      -i|--install-deps)
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
        shift
        ;;
      -t|--toolchain)
        if test $# -gt 1; then
          echo "-- Building with toolchain: $2... --"
          if [[ $2 == "clang" ]]; then
            export CXX="clang++"
          fi
        else
          echo "No toolchain specified, please choose either 'gcc' or 'clang'"
          usage
          exit 1
        fi
        shift 2
        ;;
      -h|--help|*)
        usage
        exit 1
        ;;
    esac
  done
}

export BUILD_TYPE="Release"
export CXX="gcc"

parse_args $@
build