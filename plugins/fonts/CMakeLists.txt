add_library( fonts SHARED )

target_sources( fonts
    PRIVATE
        fonts.cc
    PUBLIC
        fonts.hh
)

target_include_directories( fonts PUBLIC ${PROJECT_SOURCE_DIR} )
target_link_libraries( fonts sfml-graphics )
