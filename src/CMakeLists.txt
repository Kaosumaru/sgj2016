cmake_minimum_required (VERSION 2.8)
project (SGJ2016)

set(APPLICATION_MAJOR_VERSION 0)
set(APPLICATION_MINOR_VERSION 0)
set(APPLICATION_PATCH_VERSION 1)

set(APPLICATION_NAME "SGJ")


#snip this later
set(APPLICATION_STRING_VERSION "${APPLICATION_MAJOR_VERSION}.${APPLICATION_MINOR_VERSION}.${APPLICATION_PATCH_VERSION}")


#message(STATUS "TEST ${CMAKE_CURRENT_BINARY_DIR}")
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ProjectData)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/ProjectData/ProjectInfo.txt "${APPLICATION_NAME}\n${APPLICATION_STRING_VERSION}")

#configure build packaging system...
file(COPY ${PROJECT_SOURCE_DIR}/../Utils/release_make.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ProjectData/)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/ProjectData/deploy.bat "python release_make.py \"${CMAKE_CURRENT_BINARY_DIR}/\" \"${PROJECT_SOURCE_DIR}/\" \npause")
#end



add_definitions(-DMX_APPLICATION_MAJOR_VERSION=${APPLICATION_MAJOR_VERSION})
add_definitions(-DMX_APPLICATION_MINOR_VERSION=${APPLICATION_MINOR_VERSION})
add_definitions(-DMX_APPLICATION_PATCH_VERSION=${APPLICATION_PATCH_VERSION})

add_definitions(-DMX_APPLICATION_NAME="${APPLICATION_NAME}")
#to this.....



option (GAME_PERF_CHEATS "Cheats for perfomance?" FALSE)

if (GAME_PERF_CHEATS)
add_definitions(-DMX_GAME_PERF_CHEATS)
endif(GAME_PERF_CHEATS)

if (GAME_RELEASE)
add_definitions(-DMX_GAME_RELEASE)
endif(GAME_RELEASE)
add_definitions(-DMX_DEBUG_RES_PATH="${PROJECT_SOURCE_DIR}/Res/")

if (MSVC)
add_definitions( "/DMX_PLATFORM_MSVC  /DUNICODE /D_UNICODE" )
add_definitions( "/MP /W3 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /wd4250 /wd4244  /wd4503 /nologo /Zm500" )
endif(MSVC)



add_subdirectory(../MXLib_Cinder/MXLib "${CMAKE_CURRENT_BINARY_DIR}/MXLib_Cinder")

if (MSVC)
configure_msvc_runtime()
endif(MSVC)

add_subdirectory(../MXLib_Cinder/Cloudbase "${CMAKE_CURRENT_BINARY_DIR}/Cloudbase")
add_subdirectory(../MXLib_Cinder/LiteHTML "${CMAKE_CURRENT_BINARY_DIR}/LiteHTML")
add_subdirectory(../MXLib_Cinder/MSL "${CMAKE_CURRENT_BINARY_DIR}/MSL")

include_directories ("${MX_INCLUDES}")

set (EXTRA_LIBS ${EXTRA_LIBS} MXLib Cloudbase LiteHTML MSL RPN)


include(cotire)
include(enableC11)




# add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}/")
include_directories ("${PROJECT_SOURCE_DIR}/../MXLib_Cinder/RPN")
include_directories ("${PROJECT_SOURCE_DIR}/../MXLib_Cinder/MXLib")
include_directories ("${PROJECT_SOURCE_DIR}/../MXLib_Cinder/Cloudbase")
include_directories ("${PROJECT_SOURCE_DIR}/../MXLib_Cinder/LiteHTML")
include_directories ("${PROJECT_SOURCE_DIR}/")



##folders
add_folder(Game)


find_package(Boost REQUIRED)
include_directories( ${Boost_INCLUDE_DIRS} )

include_directories( ${CINDER_INCLUDE_DIR} )
link_directories( ${CINDER_LIBRARY_DIR}/msw/x86/ )

include_directories( ${PROJECT_SOURCE_DIR} )
include_directories( ${PROJECT_SOURCE_DIR}/Game )



add_definitions(-DASMJIT_STATIC)
if (APPLE)
	add_definitions(-DMX_PLATFORM_XCODE)

	if (GAME_RELEASE_XXX)

		FILE(GLOB_RECURSE resFiles "${PROJECT_SOURCE_DIR}/Res/*.*" )
		add_executable(SGJ2016 MACOSX_BUNDLE  main.cpp ${SRCS} ${resFiles})
  	  	set_source_files_properties(${resFiles}
  	      PROPERTIES
  	        MACOSX_PACKAGE_LOCATION Resources)

	else(GAME_RELEASE_XXX)
		add_executable(SGJ2016 MACOSX_BUNDLE main.cpp ${SRCS})
	endif(GAME_RELEASE_XXX)

	SET_TARGET_PROPERTIES(SGJ2016
	  PROPERTIES
	  MACOSX_BUNDLE true
	  MACOSX_BUNDLE_BUNDLE_NAME "SGJ2016"
	  # MACOSX_BUNDLE_INFO_STRING ""
	  MACOSX_BUNDLE_BUNDLE_VERSION "1"
	  MACOSX_BUNDLE_LONG_VERSION_STRING "1"
	  MACOSX_BUNDLE_SHORT_VERSION_STRING "1"
	  # MACOSX_BUNDLE_ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/sean.icns"
	  MACOSX_BUNDLE_GUI_IDENTIFIER "SGJ2016"
	  #MACOSX_BUNDLE_COPYRIGHT "artm"
	  )




else()
	set(WIN_RESOURCE_FILE ${PROJECT_SOURCE_DIR}/VS/Resources.rc ${PROJECT_SOURCE_DIR}/VS/resource.h)
	add_executable(SGJ2016 WIN32  main.cpp ${SRCS} ${WIN_RESOURCE_FILE})
endif()



if (MSVC)
SET_TARGET_PROPERTIES(SGJ2016 PROPERTIES LINK_FLAGS_DEBUG " /NODEFAULTLIB:\"LIBCMT\"")
endif(MSVC)


target_link_libraries (SGJ2016  ${EXTRA_LIBS})
target_link_libraries(SGJ2016 ${SDL_LIBRARY_TEMP} )
target_Link_Libraries(SGJ2016 ${RPN_Deps})
target_Link_Libraries(SGJ2016 RPN)
#set_target_properties(SDLTest PROPERTIES COTIRE_UNITY_LINK_LIBRARIES_INIT "COPY")

if (USE_COTIRE)
	cotire(SGJ2016)
	target_link_libraries(SGJ2016_unity MXLib_unity)
	target_link_libraries(SGJ2016_unity Cloudbase)
	target_link_libraries(SGJ2016_unity LiteHTML)
	target_link_libraries(SGJ2016_unity MSL)
	target_link_libraries(SGJ2016_unity ${SDL_LIBRARY_TEMP} )
endif()
