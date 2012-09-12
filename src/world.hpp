#ifndef __WORLD_CPP__
#define __WORLD_CPP__

class SceneNode;
struct Light;
class Point3D;
class Vector3D;
class Colour;

class World
{
public:

// Constructor
    World(SceneNode *scene,
          const std::list<Light*> lights,
          const Point3D *eye,
          const Vector3D *view,
          const Vector3D *up,
          double fovy,
          const Colour ambient):
        scene(scene),
        lights(lights),
        ambient(ambient),
        eye(eye),
        view(view),
        up(up),
        fovy(fovy)
    { }

    const Point3D get_eye() const { return *eye; };

    double get_fov() const { return fovy; };

    const Vector3D get_view() const { return *view; };

// Scene Information
    SceneNode *scene;
    const std::list<Light*> lights;
    Colour ambient;

// Camera Information
    const Point3D *eye;
    const Vector3D *view;
    const Vector3D *up;
    double fovy;

};

#endif
