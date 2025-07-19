cmake_minimum_required(VERSION 3.10)
project(GEM VERSION 1.0 LANGUAGES CXX)

set(DEV_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Используем pkg-config для поиска библиотек
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# Настройка путей
include_directories(
    ${DEV_PATH}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    # include/
)

# Список исходных файлов
set(SOURCES
    main.cpp
    src/sdl_graphic.cpp
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES})

# Линковка библиотек
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

# Установка прав для запуска от root
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    PERMISSIONS 
        OWNER_READ OWNER_WRITE OWNER_EXECUTE 
        GROUP_READ GROUP_EXECUTE 
        WORLD_READ WORLD_EXECUTE
)

# Копирование ресурсов в бинарную директорию
file(GLOB_RECURSE ASSETS "${CMAKE_SOURCE_DIR}/assets/*")
foreach(ASSET ${ASSETS})
    file(RELATIVE_PATH REL_ASSET ${CMAKE_SOURCE_DIR}/assets ${ASSET})
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${ASSET}"
        "${CMAKE_BINARY_DIR}/assets/${REL_ASSET}"
    )
endforeach()