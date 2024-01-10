cmake_minimum_required(VERSION 3.8.0)

project(WebsocketTransNode VERSION 3.0.0 LANGUAGES C CXX)

IF (WIN32)
	add_definitions(-D_WIN32_WINNT=0x0501)
	string(REPLACE "/MDd" "/MD" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
ENDIF()

add_definitions(-DNOMINMAX)
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

# if (CMAKE_BUILD_TYPE MATCHES "Debug")
# 	set(LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/../../Build/build/lib/Debug)
# elseif(CMAKE_BUILD_TYPE MATCHES "Release")
# 	set(LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/../../Build/build/lib/Release)
# endif()

set(LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/lib)

# set(SIMONE_SDK_PATH_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
# SET(OPENCV_HOME ${SIMONE_SDK_PATH_ROOT}/SensorRaw/3rdparty/opencv)
# SET(OPENCV_LIB_PATH ${OPENCV_HOME}/lib)	
# include_directories("${SIMONE_SDK_PATH_ROOT}/include")
# include_directories("${OPENCV_HOME}/include")

# SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
# SET(CMAKE_INSTALL_RPATH "./")

set(SAMPLES_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
set(INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
include_directories("${SAMPLES_DIR}/include")
include_directories("${INCLUDE_DIR}")

function (SetDefaultTargetProperties target)
	set_target_properties(${target} PROPERTIES CXX_STANDARD 14)
	set_target_properties(${target} PROPERTIES
		# ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
		# LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/../bin
		# ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
		# LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/../bin
		# ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib
		# LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib
		# RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin
	)
endfunction()

add_executable(TransNode "")
add_executable(MsgVerification "")


target_sources(TransNode PRIVATE "src/MsgProducer.cpp" "src/WebsocketServer.cpp" "src/Config.cpp")
target_sources(MsgVerification PRIVATE "src/MsgVerification.cpp" "src/Config.cpp")

SetDefaultTargetProperties(TransNode)
SetDefaultTargetProperties(MsgVerification)

IF (WIN32)
	# target_link_libraries(TransNode ${LIB_DIR}/SimOneAPI.lib)
	target_link_libraries(TransNode ${LIB_DIR}/boost/win/libboost_date_time-vc141-mt-x64-1_78.lib)
	# target_link_libraries(TransNode ${LIB_DIR}/boost/win/libboost_random-vc141-mt-x64-1_78.lib)
	target_link_libraries(TransNode ${LIB_DIR}/boost/win/libboost_regex-vc141-mt-x64-1_78.lib)
	target_link_libraries(TransNode ${LIB_DIR}/boost/win/libboost_system-vc141-mt-x64-1_78.lib)

	# target_link_libraries(MsgVerification ${LIB_DIR}/SimOneAPI.lib)
	target_link_libraries(MsgVerification ${LIB_DIR}/boost/win/libboost_random-vc141-mt-x64-1_78.lib)
	target_link_libraries(MsgVerification ${LIB_DIR}/boost/win/libboost_system-vc141-mt-x64-1_78.lib)
	target_link_libraries(MsgVerification ${LIB_DIR}/boost/win/libboost_date_time-vc141-mt-x64-1_78.lib)
	target_link_libraries(MsgVerification ${LIB_DIR}/boost/win/libboost_regex-vc141-mt-x64-1_78.lib)

	# target_link_libraries(TransNode ${SIMONE_SDK_PATH_ROOT}/lib/Win64/SimOneStreamingAPI.lib)
	# target_link_libraries(TransNode ${OPENCV_LIB_PATH}/opencv_world346.lib)
	
	# add_custom_command(TARGET TransNode 
	# 	POST_BUILD
	# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SIMONE_SDK_PATH_ROOT}/lib/Win64/avcodec-58.dll ${CMAKE_BINARY_DIR}/bin/avcodec-58.dll
	# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SIMONE_SDK_PATH_ROOT}/lib/Win64/avformat-58.dll ${CMAKE_BINARY_DIR}/bin/avformat-58.dll
	# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SIMONE_SDK_PATH_ROOT}/lib/Win64/avutil-56.dll ${CMAKE_BINARY_DIR}/bin/avutil-56.dll
	# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SIMONE_SDK_PATH_ROOT}/lib/Win64/swresample-3.dll ${CMAKE_BINARY_DIR}/bin/swresample-3.dll
	# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SIMONE_SDK_PATH_ROOT}/lib/Win64/swscale-5.dll ${CMAKE_BINARY_DIR}/bin/swscale-5.dll
	# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SIMONE_SDK_PATH_ROOT}/lib/Win64/SimOneStreamingAPI.dll ${CMAKE_BINARY_DIR}/bin/SimOneStreamingAPI.dll
	# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OPENCV_LIB_PATH}/opencv_world346.dll ${CMAKE_BINARY_DIR}/bin/opencv_world346.dll
    # )

ELSEIF (UNIX)
	target_link_libraries(TransNode -lpthread ${LIB_DIR}/libSimOneAPI.so)
	target_link_libraries(TransNode ${LIB_DIR}/boost/linux/libboost_date_time.a)
	# target_link_libraries(TransNode ${LIB_DIR}/boost/linux/libboost_random.a)
	target_link_libraries(TransNode ${LIB_DIR}/boost/linux/libboost_regex.a)
	target_link_libraries(TransNode ${LIB_DIR}/boost/linux/libboost_system.so)

	target_link_libraries(MsgVerification -lpthread ${LIB_DIR}/libSimOneAPI.so)
	target_link_libraries(MsgVerification ${LIB_DIR}/boost/linux/libboost_random.a)
	target_link_libraries(MsgVerification ${LIB_DIR}/boost/linux/libboost_system.a)
	target_link_libraries(MsgVerification ${LIB_DIR}/boost/linux/libboost_date_time.a)
	target_link_libraries(MsgVerification ${LIB_DIR}/boost/linux/libboost_regex.a)


ENDIF()
