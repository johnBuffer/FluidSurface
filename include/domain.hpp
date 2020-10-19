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
		columns.resize(width);
	}

	void update(float dt)
	{
		const int64_t count = columns.size() - 1;
		const float v2 = vel * vel;
		const float inv_width = 1.0f / width;
		for (int64_t i(1); i < count; ++i) {
			const float f = v2 * (columns[i - 1].height + columns[i + 1].height - 2.0f * columns[i].height) * inv_width;
			columns[i].velocity = columns[i].velocity + f * dt;
			columns[i].next_height = columns[i].height + columns[i].velocity * dt;
		}

		// First column
		{
			// Should be OK if more than 1 columns
			const float f = v2 * (columns[0].height + columns[1].height - 2.0f * columns[0].height) * inv_width;
			columns[0].velocity = columns[0].velocity + f * dt;
			columns[0].next_height = columns[0].height + columns[0].velocity * dt;
		}

		// Last column
		{
			// Should be OK if more than 1 columns
			const float f = v2 * (columns[count-1].height + columns[count].height - 2.0f * columns[count].height) * inv_width;
			columns[count].velocity = columns[count].velocity + f * dt;
			columns[count].next_height = columns[count].height + columns[count].velocity * dt;
		}

		for (Column& c : columns) {
			c.height = c.next_height;
		}
	}
};

