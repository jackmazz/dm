package dm.mech;

import dm.base.Label;

public abstract class Asset extends Feature {

// ================================================================================================
// | FIELDS |
// ==========

    private final Link link;
    private Label label;
    
    private final boolean generic;

// ================================================================================================
// | CONSTRUCTORS |
// ================

    public Asset() { this(new Link()); }
    public Asset(Link link) {
        this(
            link, 
            new Label(), 
            false
        );
    }
    public Asset(
        Link link, 
        Label label, 
        boolean generic
    ) {
        super(label);
        this.link = (link != null) ? link : new Link();
        this.generic = generic;
    }

// ================================================================================================
// | ACCESSORS |
// =============

    public final Link getLink() { return this.link; }
    public final int getFormTag() { return this.getLink().getFormTag(); }
    public final int getObjectTag() { return this.getLink().getObjectTag(); }
    public final String getFormName() { return this.getLinke().getFormName(); }
    
// ================================================================================================
// | VERIFIERS |
// =============

    public final boolean isGeneric() { return this.generic; }

// ================================================================================================
// | COMPARATORS |
// ===============

    @Override
    public boolean equals(Asset asset) {
        if (asset == null) { return false; }
        return this.getLink().equals(asset.getLink());
    }
    @Override
    public int compareTo(Asset asset) {
        if (asset == null) { return -1; }
        return this.getLink().compareTo(asset.getLink());
    }

// ================================================================================================
// | CONVERTERS |
// ==============

    public abstract String toGraphic();
    
    @Override
    public String toString() {
        return String.format(
            "(\"%s\", \"%s\", %s)",
            this.getClass().getSimpleName(),
            this.getLabel.getTitle(),
            this.getLink().toString()
        );
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(this.getLink());
    }
}

