#ifndef MDOFS_OPENDIR_H
#define MDOFS_OPENDIR_H

#include <utility>

#include "directory.pb.h"

namespace simplefs {

/// Structure representing an open directory
using Dir = std::pair<int, fsproto::Directory>;

/// Open a special directory descriptor for reading of dir contents
/// \param name path to a directory
/// \return .first = file descriptor and .second = map<string, inode> of dir records if operation successful;
///         .first = -1 otherwise
Dir opendir(const char* name);

} // simplefs

#endif //MDOFS_OPENDIR_H
