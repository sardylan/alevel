set(ALEVEL_UTILITY_SOURCES
        utility/audio.c)

set(ALEVEL_UTILITY_HEADERS
        utility/audio.h)

set(ALEVEL_SOUNDCARD_SOURCES
        soundcard/soundcard.c)

set(ALEVEL_SOUNDCARD_HEADERS
        soundcard/soundcard.h)

set(ALEVEL_SOURCES
        alevel.c
        utils.c
        ui.c
        cfg.c)

set(ALEVEL_HEADERS
        version.h
        config.h
        alevel.h
        utils.h
        ui.h
        cfg.h)

find_package(ALSA REQUIRED)
include_directories(${ALSA_INCLUDE_DIRS})

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(alevel
        ${ALEVEL_UTILITY_SOURCES} ${ALEVEL_UTILITY_HEADERS}
        ${ALEVEL_SOUNDCARD_SOURCES} ${ALEVEL_SOUNDCARD_HEADERS}
        ${ALEVEL_SOURCES} ${ALEVEL_HEADERS})

target_link_libraries(alevel
        m pthread
        ${ALSA_LIBRARIES}
        ${CURSES_LIBRARIES})
