#ifndef QBASE_201307191434_H
#define QBASE_201307191434_H

#include "qhelper.h"

QT_BEGIN_NAMESPACE

class QPointF
{
public:
	QPointF(float x, float y)
		: m_x(x), m_y(y)
	{}

	float& x() { return m_x; }
	float& y() { return m_y; }

	float x() const { return m_x; }
	float y() const { return m_y; }

private:

	float m_x, m_y;
};

class QRectF
{
public:

	QRectF()
		: m_minx(0), m_miny(0), m_maxx(0), m_maxy(0)
	{}

	QRectF(float minx, float miny, float maxx, float maxy)
		: m_minx(minx), m_miny(miny), m_maxx(maxx), m_maxy(maxy)
	{}
	
	float width() const { return m_maxx - m_minx; }

	float height() const { return m_maxy - m_miny; }

private:
	
	qreal m_minx, m_miny, m_maxx, m_maxy;

};

class QBezier
{
public:
	
	QBezier() 
	{}

	static QBezier fromPoints(const QPointF &p0, const QPointF &p1, const QPointF &p2, const QPointF &p3);
	
	static void coefficients(qreal t, qreal &a, qreal &b, qreal &c, qreal &d);

	QRectF bounds() const;

	QPointF pointAt(qreal t) const;

	QPointF pt1() const { return QPointF(x1, y1); }
	QPointF pt2() const { return QPointF(x2, y2); }
	QPointF pt3() const { return QPointF(x3, y3); }
	QPointF pt4() const { return QPointF(x4, y4); }

private:

	QRectF m_bound;

	qreal x1, y1, x2, y2, x3, y3, x4, y4;

};

inline void QBezier::coefficients(qreal t, qreal &a, qreal &b, qreal &c, qreal &d)
{
	qreal m_t = 1. - t;
	b = m_t * m_t;
	c = t * t;
	d = c * t;
	a = b * m_t;
	b *= 3. * t;
	c *= 3. * m_t;
}

inline QPointF QBezier::pointAt(qreal t) const
{
#if 1
	qreal a, b, c, d;
	coefficients(t, a, b, c, d);
	return QPointF(a*x1 + b*x2 + c*x3 + d*x4, a*y1 + b*y2 + c*y3 + d*y4);
#else
	// numerically more stable:
	qreal m_t = 1. - t;
	qreal a = x1*m_t + x2*t;
	qreal b = x2*m_t + x3*t;
	qreal c = x3*m_t + x4*t;
	a = a*m_t + b*t;
	b = b*m_t + c*t;
	qreal x = a*m_t + b*t;
	qreal a = y1*m_t + y2*t;
	qreal b = y2*m_t + y3*t;
	qreal c = y3*m_t + y4*t;
	a = a*m_t + b*t;
	b = b*m_t + c*t;
	qreal y = a*m_t + b*t;
	return QPointF(x, y);
#endif
}

inline QRectF QBezier::bounds() const
{
	qreal xmin = x1;
	qreal xmax = x1;
	if (x2 < xmin)
		xmin = x2;
	else if (x2 > xmax)
		xmax = x2;
	if (x3 < xmin)
		xmin = x3;
	else if (x3 > xmax)
		xmax = x3;
	if (x4 < xmin)
		xmin = x4;
	else if (x4 > xmax)
		xmax = x4;

	qreal ymin = y1;
	qreal ymax = y1;
	if (y2 < ymin)
		ymin = y2;
	else if (y2 > ymax)
		ymax = y2;
	if (y3 < ymin)
		ymin = y3;
	else if (y3 > ymax)
		ymax = y3;
	if (y4 < ymin)
		ymin = y4;
	else if (y4 > ymax)
		ymax = y4;
	return QRectF(xmin, ymin, xmax-xmin, ymax-ymin);
}

#define GET_INTERFACE_DECLARE(type, fname) { type fname const { return m_##fname; } }

class QPen
{
public:

	QPen()
		: m_miter_limit(2.0f), m_width(2.0f)
	{}

	qreal miterLimit() const { return m_miter_limit; };
	void setMiterLimit(qreal limit) { m_miter_limit = limit; }

	bool isCosmetic() const { return m_cosmetic; }

	qreal width() const { return m_width; }
	void setWidth(qreal width) { m_width = width; }

	Qt::PenCapStyle capStyle() const { return m_capStyle; }
	void setCapStyle(Qt::PenCapStyle pcs) { m_capStyle = pcs; }

	Qt::PenJoinStyle joinStyle() const { return m_joinStyle; }	
	void setJoinStyle(Qt::PenJoinStyle pjs) { m_joinStyle = pjs; }

private:

	Qt::PenCapStyle m_capStyle;
	Qt::PenJoinStyle m_joinStyle;	
	qreal m_width;
	qreal m_miter_limit;
	unsigned int m_cosmetic : 1;

};

class QPainterPath
{
public:

	enum ElementType
	{
		MoveToElement = 0,
		LineToElement = 1,
		CurveToElement = 2
	};

};

class QVectorPath
{
public:

	const qreal* points() const { return m_points.empty() ? NULL : &m_points[0]; }

	const QPainterPath::ElementType* elements() const { return m_elements.empty() ? NULL : &m_elements[0]; }

	int elementCount() const { return m_elements.size(); }

	bool hasImplicitClose() const { return m_has_close; }

	void setPoints(qreal* points, int pointCount)
	{		
		for (qreal* ptr = points; ptr < points+pointCount; ptr++)
			m_points.push_back(*ptr);
	}

	void moveTo(qreal px, qreal py) 
	{
		addPoint(px, py);
		addElement(Qt::QPainterPath::MoveToElement);
	} 

	void lineTo(qreal px, qreal py) 
	{
		addPoint(px, py);
		addElement(Qt::QPainterPath::LineToElement);
	} 

	void curveTo(qreal px, qreal py) 
	{
		addPoint(px, py);
		addElement(Qt::QPainterPath::CurveToElement);
	} 

	void clear() { m_points.clear(); m_elements.clear(); }
	
	bool isEmpty() { return m_points.empty() || m_elements.empty(); }

private:

	void addPoint(qreal px, qreal py) 
	{
		m_points.push_back(px);
		m_points.push_back(py);
	}

	void addElement(QPainterPath::ElementType element)
	{
		m_elements.push_back(element);
	}

	bool m_has_close;
	
	QDataBuffer<qreal>	m_points;
	QDataBuffer<QPainterPath::ElementType> m_elements;

};

QT_END_NAMESPACE

#endif
