#include "facet.h"

#include <QTextStream>
#include <Eigen/Geometry>

using Line2 = Eigen::Hyperplane<float,2>;
using Vec2  = Eigen::Vector2f;

Facet::Facet(QVector3D *a, QVector3D *b, QVector3D *c)
{
    if (!a || !b || !c)
        throw std::runtime_error("At least one point isn't defined");
    if (a == b || a == c || b == c)
        throw std::runtime_error("At least 2 points are the same");
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
		return (point - *(_vertices[0])).length();

//    var ubc = EdgeBc.Project( p );
	auto edge2 = *(_vertices[2]) - *(_vertices[1]);
	auto ubc = diff.dotProduct(point - *(_vertices[1]), edge2)/edge2.lengthSquared();

    if (uab > 1 && ubc < 0)
    {
        return (point - *(_vertices[1])).length();
    }
    if (ubc > 1 && uca < 0)
    {
        return (point - *(_vertices[2])).length();
    }
	if (ubc <= 1 && ubc >= 0)
	{
		auto aOnBC = diff.dotProduct(*(_vertices[0]) - *(_vertices[1]), edge2)/edge2.lengthSquared()* edge2 + *_vertices[1];
		if (point.dotProduct(point-*(_vertices[0]), aOnBC - *(_vertices[0]))/(aOnBC - *(_vertices[0])).lengthSquared() > 1)
			return fabs(point.distanceToLine(*(_vertices[1]), edge2.normalized()));
	}
	if (uca <= 1 && uca >= 0)
	{
		auto bOnCA = diff.dotProduct(*(_vertices[1]) - *(_vertices[2]), edge1)/edge1.lengthSquared()* edge1 + *_vertices[2];
		if (point.dotProduct(point-*(_vertices[1]), bOnCA - *(_vertices[1]))/(bOnCA - *(_vertices[1])).lengthSquared() > 1)
			return fabs(point.distanceToLine(*(_vertices[2]), edge1.normalized()));
	}
	if (uab <= 1 && uab >= 0)
	{
		auto cOnAB = diff.dotProduct(*(_vertices[2]) - *(_vertices[0]), edge0)/edge0.lengthSquared()* edge0 + *_vertices[0];
		if (point.dotProduct(point-*(_vertices[2]), cOnAB - *(_vertices[2]))/(cOnAB - *(_vertices[2])).lengthSquared() > 1)
			return fabs(point.distanceToLine(*(_vertices[0]), edge0.normalized()));
	}
//	if ((uab > 0) && (uab < 1) && (ubc > 0) && (ubc < 1) && (uca > 0) && (uca < 1))
//    {
		return fabs(point.distanceToPlane(*_vertices[0], normal()));
//    }
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
	if (f.vertices().size() != _vertices.size())
		return false;
	for (const auto &v1 : _vertices)
	{
		bool found{false};
		for (const auto &v2 : f.vertices())
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
	return fabs(e1.crossProduct(e1, e2).length())/2.;
}

void Facet::setVertice(int i, QVector3D *v)
{
	_vertices[i] = v;
}

QVector3D Facet::coordLocales(const QVector3D &p) const
{
	//On cherche les coordonnées de p exprimées en distance au plan et combinaison linéaire des 2 vecteurs
	auto x = p - *(_vertices[0]);
	auto ab = *(_vertices[1]) - *(_vertices[0]);
	auto ac = *(_vertices[2]) - *(_vertices[0]);
	QVector3D res;
	res.setZ(p.distanceToPlane(*(_vertices[0]), normal()));
	res.setX(ab.dotProduct(ab, x));
	res.setY(ac.dotProduct(ac, x));
	return res;
}

bool Facet::coteUnique(const Facet *f) const
{
	auto nbPointsCommuns = 0;
	for (const auto & v : _vertices)
		if (f->vertices().contains(v))
			nbPointsCommuns++;
	if (nbPointsCommuns > 1)
		return true;
	const auto &p = *(_vertices[0]);
	const auto &n = normal();
	auto cotes = 0.;
	auto nextCote = 0.;
	bool positif = true;
	auto curVertice = 0;
	while(curVertice < 3 && cotes == 0)
	{
		cotes = n.dotProduct(*(f->vertices()[curVertice]) - p, n);
		curVertice++;
	}
	if (cotes < 0)
	{
		positif = false;
	}
	while(curVertice < 3 && nextCote == 0)
	{
		nextCote = n.dotProduct(*(f->vertices()[curVertice]) - p, n);
		curVertice++;
	}
	cotes *= nextCote;
	//Les points d'une nouvelle facette intersectent une ancienne facette
	if (cotes < 0)
	{
		return false;
	}
	if (curVertice < 3)
	{
		cotes *= n.dotProduct(*(f->vertices()[2]) - p, n);
		if ((cotes <= 0 && positif) || (cotes >= 0 && !positif))
		{
			return false;
		}
	}
    return true;
}

QPair<QVector3D, QVector3D> Facet::commonLine(const Facet *f) const
{
    QPair<QVector3D, QVector3D>r;
    auto n1 = f->normal();
    auto n = normal();
    auto lineDir = n.crossProduct(n, n1);
    if (lineDir.isNull())
        return r;
	if (lineDir.dotProduct(lineDir, QVector3D(1, 1, 1)) < 0)
	{
		lineDir = -lineDir;
	}
	lineDir.normalize();
    //On calcule la droite d'intersection des plans des triangles
    QVector3D origin(0, 0, 0);
    auto p = origin.distanceToPlane(*(_vertices[0]), n);
    auto p1 = origin.distanceToPlane(*(f->vertices()[0]), n1);
//				n.x() * x + n.y() * y = -pH
//				n1.x() * x + n1.y() * y = -p1
    //				n1.y() * n.x() * x + n1.y() * n.y() * y = -n1.y() * pH
    //				n1.x() * n.y() * x + n1.y() * n.y() * y = -n.y() * p1
    //				(n1.y() * n.x() - n1.x() * n.y()) * x = n.y() * p1 - n1.y() * pH
    //				n1.x() * n.y() * x + n1.y() * n.y() * y = -n.y() * p1
    //				n.x() * x + n.z() * z = -pH
    //				n1.x() * x + n1.z() * z = -p1
    //				n1.z() * n.x() * x + n1.z() * n.z() * z = -n1.z() * pH
    //				n.z() * n1.x() * x + n1.z() * n.z() * z = -n.z() * p1
    // (n1.z() * n.x() - n.z() * n1.x) * x = n.z() *p1 - n1.z() * pH
    double x = 0;
    double y = 0;
    double z = 0;
    if (n1.y() * n.x() - n1.x() * n.y())
    {
        x = (n.y() * p1 - n1.y() * p) / (n1.y() * n.x() - n1.x() * n.y());
        y = n.y() ? (-p - n.x() * x)/n.y() : (-p1 - n1.x() * x)/n1.y();
    }
    else
    {
        x = (n.z() * p1 - n1.z() * p) / (n1.z() * n.x() - n1.x() * n.z());
        z = n.z() ? (-p - n.x() * x)/n.z() : (-p1 - n1.x() * x)/n1.z();
    }
    r.first = QVector3D(x, y, z);
    r.second = r.first+lineDir;
	return r;
}

QPair<QVector3D *, QVector3D *> Facet::commonEdge(const Facet *f) const
{
	QPair<QVector3D *, QVector3D *>r{nullptr, nullptr};
	for (const auto &v : _vertices)
	{
		if (f->vertices().contains(v))
		{
			if (!r.first)
				r.first = v;
			else
				r.second = v;
		}
	}
	return r;
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

bool Facet::intersect(const Facet *f) const
{
	if (coteUnique(f) || f->coteUnique(this))
    {
		return false;
    }
    auto line = commonLine(f);
    if (line.first.isNull() || line.second.isNull())
    {
        return false;
    }
	//Est-ce que les triangles se croisent sur cette ligne ?
    QList<float> listPoints;
    auto fLineA = f->coordLocales(line.first);
    auto fLineC = f->coordLocales(line.second);
    Vec2 a(fLineA.x(), fLineA.y());
    Vec2 c(fLineC.x(), fLineC.y());
	Vec2 b(0, 0);
	Vec2 d(1, 0);

    Line2 ac = Line2::Through(a,c);
	Line2 bd = Line2::Through(b,d);
    Vec2 e = d - b;
    Vec2 g = c - a;
    Vec2 numerateur = e;
    Vec2 denom = g;
    Vec2 pIntersect;
    if (!denom[0] || !denom[1])
    {
        numerateur = g;
        denom = e;
    }
    if ((!denom[0] || !denom[1]) || (numerateur[0]/denom[0] != numerateur[1]/denom[1]))
    {
        pIntersect = ac.intersection(bd);
        if ((pIntersect - b).norm() + (pIntersect-d).norm()<= (d-b).norm())
        {
            const auto dist = ac.signedDistance(pIntersect);
            listPoints << dist;
        }
    }
	d = Vec2(0, 1);

	bd = Line2::Through(b,d);
    e = d - b;
    numerateur = e;
    denom = g;
    if (!denom[0] || !denom[1])
    {
        numerateur = g;
        denom = e;
    }
    if ((!denom[0] || !denom[1]) || (numerateur[0]/denom[0] != numerateur[1]/denom[1]))
    {
        pIntersect = ac.intersection(bd);
        if ((pIntersect - b).norm() + (pIntersect-d).norm()<= (d-b).norm())
        {
            const auto dist = ac.signedDistance(pIntersect);
			if (!listPoints.contains(dist))
				listPoints << dist;
        }
    }
    b = Vec2(1, 0);
	bd = Line2::Through(b,d);
    e = d - b;
    numerateur = e;
    denom = g;
    if (!denom[0] || !denom[1])
    {
        numerateur = g;
        denom = e;
    }
    if ((!denom[0] || !denom[1]) || (numerateur[0]/denom[0] != numerateur[1]/denom[1]))
    {
        pIntersect = ac.intersection(bd);
        if ((pIntersect - b).norm() + (pIntersect-d).norm()<= (d-b).norm())
        {
            const auto dist = ac.signedDistance(pIntersect);
			if (!listPoints.contains(dist))
				listPoints << dist;
		}
    }
	if (listPoints.size() < 2)
	{
		return false;
	}
    fLineA = coordLocales(line.first);
    fLineC = coordLocales(line.second);
    QList<float> listPoints2;
    a = Vec2(fLineA.x(), fLineA.y());
    c = Vec2(fLineC.x(), fLineC.y());
	b = Vec2(0, 0);
	d = Vec2(1, 0);

    ac = Line2::Through(a,c);
	bd = Line2::Through(b,d);
    e = d - b;
    g = c - a;
    numerateur = e;
    denom = g;
    if (!denom[0] || !denom[1])
    {
        numerateur = g;
        denom = e;
    }
    if ((!denom[0] || !denom[1]) || (numerateur[0]/denom[0] != numerateur[1]/denom[1]))
    {
        pIntersect = ac.intersection(bd);
        if ((pIntersect - b).norm() + (pIntersect-d).norm()<= (d-b).norm())
        {
            const auto dist = ac.signedDistance(pIntersect);
            listPoints2 << dist;
        }
    }
	d = Vec2(0, 1);

	bd = Line2::Through(b,d);
    e = d - b;
    numerateur = e;
    denom = g;
    if (!denom[0] || !denom[1])
    {
        numerateur = g;
        denom = e;
    }
    if ((!denom[0] || !denom[1]) || (numerateur[0]/denom[0] != numerateur[1]/denom[1]))
    {
        pIntersect = ac.intersection(bd);
        if ((pIntersect - b).norm() + (pIntersect-d).norm()<= (d-b).norm())
        {
            const auto dist = ac.signedDistance(pIntersect);
			if (!listPoints2.contains(dist))
				listPoints2 << dist;
		}
    }
	b = Vec2(1, 0);
	bd = Line2::Through(b,d);
    e = d - b;
    numerateur = e;
    denom = g;
    if (!denom[0] || !denom[1])
    {
        numerateur = g;
        denom = e;
    }
    if ((!denom[0] || !denom[1]) || (numerateur[0]/denom[0] != numerateur[1]/denom[1]))
    {
        pIntersect = ac.intersection(bd);
        if ((pIntersect - b).norm() + (pIntersect-d).norm()<= (d-b).norm())
        {
            const auto dist = ac.signedDistance(pIntersect);
			if (!listPoints2.contains(dist))
				listPoints2 << dist;
		}
    }
	if (listPoints2.size()<2)
	{
		return false;
	}
	std::sort(listPoints.begin(), listPoints.end());
    if (fabs(listPoints.first()-listPoints.last()) < 1e-6)
        return false;
    std::sort(listPoints2.begin(), listPoints2.end());
    if (fabs(listPoints2.first()-listPoints2.last()) < 1e-6)
        return false;
    if (listPoints.last() > listPoints2.first() && listPoints2.last() > listPoints.first())
	{
		return true;
	}
	return false;
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
