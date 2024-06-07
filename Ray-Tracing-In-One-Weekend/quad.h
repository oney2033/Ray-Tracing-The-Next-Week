#pragma once
#include "rtweekend.h"

#include "hittable.h"

class quad : public hittable 
{
public:
    quad(const point3& Q, const vec3& u, const vec3& v, std::shared_ptr<material> mat)
        : Q(Q), u(u), v(v), mat(mat)
    {
        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);
        set_bounding_box();
    }

    virtual void set_bounding_box() 
    {
        // ���������ĸ�����ı߽�
        auto bbox_diagonal1 = aabb(Q, Q + u + v);
        auto bbox_diagonal2 = aabb(Q + u, Q + v);
        bbox = aabb(bbox_diagonal1, bbox_diagonal2);
    }

    aabb bounding_box() const override { return bbox; }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override 
    {
        auto denom = dot(normal, r.direction());

        // �������ƽ����ƽ���û����
        if (fabs(denom) < 1e-8)
            return false;

        // ������е���� t λ�����߼��֮�⣬�򷵻� false
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        //ʹ��ƽ������ȷ�����е�λ��ƽ����״��
        auto intersection = r.at(t);
        vec3 planar_hitpt_vector = intersection - Q;
        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        auto beta = dot(w, cross(u, planar_hitpt_vector));

        if (!is_interior(alpha, beta, rec))
            return false;

        //���߻��ж�ά��״�������������м�¼������ true
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(double a, double b, hit_record& rec)const
    {
        interval unit_interval = interval(0, 1);
        //����ƽ�������еĻ��е㣬�����λ��ͼԪ֮�⣬�򷵻� false���������û��м�¼ UV ���겢���� true��
        if (!unit_interval.contains(a) || !unit_interval.contains(b))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    point3 Q;
    vec3 u, v;
    vec3 w;
    std::shared_ptr<material> mat;
    aabb bbox;
    vec3 normal;
    double D;
};