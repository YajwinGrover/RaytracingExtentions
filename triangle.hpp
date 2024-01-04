#pragma once


#include "hittable.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include "aabb.hpp"
#include <algorithm>
#include <memory>
#include <cmath>
#include "utilities.hpp"


class triangle : public hittable{
    public:
        point3 v1;
        point3 v2;
        point3 v3;
        std::shared_ptr<material> mat_ptr;


        triangle(point3 _v1, point3 _v2, point3 _v3, std::shared_ptr<material> material) : v1(_v1), v2(_v2), v3(_v3), mat_ptr(material) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;


};

bool triangle::hit(const ray& r, double t_min , double t_max, hit_record& rec) const {
    // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

    //Yo no comprende mucho. Tu tienes que leer wikipedia
    const float EPSILON = 0.000001;
    vec3 edge1, edge2, h, s, q;

    float a, f, u, v;

    edge1 = v2 - v1;
    edge2 = v3 - v1;
    h = cross(r.get_direction(), edge2);
    a = dot(edge1, h);

    if(a > -EPSILON && a < EPSILON){
        return false; //Ray is parallel to triangle plane
    }

    f = 1.0 / a;
    s = r.get_origin() - v1;
    u = f * dot(s, h);

    if(u < 0.0 || u > 1.0){
        return false;
    }

    q = cross(s, edge1);
    v = f * dot(r.get_direction(), q);

    if(v < 0.0 || u + v > 1.0){
        return false;
    }

    float t = f * dot(edge2 , q);

    if( t > EPSILON){
        rec.p = r.at(t);
        rec.mat_ptr = mat_ptr;
        rec.t = t;
        vec3 norm = cross(edge1, edge2);
        
        rec.set_face_normal(r, norm / norm.length());
        rec.u = 0;
        rec.v = 0;
        return true;
    }
    else{ //Line intersection??
        return false;
    }

}

bool triangle::bounding_box(double time0, double time1, aabb &output_box) const {
    double minX = std::min(v1.x(), std::min(v2.x(), v3.x()));
    double minY = std::min(v1.y(), std::min(v2.y(), v3.y()));
    double minZ = std::min(v1.z(), std::min(v2.z(), v3.z()));

    double maxX = std::max(v1.x(), std::max(v2.x(), v3.x()));
    double maxY = std::max(v1.y(), std::max(v2.y(), v3.y()));
    double maxZ = std::max(v1.z(), std::max(v2.z(), v3.z()));

    point3 min(minX, minY, minZ);
    point3 max(maxX, maxY, maxZ);

    output_box = aabb(min, max);

    return true;
}