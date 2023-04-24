#pragma once

#include "plugins/textures/textures.hh"
#include "plugins/tools/tools.hh"
#include "plugins/tool_palette/tool_palette.hh"
#include "plugins/property_palette/property_palette.hh"
#include "widgets/placeholder.hh"

namespace xui
{

class ToolBrush final
    : public ITool
{
public:
    ToolBrush()
        : ITool{}
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
        , tool1_{ std::make_unique<ToolBrush>()}
        , tool2_{ std::make_unique<ToolBrush>()}
        , tool_button1_{}
        , tool_button2_{}
        , placeholder1_{ sf::Color::Cyan}
    {}

    void
    deserialize( const json& state) override
    {
        // tool_->setColor( state["color"]);
        tool1_->setColor( sf::Color::Red);
        tool1_->setWidth( state["width"]);
        tool2_->setColor( sf::Color::Blue);
        tool2_->setWidth( state["width"]);

        $D( "before adding tool\n");
        auto tl_mngr_plg = PluginRegistry::instance()->getPlugin<ToolsPlugin>();
        auto tl_mngr = tl_mngr_plg->getToolManager();
        tl_mngr->addTool( "ToolBrushPlugin::tool1", tool1_.get());
        tl_mngr->addTool( "ToolBrushPlugin::tool2", tool2_.get());

        $D( "before creating button\n");

        auto textures_cache = PluginRegistry::instance()->getPlugin<TexturesPlugin>();
        const sf::Texture* ph = textures_cache->getTexture( "res/ph.jpg");
        const sf::Texture* Ph = textures_cache->getTexture( "res/Ph.jpg");
        const sf::Texture* pH = textures_cache->getTexture( "res/pH.jpg");
        const sf::Texture* PH = textures_cache->getTexture( "res/PH.jpg");

        $M( "Textures: %p, %p, %p, %p\n", ph, Ph, pH, PH);
        assert( ph && Ph && pH && PH);

        xui::PushButton::TexturePack pack{
            std::array<const sf::Texture*, 2>{ ph, Ph},
            std::array<const sf::Texture*, 2>{ pH, PH}
        };

        tool_button1_ = std::make_unique<PushButton>( pack); //, sf::Vector2f{ 100, 100});
        tool_button2_ = std::make_unique<PushButton>( pack); //, sf::Vector2f{ 150, 150});

        $D ( "before getting tool_pall plugin\n");
        auto tl_pal_plg = PluginRegistry::instance()->getPlugin<ToolPalettePlugin>();
        $D ( "before getting tool_pall\n");
        auto tl_pal = tl_pal_plg->getToolPalette();
        $D ( "before adding button\n");
        tl_pal->add( "ToolBrushPlugin::tool1", *tool_button1_);
        tl_pal->add( "ToolBrushPlugin::tool2", *tool_button2_);
        $D ( "after adding button\n");

        auto init_plg = PluginRegistry::instance()->getPlugin<InitPlugin>();
        init_plg->add( placeholder1_, 1, 0.5f);

        // PluginRegistry::instance()->getPlugin<ToolsPlugin>()->getToolManager()->setActive( "ToolBrushPlugin::tool1");
        // prop_button1_ = std::make_unique<PushButton>( pack);
        // prop_button1_->bind( [=]( bool val)
        //                      {
        //                          tool1_->setColor( val ? sf::Color::Green : sf::Color::Magenta);
        //                          prop_button1_->update( val);
        //                      });

        // auto prop_pal_plg = PluginRegistry::instance()->getPlugin<PropertyPalettePlugin>();
        // prop_pal_plg->getPalette()->add( "ToolBrushPlugin::tool1", *prop_button1_, 1.0);
    }

    void
    serialize( json& state) override
    {
        // state["color"] = tool_->getColor();
        // state["width"] = tool_->getWidth();
    }

private:
    std::unique_ptr<ToolBrush> tool1_;
    std::unique_ptr<ToolBrush> tool2_;
    std::unique_ptr<PushButton> tool_button1_;
    std::unique_ptr<PushButton> tool_button2_;
    std::unique_ptr<PushButton> prop_button1_;

    Placeholder placeholder1_;
};

} // namespace xui
