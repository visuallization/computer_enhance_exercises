#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

// assembly operations table
std::map<unsigned char, std::string> operations {
    // 34 = 100010 = mov: register to/from register
    { 34, "mov" }
};

// assembly register table (8 bit)
std::map<unsigned char, std::string> registers {
    // 0 = 000 = al
    { 0b000, "al" },
    // 1 = 001 = cl
    { 0b001, "cl" },
    // 2 = 010 = dl
    { 0b010, "dl" },
    // 3 = 011 = bl
    { 0b011, "bl" },
    // 4 = 100 = ah
    { 0b100, "ah" },
    // 5 = 101 = ch
    { 0b101, "ch" },
    // 6 = 110 = dh
    { 0b110, "dh" },
    // 7 = 111 = bh
    { 0b111, "bh" }
};

// assembly wide register table (16 bit)
std::map<unsigned char, std::string> wideRegisters {
    // 0 = 000 = ax
    { 0b000, "ax" },
    // 1 = 001 = cx
    { 0b001, "cx" },
    // 2 = 010 = dx
    { 0b010, "dx" },
    // 3 = 011 = bx
    { 0b011, "bx" },
    // 4 = 100 = sp
    { 0b100, "sp" },
    // 5 = 101 = bp
    { 0b101, "bp" },
    // 6 = 110 = si
    { 0b110, "si" },
    // 7 = 111 = di
    { 0b111, "di" }
};

// the operation is stored in the upper 6 bits of the first byte: 11111100 = 252
unsigned char operationMask = 0b11111100;
// since the operation is stored in the upper 6 bits we have to shift byte 2 bits to the right to get the value
unsigned char operationShift = 2;

// the direction is stored in the second bit of the first byte: 00000010 = 2
unsigned char directionMask = 0b00000010;
// since the direction is stored in the second bit we have to shift byte 1 bit to the right to get the value
unsigned char directionShift = 1;

// the wide flag is stored in the first bit of the first byte: 00000001 = 1
unsigned char wideMask = 0b00000001;

// the source is stored in the lower 3 bits of the second byte: 00000111 = 7
unsigned char sourceMask = 0b00000111;
// since the source is stored in the lower 3 bits we don't have to shift the byte
unsigned char sourceShift = 0;

// the destination is stored in the middle 3 bits of the second byte: 00111000 = 56
unsigned char destinationMask = 0b00111000;
// since the destination is stored in the middle 3 bits we have to shift byte 3 bits to the right to get the value
unsigned char destinationShift = 3;

std::string getOperation(unsigned char code) {
    return operations[code] == "" ? "unknown operation" : operations[code];
}

std::string getRegister(unsigned char code, unsigned char wide = 0) {
    if (wide) {
        return wideRegisters[code] == "" ? "unknown register" : wideRegisters[code];
    } else {
        return registers[code] == "" ? "unknown register" : wideRegisters[code];
    }
}

int main() {

    std::ifstream input("./listings/listing_0037_single_register_mov", std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    
    unsigned char operation = (buffer[0] & operationMask) >> operationShift;
    unsigned char direction = (buffer[0] & directionMask) >> directionShift;
    unsigned char wide = (buffer[0] & wideMask);

    unsigned char source = direction 
        ? (buffer[1] & destinationMask) >> destinationShift 
        : (buffer[1] & sourceMask) >> sourceShift;
    unsigned char destination = direction 
        ? (buffer[1] & sourceMask) << sourceShift 
        : (buffer[1] & destinationMask) >> destinationShift;

    std::cout << getOperation(operation) << " " << getRegister(source, wide) << ", " << getRegister(destination, wide) << std::endl;
}

