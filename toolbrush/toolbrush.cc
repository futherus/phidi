#include "toolbrush.hh"

namespace xui
{

void ToolBrush::onMousePressed(Canvas* canvas, sf::Vector2f pos)
{
    is_pressed_ = true;
    onMouseMoved(canvas, pos);
}

void ToolBrush::onMouseReleased(Canvas* canvas, sf::Vector2f pos)
{
    is_pressed_ = false;
}

void ToolBrush::onMouseMoved(Canvas* canvas, sf::Vector2f pos)
{
    if (is_pressed_)
        canvas->drawCircle(pos, width_, color_);
}

void ToolBrush::deactivate()
{
    is_pressed_ = false;
}

const int ToolBrushPlugin::ID = 0;
static RegisterPlugin<xui::ToolBrushPlugin> X{ "ToolBrushPlugin"};

} // namespace xui

#if 0
extern "C" xui::IPlugin* getPlugin()
{
    static xui::ToolBrushPlugin plugin;

    $D("Plugin id = %s\n", plugin.getId().data());
    return &plugin;
}
#endif