cmake_minimum_required(VERSION 3.10)
project(MarkovAlgorithm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройки для компилятора MSVC
if(MSVC)
    # Явно включаем отладочную информацию в Release режиме
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi /DEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF")
    
    # Указываем генерацию PDB файлов
    set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
    set(CMAKE_PDB_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
endif()

# Пути к UnitTest++ (из vcpkg)
set(UNITTEST_INCLUDE_DIR "C:/Users/anteiku/vcpkg/installed/x64-windows/include")
set(UNITTEST_LIBRARY "C:/Users/anteiku/vcpkg/installed/x64-windows/lib/UnitTest++.lib")

message(STATUS "UnitTest++ include dir: ${UNITTEST_INCLUDE_DIR}")
message(STATUS "UnitTest++ library: ${UNITTEST_LIBRARY}")

# Добавляем пути к заголовочным файлам
include_directories(
    ${CMAKE_SOURCE_DIR}/Markov
    ${UNITTEST_INCLUDE_DIR}
)

# Тестовое приложение
add_executable(test_markov_algorithm
    Markov/tests.cpp    
    Markov/MarkovAlg.cpp
    Markov/Rule.cpp
    Markov/Tape.cpp
    Markov/RuleParser.cpp
)

# Для MSVC явно указываем генерацию PDB
if(MSVC)
    set_target_properties(test_markov_algorithm PROPERTIES
        COMPILE_PDB_NAME "test_markov_algorithm"
        COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Release"
    )
endif()

target_link_libraries(test_markov_algorithm ${UNITTEST_LIBRARY})

# Основное приложение
add_executable(markov_algorithm_main
    main.cpp
    Markov/MarkovAlg.cpp
    Markov/Rule.cpp
    Markov/Tape.cpp
    Markov/RuleParser.cpp
)

# Настройки для Windows
if(WIN32)
    target_compile_definitions(test_markov_algorithm PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(test_markov_algorithm PRIVATE /W4 /wd4996)
    
    target_compile_definitions(markov_algorithm_main PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(markov_algorithm_main PRIVATE /W4 /wd4996)
endif()

# Добавляем тест в CTest (опционально)
enable_testing()
add_test(NAME MarkovTests COMMAND test_markov_algorithm)

# Coverage configuration for Windows
if(WIN32)
    set(OPENCPPCOVERAGE "C:/Program Files/OpenCppCoverage/OpenCppCoverage.exe")
    
    if(EXISTS ${OPENCPPCOVERAGE})
        message(STATUS "Found OpenCppCoverage: ${OPENCPPCOVERAGE}")
        
        file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/Markov" SOURCES_DIR)
        file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/Release" NATIVE_BINARY_DIR)
        
        add_custom_target(coverage
            COMMAND ${OPENCPPCOVERAGE} 
                --sources "${SOURCES_DIR}"
                --modules "${NATIVE_BINARY_DIR}\\test_markov_algorithm.exe"
                --export_type html:coverage_report
                --working_dir "${NATIVE_BINARY_DIR}"
                test_markov_algorithm.exe
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Release
            DEPENDS test_markov_algorithm
            COMMENT "Running code coverage with OpenCppCoverage"
        )
        
        add_custom_target(run_tests
            COMMAND test_markov_algorithm.exe
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Release
            DEPENDS test_markov_algorithm
            COMMENT "Running tests without coverage"
        )
        
    else()
        message(WARNING "OpenCppCoverage not found")
        
        add_custom_target(coverage
            COMMAND test_markov_algorithm.exe
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Release
            DEPENDS test_markov_algorithm
            COMMENT "Running tests (OpenCppCoverage not available)"
        )
    endif()
endif()

