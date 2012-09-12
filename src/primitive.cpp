#include "primitive.hpp"
#include "a4.hpp"
#include "constants.hpp"

const NonhierSphere Sphere::nhs(Point3D(0,0,0), 1.0);
const NonhierBox Cube::nhb (Point3D(0,0,0), 1.0);

bool Sphere::hit(const Ray& ray, double &tmin, Shading &s) const
{
    return Sphere::nhs.hit(ray, tmin, s);
}

bool Cube::hit(const Ray& ray, double &tmin, Shading &s) const
{
    return Cube::nhb.hit(ray, tmin, s);
}

bool NonhierSphere::hit(const Ray& ray, double &tmin, Shading &s) const
{
    // use implicit equation of a sphere
    // plugging in line equation
    // then solve using quadratic formula

    double t;
    double radius = m_radius;

    Vector3D temp_origin = ray.origin - m_pos;
    double a = ray.direction * ray.direction;
    double b = 2.0 * temp_origin * ray.direction;
    double c = temp_origin * temp_origin - radius*radius;
    double disc = b * b - 4.0 * a * c;

    if (disc < 0.0) {
        return false;
    } else {
        double e = sqrt(disc);
        double denom = 2.0 * a;

        // negative root
        t = (-b - e) / denom;

        if (t > EPSILON) {
            tmin = t;
            s.hit_normal = (temp_origin + t * ray.direction) / radius;
            return true;
        }

        // positive root
        t = (-b + e) / denom;

        if (t > EPSILON) {
            tmin = t;
            s.hit_normal = (temp_origin + t * ray.direction) / radius;
            return true;
        }
    }

    return false;
}

bool NonhierBox::hit(const Ray& ray, double &tmin, Shading &s) const
{
    //
    // Determine the closest hit of each side of the box / square
    //

    Ray local_ray(ray);

    Shading shading(s.world);
    double tmin2 = 10E24;
    double t;
    Vector3D normal;
    for (unsigned int i=0; i<m_squares.size(); i++) {
        
        Square square = m_squares[i];
        if (square.hit(local_ray, t, shading) && (t < tmin2)) {
            shading.hit_object = true;
            tmin2 = t;
            normal = shading.hit_normal;
        }
    }

    if (shading.hit_object) {
        s.hit_object = true;
        s.t = tmin2;
        tmin = tmin2;
        s.hit_normal = normal;
        return true;
    }
    return false;

}

bool Square::hit(const Ray& ray, double &tmin, Shading &s) const
{
    double size_2 = m_size / 2.0;

    Point3D point_on_face = m_pos + (m_normal * size_2);
    double t = (point_on_face - ray.origin) * m_normal / (ray.direction * m_normal);

    if (t < EPSILON) {
        return false;
    }

    Point3D hit_point = ray.origin + t * ray.direction;

    // verify point lays inside rectangle
    if (m_normal.x() == 0.0) {
        if (hit_point.x() > m_pos.x() + size_2 || hit_point.x() < m_pos.x() - size_2) {
            return false;
        }
    }
    if (m_normal.y() == 0.0) {
        if (hit_point.y() > m_pos.y() + size_2 || hit_point.y() < m_pos.y() - size_2) {
            return false;
        }
    }
    if (m_normal.z() == 0.0) {
        if (hit_point.z() > m_pos.z() + size_2 || hit_point.z() < m_pos.z() - size_2) {
            return false;
        }
    }

    tmin = t;
    s.hit_normal = m_normal;

    return true;

}

bool Triangle::hit(const Ray& ray, double &tmin, Shading &s) const
{
    // BASED on derivation in the book "Ray Tracing from the Ground Up"
    //       by Kevin Suffern
    double a = p0->x() - p1->x();
    double b = p0->x() - p2->x();
    double c = ray.direction.x();
    double d = p0->x() - ray.origin.x();

    double e = p0->y() - p1->y();
    double f = p0->y() - p2->y();
    double g = ray.direction.y();
    double h = p0->y() - ray.origin.y();

    double i = p0->z() - p1->z();
    double j = p0->z() - p2->z();
    double k = ray.direction.z();
    double l = p0->z() - ray.origin.z();

    double m = f * k - g * j;
    double n = h * k - g * l;
    double p = f * l - h * j;

    double q = g * i - e * k;
    double r = e * j - f * i;

    double inverse_denominator = 1.0 / (a * m + b * q + c * r);

    double e1 = d * m - b * n - c * p;

    double beta = e1 * inverse_denominator;

    if (beta < 0.0) {
        return false;
    }

    double u = e * l - h * i;
    double e2 = a * n + d * q + c * u;
    double gamma = e2 * inverse_denominator;

    if (gamma < 0.0) {
        return false;
    }

    if (beta + gamma > 1.0) {
        return false;
    }

    double e3 = a * p - b * u + d * r;
    double t = e3 * inverse_denominator;

    if (t < EPSILON) {
        return false;
    }

    tmin = t;
    s.hit_normal = normal;

    return true;
}


NonhierBox::NonhierBox(const Point3D& pos, double size)
    : m_pos(Point3D(pos.x()+size/2.0, pos.y()+size/2.0, pos.z()+size/2.0)), m_size(size) {
    m_squares.push_back(Square(m_pos,Vector3D(0.0,0.0,-1.0),size)); // FRONT
    m_squares.push_back(Square(m_pos,Vector3D(0.0,0.0,1.0),size));  // BACK
    m_squares.push_back(Square(m_pos,Vector3D(0.0,1.0,0.0),size));  // TOP
    m_squares.push_back(Square(m_pos,Vector3D(0.0,-1.0,0.0),size)); // BOTTOM
    m_squares.push_back(Square(m_pos,Vector3D(1.0,0.0,0.0),size));  // RIGHT
    m_squares.push_back(Square(m_pos,Vector3D(-1.0,0.0,0.0),size)); // LEFT
}

