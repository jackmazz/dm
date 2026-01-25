package dm.base;

public final class Link implements Cloneable, Comparable<Link> {

// ================================================================================================
// | FIELDS |
// ==========

    private final Tag formTag; // id defined in a form
    private final Tag objectTag; // id for an instance
    private final String formName; // file name for a form

// ================================================================================================
// | CONSTRUCTORS |
// ================

    public Link() { this(new Tag(), new Tag(), "<formName>"); }
    public Link(Tag formTag, Tag objectTag, String formName) {    
        this.formTag = (formTag != null) ? formTag : new Tag();
        this.objectTag = (objectTag != null) ? objectTag : new Tag();
        this.formName = (formName != null) ? formName : "<formName>";
    }
    public Link(Link link) {
        this(
            link.getFormTag(),
            link.getObjectTag(),
            link.getFormName()
        );
    }

// ================================================================================================
// | ACCESSORS |
// =============
    
    public int getFormTag() { return this.formTag; }
    public int getObjectTag() { return this.objectTag; }
    public String getFormName() { return this.formName; }

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
    public Link clone() { return new Link(this); }

// ================================================================================================
// | CONVERTERS |
// ==============

    @Override
    public String toString() {
        return "("
            + this.getFormTag().toString() + ", "
            + this.getObjectTag().toString() + ", "
            + this.getFormName().toString() + ")";
    }
}

