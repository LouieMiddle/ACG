#include "photon.h"

Photon::Photon(Ray p_path, Colour p_intensity, PhotonMode p_mode) {
    path = p_path;
    intensity = p_intensity;
    mode = p_mode;
}
