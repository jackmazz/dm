package dm.base;

public final class Marker implements Cloneable, Comparable<Link> {

// ================================================================================================
// | FIELDS |
// ==========

    private final char symbol;
    private final Color color;

// ================================================================================================
// | CONSTRUCTORS |
// ================

    public Marker() { this('\0', new Color()); }
    public Marker(char symbol, Color color) {
        this.symbol = symbol;
        this.color = (color != null) ? color : new Color();
    }
    public Marker(Marker marker) {
        this(
            marker.getSymbol(),
            marker.getColor()
        );
    }

// ================================================================================================
// | ACCESSORS |
// =============

    public char getSymbol() { return this.symbol; }
    public Color getColor() { return this.color; }

// ================================================================================================
// | COMPARATORS |
// ===============

    @Override
    public boolean equals(Marker marker) {
        if (marker == null) { return false; }
        return (
            this.getSymbol() == marker.getSymbol() &&
            this.getColor().equals(marker.getColor())
        );
    }
    @Override
    public boolean compareTo(Marker marker) {
        if (marker == null) { return -1; }
        if (this.getSymbol() != marker.getSymbol()) {
            return this.getSymbol() - marker.getSymbol();
        } else {
            return this.getColor().compareTo(marker.getColor());
        }
    }

// ================================================================================================
// | REPLICATORS |
// ===============

    @Override
    public Link clone() { return new Marker(this); }

// ================================================================================================
// | CONVERTERS |
// ==============

    @Override
    public String toString() {
        return String.format(
            "(%c, %s)",
            this.getSymbol(),
            this.getColor().toString()
        );
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(
            this.getSymbol(),
            this.getColor()
        );
    }
}

