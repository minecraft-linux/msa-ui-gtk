cmake_minimum_required(VERSION 2.6)
project(msa-ui-linux C)
set(MSA_LOGIN_LINUX_PROJECT_NAME msa-ui-linux)

find_package(PkgConfig REQUIRED)
# pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GTKMM3 REQUIRED gtkmm-3.0)
pkg_check_modules(WEBKIT_GTK REQUIRED webkit2gtk-4.0)

add_executable(msa-ui-linux src/main.cpp src/WebLoginWindow.cpp src/WebLoginWindow.h src/WebView.cpp src/WebView.h)
target_link_libraries(msa-ui-linux ${GTKMM3_LIBRARIES} ${WEBKIT_GTK_LIBRARIES})
target_include_directories(msa-ui-linux PRIVATE ${GTKMM3_INCLUDE_DIRS} ${WEBKIT_GTK_INCLUDE_DIRS})
set(${MSA_LOGIN_LINUX_PROJECT_NAME}_C_FLAGS ${${MSA_LOGIN_LINUX_PROJECT_NAME}_C_FLAGS} ${GTKMM3_CFLAGS_OTHER} ${WEBKIT_GTK_CFLAGS_OTHER})