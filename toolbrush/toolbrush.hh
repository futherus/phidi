#ifndef TOOLBRUSH_HH
#define TOOLBRUSH_HH

#include "../tools/tools.hh"
#include "../tool_pallette/tool_pallette.hh"

namespace xui
{

class ToolBrush
    : public ITool
{
public:
    ToolBrush( std::string id)
        : ITool{ id}
        , width_{}
        , color_{}
        , is_pressed_{ false}
    {}

    void setWidth( float width) { width_ = width; }
    void setColor( sf::Color color) { color_ = color; }
    float getWidth() { return width_; }
    sf::Color getColor() { return color_; }

    void onMousePressed ( Canvas* canvas, sf::Vector2f pos) override;
    void onMouseReleased( Canvas* canvas, sf::Vector2f pos) override;
    void onMouseMoved   ( Canvas* canvas, sf::Vector2f pos) override;

    void deactivate() override;

private:
    float width_;
    sf::Color color_;

    bool is_pressed_;
};

class ToolBrushPlugin
    : public IPlugin
{
public:
    static const int ID;

    ToolBrushPlugin()
        : IPlugin{}
        , tool_{ new ToolBrush( "ToolBrushPlugin::tool")}
        , tool_button_{}
    {}

    ~ToolBrushPlugin() = default;

    void deserialize( const json& state) override
    {
        // tool_->setColor( state["color"]);
        tool_->setColor( sf::Color::Red);
        tool_->setWidth( state["width"]);

        $D( "before adding tool\n");
        auto tl_mngr_plg = PluginRegistry::instance()->getPlugin<ToolsPlugin>();
        auto tl_mngr = tl_mngr_plg->getToolManager();
        tl_mngr->addTool( tool_);

        $D( "before creating button\n");
        const sf::Texture* ph = gui::TextureFactory::getTexture("ph");
        const sf::Texture* Ph = gui::TextureFactory::getTexture("Ph");
        const sf::Texture* pH = gui::TextureFactory::getTexture("pH");
        const sf::Texture* PH = gui::TextureFactory::getTexture("PH");

        xui::PushButton::TexturePack pack{
            std::array<const sf::Texture*, 2>{ ph, Ph},
            std::array<const sf::Texture*, 2>{ pH, PH}
        };

        tool_button_ = new PushButton{
            Rectangle{ {110, 110}, {100, 100}},
            std::move( pack)
        };

        $D ( "before getting tool_pall plugin\n");
        auto tl_pal_plg = PluginRegistry::instance()->getPlugin<ToolPallettePlugin>();
        $D ( "before getting tool_pall\n");
        auto tl_pal = tl_pal_plg->getToolPallette();
        $D ( "before adding button\n");
        tl_pal->add( "ToolBrushPlugin::tool", tool_button_);
        $D ( "after adding button\n");
    }

    void serialize( json& state) override
    {
        // state["color"] = tool_->getColor();
        state["width"] = tool_->getWidth();
    }

private:
    ToolBrush* tool_;
    PushButton* tool_button_;
};

} // namespace xui

#endif // TOOLBRUSH_HH