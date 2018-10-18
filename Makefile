
picture_lib: Main.o Utils.o Colour.o Picture.o PicNode.o LockablePicNodeList.o PicLibrary.o
	g++ `pkg-config --cflags opencv` Main.o Utils.o Colour.o Picture.o PicNode.o LockablePicNodeList.o PicLibrary.o `pkg-config --libs opencv` -o picture_lib -lpthread
	
Utils.o: Utils.hpp Utils.cpp

Colour.o: Colour.hpp Colour.cpp

Picture.o: Utils.hpp Picture.hpp Picture.cpp

PicNode.o: PicNode.hpp PicNode.cpp

LockablePicNodeList.o: LockablePicNodeList.hpp LockablePicNodeList.cpp

PicLibrary.o: Utils.hpp PicLibrary.hpp PicLibrary.cpp

Main.o: Main.cpp Utils.hpp Colour.hpp Picture.hpp PicLibrary.hpp

%.o: %.cpp
	g++ -std=c++11 -c $< -lpthread

clean:
	rm -rf picture_lib *.o

.PHONY: clean
