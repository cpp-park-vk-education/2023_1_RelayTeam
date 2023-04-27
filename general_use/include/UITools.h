#include <QtMath>

inline qreal getFontScaling(qreal scaling) {
	return (scaling > 1 ? qSqrt(scaling) : scaling);
}
