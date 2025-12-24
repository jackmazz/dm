#include <iostream>

#include "actor.hpp"
#include "stage.hpp"
#include "tile.hpp"

int main() {
    dm::Stage *stage = dm::Stage::load("../assets/stages/demo-stage-1.dms");
    if (stage == nullptr) {
        std::cout << "Failed to load stage\n";
        return 1;
    }

    std::cout << stage->toString() << std::endl;
    dm::Stage::unload(stage->getId());
    return 0;
}

