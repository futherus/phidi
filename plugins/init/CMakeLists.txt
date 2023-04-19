add_library( init SHARED )

target_sources( init
    PRIVATE
        init.cc
    PUBLIC
        init.hh
)

target_include_directories( init PUBLIC ${PROJECT_SOURCE_DIR} )
target_link_libraries( init sfml-graphics )
