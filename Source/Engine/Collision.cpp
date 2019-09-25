#include <algorithm>
#include "../Globals.h"
#include "Collision.h"
#include "Modules/ModuleRender.h"
#include "Application.h"

OBB& AABB::transform(OBB& target, const Transform& t) const
{
    OBB o0;
    o0.set(m_min, m_max, 0.0f);
    target.m_transform.setMultiply(t, o0.m_transform);
    return target;
}

void AABB::DebugDraw(const Transform& transform) const
{
	SDL_Point pts[5];
	pts[0].x = (int)m_min.x;
	pts[0].y = (int)m_min.y;
	pts[1].x = (int)m_min.x;
	pts[1].y = (int)m_max.y;
	pts[2].x = (int)m_max.x;
	pts[2].y = (int)m_max.y;
	pts[3].x = (int)m_max.x;
	pts[3].y = (int)m_min.y;
	pts[4] = pts[0];
	Renderer->AddToColliderDrawBuffer(pts, 5,255, 0, 0);
}

const AABB aabbUnit(-vec2One, vec2One);

OBB& OBB::transform(OBB& target, const Transform& t) const
{
    target.m_transform.setMultiply(t, m_transform);
    return target;
}

void OBB::DebugDraw(const Transform& transform) const
{
	SDL_Point pts[5];
	fVector p = m_transform * -vec2One;
	pts[0].x = (int)p.x;
	pts[0].y = (int)p.y;
	p = m_transform * fVector(-1.0f, 1.0f);
	pts[1].x = (int)p.x;
	pts[1].y = (int)p.y;
	p = m_transform * vec2One;
	pts[2].x = (int)p.x;
	pts[2].y = (int)p.y;
	p = m_transform * fVector(1.0f, -1.0f);
	pts[3].x = (int)p.x;
	pts[3].y = (int)p.y;
	pts[4] = pts[0];
	Renderer->AddToColliderDrawBuffer(pts, 5, 255, 0, 0);
}

Circle& Circle::transform(Circle& target, const Transform& t) const
{
    fVector scale = t.getScale();
    target.m_center.setMultiply(t, m_center);
    target.m_radius = m_radius * scale.x;
    return target;
}

void Circle::DebugDraw(const Transform& transform) const
{
	const int circleDefinition = 21;
	SDL_Point pts[circleDefinition];
	for(unsigned i = 0; i < circleDefinition; ++i)
	{
		float angle =static_cast<float>( 2 * M_PI * i / (circleDefinition - 1));
		pts[i].x = (int)(m_center.x + cosf(angle) * m_radius);
		pts[i].y = (int)(m_center.y + sinf(angle) * m_radius);
	}
	Renderer->AddToColliderDrawBuffer(pts, circleDefinition, 255, 0, 0);
}

Line& Line::transform(Line& target, const Transform& t) const
{
    target.m_p0.setMultiply(t, m_p0);
    target.m_p1.setMultiply(t, m_p1);
    return target;
}

void Line::DebugDraw(const Transform& transform) const
{
	SDL_Point pts[2];
	pts[0].x = (int)m_p0.x;
	pts[0].y = (int)m_p0.y;
	pts[1].x = (int)m_p1.x;
	pts[1].y = (int)m_p1.y;
	Renderer->AddToColliderDrawBuffer(pts, 2, 255, 0, 0);
}

bool inside(const AABB& b0, const fVector& p1)
{
    return b0.m_min.x <= p1.x && p1.x <= b0.m_max.x && 
           b0.m_min.y <= p1.y && p1.y <= b0.m_max.y;
}

bool inside(const OBB& o0, const fVector& p1)
{
    Transform inv;
    inv.invert(o0.m_transform);
    fVector invp = inv * p1;
    return !(invp.x < -1.0f || invp.x > 1.0f || invp.y < -1.0f || invp.y > 1.0f);
}

bool inside(const Circle& c0, const fVector& p1)
{
    return (p1 - c0.m_center).getLengthSq() <= sqrt(c0.m_radius);
}

bool inside(const Line& l0, const fVector& p1)
{
    fVector v0 = l0.m_p1 - l0.m_p0;
    float t = v0.dot(p1 - l0.m_p0) / v0.dot(v0);
    fVector closest = l0.m_p0 + v0 * t;
    if (!closest.equalWithEpsilon(p1))
        return false;
    return 0.0f <= t && t <= 1.0f;
}

// Closest point on a line segment to a point
fVector closestPoint(const Line& l0, fVector p)
{
	fVector l = l0.m_p1 - l0.m_p0;
    float t = l.dot(p - l0.m_p0) / l.dot(l);
    t = CLAMP(0.0f, 1.0f, t);
    return l0.m_p0 + l * t;
}

bool intersect(const AABB& b0, const AABB& b1)
{
    return !(b0.m_min.x > b1.m_max.x || b0.m_max.x < b1.m_min.x ||
             b0.m_min.y > b1.m_max.y || b0.m_max.y < b1.m_min.y);
}

fVector push(const AABB & b0, const AABB & b1)
{
	return fVector();
}

