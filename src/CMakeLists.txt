file(GLOB_RECURSE sources ./*.cpp ./*.h ./*.hpp)

include_directories(${HF_GAME_TITLE} PUBLIC ../include/SDL/SDL2)
include_directories(${HF_GAME_TITLE} PUBLIC ../include/SDL/SDL2_image)
include_directories(${HF_GAME_TITLE} PUBLIC ../include/SDL/SDL2_ttf)
include_directories(${HF_GAME_TITLE} PUBLIC ../include/SDL/SDL2_mixer)
include_directories(${HF_GAME_TITLE} PUBLIC ../include/json)

include_directories(${HF_GAME_TITLE} PUBLIC ./core)
include_directories(${HF_GAME_TITLE} PUBLIC ./basic)
include_directories(${HF_GAME_TITLE} PUBLIC ./game)

add_executable(${HF_GAME_TITLE} ${sources})

set(HF_WIN32 (${HF_PLATFORM} STREQUAL "Win32"))
set(HF_UNIX (${HF_PLATFORM} STREQUAL "Unix"))

set(HF_64 (${HF_ARCHITECTURE} STREQUAL "x64"))

if(${HF_WIN32})
	if((CMAKE_SIZEOF_VOID_P EQUAL 8))
		file(GLOB_RECURSE libs ${CMAKE_SOURCE_DIR}/lib/win32/x64/*.lib)
	else()
		file(GLOB_RECURSE libs ${CMAKE_SOURCE_DIR}/lib/win32/x86/*.lib)
	endif()
	if(${HF_64})
		file(GLOB_RECURSE dlls ${CMAKE_SOURCE_DIR}/lib/win32/x64/*.dll)
		set(MY_EXE_DIR "${CMAKE_SOURCE_DIR}/bin/win32/x64")
	else()
		file(GLOB_RECURSE dlls ${CMAKE_SOURCE_DIR}/lib/win32/x86/*.dll)
		set(MY_EXE_DIR "${CMAKE_SOURCE_DIR}/bin/win32/x86")
	endif()
	
	set_target_properties(${HF_GAME_TITLE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MY_EXE_DIR})
	
	foreach(f ${dlls})
		file(COPY ${f} DESTINATION ${MY_EXE_DIR})
	endforeach()
	target_link_libraries(${HF_GAME_TITLE} PUBLIC ${libs})
endif(${HF_WIN32})
if(${HF_UNIX})
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
    find_package(SDL2 REQUIRED)
    target_link_libraries(${HF_GAME_TITLE} ${SDL2_LIBRARIES} SDL2_image SDL2_ttf SDL2_mixer)
    target_link_libraries (${HF_GAME_TITLE} pthread)
	
	set(MY_EXE_DIR "${CMAKE_SOURCE_DIR}/bin/unix")
endif(${HF_UNIX})

file(GLOB assets ${CMAKE_SOURCE_DIR}/assets/*)
foreach(f ${assets})
	file(COPY ${f} DESTINATION ${MY_EXE_DIR})
endforeach()

set_target_properties(${HF_GAME_TITLE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MY_EXE_DIR})