#include "config.h"
#include "App.h"

int main()
{
    std::unique_ptr<App> app = std::make_unique<App>(SCR_WIDTH, SCR_HEIGHT);
    
    app->run();

    return EXIT_SUCCESS;
}

// todo: remove unnecessary window pointers!