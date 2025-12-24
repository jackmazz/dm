#include "actor.hpp"
#include "stage.hpp"
#include "tile.hpp"

#include <iostream>

int main() {
    dm::Stage *stage = dm::Stage::load("demo-stage-1.dms");
    if (stage == nullptr) {
        std::cout << "Failed to load stage\n";
        return 1;
    }

    std::cout << stage->toString() << std::endl;
    dm::Stage::unload(stage->getId());
    return 0;
}

