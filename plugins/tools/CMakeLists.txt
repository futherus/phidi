add_library( tools SHARED )

target_sources( tools
    PRIVATE
        tools.cc
    PUBLIC
        tools.hh
)

target_include_directories( tools PUBLIC ${PROJECT_SOURCE_DIR} )
