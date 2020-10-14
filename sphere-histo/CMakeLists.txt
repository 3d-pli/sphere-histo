CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(sphere-histo)

### Require out-of-source builds
FILE(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(AUTOMOC ON)
SET(AUTOUIC ON)
SET(CMAKE_CXX_STANDARD_REQUIRED 14)

FIND_PACKAGE(OpenGL)
FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Core Gui OpenGL)

find_package( Qt5Widgets REQUIRED )
find_package( Qt5OpenGL REQUIRED )

SET(LIBS ${LIBS} OpenGL::GL Qt5::Core Qt5::Gui Qt5::OpenGL ${OpenGL_LIBRARIES} ${QT_LIBRARIES})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/resources/)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/libs/cnpy-master)
ADD_SUBDIRECTORY(libs/cnpy-master)

SET(SOURCE 
	mainwindow.cpp 
	renderdata.cpp 
	spheredepthdata.cpp 
	spherewidget.cpp
)
SET(HEADER
	mainwindow.h 
	renderdata.h 
	spheredepthdata.h 
	spherewidget.h
)

qt5_wrap_ui(SOURCE mainwindow.ui)
qt5_wrap_cpp(QT5_SOURCES ${HEADER})

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp ${QT5_SOURCES} ${SOURCE})

QT5_USE_MODULES(${PROJECT_NAME} Core Widgets OpenGL)
TARGET_LINK_LIBRARIES(
	${PROJECT_NAME}
    	${LIBS}
	cnpy
)

