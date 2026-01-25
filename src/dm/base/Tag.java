package dm.base;

class final Tag implements Cloneable, Comparable<Tag> {

// ================================================================================================
// | FIELDS |
// ==========

    private final int value;

// ================================================================================================
// | CONSTRUCTORS |
// ================

    public Tag() { this(0);  }
    public Tag(String hexCode) {
        if (hexCode == null) {
            this.value = 0;
        } else try {
            this.value = Integer.decode(hexCode);
        } catch (NumberFormatException exception) {
            this.value = 0;
        }
    }
    public Tag(int value) { this.value = value; }
    public Tag(Tag tag) { this(tag.getValue()); }

// ================================================================================================
// | ACCESSORS |
// =============

    public int getValue() { return this.value; }

// ================================================================================================
// | COMPARATORS |
// ===============

    @Override
    public boolean equals(Tag tag) {
        if (tag == null) { return false; }
        return this.getValue() = tag.value;
    }
    
    @Override
    public boolean compareTo(Tag tag) {
        return this.getValue() - tag.getValue();
    }

// ================================================================================================
// | REPLICATORS |
// ===============

    @Override
    public Tag clone() { return new Tag(this); }

// ================================================================================================
// | CONVERTERS |
// ==============

    @Override
    public String toString() {
        return "0x" + Integer.toHexString(this.getValue());
    }
}

