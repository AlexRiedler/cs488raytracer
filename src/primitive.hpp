#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include <vector>

class Ray;
class Shading;
class Material;
class NonhierSphere;
class NonhierBox;

class Primitive {
public:
    virtual ~Primitive() { }
    virtual bool hit(const Ray& ray, double &tmin, Shading &s) const {
        return false;
    }
};

class Sphere : public Primitive {
public:
    virtual ~Sphere() { };
    virtual bool hit(const Ray& ray, double &tmin, Shading &s) const;
private:
    static const NonhierSphere nhs; // underlying simplex sphere
};

class Cube : public Primitive {
public:
    virtual ~Cube() { };
    bool hit(const Ray& ray, double &tmin, Shading &s) const;
private:
    static const NonhierBox nhb; // underlying simplex cube
};

class NonhierSphere : public Primitive {
public:
    NonhierSphere(const Point3D& pos, double radius): m_pos(pos), m_radius(radius) { };
    virtual ~NonhierSphere() { };

    virtual bool hit(const Ray& ray, double &tmin, Shading &s) const;
private:
    Point3D m_pos;
    double m_radius;
};

class Square: public Primitive {
public:
    Square(const Point3D& pos, const Vector3D &normal, const double size): m_pos(pos), m_normal(normal), m_size(size) { };
    virtual ~Square() { };

    virtual bool hit(const Ray& ray, double &tmin, Shading &s) const;
private:
    Point3D m_pos;
    Vector3D m_normal;
    double m_size;
};

class Triangle: public Primitive {
public:
    Triangle(Point3D *p0, Point3D *p1, Point3D *p2) : p0(p0), p1(p1), p2(p2), normal((*p0-*p1).cross(*p0-*p2)){ };
    virtual ~Triangle() { };

    virtual bool hit(const Ray& ray, double &tmin, Shading &s) const;
private:
    Point3D *p0,*p1,*p2;
    Vector3D normal;
};

class NonhierBox : public Primitive {
public:
    NonhierBox(const Point3D& pos, double size);
    virtual ~NonhierBox() { };

    virtual bool hit(const Ray& ray, double &tmin, Shading &s) const;
private:
    std::vector<Square> m_squares;
    Point3D m_pos;
    double m_size;
};

typedef NonhierBox BoundingBox;


#endif
