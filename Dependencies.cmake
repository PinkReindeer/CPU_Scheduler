#
# Dependencies
#
include(FetchContent)

# GLFW
find_package(glfw3 3.4 QUIET)
if (NOT glfw3_FOUND)
    FetchContent_Declare(
            glfw3
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    )
    FetchContent_GetProperties(glfw3)
    if (NOT glfw3_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(glfw3)
        add_subdirectory(${glfw3_SOURCE_DIR} ${glfw3_BINARY_DIR})
    endif()
endif()

# OpenGL
find_package(OpenGL REQUIRED)

# GLAD
add_library(glad "${CMAKE_CURRENT_SOURCE_DIR}/glad.c")
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glad/include")

# GLM
find_package(glm 1.0.1 QUIET)
if (NOT glm_FOUND)
    FetchContent_Declare(
            glm
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
    )
    FetchContent_GetProperties(glm)
    if (NOT glm_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(glm)
        add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
    endif()
endif()
set_target_properties(glm PROPERTIES FOLDER "Dependencies")

# Dear ImGui
FetchContent_Declare(
    imgui
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.5.zip
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)

    add_library(imgui STATIC
        "${imgui_SOURCE_DIR}/imgui.cpp"
        "${imgui_SOURCE_DIR}/imgui_draw.cpp"
        "${imgui_SOURCE_DIR}/imgui_tables.cpp"
        "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
        "${imgui_SOURCE_DIR}/imgui_demo.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
    )

    target_include_directories(imgui PUBLIC 
        "${imgui_SOURCE_DIR}" 
        "${imgui_SOURCE_DIR}/backends"
    )

    target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
endif()

# nlohmann_json
FetchContent_Declare(
    json
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR})
endif()

# IconFontCppHeaders
FetchContent_Declare(
    icon_headers
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/juliettef/IconFontCppHeaders/archive/refs/heads/main.zip
)
FetchContent_GetProperties(icon_headers)
if(NOT icon_headers_POPULATED)
    FetchContent_Populate(icon_headers)
endif()