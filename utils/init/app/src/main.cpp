#include <iostream>
#include <string>

#include <cstdint>

#include "tclap/CmdLine.h"
#include "tclap/ValueArg.h"

#include "init.hpp"

static const unsigned int DEFAULT_MAX_FILES = 1000;

using TCLAP::ArgException;
using TCLAP::CmdLine;
using TCLAP::ValueArg;

unsigned int parseSize(const std::string& s) {
    unsigned long result = 0;
    size_t pos;
    const size_t stringSize = s.size();

    try {
        result = std::stoul(s, &pos);
    } catch (std::invalid_argument& e) {
        return 0;
    }

    // parse format letter
    if (pos != stringSize) {
        if (pos == stringSize - 1) {
            const char format = s[pos];

            if (format == 'K') {
                result *= 1024;
            } else if (format == 'M') {
                result *= 1024 * 1024;
            } else if (format == 'G') {
                result *= 1024 * 1024 * 1024;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    }

    if (result > UINT32_MAX) {
        return 0;
    }

    return result;
}

int main(int argc, char** argv) {
    CmdLine cmd("Create a new simplefs filesystem instance");

    ValueArg<std::string> fileArg("f", "file", "Path to the new filesystem file (directories must exist)",
            true, "", "path");
    cmd.add(fileArg);

    ValueArg<unsigned int> maxFilesArg("", "max-files", "Maximum file number in the filesystem (Default: 1000)", false,
            DEFAULT_MAX_FILES, "integer");
    cmd.add(maxFilesArg);

    ValueArg<std::string> sizeArg("", "size",
            "Effective filesystem size [in bytes or K - kilobytes, M - megabytes, G - gigabytes] (Default: 1M)."
            " Warning: supports only sizes lower than 4G",
            false, "1M", "size format");
    cmd.add(sizeArg);

    try {
        cmd.parse(argc, argv);
    } catch (ArgException& e) {
        std::cerr << "Error: " << e.what() << " for arg " << e.argId() << std::endl;
        return 1;
    }

    const unsigned int filesystemSize = parseSize(sizeArg.getValue());

    if (!filesystemSize) {
        std::cerr << "Error: Incorrect file size argument" << std::endl;
        return 1;
    }

    fsinit::createFilesystem(fileArg.getValue(), maxFilesArg.getValue(), filesystemSize);

    std::cout << "New mdofs instance \"" << fileArg.getValue() << "\" created successfully" << std::endl;

    return 0;
}
