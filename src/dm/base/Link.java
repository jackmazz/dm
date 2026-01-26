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
        if (link == null) { return -1; }
        if (this.getFormTag() != link.getFormTag()) {
            return this.getFormTag() - link.getFormTag();
        } else if (this.getObjectTag() != link.getObjectTag()) {
            return this.getObjectTag() - link.getObjectTag();
        } else {
            return this.getFormName() - link.getFormName();
        }
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
        return String.format(
            "(%s, %s, \"%s\")",
            this.getFormTag().toString(),
            this.getObjectTag().toString(),
            this.getFormName()
        );
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(
            this.getFormTag(),
            this.getObjectTag(),
            this.getFormName()
        );
    }
}

