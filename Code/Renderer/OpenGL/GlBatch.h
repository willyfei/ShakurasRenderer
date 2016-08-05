#pragma once
#include "../MathAndGeometry.h"
#include <vector>


SHAKURAS_BEGIN;


//·â×°Ò»¸öVertex Array Object
class GlBatch {
public:
	GlBatch();

public:
	enum Catagory {
		kNil = 0,
		kPoints,
		kLines,
		kLineLoop,
		kLineStrip,
		kTriangles,
		kTriangleStrip,
		kTriangleFan,
		kQuads,
		kQuadStrip,
		kPolygon
	};

	//begin(...)
	//setIndexBuffer(...)
	//setAttribBuffer(...)
	//end()
	//draw()

	void begin(short cat, int vert_count, bool is_static);

	void setIndexBuffer(const unsigned short* buffer, int len);

	void setAttribBuffer(const float* buffer, int index, int size);

	void end();

	void draw();

private:
	void attribGrow(int index);

private:
	unsigned int vao_;
	unsigned int primtype_;
	bool isstatic_;
	int vertcount_;
	unsigned int index_buffer_;
	std::vector<int> attib_sizebyfloats_;
	std::vector<unsigned int> attrib_buffers_;
};


SHAKURAS_END;