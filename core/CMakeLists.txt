#
# This must be OBJECT library, because compiler can throw out
# unused functions from symbol table in STATIC libs...
#
add_library( core OBJECT )

target_sources( core
    PRIVATE
        resources.cc

    PRIVATE
        lru_cache.hh
        debug.hh
        dynloader.hh
        geometry.hh
        plugin_registry.hh
        resources.h
        root.hh
        layout_tree.hh
)

target_include_directories( core PUBLIC ${PROJECT_SOURCE_DIR} )
