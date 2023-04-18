add_executable( phi )

find_package( SFML COMPONENTS graphics REQUIRED )

target_sources( phi
    PRIVATE
        button.cc
        pallette.cc
        resources.cc
        main.cc

    PUBLIC
        lru_cache.hh
        button.hh
        debug.hh
        dynloader.hh
        geometry.hh
        plugin_registry.hh
        resources.h
        widget.hh
        widget_manager.hh
        root.hh
        pallette.hh
        layout_tree.hh
)

target_link_libraries( phi sfml-graphics )
