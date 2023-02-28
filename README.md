The starting code is based on the work of the [OpenGL series](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2 "OpenGL Playlist") by [Cherno](https://github.com/TheCherno "Cherno GitHub").
The current repo structure is fairly simple:
- `res` folder contains shader source files and texture files
- `src/core` folder contains the main code needed to create a simple application
- `src/deps` folder contains some external dependencies (libraries, headers, ...) needed for building an application
- `src/tests` folder contains personal experiments 

### How to build

A Visual Studio 2022 solution for Windows 10 x64 is provided with the needed dependencies (headers, sources and libraries):

- GLEW (OpenGL loading library), GLFW (cross-platform window and input handling)
- GLM (math library, GLSL compliant)
- IMGUI (Immediate UI) 
- stb_image (image handling)

Using OpenGL 3.3, GLSL 3.3 .

### Results
- Smooth Bezier curve: you can move the control points to change the curve shape or increase/decrease the number of points used for the curve  

https://user-images.githubusercontent.com/34436621/129155991-9959625e-2662-4065-afc7-8a862ce3c8ce.mp4

> Work in progress...
