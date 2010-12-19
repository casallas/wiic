INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../wiic/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../wiicpp/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../wiicpp/logging/)

ADD_EXECUTABLE(wiic-example example.c)
TARGET_LINK_LIBRARIES(wiic-example wiic)
INSTALL(TARGETS wiic-example DESTINATION /usr/local/bin)

ADD_EXECUTABLE(wiicpp-example example.cpp)
TARGET_LINK_LIBRARIES(wiicpp-example wiicpp)
INSTALL(TARGETS wiicpp-example DESTINATION /usr/local/bin)

ADD_EXECUTABLE(wiic-logger logger.cpp)
TARGET_LINK_LIBRARIES(wiic-logger wiicpp)
INSTALL(TARGETS wiic-logger DESTINATION /usr/local/bin)
