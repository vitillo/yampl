include(ExternalProject)

###############################################################################
# LibSourcey
###############################################################################
#ExternalProject_Add(
#        LibSourcey
#
#        GIT_REPOSITORY "https://github.com/sourcey/libsourcey.git"
#        GIT_TAG "1.1.4"
#        UPDATE_COMMAND ""
#        PATCH_COMMAND ""
#
#        SOURCE_DIR "libscy"
#        CMAKE_ARGS -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_APPLICATIONS=OFF -DBUILD_TESTS=OFF -DBUILD_SAMPLES=OFF -DBUILD_MODULES=OFF -DBUILD_MODULE_base=ON -DBUILD_MODULE_uv=ON -DBUILD_MODULE_pluga=ON -DWITH_OPENCV=OFF -DWITH_WEBRTC=OFF -DWITH_FFMPEG=OFF
#
#        TEST_COMMAND ""
#)

###############################################################################
# Pluga
###############################################################################
#ExternalProject_Add_Step(
#        LibSourcey AddPluga
#        COMMAND git clone https://github.com/sourcey/pluga.git pluga
#        COMMAND mv pluga libscy/src/
#        DEPENDEES download
#)
