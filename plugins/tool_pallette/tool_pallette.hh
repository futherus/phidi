#pragma once

#include "plugins/tools/tools.hh"
#include "plugins/init/init.hh"

namespace xui
{

class ToolPallette final
{
public:
    ToolPallette()
        : tool_manager_{}
        , pallette_{ 5}
    {}

    void
    setToolManager( ToolManager* tool_manager)
    {
        tool_manager_ = tool_manager;

        pallette_.bind(
            [&]( int indx){ tool_manager_->setActive( tool_ids_.find( indx)->second); }
        );

        tool_manager->addView( *this);
    }

    void update();

    void add( std::string tool_id, PushButton& button);

    const PushPallette& getPallette() const { return pallette_; }
          PushPallette& getPallette()       { return pallette_; }

private:
    ToolManager* tool_manager_;
    PushPallette pallette_;

    std::map<int, std::string> tool_ids_;
};

class ToolPallettePlugin final
    : public IPlugin
{
public:
    static const int ID;

    ToolPallettePlugin()
        : IPlugin{}
        , tool_pallette_{ std::make_unique<ToolPallette>()}
    {$FUNC
        PluginRegistry::instance()->getPlugin<InitPlugin>()->add( tool_pallette_->getPallette());
        tool_pallette_->setToolManager( PluginRegistry::instance()->getPlugin<ToolsPlugin>()->getToolManager());
    }

    ~ToolPallettePlugin() = default;

    void
    deserialize( const json&) override
    {
        auto tl_mngr_plg = PluginRegistry::instance()->getPlugin<ToolsPlugin>();
        tool_pallette_->setToolManager( tl_mngr_plg->getToolManager());
    }

    void serialize( json&) override {}

    ToolPallette* getToolPallette() { return tool_pallette_.get(); }

private:
    std::unique_ptr<ToolPallette> tool_pallette_;
};

}
