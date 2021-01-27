#pragma once
#include "pch.hpp"
#include "Caster.hpp"
#include "Math/Geometrics.hpp"
#include "Primitives/Boundary.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Buffer.hpp"


class LineEndPointCaster : public Caster {
    Point pos;
    lwvl::VertexArray vao;
    lwvl::ArrayBuffer vbo;
    lwvl::ElementBuffer ebo;
    unsigned int currentRays;
    std::vector<Point> intersections;

public:
    explicit LineEndPointCaster(unsigned int numBounds);

    void update(float x, float y) final;

    void look(const std::vector<Boundary>& bounds) final;

    void draw() final;
};


class FilledEndPointCaster : public Caster {
    Point pos;
    lwvl::VertexArray vao;
    lwvl::ArrayBuffer vbo;
    unsigned int currentRays;
    std::vector<Point> intersections;

public:
    explicit FilledEndPointCaster(unsigned int numBounds);

    void update(float x, float y) final;

    void look(const std::vector<Boundary>& bounds) final;

    void draw() final;
};
