# Assignment_1 - DSS

## Dependencies
* Python3 - (Installed onto system)
* gcc/g++ - (Installed onto system)
* curl - (Installed onto system)
* freetype2 - (Installed onto system)
* OpenGL - (Installed onto system)
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

## Future Tasks
### Design
  * Implement Composite & Observer/Subject Design Patterns with Game Objects
    * https://github.com/Dholguin-Programmer/Assignment_1/blob/master/App/app.cpp#L211-L220
  * Create Network Device
    * https://github.com/Dholguin-Programmer/Assignment_1/blob/master/App/app.cpp#L23-L79
  * Create Space API (Model - View - Projection)
    * https://github.com/Dholguin-Programmer/Assignment_1/blob/master/App/app.cpp#L81-L94
    * https://github.com/Dholguin-Programmer/Assignment_1/blob/master/Devices/Window/DiegofiedWindow/SubComponents/DiegofiedRenderer/src/DiegofiedRenderer.cpp#L74-L80
  * Create Scene API
    * https://github.com/Dholguin-Programmer/Assignment_1/blob/master/Devices/Window/DiegofiedWindow/src/DiegofiedWindow.cpp#L219-L236
### System Improvement Fixes
* We always perform a download, whether we already have latest mlb stats or not
  * https://github.com/Dholguin-Programmer/Assignment_1/blob/master/App/app.cpp#L116 
* Be able to parse both 'old' and 'new' mlb data (not all are formatted the same)
  * https://github.com/Dholguin-Programmer/Assignment_1/blob/master/App/app.cpp#L53-L55
  * https://github.com/Dholguin-Programmer/Assignment_1/blob/master/App/app.cpp#L167
* Use some sort of intelligence when selecting core vs compatability profile
  * https://github.com/Dholguin-Programmer/Assignment_1/blob/master/Devices/Window/DiegofiedWindow/src/DiegofiedWindow.cpp#L48-L52
* Support window resizing
  * https://github.com/Dholguin-Programmer/Assignment_1/blob/master/Devices/Window/DiegofiedWindow/src/DiegofiedWindow.cpp#L59-L61
* Integrate UID into project
  * https://github.com/Dholguin-Programmer/Assignment_1/blob/master/Devices/Window/DiegofiedWindow/SubComponents/DiegofiedRenderer/src/DiegofiedRenderer.cpp#L198-L202
* Support a wider range of character codes (currently supports ASCII only)
  * https://github.com/Dholguin-Programmer/Assignment_1/blob/master/Devices/Window/DiegofiedWindow/SubComponents/DiegofiedRenderer/src/DiegofiedRenderer.cpp#L326-L334

## Possible Issues? 
* Make objects, no matter the number, fully rendered within the users screen 
* Resolve any warnings

## How to run this?
First you'll need build *GLAD* as pointed out in the *Important Notes* section. 
After that, assuming you have the other dependencies installed, you'll just need to run
* python3 build.py
* cd .build/Assignment_1/App
* ./myapp