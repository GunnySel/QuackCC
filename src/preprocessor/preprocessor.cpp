#include "preprocessor.h"
#include <cstdlib>
#include <iostream>

std::string processFile(std::string filename)
{
    std::string preprocessFilename = filename.substr(0, filename.size()-2) + "i";
    std::string preprocessCmd = 
        "clang -E -P -x c -std=c11   -undef -nostdinc   -I . -isystem .   -D__Q_LANG__=1 " +
        filename + " -o " + preprocessFilename + " ";
        
    std::system(preprocessCmd.c_str());

    return preprocessFilename;
}
