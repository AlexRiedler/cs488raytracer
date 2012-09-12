#include "material.hpp"
#include "algebra.hpp"
#include "a4.hpp"

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
    : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
}

PhongMaterial::~PhongMaterial()
{
}

Colour PhongMaterial::shade(Shading &s)
{
    // ambient lighting
    Colour L = s.world.ambient * m_kd;

    // calculate ray hit location in WCS
    s.hit_point = s.ray.origin + s.t * s.ray.direction;
    s.hit_normal.normalize();

    // apply lighting for each light source
    for (std::list<Light*>::const_iterator it = s.world.lights.begin(); it != s.world.lights.end(); it++) {
        
        Light *light = (*it);

        // calculate light direction and normalize
        Vector3D light_dir = light->position - s.hit_point;
        light_dir.normalize();
        double light_dot = s.hit_normal * light_dir;


        // check for a collision from where ray hit to light source
        Shading collision_check(s);
        Ray light_ray;
        light_ray.direction = light->position - s.hit_point;
        light_ray.origin = s.hit_point;
        double t_min = 10E24;

        bool hit = s.world.scene->hit(light_ray, t_min, collision_check);
        if (!hit || collision_check.t > 1.0) {

            // calculate attenuation
            double dist = light_ray.direction.length();
            double attenuation = 1.0/(light->falloff[0] + light->falloff[1]*dist + light->falloff[2]*dist*dist);

            // diffuse lighting
            if (light_dot > 0.0) {
                L = L + m_kd * light_dot * light->colour * attenuation;
            }

            // specular lighting
            Vector3D reflected_ray (2.0 * s.hit_normal * (s.hit_normal * light_dir) - light_dir);
            reflected_ray.normalize();
            Vector3D ray_dir_norm = s.ray.direction;
            ray_dir_norm.normalize();

            // angle between the reflection and viewer
            double reflect_dot = reflected_ray * -ray_dir_norm;
            if (reflect_dot > 0.0) {
                L = L + m_ks * pow(reflect_dot, m_shininess) * light->colour * attenuation;
            }
        }

    }
    return L;
}

