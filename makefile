.ONESHELL:

all: nialbasic

setup:
	make clean
	mkdir BuildCore/build
	mkdir BuildNial/build
	mkdir BuildNial/src

nialcore: setup
	cd BuildCore/build
	cmake ../src
	make
	mv nialcore ../../

# bootstraps nial, assuming you have a `nialcore` in the nial development dir
# useful if you're too lazy to wait for it to build twice, and already have a copy of `nialcore`
bootstrap:
	cd BuildNial/pkgblder
	./../../nialcore -defs buildfromcore
	cd ../build
	cmake ../src
	make
	mv nial ../../nialbasic

nialbasic: nialcore bootstrap

clean:
	rm -rf BuildCore/build
	rm -rf BuildNial/build
	rm -rf BuildNial/src
