#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "vec3.hpp"

class ray{
    public :
        point3 origin;
        vec3 direction;
        double tm;

        ray() {}
        ray(const point3& orig, const vec3& dir, double time = 0.0) : origin(orig), direction(dir), tm(time) {}

        point3 get_origin() const {return origin;}
        vec3 get_direction() const {return direction;}
        double get_time() const {return tm;}

        point3 at(double t) const {
            return origin + direction*t;
        }

        

};

#endif