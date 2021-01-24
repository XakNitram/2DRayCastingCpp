#pragma once

#include "pch.h"
#include "Renderer/VertexArray.h"
#include "Geometrics.h"

struct Boundary {
	LineSegment line;
	VertexArray vao;

	// TODO: If needed, add a default constructor and an 'update' method
	Boundary(float x1, float y1, float x2, float y2);
	Boundary(Boundary&& other) noexcept;

	void draw() const;
};
