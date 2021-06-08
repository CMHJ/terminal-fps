cmake_minimum_required(VERSION 3.0.0)
project(terminal-fps VERSION 0.1.0)
set(CMAKE_C_STANDARD 99)

# include(CTest)
# enable_testing()

add_executable(${CMAKE_PROJECT_NAME}
    main.c
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    m
    ncursesw
)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
