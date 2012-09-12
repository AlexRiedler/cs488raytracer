#include "scene.hpp"
#include <iostream>
#include "a4.hpp"
#include "matrices.hpp"

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
}

SceneNode::~SceneNode()
{
}

bool SceneNode::hit(const Ray& ray, double &tmin, Shading &s) {
    // transform ray so in object co-ordinates
    Ray local_ray(ray);
    local_ray.direction = m_invtrans * ray.direction;
    local_ray.origin = m_invtrans * ray.origin;

    Shading shading(s.world);
    double t;
    tmin = 10E24;
    Vector3D normal;
    Material *mat;
    for (ChildList::const_iterator it = m_children.begin(); it != m_children.end(); it++) {
        
        SceneNode *object = (*it);
        if (object->hit(local_ray, t, shading) && (t < tmin)) {
            tmin = t;
            normal = shading.hit_normal;
            mat = shading.material;
        }
    }

    if (shading.hit_object) {
        s.hit_object = true;
        s.t = tmin;
        s.hit_normal = m_invtrans.transpose() * normal;
        s.material = mat;
        return true;
    }
    return false;
}

void SceneNode::rotate(char axis, double angle)
{
    set_transform(m_trans*rotation(angle, axis));
}

void SceneNode::scale(const Vector3D& amount)
{
    set_transform(m_trans*scaling(amount));
}

void SceneNode::translate(const Vector3D& amount)
{
    set_transform(m_trans*translation(amount));
}

bool SceneNode::is_joint() const
{
  return false;
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}
 
bool GeometryNode::hit(const Ray& ray, double &tmin, Shading &s) { 
    // transform ray so in object co-ordinates
    Ray local_ray(ray);
    local_ray.direction = m_invtrans * ray.direction;
    local_ray.origin = m_invtrans * ray.origin;

    // only hit if primitive hit
    if (m_primitive->hit(local_ray, tmin, s)) {
        s.hit_object = true;
        s.material   = m_material;
        // translate normal back to WCS
        s.hit_normal = m_invtrans.transpose() * s.hit_normal;
        return true;
    }

    return false;
}

