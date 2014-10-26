MESSAGE("ENV ASSIMPDIR is $ENV{ASSIMPDIR}")

 FIND_PATH(ASSIMP_INCLUDE_DIR assimp/postprocess.h assimp/scene.h assimp/version.h assimp/cimport.h
   HINTS
   $ENV{ASSIMPDIR}
   PATH_SUFFIXES include/assimp include
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
 MESSAGE("ASSIMP_INCLUDE_DIR is ${ASSIMP_INCLUDE_DIR}")

 FIND_LIBRARY(ASSIMP_LIBRARY
   NAMES assimp
   HINTS
   $ENV{ASSIMPDIR}
   PATH_SUFFIXES lib
   PATHS
   /sw
   /opt/local
   /opt/csw
   /opt
 )

 MESSAGE("ASSIMP_LIBRARY is ${ASSIMP_LIBRARY}")

 SET(ASSIMP_FOUND "NO")
 IF(ASSIMP_LIBRARY)
   SET(ASSIMP_FOUND "YES")
 ENDIF(ASSIMP_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(ASSIMP
                                  REQUIRED_VARS ASSIMP_LIBRARY ASSIMP_INCLUDE_DIR)
