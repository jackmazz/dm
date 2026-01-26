package dm.mech;

import dm.base.Bounds;
import dm.base.Effect;
import dm.base.Label;
import dm.base.Link;
import dm.base.Marker;
import dm.base.Position;
import dm.base.Tag;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

public final class Stage extends Asset {

// ================================================================================================
// | FIELDS |
// ==========

    private final List<Tile> tiles; // tile storage
    private final Bounds bounds; // width and height of the stage
    
    private final Map<Link, Position> actorLinks; // actor position storage
    private final Map<Link, Position> chestLinks; // chest position storage
    private final Map<Link, Position> propLinks; // prop position storage

// ================================================================================================
// | CONSTRUCTORS |
// ================

    public Stage() { this(new Link(), new Bounds()); }
    public Stage(Link link, Bounds bounds) {
        this(
            link,
            new Label(),
            bounds,
            new ArrayList<>(), 
            new ArrayList<>(),
            false
        );
    }
    public Stage(
        Link link,
        Label label,
        Bounds bounds,
        List<Marker> markers,
        List<Effect> effects,
        boolean generic
    ) {
        super(link, label, generic);
        this.bounds = (bounds != null) ? bounds : new Bounds();
        this.tiles = new ArrayList<>();
        this.actorLinks = new HashMap<>();
        this.chestLinks = new HashMap<>();
        this.propLinks = new HashMap<>();
        
        markers = (markers != null) ? markers : new ArrayList<>();
        effects = (effects != null) ? effects : new ArrayList<>();
        
        int k = 0;
        for (int i = 0; i < this.getRows(); i++) {
            for (int j = 0; j < this.getColumns(); j++) {
                Marker marker = (k < markers.size()) ? markers.get(k) : new Marker();
                Effect effect = (k < effects.size()) ? effects.get(k) : new Effect();
                this.tiles.append(new Tile(
                    this, i, j,
                    marker,
                    effect
                ));
                
                k++;
            }
        }
    }

// ================================================================================================
// | ACCESSORS |
// =============

    public Bounds getBounds() { return this.bounds(); }
    public int getArea() { return this.getBounds().getArea(); }
    public int getRows() { return this.getBounds().getRows(); }
    public int getColumns() { return this.getBounds().getColumns(); }
    
    public int countTiles() { return this.tiles.size(); }
    public int countUnits() {
        int count = 0;
        for (Tile tile : this) {
            count += tile.countUnits();
        }
        return count;
    }
    public int countActors() {
        int count = 0;
        for (Tile tile : this) {
            count += tile.countActors();
        }
        return count;
    }
    public int countChests() {
        int count = 0;
        for (Tile tile : this) {
            count += tile.countChests();
        }
        return count;
    }
    public int countProps() {
        int count = 0;
        for (Tile tile : this) {
            count += tile.countProps();
        }
        return count;
    }
    
