# GeometricTransformation

## Overview

GeometricTransformation is a C++ project that provides a graphical user interface (GUI) for performing geometric transformations. The application supports manipulating graphical objects such as points, lines, circles, and figures using affine and projective transformations.

## Features

- Geometric object manipulation
- Affine transformations
- Projective transformations
- Intuitive graphical user interface
- Support for multiple geometric primitives (points, lines, circles, figures)

## Prerequisites

- Qt5 or Qt6
- CMake 3.16+
- C++17 compatible compiler (GCC, Clang, MSVC)

### Build and Run

1. Clone the repository
```bash
git clone https://github.com/budweisserr/GM.git
cd GM
```

2. Create a build directory
```bash
mkdir build
cd build
```

3. Configure the project

For Qt5 (specify Qt5 path if needed):
```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt5
./GM
```

For Qt6:
```bash
cmake ..
```

4. Build the project
```bash
cmake --build .
```

5. Run the application
```bash
./GM
```
