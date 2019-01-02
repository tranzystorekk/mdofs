#include "init.hpp"
#include "CatchTestConvenience.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "common.h"

#include "directory.pb.h"

#include "common.h"

using fsproto::Directory;

constexpr unsigned int CountDigits(unsigned int v, unsigned int n = 1) {
    return (v / 10 == 0) ? n : CountDigits(v / 10, n + 1);
}

void fillDirectory(Directory& dir) {
    const unsigned int MaxDigits = CountDigits(simplefs::MAX_FILENAME_LENGTH - 1);
    const std::string longName(simplefs::MAX_FILENAME_LENGTH - MaxDigits, '_');
    const unsigned int nodeNumber = 0;

    auto records = dir.mutable_records();

    int i = 0;
    while ( records->size() < simplefs::MAX_RECORDS_IN_DIRECTORY ) {
        std::ostringstream oss(longName, std::ios::ate);
        oss << std::setw(MaxDigits) << std::setfill('0') << i;

        records->insert({oss.str(), nodeNumber});
        ++i;
    }
}

TEST_CASE("MaxDirectorySizeIsCorrect") {
    Directory dir;
    fillDirectory(dir);

    REQUIRE_EQ(simplefs::MAX_DIRECTORY_SIZE, dir.ByteSizeLong());
}
