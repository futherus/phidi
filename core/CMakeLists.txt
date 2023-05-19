#
# This must be OBJECT library, because compiler can throw out
# unused functions from symbol table in STATIC libs...
#
add_library( core OBJECT )

target_sources( core
    PRIVATE
        layout_tree.cc
        dbg_info.cc
    PRIVATE
        lru_cache.hh
        debug.hh
        dynloader.hh
        geometry.hh
        plugin_registry.hh
        root.hh
        layout_tree.hh
        dbg_info.hh
)

target_include_directories( core PUBLIC ${PROJECT_SOURCE_DIR} )
