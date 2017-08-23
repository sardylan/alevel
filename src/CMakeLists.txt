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

add_executable(alevel
        ${ALEVEL_SOURCES}
        ${ALEVEL_HEADERS})

target_link_libraries(alevel
        asound)
