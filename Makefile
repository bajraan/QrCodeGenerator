CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

LIB_QRCODEGEN_DIR = ./lib_qrcodegen
LIB_SVGGEN_DIR = ./lib_svggen
SRC_DIR = .
OBJ_DIR = ./obj

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
	if not exist "$(OBJ_DIR)" mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(LIB_QRCODEGEN_DIR)/%.cpp
	if not exist "$(OBJ_DIR)" mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(LIB_SVGGEN_DIR)/%.cpp
	if not exist "$(OBJ_DIR)" mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