    public int countUnitsAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.countUnits();
    }
    public int countActorsAt() {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.countActors();
    }
    public int countChestsAt() {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.countChests();
    }
    public int countPropsAt() {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.countProps();
    }

    public List<Tile> getTiles() {
        List<Tile> tiles = new ArrayList<>();
        for (Tile tile : this.tiles) {
            tiles.append(tile);
        }
        return tiles;
    }
    public List<Marker> getMarkers() {
        List<Marker> markers = new ArrayList<>();
        for (Tile tile : this.tiles) {
            markers.append(tile.getMarker());
        }
        return markers;
    }
    public List<Effect> getEffects() {
        List<Effect> effects = new ArrayList<>();
        for (Tile tile : this.tiles) {
            effects.append(tile.getEffect());
        }
        return effects;
    }
    public List<Unit> getUnits() {
        List<Unit> units = new ArrayList<>();
        units.addAll(this.getActors());
        units.addAll(this.getChests());
        units.addAll(this.getProps());
        return units
    }
    public List<Actor> getActors() {
        List<Actor> actors = new ArrayList<>();
        for (Tile tile : this.tiles) {
            actors.addAll(tile.getActors());
        }
        return actors;
    }
    public List<Chest> getChests() {
        List<Chest> chests = new ArrayList<>();
        for (Tile tile : this.tiles) {
            chests.addAll(tile.getChests());
        }
        return chests;
    }
    public List<Prop> getProps() {
        List<Prop> props = new ArrayList<>();
        for (Tile tile : this.tiles) {
            props.addAll(tile.getProps());
        }
        return props;
    }
    
    public List<Unit> getLoadedUnits() {
        List<Unit> units = new ArrayList<>();
        units.addAll(this.getLoadedActors());
        units.addAll(this.getLoadedChests());
        units.addAll(this.getLoadedProps());
        return units
    }
    public List<Actor> getLoadedActors() {
        List<Actor> actors = new ArrayList<>();
        for (Link link : this.actorLinks) {
            if (!Actor.isLoaded(link)) { continue; }
            Actor.fetch(link).discard();
        }
        return actors;
    }
    public List<Chest> getLoadedChests() {
        List<Chest> chests = new ArrayList<>();
        for (Link link : this.chestLinks) {
            if (!Link.isLoaded(link)) { continue; }
            Chest.fetch(link).discard();
        }
        return chests;
    }
    public List<Prop> getLoadedProps() {
        List<Prop> props = new ArrayList<>();
        for (Link link : this.propLinks) {
            if (!Link.isLoaded(link)) { continue; }
            Prop.fetch(link).discard();
        }
        return props;
    }
    
    public Set<Link> getUnitLinks() {
        Set<Link> links = new HashSet<>();
        links.addAll(this.getActorLinks());
        links.addAll(this.getChestLinks());
        links.addAll(this.getPropLinks());
        return links;
    }
    public Set<Link> getActorLinks() { return this.getLinks(this.actorLinks); }
    public Set<Link> getChestLinks() { return this.getLinks(this.chestLinks); }
    public Set<Link> getPropLinks() { return this.getLinks(this.propLinks); }
    
    public Set<Position> getLinkedUnitPositions() {
        Set<Position> positions = new HashSet<>();
        positions.addAll(this.getLinkedActorPositions());
        positions.addAll(this.getLinkedChestPositions());
        positions.addAll(this.getLinkedPropPositions());
        return positions;
    }
    public Set<Position> getLinkedActorPositions() { return this.getLinkedPositions(this.actorLinks); }
    public Set<Position> getLinkedChestPositions() { return this.getLinkedPositions(this.chestLinks); }
    public Set<Position> getLinkedPropPositions() { return this.getLinkedPositions(this.propLinks); }
    
    public Position getLinkedUnitPosition(Link link) {
        if (isLinkedToActor(link)) { return this.getLinkedActorPosition(link); }
        if (isLinkedToChest(link)) { return this.getLinkedChestPosition(link); }
        if (isLinkedToProp(link)) { return this.getLinkedPropPosition(link); }
    }
    public Position getLinkedActorPosition(Link link) { return this.getLinkedPosition(this.actorLinks); };
    public Position getLinkedChestPosition(Link link) { return this.getLinkedPosition(this.chestLinks); };
    public Position getLinkedPropPosition(Link link) { return this.getLinkedPosition(this.propLinks); };
    
    public Tile getTileAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        return this.tiles.get(this.linearIndexOf(position));
    }
    public Marker getMarkerAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.getMarker();
    }
    public Effect getEffectAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.getEffect();
    }
    public Unit getUnitAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.getUnitAt(position.getLayer());
    }
    public Actor getActorAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.getActorAt(position.getLayer());
    }
    public Prop getChestAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.getChestAt(position.getLayer());
    }
    public Chest getPropAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.getPropAt(position.getLayer());
    }

// ================================================================================================
// | MODIFIERS |
// =============

    public void setMarkerAt(Position position, Marker marker) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.setMarker((marker != null) ? marker : new Marker());
    }
    public void setEffectAt(Position position, Effect Effect) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.setEffect((effect != null) ? effect : new Effect());
    }
    public void setActorAt(Position position, Actor actor) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.setActorAt(position.getLayer(), actor);
    }
    public void setChestAt(Position position, Chest chest) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.setChestAt(position.getLayer(), chest);
    }
    public void setPropAt(Position position, Prop prop) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.setPropAt(position.getLayer(), prop);
    }
    
    public void removeUnitAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.removeUnitAt(position.getLayer());
    }
    public void removeActorAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.removeActorAt(position.getLayer());
    }
    public void removeChestAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.removeChestAt(position.getLayer());
    }
    public void removePropAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.removePropAt(position.getLayer());
    }
    
    public void removeAllUnitsAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.removeAllUnits();
    }
    public void removeAllActorsAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.removeAllActors();
    }
    public void removeAllChestsAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.removeAllChests();
    }
    public void removeAllPropsAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.removeAllProps();
    }
    
    public void removeAllUnits() {
        for (Tile tile : this) {
            tile.removeAllUnits();
        }
    }
    public void removeAllActors() {
        for (Tile tile : this) {
            tile.removeAllActors();
        }
    }
    public void removeAllChests() {
        for (Tile tile : this) {
            tile.removeAllChests();
        }
    }
    public void removeAllProps() {
        for (Tile tile : this) {
            tile.removeAllProps();
        }
    }
    
    public void discardUnitsAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.discardUnitAt(position.getLayer());
    }
    public void discardActorAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.discardActorAt(position.getLayer());
    }
    public void discardChestAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.discardChestAt(position.getLayer());
    }
    public void discardPropAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.discardPropAt(position.getLayer());
    }
    
    public void discardAllUnitsAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.discardAllUnits();
    }
    public void discardAllActorsAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.discardAllActors();
    }
    public void discardAllChestsAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.discardAllChests();
    }
    public void discardAllPropsAt(Position position) {
        if (!this.isInBounds(position)) { return; }
        Tile tile = this.getTileAt(position);
        tile.discardAllProps();
    }
    
    public void discardAllUnits() {
        for (Tile tile : this) {
            tile.discardAllUnits();
        }
    }
    public void removeAllActors() {
        for (Tile tile : this) {
            tile.discardAllActors();
        }
    }
    public void removeAllChests() {
        for (Tile tile : this) {
            tile.discardAllChests();
        }
    }
    public void removeAllProps() {
        for (Tile tile : this) {
            tile.discardAllProps();
        }
    }
    
    public void linkActor(Link link, Position position) { this.link(this.actorLinks, link, position); }
    public void linkChest(Link link, Position position) { this.link(this.chestLinks, link, position); }
    public void linkProp(Link link, Position position) { this.link(this.propLinks, link, position); }
    
    public void unlinkUnit(Link link) {
        if (isLinkedToActor(link)) { unlinkActor(link); }
        else if (isLinkedToChest(link)) { unlinkChest(link); }
        else if (isLinkedToProp(link)) { unlinkProp(link); }
    }
    public void unlinkActor(Link link) { this.unlink(this.actorLinks, link, position); }
    public void unlinkChest(Link link) { this.unlink(this.chestLinks, link, position); }
    public void unlinkProp(Link link) { this.unlink(this.propLinks, link, position); }
    
    public void unlinkAllUnits() {
        unlinkAllActors();
        unlinkAllChests();
        unlinkAllProps();
    }
    public void unlinkAllActors() { this.unlinkAll(this.actorLinks); }
    public void unlinkAllChests() { this.unlinkAll(this.chestLinks); }
    public void unlinkAllProps() { this.unlinkAll(this.propLinks); }

