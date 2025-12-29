#include "actor.hpp"
#include "asset.hpp"
#include "config.hpp"
#include "stage.hpp"
#include "tile.hpp"
#include "utils/cache.hpp"
#include "utils/schema.hpp"
#include "utils/strings.hpp"

#include <cstddef>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#define _PROPERTIES_SECTION_HEADER "[PROPERTIES]"
#define _MARKERS_SECTION_HEADER "[MARKERS]"
#define _MODIFIERS_SECTION_HEADER "[MODIFIERS]"
#define _CONTACTS_SECTION_HEADER "[CONTACTS]"

namespace dm {

// ================================================================================================
// | CONSTRUCTORS & DESTRUCTORS |
// ==============================

    Stage::Stage(void) : Stage(0, "", 0, 0) {}

    Stage::Stage(
        unsigned long id, 
        const std::string& filePath, 
        std::size_t rowCount, 
        std::size_t columnCount
    ) : Stage(
        id, filePath, "", 
        rowCount, columnCount, 
        "", ""
    ) {}

    Stage::Stage(
        unsigned long id, 
        const std::string& filePath, 
        const std::string& name, 
        std::size_t rowCount, 
        std::size_t columnCount, 
        const std::string& markers, 
        const std::string& modifiers
    ) : Asset(id, filePath, name) {
        this->_rowCount = rowCount;
        this->_columnCount = columnCount;

        std::size_t k = 0; // marker & modifier index
        for (std::size_t i = 0; i < rowCount; i++) {
            for (std::size_t j = 0; j < columnCount; j++) {
                // use default marker if not available for index k
                char marker = '\0';
                if (k < markers.length()) {
                    marker = markers[k];
                }

                // use floor modifier if not available for index k
                char modifier = DM_TILE_MODIFIER_FLOOR;
                if (k < modifiers.length()) {
                    modifier = modifiers[k];
                }

                // append tile to tile vector
                this->_tiles.emplace_back(this, i, j, marker, modifier);
                k++;
            }
        }
    }

// ================================================================================================
// | ACCESSORS |
// =============

    std::size_t Stage::getSize(void) const {
        return this->getRowCount() * this->getColumnCount();
    }

    std::size_t Stage::getRowCount(void) const {
        return this->_rowCount;
    }

    std::size_t Stage::getColumnCount(void) const {
        return this->_columnCount;
    }

    bool Stage::isInBounds(std::size_t row, std::size_t column) const {
        return row < this->getRowCount() && column < this->getColumnCount();
    }

    const Tile* Stage::getTile(std::size_t row, std::size_t column) const {
        // return the null pointer if row & column are out-of-bounds
        if (!this->isInBounds(row, column)) {
            return nullptr;
        }
        
        // access the tile using a 1-dimensional index
        return &this->_tiles[this->_linearIndex(row, column)];
    }

    Tile* Stage::getTile(std::size_t row, std::size_t column) {
        return const_cast<Tile*>(
            static_cast<const Stage*>(this)->getTile(row, column)
        );
    }

    std::vector<const Actor*> Stage::getActors(void) const {
        // accumulate all actors which are currently loaded
        std::vector<const Actor*> actors;
        for (const Asset::Contact& contact : this->_contacts) {
            const Actor* actor = Actor::get(contact.first);
            if (actor != nullptr) {
                actors.push_back(actor);
            }
        }

        return actors;
    }

    std::vector<Actor*> Stage::getActors(void) {
        // accumulate all actors which are currently loaded
        std::vector<Actor*> actors;
        for (const Asset::Contact& contact : this->_contacts) {
            Actor* actor = Actor::get(contact.first);
            if (actor != nullptr) {
                actors.push_back(actor);
            }
        }

        return actors;
    }

    std::vector<Asset::Contact> Stage::getContacts(void) const {
        // accumulate all contact information
        std::vector<Asset::Contact> contacts;
        for (const Asset::Contact& contact : this->_contacts) {
            contacts.push_back(contact);
        }

        return contacts;
    }

// ================================================================================================
// | MODIFIERS |
// =============

    void Stage::addContact(const Asset::Contact& contact) {
        this->_contacts.insert(contact);
    }

    void Stage::addContact(unsigned long id, const std::string& filePath) {
        this->addContact(std::make_pair(id, filePath));
    }

    void Stage::removeContact(const Asset::Contact& contact) {
        this->_contacts.erase(contact);
    }

    void Stage::removeContact(unsigned long id, const std::string& filePath) {
        this->removeContact(std::make_pair(id, filePath));
    }

// ================================================================================================
// | CONVERTERS |
// ==============

    std::string Stage::toString(void) const {
        std::string string;
        std::string newLine;

        for (std::size_t i = 0; i < this->getRowCount(); i++) {
            // separate rows using a newline
            string += newLine;
            newLine = "\n";

            for (std::size_t j = 0; j < this->getColumnCount(); j++) {
                string += this->getTile(i, j)->toString();
            }
        }

        return string;
    }

