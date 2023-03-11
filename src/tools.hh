#ifndef TOOLS_H
#define TOOLS_H

#include <assert.h>

#include <SFML/Graphics.hpp>
#include "bind.hh"
#include "pallette.hh"
#include "application.hh"
#include "resources.h"

namespace xui
{

class IView
{
public:
    virtual void update() = 0;
};

/////////////////// Tools Plugin ///////////////////

class Canvas;

class ITool
{
private:
    std::string id_;

public:
    ITool( std::string id)
        : id_{ std::move( id)}
    {}

    const std::string& getId() { return id_; }

    virtual void onMousePressed ( Canvas* canvas, sf::Vector2f pos) = 0;
    virtual void onMouseReleased( Canvas* canvas, sf::Vector2f pos) = 0;
    virtual void onMouseMoved   ( Canvas* canvas, sf::Vector2f pos) = 0;

    virtual void activate() {}
    virtual void deactivate() {}
};

class ToolManager
{
private:
    std::vector<ITool*> tools_;
    std::string active_tool_;

    std::vector<IView*> views_;

    void updateViews();

public:
    ToolManager() = default;

    void setActive( const std::string& id);
    const std::string& getActive() const;

    void addTool( ITool* tool);
    ITool* getActiveTool() const;
};

class Canvas
    : public IWidget
{
private:
    ToolManager* tool_manager_;

    sf::RenderTexture pixels_;
    sf::Sprite sprite_;
    sf::Color base_color_;

    bool is_pressed_;
    bool is_hovered_;
    bool is_active_;

public:
    Canvas()
        : IWidget{ Rectangle{ { 300, 300}, { 300, 300}}}
        , tool_manager_{}
        , pixels_{}
        , sprite_{}
        , base_color_{}
        , is_pressed_{}
        , is_hovered_{}
        , is_active_{}
    {}

    void init()
    {
        pixels_.create((uint32_t) bounds().width(), (uint32_t) bounds().height());
        sprite_.setTexture(pixels_.getTexture());
        sprite_.setPosition(bounds().tl());
        clear();
    }

    void setToolManager( ToolManager* tool_manager) { tool_manager_ = tool_manager; }
    void setBaseColor( sf::Color color) { base_color_ = color; }
    sf::Color getBaseColor() { return base_color_; }

    void clear();
    void drawCircle( sf::Vector2f pos, float radius, sf::Color color);
    void drawLine( sf::Vector2f pos1, sf::Vector2f pos2, float width, sf::Color color);

    void draw( sf::RenderTarget& target) const override;

    void onMousePressed ( const sf::Event& event) override;
    void onMouseReleased( const sf::Event& event) override;
    void onMouseMoved   ( const sf::Event& event) override;
};

class ToolsPlugin final
    : public IPlugin
{
private:
    ToolManager* tool_manager_;
    Canvas* canvas_;

public:
    ToolsPlugin()
        : IPlugin( "ToolsPlugin")
        , tool_manager_{ new ToolManager}
        , canvas_{ new Canvas}
    {}

    ~ToolsPlugin() = default;

    void deserialize( Application* app, const json& state) override
    {
        tool_manager_->setActive( state["active_tool"]);

        canvas_->setToolManager( tool_manager_);
        canvas_->setBaseColor( sf::Color::White);
        canvas_->init();
        // canvas_->setBaseColor( state["basecolor"]);
    }

    void serialize( json& state) override
    {
        state["active_tool"] = tool_manager_->getActive();

        // state["basecolor"] = canvas_->getBaseColor();
    }

    ToolManager* getToolManager() { return tool_manager_; }
    Canvas* getCanvas() { return canvas_; }
};

/////////////////// ToolPallette Plugin ///////////////////

class ToolPallette
    : public IView
{
private:
    ToolManager* tool_manager_;
    PushPallette pallette_;

    std::map<int, std::string> tool_ids_;

public:
    ToolPallette()
        : IView{}
        , tool_manager_{}
        , pallette_{ Rectangle{ { 100, 100}, { 300, 300}}}
    {}

    void setToolManager( ToolManager* tool_manager)
    {
        tool_manager_ = tool_manager;

        pallette_.bind(
            [&]( int indx){ tool_manager_->setActive( tool_ids_.find( indx)->second); }
        );
    }

    void update() override;

    void add( std::string tool_id, PushButton* button);

    PushPallette* getPallette() { return &pallette_; }
};

class ToolPallettePlugin final
    : public IPlugin
{
private:
    ToolPallette* tool_pallette_;

public:
    ToolPallettePlugin()
        : IPlugin{ "ToolPallettePlugin"}
        , tool_pallette_{ new ToolPallette}
    {}

    ~ToolPallettePlugin() = default;

    void deserialize( Application* app, const json& state) override
    {
        auto tl_mngr_plg = app->getPlugin<ToolsPlugin>( "ToolsPlugin");
        tool_pallette_->setToolManager( tl_mngr_plg->getToolManager());
    }

    void serialize( json& state) override {}

    ToolPallette* getToolPallette() { return tool_pallette_; }
};

/////////////////// ToolBrush Plugin ///////////////////

class ToolBrush
    : public ITool
{
private:
    float width_;
    sf::Color color_;

    bool is_pressed_;

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
};

class ToolBrushPlugin
    : public IPlugin
{
private:
    ToolBrush* tool_;

    PushButton* tool_button_;

public:
    ToolBrushPlugin()
        : IPlugin{ "ToolBrushPlugin"}
        , tool_{ new ToolBrush( "ToolBrushPlugin::tool")}
        , tool_button_{}
    {}

    ~ToolBrushPlugin() = default;

    void deserialize( Application* app, const json& state) override
    {
        // tool_->setColor( state["color"]);
        tool_->setColor( sf::Color::Red);
        tool_->setWidth( state["width"]);

        std::fprintf( stderr, "before adding tool\n");
        auto tl_mngr_plg = app->getPlugin<ToolsPlugin>( "ToolsPlugin");
        auto tl_mngr = tl_mngr_plg->getToolManager();
        tl_mngr->addTool( tool_);

        std::fprintf( stderr, "before creating button\n");
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

        std::fprintf( stderr, "before getting tool_pall plugin\n");
        auto tl_pal_plg = app->getPlugin<ToolPallettePlugin>( "ToolPallettePlugin");
        std::fprintf( stderr, "before getting tool_pall\n");
        auto tl_pal = tl_pal_plg->getToolPallette();
        std::fprintf( stderr, "before adding button\n");
        tl_pal->add( "ToolBrushPlugin::tool", tool_button_);
        std::fprintf( stderr, "after adding button\n");
    }

    void serialize( json& state) override
    {
        // state["color"] = tool_->getColor();
        state["width"] = tool_->getWidth();
    }
};


} // namespace xui

#endif // TOOLS_H