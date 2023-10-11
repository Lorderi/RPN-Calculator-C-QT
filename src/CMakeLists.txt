cmake_minimum_required(VERSION 3.5)

project(calc_prod VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED PrintSupport)

set(PROJECT_SOURCES
        main.cpp mainwindow.cpp
        mainwindow.h mainwindow.ui
        dialog.h dialog.cpp dialog.ui
        source/polish.c source/polish.h
        source/qcustomplot.cpp qcustomplot.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(calc_prod
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET calc_prod APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(calc_prod SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(calc_prod
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(calc_prod PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Qt5::Gui
    Qt${QT_VERSION_MAJOR}::PrintSupport)

set_target_properties(calc_prod PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(calc_prod)
endif()


set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -D_GNU_SOURCE") 
set(CMAKE_LIB_FLAGS "-lm -lpthread -lrt -lsubunit")

set(CLANG-FORMAT-PATH ../materials/linters/.clang-format)
set(SRC_DIR .)
set(OBJ_DIR .)

file(GLOB APP_SOURCES_CPP "${SRC_DIR}/*.cpp")
file(GLOB APP_SOURCES "${SRC_DIR}/source/*.c")
file(GLOB TEST_FILE main_test.c)

add_library(calc STATIC ${APP_SOURCES})

add_executable(test ${TEST_FILE})
target_link_libraries(test calc check m pthread rt subunit)

target_include_directories(calc PUBLIC ${SRC_DIR})

add_custom_target(format
    COMMAND find ${SRC_DIR} -maxdepth 1 -iname '*.cpp' -o -iname '*.c' -o -iname '*.h' | xargs clang-format -i -style=Google
    COMMAND find ${SRC_DIR}/source -maxdepth 1 -iname '*.cpp' -o -iname '*.c' -o -iname '*.h' | xargs clang-format -i -style=Google
    COMMENT "Formatting code"
)

add_custom_target(test_format
    COMMAND find ${SRC_DIR} -maxdepth 1 -iname '*.cpp' -o -iname '*.c' -o -iname '*.h' | xargs clang-format -n -style=Google
    COMMAND find ${SRC_DIR}/source -maxdepth 1 -iname '*.cpp' -o -iname '*.c' -o -iname '*.h' | xargs clang-format -n -style=Google
    COMMENT "Checking code formatting"
)

add_custom_target(run_tests
    COMMAND ./test
    DEPENDS test
    COMMENT "Running tests"
)

add_custom_target(leaks
    COMMAND CK_FORK=no valgrind --leak-check=full ./test
    DEPENDS test
    COMMENT "Checking for memory leaks"
)

add_custom_target(cppcheck
    COMMAND cppcheck --enable=all --suppress=missingIncludeSystem ${APP_SOURCES} ${TEST_FILE} ${APP_SOURCES_CPP}
    COMMENT "Running cppcheck"
)

add_custom_target(gcov_report
    COMMAND find ${SRC_DIR} -name '*.gcda' -exec rm {} +;
    COMMAND ./test_cov
    COMMAND lcov -t '' -o calc.info -c -d .
    COMMAND genhtml -o report calc.info
    DEPENDS gcov_build
    COMMENT "Generating code coverage report"
)

add_custom_target(gcov_build
    COMMAND gcc -std=c11 -Wall -Wextra -Werror --coverage main_test.c source/polish.c -lcheck -lm -lpthread -lrt -lsubunit -D_GNU_SOURCE -o test_cov
    COMMENT "Building for code coverage"
)
add_custom_target(dvi
    COMMAND texi2dvi polish.texi
    COMMENT "Building .dvi"
)

install(TARGETS calc_prod
    RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/build"
)

add_custom_target(dist
    COMMENT "Archive project"
    COMMAND tar cfvz C7_SmartCalc_v1.0_src.tar.gz ${SRC_DIR}
)

add_custom_target(install_target
    DEPENDS calc_prod uninstall_target
    COMMENT "Installing the application to /build"
    COMMAND ${CMAKE_COMMAND} --build . --target install
)

add_custom_target(uninstall
    COMMENT "Uninstalling the application from /build"
    COMMAND ${CMAKE_COMMAND} -E remove_directory build calc_prod_autogen calc_autogen test_autogen
    COMMAND rm -f calc_prod install_manifest.txt libcalc.a test
)

add_dependencies(install_target uninstall)

add_custom_target(clean-all
    COMMAND rm -f calc.info test test_coverage CMakeCache.txt libcalc.a test_cov install_manifest.txt calc_prod C7_SmartCalc_v1.0_src.tar.gz
    COMMAND rm -rf report CMakeFiles test_autogen calc_autogen ../build-src-Desktop-Debug ../build/* calc_prod_autogen build
    COMMAND find ${SRC_DIR} -name '*.o' -exec rm {} +;
    COMMAND find ${SRC_DIR} -name '*.gcov' -exec rm {} +;
    COMMAND find ${SRC_DIR} -name '*.gcda' -exec rm {} +;
    COMMAND find ${SRC_DIR} -name '*.gcno' -exec rm {} +;
    COMMAND find ${SRC_DIR} -name '*.info' -exec rm {} +;
    COMMAND find ${SRC_DIR} -name '*.cmake' -exec rm {} +;
    COMMAND find ${SRC_DIR} -name '*.toc' -exec rm {} +;
    COMMAND find ${SRC_DIR} -name '*.log' -exec rm {} +;
    COMMAND find ${SRC_DIR} -name '*.dvi' -exec rm {} +;
    COMMAND find ${SRC_DIR} -name '*.cps' -exec rm {} +;
    COMMAND find ${SRC_DIR} -name '*.cp' -exec rm {} +;
    COMMAND find ${SRC_DIR} -name '*.aux' -exec rm {} +;
    COMMENT "Cleaning project"
)
