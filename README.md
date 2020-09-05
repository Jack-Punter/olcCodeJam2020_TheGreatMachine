# olcCodeJam2020 THE GREAT MACHINE!
This is my entry for the 2020 olcCodeJam. The theme is The Great Machine!

Your job is to duplicate all of the black boxes that make up the incomplete Great Machine!

# How To Build

## Windows
The `build.bat` file provided is all that is necessary to build the project.
It does however have the prerequisite that msvc's cl.exe is exported to the evironment.
In order to do this call the vcvarsall.bat file within the visual studio install path.
Below is an example on where this may be and shows the parameters required.
`C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat x64`

## Linux
The `build_gcc.sh` file provided is used to build the project. This project requires some packeges to
be installed, follow this guide for installing these: https://github.com/OneLoneCoder/olcPixelGameEngine/wiki/Compiling-on-Linux
P.s It may also be necessary to add execute permisions to the `data_desk/build_gcc.sh` file

# Level Generator
I was lazy so SPDene wrote a python script to generate the OnUserCreate for the level base classes.
To use this, from the root directory of the repostitory do (Requires Python 3.7):
`python LevelGen.py [numInputs] [numOutputs] <optionalLevelName>`
The generated file will be placed into `src/generated/`

# Data Desk
All files found in the data_desk folder are courtesy of Ryan Fleury: https://github.com/ryanfleury/data_desk
