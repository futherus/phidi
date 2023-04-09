#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <getopt.h>
#include <stdexcept>

#include "resources.h"
#include "dynloader.hh"
#include "plugin_registry.hh"
// #include "event.hh"
#include "root.hh"

xui::WidgetManager<xui::IWidget> gRootWidget{ Rectangle( { 100, 100}, { 800, 800})};

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

int
main( int argc,
      char* argv[])
{
    Options opts = GetClOptions( argc, argv);

    sf::IntRect bounds{ {0, 0}, {100, 100}};
    gui::TextureFactory::loadTexture( "ph", "res/ph.jpg", bounds);
    gui::TextureFactory::loadTexture( "pH", "res/pH.jpg", bounds);
    gui::TextureFactory::loadTexture( "Ph", "res/Ph.jpg", bounds);
    gui::TextureFactory::loadTexture( "PH", "res/PH.jpg", bounds);

    gui::FontFactory::loadFont( "default", "res/font.otf");

    $M( "Loaded textures\n");

    xui::json plugins = ReadJson( opts.plugin_ser_);

    $M( "Before loading plugins\n");

    auto* loader = xui::DynamicLoader::instance();
    auto* reg = xui::PluginRegistry::instance();
    for ( auto& plugin : plugins )
    {
        loader->load( plugin["path"]);
        reg->getPlugin( plugin["name"])->deserialize( plugin["data"]);
    }

    $M( "All plugins loaded\n");

    sf::RenderWindow window( sf::VideoMode{1600, 1000},
                             "My window name", sf::Style::Default);

    while ( window.isOpen() )
    {
        sf::Event event = {};

        while ( window.pollEvent( event) )
        {
            switch ( event.type )
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    $D( "mouse pressed\n");
                    sf::Vector2f mouse_pos{ event.mouseButton.x, event.mouseButton.y};
                    if ( gRootWidget.contains( mouse_pos) )
                         gRootWidget.onMousePressed( event);

                    break;
                }
                case sf::Event::MouseButtonReleased:
                {
                    sf::Vector2f mouse_pos{ event.mouseButton.x, event.mouseButton.y};
                    if ( gRootWidget.contains( mouse_pos) )
                         gRootWidget.onMouseReleased( event);

                    break;
                }
                case sf::Event::MouseMoved:
                {
                    gRootWidget.onMouseMoved( event);

                    break;
                }
                case sf::Event::KeyPressed:
                {
                    gRootWidget.onKeyPressed( event);

                    break;
                }
                case sf::Event::KeyReleased:
                {
                    gRootWidget.onKeyReleased( event);

                    break;
                }

                case sf::Event::TextEntered:
                {
                    gRootWidget.onTextEntered( event);

                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        window.clear( sf::Color::Black);

        gRootWidget.draw( window);
        window.display();
    }

    return 0;
}
