#include "init.hpp"
#include "CatchTestConvenience.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include "common.h"

#include "directory.pb.h"

using fsproto::Directory;

TEST_CASE("MaxDirectorySizeIsCorrect") {
    Directory dir;
    const std::string longName(simplefs::MAX_FILENAME_LENGTH - 2, 'a');
    const unsigned int nodeNumber = 0;
    unsigned int fileNumber = 0;

    auto records = dir.mutable_records();

    while (records->size() < simplefs::MAX_RECORDS_IN_DIRECTORY) {
        std::ostringstream oss(longName, std::ios::ate);
        oss.width(2);
        oss.fill('0');
        oss << fileNumber;

        records->insert({oss.str(), nodeNumber});
        ++fileNumber;
    }

    REQUIRE_EQ(simplefs::MAX_DIRECTORY_SIZE, dir.ByteSizeLong());
}
