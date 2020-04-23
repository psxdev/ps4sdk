# PS4 SDK

![](https://avatars0.githubusercontent.com/u/15625873?v=3&s=128)
**ps4sdk** is a modular open source SDK for the PS4 with userland and kernel support.

The SDK currently supports most of the standard C library, various FreeBSD 9.0 userland
 and kernel, as well as some SCE functions.

It is designed to be adaptable to new firmwares and entry points and new reverse engineered
functions can be integrated into the SDK, by adding headers, function signatures and their
names to the list of function stubs.

Currently, running user and kernel code on firmwares ~**5.05** is supported.

## Prerequisites
* A unix or linux system, VM or Container
	* If your OS doesn't have Clang installed you will need to build clang and binutils from source 
	* for Windows, i dont use it so no help from me
* `clang 3.7` or later
* `make`

## Building toolchain and binutils example for orbisdev environment

For macOS
```
  mkdir -p /usr/local/orbisdev/git
  mkdir crossllvm
  cd crossllvm
  git clone http://llvm.org/git/llvm.git
  cd llvm/tools
  git clone http://llvm.org/git/clang.git
  git clone http://llvm.org/git/lld.git
  cd ../projects
  git clone http://llvm.org/git/compiler-rt.git
  cd ../..
  ```
  
  You must create build directory outside of llvm
  
  ```
  mkdir build
  cd build
  ```
  
  Now prepare configuration cmake and python needed
  
  ```
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local/orbisdev/toolchain -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_DEFAULT_TARGET_TRIPLE=x86_64-scei-ps4 -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCOMPILER_RT_BUILD_BUILTINS:BOOL=OFF -DCOMPILER_RT_BUILD_SANITIZERS:BOOL=OFF -DCOMPILER_RT_CAN_EXECUTE_TESTS:BOOL=OFF -DCOMPILER_RT_INCLUDE_TESTS:BOOL=OFF -DLLVM_TOOL_COMPILER_RT_BUILD:BOOL=OFF  -DCLANG_BUILD_EXAMPLES:BOOL=ON -DLLVM_TARGETS_TO_BUILD=X86 -DCMAKE_C_FLAGS="-Wdocumentation -Wno-documentation-deprecated-sync" -DCMAKE_CXX_FLAGS="-std=c++11 -Wdocumentation -Wno-documentation-deprecated-sync" -DLLVM_LIT_ARGS="-v" ../llvm
  ```
  
  Compiling and install
  
  ```
  cmake --build .
  cmake --build . --target install
  ```
  
  After compiling and install you will have a fresh clang using latest version with the same compiling options from Sony. However Sony is using propietary linker so we need a valid linker for macOS(freebsd has not this problem already has one).
  
  Download binutils 2.25 and compile it with:
  ```
  ./configure --prefix="$PS4DEV/host-osx" --target="x86_64-pc-freebsd9" \
  	--disable-nls \
  	--disable-dependency-tracking \
  	--disable-werror \
  	--enable-ld \
  	--enable-lto \
  	--enable-plugins \
  	--enable-poison-system-directories
  make
  make install
  ```
  
  to avoid use osx native tools
  add this to your orbisdev.sh environment script only for osx using my configuration:
  ```
  cat orbidev.sh
  PS4DEV=/usr/local/orbisdev;export PS4DEV
  PATH=$PS4DEV/host-osx/x86_64-pc-freebsd9/bin:$PS4DEV/toolchain/bin:$PATH
  ```
  
  Clang is searching for orbis-ld when you compile to fix that :
  
  ```
  cd /usr/local/orbisdev/host-osx/x86_64-pc-freebsd9/bin
  cp ld orbis-ld
  ```
  
  Now we can compile valid elf for PlayStation 4 from macOS :)
  

## Building
The building process is quite easy, just do
```
make
```

This will take some time, as it will build all libraries. In addition, if you are updating
from a **git pull**, `make clean && make` instead of `make` may be required (and is advised),
to ensure proper functionality.

## Instalation

Before continue you need to be sure that the `PS4SDK` bash variable has been defined, because is in the specific folder used during the instalation process.

You can do something as:
```
export PS4SDK=/usr/local/orbisdev/ps4sdk
```

If all was fine install with:
```
make install
```

## Running code

To execute elf code, you will need a pkg with orbislink embed or other method. It will load homebrew.elf compiled with this sdk from host0 from your mac/pc listening with ps4sh


## Compiling your own code
The sdk is a collection of static libraries and headers. Once you have the header files
and the static libraries build or downloaded, you can compile your own code against them.

However, since the SDK is modular, it provides several interchangeable libs which overlap
in functionality and must be linked in a specific order to shadow and complement each
other appropriately. It is strongly advised that you use an example `Makefile` from
[samples](https://github.com/orbisdev/liborbis), which also provides the appropriate build flags
to the compiler.

Should you decide to use a provided example Makefile, you will have to specify the
ps4sdk folder as an environment variable e.g. `export PS4SDK=/home/<user>/ps4sdk`.
The Makefile will then build any source files in the local project folder `source`
using the headers found in the folder `include`, just as if your code would be one
of the examples. `ps4-elf` and `ps4-lib` are handy targets to generate executables
or libs also a `ps4-bin` for others sdks over there who still don't support elf format.

