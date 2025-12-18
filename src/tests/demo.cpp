#include "actor.hpp"
#include "stage.hpp"
#include "tile.hpp"

#include <iostream>

int main() {
    dm::Stage* stage = dm::Stage::load("../assets/stages/demo-stage-1.dma");
    if (stage == nullptr) {
        std::cout << "Failed to load stage\n";
        return 1;
    }
    
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    dm::Actor::load("../assets/actors/demo-actor-2.dma");
    
    std::cout << stage->toString() << std::endl;
    dm::Stage::unload(stage);
    return 0;
}

