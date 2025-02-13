`core.ocd.dct(clip, inverse=0)`
build
```
cmake .. -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_opencv_apps=OFF
ninja install
cd /trunk
clang++ -shared vsocd.cpp -o vsocd.dll -I'/c/Program Files/VapourSynth/sdk/include' -I/trunk/opencv/build/install/include /trunk/opencv/build/install/x64/mingw/staticlib/libopencv_core4110.a -v -O3 -march=x86-64-v3
```
