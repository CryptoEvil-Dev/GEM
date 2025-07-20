cmake_minimum_required(VERSION 3.10)
project(GEM VERSION 1.0 LANGUAGES CXX)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEV_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Используем pkg-config для поиска библиотек
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_GFX REQUIRED SDL2_gfx)  # Ищем SDL2_gfx через pkg-config
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)


# Настройка путей
include_directories(
    ${DEV_PATH}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_GFX_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    include/
)

# Список исходных файлов
set(SOURCES
    src/sdl_graphic.cpp
    main.cpp
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES})

# Линковка библиотек
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_GFX_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)

# Установка прав для запуска от root
if(UNIX)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        PERMISSIONS 
            OWNER_READ OWNER_WRITE OWNER_EXECUTE 
            GROUP_READ GROUP_EXECUTE 
            WORLD_READ WORLD_EXECUTE
    )
endif()

# Копирование ассетов
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets)
file(GLOB_RECURSE ASSETS "${CMAKE_SOURCE_DIR}/assets/*")
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR}/assets)