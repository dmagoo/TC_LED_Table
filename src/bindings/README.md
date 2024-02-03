## Project Setup and Build Instructions

### Setting Up the Project

1. **Clone the Repository**
   - Clone this repository to your local machine.
   - Ensure you have Python, CMake, and a C++ compiler installed.

2. **Initialize Submodules**
   - If the project uses Git submodules (e.g., for `pybind11`), initialize them:
     ```
     git submodule update --init --recursive
     ```

### Building the C++ Library

1. **Navigate to the Build Directory**
   - From the project root, create a new build directory (if not already present) and navigate into it:
     ```
     mkdir build && cd build
     ```

2. **Generate Build Configuration**
   - Run CMake to configure the project:
     ```
     cmake ..
     ```

3. **Compile the Library**
   - Compile the `LedTableApiLib` library:
     ```
     cmake --build . --target LedTableApiLib
     ```

### Setting Up Python Bindings

1. **Compile Python Bindings**
   - Ensure the `LedTableApiLib` is built as per the above steps.
   - Build the Python bindings module (`ledtableapi`):
     ```
     cmake --build . --target ledtableapi
     ```

### Testing the Bindings

1. **Testing Python Bindings**
   - After building, the Python extension module will be in the build directory. Test it by running:
     ```
     python -c "import ledtableapi; ledtableapi.hello_world()"
     ```

### Notes

- Replace `LedTableApiLib` and `ledtableapi` with actual target names used in your project if they differ.
- Adjust paths and commands based on your operating system and project structure.
