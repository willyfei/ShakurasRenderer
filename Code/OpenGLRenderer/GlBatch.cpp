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
	indexcount_ = 0;
	index_buffer_ = 0;
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


void GlVAO::begin(short cat, uint16_t vert_count, int attrib_count) {
	primtype_ = GetGlPrimtiveType(cat);
	vertcount_ = vert_count;

	attrib_buffers_.clear();
	attrib_buffers_.resize(attrib_count, 0);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
}


void GlVAO::setIndexBuffer(const uint16_t* buffer, int len) {
	glGenBuffers(1, &index_buffer_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * len, buffer, GL_DYNAMIC_DRAW);
	indexcount_ = len;
}


void GlVAO::setAttribBuffer(const float* buffer, int index, int sizebyfloat) {
	glGenBuffers(1, &attrib_buffers_[index]);
	glBindBuffer(GL_ARRAY_BUFFER, attrib_buffers_[index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizebyfloat * vertcount_, buffer, GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer((GLuint)index, sizebyfloat, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray((GLuint)index);
}


void GlVAO::end() {
	glBindVertexArray(0);
}


void GlVAO::draw() {
	glBindVertexArray(vao_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
	glDrawElements(primtype_, indexcount_, GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);
}


GlVAOFactory::GlVAOFactory() {
	reset();
}


void GlVAOFactory::reset() {
	cat_ = GlVAO::kNil;
	vert_count_ = 0;
	sizebyfloats_.clear();
	attribs_.clear();
}


void GlVAOFactory::setPrimtiveCat(short cat) {
	cat_ = cat;
}


void GlVAOFactory::registerAttrib(int index, int size_by_float) {
	attribs_.resize(index + 1);
	sizebyfloats_.resize(index + 1, 0);
	sizebyfloats_[index] = size_by_float;
}


void GlVAOFactory::addIndex(uint16_t vi) {
	index_.push_back(vi);
}


uint16_t GlVAOFactory::addVertex() {
	uint16_t vert_index = vert_count_;
	vert_count_++;

	static const float initer[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	for (size_t i = 0; i != attribs_.size(); i++) {
		attribs_[i].insert(attribs_[i].end(), initer, initer + sizebyfloats_[i]);
	}

	return vert_index;
}


void GlVAOFactory::setAttrib1f(int index, float val) {
	int sbf = sizebyfloats_[index];
	if (sbf == 1) {
		uint16_t vert_index = vert_count_ - 1;

		attribs_[index][vert_index * sbf] = val;
	}
}


void GlVAOFactory::setAttrib2f(int index, float val1, float val2) {
	int sbf = sizebyfloats_[index];
	if (sbf == 2) {
		int sbf = sizebyfloats_[index];
		uint16_t vert_index = vert_count_ - 1;

		attribs_[index][vert_index * sbf] = val1;
		attribs_[index][vert_index * sbf + 1] = val2;
	}
}


void GlVAOFactory::setAttrib2fv(int index, const float* val) {
	setAttrib2f(index, val[0], val[1]);
}


void GlVAOFactory::setAttrib3f(int index, float val1, float val2, float val3) {
	int sbf = sizebyfloats_[index];
	if (sbf == 3) {
		int sbf = sizebyfloats_[index];
		uint16_t vert_index = vert_count_ - 1;

		attribs_[index][vert_index * sbf] = val1;
		attribs_[index][vert_index * sbf + 1] = val2;
		attribs_[index][vert_index * sbf + 2] = val3;
	}
}


void GlVAOFactory::setAttrib3fv(int index, const float* val) {
	setAttrib3f(index, val[0], val[1], val[2]);
}


void GlVAOFactory::setAttrib4f(int index, float val1, float val2, float val3, float val4) {
	int sbf = sizebyfloats_[index];
	if (sbf == 4) {
		int sbf = sizebyfloats_[index];
		uint16_t vert_index = vert_count_ - 1;

		attribs_[index][vert_index * sbf] = val1;
		attribs_[index][vert_index * sbf + 1] = val2;
		attribs_[index][vert_index * sbf + 2] = val3;
		attribs_[index][vert_index * sbf + 3] = val4;
	}
}


void GlVAOFactory::setAttrib4fv(int index, const float* val) {
	setAttrib4f(index, val[0], val[1], val[2], val[3]);
} 


uint16_t GlVAOFactory::vertCount() const {
	return vert_count_;
}


GlVAOPtr GlVAOFactory::createVAO() {
	GlVAOPtr vao = std::make_shared<GlVAO>();

	vao->begin(cat_, vertCount(), (int)attribs_.size());

	for (int i = 0; i != (int)attribs_.size(); i++) {
		vao->setAttribBuffer(attribs_[i].data(), i, sizebyfloats_[i]);
	}

	vao->setIndexBuffer(index_.data(), (int)index_.size());

	vao->end();

	return vao;
}


SHAKURAS_END;