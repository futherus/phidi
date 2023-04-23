#include "plugins/tool_palette/tool_palette.hh"

namespace xui
{

const int ToolPalettePlugin::ID = 0;
static RegisterPlugin<xui::ToolPalettePlugin> X{ "ToolPalettePlugin"};

void
ToolPalette::update()
{
    std::string active = tool_manager_->getActive();
    for ( auto& p : tool_ids_ )
    {
        if ( p.second == active )
        {
            getPalette().update( p.first);
            return;
        }
    }
}

void
ToolPalette::add( std::string tool_id,
                  PushButton& button)
{
    // FIXME: button ordering is fixed
    $D( "before inserting index pair\n");
    tool_ids_.insert( std::pair<int, std::string>{ getPalette().getSize(), std::move( tool_id)});

    auto box = std::make_unique<Padding<SizedBox<PushButton&>>>( button);
    box->setPadding( 10);
    box->getChild().setSize( 160, 100);

    getPalette().add( *box, button);
    sized_boxes_.push_back( std::move( box));
}

} // namespace xui
