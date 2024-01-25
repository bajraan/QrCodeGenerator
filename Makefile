CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

LIB_QRCODEGEN_DIR = ./lib_qrcodegen
LIB_SVGGEN_DIR = ./lib_svggen
SRC_DIR = .
OBJ_DIR = ./obj


ifeq ($(OS),Windows_NT)
	MKDIR_OBJ = if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	RM = del /q
	EXE_EXT = .exe
else
	MKDIR_OBJ = $(MKDIR) -p $(OBJ_DIR)
	RM = rm -f
	EXE_EXT =
endif


QRCODEGEN_SRC = $(wildcard $(LIB_QRCODEGEN_DIR)/*.cpp)
SVGGEN_SRC = $(wildcard $(LIB_SVGGEN_DIR)/*.cpp)
DEMO_SRC = $(wildcard $(SRC_DIR)/*.cpp)

OBJ_FILES = $(patsubst $(LIB_QRCODEGEN_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(QRCODEGEN_SRC)) \
            $(patsubst $(LIB_SVGGEN_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SVGGEN_SRC)) \
            $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(DEMO_SRC))

TARGET = QrCodeGeneratorDemo

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(MKDIR_OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(LIB_QRCODEGEN_DIR)/%.cpp
	$(MKDIR_OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(LIB_SVGGEN_DIR)/%.cpp
	$(MKDIR_OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
