#include <iostream>
#include <common.h>

#include "handle-management.hpp"
#include "creat.h"
#include "read.h"
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
using boost::filesystem::exists;
using Path = boost::filesystem::path;

bool fileExists(const std::string& filename) {
    return exists(Path(filename));
}

int main(int argc, char** argv) {
    CmdLine cmd("View content of a mdofs regular file");

    ValueArg<std::string> fileArg("f", "file", "Path to the existing filesystem file",
                                  true, "", "path");
    cmd.add(fileArg);

    UnlabeledValueArg<std::string> pathArg("path", "Path to the file to be opened within the filesystem", true, "", "path");
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

    std::string str;
    char buf;
    int read_ret = 0;
    while (read_ret = simplefs::read(openedFile, &buf, 1) > 0) {
        str.push_back(buf);
    }
    if (read_ret < 0) {
        std::cerr << "Failed to read from\"" << pathArg.getValue() << '\"' << std::endl;
        return 1;
    }

//    std::cout << "File content: " << std::endl;
    std::cout << str;

    simplefs::close(openedFile);

    simplefs::closeFilesystem();

    return 0;
}




