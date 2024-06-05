#pragma once
#include"ray.h"
#include"rtweekend.h"
#include"aabb.h"

class material;

class hit_record
{
public:
    point3 p;      // ��ײ��
    vec3 normal;   // ��ײ�㴦�ķ�������
    double t;      // ���߲��� t����ʾ�����ϵĵ� P(t) = A + tB
    bool front_face; //���ߵĳ���
    std::shared_ptr<material>mat;//���ʵ��б� 
    double u;   //���������
    double v;   //����������


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