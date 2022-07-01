#!/usr/bin/env bash

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
    -u | --unittest       Run all BSM unit tests after building
    -ct| --clang-tidy     Enable clang-tidy static analysis checks. Will only run if building (i.e. source changed, '-r', or first build)
    -h | --help           Display this help message
  "
}

function clean() {
  rm -rf ${BUILD_DIR}
}

function install_deps() {
  [[ ! -d ${BUILD_DIR} ]] && mkdir -p ${BUILD_DIR}

  pushd ${BUILD_DIR} > /dev/null
  conan install ${ROOT}/conanfile.txt --build -s compiler=${CCOMPILER} -s compiler.version=${CVERSION}
  popd > /dev/null
}

function set_compile_link() {
  if [[ ! -L ${ROOT}/compile_commands.json ]]; then
    # Symlink compile_commands.json artefact to project ROOT
    # This sets LSP definitions for clangd.
    ln -s ${BUILD_DIR}/compile_commands.json ${ROOT}
  fi
}

function set_build_env() {
  [[ "${BUILD_TYPE,,}" == "debug" ]] && export BUILD_TYPE="Debug"

  if [[ "${CXX,,}" == "clang" ]]; then
    export CXX="clang++"
    export CCOMPILER="clang"
  fi
}

function build() {
  if [[ ! -f ${BUILD_DIR}/conanbuildinfo.txt ]]; then
    echo "-- No conan packages found. Installing now... --"
    install_deps
  fi

  local threads=$(nproc)
  [[ -z threads ]] && threads=1

  cmake -S ${ROOT} -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
      -DCMAKE_CXX_COMPILER=${CXX} \
      -DCLANG_TIDY=${CLANG_TIDY}

  cmake --build ${BUILD_DIR} -- -j${threads}

  set_compile_link
}

function unit_test() {
  [[ ${UNITTEST} == true ]] && ${BUILD_DIR}/bin/bsm_tests
}

function set_base_env() {
  export BUILD_TYPE="Release"
  export CXX="g++"
  export CCOMPILER="gcc"
  export CVERSION="11.2"
  export UNITTEST=false
  export CLANG_TIDY=false
}

function clear_env() {
  unset BUILD_TYPE
  unset CXX
  unset CCOMPILER
  unset CVERSION
  unset UNITTEST
  unset CLANG_TIDY
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
        shift
        ;;
      -t|--toolchain)
        if test $# -gt 1; then
          echo "-- Building with toolchain: $2... --"
          if [[ $2 == "clang" ]]; then
            export CCOMPILER="clang"
            export CXX="clang++"
            export CVERSION="14"
          fi
        else
          echo "No toolchain specified, please choose either 'gcc' or 'clang'"
          usage
          exit 1
        fi
        shift 2
        ;;
      -u|--unittest)
        echo "-- Set unit tests to run after building. --"
        export UNITTEST=true
        shift
        ;;
      -ct|--clang-tidy)
        echo "-- Enabling clang-tidy static analysis. --"
        export CLANG_TIDY=true
        shift
        ;;
      -h|--help|*)
        usage
        exit 1
        ;;
    esac
  done
}


set_base_env
parse_args $@
set_build_env
build
unit_test
clear_env
