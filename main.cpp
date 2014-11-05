#include <iostream>

#include "helpers.h"

#include "MathsTools/vec.h"

#include "MathsTools/mat4.h"

#include "Graphics/shader.h"

#include "Graphics/scene.h"

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;


    Scene scene(640, 480);

    scene.initWindow();

    scene.initGL();

    scene.mainLoop();

    return 0;
}

