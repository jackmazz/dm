package dm.base;

public final class Position implements Cloneable, Comparable<Position> {

// ================================================================================================
// | FIELDS |
// ==========

    private final int row;
    private final int column;
    private final int layer;

// ================================================================================================
// | CONSTRUCTORS |
// ================

    public Position() { this(0, 0); }
    public Position(int row, int column) { this(row, column, 0) }
    public Position(int row, int column, int layer) {
        this.row = row;
        this.column = column;
        this.layer = layer;
    }
    public Position(Position position) {
        this(
            position.getRow(),
            position.getColumn(),
            position.getLayer()
        );
    }

// ================================================================================================
// | ACCESSORS |
// =============

    public int getRow() { return this.row; }
    public int getColumn() { return this.column; }
    public int getLayer() { return this.layer; }
    
// ================================================================================================
// | VERIFIERS |
// =============

    public boolean isBoundedBy(Bounds bounds) {
        if (bounds == null) { return false; }
        return bounds.encloses(this);
    }

// ================================================================================================
// | COMPARATORS |
// ===============

    @Override
    public boolean equals(Position position) {
        if (position == null) { return false; }
        return (
            this.getRow() == position.getRow() &&
            this.getColumn() == position.getColumn() &&
            this.getLayer() == position.getLayer()
        );
    }
    
    @Override
    public boolean compareTo(Position position) {
        if (this.getRow() != position.getRow()) {
            return this.getRow() - position.getRow();
        } else if (this.getColumn() != position.getColumn()) {
            return this.getColumn() - position.getColumn();
        } else {
            return this.getLayer() - position.getLayer();
        }
    }


// ================================================================================================
// | REPLICATORS |
// ===============

    @Override
    public Bounds clone() { return new Position(this); }

// ================================================================================================
// | CONVERTERS |
// ==============

    @Override
    public String toString() {
        return "("
            + this.getRow() + ", "
            + this.getColumn() + ", "
            + this.getLayer() + ")";
    }
}

