#pragma once

#include "../tools/tools.hh"
#include "../tool_pallette/tool_pallette.hh"

namespace xui
{

class ToolBrush final
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

class ToolBrushPlugin final
    : public IPlugin
{
public:
    static const int ID;

    ToolBrushPlugin()
        : IPlugin{}
        , tool1_{ std::make_unique<ToolBrush>( "ToolBrushPlugin::tool1")}
        , tool2_{ std::make_unique<ToolBrush>( "ToolBrushPlugin::tool2")}
        , tool_button1_{}
        , tool_button2_{}
    {}

    void deserialize( const json& state) override
    {
        // tool_->setColor( state["color"]);
        tool1_->setColor( sf::Color::Red);
        tool1_->setWidth( state["width"]);
        tool2_->setColor( sf::Color::Red);
        tool2_->setWidth( state["width"]);

        $D( "before adding tool\n");
        auto tl_mngr_plg = PluginRegistry::instance()->getPlugin<ToolsPlugin>();
        auto tl_mngr = tl_mngr_plg->getToolManager();
        tl_mngr->addTool( tool1_.get());
        tl_mngr->addTool( tool2_.get());

        $D( "before creating button\n");
        const sf::Texture* ph = gui::TextureFactory::getTexture("ph");
        const sf::Texture* Ph = gui::TextureFactory::getTexture("Ph");
        const sf::Texture* pH = gui::TextureFactory::getTexture("pH");
        const sf::Texture* PH = gui::TextureFactory::getTexture("PH");
        $M( "Textures: %p, %p, %p, %p\n", ph, Ph, pH, PH);
        assert( ph && Ph && pH && PH);

        xui::PushButton::TexturePack pack{
            std::array<const sf::Texture*, 2>{ ph, Ph},
            std::array<const sf::Texture*, 2>{ pH, PH}
        };

        tool_button1_ = std::make_unique<PushButton>( pack, sf::Vector2f{ 100, 100});
        tool_button2_ = std::make_unique<PushButton>( std::move( pack), sf::Vector2f{ 100, 100});

        $D ( "before getting tool_pall plugin\n");
        auto tl_pal_plg = PluginRegistry::instance()->getPlugin<ToolPallettePlugin>();
        $D ( "before getting tool_pall\n");
        auto tl_pal = tl_pal_plg->getToolPallette();
        $D ( "before adding button\n");
        tl_pal->add( "ToolBrushPlugin::tool1", *tool_button1_);
        tl_pal->add( "ToolBrushPlugin::tool2", *tool_button1_);
        $D ( "after adding button\n");
    }

    void serialize( json& state) override
    {
        // state["color"] = tool_->getColor();
        // state["width"] = tool_->getWidth();
    }

private:
    std::unique_ptr<ToolBrush> tool1_;
    std::unique_ptr<ToolBrush> tool2_;
    std::unique_ptr<PushButton> tool_button1_;
    std::unique_ptr<PushButton> tool_button2_;
};

} // namespace xui
