#ifndef QT_HELPER_201307191431_H
#define QT_HELPER_201307191431_H

#include <vector>
#include <cmath>
#include <assert.h>

#ifndef qreal 
	#define qreal float
#endif

#ifndef qAbs
	#define qAbs abs
#endif

#ifndef qFastSin
	#define qFastSin sinf
#endif

#ifndef qFastCos
	#define qFastCos cosf
#endif

#ifndef QDataBuffer
	#define QDataBuffer std::vector
#endif

#ifndef QVarLengthArray
	#define QVarLengthArray std::vector
#endif

#ifndef Q_PI
	#define Q_PI 3.1415926f
#endif

#ifndef Q_ASSERT
	#define Q_ASSERT assert
#endif

#ifndef QT_BEGIN_NAMESPACE 
	#define QT_BEGIN_NAMESPACE namespace Qt {
#endif

#ifndef QT_END_NAMESPACE
	#define QT_END_NAMESPACE } // namespace Qt
#endif

QT_BEGIN_NAMESPACE

template <typename Type> Type qMax(Type a, Type b) 
{
	return a < b ? b : a;
}

template <typename Type> Type qMin(Type a, Type b) 
{
	return a < b ? a : b;
}

static inline bool qFuzzyIsNull(float f)
{
	return qAbs(f) <= 0.00001f;
}

enum PenJoinStyle
{
	BevelJoin = 0,
	SvgMiterJoin = 1,
	MiterJoin = 2,
	RoundJoin = 3	
};

enum PenCapStyle
{
	FlatCap = 0,
	SquareCap = 1,
	RoundCap = 2
};

QT_END_NAMESPACE

#endif
