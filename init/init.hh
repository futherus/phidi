#pragma once

// #include "../src/widget_manager.hh"
#include "../src/column.hh"
#include "../src/plugin_registry.hh"
// #include "../src/event.hh"
#include "../src/root.hh"

namespace xui
{

class InitPlugin final
    : public IPlugin
{
private:
    std::unique_ptr<Column<WidgetPtr>> manager_;

public:
    static const int ID;

    InitPlugin()
        : IPlugin{}
        , manager_{ std::make_unique<Column<WidgetPtr>>( 10)}
    {
        gRootWidget->getWidgets().push_back( WidgetPtr{ manager_.get()});
    }

    ~InitPlugin() = default;

    void deserialize( const json&) override
    {
        // auto tl_mngr_plg = PluginRegistry::instance()->getPlugin<ToolsPlugin>();
        // tool_pallette_->setToolManager( tl_mngr_plg->getToolManager());
    }

    void serialize( json&) override {}

    void add( WidgetPtr widget)
    {
        // FIXME: Find in serialization.
        manager_->getWidgets().push_back( std::move( widget));
    }
};

}
