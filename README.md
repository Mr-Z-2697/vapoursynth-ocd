`core.ocd.dct(clip, inverse=0)`    

### build
I only tried mingw    
opencv has so many default enabled 3rd party components which is not needed for this simple plugin turn them off I'm too lazy to write them down 😥    
```
git clone https://github.com/opencv/opencv --depth 1 --branch 4.11.0
pushd opencv
mkdir build
pushd build
cmake .. -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_opencv_apps=OFF
cmake --build . --target install
popd
popd
c++ -shared vsocd.cpp -o vsocd.dll -I'/c/Program Files/VapourSynth/sdk/include' -I./opencv/build/install/include ./opencv/build/install/x64/mingw/staticlib/libopencv_core4110.a -v -O3 -march=x86-64-v3
```
