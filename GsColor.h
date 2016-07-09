#pragma once


//ÑÕÉ«×ª»»
template<class V3>
uint32_t IRgb(const V3& v) {
	uint32_t u = 0;
	unsigned char* pu = (unsigned char*)&u;
	pu[0] = (int)(v.x * 255);
	pu[1] = (int)(v.y * 255);
	pu[2] = (int)(v.z * 255);
	pu[3] = 255;
	return u;
}
template<class V4>
uint32_t IRgba(const V4& v) {
	uint32_t u = 0;
	unsigned char* pu = (unsigned char*)&u;
	pu[0] = (int)(v.x * 255);
	pu[1] = (int)(v.y * 255);
	pu[2] = (int)(v.z * 255);
	pu[3] = (int)(v.w * 255);
	return u;
}
template<class V3>
V3 VRgb(uint32_t u) {
	V3 v;
	const unsigned char* pu = (const unsigned char*)&u;
	v.x = pu[0] / 255.0f;
	v.y = pu[1] / 255.0f;
	v.z = pu[2] / 255.0f;
	return v;
}
template<class V4>
V4 VRgba(uint32_t u) {
	V4 v;
	const unsigned char* pu = (const unsigned char*)&u;
	v.x = pu[0] / 255.0f;
	v.y = pu[1] / 255.0f;
	v.z = pu[2] / 255.0f;
	v.w = pu[3] / 255.0f;
	return v;
}


template<class V3>
uint32_t IBgr(const V3& v) {
	uint32_t u = 0;
	unsigned char* pu = (unsigned char*)&u;
	pu[2] = (int)(v.x * 255);
	pu[1] = (int)(v.y * 255);
	pu[0] = (int)(v.z * 255);
	pu[3] = 255;
	return u;
}
template<class V4>
uint32_t IBgra(const V4& v) {
	uint32_t u = 0;
	unsigned char* pu = (unsigned char*)&u;
	pu[2] = (int)(v.x * 255);
	pu[1] = (int)(v.y * 255);
	pu[0] = (int)(v.z * 255);
	pu[3] = (int)(v.w * 255);
	return u;
}
template<class V3>
V3 VBgr(uint32_t u) {
	V3 v;
	const unsigned char* pu = (const unsigned char*)&u;
	v.x = pu[2] / 255.0f;
	v.y = pu[1] / 255.0f;
	v.z = pu[0] / 255.0f;
	return v;
}
template<class V4>
V4 VBgra(uint32_t u) {
	V4 v;
	const unsigned char* pu = (const unsigned char*)&u;
	v.x = pu[2] / 255.0f;
	v.y = pu[1] / 255.0f;
	v.z = pu[0] / 255.0f;
	v.w = pu[3] / 255.0f;
	return v;
}