#include "GlBatch.h"
#include <windows.h>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <gl\glew.h>
#include <gl\gl.h>


SHAKURAS_BEGIN;


unsigned int GetGlPrimtiveType(short cat) {
	unsigned int primtype = GlBatch::kNil;
	switch (cat) {
	case GlBatch::kPoints:
		primtype = GL_POINTS;
		break;
	case GlBatch::kLines:
		primtype = GL_LINES;
		break;
	case GlBatch::kLineLoop:
		primtype = GL_LINE_LOOP;
		break;
	case GlBatch::kLineStrip:
		primtype = GL_LINE_STRIP;
		break;
	case GlBatch::kTriangles:
		primtype = GL_TRIANGLES;
		break;
	case GlBatch::kTriangleStrip:
		primtype = GL_TRIANGLE_STRIP;
		break;
	case GlBatch::kTriangleFan:
		primtype = GL_TRIANGLE_FAN;
		break;
	case GlBatch::kQuads:
		primtype = GL_QUADS;
		break;
	case GlBatch::kQuadStrip:
		primtype = GL_QUAD_STRIP;
		break;
	case GlBatch::kPolygon:
		primtype = GL_POLYGON;
		break;
	default:
		break;
	}

	return primtype;
}


GlBatch::GlBatch() {
	vao_ = 0;
	primtype_ = 0;
	vertcount_ = 0;
	index_buffer_ = 0;
	isstatic_ = false;
}


void GlBatch::begin(short cat, int vert_count, bool is_static) {
	primtype_ = GetGlPrimtiveType(cat);
	vertcount_ = vert_count;
	isstatic_ = is_static;

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
}


void GlBatch::setIndexBuffer(const unsigned short* buffer, int len) {
	glGenBuffers(1, &index_buffer_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * len, buffer, isstatic_? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}


void GlBatch::setAttribBuffer(const float* buffer, int index, int sizebyfloat) {
	attribGrow(index);

	glGenBuffers(1, &attrib_buffers_.back());
	glBindBuffer(GL_ARRAY_BUFFER, index_buffer_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizebyfloat * vertcount_, buffer, isstatic_ ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

	attib_sizebyfloats_[index] = sizebyfloat;
}


void GlBatch::attribGrow(int index) {
	while (attrib_buffers_.size() < index + 1) {
		attrib_buffers_.push_back(0);
		attib_sizebyfloats_.push_back(0);
	}
}


void GlBatch::end() {
	auto bind_attrib_pointer = [&](size_t i) {
		unsigned int attrib_buffer = attrib_buffers_[i];
		int sizebyfloat = attib_sizebyfloats_[i];

		if (attrib_buffer != 0 && sizebyfloat != 0) {
			glEnableVertexAttribArray(i);
			glBindBuffer(GL_ARRAY_BUFFER, attrib_buffer);
			glVertexAttribPointer(i, sizebyfloat, GL_FLOAT, GL_FALSE, 0, 0);
		}
	};

	glBindVertexArray(vao_);

	for (size_t i = 0; i != attrib_buffers_.size(); i++) {
		bind_attrib_pointer(i);
	}

	glBindVertexArray(0);
}


void GlBatch::draw() {
	glBindVertexArray(vao_);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);

	// Draw the triangles !
	glDrawElements(
		primtype_,
		vertcount_,
		GL_UNSIGNED_SHORT,
		(void*)0
	);

	glBindVertexArray(0);
}


SHAKURAS_END;