    Schema Stage::toSchema(void) const {
        Schema schema;
        std::vector<std::string> properties;
        std::vector<std::string> markers;
        std::vector<std::string> modifiers;
        std::vector<std::string> contacts;

        // append properties
        properties.push_back("id = " + std::to_string(this->getId()));
        properties.push_back("name = " + this->getName());
        properties.push_back("row-count = " + std::to_string(this->getRowCount()));
        properties.push_back("column-count = " + std::to_string(this->getColumnCount()));

        // append each row of markers & modifiers
        for (std::size_t i = 0; i < this->getRowCount(); i++) {
            std::string markerRow;
            std::string modifierRow;

            for (std::size_t j = 0; j < this->getColumnCount(); j++) {
                const Tile* tile = this->getTile(i, j);
                markerRow += std::string(1, tile->getMarker());
                modifierRow += std::string(1, tile->getModifier());
            }

            markers.push_back(markerRow);
            modifiers.push_back(modifierRow);
        }

        // append each row of contact information
        for (const Asset::Contact& contact : this->_contacts) {
            contacts.push_back(std::to_string(contact.first) + ", " + contact.second);
        }

        // map headers to entries
        schema[_PROPERTIES_SECTION_HEADER] = properties;
        schema[_MARKERS_SECTION_HEADER] = markers;
        schema[_MODIFIERS_SECTION_HEADER] = modifiers;
        schema[_CONTACTS_SECTION_HEADER] = contacts;

        return schema;
    }

// ================================================================================================
// | LOGISTICS |
// =============

    Cache<Stage, DM_STAGE_CACHE_CAP> Stage::_cache;

    Stage* Stage::get(unsigned long id) {
        return Stage::_cache.get(id);
    }

    bool Stage::contains(unsigned long id) {
        return Stage::_cache.contains(id);
    }

    Stage* Stage::load(const std::string& filePath) {
        std::string fullPath = DM_STAGE_DIR+"/" + filePath;
    
        // attempt to read the schema
        Schema schema;
        if (!schema.read(fullPath)) {
            return nullptr;
        }

        unsigned long id;
        std::string name;
        std::size_t rowCount;
        std::size_t columnCount;
        std::string markers = "";
        std::string modifiers = "";
        std::vector<Asset::Contact> contacts;

        for (const Schema::Section& section : schema) {
            // process the properties section
            if (section.first == _PROPERTIES_SECTION_HEADER) {
                std::unordered_map<std::string, std::string> keyValues 
                    = strings::parseIni(section.second, "=");

                // attempt to parse the loaded properties
                try {
                    id = stol(keyValues["id"]);
                    name = keyValues["name"];
                    rowCount = stol(keyValues["row-count"]);
                    columnCount = stol(keyValues["column-count"]);
                }

                // make sure missing or invalid properties don't cause a crash
                catch (const std::invalid_argument& exception) {
                    return nullptr;
                } catch (const std::out_of_range& exception) {
                    return nullptr;
                }
            }

            // process the markers section
            else if (section.first == _MARKERS_SECTION_HEADER) {
                for (const std::string& entry : section.second) {
                    markers += entry;
                }
            }

            // process the modifers section
            else if (section.first == _MODIFIERS_SECTION_HEADER) {
                for (const std::string& entry : section.second) {
                    modifiers += entry;
                }
            }

            // process the contacts section
            else if (section.first == _CONTACTS_SECTION_HEADER) {
                std::vector<std::vector<std::string>> rows 
                    = strings::parseDsv(section.second, ",", 2);

                for (const std::vector<std::string>& entry : rows) {
                    // attempt to parse the contact information
                    try {
                        unsigned long contactId = stol(strings::trim(entry[0]));
                        std::string contactPath = strings::trim(entry[1]);
                        contacts.emplace_back(contactId, contactPath);
                    }

                    // make sure invalid information doesn't cause a crash
                    catch (const std::invalid_argument& exception) {
                        return nullptr;
                    }
                }
            }
        }

        // attempt to store this stage
        Stage* stage = Stage::_cache.store(
            id, fullPath, name, 
            rowCount, columnCount, 
            markers, modifiers
        );
        if (stage == nullptr) {
            return nullptr;
        }

        return stage;
    }

    Stage* Stage::select(unsigned long id) {
        return Stage::_cache.select(id);
    }

    bool Stage::unload(unsigned long id) {
        // remove any loaded actors from this stage
        Stage* stage = Stage::_cache.get(id);
        for (Actor *actor : stage->getActors()) {
            actor->setTile(nullptr);
        }
        
        return Stage::_cache.remove(id);
    }

// ================================================================================================
// | UTILITIES |
// =============

    std::size_t Stage::_linearIndex(
        std::size_t row, 
        std::size_t column
    ) const {
        // convert the 2-dimensional row/column to a 1-dimensional index
        return row * this->getColumnCount() + column;
    }
}

