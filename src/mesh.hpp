#ifndef CS488_MESH_HPP
#define CS488_MESH_HPP

#include <vector>
#include <iosfwd>
#include "primitive.hpp"
#include "algebra.hpp"

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh(const std::vector<Point3D>& verts,
       const std::vector< std::vector<int> >& faces);
  ~Mesh();

  virtual bool hit(const Ray& ray, double &tmin, Shading &s) const;
  typedef std::vector<int> Face;
  
private:
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;
  std::vector<Triangle> m_triangles;

  BoundingBox *m_boundbox;

  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif
