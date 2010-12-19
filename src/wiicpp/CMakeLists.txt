INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../wiic/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/logging/)

SET(WIICPP_SOURCES wiicpp.cpp logging/dataset.cpp logging/training.cpp)
ADD_LIBRARY(wiicpp SHARED ${WIICPP_SOURCES})
TARGET_LINK_LIBRARIES(wiicpp wiic)

INSTALL(TARGETS wiicpp DESTINATION /usr/local/lib)
INSTALL(FILES wiicpp.h DESTINATION /usr/local/include/wiic/)
INSTALL(FILES logging/training.h DESTINATION /usr/local/include/wiic/)
INSTALL(FILES logging/dataset.h DESTINATION /usr/local/include/wiic/)
INSTALL(FILES logging/sample.h DESTINATION /usr/local/include/wiic/)
INSTALL(FILES logging/accsample.h DESTINATION /usr/local/include/wiic/)
