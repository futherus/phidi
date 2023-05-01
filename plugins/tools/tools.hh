#pragma once

#include <cassert>
#include <SFML/Graphics.hpp>

#include "core/plugin_registry.hh"
#include "widgets/palette.hh"
#include "widgets/text.hh"
#include "plugins/init/init.hh"
#include "plugins/fonts/fonts.hh"

namespace xui
{

class ToolManagerViewDelegate
{
public:
    ToolManagerViewDelegate( const ToolManagerViewDelegate& other) = delete;
    ToolManagerViewDelegate& operator=( const ToolManagerViewDelegate& other) = delete;

    ToolManagerViewDelegate( ToolManagerViewDelegate&& other) = default;
    ToolManagerViewDelegate& operator=( ToolManagerViewDelegate&& other) = default;

    ~ToolManagerViewDelegate() = default;

    //
    // We have to protect from non-const copy constructor because
    // the instantiated template version with ViewT=ToolManagerViewDelegate is incorrect.
    //
    template <typename ViewT,
              std::enable_if_t<!std::is_same<ToolManagerViewDelegate, std::decay_t<ViewT>>::value, bool> = true>
    ToolManagerViewDelegate( ViewT& view)
        : view_{ std::addressof( view)}
        , update_{ []( void* view_bytes)
                   {
                       auto* tmp = static_cast<ViewT*>( view_bytes);
                       tmp->update();
                   }}
    {}

    void update() { update_( view_); }

private:
    using UpdateOp = void ( void*);

    void* view_;
    UpdateOp* update_;
};

class Canvas;

class ITool
{
protected:
    ~ITool() = default;

public:
    ITool() = default;

    ITool( const ITool&) = delete;
    ITool& operator=( const ITool&) = delete;
    ITool( ITool&&) = delete;
    ITool& operator=( ITool&&) = delete;

public:
    virtual void onMousePressed ( Canvas* canvas, sf::Vector2f pos) = 0;
    virtual void onMouseReleased( Canvas* canvas, sf::Vector2f pos) = 0;
    virtual void onMouseMoved   ( Canvas* canvas, sf::Vector2f pos) = 0;

    virtual void activate() {}
    virtual void deactivate() {}
};

class ToolManager
{
public:
    ToolManager()
        : tools_{}
        , active_tool_{ kNoActive}
        , views_{}
    {}

    ToolManager( const ToolManager&) = delete;
    ToolManager& operator=( const ToolManager&) = delete;
    ToolManager( ToolManager&&) = delete;
    ToolManager& operator=( ToolManager&&) = delete;
    ~ToolManager() = default;

public:
    void addTool( std::string id, ITool* tool);
    void addView( ToolManagerViewDelegate&& view);

    ITool* getActiveTool() const;

public:
    void
    verify() const
    {
        assert( tools_.size() > 0 && "No tools were added");
        assert( active_tool_ != kNoActive && "Active tool was not set");
        assert( views_.size() > 0 && "ToolManager has no views");
    }

    void setActive( std::string id);
    const std::string& getActive() const;

private:
    static constexpr const char* kNoActive = "No active tool";

    void updateViews();

    std::map<std::string, ITool*> tools_;
    std::string active_tool_;

    std::vector<ToolManagerViewDelegate> views_;
};

class ToolManagerStatus final
{
public:
    ToolManagerStatus( ToolManager* tool_manager)
        : tool_manager_{ tool_manager}
        , current_tool_{ 30,
                         PluginRegistry::instance()->getPlugin<FontsPlugin>()->getFont( "res/font.otf"),
                         "<Current tool>"}
    {
        tool_manager->addView( *this);
    }

    ToolManagerStatus( const ToolManagerStatus&) = delete;
    ToolManagerStatus& operator=( const ToolManagerStatus&) = delete;
    ToolManagerStatus( ToolManagerStatus&&) = delete;
    ToolManagerStatus& operator=( ToolManagerStatus&&) = delete;
    ~ToolManagerStatus() = default;

public:
    void update() { current_tool_.setString( tool_manager_->getActive()); }

    const Text& getText() const { return current_tool_; }
          Text& getText()       { return current_tool_; }

private:
    ToolManager* tool_manager_;
    Text current_tool_;
};

void Render( const ToolManagerStatus& status, const Geometry& geometry, sf::RenderTarget& target);
LayoutObject Layout( const ToolManagerStatus& status, const Constraints& cons);

class Canvas final
{
public:
    Canvas()
        : size_{ 300, 300}
        , tool_manager_{}
        , pixels_{}
        , base_color_{}
        , is_pressed_{}
        , is_hovered_{}
        , is_active_{}
    {
        $M( "Canvas: %p\n", this);
        EventManager::instance().subOnMousePressed ( *this);
        EventManager::instance().subOnMouseReleased( *this);
        EventManager::instance().subOnMouseMoved   ( *this);
    }

    Canvas( const Canvas&) = delete;
    Canvas& operator=( const Canvas&) = delete;
    Canvas( Canvas&&) = delete;
    Canvas& operator=( Canvas&&) = delete;
    ~Canvas() = default;

    void
    init()
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

    void onMousePressed ( sf::Vector2f);
    void onMouseReleased( sf::Vector2f);
    void onMouseMoved   ( sf::Vector2f);

private:
    sf::Vector2f size_;
    ToolManager* tool_manager_;

    sf::RenderTexture pixels_;
    sf::Color base_color_;

    bool is_pressed_;
    bool is_hovered_;
    bool is_active_;

};

void Render( const Canvas& canvas, const Geometry& geometry, sf::RenderTarget& target);
LayoutObject Layout( const Canvas& canvas, const Constraints& cons);

class ToolsPlugin final
    : public IPlugin
{
public:
    static const int ID;

    ToolsPlugin()
        : IPlugin{}
        , tool_manager_{ std::make_unique<ToolManager>()}
        , canvas_{ std::make_unique<Canvas>()}
        , status_{ tool_manager_.get()}
    {$FUNC}

    ~ToolsPlugin() = default;

    void
    deserialize( const json& state) override
    {
        auto init_plg = PluginRegistry::instance()->getPlugin<InitPlugin>();
        init_plg->add( getCanvas(), 0, 0.0f);
        $D( "Canvas size: (%f, %f)\n", getCanvas().getSize().x, getCanvas().getSize().y);
        init_plg->add( status_, 0, 0.5f);

        tool_manager_->setActive( state["active_tool"]);

        getCanvas().setToolManager( tool_manager_.get());
        getCanvas().setBaseColor( sf::Color::White);
        getCanvas().init();
        // canvas_->setBaseColor( state["basecolor"]);
    }

    void
    serialize( json& state) override
    {
        state["active_tool"] = tool_manager_->getActive();

        // state["basecolor"] = canvas_->getBaseColor();
    }

    ToolManager* getToolManager() { return tool_manager_.get(); }
    const Canvas& getCanvas() const { return *canvas_; }
          Canvas& getCanvas()       { return *canvas_; }

private:
    std::unique_ptr<ToolManager> tool_manager_;
    std::unique_ptr<Canvas> canvas_;

    ToolManagerStatus status_;
};

} // namespace xui
