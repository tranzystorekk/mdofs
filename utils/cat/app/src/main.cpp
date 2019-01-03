#include <iostream>
#include <string>

#include "tclap/CmdLine.h"
#include "tclap/ValueArg.h"

#include "auxilliary.h"
#include "handle-management.hpp"
#include "common.h"

using TCLAP::ArgException;
using TCLAP::CmdLine;
using TCLAP::UnlabeledValueArg;
using TCLAP::ValueArg;

int main(int argc, char** argv) {
    CmdLine cmd("Print contents of an mdofs regular file");

    ValueArg<std::string> fileArg("f", "file", "Path to the new filesystem file (directories must exist)",
            true, "", "path");
    cmd.add(fileArg);

    UnlabeledValueArg<std::string> pathArg("path", "Path to the file in filesystem", true, "", "path");
    cmd.add(pathArg);

    try {
        cmd.parse(argc, argv);
    } catch (ArgException& e) {
        std::cerr << "Error: " << e.what() << " for arg " << e.argId() << std::endl;
        return 1;
    }

    simplefs::openFilesystem(fileArg.getValue().c_str(), O_RDONLY);

    std::cout << "Path \"" << pathArg.getValue() << "\" points to inode number "
              << simplefs::lookup(pathArg.getValue().c_str()) << std::endl;

    simplefs::closeFilesystem();

    return 0;
}