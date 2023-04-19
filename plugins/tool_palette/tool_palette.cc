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
    tool_ids_.insert( std::pair<int, std::string>{ getPalette().getColumn().size(), std::move( tool_id)});

    getPalette().add( button);
}

} // namespace xui
