#include "GlBatch.h"
#include <windows.h>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "gl\glew.h"
#include "gl\wglew.h"


SHAKURAS_BEGIN;


unsigned int GetGlPrimtiveType(short cat) {
	unsigned int primtype = GlVAO::kNil;
	switch (cat) {
	case GlVAO::kPoints:
		primtype = GL_POINTS;
		break;
	case GlVAO::kLines:
		primtype = GL_LINES;
		break;
	case GlVAO::kLineLoop:
		primtype = GL_LINE_LOOP;
		break;
	case GlVAO::kLineStrip:
		primtype = GL_LINE_STRIP;
		break;
	case GlVAO::kTriangles:
		primtype = GL_TRIANGLES;
		break;
	case GlVAO::kTriangleStrip:
		primtype = GL_TRIANGLE_STRIP;
		break;
	case GlVAO::kTriangleFan:
		primtype = GL_TRIANGLE_FAN;
		break;
	case GlVAO::kQuads:
		primtype = GL_QUADS;
		break;
	case GlVAO::kQuadStrip:
		primtype = GL_QUAD_STRIP;
		break;
	case GlVAO::kPolygon:
		primtype = GL_POLYGON;
		break;
	default:
		break;
	}

	return primtype;
}


GlVAO::GlVAO() {
	vao_ = 0;
	primtype_ = 0;
	vertcount_ = 0;
	index_buffer_ = 0;
	isstatic_ = false;
}


GlVAO::~GlVAO() {
	for (auto i = attrib_buffers_.begin(); i != attrib_buffers_.end(); i++) {
		if (*i != 0) {
			glDeleteBuffers(1, &(*i));
		}
	}

	if (index_buffer_ != 0) {
		glDeleteBuffers(1, &index_buffer_);
	}

	glDeleteVertexArrays(1, &vao_);
}


void GlVAO::begin(short cat, uint16_t vert_count, int attrib_count, bool is_static) {
	primtype_ = GetGlPrimtiveType(cat);
	vertcount_ = vert_count;
	isstatic_ = is_static;

	attrib_buffers_.resize(attrib_count, 0);
	attib_sizebyfloats_.resize(attrib_count, 0);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
}


void GlVAO::setIndexBuffer(const uint16_t* buffer, int len) {
	glGenBuffers(1, &index_buffer_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * len, buffer, isstatic_? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}


void GlVAO::setAttribBuffer(const float* buffer, int index, int sizebyfloat) {
	glGenBuffers(1, &attrib_buffers_[index]);
	glBindBuffer(GL_ARRAY_BUFFER, index_buffer_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizebyfloat * vertcount_, buffer, isstatic_ ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

	attib_sizebyfloats_[index] = sizebyfloat;
}


void GlVAO::end() {
	auto bind_attrib_pointer = [&](size_t i) {
		unsigned int attrib_buffer = attrib_buffers_[i];
		int sizebyfloat = attib_sizebyfloats_[i];

		if (attrib_buffer != 0 && sizebyfloat != 0) {
			glEnableVertexAttribArray((GLuint)i);
			glBindBuffer(GL_ARRAY_BUFFER, attrib_buffer);
			glVertexAttribPointer((GLuint)i, sizebyfloat, GL_FLOAT, GL_FALSE, 0, 0);
		}
	};

	glBindVertexArray(vao_);

	for (size_t i = 0; i != attrib_buffers_.size(); i++) {
		bind_attrib_pointer(i);
	}

	glBindVertexArray(0);
}


void GlVAO::draw() {
	glBindVertexArray(vao_);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);

	glDrawElements(
		primtype_,
		vertcount_,
		GL_UNSIGNED_SHORT,
		(void*)0
	);

	glBindVertexArray(0);
}


GlVAOFactory::GlVAOFactory() {
	reset();
}


void GlVAOFactory::reset() {
	cat_ = GlVAO::kNil;
	attrib_count_ = 0;
	vertex_count_ = 0;
	is_static_ = false;
	attib_sizebyfloats_.clear();
	attribs_.clear();
}


void GlVAOFactory::setPrimtiveCat(short cat) {
	cat_ = cat;
}


void GlVAOFactory::setAttribCount(int attrib_count) {
	attrib_count_ = attrib_count;

	attib_sizebyfloats_.resize(attrib_count, 0);
	attribs_.resize(attrib_count);
}


void GlVAOFactory::setStatic(bool is_static) {
	is_static_ = is_static;
}


void GlVAOFactory::addIndex(uint16_t vi) {
	index_.push_back(vi);
}


void GlVAOFactory::addVertexAttrib1f(int index, float val) {
	attib_sizebyfloats_[index] = 1;
	attribs_[index].push_back(val);
}


void GlVAOFactory::addVertexAttrib2f(int index, float val1, float val2) {
	attib_sizebyfloats_[index] = 2;
	attribs_[index].push_back(val1);
	attribs_[index].push_back(val2);
}


void GlVAOFactory::addVertexAttrib2fv(int index, const float* val) {
	addVertexAttrib2f(index, val[0], val[1]);
}


void GlVAOFactory::addVertexAttrib3f(int index, float val1, float val2, float val3) {
	attib_sizebyfloats_[index] = 3;
	attribs_[index].push_back(val1);
	attribs_[index].push_back(val2);
	attribs_[index].push_back(val3);
}


void GlVAOFactory::addVertexAttrib3fv(int index, const float* val) {
	addVertexAttrib3f(index, val[0], val[1], val[2]);
}


void GlVAOFactory::addVertexAttrib4f(int index, float val1, float val2, float val3, float val4) {
	attib_sizebyfloats_[index] = 4;
	attribs_[index].push_back(val1);
	attribs_[index].push_back(val2);
	attribs_[index].push_back(val3);
	attribs_[index].push_back(val4);
}


void GlVAOFactory::addVertexAttrib4fv(int index, const float* val) {
	addVertexAttrib4f(index, val[0], val[1], val[2], val[3]);
}


uint16_t GlVAOFactory::attribCount(int index) const {
	return (uint16_t)(attribs_[index].size() / attib_sizebyfloats_[index]);
}


uint16_t GlVAOFactory::vertCount() const {
	if (attrib_count_ == 0) {
		return 0;
	}

	uint16_t count = attribCount(0);
	for (int i = 1; i < attrib_count_; i++) {
		if (attribCount(i) != count) {
			return 0;
		}
	}

	return count;
}


GlVAOPtr GlVAOFactory::createVAO() {
	GlVAOPtr vao = std::make_shared<GlVAO>();

	vao->begin(cat_, vertex_count_, attrib_count_, is_static_);

	vao->setIndexBuffer(index_.data(), (int)index_.size());

	for (int i = 0; i != attrib_count_; i++) {
		vao->setAttribBuffer(attribs_[i].data(), i, attib_sizebyfloats_[i]);
	}

	vao->end();

	return vao;
}


SHAKURAS_END;