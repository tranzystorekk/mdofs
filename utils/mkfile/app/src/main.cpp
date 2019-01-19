#include <iostream>
#include <sstream>
#include <common.h>

#include "handle-management.hpp"
#include "creat.h"
#include "write.h"
#include "open.h"

#include "tclap/CmdLine.h"
#include "tclap/ValueArg.h"
#include "tclap/SwitchArg.h"

#include <boost/filesystem.hpp>

#include <string>
#include <fstream>
#include <streambuf>

using TCLAP::ArgException;
using TCLAP::CmdLine;
using TCLAP::UnlabeledValueArg;
using TCLAP::ValueArg;
using TCLAP::SwitchArg;
using boost::filesystem::exists;
using Path = boost::filesystem::path;

bool fileExists(const std::string& filename) {
    return exists(Path(filename));
}

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

    SwitchArg inputArg("i", "input", "Read file from standard input");

    cmd.add(inputArg);

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

    if (newFile < 0) {
        std::cerr << "creat operation failed" << std::endl;

        return 1;
    }

    if (!copyFromArg.getValue().empty() && inputArg.getValue()) {
        std::cerr << "Both flags -i and -c set" << std::endl;
        return 1;
    }

    if (!copyFromArg.getValue().empty()) {
        if(fileExists(copyFromArg.getValue())) {
            std::ifstream fileToCopy(copyFromArg.getValue().c_str());
            std::string str((std::istreambuf_iterator<char>(fileToCopy)), std::istreambuf_iterator<char>());

            if (simplefs::write(newFile, str.c_str(), str.size()) < 0) {
                std::cerr << "Failed to write to file in filesystem\"" << pathArg.getValue() << '\"' << std::endl;
                return 1;
            }
        } else {
            std::cerr << "File \"" << copyFromArg.getValue() << "\" does not exist to copy from\"" << std::endl;
            return 1;
        }
    } else if (inputArg.getValue()){
        std::cin >> std::noskipws;
        std::istream_iterator<char> it(std::cin);
        std::istream_iterator<char> end;
        std::string str(it, end);

        if (simplefs::write(newFile, str.c_str(), str.size()) < 0) {
            std::cerr << "Failed to write to file in filesystem\"" << pathArg.getValue() << '\"' << std::endl;
            return 1;
        }
    }

    simplefs::close(newFile);

    std::cout << "File \"" << pathArg.getValue() << "\" was created successfully" << std::endl;

    simplefs::closeFilesystem();

    return 0;
}


