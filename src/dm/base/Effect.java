package dm.base;

public enum Effect {

// ================================================================================================
// | CONSTANTS |
// =============

    INVALID('\0'),
    FLOOR('f'),
    WALL('w'),
    PIT('p');
    
// ================================================================================================
// | FIELDS |
// ==========

    private char symbol;

// ================================================================================================
// | CONSTRUCTORS |
// ================
    
    private Effect(char symbol) { this.symbol = symbol; }

// ================================================================================================
// | ACCESSORS |
// =============

    public char getSymbol() { return this.symbol; }

// ================================================================================================
// | PARSERS |
// ===========

    public static Effect parse(char symbol) {
        for (Effect effect : values()) {
            if (effect.symbol == symbol) {
                return effect;
            }
        }
        return INVALID;
    }
}

