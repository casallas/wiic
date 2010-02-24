INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../wiic)
ADD_LIBRARY(wiiusecpp SHARED wiiusecpp.cpp)
TARGET_LINK_LIBRARIES(wiiusecpp wiic)

INSTALL(TARGETS	wiiusecpp DESTINATION /usr/local/lib)
INSTALL(FILES wiiusecpp.h DESTINATION /usr/local/include)
