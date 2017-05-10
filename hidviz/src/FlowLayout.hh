/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>
#include <QStyle>

/**
 * Dynamic layout.
 *
 * This layout will try to old as much widgets as it can in one row.
 * When there's not enough space, widgets will overflow into next row.
 * This layout is slightly modified from the one in Qt examples as it lays
 * widgets in grid.
 */
class FlowLayout : public QLayout
{
public:
    /**
     * Constructs FlowLayout instance.
     * @param margin Margin around the layout
     * @param hSpacing Horizontal spacing between widgets
     * @param vSpacing Vertical spacing between widgets
     * @param parent Parent widget
     */
    explicit FlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1, QWidget* parent = nullptr);

    /// Destructs FlowLayout
    ~FlowLayout() override;

    /**
     * Adds item into layout.
     * @param item LayoutItem to be added
     */
    void addItem(QLayoutItem *item) override;

    /**
     * Returns horizontal spacing.
     * @return Horizontal spacing of this layout
     */
    int horizontalSpacing() const;


    /**
     * Returns vertical spacing.
     * @return Vertical spacing of this layout
     */
    int verticalSpacing() const;

    /**
     * Returns expanding directions.
     * @return Expanding directions
     */
    Qt::Orientations expandingDirections() const override;

    /**
     * Returns true if this layout has height for width.
     * @return Height for width availability (true for FlowLayout)
     */
    bool hasHeightForWidth() const override;

    /**
     * Returns height for width given.
     * @param width Input width to be calculated with
     * @return Height for given width
     */
    int heightForWidth(int width) const override;

    /**
     * Returns count of children.
     * @return Count of children
     */
    int count() const override;

    /**
     * Returns item at specified index.
     * @param index Index from which the child will be returned
     * @return Child item at given index
     */
    QLayoutItem *itemAt(int index) const override;

    /**
     * Returns layout's minimum size.
     * @return Layout's minimum size.
     */
    QSize minimumSize() const override;

    /**
     * Sets geometry of this widget.
     * @param rect Target rectangle, where this layout will be drawn.
     */
    void setGeometry(const QRect &rect) override;

    /**
     * Returns size hint of this layout.
     * @return Returns layout's size hint.
     */
    QSize sizeHint() const override;

    /**
     * Removes child item at specified index.
     * @param index Index from which the child will be removed
     * @return Removed child
     */
    QLayoutItem *takeAt(int index) override;

private:
    /// List of child items
    QList<QLayoutItem *> itemList;

    /// Horizontal spacing
    int m_hSpace;

    /// Vertical spacing
    int m_vSpace;
    /**
     * Performs  calculation of children' positions.
     * @param rect Target rectangle
     * @param testOnly If set to true, the actual positions will not be passed to children
     * @return Layout height
     */
    int doLayout(const QRect &rect, bool testOnly) const;

    /**
     * Returns spacing
     * @param pm
     * @return
     */
    int smartSpacing(QStyle::PixelMetric pm) const;
};

#endif // FLOWLAYOUT_H
