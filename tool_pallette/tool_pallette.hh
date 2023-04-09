#ifndef TOOL_PALLETTE_HH
#define TOOL_PALLETTE_HH

#include "../tools/tools.hh"
#include "../init/init.hh"

namespace xui
{

class ToolPallette
    : public IView
{
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
        , tool_pallette_{ new ToolPallette}
    {
        PluginRegistry::instance()->getPlugin<InitPlugin>()->add( tool_pallette_->getPallette());
    }

    ~ToolPallettePlugin() = default;

    void deserialize( const json& state) override
    {
        auto tl_mngr_plg = PluginRegistry::instance()->getPlugin<ToolsPlugin>();
        tool_pallette_->setToolManager( tl_mngr_plg->getToolManager());
    }

    void serialize( json& state) override {}

    ToolPallette* getToolPallette() { return tool_pallette_; }

private:
    ToolPallette* tool_pallette_;
};

}

#endif // TOOL_PALLETTE_HH
