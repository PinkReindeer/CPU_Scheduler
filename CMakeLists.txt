cmake_minimum_required(VERSION 3.10)

project(Final)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} main.cpp
                                Application.h
                                Application.cpp
                                Graphics/SchedulerUI.h
                                Graphics/SchedulerUI.cpp
                                Logic/FCFS.h
                                Logic/FCFS.cpp
                                Logic/Preemptive.h
                                Logic/Preemptive.cpp
                                Logic/SchedulerCommon.h
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/fonts"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts"
)

# Dependencies
include(Dependencies.cmake)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    glfw
    OpenGL::GL
    imgui
)

target_include_directories(${PROJECT_NAME} PRIVATE ${icon_headers_SOURCE_DIR})

target_precompile_headers(${PROJECT_NAME} PUBLIC gpch.h)