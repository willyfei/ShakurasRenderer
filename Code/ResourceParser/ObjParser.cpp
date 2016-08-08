#include "SurfaceParser.h"
#include "ObjParser.h"
#include <array>
#include <map>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include "ResUtility.h"


using namespace shakuras;

bool LoadObjMtl(std::vector<ObjMtl>& mtls, const std::string& mtl_file, _FSPFX path dir_path) {
	_FSPFX path full_path = dir_path;
	full_path.concat("/");
	full_path.concat(mtl_file);

	std::ifstream mtlf(full_path.string());
	if (!mtlf) return false;

	std::string cmd;

	ObjMtl* pmtl = NULL;
	for (;;) {
		mtlf >> cmd;
		if (!mtlf) { break; }

		if (cmd == "#") {
			mtlf.ignore(1000, '\n');
			continue;
		}

		if (cmd == "newmtl") {
			std::string mtl_name;
			mtlf >> mtl_name;

			for (size_t i_mtl = 0; i_mtl < mtls.size(); ++i_mtl) {
				if (mtls[i_mtl].name == mtl_name) {
					pmtl = &(mtls[i_mtl]);
					break;
				}
			}
		}

		if (!pmtl) { continue; }

		if (cmd == "Ka") {
			float r, g, b;
			mtlf >> r >> g >> b;
			pmtl->ambient.set(r, g, b);
		}
		else if (cmd == "Kd") {
			float r, g, b;
			mtlf >> r >> g >> b;
			pmtl->diffuse.set(r, g, b);
		}
		else if (cmd == "Ks") {
			float r, g, b;
			mtlf >> r >> g >> b;
			pmtl->specular.set(r, g, b);
		}
		else if (cmd == "d" || cmd == "Tr") {
			mtlf >> pmtl->alpha;
		}
		else if (cmd == "Ns") {
			mtlf >> pmtl->shininess;
		}
		else if (cmd == "illum") {
			int illumination;
			mtlf >> illumination;
			pmtl->is_specular = (illumination == 2);
		}
		else if (cmd == "map_Kd") {
			mtlf >> pmtl->tex_name;

			full_path = dir_path;
			full_path.concat("/");
			full_path.concat(pmtl->tex_name);

			pmtl->tex = CreateSoftMipmap(LoadSurface(full_path.string(), false));
		}
		else {
			// Unrecognized command
		}
		mtlf.ignore(1000, '\n');
	}

	mtlf.close();
	return true;
}


bool LoadObj(std::string fname, std::vector<ObjVert>& verts, std::vector<uint32_t>& indices, std::vector<uint32_t>& attrs, std::vector<ObjMtl>& mtls, bool flip_tex_v) {
	fname = _FSPFX absolute(fname, ResourceDir()).string();

	std::ifstream objf(fname);

	if (!objf) return false;

	std::string obj_cmd;

	std::vector<Vector3f> positions;
	std::vector<Vector2f> uvs;
	std::vector<Vector3f> normals;

	ObjMtl mtl;
	std::string mtl_fname;

	// init_material
	mtls.push_back(mtl);

	uint32_t subset = 0;

	typedef std::array<uint32_t, 3> smooth_id_t;
	std::map<smooth_id_t, uint32_t> smooth_id_to_vertex_index;

	for (;;) {
		objf >> obj_cmd;
		if (!objf) { break; }
		if (obj_cmd == "#") {
			; // Comment
		}
		else if (obj_cmd == "v") {
			float x = 0.0f, y = 0.0f, z = 0.0f;
			objf >> x >> y >> z;
			positions.push_back(Vector3f(x, y, z));
		}
		else if (obj_cmd == "vt") {
			float u = 0.0f, v = 0.0f;
			objf >> u >> v;
			uvs.push_back(Vector2f(u, (flip_tex_v ? 1.0f - v : v)));
		}
		else if (obj_cmd == "vn") {
			float x = 0.0f, y = 0.0f, z = 0.0f;
			objf >> x >> y >> z;
			normals.push_back(Vector3f(x, y, z));
		}
		else if (obj_cmd == "f") {

			uint32_t pos_index = 0, texcoord_index = 0, normal_index = 0;

			for (uint32_t face_index = 0; face_index < 3; ++face_index) {
				ObjVert vert;

				objf >> pos_index;
				vert.pos = positions[pos_index - 1];

				if (objf.peek() == '/') {
					objf.ignore();

					if (objf.peek() != '/') {
						objf >> texcoord_index;
						vert.uv = uvs[texcoord_index - 1];
					}

					if ('/' == objf.peek()) {
						objf.ignore();
						objf >> normal_index;
						vert.normal = normals[normal_index - 1];
					}
				}

				uint32_t vert_index = 0;
				smooth_id_t smooth_id = { pos_index, texcoord_index, normal_index };
				if (smooth_id_to_vertex_index.count(smooth_id) == 0) {
					vert_index = static_cast<uint32_t>(verts.size());
					smooth_id_to_vertex_index[smooth_id] = vert_index;
					verts.push_back(vert);
				}
				else {
					vert_index = smooth_id_to_vertex_index[smooth_id];
				}
				indices.push_back(vert_index);
			}

			attrs.push_back(subset);
		}
		else if (obj_cmd == "mtllib") {
			objf >> mtl_fname;
		}
		else if (obj_cmd == "usemtl") {
			std::string name;
			objf >> name;
			bool found = false;
			for (size_t mtl_index = 0; mtl_index < mtls.size(); ++mtl_index) {
				ObjMtl* pmtl = &mtls[mtl_index];
				if (pmtl->name == name) {
					found = true;
					subset = static_cast<uint32_t>(mtl_index);
					break;
				}
			}

			if (!found) {
				subset = static_cast<uint32_t>(mtls.size());
				mtls.push_back(ObjMtl());
				mtls.back().name = name;
			}

		}
		else {
			; // Unrecognized command
		}

		objf.ignore(1000, '\n');
	}

	objf.close();

	_FSPFX path abspath = _FSPFX absolute(_FSPFX path(fname));
	if (!mtl_fname.empty()) {
		LoadObjMtl(mtls, mtl_fname, abspath.parent_path());
	}
	return true;
}



bool LoadObjMesh(const std::string& fname, std::vector<ObjMesh>& meshs, bool flip_tex_v) {
	std::vector<ObjVert> verts;
	std::vector<uint32_t> indices;
	std::vector<uint32_t> attrs;
	std::vector<ObjMtl> mtls;

	if (!LoadObj(fname, verts, indices, attrs, mtls, flip_tex_v)) {
		return false;
	}

	for (size_t i_mtl = 0; i_mtl < mtls.size(); ++i_mtl) {
		ObjMesh mesh;

		// Fill data
		mesh.verts = verts;
		mesh.mtl = mtls[i_mtl];

		// Construct vertex indices
		for (size_t i_indices = 0; i_indices < indices.size(); ++i_indices) {
			if (attrs[i_indices / 3] == i_mtl) {
				mesh.tris.push_back(indices[i_indices]);
			}
		}

		// Set mesh indices.
		if (!mesh.tris.empty()) {
			meshs.push_back(mesh);
		}
	}

	return true;
}