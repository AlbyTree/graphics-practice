The starting code is based on the work of the [OpenGL series](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2 "OpenGL Playlist") by [Cherno](https://github.com/TheCherno "Cherno GitHub").
The current repo structure is fairly simple:
- `res` folder contains shader source files and texture files
- `src/core` folder contains the main code needed to create a simple application
- `src/deps` folder contains all the external dependencies (libraries, headers, ...) needed for building an application
- `src/tests` folder contains personal experiments to test graphics concepts

### How to build

**IMPORTANT**: I tested this only on Windows 10 with Visual Studio 2019.  
An application with this code requires:

- OpenGL 3.3, GLSL 1.3
- [GLEW](http://glew.sourceforge.net/index.html "GLEW") >= 2.1.0 (header files and static library)
- [GLFW](https://www.glfw.org/download "GLFW") >= 3.3.4 (header files and static library)

Add to the compiler include directories:
- The GLEW and GLFW header files folders
- The `src\deps` folder

Add to the compiler library directories:
- The GLEW and GLFW static libraries folders

Add to the linker input files the libraries:
- `opengl32, glfw3, glew32s`

### Results
> Work in progress...
