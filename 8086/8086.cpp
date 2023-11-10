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

// assembly register table
std::map<unsigned char, std::string> registers {
    // 0 = 000 = ax
    { 0, "ax" },
    // 1 = 001 = cx
    { 1, "cx" },
    // 2 = 010 = dx
    { 2, "dx" },
    // 3 = 011 = bx
    { 3, "bx" },
};

// the operation is stored in the upper 6 bits of the first byte: 11111100 = 252
unsigned char operationMask = 252;
// since the operation is stored in the upper 6 bits we have to shift byte 2 bits to the right to get the value
unsigned char operationShift = 2;

// the direction is stored in the second bit of the first byte: 00000010 = 2
unsigned char directionMask = 2;
// since the direction is stored in the second bit we have to shift byte 1 bit to the right to get the value
unsigned char directionShift = 1;

// the source is stored in the lower 3 bits of the second byte: 00000111 = 7
unsigned char sourceMask = 7;
// since the source is stored in the lower 3 bits we don't have to shift the byte
unsigned char sourceShift = 0;
// the destination is stored in the middle 3 bits of the second byte: 00111000 = 56
unsigned char destinationMask = 56;
// since the destination is stored in the middle 3 bits we have to shift byte 3 bits to the right to get the value
unsigned char destinationShift = 3;

std::string getOperation(unsigned char code) {
    return operations[code] == "" ? "unknown operation" : operations[code];
}

std::string getRegister(unsigned char code) {
    return registers[code] == "" ? "unknown register" : registers[code];
}

int main() {

    std::ifstream input("./listing_0037_single_register_mov", std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    
    unsigned char operation = (buffer[0] & operationMask) >> operationShift;
    unsigned char direction = (buffer[0] & directionMask) >> directionShift;

    unsigned char source = direction 
        ? (buffer[1] & destinationMask) >> destinationShift 
        : (buffer[1] & sourceMask) >> sourceShift;
    unsigned char destination = direction 
        ? (buffer[1] & sourceMask) << sourceShift 
        : (buffer[1] & destinationMask) >> destinationShift;

    std::cout << getOperation(operation) << " " << getRegister(source) << ", " << getRegister(destination) << std::endl;
}

