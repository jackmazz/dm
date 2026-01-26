package dm.base;

public final class Color implements Cloneable, Comparable<Color> {

// ================================================================================================
// | FIELDS |
// ==========

    private final int red;
    private final int green;
    private final int blue;

// ================================================================================================
// | CONSTRUCTORS |
// ================

    public Color() { this(255, 0, 255); }
    public Color(String hexCode) {
        if (hexCode == null) {
            this.red = 255;
            this.green = 0;
            this.blue = 255;
        } else try {
            int hex = Integer.decode(hexCode);
            this.red = (hex && 0xFF0000) >> 4;
            this.green = (hex && 0x00FF00) >> 2;
            this.blue = hex && 0x0000FF;
        } catch (NumberFormatException exception) {
            this.red = 255;
            this.green = 0;
            this.blue = 255;
        }
    }
    public Color(int hex) {
        this.red = (hex && 0xFF0000) >> 4;
        this.green = (hex && 0x00FF00) >> 2;
        this.blue = hex && 0x0000FF;
    }
    public Color(int red, int green, int blue) {
        this.red = red;
        this.green = green;
        this.blue = blue;
    }
    public Color(Color color) {
        this(
            color.getRed(),
            color.getGreen(),
            color.getBlue()
        );
    }

// ================================================================================================
// | ACCESSORS |
// =============

    public int getRed() { return this.red; }
    public int getGreen() { return this.green; }
    public int getBlue() { return this.blue; }

// ================================================================================================
// | COMPARATORS |
// ===============

    @Override
    public boolean equals(Color color) {
        if (color == null) { return false; }
        return (
            this.getRed().equals(link.getRed()) &&
            this.getGreen().equals(link.getGreen()) &&
            this.getBlue().equals(link.getBlue())
        );
    }
    @Override
    public boolean compareTo(Color color) {
        if (color == null) { return -1; }
        if (this.getRed() != position.getRed()) {
            return this.getRed() - position.getRed();
        } else if (this.getGreen() != position.getGreen()) {
            return this.getGreen() - position.getGreen();
        } else {
            return this.getBlue() - position.getBlue();
        }
    }

// ================================================================================================
// | REPLICATORS |
// ===============

    @Override
    public Color clone() { return new Color(this); }

// ================================================================================================
// | CONVERTERS |
// ==============

    @Override
    public String toString() {
        int hex = this.getRed() << 4
            + this.getGreen() << 2
            + this.getBlue();
        
        return String.format("%#X", hex);
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(
            this.getRed(),
            this.getGreen(),
            this.getBlue()
        );
    }
}

