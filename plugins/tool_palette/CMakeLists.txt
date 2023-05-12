add_library( tool_palette SHARED )

target_sources( tool_palette
    PRIVATE
        tool_palette.cc
    PUBLIC
        tool_palette.hh
)

target_include_directories( tool_palette PUBLIC ${PROJECT_SOURCE_DIR} )
