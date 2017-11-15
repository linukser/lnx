lnx: lnx.cpp
	g++ -std=c++17 -o lnx lnx.cpp
install:
	cp -i lnx /usr/bin/
