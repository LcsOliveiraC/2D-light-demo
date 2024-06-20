#include "matrix.h"

// 2D matrix/grid class

void Matrix::set_array(Array p_array) {
    array = p_array;
}

Array Matrix::get_array() {
    return array;
}

void Matrix::set_region(Rect2 _region) {
    region = _region;
    while (array.size() > int(region.size.x) * int(region.size.y)) {
        array.remove(array.size());
    }
	while (array.size() < int(region.size.x) * int(region.size.y)) {
        array.append(0);
    }
}

Rect2 Matrix::get_region() {
    return region;
}

void Matrix::set_value(int value, int x, int y) {
    array[(x - region.position.x) * region.size.y + y - region.position.y] = value;
}

int Matrix::get_value(int x, int y) {
    return array[(x - region.position.x) * region.size.y + y - region.position.y];
}

void Matrix::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_array", "array"), &Matrix::set_array);
    ClassDB::bind_method(D_METHOD("get_array"), &Matrix::get_array);
    ClassDB::bind_method(D_METHOD("set_region", "region"), &Matrix::set_region);
    ClassDB::bind_method(D_METHOD("get_region"), &Matrix::get_region);
    ClassDB::bind_method(D_METHOD("set_value", "value", "x", "y"), &Matrix::set_value);
    ClassDB::bind_method(D_METHOD("get_value", "x", "y"), &Matrix::get_value);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "array"), "set_array", "get_array");
    ADD_PROPERTY(PropertyInfo(Variant::RECT2, "region"), "set_region", "get_region");
}

Matrix::Matrix() {
}