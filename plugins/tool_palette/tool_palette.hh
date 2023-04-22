#pragma once

#include "plugins/tools/tools.hh"
#include "plugins/init/init.hh"
#include "widgets/padding.hh"

namespace xui
{

class ToolPalette final
{
public:
    ToolPalette()
        : tool_manager_{}
        , palette_{ 5}
    {}

    void
    setToolManager( ToolManager* tool_manager)
    {
        tool_manager_ = tool_manager;

        palette_.bind(
            [&]( int indx){ tool_manager_->setActive( tool_ids_.find( indx)->second); }
        );

        tool_manager->addView( *this);
    }

    void update();

    void add( std::string tool_id, PushButton& button);

    const PushPalette& getPalette() const { return palette_; }
          PushPalette& getPalette()       { return palette_; }

private:
    ToolManager* tool_manager_;
    PushPalette palette_;

    std::vector<std::unique_ptr<Padding<PushButton&>>> paddings_;

    std::map<int, std::string> tool_ids_;
};

class ToolPalettePlugin final
    : public IPlugin
{
public:
    static const int ID;

    ToolPalettePlugin()
        : IPlugin{}
        , tool_palette_{ std::make_unique<ToolPalette>()}
    {$FUNC
        PluginRegistry::instance()->getPlugin<InitPlugin>()->add( tool_palette_->getPalette());
        tool_palette_->setToolManager( PluginRegistry::instance()->getPlugin<ToolsPlugin>()->getToolManager());
    }

    ~ToolPalettePlugin() = default;

    void
    deserialize( const json&) override
    {
        auto tl_mngr_plg = PluginRegistry::instance()->getPlugin<ToolsPlugin>();
        tool_palette_->setToolManager( tl_mngr_plg->getToolManager());
    }

    void serialize( json&) override {}

    ToolPalette* getToolPalette() { return tool_palette_.get(); }

private:
    std::unique_ptr<ToolPalette> tool_palette_;
};

}
