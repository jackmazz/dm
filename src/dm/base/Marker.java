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
    public boolean equals(Link link) {
        if (link == null) { return false; }
        return (
            this.getFormTag().equals(link.getFormTag()) &&
            this.getObjectTag().equals(link.getObjectTag()) &&
            this.getFormName().equals(link.getFormName())
        );
    }
    
    @Override
    public boolean compareTo(Link link) {
        return this.getTag().getValue() - link.getTag().getValue();
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
        return "("
            + this.getSymbol().toString() + ", "
            + this.getColor().toString() + ")";
    }
    
    public String toAsciiArt() {
        return String.valueOf(this.getSymbol());
    }
}

