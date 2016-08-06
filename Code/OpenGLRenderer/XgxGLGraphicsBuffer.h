#ifndef XGISX_SRC_XGXGRAPHICS_XGXGLGRAPHICSBUFFER_H_
#define XGISX_SRC_XGXGRAPHICS_XGXGLGRAPHICSBUFFER_H_

#include "XgxGraphicsExport.h"
#include "XgxGraphicsInterface.h"
#include <windows.h>


namespace agf { class graphics; }

class XGRPC_DLL XgxGLGraphicsBuffer
	: public XgxGraphicsInterface::XGraphics
{
public:
	XgxGLGraphicsBuffer(int width, int height);
	virtual ~XgxGLGraphicsBuffer();

public:
	bool init(int width, int height);
	bool valid() const;
	void uninit();

	virtual void display(void* hdc);
	virtual void displayStretch(void* hdc, const xrectn& rc);
	virtual void clear(xrgba_t color);
	virtual xuint32_t width() const;
	virtual xuint32_t height() const;
	virtual void draw(const xgraphic_statesrc& statesrc);

	bool saveBmp(std::string filename);
	bool savePng(std::string filename);
	bool isEmpty(xrgba_t backcolor);
	XgxGLGraphicsBuffer* clone();

private:
	std::unique_ptr<agf::graphics> grpc_;
	HDC		hDC_;
	HDC		hMemDC_;
	HGLRC	hMemRC_;
	HBITMAP hMemBitmap_;
	xrgba_t clearcolor_;
};

#endif //XGISX_SRC_XGXGRAPHICS_XGXGLGRAPHICSBUFFER_H_