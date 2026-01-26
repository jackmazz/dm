package dm.base;

public final class Bounds implements Cloneable, Comparable<Bounds> {

// ================================================================================================
// | FIELDS |
// ==========

    private final int rows;
    private final int columns;
    private final int layers;

// ================================================================================================
// | CONSTRUCTORS |
// ================

    public Bounds() { this(1, 1); }
    public Bounds(int rows, int columns) { this(rows, columns, 1); }
    public Bounds(int rows, int columns, int layers) {
        this.rows = rows;
        this.columns = columns;
        this.layers = layers;
    }
    public Bounds(Bounds bounds) {
        this(
            bounds.getRows(), 
            bounds.getColumns(), 
            bounds.getLayers()
        );
    }

// ================================================================================================
// | ACCESSORS |
// =============

    public int getArea() { 
        return this.getRows() * this.getColumns() * this.getLayers();
    }
    
    public int getRows() { return this.rows; }
    public int getColumns() { return this.columns; }
    public int getLayers() { return this.layers; }

// ================================================================================================
// | VERIFIERS |
// =============

    public boolean encloses(Position position) {
        if (position == null) { return false; }
        return (
            position.getRow() < this.getRows() &&
            position.getColumn() < this.getColumns() &&
            position.getLayers() < this.getLayers()
        );
    }

// ================================================================================================
// | COMPARATORS |
// ===============

    @Override
    public boolean equals(Bounds bounds) {
        if (bounds == null) { return false; }
        return (
            this.getRows() == bounds.getRows() &&
            this.getColumns() == bounds.getColumns() &&
            this.getLayers() == bounds.getLayers()
        );
    }
    @Override
    public boolean compareTo(Bounds bounds) {
        if (bounds == null) { return -1; }
        if (this.getRows() != bounds.getRows()) {
            return this.getRows() - bounds.getRows();
        } else if (this.getColumns() != bounds.getColumns()) {
            return this.getColumns() - bounds.getColumns();
        } else {
            return this.getLayers() - bounds.getLayers();
        }
    }

// ================================================================================================
// | REPLICATORS |
// ===============

    @Override
    public Bounds clone() { return new Bounds(this); }

// ================================================================================================
// | CONVERTERS |
// ==============

    @Override
    public String toString() {
        return String.format(
            "(%d, %d, %d)",
            this.getRows(),
            this.getColumns(),
            this.getLayers()
        );
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(
            this.getRows(),
            this.getColumns(),
            this.getLayers()
        );
    }
}

