#pragma once
#include"ray.h"
#include"rtweekend.h"

class material;

class hit_record
{
public:
    point3 p;      // ��ײ��
    vec3 normal;   // ��ײ�㴦�ķ�������
    double t;      // ���߲��� t����ʾ�����ϵĵ� P(t) = A + tB
    bool front_face;
    std::shared_ptr<material>mat;

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
};