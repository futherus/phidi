#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <random>

// #include "button.hh"
// #include "pallette.hh"
#include "resources.h"
#include "loader.hh"
#include "plugin_registry.hh"
#include "init.hh"
// #include "tools.hh"

xui::WidgetManager<xui::IWidget> MANAGER{ Rectangle( { 100, 100}, { 800, 800})};

/*
PushPallette* createToolPallette(Rectangle bounds)
{
    const sf::Texture* ph = &TextureFactory::getTexture("ph");
    const sf::Texture* Ph = &TextureFactory::getTexture("Ph");
    const sf::Texture* pH = &TextureFactory::getTexture("pH");
    const sf::Texture* PH = &TextureFactory::getTexture("PH");

    PushButton* button_brush1 = new PushButton(Rectangle({100, 100}, {100, 100}),
                                               {ph, Ph, pH, PH});
    PushButton* button_brush2 = new PushButton(Rectangle({100, 100}, {100, 100}),
                                               {ph, Ph, pH, PH});
    // PushButton* button_line1  = new PushButton(Rectangle({100, 100}, {100, 100}),
    //                                            {ph, Ph, pH, PH});

    PushPallette* tool_pall = new PushPallette(bounds, 100, 10);
    tool_pall->add(button_brush1);
    tool_pall->add(button_brush2);
    // tool_pall->add(button_line1);

    return tool_pall;
}

void addToolBrush(ToolManager* tool_manager, MutexWidgetManager<IWidget>* widget_manager)
{
    PropPallette* props = new PropPallette(Rectangle({100, 500}, {60, 150}));

    sf::Vector2f padding(0, 10);
    sf::Vector2f empty = props->getEmpty() + padding;
    LineEdit* lineedit_red = new LineEdit(Rectangle(empty, {60, 40}));
    lineedit_red->setPadding(5);
    lineedit_red->setFont(FontFactory::getFont("default"), 30);
    lineedit_red->setColor(sf::Color::Red, sf::Color::White);
    props->add(lineedit_red);

    empty = props->getEmpty() + padding;
    LineEdit* lineedit_green = new LineEdit(Rectangle(empty, {60, 40}));
    lineedit_green->setPadding(5);
    lineedit_green->setFont(FontFactory::getFont("default"), 30);
    lineedit_green->setColor(sf::Color::Green, sf::Color::White);
    props->add(lineedit_green);

    empty = props->getEmpty() + padding;
    LineEdit* lineedit_blue = new LineEdit(Rectangle(empty, {60, 40}));
    lineedit_blue->setPadding(5);
    lineedit_blue->setFont(FontFactory::getFont("default"), 30);
    lineedit_blue->setColor(sf::Color::Blue, sf::Color::White);
    props->add(lineedit_blue);

    ToolBrush* tool_brush = new ToolBrush(5, sf::Color::Red);
    tool_brush->setupColor(lineedit_red, lineedit_green, lineedit_blue);

    tool_manager->add(tool_brush);
    widget_manager->add(props);
}
*/

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 1000),
                            "My window name", sf::Style::Default);

    sf::IntRect bounds({0, 0}, {100, 100});
    gui::TextureFactory::loadTexture("ph", "res/ph.jpg", bounds);
    gui::TextureFactory::loadTexture("pH", "res/pH.jpg", bounds);
    gui::TextureFactory::loadTexture("Ph", "res/Ph.jpg", bounds);
    gui::TextureFactory::loadTexture("PH", "res/PH.jpg", bounds);

    gui::FontFactory::loadFont("default", "res/font.otf");

    const sf::Texture* ph = gui::TextureFactory::getTexture("ph");
    const sf::Texture* Ph = gui::TextureFactory::getTexture("Ph");
    const sf::Texture* pH = gui::TextureFactory::getTexture("pH");
    const sf::Texture* PH = gui::TextureFactory::getTexture("PH");

    // xui::PushButton::TexturePack pack{
    //     std::array<const sf::Texture*, 2>{ ph, Ph},
    //     std::array<const sf::Texture*, 2>{ pH, PH}
    // };

    $M( "Loaded textures\n");

