#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "./constants.h"

int main() {

    std::ifstream input("./listings/listing_0038_many_register_mov", std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

    for (int i = 0; i < buffer.size(); i += 2) {
        unsigned char operation = (buffer[i] & operationMask) >> operationShift;
        unsigned char direction = (buffer[i] & directionMask) >> directionShift;
        unsigned char wide = (buffer[i] & wideMask);

        unsigned char source = direction 
            ? (buffer[i + 1] & destinationMask) >> destinationShift 
            : (buffer[i + 1] & sourceMask) >> sourceShift;
        unsigned char destination = direction 
            ? (buffer[i + 1] & sourceMask) << sourceShift 
            : (buffer[i + 1] & destinationMask) >> destinationShift;

        std::cout << getOperation(operation) << " " << getRegister(source, wide) << ", " << getRegister(destination, wide) << std::endl;
    }
}

