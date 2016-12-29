cmake_minimum_required(VERSION 3.3)

project(untitled3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(BOOST_ROOT CACHE PATH "boost_1_62_0")
FIND_PACKAGE(Boost COMPONENTS serialization system)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

add_executable(untitled3 main.cpp src/sockets/Socket.cpp src/sockets/Socket.h src/sockets/Udp.cpp src/sockets/Udp.h src/Point.cpp src/Point.h src/Node.cpp src/Node.h src/BFSPoint.cpp src/BFSPoint.h src/Grid.cpp src/Grid.h src/Bfs.cpp src/Bfs.h src/Cab.cpp src/Cab.h src/TripInfo.cpp src/TripInfo.h src/TaxiCentre.cpp src/TaxiCentre.h src/Driver.cpp src/Driver.h src/StandartCab.cpp src/StandartCab.h src/LuxuryCab.cpp src/LuxuryCab.h src/Passenger.cpp src/Passenger.h)

target_link_libraries(untitled3 ${Boost_LIBRARIES})








