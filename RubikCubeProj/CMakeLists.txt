cmake_minimum_required(VERSION 3.10)
project(RubikCubeTests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Для Windows - используем динамическую runtime библиотеку
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    # Включить отладочную информацию в Release
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi /DEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF")
endif()

# Пути к UnitTest++ (из vcpkg)
set(UNITTEST_INCLUDE_DIR "C:/Users/anteiku/vcpkg/installed/x64-windows/include")
set(UNITTEST_LIBRARY "C:/Users/anteiku/vcpkg/installed/x64-windows/lib/UnitTest++.lib")

message(STATUS "UnitTest++ include dir: ${UNITTEST_INCLUDE_DIR}")
message(STATUS "UnitTest++ library: ${UNITTEST_LIBRARY}")

# Добавляем пути к заголовочным файлам
include_directories(
    ${CMAKE_SOURCE_DIR}/RubikCube
    ${UNITTEST_INCLUDE_DIR}
)

# Тестовое приложение
add_executable(test_rubikcube 
    RubikCube/test_rubikcube.cpp 
    RubikCube/RubikCube.cpp
)

target_link_libraries(test_rubikcube ${UNITTEST_LIBRARY})

# Основное приложение
add_executable(rubik_cube_main 
    maincpp.cpp 
    RubikCube/RubikCube.cpp
)

# Настройки для Windows
if(WIN32)
    target_compile_definitions(test_rubikcube PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(test_rubikcube PRIVATE /W4 /wd4996)
    
    target_compile_definitions(rubik_cube_main PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(rubik_cube_main PRIVATE /W4 /wd4996)
endif()

# Coverage configuration for Windows
if(WIN32)
    set(OPENCPPCOVERAGE "C:/Program Files/OpenCppCoverage/OpenCppCoverage.exe")
    
    if(EXISTS ${OPENCPPCOVERAGE})
        message(STATUS "Found OpenCppCoverage: ${OPENCPPCOVERAGE}")
        
        # Преобразуем пути к Windows формату
        file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/RubikCube" SOURCES_DIR)
        file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/Release" NATIVE_BINARY_DIR)
        
        add_custom_target(coverage
            COMMAND ${OPENCPPCOVERAGE} 
                --sources "${SOURCES_DIR}"
                --modules "${NATIVE_BINARY_DIR}\\test_rubikcube.exe"
                --export_type html:coverage_report
                --working_dir "${NATIVE_BINARY_DIR}"
                test_rubikcube.exe
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Release
            DEPENDS test_rubikcube
            COMMENT "Running code coverage with OpenCppCoverage"
        )
        
        # Также добавим отдельную цель для тестов
        add_custom_target(run_tests
            COMMAND test_rubikcube.exe
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Release
            DEPENDS test_rubikcube
            COMMENT "Running tests without coverage"
        )
        
    else()
        message(WARNING "OpenCppCoverage not found at: ${OPENCPPCOVERAGE}")
        
        # Альтернатива - просто запуск тестов
        add_custom_target(coverage
            COMMAND test_rubikcube.exe
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Release
            DEPENDS test_rubikcube
            COMMENT "Running tests (OpenCppCoverage not available)"
        )
        
        add_custom_target(run_tests
            COMMAND test_rubikcube.exe
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Release
            DEPENDS test_rubikcube
            COMMENT "Running tests"
        )
    endif()
endif()