cmake_minimum_required(VERSION 3.10)

set(TEST_SOURCES
    ${DEV_PATH}/src/GReport.cpp
    main.cpp
)

add_executable(GReport ${TEST_SOURCES})

target_link_libraries(GReport
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_GFX_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

# add_test(NAME GReport COMMAND GReport)

# Копирование ассетов
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets)
file(GLOB_RECURSE ASSETS "${CMAKE_SOURCE_DIR}/assets/*")
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR}/assets)

# set_target_properties(GReport PROPERTIES
#                         RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})