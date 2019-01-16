#ifndef MDOFS_OPENDIR_H
#define MDOFS_OPENDIR_H

#include <utility>

#include "directory.pb.h"

namespace simplefs {

using Dir = std::pair<int, fsproto::Directory>;

Dir opendir(const char* name);

} // simplefs

#endif //MDOFS_OPENDIR_H
