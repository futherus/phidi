#include "tools.hh"

namespace xui
{

void ToolManager::updateViews()
{
    for (auto v : views_)
        v->update();
}

void ToolManager::setActive( const std::string& id)
{
    // FIXME: is available?
    active_tool_ = id;
}

const std::string& ToolManager::getActive() const
{
    return active_tool_;
}

void ToolManager::addTool( ITool* tool)
{
    // FIXME: update views or leave it on them?
    tools_.push_back( tool);
}

ITool* ToolManager::getActiveTool() const
{
    // FIXME: store pointer to active tool instead string
    std::cerr << "active_tool_ = " << active_tool_ << std::endl;

    for (auto t : tools_)
        if (t->getId() == active_tool_)
            return t;

    assert(0 && "Tool was not found");
}


} // namespace xui