CSW = -O3 -Wall -std=c++11
LSW = -std=c++11

all:
	make dc dc2

dc: dc.o Makefile
	g++ dc.o -o dc $(LSW)

dc.o: dircount.cc Makefile
	g++ dircount.cc -c -o dc.o $(CSW)

dc2: dc2.o Makefile
	g++ dc2.o -o dc2 $(LSW)

dc2.o: dircount_v2.cc Makefile
	g++ dircount_v2.cc -c -o dc2.o $(CSW)

clean:
	touch Makefile; make
