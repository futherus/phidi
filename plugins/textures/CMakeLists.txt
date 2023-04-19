add_library( textures SHARED )

target_sources( textures
    PRIVATE
        textures.cc
    PUBLIC
        textures.hh
)

target_include_directories( textures PUBLIC ${PROJECT_SOURCE_DIR} )
target_link_libraries( textures sfml-graphics )
