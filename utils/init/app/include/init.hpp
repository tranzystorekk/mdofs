#ifndef MDOFS_INIT_HPP
#define MDOFS_INIT_HPP

#include <string>

namespace fsinit {

void createFilesystem(const std::string& filepath, unsigned int maxFiles, unsigned int size);

} // fsinit

#endif //MDOFS_INIT_HPP
