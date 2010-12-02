INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)
SET(WIICPP_SOURCES wiicpp.cpp logging/training.cpp logging/dataset.cpp)
ADD_LIBRARY(wiicpp SHARED ${WIICPP_SOURCES})
TARGET_LINK_LIBRARIES(wiicpp wiic)

INSTALL(TARGETS	wiicpp DESTINATION /usr/local/lib)
INSTALL(FILES wiicpp.h DESTINATION /usr/local/include/wiicpp/)
INSTALL(FILES logging/training.h DESTINATION /usr/local/include/wiicpp/logging/)
INSTALL(FILES logging/dataset.h DESTINATION /usr/local/include/wiicpp/logging/)
