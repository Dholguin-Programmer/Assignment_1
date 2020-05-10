set(target_name BaseWindow)
add_library(${target_name} STATIC)
add_library(Device::${target_name} ALIAS ${target_name})

target_include_directories(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SubComponents/BaseRenderer/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SubComponents/BaseShader/include>

		# Pre-compiled paths
		"${GLAD_INCLUDE_DIRECTORIES}"
)

target_sources(${target_name}
	PRIVATE
		src/WindowBase.cpp
		SubComponents/BaseRenderer/src/BaseRenderer.cpp
		SubComponents/BaseShader/src/BaseShader.cpp
)

target_link_libraries(${target_name}
	# Pre-compiled libraries
	${GLAD_TARGET}
	${CMAKE_DL_LIBS}
	glm::glm
)

assignment_install(${target_name})