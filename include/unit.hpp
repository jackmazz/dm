#pragma once

#include "asset.hpp"
#include "tile.hpp"
#include "utils/schema.hpp"

#include <string>

namespace dm {
    class Unit : public Asset {
        
        // ========================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
        public:
            Unit(void);

            Unit(
                unsigned long id,
                const std::string &filePath
            );

            Unit(
                unsigned long id, 
                const std::string &filePath,
                const std::string& name, 
                char marker
            );
            
            ~Unit(void) = default;
        
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
            void setMarker(char marker);
            
            void setTile(Tile* tile);
            void discard(void);
        
        // ========================================================================================
        // | CONVERTERS |
        // ==============
        
        public:
            std::string toString(void) const override;
        
        // ========================================================================================
        // | LOGISTICS |
        // =============
        
        public:
            static Unit* search(unsigned long id);
            static bool contains(unsigned long id);

            static Unit* load(const std::string& filePath);
            static Unit* select(unsigned long id);
            static bool unload(unsigned long id);
        
        // ========================================================================================
        // | MEMBERS |
        // ===========
        
         private:
            char _marker; // display character
            Tile* _tile; // tile the unit occupies
    };
}

