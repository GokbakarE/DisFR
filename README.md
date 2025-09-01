# DisFR - SuperH Disassembler

DisFR is a professional, lightweight SuperH architecture disassembler written in C++20.  
It compiles with modern C++ standards and is designed to be easily extensible and efficient.

## Features

- Supports Fujitsu FR instruction set 
- Modular source code structure with separate libraries  
- Static library support for reusable components  
- Cross-platform build support for Linux and macOS  
- Simple and clean Makefile-based build system  

## Requirements

- C++20 compatible compiler (e.g., g++ 10 or newer)  
- GNU Make  
- Linux or macOS environment  

## Build and Installation

To build the project, clone the repository and run:

```bash
git clone https://github.com/GokbakarE/DisFR.git
cd DisFR
make            
cd lib
make            
```

- After building and installation, the executable `DisFR` will be available in your system.

## Usage

Run the executable with the desired options and input files:

```bash
DisFR [options] <16-bit-input>
```

Options are:
```bash
--FR
```
## License

This project is licensed under the GNU AGPLv3 - see the [LICENSE.md](LICENSE.md) file for details.

## Author

GokbakarE
© 2025 GokbakarE. All rights reserved.