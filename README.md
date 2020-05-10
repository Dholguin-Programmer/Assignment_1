# Assignment_1 - DSS

## Dependencies
* Python3 - (Installed onto system)
* gcc/g++ - (Installed onto system)
* curl - (Installed onto system)
* freetype2 - (Installed onto system)
* Cmake 3.11 - (Installed onto system)
* GLAD - Built from source (see 3rdParty directory)
* GLM - Built from source (see 3rdParty directory)
* glfw - Built from source (see 3rdParty directory)
* json - Built from source (see 3rdParty directory)

## More details about 3rd party dependencies
* Cmake 3.11 (https://cmake.org/download/)
* GLM (https://glm.g-truc.net/0.9.9/index.html)
* json (https://github.com/nlohmann/json)
* GLAD (https://github.com/Dav1dde/glad)
* GLFW (https://github.com/glfw/glfw)

## Important Notes
For the *GLAD* dependency, the Assignment_1's CMakeLists assumes that this repository is build "in source"
i.e ran the following commands: 
* cd 3rdPartyLibs/glad
* cmake .
* make

## Possible Improvements

## How to run this?
First you'll need build *GLAD* as pointed out in the *Important Notes* section. 
After that, assuming you have the other dependencies installed, you'll just need to run
* python3 build.py
* cd .build/Assignment_1/App
* ./myapp