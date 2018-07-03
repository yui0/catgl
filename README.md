# catgl

Catgl is a cross-platform 3D graphics engine intended to be easy-to-use and high performance, which also supports the features for game development, such as animation, input device handling and sound playing.
This application is designed for Android, iOS, Linux, Windows.

## Technologies

- OpenGL / OpenGLES
- GLSL (OpenGL Shading Language)
- NanoVG (https://github.com/memononen/nanovg)
- stb single-file public domain libraries (https://github.com/nothings/stb)
- GLFW (for Linux)
- Android NDK (for Android)

## Install

- for Linux
    - dnf install glfw-devel
- for Android
    - Download android-sdk-linux and install it.
        - tools/android update sdk
            - Android SDK Tools
            - Android SDK Platform-tools
            - Android SDK Build-tools
            - API 10 SDK Platform
            - Android Support Library
    - Download android-ndk
    - dnf install ant

## Examples

- Linux

```bash
$ cd sample/box
$ make
```

- Android

```bash
$ cd sample/box
$ make android
```

## How to use

```main.c
#define CATGL_IMPLEMENTATION
#include "catgl.h"

void caInit(int width, int height)
{
}

void caRender()
{
}

void caEnd()
{
}
```

```bash
$ clang -std=c11 -Os -MMD -MP -Wall -Wextra -Winit-self -Wno-unused-parameter -Wno-float-equal -Wno-missing-braces -I../../ -I../../nanovg -o obj/main.o -c main.c
$ clang++ -o hello obj/main.o /lib/libglfw.so /lib/libGL.so /lib/libm.so 
```

## Screenshot

![Screenshot](sample/nanovg_eyes/eyes.png)
![Screenshot](sample/box_GL1/box.png)
![Screenshot](sample/viewer/viewer.png)
[![Screenshot](sample/glsl/glsl00.png)](http://yui0.github.io/catgl/sample/glsl/WebGL/000.html)
[![Screenshot](sample/glsl/glsl01.png)](http://yui0.github.io/catgl/sample/glsl/WebGL/001.html)
[![Screenshot](sample/glsl/glsl02.png)](http://yui0.github.io/catgl/sample/glsl/WebGL/002.html)
