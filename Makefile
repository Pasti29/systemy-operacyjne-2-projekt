SHELL = '/bin/sh'

TARGET_EXEC := program

BINARY_DIR := ./bin
BUILD_DIR := ./build
SOURCE_DIR := ./src

OBJS := main.o track.o track1.o track2.o


$(BINARY_DIR)/$(TARGET_EXEC) : $(BUILD_DIR)/main.o $(BUILD_DIR)/track.o $(BUILD_DIR)/track1.o $(BUILD_DIR)/track2.o
	mkdir -p $(@D)
	g++ -std=c++17 -lncurses -lpthread -o $(BINARY_DIR)/$(TARGET_EXEC) $(BUILD_DIR)/main.o $(BUILD_DIR)/track.o $(BUILD_DIR)/track1.o $(BUILD_DIR)/track2.o

$(BUILD_DIR)/main.o : ./src/main.cpp
	mkdir -p $(@D)
	g++ -c -std=c++17 ./src/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/track2.o : ./src/track2.cpp ./src/track2.h
	mkdir -p $(@D)
	g++ -c -std=c++17 ./src/track2.cpp -o $(BUILD_DIR)/track2.o

$(BUILD_DIR)/track1.o : ./src/track1.cpp ./src/track1.h
	mkdir -p $(@D)
	g++ -c -std=c++17 ./src/track1.cpp -o $(BUILD_DIR)/track1.o

$(BUILD_DIR)/track.o : ./src/track.cpp ./src/track.h
	mkdir -p $(@D)
	g++ -c -std=c++17 ./src/track.cpp -o $(BUILD_DIR)/track.o

.PHONY : clean
clean:
	rm -rf $(BUILD_DIR) $(BINARY_DIR)