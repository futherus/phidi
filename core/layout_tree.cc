#include "../core/layout_tree.hh"
#include "fmt/core.h"
#include <regex>
#include <cxxabi.h>

namespace xui {

std::string
Demangle( const char* mangled)
{
    std::string demangled = mangled;
    char* name = abi::__cxa_demangle( mangled, nullptr, nullptr, nullptr);
    if ( name )
    {
        demangled = name;
    }

    free( name);
    return demangled;
}

static gv::Node
DumpRecursive( gv::Graph& graph,
               const LayoutObject& obj)
{
    Geometry geometry = obj.getGeometry();
    const std::string geometry_info =  fmt::format( "{{ x:{:03.2f} y:{:03.2f} w:{:03.2f} h:{:03.2f}}}",
                                                    geometry.left(), geometry.right(),
                                                    geometry.width(), geometry.height());

    const int kJsonIndent = 2;
    std::string widget_info = fmt::format( "{}", DumpInfo( obj).dump( kJsonIndent, ' ', true));
    widget_info = std::regex_replace( widget_info, std::regex(R"(\n)"), "\\l");
    widget_info = std::regex_replace( widget_info, std::regex(R"(<)"), "\\<");
    widget_info = std::regex_replace( widget_info, std::regex(R"(>)"), "\\>");

    std::fprintf( stderr, "Widget info: %s\n", widget_info.c_str());
    const std::string label = fmt::format( "{{ Layout Object | address: {} | {} | {} }}",
                                            static_cast<const void*>( std::addressof( obj)),
                                            geometry_info,
                                            widget_info);

    gv::Node self_node = graph.node( label, {{"label", label}});

    for ( auto& child : obj )
    {
        gv::Node child_node = DumpRecursive( graph, child);
        graph.edge( self_node, child_node);
    }

    return self_node;
}

void
LayoutObject::showGraph( FILE* file) const
{
    gv::Graph graph{ "lot-dump"};

    graph.attrs( gv::ObjectTag::NodeTag,
    {
        {"shape", "record"},
        {"fillcolor", "white"},
        {"color", "purple"},
        {"height", "0"},
        {"style", "rounded,filled"},
        {"fontname", "Courier"},
        {"label", "\\N"}
    });

    DumpRecursive( graph, *this);

    graph.dump( stderr);
    graph.render( file);
}


} // namespace xui
