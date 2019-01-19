#include <iostream>
#include <common.h>

#include "handle-management.hpp"
#include "lseek.h"
#include "open.h"

#include "tclap/CmdLine.h"
#include "tclap/ValueArg.h"

#include <boost/filesystem.hpp>

#include <string>
#include <fstream>
#include <streambuf>

using TCLAP::ArgException;
using TCLAP::CmdLine;
using TCLAP::UnlabeledValueArg;
using TCLAP::ValueArg;

int main(int argc, char** argv) {
    CmdLine cmd("Print size of a mdofs regular file");

    ValueArg<std::string> fileArg("f", "file", "Path to the existing filesystem file",
                                  true, "", "path");
    cmd.add(fileArg);

    UnlabeledValueArg<std::string> pathArg("path", "Path to the file to print size within the filesystem", true, "", "path");
    cmd.add(pathArg);

    try {
        cmd.parse(argc, argv);
    } catch (ArgException& e) {
        std::cerr << "Error: " << e.what() << " for arg " << e.argId() << std::endl;
        return 1;
    }

    const int errCode = simplefs::openFilesystem(fileArg.getValue().c_str(), O_RDONLY);

    if (errCode) {
        std::cerr << "Failed to open filesystem file\"" << fileArg.getValue() << '\"' << std::endl;
        return 1;
    }

    const int openedFile = simplefs::open(pathArg.getValue().c_str(), simplefs::AccessFlag::READ);

    if (openedFile < 0) {
        std::cout << "Failed to open file\"" << pathArg.getValue() << "\"" << std::endl;


        return 1;
    }

//    std::cout << "File \"" << pathArg.getValue() << "\" was opened successfully" << std::endl;

    int file_size = 0;
    if ((file_size = simplefs::lseek(openedFile, simplefs::SeekFlag::END)) < 0) {
        std::cerr << "Failed to print size of filesystem file\"" << pathArg.getValue() << '\"' << std::endl;
        return 1;
    }

    std::cout << "File: \"" << pathArg.getValue() << "\"    " << "Size: " << file_size << std::endl;

    simplefs::close(openedFile);

    simplefs::closeFilesystem();

    return 0;
}





