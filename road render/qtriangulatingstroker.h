#ifndef QTRIANGULATINGSTROKER_201307191412_H
#define QTRIANGULATINGSTROKER_201307191412_H

#include "qhelper.h"
#include "qbase.h"

QT_BEGIN_NAMESPACE

class QTriangulatingStroker
{
public:
    void process(const QVectorPath &path, const QPen &pen);

    inline int vertexCount() const { return m_vertices.size(); }

	inline const float *vertices() const { return m_vertices.empty() ? NULL : &m_vertices[0]; }

    inline void setInvScale(qreal invScale) { m_inv_scale = invScale; }

	inline void clear() { m_vertices.clear(); };

private:
    inline void emitLineSegment(float x, float y, float nx, float ny);
    void moveTo(const qreal *pts);
    inline void lineTo(const qreal *pts);
    void cubicTo(const qreal *pts);
    void join(const qreal *pts);
    inline void normalVector(float x1, float y1, float x2, float y2, float *nx, float *ny);
    void endCap(const qreal *pts);
    void arcPoints(float cx, float cy, float fromX, float fromY, float toX, float toY, QVarLengthArray<float> &points);
    void endCapOrJoinClosed(const qreal *start, const qreal *cur, bool implicitClose, bool endsAtStart);


    QDataBuffer<float> m_vertices;

    float m_cx, m_cy;           // current points
    float m_nvx, m_nvy;         // normal vector...
    float m_width;
    qreal m_miter_limit;

    int m_roundness;            // Number of line segments in a round join
    qreal m_sin_theta;          // sin(m_roundness / 360);
    qreal m_cos_theta;          // cos(m_roundness / 360);
    qreal m_inv_scale;
    float m_curvyness_mul;
    float m_curvyness_add;

    Qt::PenJoinStyle m_join_style;
    Qt::PenCapStyle m_cap_style;
};

inline float qpen_widthf (const QPen& p) 
{
	return p.width();
}

inline Qt::PenJoinStyle qpen_joinStyle(const QPen& p)
{
	return p.joinStyle();
}

inline Qt::PenCapStyle qpen_capStyle(const QPen& p)
{
	return p.capStyle();
}

inline void QTriangulatingStroker::normalVector(float x1, float y1, float x2, float y2,
                                                float *nx, float *ny)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    float pw;

    if (dx == 0)
        pw = m_width / qAbs(dy);
    else if (dy == 0)
        pw = m_width / qAbs(dx);
    else
        pw = m_width / sqrt(dx*dx + dy*dy);

    *nx = -dy * pw;
    *ny = dx * pw;
}

inline void QTriangulatingStroker::emitLineSegment(float x, float y, float vx, float vy)
{
	m_vertices.push_back(x + vx);
	m_vertices.push_back(y + vy);
	m_vertices.push_back(x - vx);
	m_vertices.push_back(y - vy);

//     m_vertices.add(x + vx);
//     m_vertices.add(y + vy);
//     m_vertices.add(x - vx);
//     m_vertices.add(y - vy);
}

void QTriangulatingStroker::lineTo(const qreal *pts)
{
    emitLineSegment(pts[0], pts[1], m_nvx, m_nvy);
    m_cx = pts[0];
    m_cy = pts[1];
}

QT_END_NAMESPACE

#endif
