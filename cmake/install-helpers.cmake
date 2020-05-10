include(GNUInstallDirs)

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "" CACHE PATH "..." FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_INSTALL_PREFIX "" CACHE PATH "..." FORCE)
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

string(APPEND CMAKE_INSTALL_BINDIR "${CMAKE_INSTALL_XPATH}")
string(APPEND CMAKE_INSTALL_LIBDIR "${CMAKE_INSTALL_XPATH}")

set(CMAKE_INSTALL_BINDIR $ENV{OE_INSTALL_PREFIX}/bin)
set(CMAKE_INSTALL_LIBDIR $ENV{OE_INSTALL_PREFIX}/lib)

message(STATUS "CMAKE_INSTALL_BINDIR: ${CMAKE_INSTALL_BINDIR}")
message(STATUS "CMAKE_INSTALL_LIBDIR: ${CMAKE_INSTALL_LIBDIR}")


###########################################################
function(assignment_install target)

    # Install into standard, non-uniform buckets
    install(TARGETS ${target_name}
		    ARCHIVE DESTINATION lib
		    LIBRARY DESTINATION lib
		    RUNTIME DESTINATION bin
    )
    
    # Install libs into project specific directories 
    # project_install(${target_name})
endfunction()