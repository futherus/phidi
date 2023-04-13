add_library( tools SHARED )

target_sources( tools
    PRIVATE
        tools.cc
    PUBLIC
        tools.hh
)

target_link_libraries( tools sfml-graphics )
