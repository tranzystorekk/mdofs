#include <iostream>

#include "open.h"
#include "opendir.h"

#include "handle-management.hpp"

#include "tclap/CmdLine.h"
#include "tclap/ValueArg.h"

using TCLAP::ArgException;
using TCLAP::CmdLine;
using TCLAP::UnlabeledValueArg;
using TCLAP::ValueArg;

int main(int argc, char** argv) {
    CmdLine cmd("Print contents of an mdofs regular file");

    ValueArg<std::string> fileArg("f", "file", "Path to the existing filesystem file",
                                  true, "", "path");
    cmd.add(fileArg);

    UnlabeledValueArg<std::string> pathArg("path", "Path to the listed directory", true, "", "path");
    cmd.add(pathArg);

    try {
        cmd.parse(argc, argv);
    } catch (ArgException& e) {
        std::cerr << "Error: " << e.what() << " for arg " << e.argId() << std::endl;
        return 1;
    }

    simplefs::openFilesystem(fileArg.getValue().c_str(), O_RDONLY);

    simplefs::Dir listedDir = simplefs::opendir(pathArg.getValue().c_str());

    if (listedDir.first == -1) {
        std::cerr << "Could not list contents of \"" << pathArg.getValue() << "\"" << std::endl;

        return -1;
    }

    for (auto& record : *listedDir.second.mutable_records()) {
        std::cout << record.first << '\n';
    }

    simplefs::close(listedDir.first);

    simplefs::closeFilesystem();

    return 0;
}