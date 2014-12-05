#include <iostream>

#ifdef USE_QT
    #include <QApplication>
    #include "QtGUI/mainwindow.h"
#endif

#ifdef USE_SDL2
    #include "Graphics/scene.h"
#endif

#include "Graphics/renderer.h"

namespace Michel {
    class Speaker {
    public:
        static void say() {
            std::cerr << "Hi! I'm Michel! \n";
        }
    };
}

namespace Bob {
    class Speaker {
    public:
        static void say() {
            std::cerr << "Bonjour! Je suis Bob. \n";
        }
    };
}

namespace MainSpeaker = Michel;

int main(int argc, char *argv[])
{   
    #ifdef USE_SDL2

    (void) argc;
    (void) argv;

    Scene scene(640, 480);

    scene.initWindow();

    scene.initGL();

    scene.mainLoop();

    #endif


    #ifdef USE_QT

    QApplication app(argc, argv);

    Renderer renderer(640, 480);

    MainWindow w;
    w.setRenderer(&renderer); //DO NOT FORGET

    w.show();

    return(app.exec());

    #endif

    std::cerr << "Bob\n";

    MainSpeaker::Speaker::say();

    return 0;
}

