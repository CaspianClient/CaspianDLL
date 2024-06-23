include(FetchContent)

cmake_minimum_required(VERSION 3.24.0)
project(Caspian VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options(/MT /MP)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()


file(GLOB_RECURSE sources "src/**/*.cpp" "src/**/*.hpp" "lib/**/*.cpp")

add_library(Caspian SHARED "main.cpp" ${sources})

if("${MSVC_RUNTIME}" STREQUAL "")
    set(MSVC_RUNTIME "static")

    if(${MSVC_RUNTIME} STREQUAL "static")
        message(STATUS
                "MSVC -> forcing use of statically-linked runtime."
        )
        foreach(variable ${variables})
            if(${variable} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
            endif()
        endforeach()
    else()
        message(STATUS
                "MSVC -> forcing use of dynamically-linked runtime."
        )
        foreach(variable ${variables})
            if(${variable} MATCHES "/MT")
                string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
            endif()
        endforeach()
    endif()
endif()

target_include_directories(Caspian PRIVATE "${CMAKE_CURRENT_LIST_DIR}/lib/include/")

# download(packagename github repo git tag)
function(download NAME REPO TAG)
    FetchContent_Declare(
        ${NAME}
        GIT_REPOSITORY ${REPO}
        GIT_TAG ${TAG} 
    )
    FetchContent_MakeAvailable(${NAME})
endfunction()

download(minhook https://github.com/TsudaKageyu/minhook.git f5485b8454544c2f034c78f8f127c1d03dea3636)
download(entt https://github.com/skypjack/entt.git fedcb920ce0068c35ffbc66fd4e84864e6ef71ef)
download(libhat https://github.com/BasedInc/libhat.git 82de2e6f19d3a5c0123d32ef6a5059034312df7b)
download(glm https://github.com/g-truc/glm.git 673a963a0f1eb82f5fcef00b7b873371555e5814)
download(nes https://github.com/DisabledMallis/NuvolaEventSystem c246a10bb5d3c952d62fcc175e5f6da0f81843bf)

# Disable RTTI
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(Caspian PRIVATE -fno-rtti)
elseif (MSVC)
    target_compile_options(Caspian PRIVATE /GR-)
endif()

target_link_libraries(Caspian PRIVATE windowscodecs.lib urlmon.lib dwrite.lib d3d12.lib dxgi.lib d3d11.lib d2d1.lib wininet.lib minhook EnTT libhat NES version)