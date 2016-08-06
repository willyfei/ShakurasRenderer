#include "XgxGLGraphicsBuffer.h"
#include "XgxAgfUtilities.h"
#include "..\xgisx_utilities\XgxRegionParser.h"
#include "agf\agf_text_freetype_renderer.h"
#include "poly2tri\poly2tri.h"
#include "poly2tri\common\utils.h"
#include <windowsx.h>
#include <algorithm>
#include "gl\glew.h"
#include <fstream>
#include <winuser.h>
#include "ximadef.h"


class OpenGLBinding
{
public:
	OpenGLBinding(HDC hDC, HGLRC hRC)
	{
		hDC_ = hDC;
		m_hHC = hRC;
		wglMakeCurrent(hDC_, m_hHC);
	}
	~OpenGLBinding()
	{
		wglMakeCurrent(NULL, NULL);
	}

private:
	HDC hDC_;
	HGLRC m_hHC;
};


XgxGLGraphicsBuffer::XgxGLGraphicsBuffer(int width, int height)
{
	hDC_ = 0;
	hMemDC_ = 0;
	hMemRC_ = 0;
	hMemBitmap_ = 0;
	grpc_.reset();
	clearcolor_ = xrgba(255, 255, 255, 0);
	init(width, height);
}

XgxGLGraphicsBuffer::~XgxGLGraphicsBuffer()
{
	uninit();
}

HGLRC InitOpenGLWithMemoryDC(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL| PFD_SUPPORT_GDI,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		0,0,0,0,0,0,0,
		32,
		0,0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);
	return wglCreateContext(hDC);   
}


bool XgxGLGraphicsBuffer::init( int width, int height )
{
	if (valid())
	{
		return true;
	}

	// 屏幕和内存设备描述表
	HDC hScrDC, hMemDC;  

	// 为屏幕创建设备描述表
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);

	// 为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);

	// 创建一个与屏幕设备描述表兼容的位图
	std::unique_ptr<agf::graphics> grpc(new agf::graphics);
	HBITMAP hBitmap = grpc->reset(hMemDC_, width, height);

	// 把新位图选到内存设备描述表中
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// 利用OpenGL在内存dc绘制图形
	HGLRC hMemRC = InitOpenGLWithMemoryDC(hMemDC);

	OpenGLBinding binding(hMemDC, hMemRC);
	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	grpc_.reset(grpc.release());
	hDC_ = hScrDC;
	hMemDC_ = hMemDC;
	hMemRC_ = hMemRC;
	hMemBitmap_ = hBitmap;

	return true;
}


void SetGLState()
{
	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.1, 1.0);
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);
	glDisable(GL_TEXTURE_2D);

	//透明材质
	glAlphaFunc(GL_GREATER, 0.3f);//0.5可以换成任何在0~1之间的数 
	glPolygonMode(GL_FRONT, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
}


