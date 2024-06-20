cmake_minimum_required(VERSION 3.27)
project(WhatsappStats)

set(CMAKE_CXX_STANDARD 17)

add_executable(WhatsappStats
        src/cpp/main.cpp
        src/h/main.h
        src/cpp/calculateTimeDifference.cpp
        src/h/calculateTimeDifference.h
        src/cpp/calculateTimeOfDay.cpp
        src/h/calculateTimeOfDay.h
        src/cpp/countWords.cpp
        src/h/countWords.h
        src/cpp/findLocationData.cpp
        src/h/findLocationData.h
        src/cpp/findPollData.cpp
        src/h/findPollData.h
        src/cpp/findSenderOfMessage.cpp
        src/h/findSenderOfMessage.h
        src/h/Poll.h
        src/h/globals.h
        src/cpp/globals.cpp
)