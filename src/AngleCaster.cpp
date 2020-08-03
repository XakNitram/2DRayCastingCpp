#include "AngleCaster.h"

static constexpr float M_PI = 3.14159265358979323846f;
static constexpr float M_TAU = M_PI * 2.0f;

static const unsigned int numRays = 64;

AngleCaster::AngleCaster(float x, float y): pos(x, y), vao(true) {
	rays.reserve(numRays);

	float positions[2 * (numRays + 1)];
	unsigned int indices[2 * numRays];

	positions[0] = pos.x;
	positions[1] = pos.y;

	const float slice = M_TAU / float(numRays);
	for (unsigned int i = 0; i < numRays; i++) {
		rays.emplace_back(pos.x, pos.y, float(i) * slice);

		positions[(i + 1) * 2 + 0] = 0.0f;
		positions[(i + 1) * 2 + 1] = 0.0f;

		indices[i * 2 + 0] = 0;
		indices[i * 2 + 1] = i + 1;
	}

	// Construct array buffer.
	vao.constructArrayBuffer(2 * (numRays + 1) * sizeof(float), positions, GL_DYNAMIC_DRAW);
	vao.attachAttribute(2, GL_FLOAT, 2 * sizeof(float));

	// Construct index buffer.
	vao.constructIndexBuffer(2 * numRays * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
}

void AngleCaster::update(const float x, const float y) {
	pos.x = x;
	pos.y = y;

	float positions[2] = { x, y };

	for (unsigned int i = 0; i < numRays; i++) {
		Ray& ray = rays[i];
		ray.pos.x = x;
		ray.pos.y = y;
	}

	vao.setArrayData(0, 2 * sizeof(float), positions);
}

void AngleCaster::look(const std::vector<Boundary>& bounds) {
	float positions[2 * numRays];
	const unsigned int numBounds = bounds.size();

	std::vector<std::unique_ptr<Point>> intersections;
	intersections.reserve(numBounds);
	
	// Iterate over all rays to find where they intersect.
	for (unsigned int i = 0; i < numRays; i++) {
		const Ray& ray = rays[i];

		const unsigned int index = i * 2;
		pushIntersections(ray, bounds, intersections);
		if (intersections.size()) {
			std::unique_ptr<Point> shortestPath = closestIntersection(ray, intersections);

			positions[index + 0] = shortestPath->x;
			positions[index + 1] = shortestPath->y;
		}

		else {
			positions[index + 0] = pos.x;
			positions[index + 1] = pos.y;
		}
		
		intersections.clear();
	}

	vao.setArrayData(2 * sizeof(float), 2 * numRays * sizeof(float), positions);
}

void AngleCaster::draw() const {
	vao.drawElements(GL_LINES, 2 * numRays, GL_UNSIGNED_INT);
}


// Filled AngleCaster
FilledAngleCaster::FilledAngleCaster(float x, float y): pos(x, y), vao(false) {
	rays.reserve(numRays);

	const unsigned int bufferSize = (numRays + 2) * 2;
	float positions[bufferSize];
	positions[0] = float(x);
	positions[1] = float(y);

	const float slice = (M_TAU / float(numRays));
	for (unsigned int i = 0; i < numRays; i++) {
		rays.emplace_back(x, y, float(i) * slice);
		positions[(i + 1) * 2 + 0] = float(x);
		positions[(i + 1) * 2 + 1] = float(y);
	}

	positions[bufferSize - 2] = positions[2];
	positions[bufferSize - 1] = positions[3];

	vao.constructArrayBuffer(bufferSize * sizeof(float), positions, GL_STATIC_DRAW);
	vao.attachAttribute(2, GL_FLOAT, 2 * sizeof(float));
}

void FilledAngleCaster::update(const float x, const float y) {
	pos.x = x;
	pos.y = y;

	for (Ray& ray : rays) {
		ray.pos.x = x;
		ray.pos.y = y;
	}

	float positions[2] = { float(x), float(y) };
	vao.setArrayData(0, 2 * sizeof(float), positions);
}

void FilledAngleCaster::look(const std::vector<Boundary>& bounds) {
	const unsigned int bufferSize = (numRays + 1) * 2;
	float positions[bufferSize];
	positions[0] = 0.0f;
	positions[1] = 0.0f;

	std::vector<std::unique_ptr<Point>> intersections;
	for (unsigned int i = 0; i < numRays; i++) {
		const Ray& ray = rays[i];

		pushIntersections(ray, bounds, intersections);
		if (intersections.size()) {
			std::unique_ptr<Point> shortestPath = closestIntersection(ray, intersections);

			const unsigned int index = i * 2;
			positions[index + 0] = shortestPath->x;
			positions[index + 1] = shortestPath->y;
		}

		intersections.clear();
	}

	positions[bufferSize - 2] = positions[0];
	positions[bufferSize - 1] = positions[1];

	vao.setArrayData(2 * sizeof(float), bufferSize * sizeof(float), positions);
}

void FilledAngleCaster::draw() const {
	vao.drawArrays(GL_TRIANGLE_FAN, numRays + 2);
}
