lnx: lnx.cpp
	g++ -std=c++14 -o lnx lnx.cpp
install:
	cp -i lnx /usr/bin/
