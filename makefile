CXX	:=g++
CFLAGS	:=-Wall -O -I"$(ALLEGRO_PATH)/include" -L"$(ALLEGRO_PATH)/lib"
LIBS	:=-lallegro-5.0.4-mt -lallegro_dialog-5.0.4-mt -lallegro_primitives-5.0.4-mt

.PHONY:	all
all:
	$(CXX) -o yace YACE/src/Chip8.cpp YACE/src/CPU.cpp src/MainApp.cpp src/main.cpp -s $(CFLAGS) $(LIBS)

.PHONY: debug
debug:
	$(CXX) -o yace-debug YACE/src/Chip8.cpp YACE/src/CPU.cpp src/MainApp.cpp src/main.cpp -D _DEBUG_ -g $(CFLAGS) $(LIBS)

