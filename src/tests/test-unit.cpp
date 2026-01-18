#include "unit.hpp"
#include "stage.hpp"
#include "tile.hpp"
#include "utils/dev-tools.hpp"
#include "utils/schema.hpp"

#include <iostream>
#include <string>

// ================================================================================================
// | CONSTRUCTOR & DESTRUCTOR TESTS |
// ==================================

static void _testBasicConstructors(void) {
    unsigned long id = 0xA0000000;
    std::string filePath = "file-path-A";
    std::string name = "unit-A";
    char marker = 'A';
    
    // call each constructor
    dm::Unit unit;
    unit = dm::Unit(id, filePath);
    unit = dm::Unit(id, filePath, name, marker);
    
    std::cout << "test-basic-constructors passed\n";
}

static void _testBasicDestructors(void) {
    unsigned long id = 0xA0000000;
    std::string filePath = std::string("file-path-A");
    std::string name = std::string("unit-A");
    char marker = 'A';
    
    // destroy a local object
    dm::Unit unit = dm::Unit(id, filePath, name, marker);
    unit.~Unit();
    
    // destroy a dynamically-allocated object
    dm::Unit* heapUnit = new dm::Unit(id, filePath, name, marker);
    delete heapUnit;
    
    std::cout << "test-basic-destructors passed\n";
}

// ================================================================================================
// | ACCESSOR TESTS |
// ==================

static void _testBasicAccessors(void) {
    unsigned long id = 0xA0000000;
    std::string filePath = std::string("file-path-A");
    std::string name = std::string("unit-A");
    char marker = 'A';

    dm::Unit unit = dm::Unit(id, filePath, name, marker);
    
    // test getId()
    dm::assertEquals(
        "test-basic-accessors-1",
        "getId() did not return the expected value",
        id, unit.getId()
    );
    
    // test getFilePath()
    dm::assertEquals(
        "test-basic-accessors-2",
        "getFilePath() did not return the expected value",
        filePath, unit.getFilePath()
    );
    
    // test getName()
    dm::assertEquals(
        "test-basic-accessors-3",
        "getName() did not return the expected value",
        name, unit.getName()
    );
    
    // test getMarker()
    dm::assertEquals(
        "test-basic-accessors-4",
        "getMarker() did not return the expected value",
        marker, unit.getMarker()
    );
    
    // test getTile()
    dm::assertEquals<void*>(
        "test-basic-accessors-5",
        "getTile() did not return the null pointer",
        nullptr, unit.getTile()
    );
    
    std::cout << "test-basic-accessors passed\n";
}

// ================================================================================================
// | MODIFIER TESTS |
// ==================

static void _testBasicModifiers(void) {
    unsigned long id = 0xA0000000;
    std::string filePath = std::string("file-path-A");
    std::string name = std::string("unit-A");
    char marker = 'A';
    
    dm::Unit unit = dm::Unit(id, filePath, name, marker);
    
    char newMarker = 'B';
    
    // test setMarker()
    unit.setMarker(newMarker);
    dm::assertEquals(
        "test-basic-modifiers-1",
        "setMarker() did not update the marker",
        newMarker, unit.getMarker()
    );
    
    std::cout << "test-basic-modifiers passed\n";
}

