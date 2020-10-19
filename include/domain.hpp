#pragma once
#include <vector>

struct Column
{
	float height;
	float velocity;
	float next_height;

	Column()
		: height(0.0f)
		, velocity(0.0f)
		, next_height(0.0f)
	{

	}
};

struct Domain
{
	float width;
	float vel;
	std::vector<Column> columns;

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
		return columns[i].height;
	}

	void update(float dt)
	{
		const int64_t count = columns.size() - 1;
		const float v2 = vel * vel;
		const float inv_width = 1.0f / (width * width);

		const float k = -0.90f;
		
		for (int64_t i(1); i < count; ++i) {
			float f = v2 * (getHeight(i - 1) + getHeight(i + 1) - 2.0f * getHeight(i)) * inv_width;
			f += k * columns[i].velocity;
			columns[i].velocity = columns[i].velocity + f * dt;
		}

		// First column
		{
			// Should be OK if more than 1 columns
			float f = v2 * (columns[0].height + columns[1].height - 2.0f * columns[0].height) * inv_width;
			f += k * columns[0].velocity;
			columns[0].velocity = columns[0].velocity + f * dt;
		}

		// Last column
		{
			// Should be OK if more than 1 columns
			float f = v2 * (columns[count-1].height + columns[count].height - 2.0f * columns[count].height) * inv_width;
			f += k * columns[count].velocity;
			columns[count].velocity = columns[count].velocity + f * dt;
		}

		for (Column& c : columns) {
			c.height += c.velocity;
		}
	}
};

