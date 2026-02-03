## Installing CMake

### Windows
1. Download CMake from: https://cmake.org/download/
2. Run the installer
3. During installation, **check the option**:  
   `Add CMake to system PATH`
4. Restart your terminal

**Verify installation:**
```bash
cmake --version
```

## Building Project
1. Generate build files with CMake
```bash
cmake -B build
```
2. Build the project
```bash
cmake --build build
```

## Running the Program

After build the project, you will run the program:
```bash
.\build\Debug\Final.exe
```

### Note: Working with MSVC compiler or Visual Studio 17 2022
