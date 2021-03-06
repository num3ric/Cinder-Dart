### Dart runtime libs

This folder contains binaries and header files needed to embed the [dart][dartlang] runtime in a C / C++ app. It is meant to be used with [Cinder-Dart](https://github.com/richardeakin/Cinder-Dart). Currently only OS X binaries built with libc++ / 64bit are included.

Here are google's [checkout][checkout] and [build][build] instructions.

### Steps to rebuild dart static binaries for cinder


##### Mac OS X (62-bit):

*this is my personal experience when following the [official instructions][checkout]*

###### Install depot tools:

```
cd <directory where you want the depot_tools to live>
svn co http://src.chromium.org/svn/trunk/tools/depot_tools
export PATH=$PATH:`pwd`//depot_tools
```

###### Full svn checkout:
 
```
mkdir dart-svn-VERSION
cd dart-svn-VERSION
gclient config http://dart.googlecode.com/svn/branches/VERSION/deps/all.deps
gclient sync
```

###### Build:

First, need to set the C++ standard library to libc++. To do this, open up dart/runtime/dart-runtime.xcodeproject, go to the main project settings, and change it from “Compiler Default” to “libc++"

Then, close the Xcode project and run the following (faster than building in Xcode):

```
cd dart
./tools/build.py --arch=x64 runtime
./tools/build.py --arch=x64 --mode=release runtime
```

###### Packaging binaries with libtool:

Currently we need 6 of the static binaries produced by the above build commands, for each configuration. To improve this, the following commands smash these binaries into just one for each config, called 'libdart_runtime.a':

```
cd xcodebuild/DebugX64

libtool -static -o libdart_runtime.a libdart_builtin.a libdart_lib_withcore.a libdart_vm.a libdart_withcore.a libdouble_conversion.a libjscre.a
cp libdart_runtime.a $CINDER_DART_PATH/dart-runtime/lib/macosx/Debug/

cd ../ReleaseX64
// run the same libtool command as above
cp libdart_runtime.a $CINDER_DART_PATH/dart-runtime/lib/macosx/Release/
```

###### To generate the library snapshop binary

Run the followinng from xcodebuild/Debug:

```
./gen_snapshot --snapshot=snapshot_gen.bin
```


[dartlang]: http://www.dartlang.org/
[checkout]: https://code.google.com/p/dart/wiki/GettingTheSource
[build]: https://code.google.com/p/dart/wiki/Building#Building_everything