/*
    sf::Clock clock;

    // sf::Sprite  sprite;
    // sprite.setTextureRect(sf::IntRect({0, 0}, {1000, 1000}));
    // sprite.setTexture(texture);
    // sprite.setPosition({100, 100});
    // sprite.scale(0.5, 0.5);

    ToolManager* tool_manager = new ToolManager();
    MutexWidgetManager<IWidget>* mutex_props = new MutexWidgetManager<IWidget>(Rectangle({100, 500}, {60, 150}));
    addToolBrush(tool_manager, mutex_props);
    addToolBrush(tool_manager, mutex_props);
    // ToolBrush*   tool_brush1  = new ToolBrush(5, sf::Color::Red);
    // ToolBrush*   tool_brush2  = new ToolBrush(5, sf::Color::Green);
    // ToolLine*    tool_line1   = new ToolLine (5, sf::Color::Blue);
    // tool_manager->add(tool_brush1);
    // tool_manager->add(tool_brush2);
    // tool_manager->add(tool_line1);

    LineEdit* lineedit = new LineEdit(Rectangle({700, 700}, {100, 40}));
    lineedit->setPadding(5);
    lineedit->setFont(FontFactory::getFont("default"), 30);
    lineedit->setColor(sf::Color::Blue, sf::Color::Red);

    PushPallette* tool_pall1 = createToolPallette(Rectangle({100, 100}, {100, 500}));
    PushPallette* tool_pall2 = createToolPallette(Rectangle({700, 100}, {100, 500}));

    tool_manager->add(tool_pall1);
    tool_manager->add(tool_pall2);
    tool_manager->add(lineedit);
    tool_manager->add(mutex_props);
    tool_manager->setActive(1);

    Canvas* canvas = new Canvas(Rectangle({300, 300}, {400, 400}), tool_manager);

*/

#if 0
    auto button1 = std::make_unique<xui::PushButton>( Rectangle{ { 120, 120}, { 100, 100}}, pack);
    std::cerr << "after button1\n";

    auto button2 = std::make_unique<xui::PushButton>( Rectangle{ { 120, 230}, { 100, 100}}, pack);
    std::cerr << "after button2\n";

    auto pallette = std::make_unique<xui::PushPallette>( Rectangle{ { 110, 110}, { 300, 300}});
    std::cerr << "after pallette\n";

    pallette->add( button1.get());
    pallette->add( button2.get());
    std::cerr << "after add buttons\n";

    auto manager = std::make_unique<xui::WidgetManager<xui::IWidget>>( Rectangle( { 100, 100}, { 800, 800}));
    std::cerr << "after manager\n";

    manager->add( pallette.get());
    std::cerr << "after add pallette\n";

    pallette->bind([]( int val){ foo( true, val); });
    std::cerr << "after bind\n";
#endif

    std::ifstream istr("tmp.json");
    if (!istr.is_open())
    {
        std::cerr << "failed to open\n";
        return 1;
    }

    gui::json data = gui::json::parse(istr);
    istr.close();

    $M( "Before loading plugins\n");

    auto* loader = xui::Loader::getLoader();

    loader->load( "./bin/tools.so");
    loader->load( "./bin/tool_pallette.so");
    loader->load( "./bin/toolbrush.so");

    $M( "All plugins loaded\n");

    $M( "Before deserialization\n");
    auto* reg = xui::PluginRegistry::getPluginRegistry();

    $M( "tools deserialization:\n");
    reg->getPlugin( "ToolsPlugin")->deserialize( data["tools"]);
    $M( "tool_pall deserialization:\n");
    reg->getPlugin( "ToolPallettePlugin")->deserialize( data["tool_pall"]);
    $M( "brush deserialization:\n");
    reg->getPlugin( "ToolBrushPlugin")->deserialize( data["brush"]);

    $M( "All plugins deserialized\n");

#if 0
    auto manager = std::make_unique<xui::WidgetManager<xui::IWidget>>( Rectangle( { 100, 100}, { 800, 800}));
    $M( "Before manager filling\n");
    manager->add( tool_pall.getToolPallette()->getPallette());
    manager->add( tools.getCanvas());
#endif

    while(window.isOpen())
    {
        sf::Event event = {};

        while(window.pollEvent( event))
        {
            switch( event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    fprintf( stderr, "mouse_pressed\n");
                    sf::Vector2f mouse_pos( event.mouseButton.x, event.mouseButton.y);
                    if ( MANAGER.contains( mouse_pos))
                        MANAGER.onMousePressed( event);

                    break;
                }
                case sf::Event::MouseButtonReleased:
                {
                    sf::Vector2f mouse_pos( event.mouseButton.x, event.mouseButton.y);
                    if ( MANAGER.contains( mouse_pos))
                        MANAGER.onMouseReleased( event);

                    break;
                }
                case sf::Event::MouseMoved:
                {
                    MANAGER.onMouseMoved( event);

                    break;
                }
                case sf::Event::KeyPressed:
                {
                    MANAGER.onKeyPressed( event);

                    break;
                }
                case sf::Event::KeyReleased:
                {
                    MANAGER.onKeyReleased( event);

                    break;
                }

                case sf::Event::TextEntered:
                {
                    MANAGER.onTextEntered( event);

                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        // sf::Time elapsed = clock.restart();
        // button1->move({0, 10 * elapsed.asSeconds()});
        window.clear( sf::Color::Black);

        // std::cout << "1: " << button1->is_pressed() << std::endl;
        // std::cout << "2: " << button2->is_pressed() << std::endl;

        // pallette.draw(window);
        // canvas.draw(window);
        MANAGER.draw( window);
        window.display();
    }

    return 0;
}
