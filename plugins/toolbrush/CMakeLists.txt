add_library( toolbrush SHARED )

target_sources( toolbrush
    PRIVATE
        toolbrush.cc
    PUBLIC
        toolbrush.hh
)

target_include_directories( toolbrush PUBLIC ${PROJECT_SOURCE_DIR} )
