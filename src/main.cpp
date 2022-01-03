#include <SFML/Window.hpp>
using namespace sf;
int main()
{
    unsigned height = 720, width = 1280;
    ContextSettings ctxSettings;
    ctxSettings.antialiasingLevel = 1;
    ctxSettings.attributeFlags = ctxSettings.Core;
    ctxSettings.depthBits = 24;
    ctxSettings.majorVersion = 4;
    ctxSettings.minorVersion = 4;
    ctxSettings.stencilBits = 0;
    Window window(VideoMode(width, height), "sobel-filter", Style::Default, ctxSettings);
    while (true)
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                return 0;
                break;

            default:
                break;
            }
        }
        window.display();
    }
}
