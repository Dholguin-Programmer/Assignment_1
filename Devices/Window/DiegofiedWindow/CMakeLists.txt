# Use Standard CMake modules to find binary/include paths
# for required packages
find_package(OpenGL REQUIRED)
find_package(CURL REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLM REQUIRED)

set(target_name DiegofiedWindow)
add_library(${target_name} STATIC)
add_library(Device::${target_name} ALIAS ${target_name})

target_include_directories(${target_name}
	PUBLIC
		# My include paths
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SubComponents/DiegofiedRenderer/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SubComponents/DiegofiedShader/include>

		# Pre-compiled paths
		"${GLAD_INCLUDE_DIRECTORIES}"
		
		# Standard CMake module paths
		"${CURL_INCLUDE_DIRS}"
		"${FREETYPE_INCLUDE_DIRS}"
		"${GLM_INCLUDE_DIRS}"
)

target_sources(${target_name}
	PRIVATE
		src/DiegofiedWindow.cpp
		SubComponents/DiegofiedRenderer/src/DiegofiedRenderer.cpp
		SubComponents/DiegofiedShader/src/DiegofiedShader.cpp
)

target_link_libraries(${target_name}
	# Pre-compiled libraries
	${GLAD_TARGET}
	${CMAKE_DL_LIBS}
	
	# Compiled from 3rd Party source code libraries
	glfw 
	nlohmann_json::nlohmann_json
	
	# Standard CMake Module based libraries
	${CURL_LIBRARIES}
	${FREETYPE_LIBRARIES}
	OpenGL::GL
	glm::glm
	
	# My libraries
	Device::BaseWindow
)

assignment_install(${target_name})