#include "gdalimage.h"
#include <Material.hpp>
#include <Image.hpp>
#include <Color.hpp>
#include <Reference.hpp>
#include "gdal/gdal_priv.h"
#include "gdal/gdal.h"
#include "gdal/cpl_conv.h"
#include <stdio.h>


using namespace godot;

void GDALImage::_register_methods() {
    //printf("In GDAL register_methods\n");
    register_method("create_image", &GDALImage::create_image);
}

GDALImage::GDALImage() {
    GDALAllRegister();
    const char *gdalVirt = "/data/dted/dted0/all_dt0_files.vrt";
    poDataset = (GDALDataset *) GDALOpen(gdalVirt, GA_ReadOnly);
    if(NULL == poDataset) {
        printf("Error loading %s\n", gdalVirt);
        return;
    } else {
        printf("Successfully opened GDAL\n");
    }
}

GDALImage::~GDALImage() {
}

void GDALImage::_init() {
//	printf("In init. %f,%f -> %f,%f\n", lat_dd_d, lon_dd_l, lat_dd_u, lon_dd_r);
}

Ref<Image> GDALImage::create_image(float lat_dd_d, float lon_dd_l,
    float lat_dd_u, float lon_dd_r, int img_px_x, int img_px_y) {
    
    GDALRasterBand *band = poDataset->GetRasterBand(1);
    
    double transform[6];
    poDataset->GetGeoTransform(transform);
    for(int i = 0; i < 6; i++) {
        printf("Transform[%d] = %f\n", i, transform[i]);
    }
    double data_min_lon = transform[0];
    double data_min_lat = transform[3]; 
    double lon_deg_per_px = transform[1];
    double lat_deg_per_px = transform[5];
    
    int x_pixels = (lon_dd_r - lon_dd_l) / lon_deg_per_px;
    int y_pixels = (lat_dd_u - lat_dd_d) / lat_deg_per_px;
    int x_off = -(data_min_lon - lon_dd_l) / lon_deg_per_px;
    int y_off = -(data_min_lat - lat_dd_d) / lat_deg_per_px;
    
    // printf(" %f,%f -> %f,%f => %d x %d + %d + %d px\n", lat_dd_d, lon_dd_l, lat_dd_u, lon_dd_r, x_pixels, y_pixels, x_off, y_off);
    float *scanline = (float *) CPLMalloc(img_px_x * img_px_y * sizeof(float));
    
    // FILE * f = fopen("../terr_test.csv", "w");
    
    CPLErr err = band->RasterIO(GF_Read, x_off, y_off, x_pixels, y_pixels,
            scanline, img_px_x, img_px_y, GDT_Float32, 0, 0);
    Image * im = Image::_new();
    im->create(img_px_x, img_px_x, false, Image::FORMAT_RGB8);
    im->lock();
    int positive_height = 0;
    int total_px = 0;
    for(int x = 0; x < img_px_x; x++) {
        for(int y = 0; y < img_px_x; y++) {
            float height = scanline[x + y * x_pixels];
            if(height < -32760.0) { // "Missing" in DTED
                height = 0; // Assume we have the whole world, and "missing" means "water"
            }
            total_px ++;
            if(height > 0) {
                positive_height++;
            }
            // fprintf(f, "%f,", height);
            double max_height = 10000;
            double q = height / max_height;
            if(q > 1.0) {
                q = 1.0;
            }
            Color col = Color(q, q, q);
            im->set_pixel(x, y, col);
        }
        // fprintf(f, "\n");
    }
    // fclose(f);
    im->unlock();
    CPLFree(scanline);
    // printf(" Total pixels: %d , Height > 0: %d\n", total_px, positive_height);
    
    return Ref(im);
}

