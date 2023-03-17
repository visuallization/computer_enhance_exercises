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

// the operation is stored in the upper 6 bits of the first byte: 11111100 = 252
unsigned char operationMask = 252;
// since the operation is stored in the upper 6 bits we have to shift byte 2 bits to the right to get the value
unsigned char operationShift = 2;

std::string getOperation(unsigned char operation) {
    return operations[operation] == "" ? "unknown operation" : operations[operation];
}

int main() {

    std::ifstream input("./listing_0037_single_register_mov", std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    
    unsigned char operation = (buffer[0] & operationMask) >> operationShift;
    std::cout << getOperation(operation) << std::endl;
}

