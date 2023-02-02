#include "facet.h"

#include <QTextStream>

Facet::Facet(QVector3D *a, QVector3D *b, QVector3D *c)
{
    _vertices << a << b << c;
}

QVector3D Facet::normal() const
{
    auto e1 = *_vertices[1] - *_vertices[0];
    auto e2 = *_vertices[2] - *_vertices[0];
    return e1.crossProduct(e1, e2).normalized();
}

const QList<QVector3D *> &Facet::vertices() const
{
    return _vertices;
}

double Facet::distanceTo(QVector3D point) const
{
    auto diff = point - *(_vertices[0]);
	auto edge0 = *(_vertices[1]) - *(_vertices[0]);
	auto edge1 = *(_vertices[0]) - *(_vertices[2]);
	auto uab = diff.dotProduct(point - *(_vertices[0]), edge0)/edge0.lengthSquared();
	auto uca = diff.dotProduct(point - *(_vertices[2]), edge1)/edge1.lengthSquared();
//    Project = (p - A).Dot( Delta ) / LengthSquared;
//    var uab = EdgeAb.Project( p );
//    var uca = EdgeCa.Project( p );

    if (uca > 1 && uab < 0)
        return diff.length();

//    var ubc = EdgeBc.Project( p );
	auto edge2 = *(_vertices[2]) - *(_vertices[1]);
	auto ubc = diff.dotProduct(point - *(_vertices[1]), edge2)/edge2.lengthSquared();

    if (uab > 1 && ubc < 0)
        return (point - *(_vertices[1])).length();

    if (ubc > 1 && uca < 0)
        return (point - *(_vertices[2])).length();

	if ((uab > 0) && (uab < 1) && (ubc > 0) && (ubc < 1) && (uca > 0) && (uca < 1))
    {
		return fabs(point.distanceToPlane(*_vertices[0], normal()));
    }
	return qMin(fabs(point.distanceToLine(*(_vertices[0]), edge0.normalized())),
			qMin(fabs(point.distanceToLine(*(_vertices[1]), edge2.normalized())),
			fabs(point.distanceToLine(*(_vertices[2]), edge1.normalized()))));
//    if (ZeroToOne.Contains( uab ) && !PlaneAb.IsAbove( p ))
//        return EdgeAb.PointAt( uab );

//    if (ZeroToOne.Contains( ubc ) && !PlaneBc.IsAbove( p ))
//        return EdgeBc.PointAt( ubc );

//    if (ZeroToOne.Contains( uca ) && !PlaneCa.IsAbove( p ))
//        return EdgeCa.PointAt( uca );

    // The closest point is in the triangle so
    // project to the plane to find it
}
//    const auto zero = 0.;
//    const auto one = 1.;
//    const auto two = 2.;
// //   auto diff = *(_vertices[0]) - point;
////    auto edge0 = *(_vertices[1]) - *(_vertices[0]);
//    auto a00 = edge0.dotProduct(edge0, edge0);
//    auto a01 = edge0.dotProduct(edge0, edge1);
//    auto a11 = edge1.dotProduct(edge1, edge1);
//    auto b0 = diff.dotProduct(diff, edge0);
//    auto b1 = diff.dotProduct(diff, edge1);
//    auto det = qMax(a00 * a11 - a01 * a01, zero);
//    auto s = a01 * b1 - a11 * b0;
//    auto t = a01 * b0 - a00 * b1;

//    if (s + t <= det)
//    {
//        if (s < zero)
//        {
//            if (t < zero)  // region 4
//            {
//                if (b0 < zero)
//                {
//                    t = zero;
//                    if (-b0 >= a00)
//                    {
//                        s = one;
//                    }
//                    else
//                    {
//                        s = -b0 / a00;
//                    }
//                }
//                else
//                {
//                    s = zero;
//                    if (b1 >= zero)
//                    {
//                        t = zero;
//                    }
//                    else if (-b1 >= a11)
//                    {
//                        t = one;
//                    }
//                    else
//                    {
//                        t = -b1 / a11;
//                    }
//                }
//            }
//            else  // region 3
//            {
//                s = zero;
//                if (b1 >= zero)
//                {
//                    t = zero;
//                }
//                else if (-b1 >= a11)
//                {
//                    t = one;
//                }
//                else
//                {
//                    t = -b1 / a11;
//                }
//            }
//        }
//        else if (t < zero)  // region 5
//        {
//            t = zero;
//            if (b0 >= zero)
//            {
//                s = zero;
//            }
//            else if (-b0 >= a00)
//            {
//                s = one;
//            }
//            else
//            {
//                s = -b0 / a00;
//            }
//        }
//        else  // region 0
//        {
//            // minimum at interior point
//            s /= det;
//            t /= det;
//        }
//    }
//    else
//    {
//        auto tmp0{0.}, tmp1{0.}, numer{0.}, denom{0.};

