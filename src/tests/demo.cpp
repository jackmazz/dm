#include "actor.hpp"
#include "stage.hpp"
#include "tile.hpp"

#include <iostream>

int main() {
    dm::Stage* stage = dm::Stage::load("../assets/stages/demo-stage-1.dms");
    if (stage == nullptr) {
        std::cout << "Failed to load stage\n";
        return 1;
    }
    
    dm::Stage::load("../assets/stages/demo-stage-2.dms");
    dm::Stage::load("../assets/stages/demo-stage-2.dms");
    dm::Stage::load("../assets/stages/demo-stage-2.dms");
    dm::Stage::load("../assets/stages/demo-stage-2.dms");
    dm::Stage::load("../assets/stages/demo-stage-2.dms");
    
    /*
    dm::Stage* selected = dm::Stage::select(1);
    if (selected == nullptr) {
        std::cout << "Failed to select stage\n";
        return 1;
    }
    */
    
    /*
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    dm::Actor::load("../assets/actors/demo-actor-2.dms");
    */
    
    std::cout << stage->toString() << std::endl;
    dm::Stage::unload(stage->getId());
    return 0;
}

