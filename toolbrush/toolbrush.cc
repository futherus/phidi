#include "toolbrush.hh"

namespace xui
{

void
ToolBrush::onMousePressed( Canvas* canvas, sf::Vector2f pos)
{
    is_pressed_ = true;
    onMouseMoved( canvas, pos);
}

void
ToolBrush::onMouseReleased( Canvas*, sf::Vector2f)
{$FUNC
    is_pressed_ = false;
}

void
ToolBrush::onMouseMoved( Canvas* canvas, sf::Vector2f pos)
{$FUNC
    if ( is_pressed_ )
        canvas->drawCircle( pos, width_, color_);
}

void
ToolBrush::deactivate()
{$FUNC
    is_pressed_ = false;
}

const int ToolBrushPlugin::ID = 0;
static RegisterPlugin<xui::ToolBrushPlugin> X{ "ToolBrushPlugin"};

} // namespace xui
