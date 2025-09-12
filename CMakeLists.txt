cmake_minimum_required(VERSION 3.14)
project(scs_serde VERSION 1.0.0 LANGUAGES CXX)

# Set policies to avoid warnings
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)  # Handle download timestamp issues
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project options
option(SCS_BUILD_TESTS "Build tests for SCS" ON)
option(SCS_BUILD_EXAMPLES "Build example programs" ON)
option(SCS_USE_SYSTEM_GTEST "Use system-installed GTest" OFF)

# Main library target (header-only)
add_library(scs_serde INTERFACE)
target_include_directories(scs_serde INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Main executable (if exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc")
    add_executable(scs_main src/main.cc)
    target_link_libraries(scs_main PRIVATE scs_serde)
endif()

# Test configuration
if(SCS_BUILD_TESTS)
    # GTest configuration
    if(SCS_USE_SYSTEM_GTEST)
        find_package(GTest REQUIRED)
        message(STATUS "Using system-installed GTest")
    else()
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/release-1.12.1.zip
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        )
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
        message(STATUS "Using bundled GTest")
    endif()

    enable_testing()

    # Test executable
    add_executable(scs_tests
        tests/test_serialization.cc
        tests/test_deserialization.cc
        tests/test_structures.cc
    )

    target_link_libraries(scs_tests PRIVATE
        scs_serde
        GTest::gtest_main
    )

    # Add test cases
    add_test(NAME SerializationTests COMMAND scs_tests --gtest_filter=SerializationTest*)
    add_test(NAME DeserializationTests COMMAND scs_tests --gtest_filter=DeserializationTest*)
    add_test(NAME StructureTests COMMAND scs_tests --gtest_filter=StructureTest*)
endif()

# Example programs configuration
if(SCS_BUILD_EXAMPLES AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    add_subdirectory(examples)
endif()

# Installation configuration
install(DIRECTORY include/scs DESTINATION include)
install(TARGETS scs_serde EXPORT scs_serde-targets)

# Export targets
install(EXPORT scs_serde-targets
    FILE scs_serdeTargets.cmake
    NAMESPACE scs::
    DESTINATION lib/cmake/scs_serde
)

# Package configuration
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/scs_serdeConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/scs_serdeConfig.cmake
    INSTALL_DESTINATION lib/cmake/scs_serde
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/scs_serdeConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/scs_serdeConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/scs_serdeConfigVersion.cmake
    DESTINATION lib/cmake/scs_serde
)