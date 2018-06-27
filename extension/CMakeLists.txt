cmake_minimum_required(VERSION 2.6)
project(msa-ui-gtk-extension C)
set(MSA_UI_EXTENSION_PROJECT_NAME msa-ui-gtk-extension)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM3 REQUIRED gtkmm-3.0)
pkg_check_modules(WEBKIT_GTK REQUIRED webkit2gtk-4.0)

add_library(msa-ui-gtk-extension SHARED main.cpp ExternalObject.cpp ExternalObject.h JSUtils.cpp JSUtils.h IPCClient.cpp IPCClient.h)
target_link_libraries(msa-ui-gtk-extension ${GTKMM3_LIBRARIES} ${WEBKIT_GTK_LIBRARIES})
target_include_directories(msa-ui-gtk-extension PRIVATE ${GTKMM3_INCLUDE_DIRS} ${WEBKIT_GTK_INCLUDE_DIRS})
set(${MSA_UI_EXTENSION_PROJECT_NAME}_C_FLAGS ${${MSA_UI_EXTENSION_PROJECT_NAME}_C_FLAGS} ${GTKMM3_CFLAGS_OTHER} ${WEBKIT_GTK_CFLAGS})