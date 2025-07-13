#!bash
git clone https://github.com/opencv/opencv --depth 1 --branch 5.x
pushd opencv
cmake . -Bbuild -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_opencv_apps=OFF
cmake --build build --target install
popd
CXX_FLAGS="-I./opencv/build/install/include -L./opencv/build/install/x64/mingw/staticlib -lopencv_imgproc500 -lopencv_core500 -llibclapack -v -O3 -march=x86-64-v3 -s"
c++ -c -o dct.o dct.cpp $CXX_FLAGS
c++ -c -o gaussianblur.o gaussianblur.cpp $CXX_FLAGS
c++ -shared -o vsocd.dll vsocd.cpp dct.o gaussianblur.o $CXX_FLAGS
