#pragma once
#include <vector>


struct Column
{
	float height;
	float velocity;
	float next_height;
	float moved;

	Column()
		: height(0.0f)
		, velocity(0.0f)
		, next_height(0.0f)
		, moved(0.0f)
	{

	}

	float getHeight() const
	{
		return height + moved;
	}
};

struct Object
{
	float x, y;
	float mass;
	float vx, vy;
	float radius;

	Object()
		: x(0.0f)
		, y(0.0f)
		, mass(1.0f)
		, vx(0.0f)
		, vy(0.0f)
		, radius(0.0f)
	{}

	void update(float dt)
	{
		const float gravity = 10.0f;
		vx += gravity * dt;
		vy += gravity * dt;

		x += vx * dt;
		y += vy * dt;
	}
};

struct Domain
{
	float width;
	float vel;
	std::vector<Column> columns;
	std::vector<Object> objects;

	Domain(uint32_t count, float column_width, float velocity)
		: width(column_width)
		, vel(velocity)
	{
		columns.resize(count);
		for (Column& c : columns) {
			c.height = 0.0f;
		}
	}

	float getHeight(uint32_t i) const
	{
		return columns[i].getHeight();
	}

	void checkObjectsCollisions()
	{
		int32_t i = 0;
		float x = width * 0.5f;
		for (Column& c : columns) {
			for (Object& o : objects) {
				const float dx = std::abs(x - o.x);
				// If the object overlaps the column
				if (dx < o.radius) {
					// Compute the immerged height
					const float height = sqrt(o.radius * o.radius - dx * dx);
					const float bottom = o.y + height;
					const float dy = bottom - (900 - c.height);
					const float moved = std::min(std::max(dy, 0.0f), 2.0f * height);
					if (i > 0 && i < columns.size() - 1) {
						columns[i + 1].height += moved * 0.5f;
						columns[i - 1].height += moved * 0.5f;
					}
					columns[i].height -= moved;
				}
				else {
					c.moved = 0.0f;
				}
			}

			++i;
			x += width;
		}
	}

	void update(float dt)
	{
		checkObjectsCollisions();

		const int64_t count = columns.size() - 1;
		const float v2 = vel * vel;
		const float inv_width = 1.0f / (width * width);

		const float k = 0.50f;
		
		for (int64_t i(1); i < count; ++i) {
			float f = v2 * (getHeight(i - 1) + getHeight(i + 1) - 2.0f * getHeight(i)) * inv_width;
			f -= k * columns[i].velocity;
			columns[i].velocity = columns[i].velocity + f * dt;
		}

		// First column
		{
			// Should be OK if more than 1 columns
			float f = v2 * (columns[0].height + columns[1].height - 2.0f * columns[0].height) * inv_width;
			f -= k * columns[0].velocity;
			columns[0].velocity = columns[0].velocity + f * dt;
		}

		// Last column
		{
			// Should be OK if more than 1 columns
			float f = v2 * (columns[count-1].height + columns[count].height - 2.0f * columns[count].height) * inv_width;
			f -= k * columns[count].velocity;
			columns[count].velocity = columns[count].velocity + f * dt;
		}

		for (Column& c : columns) {
			c.height += c.velocity * dt;
		}
	}
};

