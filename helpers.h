#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <iostream>
#include <string>
#include <fstream>

#define LOGERR(msg) std::cerr << msg << '\n';

inline char* readFile(const std::string& filename, int* outLength) {
    std::ifstream file(filename, std::ifstream::binary);

    file.seekg (0, file.end);
    int length = file.tellg();
    file.seekg (0, file.beg);

//    std::cerr << "File length: " << length << '\n';

    char* buffer = new char [length];

    file.read(buffer, length);

    //buffer[length] = 0;

    if (file) {
//        std::cerr << "all characters read successfully.\n";
    }
    else {
        std::cerr << "error: only " << file.gcount() << " could be read\n";
        delete[] buffer;

        buffer = nullptr;
    }

    file.close();

    *outLength = length;
    return buffer;
}

#endif // HELPERS_H
