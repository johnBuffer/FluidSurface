#pragma once
#include <vector>
#include <list>


struct Column
{
	float height;
	float velocity;
	float next_height;
	float moved;
	float f;

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

	void setForce(float force)
	{
		const float k = .50f;
		f += force;
		f -= velocity * k;
	}

	void updateVelocity(float dt)
	{
		velocity += f * dt;
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
		const float gravity = 2500.0f;
		vy += gravity * dt;

		x += vx * dt;
		y += vy * dt;
	}
};

struct Particle
{
	sf::Vector2f position;
	sf::Vector2f velocity;
	float life_time;
	float current_time;

	Particle()
		: position(0.0f, 0.0f)
		, velocity(0.0f, 0.0f)
		, life_time(0.0f)
		, current_time(0.0f)
	{

	}

	Particle(float life)
		: position(0.0f, 0.0f)
		, velocity(0.0f, 0.0f)
		, life_time(life)
		, current_time(life)
	{

	}

	float getRatio() const
	{
		return current_time / life_time;
	}

	bool isDone() const
	{
		return current_time < 0.0f;
	}

	void update(float dt)
	{
		const float gravity = 2500.0f;
		velocity += sf::Vector2f(0.0f, gravity * dt);
		position += velocity * dt;
		current_time -= dt;
	}
};

struct Domain
{
	float width;
	float vel;
	std::vector<Column> columns;
	std::vector<Object> objects;
	std::list<Particle> particles;

	Domain(uint32_t count, float column_width, float velocity)
		: width(column_width)
		, vel(velocity)
	{
		columns.resize(count);
		for (Column& c : columns) {
			c.height = 500.0f;
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
					const float dy = bottom - (1080.0f - c.height);
					const float moved = std::min(std::max(dy, 0.0f), 2.0f * height);
					c.moved = moved;
					o.vy -= moved / float(columns.size()) * width * 0.35f;

					if (i > 0 && i < columns.size() - 1 && moved > 0.0f) {
						const float vh = (c.height - columns[i - 1].height) + (c.height - columns[i + 1].height);
						o.x += vh * 0.008f * width;
					}
				}
			}

			++i;
			x += width;
		}
	}

	void update(float dt)
	{
		const int64_t count = columns.size() - 1;
		const float v2 = vel * vel;
		const float inv_width = 1.0f / (width * width);

		for (Particle& p : particles) {
			p.update(dt);
		}
		particles.remove_if([&](const Particle& p) {return p.isDone(); });

		for (Object& o : objects) {
			o.update(dt);
		}

		for (Column& c : columns) {
			c.f = 0.0f;
			c.moved = 0.0f;
		}

		checkObjectsCollisions();
		
		for (int64_t i(1); i < count; ++i) {
			const float f = v2 * (getHeight(i - 1) + getHeight(i + 1) - 2.0f * getHeight(i)) * inv_width;
			columns[i].setForce(f);
			columns[i].updateVelocity(dt);
		}

		// First column
		{
			// Should be OK if more than 1 columns
			const float f = v2 * (columns[0].height + columns[1].height - 2.0f * columns[0].height) * inv_width;
			columns[0].setForce(f);
			columns[0].updateVelocity(dt);
		}

		// Last column
		{
			// Should be OK if more than 1 columns
			float f = v2 * (columns[count-1].height + columns[count].height - 2.0f * columns[count].height) * inv_width;
			columns[count].setForce(f);
			columns[count].updateVelocity(dt);
		}

		for (Column& c : columns) {
			c.height += c.velocity * dt;
		}
	}
};

