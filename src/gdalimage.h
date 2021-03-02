#ifndef GDALIMAGE_H
#define GDALIMAGE_H

#include <Godot.hpp>
#include <Image.hpp>
#include <Object.hpp>
#include <MeshInstance.hpp>
#include <math.h>
#include "gdal/gdal.h"
#include "gdal/gdal_priv.h"

namespace godot {

class GDALImage : public MeshInstance {
    GODOT_CLASS(GDALImage, MeshInstance)

private:
    GDALDataset *poDataset;
    
public:
    static void _register_methods();

    GDALImage();
    ~GDALImage();

    void _init();

    Ref<Image> create_image(float lat_dd_u, float lat_dd_d,
        float lon_dd_l,float lon_dd_r);

};

}

#endif

