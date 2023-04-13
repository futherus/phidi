add_library( toolbrush SHARED )

target_sources( toolbrush
    PRIVATE
        toolbrush.cc
    PUBLIC
        toolbrush.hh
)

target_link_libraries( toolbrush sfml-graphics )