// ================================================================================================
// | VERIFIERS |
// =============

    public boolean isInBounds(Position position) { return this.getBounds().encloses(position); }
    
    public boolean hasUnitsAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.hasUnits();
    }
    public boolean hasActorsAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.hasActors();
    }
    public boolean hasChestsAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.hasChests();
    }
    public boolean hasPropsAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.hasProps();
    }
    
    public boolean hasUnitAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.hasUnitAt(position.getLayer());
    }
    public boolean hasActorAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.hasActorAt(position.getLayer());
    }
    public boolean hasChestAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.hasChestAt(position.getLayer());
    }
    public boolean hasPropAt(Position position) {
        if (!this.isInBounds(position)) { return null; }
        Tile tile = this.getTileAt(position);
        return tile.hasPropAt(position.getLayer());
    }
    
    public boolean isLinkedToUnit(Link link) {
        return (
            this.isLinkedToActor(link) ||
            this.isLinkedToChest(link) ||
            this.isLinkedToProp(link)
        );
    }
    public boolean isLinkedToActor(Link link) { return this.isLinked(this.actorLinks, link); }
    public boolean isLinkedToChest(Link link) { return this.isLinked(this.chestLinks, link); }
    public boolean isLinkedToProp(Link link) { return this.isLinked(this.propLinks, link); }
 
// ================================================================================================
// | ITERATORS |
// =============
 
    @Override
    public Iterator<Tile> iterator() { return this.tiles.iterator(); }

// ================================================================================================
// | CONVERTERS |
// ==============

    @Override
    public String toGraphic() {
        StringBuilder builder = new StringBuilder();
        String newLine = "";
        for (int i = 0; i < this.getRows(); i++) {
            builder.append(newLine);
            newLine = System.lineSeparator()
        
            for (int j = 0; j < this.getColumns(); j++) {
                builder.append(this.getMarkerAt(i, j));
            }
        }
        
        return builder.toString();
    }

// ================================================================================================
// | UTILITIES |
// =============

    private Set<Link> <K, V> getLinks(Map<K, V> links) {
        Set<Link> linkSet = new HashSet<>();
        for (Link link : links.keySet()) {
            linkSet.add(link);
        }
        return linkSet;
    }
    private Set<Position> <K, V> getLinkedPositions(Map<K, V> links) {
        Set<Link> positionSet = new HashSet<>();
        for (Position position : links.values()) {
            positionSet.add(position);
        }
        return positionSet;
    }
    private Position getLinkedPosition(Map<K, V> links, Link link) {
        return links.getOrDefault(link, null);
    }
    
    private void <K, V> link(Map<K, V> links, Link link, Position position) {
        if (link == null || position == null) { return; }
        if (this.isLinkedToUnit(link)) { return; }
        links.put(link, position);
    }
    private void <K, V> unlink(Map<K, V> links, Link link) {
        if (link == null) { return; }
        links.remove(link);
    }
    private void <K, V> unlinkAll(Map<K, V> links) {
        links.clear();
    }
    private boolean <K, V> isLinked(Map<K, V> map, Link link) {
        if (link == null) { return false; }
        return links.contains(link);
    }

    private int linearIndexOf(Position position) {
        return this.rows() * position.row() + position.column();
    }
}

