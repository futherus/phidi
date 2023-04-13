#pragma once

#include <cassert>
#include <SFML/Graphics.hpp>

#include "../src/pallette.hh"
#include "../src/plugin_registry.hh"
#include "../src/resources.h"
#include "../init/init.hh"

namespace xui
{

class IView
{
protected:
    ~IView() = default;
public:
    virtual void update() = 0;
};

class Canvas;

class ITool
{
private:
    std::string id_;

protected:
    ~ITool() = default;

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
{
private:
    sf::Vector2f size_;
    ToolManager* tool_manager_;

    sf::RenderTexture pixels_;
    sf::Color base_color_;

    bool is_pressed_;
    bool is_hovered_;
    bool is_active_;

public:
    Canvas()
        : size_{ 300, 300}
        , tool_manager_{}
        , pixels_{}
        , base_color_{}
        , is_pressed_{}
        , is_hovered_{}
        , is_active_{}
    {}

    void init()
    {
        pixels_.create( size_.x, size_.y);
        // sprite_.setTexture( pixels_.getTexture());
        // sprite_.setPosition( bounds().tl());
        clear();
    }

    void setToolManager( ToolManager* tool_manager) { tool_manager_ = tool_manager; }
    void setBaseColor( sf::Color color) { base_color_ = color; }

    sf::Color getBaseColor() const { return base_color_; }
    sf::Vector2f getSize() const { return size_; }
    const sf::RenderTexture* getRenderTexture() const { return &pixels_; }

    void clear();
    void drawCircle( sf::Vector2f pos, float radius, sf::Color color);
    void drawLine( sf::Vector2f pos1, sf::Vector2f pos2, float width, sf::Color color);

    // void draw( sf::RenderTarget& target) const override;

    // void onMousePressed ( const sf::Event& event) override;
    // void onMouseReleased( const sf::Event& event) override;
    // void onMouseMoved   ( const sf::Event& event) override;
};


void Render( const Canvas& canvas, const Geometry& geometry, sf::RenderTarget& target);
LayoutObject Layout( const Canvas* canvas, const Constraints& cons);

class ToolsPlugin final
    : public IPlugin
{
private:
    ToolManager* tool_manager_;
    Canvas* canvas_;

public:
    static const int ID;

    ToolsPlugin()
        : IPlugin{}
        , tool_manager_{ new ToolManager}
        , canvas_{ new Canvas}
    {$FUNC
        // PluginRegistry::instance()->getPlugin<InitPlugin>()->add( canvas_);
    }

    ~ToolsPlugin() = default;

    void deserialize( const json& state) override
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

} // namespace xui
