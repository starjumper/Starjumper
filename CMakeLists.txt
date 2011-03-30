#starjumper root CMakeLists.txt
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
PROJECT( StarJumper )

#include cmake modules to find osg, osgw and osgb
SET( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}" )


#define where to place starjumper.exe, this should be the root dirctory so models an other files can be found
#SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bintest )
#MAKE_DIRECTORY( ${EXECUTABLE_OUTPUT_PATH} )
#MARK_AS_ADVANCED( EXECUTABLE_OUTPUT_PATH )
SET( EXECUTABLE_OUTPUT_PATH "../bin/" )


#enable multi processor building with Windows, OSX coming soon
IF( WIN32 AND MSVC )
    OPTION( WIN32_USE_MP "Build with multiple processors." ON )
    MARK_AS_ADVANCED( WIN32_USE_MP )
    IF( WIN32_USE_MP )
        SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" )
    ENDIF( WIN32_USE_MP )
ENDIF( WIN32 AND MSVC )

#add cmake macros
INCLUDE( CMakeMacros )



#try to find necessary libraries
FIND_PACKAGE( OSG REQUIRED )
FIND_PACKAGE( OSGWorks REQUIRED )
FIND_PACKAGE( Bullet REQUIRED )
FIND_PACKAGE( OSGBullet REQUIRED )
FIND_PACKAGE ( OpenAL REQUIRED )
FIND_PACKAGE ( ALUT REQUIRED )
FIND_PACKAGE ( OGG REQUIRED )
FIND_PACKAGE ( Vorbis REQUIRED )
FIND_PACKAGE ( VorbisFile REQUIRED )

#include necessary files
INCLUDE_DIRECTORIES(
    include
	${OSG_INCLUDE_DIR}
	${OGG_INCLUDE_DIE}
	${BULLET_INCLUDE_DIR}
	${OSGWORKS_INCLUDE_DIR}
	${OSGBULLET_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${VORBIS_INCLUDE_DIR}
    ${VORBISFILE_INCLUDE_DIR}
)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)

#installer configuration
INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_GENERATOR "NSIS")
SET(CPACK_PACKAGE_VERSION_MAJOR "6")
SET(CPACK_PACKAGE_VERSION_MINOR "6")
SET(CPACK_PACKAGE_VERSION_PATCH "6")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The awesome Starjumper project")
SET(CPACK_PACKAGE_VENDOR "Quartet of awesomenes: Tim, Robert, Johannes, Stefan")

INCLUDE( CPack )