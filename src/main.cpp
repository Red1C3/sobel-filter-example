#include <SFML/Window.hpp>
#include <Renderer.h>
using namespace sf;
int main()
{
    unsigned height = 576, width = 1024;
    ContextSettings ctxSettings;
    ctxSettings.antialiasingLevel = 1;
    ctxSettings.attributeFlags = ctxSettings.Core;
    ctxSettings.depthBits = 24;
    ctxSettings.majorVersion = 4;
    ctxSettings.minorVersion = 4;
    ctxSettings.stencilBits = 0;
    Window window(VideoMode(width, height), "sobel-filter", Style::Default, ctxSettings);
    RENDERER.init(height, width);
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
        RENDERER.drawFirstRenderPass();
        RENDERER.drawSecondRenderPass();
        window.display();
    }
}
