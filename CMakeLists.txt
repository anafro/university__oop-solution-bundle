cmake_minimum_required(VERSION 3.21)

# Fetch content
#   The 'Fetch Content' library is used to load information from URLs.
#   This CMake uses it to load the Google Test Framework to this project.
include(FetchContent)


# Standard utilities
#   These utilities are used to display messages and so on.
include(cmake-scripts/utils/logging.cmake)
include(cmake-scripts/utils/booleans.cmake)


# Project name
#   This is a project name. Change it to your own.
set(PROJECT_NAME "oop")


# Logger prefix
#   You can change the default logger prefix if you wish.
set(LOGGER_PREFIX "${PROJECT_NAME}: Project building")


# Test mode
#   You can set the test mode manually, just uncomment the line and set the variable you want
#   WARNING: If this line in not commented, command line argument -DTest will be ignored.
set(Test yes)


# C++ version
#   Change C++ version here (e.g. if you want C++14, enter 14)
set(CPP_LANGUAGE_STANDARD 20)


# Project directory
#   Set it to your project files directory.
#   It is the same as the project name by default.
set(PROJECT_DIRECTORY "${PROJECT_NAME}")

# Project sources
#   Often, the IDE adds the sources automatically, but if it doesn't, please,
#   add all of your sources here, except the test.cpp and the main.cpp:
set(ProjectSources oop/bigint.h++ oop/utils/strings.h++ oop/utils/type_demangler.h++ oop/logger.h++ oop/csv.h++ oop/dictionary.h++ oop/stack.h++ oop/bigint_command_executor.c++ oop/bigint_command_executor.h++ oop/auth.c++ oop/auth.h++ oop/bigint.c++ oop/csv.c++ oop/logger.c++)


# --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


# Defining the C++ marcos of set variables:
add_compile_definitions(PROJECT_NAME=${PROJECT_NAME})
add_compile_definitions(LOGGER_PREFIX=${CPP_LANGUAGE_STANDARD})
add_compile_definitions(CPP_LANGUAGE_STANDARD=${CPP_LANGUAGE_STANDARD})
add_compile_definitions(PROJECT_DIRECTORY=${PROJECT_DIRECTORY})

# Setting the CMake project stuff:
set(CMAKE_CXX_STANDARD ${CPP_LANGUAGE_STANDARD})
include_directories(${PROJECT_DIRECTORY})

# Adding BCrypt to a project
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/BCrypt.cpp)

macro(build building_mode main_file)
    log("Building...")
    set(PROJECT_TITLE ${PROJECT_NAME}-${building_mode})
    set(BUILDING_MODE ${building_mode})
    set(MAIN_FILE ${main_file})
    project(${PROJECT_TITLE})
    add_executable(${PROJECT_NAME}-${building_mode} ${main_file} ${ProjectSources})
    set_target_properties(${PROJECT_NAME}-${building_mode} PROPERTIES LINKER_LANGUAGE CXX)
endmacro()

# Resolving, which mode should be used: Production or Test:
if(DEFINED Test)
    string(TOLOWER ${Test} Test)
    if(Test IN_LIST TruthyAnswers)
        log("Building project in Test mode, only testing modules will be built")
        log("Loading the Google Test Framework, it will take a while...")
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
        )
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        log("Unpacking the Google Test Framework sources...")
        FetchContent_MakeAvailable(googletest)

        set(building_mode test)
        set(main_file test.cpp)
        set(PROJECT_TITLE ${PROJECT_NAME}-${building_mode})
        project(${PROJECT_TITLE})
        add_executable(${PROJECT_TITLE} ${main_file} ${ProjectSources})

        enable_testing()
        target_link_libraries(${PROJECT_TITLE} gtest_main)
        include(GoogleTest)
        gtest_discover_tests(${PROJECT_TITLE})
    elseif(Test IN_LIST FalsyAnswers)
        log("Building project in Production mode, all the logic sources and headers will be built")
        set(building_mode production)
        set(main_file main.cpp)
        set(PROJECT_TITLE ${PROJECT_NAME}-${building_mode})
        project(${PROJECT_TITLE})
        add_executable(${PROJECT_TITLE} ${main_file} ${ProjectSources})
    else()
        string(REPLACE ";" ", or " TruthyAnswers_ToString "${TruthyAnswers}")
        string(REPLACE ";" ", or " FalsyAnswers_ToString "${FalsyAnswers}")
        error("What do you mean by '-DTest=${Test}'? If you want to build test, enter ${TruthyAnswers_ToString} instead of ${Test}, otherwise enter ${FalsyAnswers_ToString} to build your project in Production mode")
    endif()
else()
    error("Test mode is not specified. Please, add a -DTest=yes OR DTest=no or set it manually in 'CMakeLists.txt' to let me know what type of build you want to get.")
    set(Test no)
endif()

target_link_libraries(${PROJECT_TITLE} bcrypt)

set_target_properties(${PROJECT_TITLE} PROPERTIES LINKER_LANGUAGE CXX)