//        if (s < zero)  // region 2
//        {
//            tmp0 = a01 + b0;
//            tmp1 = a11 + b1;
//            if (tmp1 > tmp0)
//            {
//                numer = tmp1 - tmp0;
//                denom = a00 - two * a01 + a11;
//                if (numer >= denom)
//                {
//                    s = one;
//                    t = zero;
//                }
//                else
//                {
//                    s = numer / denom;
//                    t = one - s;
//                }
//            }
//            else
//            {
//                s = zero;
//                if (tmp1 <= zero)
//                {
//                    t = one;
//                }
//                else if (b1 >= zero)
//                {
//                    t = zero;
//                }
//                else
//                {
//                    t = -b1 / a11;
//                }
//            }
//        }
//        else if (t < zero)  // region 6
//        {
//            tmp0 = a01 + b1;
//            tmp1 = a00 + b0;
//            if (tmp1 > tmp0)
//            {
//                numer = tmp1 - tmp0;
//                denom = a00 - two * a01 + a11;
//                if (numer >= denom)
//                {
//                    t = one;
//                    s = zero;
//                }
//                else
//                {
//                    t = numer / denom;
//                    s = one - t;
//                }
//            }
//            else
//            {
//                t = zero;
//                if (tmp1 <= zero)
//                {
//                    s = one;
//                }
//                else if (b0 >= zero)
//                {
//                    s = zero;
//                }
//                else
//                {
//                    s = -b0 / a00;
//                }
//            }
//        }
//        else  // region 1
//        {
//            numer = a11 + b1 - a01 - b0;
//            if (numer <= zero)
//            {
//                s = zero;
//                t = one;
//            }
//            else
//            {
//                denom = a00 - two * a01 + a11;
//                if (numer >= denom)
//                {
//                    s = one;
//                    t = zero;
//                }
//                else
//                {
//                    s = numer / denom;
//                    t = one - s;
//                }
//            }
//        }
//    }

//    QList<QVector3D>closest;
//    closest << point;
//    closest << *(_vertices[0]) + s * edge0 + t * edge1;
//    diff = closest[0] - closest[1];
//    auto sqrDistance = diff.dotProduct(diff, diff);
//    auto distance = std::sqrt(sqrDistance);
////    result.barycentric[0] = one - s - t;
////    result.barycentric[1] = s;
////    result.barycentric[2] = t;
//    return distance;
//}

bool Facet::operator==(const Facet &f) const
{
	return false;
	if (f.vertices().size() != _vertices.size())
		return false;
	for (auto &v1 : _vertices)
	{
		bool found{false};
		for (auto &v2 : f.vertices())
		{
			if (v2 == v1)
				found = true;
		}
		if (!found)
			return false;
	}
	return true;
}

bool Facet::isContaining(const QVector3D &p) const
{
	auto u = *_vertices[0] - *_vertices[1];//dif(p1,p2);
	auto v = *_vertices[2] - *_vertices[1];//dif(p3,p2);
	auto w = p - *_vertices[1];//dif(p,p2);
	auto l1 = u.dotProduct(u, w);//np.dot(u,w);
	if (l1 < 0)
		return false;
	auto l2 = v.dotProduct(v,w);
	if (l2 < 0)
		return false;
	u = *_vertices[0] - *_vertices[2];//dif(p1,p3);
	v = *_vertices[1] - *_vertices[2];//dif(p2,p3);
	w = p - *_vertices[2];//dif(p,p3);
	l1 = u.dotProduct(u, w);//np.dot(u,w);
	if (l1 < 0)
		return false;
	l2 = v.dotProduct(v,w);//np.dot(v,w);
	if (l2 < 0)
		return false;
	return true;
}

double Facet::surface() const
{
	auto e1 = *_vertices[1] - *_vertices[0];
	auto e2 = *_vertices[2] - *_vertices[0];
	return e1.crossProduct(e1, e2).length()/2.;
}

void Facet::setVertice(int i, QVector3D *v)
{
	_vertices[i] = v;
}

bool Facet::intersect(const QVector3D *a, const QVector3D *b) const
{
	//p1 = [10,0,0]
	//p2 = [0,10,0]
	//p3 = [0,0,10]
	//q0 = [10,13,15]
	//w  = [1,1,1]
    if (_vertices.contains(b))
    {
        return false;
    }
	auto u = *_vertices[0] - *_vertices[1];//dif(p1,p2);
	auto v = *_vertices[2] - *_vertices[1];//dif(p3,p2);
//    auto w = (*b - *a).normalized();
	auto n = u.crossProduct(u, v);//n  = list(np.cross(u,v))
    return (n.dotProduct(n, *a) * n.dotProduct(n, *b) <= 0);
//    auto p0 = *_vertices[0] - *a;//(*_vertices[0] + *_vertices[1] + *_vertices[2])/3.;//p0 = prod(add(add(p1,p2),p3),1/3.)
//	auto wDotn = w.dotProduct(w, n);
//	if (wDotn == 0.)
//		return false;
//    auto pi = *a + w * p0.dotProduct(p0, n)/wDotn;//  * - w * w.dotProduct(a - p0, n)/wDotn;//add(q0,prod(w,-np.dot(dif(q0,p0),n)/np.dot(w,n)))
//    if (w.lengthSquared() > (*b-pi).lengthSquared() && w.lengthSquared() > (*a-pi).lengthSquared())
//		return isContaining(pi);
//	return false;
//	//if isin(pi,p1,p2,p3):
//	//    print(pi, " internal point")
//	//else:
//	//    print(pi, " external point")
////	return true;
}

QTextStream &operator<<(QTextStream &out, const Facet &facet)
{
    out << "\tfacet normal " << facet.normal() << Qt::endl;
    out << "\t\touter loop" << Qt::endl;
	for (auto &x : facet.vertices())
    {
        out << "\t\t\tvertex " << (*x) <<Qt::endl;
    }
    out << "\t\tendloop" << Qt::endl;
    out << "\tendfacet" << Qt::endl;
    return out;
}

QTextStream &operator<<(QTextStream &out, const QVector3D &vertex)
{
    out << vertex.x() << " " << vertex.y() << " " << vertex.z();
    return out;
}
