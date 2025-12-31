#pragma once

#include "asset.hpp"
#include "config.hpp"
#include "tile.hpp"
#include "utils/cache.hpp"
#include "utils/schema.hpp"

#include <string>

namespace dm {
    class Actor : public Asset {
        
        // ========================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
        public:
            Actor(void);

            Actor(unsigned long id, const std::string& filePath);

            Actor(
                unsigned long id, 
                const std::string& filePath, 
                const std::string& name, 
                char marker
            );
            
            ~Actor(void) = default;
        
        // ========================================================================================
        // | ACCESSORS |
        // =============
        
        public:
            char getMarker(void) const;

            const Tile* getTile(void) const;
            Tile* getTile(void);
            bool isPlaced(void) const;
        
        // ========================================================================================
        // | MODIFIERS |
        // =============
        
        public:
            void setTile(Tile* tile);
            void setTile(Tile* tile, bool transit);

            void setMarker(char marker);
        
        // ========================================================================================
        // | CONVERTERS |
        // ==============
        
        public:
            std::string toString(void) const override;
            Schema toSchema(void) const override;
        
        // ========================================================================================
        // | LOGISTICS |
        // =============
        
        public:
            static Actor* get(unsigned long id);
            static bool contains(unsigned long id);

            static Actor* load(const std::string& filePath);
            static Actor* select(unsigned long id);
            static bool unload(unsigned long id);
        
        // ========================================================================================
        // | MEMBERS |
        // ===========
        
         private:
            static Cache<Actor, DM_ACTOR_CACHE_CAP> _cache; // actor storage

            char _marker; // display character

            Tile* _tile; // tile the actor occupies
    };
}

