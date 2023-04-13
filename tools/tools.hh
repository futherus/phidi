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

struct CanvasImpl final
{
    CanvasImpl()
        : size_{ 300, 300}
        , tool_manager_{}
        , pixels_{}
        , base_color_{}
        , is_pressed_{}
        , is_hovered_{}
        , is_active_{}
    {}

    sf::Vector2f size_;
    ToolManager* tool_manager_;

    sf::RenderTexture pixels_;
    sf::Color base_color_;

    bool is_pressed_;
    bool is_hovered_;
    bool is_active_;

};

class Canvas final
    : public Impl<CanvasImpl>
{
public:
    using Impl::Impl;

    void init()
    {
        impl().pixels_.create( impl().size_.x, impl().size_.y);
        // sprite_.setTexture( pixels_.getTexture());
        // sprite_.setPosition( bounds().tl());
        clear();
    }

    void setToolManager( ToolManager* tool_manager) { impl().tool_manager_ = tool_manager; }
    void setBaseColor( sf::Color color) { impl().base_color_ = color; }

    sf::Color getBaseColor() const { return impl().base_color_; }
    sf::Vector2f getSize() const { return impl().size_; }
    const sf::RenderTexture* getRenderTexture() const { return &impl().pixels_; }

    void clear();
    void drawCircle( sf::Vector2f pos, float radius, sf::Color color);
    void drawLine( sf::Vector2f pos1, sf::Vector2f pos2, float width, sf::Color color);

    // void draw( sf::RenderTarget& target) const override;

    // void onMousePressed ( const sf::Event& event) override;
    // void onMouseReleased( const sf::Event& event) override;
    // void onMouseMoved   ( const sf::Event& event) override;
};


void Render( Canvas canvas, const Geometry& geometry, sf::RenderTarget& target);
LayoutObject Layout( Canvas canvas, const Constraints& cons);

class ToolsPlugin final
    : public IPlugin
{
public:
    static const int ID;

    ToolsPlugin()
        : IPlugin{}
        , tool_manager_{ std::make_unique<ToolManager>()}
        , canvas_{ std::make_unique<CanvasImpl>()}
    {$FUNC
        PluginRegistry::instance()->getPlugin<InitPlugin>()->add( getCanvas());

        $D( "Canvas size: (%f, %f)\n", getCanvas().getSize().x, getCanvas().getSize().y);
    }

    ~ToolsPlugin() = default;

    void deserialize( const json& state) override
    {
        tool_manager_->setActive( state["active_tool"]);

        getCanvas().setToolManager( tool_manager_.get());
        getCanvas().setBaseColor( sf::Color::White);
        getCanvas().init();
        // canvas_->setBaseColor( state["basecolor"]);
    }

    void serialize( json& state) override
    {
        state["active_tool"] = tool_manager_->getActive();

        // state["basecolor"] = canvas_->getBaseColor();
    }

    ToolManager* getToolManager() { return tool_manager_.get(); }
    Canvas getCanvas() { return canvas_.get(); }

private:
    std::unique_ptr<ToolManager> tool_manager_;
    std::unique_ptr<CanvasImpl> canvas_;
};

} // namespace xui
