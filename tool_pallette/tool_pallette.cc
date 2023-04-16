#include "tool_pallette.hh"

namespace xui
{

const int ToolPallettePlugin::ID = 0;
static RegisterPlugin<xui::ToolPallettePlugin> X{ "ToolPallettePlugin"};

void
ToolPallette::update()
{
    std::string active = tool_manager_->getActive();

    for ( auto& p : tool_ids_ )
    {
        if ( p.second == active )
        {
            getPallette().update( p.first);
            return;
        }
    }
}

void
ToolPallette::add( std::string tool_id,
                   PushButton& button)
{
    // FIXME: button ordering is fixed
    $D( "before inserting index pair\n");
    tool_ids_.insert( std::pair<int, std::string>{ getPallette().getColumn().getWidgets().size(), std::move( tool_id)});

    getPallette().add( BoolControlRef{ button});
}

} // namespace xui
