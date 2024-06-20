#ifndef LIGHT_DEMO_H
#define LIGHT_DEMO_H

#include "core/reference.h"

class Matrix : public Reference {
    GDCLASS(Matrix, Reference);
    
protected:
    static void _bind_methods();

public:
    Array array;
    Rect2 region;
    
    void set_array(Array p_array);
    Array get_array();
    void set_region(Rect2 _region);
    Rect2 get_region();
    
    void set_value(int value, int x, int y);
    int get_value(int x, int y);

    Matrix();
};

#endif // LIGHT_DEMO_H

