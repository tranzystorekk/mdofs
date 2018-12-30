# mdofs

Minimum Data Obfuscation filesystem

A very simple userspace filesystem implementation with concurrent process access.

Project developed for the UXP1A unix programming course at WUT.

### Prerequisites

You need to have the following libraries/software installed:

* protobuf (specifically protobuf-lite)
* cmake

### Building

For more automated project configuration, import this project with the CLion IDE instead.

#### Step 1: Configure cmake

Run from project root:

`cmake -DCMAKE_BUILD_TYPE=<build type> -B<build dest> -H.`

where `<build type>` is one of {Debug, Release},
`<build dest>` is the build/cmake output directory for the chosen build type, e.g. `cmake-build-debug` for Debug.

#### Step 2: Build a target

`cmake --build <build dest> --target <target name> -j<number_of_jobs>`

### Components

#### Simplefs

This is the API for interacting with the filesystem.

#### Utils

These programs provide a user-friendly command line entry point to the filesystem.

##### init

##### cat

##### ls

##### mkdir

##### mkfile

##### rm
