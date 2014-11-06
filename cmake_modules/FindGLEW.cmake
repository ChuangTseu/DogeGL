MESSAGE("ENV GLEWDIR is $ENV{GLEWDIR}")

 FIND_PATH(GLEW_INCLUDE_DIR GL/glew.h
   HINTS
   $ENV{GLEWDIR}
   PATH_SUFFIXES include/GL include
   PATHS
   ~/Library/Frameworks
   /Library/Frameworks
   /usr/local/include/GL
   /usr/include/GL
   /sw # Fink
   /opt/local # DarwinPorts
   /opt/csw # Blastwave
   /opt
 )
 MESSAGE("GLEW_INCLUDE_DIR is ${GLEW_INCLUDE_DIR}")

 FIND_LIBRARY(GLEW_LIBRARY
   NAMES glew32 GLEW
   HINTS
   $ENV{GLEWDIR}
   PATH_SUFFIXES lib64 lib lib/Release/Win32
   PATHS
   /sw
   /opt/local
   /opt/csw
   /opt
 )

 MESSAGE("GLEW_LIBRARY is ${GLEW_LIBRARY}")

 SET(GLEW_FOUND "NO")
 IF(GLEW_LIBRARY)
   # For SDL2main

   SET(GLEW_FOUND "YES")
 ENDIF(GLEW_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW
                                  REQUIRED_VARS GLEW_LIBRARY GLEW_INCLUDE_DIR)
