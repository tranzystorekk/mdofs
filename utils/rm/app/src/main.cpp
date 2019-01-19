#include <iostream>
#include <sstream>
#include <common.h>

#include "handle-management.hpp"
#include "unlink.h"

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
    CmdLine cmd("Delete file from mdofs filesystem");

    ValueArg<std::string> fileArg("f", "file", "Path to the existing filesystem file",
                                  true, "", "path");
    cmd.add(fileArg);

    UnlabeledValueArg<std::string> pathArg("path", "Path to the file to be deleted within the filesystem", true, "", "path");
    cmd.add(pathArg);

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

    const int ret_status = simplefs::unlink(pathArg.getValue().c_str());

    if (ret_status < 0) {
        std::cerr << "unlink operation failed" << std::endl;

        return 1;
    }

    std::cout << "File \"" << pathArg.getValue() << "\" was deleted successfully" << std::endl;

    simplefs::closeFilesystem();

    return 0;
}




