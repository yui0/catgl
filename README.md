# catgl

Catgl is a cross-platform 3D graphics engine intended to be easy-to-use and high performance, which also supports the features for game development, such as animation, input device handling and sound playing.
This application is designed for Android, iOS, Linux, Windows.

## Technologies

- OpenGL / OpenGLES
- NanoVG (https://github.com/memononen/nanovg)
- GLFW (for Linux)
- Android NDK (for Android)

## Install

- Download android-sdk-linux and install it.
 - tools/android update sdk
  - Android SDK Tools
  - Android SDK Platform-tools
  - Android SDK Build-tools
  - API 10 SDK Platform
  - Android Support Library
- Download android-ndk
- yum install ant

## Examples

- Linux
```
cd sample/box
make
```

- Android
```
cd sample/box
make android
```
