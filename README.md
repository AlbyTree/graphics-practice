The starting code is based on the work of the [OpenGL series](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2 "OpenGL Playlist") by [Cherno](https://github.com/TheCherno "Cherno GitHub").
The current repo structure is fairly simple:
- `res` folder contains shader source files and texture files
- `src/core` folder contains the main code needed to create a simple application
- `src/deps` folder contains some external dependencies (libraries, headers, ...) needed for building an application
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

The application will search the shaders and textures inside the `res` folder from the current working directory: if the application crashes, try to set the working directory to where you cloned the repository (for example, I have set my working directory in Visual Studio to `$(ProjectDir)\repo` where `repo` is where I have cloned this repository); otherwise, copy the `res` folder in your current working directory.

### Results
- Smooth Bezier curve: you can move the control points to change the curve shape or increase/decrease the number of points used for the curve  

https://user-images.githubusercontent.com/34436621/129155991-9959625e-2662-4065-afc7-8a862ce3c8ce.mp4

> Work in progress...
