#include <iostream>
#include <common.h>

#include "handle-management.hpp"
#include "creat.h"
#include "write.h"
#include "open.h"

#include "tclap/CmdLine.h"
#include "tclap/ValueArg.h"

using TCLAP::ArgException;
using TCLAP::CmdLine;
using TCLAP::UnlabeledValueArg;
using TCLAP::ValueArg;

int main(int argc, char** argv) {
    CmdLine cmd("Create new mdofs regular file");

    ValueArg<std::string> fileArg("f", "file", "Path to the existing filesystem file",
                                  true, "", "path");
    cmd.add(fileArg);

    UnlabeledValueArg<std::string> pathArg("path", "Path to the new file within the filesystem", true, "", "path");
    cmd.add(pathArg);

    ValueArg<std::string> copyFromArg("c", "copy", "Path to the existing file in native filesystem to copy content from",
                                  false, "", "path");
    cmd.add(copyFromArg);

    try {
        cmd.parse(argc, argv);
    } catch (ArgException& e) {
        std::cerr << "Error: " << e.what() << " for arg " << e.argId() << std::endl;
        return 1;
    }

    const int errCode = simplefs::openFilesystem(fileArg.getValue().c_str(), O_RDWR);

    if (errCode) {
        std::cerr << "Failed to open filesystem file\"" << fileArg.getValue() << '\"' << std::endl;
        return 1;
    }

    const int newFile = simplefs::creat(pathArg.getValue().c_str(), simplefs::AccessFlag::RDWR);

    if (newFile) {
        std::cerr << "creat operation failed" << std::endl;

        return 1;
    }

    simplefs::close(newFile);

    std::cout << "File \"" << pathArg.getValue() << "\" was created successfully" << std::endl;

    simplefs::closeFilesystem();

    return 0;
}


