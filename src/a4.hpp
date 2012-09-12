#ifndef CS488_A4_HPP
#define CS488_A4_HPP

#include <string>
#include "algebra.hpp"
#include "scene.hpp"
#include "light.hpp"
#include "world.hpp"

const int PNG_RED   = 0;
const int PNG_GREEN = 1;
const int PNG_BLUE  = 2;

void a4_render(// What to render
               SceneNode* root,
               // Where to output the image
               const std::string& filename,
               // Image size
               int width, int height,
               // Viewing parameters
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               // Lighting parameters
               const Colour& ambient,
               const std::list<Light*>& lights
               );

class Ray {

public:
    Ray() {};
    Ray(const Ray& ray) : origin(ray.origin), direction(ray.direction) { };

    Point3D origin;
    Vector3D direction;

};

class Shading {

public:
    Shading(World &world);
    Shading(const Shading& s);

    World       &world;     // the scene and related properties
    bool        hit_object; // did it hit an object?
    Material    *material;  // material that was hit
    Point3D     hit_point;  // WCS point of hit
    Vector3D    hit_normal; // WCS normal at point of hit
    Ray         ray;        // ray that hit the object
    double      t;          // parametric intersection point
    Colour      colour;     // colour of the pixel

};


// image data class that is used across mutliple threads
// this is used mainly because libpng structures are not thread safe
class ImageData {
    
public:
    ImageData(const int width, const int height): m_width(width), m_height(height) {
        data = new ColorData[width*height];
    }

    ~ImageData() {
        delete[] data;
    }
    void set(const int x, const int y, const int c, const double v) {
        if (c == PNG_RED)
            data[get_index(x,y)].r = v;
        if (c == PNG_GREEN)
            data[get_index(x,y)].g = v;
        if (c == PNG_BLUE)
            data[get_index(x,y)].b = v;
    }
    void set(const int x, const int y, const double r, const double g, const double b) {
        data[get_index(x,y)].r = r;
        data[get_index(x,y)].g = g;
        data[get_index(x,y)].b = b;
    }
    double get(const int x, const int y, const int c) const {
        if (c == PNG_RED)
            return data[get_index(x,y)].r;
        if (c == PNG_GREEN)
            return data[get_index(x,y)].g;
        if (c == PNG_BLUE)
            return data[get_index(x,y)].b;
        return 0.0;
    }

    int get_height() const { return m_height; }
    int get_width() const { return m_width; }

private:

    inline int get_index(int x, int y) const {
        return x + y*m_width;
    }

    struct ColorData {
        double r,g,b;
    };
    ColorData *data;
    int m_width;
    int m_height;

};

// thread rendering directive for splitting up
// workload across multiple threads
struct RenderDirective {
    World *world;       // input scene and camera information
    ImageData *img;     // output image
    long start_pixel;   // index of start pixel to render
    long num_pixels;    // the number of pixels it should render
    int thread_no;      // thread #
};

// initialization function for new threads
void * a4_trace_ray_set(void * render_directive);

#endif
