.ONESHELL:

nial: nialbasic
	rm -rf BuildNial/build/*
	rm -rf BuildNial/src/*
	cd BuildNial/pkgblder
	./nial_basic -defs addfeatures
	cd ../build
	cmake ../src
	make
	mv nial ../../
	
setup:
	mkdir -p BuildCore/build
	mkdir -p BuildNial/build
	mkdir -p BuildNial/src

nialcore: setup
	cd BuildCore/build && cmake ../src && make
	mv nialcore ../../

nialbasic: nialcore
	mv BuildCore/build/nialcore BuildNial/pkgblder/nial_basic
	cd BuildNial/pkgblder
	./nialcore -defs buildfromcore
	cd ../build
	cmake ../src
	make
	cp nial ../pkgblder/nial_basic
	mv nial ../../nialbasic

clean:
	rm -rf BuildCore/build/*
	rm -rf BuildNial/build/*
	rm -rf BuildNial/src/*

