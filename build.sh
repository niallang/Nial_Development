# script to build nial on linux
# further information can be found at BuildCore/README.md and BuildNial/README.md
cd BuildCore
mkdir -p build
rm -rf build/*
cd build
cmake ../src
make
cp nialcore ../../BuildNial/pkgblder
cd ../..

cd BuildNial
mkdir -p build
mkdir -p src
rm -rf build/*
rm -rf src/*
cd pkgblder
./nialcore -defs buildfromcore
cd ../build
cmake ../src
make
mv nial ../pkgblder/nial_basic
