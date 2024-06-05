#pragma once
#include"rtweekend.h"

class textrue
{
public:
	virtual ~textrue() = default;
	virtual color value(double u, double v, const point3& p)const = 0;
};

class solid_color :public textrue
{
public:
	solid_color(const color& albedo):albedo(albedo){}

	solid_color(double red,double green,double blue):solid_color(color(red,green,blue)){}

	color value(double u, double v, const point3& p)const override
	{
		return albedo;
	}

private:
	color albedo;
};

class checker_texture :public textrue
{
public:
	checker_texture(double scale,std::shared_ptr<textrue>even,std::shared_ptr<textrue>odd)
		:inv_scale(1.0/scale),even(even),odd(odd){}

	checker_texture(double scale,const color& c1,const color& c2)
		:inv_scale(1.0/scale),even(std::make_shared<solid_color>(c1)),odd(std::make_shared<solid_color>(c2))
	{}

	color value(double u, double v, const point3& p)const override
	{
		auto xInteger = int(std::floor(inv_scale * p.x()));
		auto yInteger = int(std::floor(inv_scale * p.y()));
		auto zInteger = int(std::floor(inv_scale * p.z()));

		bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

		return isEven ? even->value(u, v, p) : odd->value(u, v, p);
	}

private:
	double inv_scale;
	std::shared_ptr<textrue>even;
	std::shared_ptr<textrue>odd;
};