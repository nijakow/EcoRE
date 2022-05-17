#ifndef ECO_CORE_GC_TAGS_H
#define ECO_CORE_GC_TAGS_H

enum Eco_GC_Tag
{
    Eco_GC_Tag_WHITE = 0x00,
    Eco_GC_Tag_GRAY  = 0x01,
    Eco_GC_Tag_MOVED = 0x02,
    Eco_GC_Tag_BLACK = 0x03
};

#endif
