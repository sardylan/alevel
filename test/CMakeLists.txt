find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(test-ncurses ncurses.c)
target_link_libraries(test-ncurses ${CURSES_LIBRARIES})
add_test(ncurses test-ncurses)
