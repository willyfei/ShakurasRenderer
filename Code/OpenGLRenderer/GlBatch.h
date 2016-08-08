#pragma once
#include "Core/Utility.h"
#include <vector>


SHAKURAS_BEGIN;


//封装一个Vertex Array Object
//析构时释放受控的gl对象
class GlBatch {
public:
	GlBatch();
	~GlBatch();

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

	void setAttribBuffer1f(const float* buffer, int index);
	void setAttribBuffer2f(const float* buffer, int index);
	void setAttribBuffer3f(const float* buffer, int index);
	void setAttribBuffer4f(const float* buffer, int index);

	void end();

	void draw();

private:
	void attribGrow(int index);

	void setAttribBuffer(const float* buffer, int index, int size);

private:
	unsigned int vao_;
	unsigned int primtype_;
	bool isstatic_;
	int vertcount_;
	unsigned int index_buffer_;
	std::vector<int> attib_sizebyfloats_;
	std::vector<unsigned int> attrib_buffers_;
};


SHAKURAS_SHARED_PTR(GlBatch);


SHAKURAS_END;