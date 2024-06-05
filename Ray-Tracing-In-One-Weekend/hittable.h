#pragma once
#include"ray.h"
#include"rtweekend.h"
#include"aabb.h"

class material;

class hit_record
{
public:
    point3 p;      // 碰撞点
    vec3 normal;   // 碰撞点处的法线向量
    double t;      // 光线参数 t，表示光线上的点 P(t) = A + tB
    bool front_face; //法线的朝向
    std::shared_ptr<material>mat;//材质的列表 
    double u;   //纹理横坐标
    double v;   //纹理纵坐标


    void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
	virtual ~hittable() = default;
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
    virtual aabb bounding_box() const = 0;
};