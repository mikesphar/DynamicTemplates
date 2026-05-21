//
// RenderArea--drawTitlePage.cpp
//
// This file is part of DIY_Dynamic_Templates_V2
//
// DIY_Dynamic_Templates_V2 is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// DIY_Dynamic_Templates_V2 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// Refer to the GNU General Public License in file "license.txt"
// Otherwise, see <http://www.gnu.org/licenses/>.

#include <QPainter>
#include "RenderArea.h"

// ---------------------------------------------------------------------------
// drawTitlePage
//
// Renders a full-page title page suitable for use as the first leaf of a
// double-sided printed planner.  The design stays within the same visual
// language as the rest of the document: Blue Highway / Monaco fonts, the
// standard grey palette, and gradient-bar accents.
//
// Layout (top → bottom, within thePage margins):
//
//   ┌─────────────────────────────────────────┐  ← thePage.top()
//   │  HEADER BAR  "DIY Planner"  (2.5 dLS)  │
//   ├─────────────────────────────────────────┤
//   │  blank space (~3 dLS)                   │
//   │  YEAR   "2026"  or  "2026–27"  (8 dLS) │
//   │  blank space (~0.5 dLS)                 │
//   │  thin rule  (0.35 dLS)                  │
//   │  blank space (~1.5 dLS)                 │
//   │  DATE RANGE  "January – December 2026"  │
//   │            (2 dLS)                      │
//   │  remaining space (white)                │
//   ├─────────────────────────────────────────┤
//   │  FOOTER BAR  (0.75 dLS)                 │
//   └─────────────────────────────────────────┘  ← thePage.bottom()
// ---------------------------------------------------------------------------

void RenderArea::drawTitlePage ( QPainter* painter )
{
    const QChar   emdash  ( 0x2014 ) ;   // —
    const QChar   endash  ( 0x2013 ) ;   // –  (used in year spans)

    QFont werdz ( "Blue Highway" ) ;
    QFont dayz  ( "Monaco" ) ;

    QPen   thePen   ( Qt::white, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) ;
    QBrush theBrush ( Qt::white, Qt::SolidPattern ) ;

    // ── Erase the page ───────────────────────────────────────────────────────
    painter->setBrush ( theBrush ) ;
    painter->setPen   ( thePen   ) ;
    painter->drawRect ( thePaper ) ;

    QRectF page = thePage ;

    // ── Header bar: "DIY Planner" ────────────────────────────────────────────
    qreal bannerH = 2.5 * displayLineSpacing ;
    QRectF bannerRect ( page.x(), page.y(), page.width(), bannerH ) ;

    drawGradientBox ( painter, bannerRect, lineColors[0], lineColors[3], GRAD_DOWN, BAR_RECT ) ;

    thePen.setColor ( Qt::white ) ;
    painter->setPen ( thePen ) ;
    painter->setFont ( werdz ) ;
    fitStringInRect ( bannerRect, tr("DIY Planner"), painter,
                      Qt::AlignVCenter | Qt::AlignHCenter, 0.62 ) ;

    // ── Year (very large, Monaco) ─────────────────────────────────────────────
    qreal yearTop = page.y() + bannerH + 3.0 * displayLineSpacing ;
    QRectF yearRect ( page.x(), yearTop, page.width(), 8.0 * displayLineSpacing ) ;

    thePen.setColor ( lineColors[0] ) ;
    painter->setPen ( thePen ) ;
    painter->setFont ( dayz ) ;

    QString yearStr ;
    if ( startDate.year() == stopDate.year() )
    {   yearStr = startDate.toString ( "yyyy" ) ;
    }
    else
    {   // e.g. "2026–27"
        yearStr = startDate.toString ( "yyyy" ) + endash + stopDate.toString ( "yy" ) ;
    }
    fitStringInRect ( yearRect, yearStr, painter, Qt::AlignCenter, 0.88 ) ;

    // ── Thin horizontal rule ──────────────────────────────────────────────────
    qreal ruleTop = yearTop + 8.5 * displayLineSpacing ;
    QRectF ruleRect ( page.x(), ruleTop, page.width(), 0.35 * displayLineSpacing ) ;
    drawGradientBox ( painter, ruleRect, lineColors[2], lineColors[5], GRAD_DOWN, BAR_RECT ) ;

    // ── Date range (Blue Highway) ─────────────────────────────────────────────
    qreal dateTop = ruleTop + 1.5 * displayLineSpacing ;
    QRectF dateRect ( page.x(), dateTop, page.width(), 2.0 * displayLineSpacing ) ;

    // Build a concise description of the selected date range.
    // The year is already displayed prominently above, so omit it whenever
    // the entire range falls within a single calendar year.
    //
    //   Same day               →  "May 20"
    //   Same month & year      →  "May"
    //   Same year, diff months →  "May — December"
    //   Different years        →  "May 2026 — April 2027"  (years needed here)
    //
    QString dateRange ;
    if ( startDate == stopDate )
    {   // Single day — omit year, already shown above
        dateRange = startDate.toString ( "MMMM d" ) ;
    }
    else if (   startDate.year()  == stopDate.year()
             && startDate.month() == stopDate.month() )
    {   // Same month — just the month name
        dateRange = startDate.toString ( "MMMM" ) ;
    }
    else if ( startDate.year() == stopDate.year() )
    {   // Same year, different months — omit year
        dateRange = startDate.toString ( "MMMM" ) + " " + emdash + " "
                  + stopDate.toString  ( "MMMM" ) ;
    }
    else
    {   // Spans a year boundary — both years needed
        dateRange = startDate.toString ( "MMMM yyyy" ) + " " + emdash + " "
                  + stopDate.toString  ( "MMMM yyyy" ) ;
    }

    thePen.setColor ( lineColors[1] ) ;
    painter->setPen ( thePen ) ;
    painter->setFont ( werdz ) ;
    fitStringInRect ( dateRect, dateRange, painter, Qt::AlignCenter, 0.85 ) ;

    // ── Footer accent bar ─────────────────────────────────────────────────────
    qreal footerH = 0.75 * displayLineSpacing ;
    QRectF footerRect ( page.x(), page.bottom() - footerH, page.width(), footerH ) ;
    drawGradientBox ( painter, footerRect, lineColors[3], lineColors[6], GRAD_DOWN, BAR_RECT ) ;
}
