#include "plugins/tool_palette/tool_palette.hh"

namespace xui
{

const int ToolPalettePlugin::ID = 0;
static RegisterPlugin<xui::ToolPalettePlugin> X{ "ToolPalettePlugin"};

void
ToolPalette::update()
{
    std::string active = tool_manager_->getActive();

    for ( const auto& id_pair : tool_ids_ )
    {
        if ( id_pair.second == active )
        {
            getPalette().update( id_pair.first);
            return;
        }
    }

    assert( 0 && "No entry in ToolPalette for such tool");
}

void
ToolPalette::add( std::string tool_id,
                  PushButton& button)
{
    // FIXME: button ordering is fixed
    $D( "before inserting index pair\n");

    auto box = std::make_unique<Padding<PushButton&>>( button);
    box->setPadding( 10);

    int index = getPalette().add( *box, 1.0f, button);
    tool_ids_.insert( {index, std::move( tool_id)});

    sized_boxes_.push_back( std::move( box));
}

} // namespace xui
