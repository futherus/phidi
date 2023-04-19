add_library( tool_pallette SHARED )

target_sources( tool_pallette
    PRIVATE
        tool_pallette.cc
    PUBLIC
        tool_pallette.hh
)

target_include_directories( tool_pallette PUBLIC ${PROJECT_SOURCE_DIR} )
target_link_libraries( tool_pallette sfml-graphics )
