#--------------------------------------------------------
# The CMakeLists.txt for:                       pCommunicationAngle
# Author(s):                              Kristen Railey
#--------------------------------------------------------

SET(SRC
  CommunicationAngle.cpp
  CommunicationAngle_Info.cpp
  AcousticPath.cpp
  main.cpp
)

ADD_EXECUTABLE(pCommunicationAngle ${SRC})

TARGET_LINK_LIBRARIES(pCommunicationAngle
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

