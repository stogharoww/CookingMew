#include "Line.h"
#include <QPainter>
#include <algorithm>

Line::Line(ColorScheme scheme)
    : _scheme(scheme)
{
    _mainColor = scheme.borderGet();
    _bounds = QRectF();
}

void Line::addLine(QPointF from, QPointF to)
{
    addLine(from, to, _mainColor);
}

void Line::addLine(QPointF from, QPointF to, QColor color)
{
    prepareGeometryChange(); // обязательно!

    _lines.append({from, to});
    _colors.append(color);

    updateBoundingRect();
}

void Line::updateBoundingRect()
{
    if (_lines.isEmpty()) {
        _bounds = QRectF();
        return;
    }

    qreal minX = _lines[0].first.x();
    qreal minY = _lines[0].first.y();
    qreal maxX = _lines[0].first.x();
    qreal maxY = _lines[0].first.y();

    for (const auto& l : _lines) {
        minX = std::min({minX, l.first.x(), l.second.x()});
        minY = std::min({minY, l.first.y(), l.second.y()});
        maxX = std::max({maxX, l.first.x(), l.second.x()});
        maxY = std::max({maxY, l.first.y(), l.second.y()});
    }

    // Добавляем толщину, чтобы линия не обрезалась
    _bounds = QRectF(QPointF(minX - 2, minY - 2),
                     QPointF(maxX + 2, maxY + 2));
}

QRectF Line::boundingRect() const
{
    return _bounds;
}

void Line::paint(QPainter* painter,
                 const QStyleOptionGraphicsItem*,
                 QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing);

    for (int i = 0; i < _lines.size(); i++) {
        QPen pen(_colors[i], 1);
        painter->setPen(pen);
        painter->drawLine(_lines[i].first, _lines[i].second);
    }
}
