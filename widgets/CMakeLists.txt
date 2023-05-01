#
# This must be OBJECT library, because compiler can throw out
# unused functions from symbol table in STATIC libs...
#
add_library( widgets OBJECT )

target_sources( widgets
    PRIVATE
        button.cc
        palette.cc
        text_field.cc

    PRIVATE
        column.hh
        button.hh
        widget.hh
        palette.hh
)

target_include_directories( widgets PUBLIC ${PROJECT_SOURCE_DIR} )
