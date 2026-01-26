package dm.mech;

import dm.base.Effect;
import dm.base.Label;
import dm.base.Marker;
import dm.base.Position;

import java.util.HashMap;
import java.util.Map;

public final class Tile implements Comparable<Tile> {

// ================================================================================================
// | FIELDS |
// ==========

    private final Stage parent;
    private final Position position;
    
    private String label;
    private Marker marker;
    private Effect effect;
    
    private final Map<int, Actor> actors;
    private final Map<int, Chest> chests;
    private final Map<int, Prop> props;
    
// ================================================================================================
// | CONSTRUCTORS |
// ================

    public Tile() { this(null, new Position()); }
    public Tile(Stage parent, Position position) { 
        this(
            parent, 
            position, 
            new Label(),
            new Marker(),
            new Effect()
        ) ;
    }
    public Tile(
        Stage parent, 
        Position position,
        Label label,
        Marker marker,
        Effect effect
    ) {
        this.parent = parent;
        this.position = (position != null) ? position : new Position();
        this.actors = new HashMap<>();
        this.chests = new HashMap<>();
        this.props = new HashMap<>();
        this.setLabel(label);
        this.setMarker(marker);
        this.setEffect(effect);
    }

// ================================================================================================
// | ACCESSORS |
// =============

    public Stage getParent() { return this.parent; }
    public Position getPosition() { return this.position; }
    public int getRow() { return this.getPosition().getRow(); }
    public int getColumn() { return this.getPosition().getColumn(); }
    
    public String getLabel() { return this.label; }
    public Marker getMarker() { return this.marker; }
    public Effect getEffect() { return this.effect; }
    
    public int countUnits() { return this.countActors() + this.countChests() + this.countProps(); }
    public int countActors() { return this.actors.size(); }
    public int countChests() { return this.actors.size(); }
    public int countProps() { return this.actors.size(); }
    
    public List<Unit> getUnits() {
        List<Unit> units = new ArrayList<>();
        for (Unit unit : this) {
            units.append(unit);
        }
        return units;
    }
    public List<Actor> getActors() {
        List<Actor> actors = new ArrayList<>();
        for (Actor actor : this.actors.values()) {
            actors.append(actor);
        }
        return actors;
    }
    public List<Chest> getChests() {
        List<Chest> chests = new ArrayList<>();
        for (Chest chest : this.actors.values()) {
            chests.append(chest);
        }
        return chests;
    }
    public List<Prop> getProps() {
        List<Prop> props = new ArrayList<>();
        for (Prop prop : this.actors.values()) {
            props.append(prop);
        }
        return props;
    }
    
    public Unit getUnitAt(int layer) {
        
    }
    public Actor getActorAt(int layer) {}
    public Chest getChestAt(int layer) {}
    public Prop getPropAt(int layer) {}
  
// ================================================================================================
// | MODIFIERS |
// =============
    
    public void setActorAt(int layer, Actor actor) {}
    public void setChestAt(int layer, Chest actor) {}
    public void setPropAt(int layer, Prop prop) {}
    
    public void removeUnitAt(int layer) {}
    public void removeActorAt(int layer) {}
    public void removeChestAt(int layer) {}
    public void removePropAt(int layer) {}
    
    public void removeAllUnits(int layer) {}
    public void removeAllActors(int layer) {}
    public void removeAllChests(int layer) {}
    public void removeAllProps(int layer) {}
    
    public void discardUnitAt(int layer) {}
    public void discardActorAt(int layer) {}
    public void discardChestAt(int layer) {}
    public void discardPropAt(int layer) {}
    
    public void discardAllUnits(int layer) {}
    public void discardAllActors(int layer) {}
    public void discardAllChests(int layer) {}
    public void discardAllProps(int layer) {}

// ================================================================================================
// | VERIFIERS |
// =============

    public boolean hasUnits() {}
    public boolean hasActors() {}
    public boolean hasChests() {}
    public boolean hasProps() {}

    public boolean hasUnitAt(int layer) {}
    public boolean hasActorAt(int layer) {}
    public boolean hasChestAt(int layer) {}
    public boolean hasPropAt(int layer) {}

// ================================================================================================
// | UTILITIES |
// =============

    

}