// returns true of the OBB is completely on one side of one of AABB's borders.
bool outside(const AABB& b0, const OBB& o1)
{
    using std::min; using std::max;
    fVector p00 = o1.m_transform * fVector(-1.0f, -1.0f);
	fVector p01 = o1.m_transform * fVector(-1.0f, 1.0f);
	fVector p10 = o1.m_transform * fVector(1.0f, -1.0f);
	fVector p11 = o1.m_transform * fVector(1.0f, 1.0f);
    float maxX = max(p00.x, max(p01.x, max(p10.x, p11.x)));
    float maxY = max(p00.y, max(p01.y, max(p10.y, p11.y)));
    if (maxX < b0.m_min.x || maxY < b0.m_min.y)
        return true;
    float minX = min(p00.x, min(p01.x, min(p10.x, p11.x)));
    float minY = min(p00.y, min(p01.y, min(p10.y, p11.y)));
    if (minX > b0.m_max.x || minY > b0.m_max.y)
        return true;
    return false;
}

bool intersect(const AABB& b0, const OBB& o1)
{
    if (outside(b0, o1))
        return false;
    Transform inv;
    inv.invert(o1.m_transform);
    OBB o0Inv;
    o0Inv = b0.transform(inv);
    return !outside(aabbUnit, o0Inv);
}

fVector push(const AABB & b0, const OBB & o1)
{
	return fVector();
}

bool intersect(const AABB& b0, const Circle& c1)
{
    fVector closest(CLAMP(b0.m_min.x, b0.m_max.x, c1.m_center.x),
		CLAMP(b0.m_min.y, b0.m_max.y, c1.m_center.y));
    return (closest - c1.m_center).getLengthSq() <= sqrt(c1.m_radius);
}

fVector push(const AABB & b0, const Circle & c1)
{
	return fVector();
}

bool intersect(const AABB& b0, const Line& l1)
{
    if (inside(b0, l1.m_p0) || inside(b0, l1.m_p1))
        return true;

    Line diag;
    if (l1.m_p0.x < l1.m_p1.x && l1.m_p0.y < l1.m_p1.y || l1.m_p0.x >= l1.m_p1.x && l1.m_p0.y >= l1.m_p1.y)
    {
        diag.m_p0 = fVector(b0.m_max.x, b0.m_min.y);
        diag.m_p1 = fVector(b0.m_min.x, b0.m_max.y);
    }
    else
    {
        diag.m_p0 = b0.m_min;
        diag.m_p1 = b0.m_max;
    }
    return intersect(diag, l1);
}

fVector push(const AABB & b0, const Line & l1)
{
	return fVector();
}

bool intersect(const OBB& o0, const OBB& o1)
{
    OBB o1Inv = o1.transform(o0.m_transform.getInverse());
    return intersect(aabbUnit, o1Inv);
}

fVector push(const OBB & o0, const OBB & o1)
{
	return fVector();
}

bool intersect(const OBB& o0, const Circle& c1)
{
    Transform o0Inv;
    o0Inv.invert(o0.m_transform);
    fVector scale = o0Inv.getScale();
    Transform rescale;
    rescale.setScale(fVector(1.0f, scale.x / scale.y));
    Transform transform = rescale * o0Inv;
    fVector scaleVec(1.0f, scale.x / scale.y);
    AABB b0(-scaleVec, scaleVec);
    return intersect(b0, c1.transform(transform));
}

fVector push(const OBB & o0, const Circle & c1)
{
	return fVector();
}

bool intersect(const OBB& o0, const Line& l1)
{
    Line l1Inv = l1.transform(o0.m_transform.getInverse());
    return intersect(aabbUnit, l1Inv);
}

fVector push(const OBB & o0, const Line & l1)
{
	return fVector();
}

bool intersect(const Circle& c0, const Circle& c1)
{
    return (c1.m_center - c0.m_center).getLengthSq() <= sqrt(c1.m_radius + c0.m_radius);
}

fVector push(const Circle & c0, const Circle & c1)
{
	float toMove = (c1.m_center - c0.m_center).getLengthSq() <= sqrt(c1.m_radius + c0.m_radius);
	fVector director = (c1.m_center - c0.m_center).normalize();
	return c1.m_center+director*toMove;
}

bool intersect(const Circle& c0, const Line& l1)
{
	fVector closest = closestPoint(l1, c0.m_center);
    return (closest - c0.m_center).getLengthSq() <= sqrt(c0.m_radius);
}

fVector push(const Circle & c0, const Line & l1)
{
	return fVector();
}

bool intersect(const Line& l0, const Line& l1)
{
	fVector v0 = l0.m_p1 - l0.m_p0;
	fVector v1 = l1.m_p1 - l1.m_p0;
	fVector v1n = v1.getPerp();
    float denom = v0.dot(v1n);
    if (EQEPSILON(denom, 0.0f, EPS))
    {
        // Parallel lines
        float t0 = v0.dot(l1.m_p0 - l0.m_p0) / v0.dot(v0);
		fVector closest = l0.m_p0 + v0 * t0;
        if (!closest.equalWithEpsilon(l1.m_p0))
            return false;
        float t1 = v0.dot(l1.m_p1 - l0.m_p0) / v0.dot(v0);
        if (t0 > t1)
            std::swap(t0, t1);
        return !(t1 < 0.0f || t0 > 1.0f);
    }
    float t0 = (l1.m_p0 - l0.m_p0).dot(v1n) / denom;
    if (t0 < 0.0f || t0 > 1.0f)
        return false;
	fVector v0n = v0.getPerp();
    float t1 = (l0.m_p0 - l1.m_p0).dot(v0n) / v1.dot(v0n);
    return 0.0f <= t1 && t1 <= 1.0f;
}

fVector push(const Line & l0, const Line & l1)
{
	return fVector();
}
