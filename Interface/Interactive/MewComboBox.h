#pragma once

#include <QGraphicsObject>
#include <QVector>
#include <QString>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QFontDatabase>
#include <QFont>
#include "../ColorScheme.h"

class PopupRect;

class MewComboBox : public QGraphicsObject
{
    Q_OBJECT

public:
    MewComboBox(ColorScheme& scheme, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) override;

    void setSize(qreal w, qreal h);

    void addItem(const QString& text, int data);
    int count() const { return items.size(); }

    QString itemText(int index) const;
    int itemData(int index) const;

    void setCurrentIndex(int index);
    int currentIndex() const { return current; }

    QString currentText() const;
    int currentData() const;

signals:
    void changed(int index);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    struct Item {
        QString text;
        int data;
    };

    QVector<Item> items;
    int current = 0;

    qreal width = 120;
    qreal height = 30;

    ColorScheme scheme;

    // === Popup ===
    bool popupVisible = false;
    PopupRect* popupRect = nullptr;

    int hoveredIndex = -1;

    // === FONT ===
    QFont font;

    void showPopup();
    void hidePopup();
    void rebuildPopup();
    void raiseZ();
    void restoreZ();

    static MewComboBox* openedCombo;

    friend class PopupRect;
};
