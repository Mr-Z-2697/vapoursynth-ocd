#!bash
git clone https://github.com/opencv/opencv --depth 1 --branch 5.x
pushd opencv
git apply ../opencv-no3rd-5x.diff
cmake . -Bbuild -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_opencv_apps=OFF -DWITH_FFMPEG=OFF -DWITH_UNIFONT=OFF
cmake --build build --target install
popd
CXX_FLAGS="-I./opencv/build/install/include -D_FORTIFY_SOURCE=2 -fstack-protector-strong -pipe -D__USE_MINGW_ANSI_STDIO=1 --start-no-unused-arguments -static-libgcc -static-libstdc++ --end-no-unused-arguments -O3 -march=x86-64-v3"
LD_FLAGS="-L./opencv/build/install/x64/mingw/staticlib -lopencv_imgproc500 -lopencv_core500 -llibclapack"
c++ -c -o dct.o dct.cpp $CXX_FLAGS
c++ -c -o gaussianblur.o gaussianblur.cpp $CXX_FLAGS
c++ -s -shared -o vsocd.dll vsocd.cpp dct.o gaussianblur.o $CXX_FLAGS $LD_FLAGS
