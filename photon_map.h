//#include "transform.h"
//#include "polymesh_object.h"
//#include "vector.h"
//#include "light.h"
//#include "point_light.h"
//#include "directional_light.h"
//#include "plane_object.h"
//#include "sphere_object.h"
//#include "photon.h"
//#include "alglib/stdafx.h"
//#include "alglib/alglibmisc.h"
//
//using namespace std;
//using namespace alglib;
//
//class PhotonMap {
//public:
//    Object *objects;
//    Light *lights;
//
//    PhotonMap(Object *p_objects, Light *p_lights);
//
//    Colour estimate_radiance(Ray &ray, Hit &hit, kdtree &tree, vector<Photon> &photons, vector<Photon *> local_photons);
//
//    vector<Photon *> gather_photons(Vertex p, int k, kdtree &tree, vector<Photon> &photons);
//
//    void build_kd_tree(vector<double> &points, kdtree &tree, vector<long> &tags);
//
//    void store_photon(Photon p, vector<double> &points, vector<Photon> &photons, vector<long> &tags);
//};
