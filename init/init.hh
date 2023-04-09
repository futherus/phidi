#ifndef INIT_HH
#define INIT_HH

#include "../src/widget_manager.hh"
#include "../src/plugin_registry.hh"
// #include "../src/event.hh"
#include "../src/root.hh"
#include "../src/rectangle.hh"

namespace xui
{

class InitPlugin final
    : public IPlugin
{
private:
    WidgetManager<IWidget>* manager_;

public:
    static const int ID;

    InitPlugin()
        : IPlugin{}
        , manager_{ new WidgetManager<IWidget>{ gRootWidget.bounds()}}
    {
        gRootWidget.add( manager_);
    }

    ~InitPlugin() = default;

    void deserialize( const json& state) override
    {
        // auto tl_mngr_plg = PluginRegistry::instance()->getPlugin<ToolsPlugin>();
        // tool_pallette_->setToolManager( tl_mngr_plg->getToolManager());
    }

    void serialize( json& state) override {}

    void add( IWidget* widget)
    {
        // FIXME: Find in serialization.
        manager_->add( widget);
    }
};

}

#endif // INIT_HH