void SetGLViewport(int width, int height)
{
	if (width == 0) { width = 1; }
	if (height == 0) { height = 1; }

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.5*width, -0.5*width, -0.5*height, 0.5*height, 100, -100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void LookGLCenter(int width, int height)
{
	gluLookAt(0.5*width, 0.5*height, 100, 0.5*width, 0.5*height, -100, 0, -1, 0);
}


void XgxGLGraphicsBuffer::uninit()
{
	if (!valid())
	{
		return;
	}

	::wglDeleteContext(hMemRC_);
	::DeleteDC(hMemDC_);
	::DeleteDC(hDC_);

	grpc_.reset();
	hMemBitmap_ = 0;
	hMemRC_ = 0;
	hMemDC_ = 0;
	hDC_ = 0;
}

bool XgxGLGraphicsBuffer::valid() const
{
	return (grpc_ && hMemRC_ != 0 && hMemDC_ != 0 && hDC_ != 0 && hMemBitmap_ != 0);
}


void XgxGLGraphicsBuffer::display(void* hdc)
{
	if (!valid())
	{
		return;
	}

	grpc_->draw((HDC)hdc);
}

void XgxGLGraphicsBuffer::displayStretch(void* hdc, const xrectn& rc)
{
	if (!valid())
	{
		return;
	}

	RECT crc;
	crc.left = rc.xmin_;
	crc.right = rc.xmax_;
	crc.top = rc.ymin_;
	crc.bottom = rc.ymax_;

	grpc_->draw((HDC)hdc, &crc);
}

void XgxGLGraphicsBuffer::clear(xrgba_t color)
{
	if (!valid())
	{
		return;
	}

	OpenGLBinding binding(hMemDC_, hMemRC_);

	//背景色
	glClearColor(xr(color)/255.0f, xg(color)/255.0f, xb(color)/255.0f, xa(color)/255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	clearcolor_ = color;
}

xuint32_t XgxGLGraphicsBuffer::width() const
{
	if (!valid())
	{
		return 0;
	}

	return grpc_->width();
}

xuint32_t XgxGLGraphicsBuffer::height() const
{
	if (!valid())
	{
		return 0;
	}

	return grpc_->height();
}


bool XgxGLGraphicsBuffer::saveBmp(std::string filename)
{
	if (!valid())
	{
		return false;
	}

	return grpc_->save_as_bmp(filename);
}

bool XgxGLGraphicsBuffer::savePng( std::string filename )
{
	if (!valid())
	{
		return false;
	}

	return grpc_->save_as_png(filename);
}

bool XgxGLGraphicsBuffer::isEmpty(xrgba_t backcolor)
{
	if (!valid())
	{
		return false;
	}

	return grpc_->empty(agf::graphics::color_t(xr(backcolor), xg(backcolor), xb(backcolor), xa(backcolor)));
}

XgxGLGraphicsBuffer* XgxGLGraphicsBuffer::clone()
{
	if (!valid())
	{
		return 0;
	}

	XgxGLGraphicsBuffer* obj = new XgxGLGraphicsBuffer(height(), width());
	if (!obj->valid())
	{
		return 0;
	}

	obj->grpc_->copy_from(grpc_.get());
	return obj;
}


void GLLineStringSolidDraw(const xgraphic_state& state, const std::list<xgraphic_shapesrc>& vertsrcs)
{
	xrgba_t color = state.color_;
	glColor4ub(xr(color), xg(color), xb(color), xa(color));
	glLineWidth(state.width_);

	for (auto i = vertsrcs.begin(); i != vertsrcs.end(); i++)
	{
		const xgraphic_shapesrc& vs = *i;
		for (auto ii = vs.begin(); ii != vs.end(); ii++)
		{
			const xvertex_element2& ve = *ii;

			glNormal3d(0,0,1);
			glBegin(GL_LINE_STRIP);

			for (size_t iii = 0; iii != ve.vertex_count(); iii++)
			{
				xvertex2 v2 = ve.vert(iii);
				glVertex2d(v2.x, v2.y);
			}

			glEnd();
		}
	}
}


std::vector<p2t::Point*> xconv(const xvertex_array2& arr)
{
	std::vector<p2t::Point*> arrp2t;
	for (size_t ii = 0; ii+1 < arr.vertscount_; ii++)
	{
		xvertex2 v = arr.vert(ii);
		arrp2t.push_back(new p2t::Point(v.x, v.y));
	}

	xvertex2 vlast = arr.vert(arr.vertscount_-1);
	if (!vlast.equals(arr.vert(0)))
	{
		arrp2t.push_back(new p2t::Point(vlast.x, vlast.y));
	}
	
	return arrp2t;
}


void GLDrawRegion(const std::vector<xvertex_array2>& paths, const std::vector<size_t>& regions)
{
	if (regions.empty())
	{
		return;
	}

	std::vector<std::vector<p2t::Point*> > contourp2t;
	contourp2t.push_back(xconv(paths[regions[0]]));
	for (auto i = std::next(regions.begin()); i != regions.end(); i++)
	{
		contourp2t.push_back(xconv(paths[*i]));
	}

	p2t::CDT cdt(contourp2t[0]);
	for (auto i = std::next(contourp2t.begin()); i != contourp2t.end(); i++)
	{
		cdt.AddHole(*i);
	}
	cdt.Triangulate();
	std::vector<p2t::Triangle*> trisp2t = cdt.GetTriangles();

	//draw hatch
	glNormal3d(0,0,1);
	glBegin(GL_TRIANGLES);
	for (auto i = trisp2t.begin(); i != trisp2t.end(); i++)
	{
		p2t::Triangle* tri = *i;
		p2t::Point* pt0 = tri->GetPoint(0);
		p2t::Point* pt1 = tri->GetPoint(1);
		p2t::Point* pt2 = tri->GetPoint(2);

		glVertex2d(pt0->x, pt0->y);
		glVertex2d(pt1->x, pt1->y);
		glVertex2d(pt2->x, pt2->y);
	}
	glEnd();

	glNormal3d(0,0,1);
	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_TRIANGLES);
	for (auto i = trisp2t.begin(); i != trisp2t.end(); i++)
	{
		p2t::Triangle* tri = *i;
		p2t::Point* pt0 = tri->GetPoint(0);
		p2t::Point* pt1 = tri->GetPoint(1);
		p2t::Point* pt2 = tri->GetPoint(2);

		glVertex2d(pt0->x, pt0->y);
		glVertex2d(pt1->x, pt1->y);
		glVertex2d(pt2->x, pt2->y);
	}
	glEnd();
	glPolygonMode(GL_FRONT, GL_FILL);

	for (auto i = contourp2t.begin(); i != contourp2t.end(); i++)
	{
		std::for_each(i->begin(), i->end(), xdelete_ptr<p2t::Point>);
	}
}


//0-a within b, 1- b within a, 2-other
int ContourOrient(const xvertex_array2& a, const xvertex_array2& b)
{
	if (a.vertscount_ < 3 || b.vertscount_ < 3)
	{
		return 2;
	}

	if (xwithin(a.vert(0), b))
	{
		return 0;
	}
	else if (xwithin(b.vert(0), a))
	{
		return 1;
	}

	return 2;
}


struct XRegionParsableContour
{
	XRegionParsableContour(xvertex_array2* verts) : verts_(verts) {}

	int within(std::shared_ptr<XRegionParsableContour> r)
	{
		return ContourOrient(*verts_, *(r->verts_));
	}

	xvertex_array2* verts_;
};


template<class VSPtr>
void GLContourFillDraw(VSPtr& contour_ptr)
{
	if (!contour_ptr)
	{
		return;
	}

	std::vector<xvertex_array2> contours;
	xread_path_agf(*contour_ptr, contours);

	std::vector<std::shared_ptr<XRegionParsableContour> > parse_contours(contours.size());
	for (size_t i = 0; i != parse_contours.size(); i++)
	{
		parse_contours[i] = std::make_shared<XRegionParsableContour>(&contours[i]);
	}

	//get regions
	std::vector<std::vector<size_t> > regions;
	XRegionParse(parse_contours, regions);

	//draw
	for (size_t i = 0; i != regions.size(); i++)
	{
		GLDrawRegion(contours, regions[i]);
	}
}


void GLTextDraw(double wpp, const xgraphic_state& state, const std::list<xgraphic_textsrc>& textsrcs, xrgba_t clearcolor)
{
	agf::graphics grp;
	agf::text_freetype_renderer ren(grp);

	//ren.color(xconvert_agf(state.color_));
	ren.font(xfont_file().c_str());
	double texth = state.texthwcs_ ? state.textheight_/wpp : state.textheight_;
	ren.text_height(texth);
	ren.quality(agf::text_freetype_renderer::kHigh);

	glColor4ub(xr(state.color_), xg(state.color_), xb(state.color_), xa(state.color_));
	for (auto pos = textsrcs.begin(); pos != textsrcs.end(); pos++)
	{
		const xgraphic_textsrc& textsrc = *pos;
		xvertex2 textlb = xgraphic_tool(state).left_bottom(textsrc, wpp);
		ren.text_rotate(textsrc.stat_.rotate_);

		auto contour_path = ren.text_contour(textlb.x, textlb.y, textsrc.text_.c_str());

		GLContourFillDraw(contour_path);
	}
}

void GLPointSolidDraw(const xgraphic_state& state, const std::list<xgraphic_shapesrc>& vertsrcs)
{
	glPointSize(2*state.size_);
	glColor4ub(xr(state.color_), xg(state.color_), xb(state.color_), xa(state.color_));
	glBegin(GL_POINTS);
	for (auto i = vertsrcs.begin(); i != vertsrcs.end(); i++)
	{
		const xgraphic_shapesrc& vs = *i;
		for (auto ii = vs.begin(); ii != vs.end(); ii++)
		{
			const xvertex_element2& ve = *ii;
			for (size_t iii = 0; iii != ve.vertscount_; iii++)
			{
				glVertex2d(ve.coords_[2*iii], ve.coords_[2*iii+1]);
			}
		}
	}
	glEnd();
}

void XgxGLGraphicsBuffer::draw(const xgraphic_statesrc& statesrc)
{
	OpenGLBinding binding(hMemDC_, hMemRC_);
	
	SetGLState();
	SetGLViewport(width(), height());

	glRenderMode(GL_RENDER);

	//相机
	LookGLCenter(width(), height());

	const xgraphic_state& state = statesrc.state_;
	const std::list<xgraphic_shapesrc>& vertsrcs = statesrc.vertsrclist_;
	const std::list<xgraphic_textsrc>& textsrcs = statesrc.textsrclist_;
	const std::list<xgraphic_imagesrc>& imagesrcs = statesrc.imagesrclist_;

	switch (state.type_)
	{
	case xgraphic_state::kSolidPoint:
		GLPointSolidDraw(state, vertsrcs);
		break;

	case xgraphic_state::kImagePoint:
		break;

	case xgraphic_state::kSolidLine:
		GLLineStringSolidDraw(state, vertsrcs);
		break;

	case xgraphic_state::kDashGapLine:
		break;

	case xgraphic_state::kImageLine:
		break;

	case xgraphic_state::kSolidFace:
		break;

	case xgraphic_state::kImageFace:
		break;

	case xgraphic_state::kText:
		GLTextDraw(statesrc.wpp_, state, textsrcs, clearcolor_);
		break;

	case xgraphic_state::kImage:
		break;

	default:
		break;
	}

	glFinish();
}
