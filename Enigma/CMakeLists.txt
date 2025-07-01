cmake_minimum_required(VERSION 3.10)
project(EnigmaMachine)

# Настройка стандарта C++ (возьмите 17 или новее)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем, где искать заголовочные файлы
include_directories(inc)

# Собираем список исходных файлов
set(SOURCES
    src/PlugBoard.cpp
    src/Rotor.cpp
    src/Reflector.cpp
    src/EnigmaMachine.cpp
    src/utils.cpp
    main.cpp
)

# Создаем исполняемый файл
add_executable(enigma ${SOURCES})

# Для Windows: добавляем консольное окно
if(WIN32)
    target_link_libraries(enigma -mconsole)
endif()