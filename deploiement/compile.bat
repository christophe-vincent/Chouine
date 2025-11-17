
cd Algo\wrapper_godot

REM Debug mode when developing with Godot
scons platform=windows

REM Web/html5 build
scons platform=web target=template_release

REM Android build - Release mode Key store needed
REM scons platform=android target=template_release arch=arm32
REM scons platform=android target=template_release arch=arm64 generate_android_binaries=yes

REM Android build - Debug mode
scons platform=android target=template_debug arch=arm32
scons platform=android target=template_debug arch=arm64 generate_android_binaries=yes