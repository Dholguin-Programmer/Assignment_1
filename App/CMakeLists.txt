find_package(OpenGL REQUIRED)
set(target_name myapp)

add_executable(${target_name} app.cpp)

# target_include_directories(${target_name}
# PUBLIC
# 	"${GLAD_INCLUDE_DIRECTORIES}"
# )

target_link_libraries(${target_name}
	Device::DiegofiedWindow
	Device::BaseWindow
)

file(COPY fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY images DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

assignment_install(${target_name})