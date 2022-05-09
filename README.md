# A timeslice library {#mainpage}

[![doxygen](https://github.com/tqfx/timeslice/actions/workflows/doxygen.yml/badge.svg)](https://github.com/tqfx/timeslice/actions/workflows/doxygen.yml)

## required tools

- C/C++ compiler
- [cmake](https://cmake.org/download)

## build

```bash
cmake -S . -B build
cmake --build build
```

## using

```cmake
find_package(timeslice CONFIG REQUIRED)
target_link_libraries(<TARGET> PRIVATE timeslice::timeslice)
```

## Copyright {#copyright}

Copyright (C) 2020 tqfx, All rights reserved.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.
