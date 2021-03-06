#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"

class Shading;

class Material {
public:
    virtual ~Material();
    virtual Colour shade(Shading &s) = 0;

protected:
    Material()
    {
    }
};

class PhongMaterial : public Material {
public:
    PhongMaterial(const Colour& kd, const Colour& ks, double shininess);
    virtual ~PhongMaterial();

    virtual Colour shade(Shading &s);

private:
    Colour m_kd;
    Colour m_ks;

    double m_shininess;
};


#endif
