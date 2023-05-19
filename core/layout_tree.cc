#include <cxxabi.h>
#include "fmt/core.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
/* I hate it this bullshit... */
#include <regex>
#pragma GCC diagnostic pop

#include "../core/layout_tree.hh"

namespace xui {

void
Render( const LayoutObject& obj,
        sf::RenderTarget& target)
{
    obj.render_( obj.widget_, target, obj.geometry_);
    for ( auto& child : obj.children_ )
    {
        Render( child, target);
    }
}

Json
DumpInfo( const LayoutObject& obj,
          int tree_depth = 0)
{
    Json info = obj.dump_info_( obj.widget_);

    if ( tree_depth )
    {
        Json child_info{};
        for ( const auto& child : obj.children_ )
        {
            child_info.emplace_back( DumpInfo( child, tree_depth - 1));
        }

        info["children"] = std::move( child_info);
    }

    return info;
}

void
LayoutObject::adjust()
{
    for ( auto& child : children_ )
    {
        child.geometry_.translate( geometry_.tl());
        child.adjust();
    }
}

void
LayoutObject::drawOutline( sf::RenderTarget& target,
             const Geometry& geometry)
{
    sf::RectangleShape rectangle;

    rectangle.setSize( sf::Vector2f{ geometry.size().x, geometry.size().y});
    rectangle.setFillColor( sf::Color::Transparent);
    rectangle.setOutlineColor( sf::Color::Red);
    rectangle.setOutlineThickness( 1);
    rectangle.setPosition( geometry.tl().x, geometry.tl().y);

    target.draw( rectangle);
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

    widget_info = std::regex_replace( widget_info, std::regex{ R"(\n)"}, "\\n");
    widget_info = std::regex_replace( widget_info, std::regex{ R"(>)"},  "\\>");
    widget_info = std::regex_replace( widget_info, std::regex{ R"(<)"},  "\\<");

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
