#include "facet.h"

#include <QTextStream>

Facet::Facet(QVector3D *a, QVector3D *b, QVector3D *c)
{
    _vertices << a << b << c;
}

QVector3D Facet::normal() const
{
    return QVector3D(0., 0., -1.);
}

const QList<QVector3D *> &Facet::vertices() const
{
    return _vertices;
}

double Facet::distanceTo(QVector3D point) const
{
    const auto zero = 0.;
    const auto one = 1.;
    const auto two = 2.;
    auto diff = *(_vertices[0]) - point;
    auto edge0 = *(_vertices[1]) - *(_vertices[0]);
    auto edge1 = *(_vertices[2]) - *(_vertices[0]);
    auto a00 = edge0.dotProduct(edge0, edge0);
    auto a01 = edge0.dotProduct(edge0, edge1);
    auto a11 = edge1.dotProduct(edge1, edge1);
    auto b0 = diff.dotProduct(diff, edge0);
    auto b1 = diff.dotProduct(diff, edge1);
    auto det = qMax(a00 * a11 - a01 * a01, zero);
    auto s = a01 * b1 - a11 * b0;
    auto t = a01 * b0 - a00 * b1;

    if (s + t <= det)
    {
        if (s < zero)
        {
            if (t < zero)  // region 4
            {
                if (b0 < zero)
                {
                    t = zero;
                    if (-b0 >= a00)
                    {
                        s = one;
                    }
                    else
                    {
                        s = -b0 / a00;
                    }
                }
                else
                {
                    s = zero;
                    if (b1 >= zero)
                    {
                        t = zero;
                    }
                    else if (-b1 >= a11)
                    {
                        t = one;
                    }
                    else
                    {
                        t = -b1 / a11;
                    }
                }
            }
            else  // region 3
            {
                s = zero;
                if (b1 >= zero)
                {
                    t = zero;
                }
                else if (-b1 >= a11)
                {
                    t = one;
                }
                else
                {
                    t = -b1 / a11;
                }
            }
        }
        else if (t < zero)  // region 5
        {
            t = zero;
            if (b0 >= zero)
            {
                s = zero;
            }
            else if (-b0 >= a00)
            {
                s = one;
            }
            else
            {
                s = -b0 / a00;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            s /= det;
            t /= det;
        }
    }
    else
    {
        auto tmp0{0.}, tmp1{0.}, numer{0.}, denom{0.};

        if (s < zero)  // region 2
        {
            tmp0 = a01 + b0;
            tmp1 = a11 + b1;
            if (tmp1 > tmp0)
            {
                numer = tmp1 - tmp0;
                denom = a00 - two * a01 + a11;
                if (numer >= denom)
                {
                    s = one;
                    t = zero;
                }
                else
                {
                    s = numer / denom;
                    t = one - s;
                }
            }
            else
            {
                s = zero;
                if (tmp1 <= zero)
                {
                    t = one;
                }
                else if (b1 >= zero)
                {
                    t = zero;
                }
                else
                {
                    t = -b1 / a11;
                }
            }
        }
        else if (t < zero)  // region 6
        {
            tmp0 = a01 + b1;
            tmp1 = a00 + b0;
            if (tmp1 > tmp0)
            {
                numer = tmp1 - tmp0;
                denom = a00 - two * a01 + a11;
                if (numer >= denom)
                {
                    t = one;
                    s = zero;
                }
                else
                {
                    t = numer / denom;
                    s = one - t;
                }
            }
            else
            {
                t = zero;
                if (tmp1 <= zero)
                {
                    s = one;
                }
                else if (b0 >= zero)
                {
                    s = zero;
                }
                else
                {
                    s = -b0 / a00;
                }
            }
        }
        else  // region 1
        {
            numer = a11 + b1 - a01 - b0;
            if (numer <= zero)
            {
                s = zero;
                t = one;
            }
            else
            {
                denom = a00 - two * a01 + a11;
                if (numer >= denom)
                {
                    s = one;
                    t = zero;
                }
                else
                {
                    s = numer / denom;
                    t = one - s;
                }
            }
        }
    }

    QList<QVector3D>closest;
    closest << point;
    closest << *(_vertices[0]) + s * edge0 + t * edge1;
    diff = closest[0] - closest[1];
    auto sqrDistance = diff.dotProduct(diff, diff);
    auto distance = std::sqrt(sqrDistance);
//    result.barycentric[0] = one - s - t;
//    result.barycentric[1] = s;
//    result.barycentric[2] = t;
    return distance;
}

bool Facet::operator==(const Facet &f) const
{
    if (f.vertices().size() != _vertices.size())
        return false;
    for (const auto &v1 : _vertices)
    {
        bool found{false};
        for (const auto &v2 : f.vertices())
        {
            if (*v2 == *v1)
                found = true;
        }
        if (!found)
            return false;
    }
    return true;
}

QTextStream &operator<<(QTextStream &out, const Facet &facet)
{
    out << "\tfacet normal " << facet.normal() << Qt::endl;
    out << "\t\touter loop" << Qt::endl;
    for (const auto &x : facet.vertices())
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