static void _testSetTile(void) {
    unsigned long id = 0xA0000000;
    std::string filePath = std::string("file-path-A");
    std::string name = std::string("unit-A");
    char marker = 'A';
    
    dm::Unit unit = dm::Unit(id, filePath, name, marker);

    dm::Stage stageA;
    dm::Stage stageB;
    dm::Tile tileA(&stageA, 0, 0);
    dm::Tile tileB(&stageB, 0, 0);
    dm::Tile tileC(&stageA, 0, 0);
    
    auto subtest = [&unit](
        const std::string& testName, 
        dm::Tile* tile
    ) {
        dm::Tile* prevTile = unit.getTile();
    
        // test setTile()
        unit.setTile(tile);
        dm::assertEquals(
            testName+"A",
            "setTile() did not update the tile",
            tile, unit.getTile()
        );
        
        if (prevTile != nullptr) {
            if (prevTile != tile) {
                // test that the unit's previous tile isn't occupied 
                dm::assertFalse(
                    testName+"B",
                    "the unit's previous tile is still occupied",
                    prevTile->isOccupied()
                );
            }
            
            if (tile == nullptr || prevTile->getParent() != tile->getParent()) {
                // test that the unit's previous stage doesn't have the unit's contact
                dm::assertFalse(
                    testName+"C",
                    "the unit's previous stage still has the unit's contact",
                    prevTile->getParent()->hasContact(
                        unit.getContact()
                    )
                );
            }
        }
        
        if (tile != nullptr) {        
            // test that the tile points to the unit
            dm::assertEquals(
                testName+"D",
                "the unit's tile does point to the unit",
                &unit, unit.getTile()->getUnit()
            );
            
            // test that the unit's stage has the unit's contact
            dm::assertTrue(
                testName+"E",
                "the unit's stage doesn't have the unit's contact",
                unit.getTile()->getParent()->hasContact(
                    unit.getContact()
                )
            );
        }
    };
    
    subtest("test-set-tile-1", nullptr); // from nothing to nothing
    subtest("test-set-tile-2", &tileA); // from nothing to a tile
    subtest("test-set-tile-3", &tileA); // move to the same tile
    subtest("test-set-tile-4", &tileB); // move to a tile on the same stage
    subtest("test-set-tile-5", &tileC); // move to a tile on a different stage
    subtest("test-set-tile-6", nullptr); // move from a tile to nothing
    
    std::cout << "test-set-tile passed\n";
}

// ================================================================================================
// | CONVERTER TESTS |
// ===================

static void _testToString(void) {
    unsigned long id = 0xA0000000;
    std::string filePath = std::string("file-path-A");
    std::string name = std::string("unit-A");
    char marker = 'A';
    
    dm::Unit unit = dm::Unit(id, filePath, name, marker);
    
    // test toString()
    dm::assertEquals(
        "test-to-string-1",
        "toString() did not return the expected string",
        std::string(1, marker), unit.toString()
    );
    
    std::cout << "test-to-string passed\n";
}

static void _testToSchema(void) {
    unsigned long id = 0xA0000000;
    std::string filePath = std::string("file-path-A");
    std::string name = std::string("unit-A");
    char marker = 'A';
    
    dm::Unit unit = dm::Unit(id, filePath, name, marker);
    
    dm::Schema schema = dm::Schema::decode(
        std::string() +
        "[PROPERTIES]\n" +
        "id = " + std::to_string(id) + "\n" +
        "name = " + name + "\n" +
        "marker = " + std::string(1, marker) + "\n"
    );
    
    // test toSchema()
    dm::assertEquals(
        "test-to-schema-1",
        "toSchema() did not return the expected schema",
        schema, unit.toSchema()
    );

    std::cout << "test-to-schema passed\n";
}

// ================================================================================================
// | MAIN |
// ========

int main(int argCount, char** args) {
    std::cout << "\nTesting unit.cpp\n";

    std::string testName = "all";
    if (argCount > 1) {
        testName = args[1];
    }

    if (testName == "all") {
        _testBasicConstructors();
        _testBasicDestructors();
        _testBasicAccessors();
        _testBasicModifiers();
        _testSetTile();
        _testToString();
        _testToSchema();
    }
    
    else if (testName == "basic-constructors") {
        _testBasicConstructors();
    } 
    
    else if (testName == "basic-destructors") {
        _testBasicDestructors();
    } 
    
    else if (testName == "basic-accessors") {
        _testBasicAccessors();
    } 
    
    else if (testName == "basic-modifiers") {
        _testBasicModifiers();
    }
    
    else if (testName == "set-tile") {
        _testSetTile();
    }
    
    else if (testName == "to-string") {
        _testToString();
    }
    
    else if (testName == "to-schema") {
        _testToSchema();
    }
    
    std::cout << "All tests passed\n";
    return 0;
}

