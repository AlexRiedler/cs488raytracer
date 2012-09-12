#include <pthread.h>
#include "a4.hpp"
#include "image.hpp"
#include "world.hpp"
#include "constants.hpp"

// Pixel Mutex Counter
pthread_mutex_t pixel_mutex = PTHREAD_MUTEX_INITIALIZER;
int pixel_count = 0;
int last_printed_percent = -1;

// Thread Safe Pixel Count Increaser
void increase_pixel_count(long total_pixels)
{
    pthread_mutex_lock(&pixel_mutex);
    pixel_count++;
    // Print after every 10% it completes
    if ((pixel_count*100)/total_pixels % 5 == 0 && last_printed_percent != (pixel_count*100/total_pixels)) {
        last_printed_percent = (pixel_count*100 / total_pixels);
        std::cerr << last_printed_percent << " percent completed" << std::endl;
    }
    pthread_mutex_unlock(&pixel_mutex);
}

Colour background(double x, double y, double width, double height)
{
    if ((int)x % 2 == 0 || (int)x % 7 == 0) {
        return Colour(0.0, 0.0, 0.0);
    } else if ((int)y % 2 == 0) {
        return Colour(0.0, 0.0, 0.0);
    } else {
        return Colour((height-y)/height, y/height, 0.0);
    }
}


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
)
{

    // create world object to store information
    World world (root, lights, &eye, &view, &up, fov, ambient);

    // print information about scene being rendered
    std::cerr << "------------------------------" << std::endl;
    std::cerr << "Rendering: " << filename << std::endl;
    std::cerr << "------------------------------" << std::endl;

    // create a thread set to render
    pthread_t thread[NUM_THREADS];
    RenderDirective rd[NUM_THREADS];
    ImageData imgdata(width, height);

    std::cerr << "Spawning " << NUM_THREADS << " threads to render scene." << std::endl;

    // build a render directive for each thread, and initiate thread start
    // this makes each thread render the same number of pixels (may not be optimal)
    // could potentially change into mutex locks for each pixel
    for (int i=0; i<NUM_THREADS; i++) {
        rd[i].img = &imgdata;
        rd[i].world = &world;
        rd[i].start_pixel = i*width*height/NUM_THREADS;
        if (i == NUM_THREADS-1) {
            rd[i].num_pixels = width*height - rd[i].start_pixel;
        } else {
            rd[i].num_pixels = width*height/NUM_THREADS;
        }
        rd[i].thread_no = i+1;

        // make last thread be the initiator thread
        if (i == NUM_THREADS - 1) {
            a4_trace_ray_set(static_cast<void*>(rd+i));
        } else {
            pthread_create(&thread[i], NULL, a4_trace_ray_set, static_cast<void*>(rd+i));
        }
    }

    // wait for all threads to finish
    for (int i=0; i<NUM_THREADS-1; i++) {
        pthread_join(thread[i], NULL);    
    }

    // copy image data to output image PNG
    Image img(width, height, 3); // Create PNG image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < 3; c++) {
                img(x, y, c) = imgdata.get(x,y,c);
            }
        }
    }
    img.savePng(filename);

    std::cerr << "------------------------------" << std::endl;
    std::cerr << "Completed Succesfully" << std::endl;
    std::cerr << "------------------------------" << std::endl;
}

void * a4_trace_ray_set(void * render_directive)
{
    RenderDirective *rd = static_cast<RenderDirective*>(render_directive);

    // calculate change of basis
    Vector3D w = (*rd->world->view);
    w.normalize();
    Vector3D u = rd->world->up->cross(w);
    u.normalize();
    Vector3D v = w.cross(u);

    // for each pixel requested to render
    for (int i = rd->start_pixel; i < rd->num_pixels + rd->start_pixel; i++) {
        // determine which pixel to render based on index
        int x = i % rd->img->get_width();
        int y = i/rd->img->get_width();

        // DEFER: extend to non-axis aligned rendering

        // create ray using perspective
        Ray ray;
        // origin is the eye
        ray.origin = rd->world->get_eye();

        // Calculate view-plane-height = 2 * d * tan ( fovy / 2 )
        double view_plane_height = 2.0*rd->world->view->z()*tan((rd->world->get_fov()*M_PI/180.0)/2);
        double view_plane_width  = view_plane_height * rd->img->get_width()/rd->img->get_height();

        // direction is 
        // DEFER: not sure if this calculate is quite correct ...
        ray.direction = Vector3D(
                -(view_plane_width/rd->img->get_width())*((double)x - 0.5 * rd->img->get_width()),
                (view_plane_height/rd->img->get_height())*((double)y - 0.5 * rd->img->get_height()),
                rd->world->view->z());

        ray.direction.normalize();

        // trace the ray onto given image
        Shading shade(*rd->world);
        shade.ray = ray;
        double tmin = 10E24;
        rd->world->scene->hit(ray, tmin, shade);

        if (shade.hit_object) {
            // use the material to apply shading
            Colour point_colour = shade.material->shade(shade);

            // set the colour on image
            rd->img->set(x, y, point_colour.R(), point_colour.G(), point_colour.B());
        } else {
            // clear pixel to background color
            Colour bg_pixel = background(x,y,rd->img->get_width(),rd->img->get_height());
            rd->img->set(x,y,bg_pixel.R(), bg_pixel.G(), bg_pixel.B());
        }

        increase_pixel_count(rd->img->get_width()*rd->img->get_height());
    }

    return NULL;
}

Shading::Shading(World& world):
    world(world),
    hit_object(false),
    material(NULL),
    hit_point(),
    hit_normal(),
    ray(),
    t(0.0),
    colour(0.0)
{}

Shading::Shading(const Shading& s):
    world(s.world),
    hit_object(s.hit_object),
    material(s.material),
    hit_point(s.hit_point),
    hit_normal(s.hit_normal),
    ray(s.ray),
    t(s.t),
    colour(s.colour)
{}

