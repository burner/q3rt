TARGET=q3rt
CFLAGS=-I/usr/include/QtCore -I/usr/include/QtGui -I/usr/include/QtOpenGL \
-ggdb -Wall -D STIMER -finline-functions -I/gl -I./glm

LDFLAGS=-ggdb -Wall -lGL -lQtOpenGL -lQtGui -lQtCore -lpthread

OBJS=main.o ShaderHandle.o vec3f.o tri.o ObjLoader.o Obj.o vec2f.o \
Camera.o utils.o vec3d.o Octree.o ObjInsManager.o ObjIns.o geomUtil.o Renderer.o \
SkyBox.o vec3q.o Focus.o

all: $(TARGET)

BUILD_NUMBER_FILE=../buildID
BUILD_NUMBER_LDFLAGS  = -Xlinker --defsym -Xlinker __BUILD_DATE=$$(date +'%Y%m%d')
BUILD_NUMBER_LDFLAGS += -Xlinker --defsym -Xlinker __BUILD_NUMBER=$$(cat $(BUILD_NUMBER_FILE))

$(TARGET) : $(LIBS) $(OBJS)
	@if ! test -f $(BUILD_NUMBER_FILE); then echo 0 > $(BUILD_NUMBER_FILE); fi
	@echo $$(($$(cat $(BUILD_NUMBER_FILE)) + 1)) > $(BUILD_NUMBER_FILE) 
	$(LD) $(LDFLAGS) -Wall $(OBJS) -o $(TARGET) $(shell /usr/local/bin/sdl-config --cflags --libs) $(BUILD_NUMBER_LDFLAGS)

count:
	wc -l `find -name "*.h" -or -name "*.cpp" -o -path ./glm -prune`

clean:
	rm -f *.o $(TARGET)


SkyBox.o: src/SkyBox.cpp src/SkyBox.h Makefile
	$(CC) $(CFLAGS) -c src/SkyBox.cpp

ShaderHandle.o: src/ShaderHandle.cpp src/ShaderHandle.h Makefile
	$(CC) $(CFLAGS) -c src/ShaderHandle.cpp

vec2f.o: src/vec2f.cpp src/vec2f.h Makefile
	$(CC) $(CFLAGS) -c src/vec2f.cpp

vec3f.o: src/vec3f.cpp src/vec3f.h Makefile
	$(CC) $(CFLAGS) -c src/vec3f.cpp

main.o: src/main.cpp Makefile
	$(CC) $(CFLAGS) -c src/main.cpp

tri.o: src/tri.cpp src/tri.h Makefile
	$(CC) $(CFLAGS) -c src/tri.cpp

ObjLoader.o: src/ObjLoader.cpp src/ObjLoader.h src/vec3f.h src/vec2f.h src/tri.h Makefile
	$(CC) $(CFLAGS) -c src/ObjLoader.cpp

utils.o: src/utils.cpp src/utils.h Makefile
	$(CC) $(CFLAGS) -c src/utils.cpp

Obj.o: src/Obj.cpp src/Obj.h Makefile
	$(CC) $(CFLAGS) -c src/Obj.cpp

Focus.o: src/Focus.cpp src/Focus.h Makefile
	$(CC) $(CFLAGS) -c src/Focus.cpp

Camera.o: src/Camera.h src/Camera.cpp Makefile
	$(CC) $(CFLAGS) -c src/Camera.cpp

vec3d.o: src/vec3d.cpp src/vec3d.h Makefile
	$(CC) $(CFLAGS) -c src/vec3d.cpp

vec3q.o: src/vec3q.cpp src/vec3q.h Makefile
	$(CC) $(CFLAGS) -c src/vec3q.cpp

Octree.o: src/Octree.cpp src/Octree.h Makefile
	$(CC) $(CFLAGS) -c src/Octree.cpp

ObjInsManager.o: src/ObjInsManager.cpp src/ObjInsManager.h Makefile
	$(CC) $(CFLAGS) -c src/ObjInsManager.cpp

ObjIns.o: src/ObjIns.cpp src/Octree.h Makefile
	$(CC) $(CFLAGS) -c src/ObjIns.cpp

geomUtil.o: src/geomUtil.cpp src/geomUtil.h Makefile
	$(CC) $(CFLAGS) -c src/geomUtil.cpp

Renderer.o: src/Renderer.h src/Renderer.cpp Makefile
	$(CC) $(CFLAGS) -c src/Renderer.cpp	
