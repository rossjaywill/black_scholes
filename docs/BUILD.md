## Building BSM

### Build System

A shell script, `build.sh`, is provided to wrap the build system for BSM.
The build system is comprised of:
* CMake (generate Makefile targets)
* Conan package manager
* GNU gcc (default compiler toolchain), or optionally clang
* Optional Catch2 unit tests (included via Conan in build)

### System Dependencies

In order to avail of the above and build BSM, pre-requisite system dependencies must be installed:

* g++/clang++
* make
* cmake
* python3
* pip (python pip package manager, required for Conan)

This can be done on a debian based system with:
```bash
sudo apt install -y gcc clang make cmake python3 python3-pip
sudo pip install --user conan
```

### Building

Once the system dependencies are installed, the build can be run with:

```bash
mkdir build
./build.sh    # build release variant
./build.sh -d # build debug variant
./build.sh -u # build with unit tests
./build.sh -r # force rebuild, helpful if switching toolchains
./build.sh -c # clean build artefacts
```
All build flags:
```bash
  -d | --debug          Debug build
  -t | --toolchain      Specify which compiler toolchain to use (gcc/clang)
  -i | --install-deps   Install conan dependencies before building
  -c | --clean          Clean build artefacts
  -r | --rebuild        Clean build artefacts before fully rebuilding
  -u | --unittest       Run all BSM unit tests after building
  -ct| --clang-tidy     Enable clang-tidy static analysis checks. Will only run if building (i.e. source changed, '-r', or first build)
  -h | --help           Display this help message
```
### Troubleshooting

Common issues include:
1. If switching toolchains/variants you may run into an issue where conan dependencies do not have ABI with your current build:
```bash
-- Current conanbuildinfo.cmake directory: /home/rjw/code/black_scholes/build
CMake Error at build/conanbuildinfo.cmake:804 (message):
  Incorrect 'clang', is not the one detected by CMake: 'GNU'
```
  1. a. If this is the case, the dependencies can be rebuilt by passing `r` to the build script above.
