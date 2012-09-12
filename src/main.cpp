#include <iostream>
#include "scene_lua.hpp"

// set default number of threads
int NUM_THREADS = 4;

int main(int argc, char** argv)
{
    std::string filename = "scene.lua";
    if (argc >= 2) {
        filename = argv[1];
    }
    if (argc >= 3) {
        NUM_THREADS = atoi(argv[2]);
        if (NUM_THREADS == 0) {
            std::cerr << "At least one thread is needed to render." << std::endl;
        }
    }

    if (!run_lua(filename)) {
        std::cerr << "Could not open " << filename << std::endl;
        return 1;
    }
}

