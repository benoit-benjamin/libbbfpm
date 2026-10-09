<div align="center">
	<img src="assets/logo.png" width="150" height="150">
	<h1>BBFPM (Benjamin Benoit's Format for Project Metadata) format processor library for C</h1>
</div>

<div>
    <img alt="GitHub Actions Workflow Status" src="https://img.shields.io/github/actions/workflow/status/benoit-benjamin/libbbfpm/cmake-single-platform.yml?style=plastic">
    <img alt="GitHub License" src="https://img.shields.io/github/license/benoit-benjamin/libbbfpm?style=plastic">
    <img alt="GitHub Release" src="https://img.shields.io/github/v/release/benoit-benjamin/libbbfpm?style=plastic">
    <img alt="GitHub Tag" src="https://img.shields.io/github/v/tag/benoit-benjamin/libbbfpm?style=plastic">
</div>

## Introduction

BBFPM _(Benjamin Benoit's Format for Project Metadata)_ is a format created with the purpose of having project metadata (e.g: name, current version, last updated date, etc..) in one place, editing and accessing to it in an easy way.

This project started with a specific problem: project's metadata definition inside the source code. An example of this, is the version. Some times, I like to print out the current version of the program, and I used to put it inside some kind of "metadata file" inside the source code of the project, but, in reality, I wasn't really comfortable storing metadata this way. So, it led to in the idea of creating my own type of file format where I could store, edit and use all of that metadata, separated from the source code. And I came up with this format.

**But.. Why not TOML, YAML, JSON or any other existing format?**

The reason is, because I thought it could be a fun project to do.

## BBFPM Syntax

Read `documentation/BBFPM/Syntax.md` for BBFPM syntax reference.

## Library API

Read `documentation/BBFPM/API.md` for library API reference.

## Build and installation

Get the source code:

```
git clone https://github.com/benoit-benjamin/libbbfpm
```

Import the library to your CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.16)
project(your_project)

# This assumes the source code is available in third_party/libbbfpm
add_subdirectory(third_party/libbbfpm EXCLUDE_FROM_ALL)

add_executable(your_executable main.c)

# Link to the libbfpm library.
target_link_libraries(your_executable PRIVATE libbbfpm)
```

## License

This project is licensed under the GPLv3 License. Read LICENSE.md for more information.
