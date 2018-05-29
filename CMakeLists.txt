cmake_minimum_required(VERSION 2.6)
project(msa-ui-linux C)
set(MSA_LOGIN_LINUX_PROJECT_NAME msa-ui-linux)

add_subdirectory(extension)

find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)
# pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GTKMM3 REQUIRED gtkmm-3.0)
pkg_check_modules(WEBKIT_GTK REQUIRED webkit2gtk-4.0)

add_executable(msa-ui-linux src/main.cpp src/WebLoginWindow.cpp src/WebLoginWindow.h src/WebView.cpp src/WebView.h src/ExtensionIPCServer.cpp src/ExtensionIPCServer.h src/LoginIPCService.cpp src/LoginIPCService.h src/UIThreadExecutor.cpp src/UIThreadExecutor.h src/PickAccountWindow.cpp src/PickAccountWindow.h src/ProfilePictureManager.cpp src/ProfilePictureManager.h src/MsaDaemonManager.cpp src/MsaDaemonManager.h)
target_link_libraries(msa-ui-linux simpleipc daemon_server_utils msa_daemon_client ${GTKMM3_LIBRARIES} ${WEBKIT_GTK_LIBRARIES} ${CURL_LIBRARIES})
target_include_directories(msa-ui-linux PRIVATE ${GTKMM3_INCLUDE_DIRS} ${WEBKIT_GTK_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})
add_dependencies(msa-ui-linux msa-ui-linux-extension)
set(${MSA_LOGIN_LINUX_PROJECT_NAME}_C_FLAGS ${${MSA_LOGIN_LINUX_PROJECT_NAME}_C_FLAGS} ${GTKMM3_CFLAGS_OTHER} ${WEBKIT_GTK_CFLAGS_OTHER})

add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/resources.c" PRE_BUILD
        COMMAND glib-compile-resources --target "${CMAKE_CURRENT_BINARY_DIR}/resources.c" --sourcedir "${CMAKE_CURRENT_SOURCE_DIR}/res/" --generate-source "${CMAKE_CURRENT_SOURCE_DIR}/res/resources.xml"
)
target_sources(msa-ui-linux PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/resources.c)