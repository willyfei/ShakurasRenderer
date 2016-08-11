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
	
	void begin(short cat, unsigned int vert_count, int attrib_count);

	void setIndexBuffer(const unsigned int* buffer, int len);

	void setAttribBuffer(const float* buffer, int index, int size);

	void end();

	virtual void draw();

private:
	unsigned int vao_;
	unsigned int primtype_;
	unsigned int vertcount_;
	int indexcount_;
	unsigned int index_buffer_;
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
	void registerAttrib(int index, int size_by_float);

	//顶点索引
	void addIndex(unsigned int vi);

	//添加顶点
	unsigned int addVertex();

	void setAttrib1f(int index, float val);
	void setAttrib2f(int index, float val1, float val2);
	void setAttrib2fv(int index, const float* val);
	void setAttrib3f(int index, float val1, float val2, float val3);
	void setAttrib3fv(int index, const float* val);
	void setAttrib4f(int index, float val1, float val2, float val3, float val4);
	void setAttrib4fv(int index, const float* val);

	//顶点数，如果属性未对齐，返回0
	unsigned int vertCount() const;

	//生成VAO
	GlVAOPtr createVAO();

private:
	short cat_;
	unsigned int vert_count_;
	std::vector<unsigned int> index_;
	std::vector<int> sizebyfloats_;
	std::vector<std::vector<float> > attribs_;
};


SHAKURAS_END;


#pragma warning(pop) 