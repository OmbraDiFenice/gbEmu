#pragma once

class ObjectAttributeMemoryElement {
   public:
    enum Flag {
        priority = 7,
        yFlip = 6,
        xFlip = 5,
        paletteNumber = 4
    };

   public:
    inline float getY() const { return _y; }
    inline float getX() const { return _x; }
    inline unsigned int getPatternIndex() const { return _patternIndex; }
    inline bool getFlag(Flag flag) const { return (_flags & (1 << flag)) != 0; }

   private:
    unsigned char _y;
    unsigned char _x;
    unsigned char _patternIndex;
    unsigned char _flags;
};

using ObjectAttributeMemory = ObjectAttributeMemoryElement*;