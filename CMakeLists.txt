cmake_minimum_required(VERSION 3.23)
project(Coursework)

set(CMAKE_CXX_STANDARD 14)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/teapot_smaller.ply
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(
        Coursework
        main_lab1.cpp
        main_lab2.cpp
        main_lab34.cpp
        framebuffer.cpp framebuffer.h
        linedrawer.cpp linedrawer.h
        polymesh_object.cpp polymesh_object.h
        hit.cpp hit.h
        transform.h
        vertex.h
        vector.h
        ray.h
        object.h
        material.h
        colour.h
        camera.h
        compound_material.cpp compound_material.h
        directional_light.cpp directional_light.h
        environment.h
        falsecolour_material.cpp falsecolour_material.h
        full_camera.cpp full_camera.h
        light.h
        phong_material.cpp phong_material.h
        plane_object.cpp plane_object.h
        scene.cpp scene.h
        simple_camera.cpp simple_camera.h
        sphere_object.cpp sphere_object.h
)
