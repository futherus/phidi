#ifndef APPLICATION_HH
#define APPLICATION_HH

#include "../include/json/single_include/nlohmann/json.hpp"


namespace xui
{

using json = nlohmann::ordered_json;

class Application;

class IPlugin
{
private:
    std::string id_;

protected:
    ~IPlugin() = default;

public:
    IPlugin(std::string id)
        : id_{ id}
    {}

    std::string getId() { return id_; }

    virtual void deserialize( Application* app, const json& state) = 0;
    virtual void serialize( json& state) = 0;
};

class Application
{
private:
    // FIXME: plugins already store id
    std::map<std::string, IPlugin*> plugins_;

public:
    Application() = default;

    template <typename T>
    T* getPlugin( std::string id)
    {
        return static_cast<T*>( plugins_.find( id)->second);
    }

    void add( IPlugin* plugin)
    {
        plugins_.insert( std::pair<std::string, IPlugin*>{ plugin->getId(), plugin});
    }
};

} // namespace xui

#endif // APPLICATION_HH