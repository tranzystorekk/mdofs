#include "init.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include "directory.pb.h"
#include "inode.pb.h"

#include "common.h"
#include "proto-helpers.hpp"

static const unsigned int FIRST_FREE_NODE_INDEX = 1;

using boost::filesystem::absolute;
using boost::filesystem::exists;
using fsproto::Directory;
using fsproto::Inode;
using fsproto::InodeTable;
using simplefs::AccessFlag;
using simplefs::protohelpers::lengthEncodeMsg;
using simplefs::protohelpers::setInode;
using simplefs::protohelpers::setInodeAsDirectory;
using Path = boost::filesystem::path;

bool fileExists(const std::string& filename) {
    return exists(Path(filename));
}

bool parentPathExists(const std::string& filename) {
    return exists( absolute(Path(filename)).parent_path() );
}

void fillInodeTable(InodeTable& table, unsigned int targetSize) {
    while (table.inodes_size() < targetSize) {
        Inode* node = table.add_inodes();
        setInode(node, 0, 0, 0, true);
    }
}

void fsinit::createFilesystem(const std::string& filepath, unsigned int maxFiles, unsigned int size) {
    if ( fileExists(filepath) ) {
        std::cerr << "Error: specified file already exists" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if ( !parentPathExists(filepath) ) {
        std::cerr << "Error: parent path of specified file does not exist" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // add root with '.' and '..' records
    Directory rootDirectory;

    // records is a Protobuf Map pointer
    auto records = rootDirectory.mutable_records();
    records->insert({".", 0});
    records->insert({"..", 0});

    InodeTable table;
    table.set_first_free_node_index(FIRST_FREE_NODE_INDEX);
    table.set_filesystem_origin(0);
    table.set_first_free_byte(0);

    // directories are length encoded with their effective size
    // but preallocated according to the Maximum Size
//    const unsigned int directorySize = simplefs::MAX_DIRECTORY_SIZE + sizeof(uint32_t);
    table.set_directory_size(simplefs::MAX_ENCODED_DIRECTORY_SIZE);

    table.set_max_inodes(maxFiles);

    Inode* root = table.add_inodes();
    setInodeAsDirectory(root, 0, AccessFlag::RWEX);

    fillInodeTable(table, maxFiles);

    // size of inode table plus length encoding
    const size_t tableSize = table.ByteSize() + sizeof(uint32_t);

    table.set_filesystem_origin(tableSize);
    table.set_first_free_byte(simplefs::MAX_ENCODED_DIRECTORY_SIZE);

    // create new filesystem file and resize it to desired length
    int newFile = creat(filepath.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    ftruncate(newFile, size + tableSize);

    // serialize inode table to new file
    const std::string encodedInodeTable = lengthEncodeMsg(table);
    write(newFile, encodedInodeTable.c_str(), encodedInodeTable.size());

    // serialize root directory
    const std::string encodedRootDir = lengthEncodeMsg(rootDirectory);
    write(newFile, encodedRootDir.c_str(), encodedRootDir.size());

    close(newFile);
}
