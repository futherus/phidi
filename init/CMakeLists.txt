add_library( init SHARED )

target_sources( init
    PRIVATE
        init.cc
    PUBLIC
        init.hh
)

target_link_libraries( init sfml-graphics )
