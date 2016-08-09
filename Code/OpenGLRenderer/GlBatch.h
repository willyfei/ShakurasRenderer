#pragma once
#include "GlRendererDll.h"
#include "Core/Utility.h"
#include <vector>

#pragma warning(push) 
#pragma warning(disable:4251)


SHAKURAS_BEGIN;


class GLRENDERER_DLL GlBatch {
public:
	virtual ~GlBatch() {}
	virtual void draw() = 0;
};


SHAKURAS_SHARED_PTR(GlBatch);


//封装一个Vertex Array Object
//析构时释放受控的gl对象
class GLRENDERER_DLL GlVAO : public GlBatch {
public:
	GlVAO();
	virtual ~GlVAO();

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
	//...
	//draw()

	void begin(short cat, uint16_t vert_count, int attrib_count, bool is_static);

	void setIndexBuffer(const uint16_t* buffer, int len);

	void setAttribBuffer(const float* buffer, int index, int size);

	void end();

	virtual void draw();

private:
	unsigned int vao_;
	unsigned int primtype_;
	bool isstatic_;
	uint16_t vertcount_;
	unsigned int index_buffer_;
	std::vector<int> attib_sizebyfloats_;
	std::vector<unsigned int> attrib_buffers_;
};


SHAKURAS_SHARED_PTR(GlVAO);


class GLRENDERER_DLL GlVAOFactory {
public:
	GlVAOFactory();

public:
	void reset();

	//图元类别
	void setPrimtiveCat(short cat);

	//属性数
	void setAttribCount(int attrib_count);

	//静态图元
	void setStatic(bool is_static);

	//顶点索引
	void addIndex(uint16_t vi);

	//顶点属性
	void addVertexAttrib1f(int index, float val);
	void addVertexAttrib2f(int index, float val1, float val2);
	void addVertexAttrib2fv(int index, const float* val);
	void addVertexAttrib3f(int index, float val1, float val2, float val3);
	void addVertexAttrib3fv(int index, const float* val);
	void addVertexAttrib4f(int index, float val1, float val2, float val3, float val4);
	void addVertexAttrib4fv(int index, const float* val);

	//属性数
	uint16_t attribCount(int index) const;

	//顶点数，如果属性未对齐，返回0
	uint16_t vertCount() const;

	//生成VAO
	GlVAOPtr createVAO();

private:
	short cat_;
	int attrib_count_;
	uint16_t vertex_count_;
	bool is_static_;
	std::vector<uint16_t> index_;
	std::vector<int> attib_sizebyfloats_;
	std::vector<std::vector<float> > attribs_;
};


SHAKURAS_END;


#pragma warning(pop) 