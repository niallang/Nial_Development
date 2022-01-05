# script to build nial on linux
# further information can be found at BuildCore/README.md and BuildNial/README.md
cd BuildCore
[ ! -d "build" ] && mkdir build
rm -r build/*
cd build
cmake ../src
make
cp nialcore ../../BuildNial/pkgblder
cd ../..

cd BuildNial
[ ! -d "build" ] && mkdir build
[ ! -d "src" ] && mkdir src
rm -r build/*
rm -r src/*
cd pkgblder
./nialcore -defs buildfromcore
cd ../build
cmake ../src
make
mv nial ../pkgblder/nial_basic
