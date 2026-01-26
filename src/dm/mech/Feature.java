package dm.mech;

import dm.basic.Label;

public abstract class Feature implements Comparable<Asset> {

// ================================================================================================
// | FIELDS |
// ==========

    private Label;
    
// ================================================================================================
// | CONSTRUCTORS |
// ================

    public Feature() { this(new Label()); }
    public Feature(Label label) { this.setLabel(label); }

// ================================================================================================
// | ACCESSORS |
// =============

    public final Label getLabel() { return this.label; }
    public final String getTitle() { return this.getLabel().getTitle(); }
    public final String getCaption() { return this.getLabel().getCaption(); }

// ================================================================================================
// | MODIFIERS |
// =============

    public final void setLabel(Label label) {
        this.label = (label != null) ? label : new Label();
    }    
    public final void setTitle(String title) {
        this.setLabel(new Label(title, this.getLabel().getCaption());
    }
    public final void setCaption(String caption) {
        this.setLabel(new Label(this.getLabel().getCaption(), caption));
    }

// ================================================================================================
// | COMPARATORS |
// ===============

    @Override
    public boolean equals(Feature feature) {
        if (feature == null) { return false; }
        return this.getLabel().equals(feature.getLabel());
    }
    @Override
    public int compareTo(Feature feature) {
        if (feature == null) { return -1; }
        return this.getLabel().compareTo(feature.getLabel());
    }

// ================================================================================================
// | CONVERTERS |
// ==============

    public abstract String toGraphic();
    
    @Override
    public String toString() {
        return this.getLabel().toString();
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(this.getLabel());
    }
}

