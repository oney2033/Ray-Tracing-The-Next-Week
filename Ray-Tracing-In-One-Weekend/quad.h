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
        // 计算所有四个顶点的边界
        auto bbox_diagonal1 = aabb(Q, Q + u + v);
        auto bbox_diagonal2 = aabb(Q + u, Q + v);
        bbox = aabb(bbox_diagonal1, bbox_diagonal2);
    }

    aabb bounding_box() const override { return bbox; }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override 
    {
        auto denom = dot(normal, r.direction());

        // 如果光线平行于平面就没击中
        if (fabs(denom) < 1e-8)
            return false;

        // 如果命中点参数 t 位于射线间隔之外，则返回 false
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        //使用平面坐标确定击中点位于平面形状内
        auto intersection = r.at(t);
        vec3 planar_hitpt_vector = intersection - Q;
        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        auto beta = dot(w, cross(u, planar_hitpt_vector));

        if (!is_interior(alpha, beta, rec))
            return false;

        //射线击中二维形状；设置其余命中记录并返回 true
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(double a, double b, hit_record& rec)const
    {
        interval unit_interval = interval(0, 1);
        //给定平面坐标中的击中点，如果它位于图元之外，则返回 false，否则设置击中记录 UV 坐标并返回 true。
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