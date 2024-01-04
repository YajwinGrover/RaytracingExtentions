#pragma once

#include "hittable_list.hpp"
#include "material.hpp"
#include "triangle.hpp"
#include "vec3.hpp"
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"
#include <string>

class ModelLoader{
    private:
        std::string fileName_;
        std::string materialSearchPath_;

    public: 
        ModelLoader(std::string fileName, std::string materialSearchPath) : fileName_(fileName), materialSearchPath_(materialSearchPath){}

        hittable_list load(std::shared_ptr<material> mat);
};


hittable_list ModelLoader::load(std::shared_ptr<material> mat){
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = materialSearchPath_;

    tinyobj::ObjReader reader;
    

    if(!reader.ParseFromFile(fileName_, reader_config)){
        if(!reader.Error().empty()){
           throw std::runtime_error("TinyObj error" + reader.Error());
        }
        throw std::runtime_error("Failed to load 3d model");
    }

    if(!reader.Warning().empty()){
        throw std::runtime_error("TinyObj warning" + reader.Warning());
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    std::cerr << "Number of triangles " << (attrib.vertices.size() / 9) << std::endl;

    //Remove eventually
    //std::cout << "Model loading was success" << std::endl;
    std::vector<std::shared_ptr<triangle> > triList;
    for(size_t s = 0; s < shapes.size(); s++){
        size_t index_offset = 0;

        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++){
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            vec3 verts[3];
            color colors[3];
            for(size_t v = 0; v < fv; v++){
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index) + 2];

                tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

                verts[v] = vec3(vx,vy,vz);
                colors[v] = color(red, green, blue);
            }
            //triList.push_back(std::make_shared<triangle>(verts[0], verts[1], verts[2], std::make_shared<lambertian>(color(colors[0].x(), colors[0].y(), colors[0].z()))));
            triList.push_back(std::make_shared<triangle>(verts[0], verts[1], verts[2], mat));
            index_offset += fv;
        }
    }

    hittable_list list;
    for(std::shared_ptr<triangle> t : triList){
        list.add(t);
    }

    return list;
}