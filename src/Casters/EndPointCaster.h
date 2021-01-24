#pragma once
#include "pch.h"
#include "Caster.h"
#include "Math/Geometrics.h"
#include "Math/Boundary.h"
#include "Renderer/VertexArray.h"


class LineEndPointCaster : public Caster {
    Point pos;
    VertexArray vao;
    unsigned int currentRays;
    std::vector<Point> intersections;

public:
    LineEndPointCaster(unsigned int numBounds);

    void update(const float x, const float y);

    void look(const std::vector<Boundary>& bounds);

    void draw() const;
};


class FilledEndPointCaster : public Caster {
    Point pos;
    VertexArray vao;
    unsigned int currentRays;
    std::vector<Point> intersections;

public:
    FilledEndPointCaster(unsigned int numBounds);

    void update(const float x, const float y);

    void look(const std::vector<Boundary>& bounds);

    void draw() const;
};
