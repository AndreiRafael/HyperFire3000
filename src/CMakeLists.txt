file(GLOB_RECURSE sources ./*.cpp ./*.h ./*.hpp)

include_directories(vht PUBLIC ../include/SDL/SDL2/include)
include_directories(vht PUBLIC ../include/SDL/SDL2_image)
include_directories(vht PUBLIC ../include/SDL/SDL2_ttf)
include_directories(vht PUBLIC ../include/SDL/SDL2_mixer)
include_directories(vht PUBLIC ../include/json)

add_executable(vht ${sources})

if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		file(GLOB_RECURSE libs ${CMAKE_SOURCE_DIR}/lib/win32/x64/*.lib)
		file(GLOB_RECURSE dlls ${CMAKE_SOURCE_DIR}/lib/win32/x64/*.dll)
		set(MY_EXE_DIR "${CMAKE_SOURCE_DIR}/bin/win32/x64")
	else()
		file(GLOB_RECURSE libs ${CMAKE_SOURCE_DIR}/lib/win32/x86/*.lib)
		file(GLOB_RECURSE dlls ${CMAKE_SOURCE_DIR}/lib/win32/x86/*.dll)
		set(MY_EXE_DIR "${CMAKE_SOURCE_DIR}/bin/win32/x86")
	endif()
	
	set_target_properties(vht PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MY_EXE_DIR})
	
	foreach(f ${dlls})
		file(COPY ${f} DESTINATION ${MY_EXE_DIR})
	endforeach()
	target_link_libraries(vht PUBLIC ${libs})
endif(WIN32)
if(UNIX)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
    find_package(SDL2 REQUIRED)
    target_link_libraries(vht ${SDL2_LIBRARIES} SDL2_image SDL2_ttf SDL2_mixer)
    target_link_libraries (vht pthread)
	
	set(MY_EXE_DIR "${CMAKE_SOURCE_DIR}/bin/unix")
endif(UNIX)

	file(GLOB resources ${CMAKE_SOURCE_DIR}/resources/*)
	foreach(f ${resources})
		file(COPY ${f} DESTINATION ${MY_EXE_DIR})
	endforeach()

	set_target_properties(vht PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MY_EXE_DIR})