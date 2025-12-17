#include "actor.hpp"
#include "stage.hpp"
#include "tile.hpp"

#include <iostream>

int main() {
    dm::Stage* stage = dm::Stage::load("../assets/stages/demo-stage-1.dmo");
    if (stage == nullptr) {
        std::cout << "Failed to load stage\n";
        return 1;
    }
    
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    dm::Actor::load("../assets/actors/demo-actor-2.dmo");
    
    /*
    dm::Actor* actor = stage->findOccupant(1);
    if (actor == nullptr) {
        std::cout << "Failed to find actor\n";
        return 1;
    }
    
    std::cout << "stage-address: " << stage << "\n";
    if (actor->isPlaced()) {
        std::cout << "actor-parent-address: " << actor->placement()->parent() << "\n";
    } else {
        std::cout << "actor-parent-address: " << nullptr << "\n";
    }
    
    std::cout << stage->toString() << std::endl;
    dm::Actor::unload(actor);
    */
    
    std::cout << stage->toString() << std::endl;
    dm::Stage::unload(stage);
    return 0;
}

