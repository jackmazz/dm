package dm.base;

public final class Label implements Cloneable, Comparable<Label> {

// ================================================================================================
// | FIELDS |
// ==========

    private final String title;
    private final String caption;

// ================================================================================================
// | CONSTRUCTORS |
// ================

    public Label() { this("<label>"); }
    public Label(String title) { this(title, "<caption>"); }
    public Label(String title, String caption) {
        this.title = (title != null) ? title : "<title>";
        this.caption = (caption != null) ? caption : "<caption>";
    }
    public Label(Label label) {
        this(
            label.getTitle(),
            label.getCaption()
        );
    }

// ================================================================================================
// | ACCESSORS |
// =============

    public String getTitle() { return this.title; }
    public String getCaption() { return this.caption; }

// ================================================================================================
// | COMPARATORS |
// ===============

    @Override
    public boolean equals(Label label) {
        if (label == null) { return false; }
        return (
            this.getTitle().equals(label.getTitle()) &&
            this.getCaption().equals(label.getCaption())
        );
    }
    @Override
    public boolean compareTo(Color color) {
        if (label == null) { return false; }
        if (!this.getTitle().equals(color.getTitle())) {
            return this.getTitle().compareTo(color.getTitle());
        } else {
            return this.getCaption().compareTo(color.getCaption());
        }
    }

// ================================================================================================
// | REPLICATORS |
// ===============

    @Override
    public Label clone() { return new Label(this); }

// ================================================================================================
// | CONVERTERS |
// ==============

    @Override
    public String toString() {
        return String.format(
            "(\"%s\", \"%s\")",
            this.getTitle(),
            this.getCaption()
        );
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(
            this.getTitle(),
            this.getCaption()
        );
    }
}

