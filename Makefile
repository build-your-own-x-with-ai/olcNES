# Makefile for olcNES on macOS
# Builds the NES emulator using olcPixelGameEngine

CXX := clang++
CXXFLAGS := -std=c++17 -O2 -Wall -Wno-unknown-pragmas -Wno-inconsistent-missing-override
FRAMEWORKS := -framework OpenGL -framework GLUT -framework Carbon -framework OpenAL

# Use pkg-config for libpng if available, otherwise use Homebrew paths
PNG_CFLAGS := $(shell pkg-config --cflags libpng 2>/dev/null || echo "-I/opt/homebrew/opt/libpng/include/libpng16")
PNG_LIBS := $(shell pkg-config --libs libpng 2>/dev/null || echo "-L/opt/homebrew/opt/libpng/lib -lpng16")

# Output binary
TARGET := nes_emulator

.PHONY: all clean part7 run help

all: part7

part7: $(TARGET)

$(TARGET):
	@echo "Building NES Emulator (Part 7 - Full Version with Sound)..."
	cd "Part #7 - Mappers & Basic Sounds" && \
	$(CXX) $(CXXFLAGS) $(PNG_CFLAGS) \
		olcNes_Sounds1.cpp \
		Bus.cpp \
		Cartridge.cpp \
		Mapper.cpp \
		Mapper_000.cpp \
		Mapper_001.cpp \
		Mapper_002.cpp \
		Mapper_003.cpp \
		Mapper_004.cpp \
		Mapper_066.cpp \
		olc6502.cpp \
		olc2C02.cpp \
		olc2A03.cpp \
		$(FRAMEWORKS) $(PNG_LIBS) -o ../$(TARGET)
	@echo "Build successful! Binary: ./$(TARGET)"

run: $(TARGET)
	@echo "Running NES Emulator..."
	./$(TARGET)

clean:
	@echo "Cleaning build artifacts..."
	rm -f $(TARGET)
	@echo "Clean complete."

help:
	@echo "olcNES Makefile for macOS"
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build Part 7 (full emulator with sound)"
	@echo "  make part7    - Build Part 7 explicitly"
	@echo "  make run      - Build and run the emulator"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Requirements:"
	@echo "  - Xcode Command Line Tools"
	@echo "  - libpng (brew install libpng)"
	@echo ""
	@echo "Usage:"
	@echo "  ./$(TARGET) [path/to/rom.nes]"
