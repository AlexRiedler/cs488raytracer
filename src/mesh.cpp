#include "mesh.hpp"

#include <iostream>
#include <float.h>
#include "primitive.hpp"
#include "a4.hpp"

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
  : m_verts(verts),
    m_faces(faces),
    m_boundbox(NULL)
{
    double min[3] = { DBL_MAX, DBL_MAX, DBL_MAX };
    double max[3] = { DBL_MIN, DBL_MIN, DBL_MIN };
    for (unsigned int i=0; i<m_faces.size(); i++) {
        for (unsigned int j=1; j<m_faces[i].size()-1; j++) {
            m_triangles.push_back(Triangle(&m_verts[m_faces[i][0]], &m_verts[m_faces[i][j]], &m_verts[m_faces[i][j+1]]));
        }
    }
    for (unsigned int i=0; i<m_faces.size(); i++) {
        for (unsigned int j=0; j<m_faces[i].size(); j++) {
            for (unsigned int k=0; k<3; k++) {
                min[k] = std::min(m_verts[m_faces[i][j]][k],min[k]);
                max[k] = std::max(m_verts[m_faces[i][j]][k],max[k]);
            }
        }
    }
    double width    = max[0] - min[0];
    double height   = max[1] - min[1];
    double depth    = max[2] - min[2];
    double size = std::max(std::max(width,height),depth);
    m_boundbox = new BoundingBox(Point3D(min[0], min[1], min[2]), size);
}

Mesh::~Mesh() {
    delete m_boundbox;
}

bool Mesh::hit(const Ray& ray, double &tmin, Shading &s) const
{
    Ray local_ray(ray);
    Shading shading(s.world);
    double t;
    tmin = 10E24;
    Vector3D normal;

    // check bounding box first
    Shading shading2(s.world);
    bool was_bound = m_boundbox->hit(ray,tmin,shading2);
    if (!m_boundbox->hit(ray,tmin,shading2)) {
        return false;
    }

    tmin = 10E24;
    for (unsigned int i=0; i<m_triangles.size(); i++) {
        
        Triangle triangle = m_triangles[i];
        if (triangle.hit(local_ray, t, shading) && (t < tmin)) {
            shading.hit_object = true;
            tmin = t;
            normal = shading.hit_normal;
        }
    }

    if (shading.hit_object) {
        s.hit_object = true;
        s.t = tmin;
        s.hit_normal = normal;
        return true;
    }
    return false;

}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  std::cerr << "mesh({";
  for (std::vector<Point3D>::const_iterator I = mesh.m_verts.begin(); I != mesh.m_verts.end(); ++I) {
    if (I != mesh.m_verts.begin()) std::cerr << ",\n      ";
    std::cerr << *I;
  }
  std::cerr << "},\n\n     {";
  
  for (std::vector<Mesh::Face>::const_iterator I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I) {
    if (I != mesh.m_faces.begin()) std::cerr << ",\n      ";
    std::cerr << "[";
    for (Mesh::Face::const_iterator J = I->begin(); J != I->end(); ++J) {
      if (J != I->begin()) std::cerr << ", ";
      std::cerr << *J;
    }
    std::cerr << "]";
  }
  std::cerr << "});" << std::endl;
  return out;
}
