# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


SET(CPACK_BINARY_7Z "")
SET(CPACK_BINARY_BUNDLE "")
SET(CPACK_BINARY_CYGWIN "")
SET(CPACK_BINARY_DEB "OFF")
SET(CPACK_BINARY_DRAGNDROP "")
SET(CPACK_BINARY_FREEBSD "OFF")
SET(CPACK_BINARY_IFW "OFF")
SET(CPACK_BINARY_NSIS "OFF")
SET(CPACK_BINARY_OSXX11 "")
SET(CPACK_BINARY_PACKAGEMAKER "")
SET(CPACK_BINARY_PRODUCTBUILD "")
SET(CPACK_BINARY_RPM "OFF")
SET(CPACK_BINARY_STGZ "ON")
SET(CPACK_BINARY_TBZ2 "OFF")
SET(CPACK_BINARY_TGZ "ON")
SET(CPACK_BINARY_TXZ "OFF")
SET(CPACK_BINARY_TZ "ON")
SET(CPACK_BINARY_WIX "")
SET(CPACK_BINARY_ZIP "")
SET(CPACK_BUILD_SOURCE_DIRS "/home/ntauthority/Desktop/CERN-HSF/yampl/build/libscy;/home/ntauthority/Desktop/CERN-HSF/yampl/build/LibSourcey-prefix/src/LibSourcey-build")
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
SET(CPACK_COMPONENTS_ALL "dev;libs")
SET(CPACK_COMPONENT_DEV_DESCRIPTION "Development files for LibSourcey")
SET(CPACK_COMPONENT_DEV_DISPLAY_NAME "Development files")
SET(CPACK_COMPONENT_LIBS_DESCRIPTION "LibSourcey")
SET(CPACK_COMPONENT_LIBS_DISPLAY_NAME "Libraries and data")
SET(CPACK_COMPONENT_SAMPLES_DESCRIPTION "Samples for LibSourcey")
SET(CPACK_COMPONENT_SAMPLES_DISPLAY_NAME "Samples")
SET(CPACK_COMPONENT_TESTS_DESCRIPTION "Accuracy and performance tests for LibSourcey")
SET(CPACK_COMPONENT_TESTS_DISPLAY_NAME "Tests")
SET(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
SET(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
SET(CPACK_DEBIAN_COMPONENT_DEV_NAME "libsourcey-dev")
SET(CPACK_DEBIAN_COMPONENT_LIBS_NAME "libsourcey")
SET(CPACK_DEBIAN_COMPONENT_SAMPLES_NAME "libsourcey-samples")
SET(CPACK_DEBIAN_COMPONENT_TESTS_NAME "libsourcey-tests")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://sourcey.com")
SET(CPACK_DEB_COMPONENT_INSTALL "TRUE")
SET(CPACK_GENERATOR "STGZ;TGZ;TZ")
SET(CPACK_INSTALL_CMAKE_PROJECTS "/home/ntauthority/Desktop/CERN-HSF/yampl/build/LibSourcey-prefix/src/LibSourcey-build;LibSourcey;ALL;/")
SET(CPACK_INSTALL_PREFIX "/usr/local")
SET(CPACK_MODULE_PATH "/home/ntauthority/Desktop/CERN-HSF/yampl/build/libscy/cmake")
SET(CPACK_NSIS_DISPLAY_NAME "LibSourcey 1.1.4")
SET(CPACK_NSIS_INSTALLER_ICON_CODE "")
SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
SET(CPACK_NSIS_PACKAGE_NAME "LibSourcey 1.1.4")
SET(CPACK_OUTPUT_CONFIG_FILE "/home/ntauthority/Desktop/CERN-HSF/yampl/build/LibSourcey-prefix/src/LibSourcey-build/CPackConfig.cmake")
SET(CPACK_PACKAGE_CONTACT "Kam Low <hello@sourcey.com>")
SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "/home/ntauthority/Desktop/CERN-HSF/yampl/build/libscy/README.md")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "High-performance Networking and Media Libraries")
SET(CPACK_PACKAGE_FILE_NAME "libsourcey-1.1.4-x86_64")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "LibSourcey 1.1.4")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "LibSourcey 1.1.4")
SET(CPACK_PACKAGE_NAME "LibSourcey")
SET(CPACK_PACKAGE_RELOCATABLE "true")
SET(CPACK_PACKAGE_VENDOR "Sourcey")
SET(CPACK_PACKAGE_VERSION "1.1.4")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "4")
SET(CPACK_RESOURCE_FILE_LICENSE "/home/ntauthority/Desktop/CERN-HSF/yampl/build/libscy/LICENSE.md")
SET(CPACK_RESOURCE_FILE_README "/usr/share/cmake-3.10/Templates/CPack.GenericDescription.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "/usr/share/cmake-3.10/Templates/CPack.GenericWelcome.txt")
SET(CPACK_RPM_COMPONENT_INSTALL "TRUE")
SET(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
SET(CPACK_RPM_PACKAGE_LICENSE "LGPL-2.1+")
SET(CPACK_RPM_PACKAGE_SUMMARY "High-performance Networking and Media Libraries")
SET(CPACK_RPM_PACKAGE_URL "https://sourcey.com")
SET(CPACK_SET_DESTDIR "TRUE")
SET(CPACK_SOURCE_7Z "")
SET(CPACK_SOURCE_CYGWIN "")
SET(CPACK_SOURCE_GENERATOR "TBZ2;TGZ;TXZ;TZ")
SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/home/ntauthority/Desktop/CERN-HSF/yampl/build/LibSourcey-prefix/src/LibSourcey-build/CPackSourceConfig.cmake")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "libsourcey-1.1.4-x86_64")
SET(CPACK_SOURCE_RPM "OFF")
SET(CPACK_SOURCE_TBZ2 "ON")
SET(CPACK_SOURCE_TGZ "ON")
SET(CPACK_SOURCE_TXZ "ON")
SET(CPACK_SOURCE_TZ "ON")
SET(CPACK_SOURCE_ZIP "OFF")
SET(CPACK_SYSTEM_NAME "Linux")
SET(CPACK_TOPLEVEL_TAG "Linux")
SET(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/home/ntauthority/Desktop/CERN-HSF/yampl/build/LibSourcey-prefix/src/LibSourcey-build/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()