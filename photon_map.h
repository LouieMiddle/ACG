#include "transform.h"
#include "polymesh_object.h"
#include "vector.h"
#include "light.h"
#include "point_light.h"
#include "directional_light.h"
#include "plane_object.h"
#include "sphere_object.h"
#include "photon.h"
#include "alglib/stdafx.h"
#include "alglib/alglibmisc.h"
#include "utils.h"

using namespace std;
using namespace alglib;

class PhotonMap {
public:
    Object *object_list;
    Light *light_list;

    vector<Photon> photons;
    vector<double> points;
    vector<long long> tags;
    kdtree tree;

    PhotonMap(Object *p_objects, Light *p_lights);

    void emit_photons(int number_photons, int recurse);

    void trace_photon(Photon photon, int recurse);

    void build_kd_tree();

    void gather_photons(Vertex position, int neighbours, vector<Photon *> &local_photons);

    void store_photon(Photon photon);
};
