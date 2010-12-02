INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../wiic)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../wiicpp)

ADD_EXECUTABLE(wiic-example example.c)
TARGET_LINK_LIBRARIES(wiic-example wiic)
INSTALL(TARGETS wiic-example DESTINATION /usr/local/bin)

ADD_EXECUTABLE(wiicpp-example example.cpp)
TARGET_LINK_LIBRARIES(wiicpp-example wiicpp)
INSTALL(TARGETS wiicpp-example DESTINATION /usr/local/bin)
