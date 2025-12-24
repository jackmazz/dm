#pragma once

#include <string>
#include <utility>

#include "config.hpp"
#include "tile.hpp"
#include "utils/asset.hpp"
#include "utils/cache.hpp"
#include "utils/schema.hpp"

namespace dm {
    class Actor : public Asset {
    
        public:
        // ============================================================================================
        // | TYPES |
        // =========
        
            using Contact = std::pair<unsigned long, std::string>;
        
        public:
        // ============================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
            Actor(void);

            Actor(unsigned long id, const std::string& filePath);

            Actor(
                unsigned long id, 
                const std::string& filePath, 
                const std::string& name, 
                char marker
            );

            ~Actor(void) = default;

        public:
        // ============================================================================================
        // | ACCESSORS |
        // =============
        
            Contact getContact(void) const;

            char getMarker(void) const;

            const Tile* getTile(void) const;
            Tile* getTile(void);
            bool isPlaced(void) const;

        public:
        // ============================================================================================
        // | MODIFIERS |
        // =============
        
            void setTile(Tile* tile);
            void setTile(Tile* tile, bool transit);

            void setMarker(char marker);

        public:
        // ============================================================================================
        // | CONVERTERS |
        // ==============
        
            std::string toString(void) const override;
            Schema toSchema(void) const override;

        public:
        // ============================================================================================
        // | LOGISTICS |
        // =============
        
            static Actor* get(unsigned long id);
            static bool contains(unsigned long id);

            static Actor* load(const std::string& filePath);
            static Actor* select(unsigned long id);
            static bool unload(unsigned long id);
        
        private:
        // ============================================================================================
        // | MEMBERS |
        // ===========
        
            static Cache<Actor, ACTOR_CACHE_CAP> _cache; // actor storage

            char _marker; // display character

            Tile* _tile; // tile the actor occupies
    };
}

