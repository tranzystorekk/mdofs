#include <iostream>

#include "handle-management.hpp"
#include "mkdir.h"

#include "tclap/CmdLine.h"
#include "tclap/ValueArg.h"

using TCLAP::ArgException;
using TCLAP::CmdLine;
using TCLAP::UnlabeledValueArg;
using TCLAP::ValueArg;

int main(int argc, char** argv) {
    CmdLine cmd("Print contents of an mdofs regular file");

    ValueArg<std::string> fileArg("f", "file", "Path to the new filesystem file (directories must exist)",
                                  true, "", "path");
    cmd.add(fileArg);

    UnlabeledValueArg<std::string> pathArg("path", "Path to the new directory within the filesystem", true, "", "path");
    cmd.add(pathArg);

    try {
        cmd.parse(argc, argv);
    } catch (ArgException& e) {
        std::cerr << "Error: " << e.what() << " for arg " << e.argId() << std::endl;
        return 1;
    }

    simplefs::openFilesystem(fileArg.getValue().c_str(), O_RDWR);

    const int errCode = simplefs::mkdir(pathArg.getValue().c_str());

    if (errCode) {
        std::cerr << "mkdir operation failed" << std::endl;

        return 1;
    }

    std::cout << "Directory \"" << pathArg.getValue() << "\" was created successfully" << std::endl;

    simplefs::closeFilesystem();

    return 0;
}
