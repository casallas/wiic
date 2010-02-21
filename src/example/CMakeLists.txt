INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../wiic)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../wiiusecpp)

ADD_EXECUTABLE(wiic-example example.c)
TARGET_LINK_LIBRARIES(wiic-example wiic)
INSTALL(TARGETS wiic-example DESTINATION /usr/local/bin)

ADD_EXECUTABLE(wiiusecpp-example example.cpp)
TARGET_LINK_LIBRARIES(wiiusecpp-example wiiusecpp)
INSTALL(TARGETS wiiusecpp-example DESTINATION /usr/local/bin)
