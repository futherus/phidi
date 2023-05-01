#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <getopt.h>
#include <stdexcept>

//#include "core/lru_cache.hh"
#include "core/dynloader.hh"
#include "core/plugin_registry.hh"
#include "core/event.hh"
//#include "core/root.hh"

//#include "widgets/column.hh"
#include "widgets/button.hh"
#include "widgets/widget_manager.hh"

xui::WidgetManager* gRootWidget = nullptr;

struct Options
{
    std::string plugin_ser_;
};

static Options
GetClOptions( int argc,
              char* argv[])
{
    /**
     * Redirect long options straight to the short options handlers
     * Note: the last element has to be filled with zeroes.
     */
    static struct option options[] = {
            {"plugins", required_argument, NULL, 'p'},
            { 0,        0,                 0,     0 },
    };

    Options opts;
    int opt = 0;
    while ( (opt = getopt_long( argc, argv, "p:", options, NULL)) != -1 )
    {
        switch ( opt )
        {
            case 'p':
                opts.plugin_ser_ = optarg;
                break;
            case '?':
            case ':':
                exit( 1);
            default:
                assert( 0);
        }
    }

    if ( (argc - optind) > 0 )
    {
        $M( "Invalid number of options\n");
        exit( 1);
    }

    if ( opts.plugin_ser_ == "" )
    {
        $M( "-p (--plugins) is mandatory\n");
        exit( 1);
    }

    return opts;
}

static xui::json
ReadJson( const std::string& path)
{
    std::ifstream istr{ path};
    if ( !istr.is_open() )
    {
        $M( "Failed to open \"%s\"\n", path.data());
        exit( 1);
    }

    xui::json data = xui::json::parse( istr);
    istr.close();

    return data;
}

const float WINDOW_WIDTH = 1200;
const float WINDOW_HEIGHT = 800;

int
main( int argc,
      char* argv[])
{
    Options opts = GetClOptions( argc, argv);

    $M( "Loaded textures\n");

    auto root_widget = std::make_unique<xui::WidgetManager>( sf::Vector2f{ WINDOW_WIDTH / 1.5, WINDOW_HEIGHT});
    gRootWidget = root_widget.get();

    $M( "Created root widget\n");

    xui::json plugins = ReadJson( opts.plugin_ser_);

    $M( "Before loading plugins\n");

    auto* loader = xui::DynamicLoader::instance();
    auto* reg = xui::PluginRegistry::instance();
    for ( auto& plugin : plugins )
        loader->load( plugin["path"], plugin["name"]);

    $M( "Before deserializing plugins\n");

    for ( auto it = plugins.rbegin(); it != plugins.crend(); it++ )
        reg->getPlugin( (*it)["name"])->deserialize( (*it)["data"]);

    $M( "All plugins loaded\n");

    sf::RenderWindow window( sf::VideoMode{ WINDOW_WIDTH, WINDOW_HEIGHT},
                             "My window name", sf::Style::Default);

    $$
    xui::LayoutObject obj = Layout( *gRootWidget, Constraints{ WINDOW_WIDTH / 1.5, WINDOW_HEIGHT});

    $D( "After initial layout\n");

    Render( obj, window);

    std::vector<const void*> clicked{};
    clicked.reserve( 0x10);

    while ( window.isOpen() )
    {
        sf::Event event = {};

        while ( window.pollEvent( event) )
        {
            xui::EventManager::instance().handleEvents( obj, event);

            switch ( event.type )
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::Resized:
                {
                    sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                    window.setView(sf::View(visibleArea));
                    $$
                    obj = Layout( *gRootWidget, Constraints{event.size.width / 1.5, event.size.height});

                    obj.adjust();

                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        window.clear( sf::Color::Black);
        Render( obj, window);
        window.display();
    }

    return 0;
}
