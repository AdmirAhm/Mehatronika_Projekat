//#include "main.hpp"
#pragma once
#include "Socket/IncWSLib.h"

#include "Socket/client.h"
#include "Socket/server.h"

#include <thread>
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>
#include "global.h"


int start_gui(int argc, const char* argv[])
{
    Application app(argc, argv);
    app.init("BA");
    return app.run();
}

int start_server() {
    server s;
    s.initAndListenForConnections("44201", server::protocol::TCP);
    return 0;
}

int main(int argc, const char* argv[]) {
    _message = "+";
    close_server = false;
    std::thread guithr(start_gui, argc, argv);
    std::thread serthr(start_server);

    guithr.join();
    serthr.join();
    return 0;
}
