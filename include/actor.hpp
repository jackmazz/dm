#pragma once

#include "asset.hpp"
#include "config.hpp"
#include "unit.hpp"
#include "utils/cache.hpp"
#include "utils/form.hpp"

#include <string>

#define DM_ACTOR_TYPE_ID 0xB0000000
#define DM_ACTOR_TYPE_NAME "actor"

namespace dm {
    class Actor : public Unit {
        
        // ========================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
        public:
            Actor(void);

            Actor(
                unsigned long primeId,
                const std::string& filePath
            );

            Actor(
                unsigned long primeId, 
                const std::string& filePath,
                const std::string& name, 
                char marker
            );
            
            ~Actor(void) = default;
        
        // ========================================================================================
        // | ACESSORS |
        // ============
        
        public:
            unsigned long getTypeId(void) const override;
            std::string getTypeName(void) const override;
            
            static bool checkType(const Asset* asset);
            static bool checkType(const Asset::Contact& contact);
            
            static const Actor* cast(const Asset* asset);
            static Actor* cast(Asset* asset);
        
        // ========================================================================================
        // | CONVERTERS |
        // ==============
        
        public:
            Form toForm(void) const override;
        
        // ========================================================================================
        // | LOGISTICS |
        // =============
        
        public:
            static Actor* fetch(unsigned long primeId);
            static bool isLoaded(unsigned long primeId);

            static Actor* load(const std::string& filePath);
            static Actor* select(unsigned long primeId);
            static bool unload(unsigned long primeId);
        
        // ========================================================================================
        // | MEMBERS |
        // ===========
        
         private:
            static Cache<Actor, DM_ACTOR_CACHE_CAP> _cache; // actor storage
    };
}

