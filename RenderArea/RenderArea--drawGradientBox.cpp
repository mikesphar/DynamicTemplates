//
// $Id: RenderArea--drawGradientBox.cpp,v 1.1.1.1 2008/11/29 01:25:55 igor Exp $
//
// This file is part of DIY_Dynamic_Templates_V2
//
// DIY_Dynamic_Templates_V2 is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// DIY_Dynamic_Templates_V2 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Refer to the GNU General Public License in file "license.txt"
// Otherwise, see <http://www.gnu.org/licenses/>.


#include <QtDebug>  // this is here for debugging
#include <QPainterPath>

#include "RenderArea.h"

void RenderArea::drawGradientBox ( QPainter* painter, QRectF theRect, QColor theFirstColor, QColor theSecondColor, GradType whichWay, BarType whatKind )
{
	painter->save() ;

	// Qt's QPrinter PDF backend silently drops QLinearGradient brushes.
	// We simulate the gradient by painting thin solid-colour bands clipped
	// to the bar shape, which works identically on screen and in PDF output.

	// --- Build a clip path matching the bar shape ---
	QPainterPath clipPath ;
	switch ( (int)whatKind )
	{
		case BAR_ROUND:
			clipPath.moveTo ( theRect.left(), theRect.top() ) ;
			clipPath.lineTo ( theRect.right() - theRect.height(), theRect.top() ) ;
			clipPath.arcTo
			(	theRect.right() - theRect.height() - theRect.height(),
				theRect.top(),
				2 * theRect.height(),
				2 * theRect.height(),
				90.0, -88.0
			) ;
			clipPath.lineTo ( theRect.right(), theRect.bottom() ) ;
			clipPath.lineTo ( theRect.left(),  theRect.bottom() ) ;
			clipPath.closeSubpath() ;
			break ;

		case BAR_ROUND_RIGHT:
			clipPath.moveTo ( theRect.right(), theRect.top() ) ;
			clipPath.lineTo ( theRect.left() + theRect.height(), theRect.top() ) ;
			clipPath.arcTo
			(	theRect.left() + theRect.height() - theRect.height(),
				theRect.top(),
				2 * theRect.height(),
				2 * theRect.height(),
				90.0, 88.0
			) ;
			clipPath.lineTo ( theRect.left(),  theRect.bottom() ) ;
			clipPath.lineTo ( theRect.right(), theRect.bottom() ) ;
			clipPath.closeSubpath() ;
			break ;

		default: // BAR_RECT and anything else
			clipPath.addRect ( theRect ) ;
			break ;
	}

	painter->setClipPath ( clipPath, Qt::IntersectClip ) ;
	painter->setPen ( Qt::NoPen ) ;

	// --- Paint bands ---
	const int steps = 64 ;
	for ( int i = 0 ; i < steps ; i++ )
	{
		qreal t = (qreal)i / (qreal)( steps - 1 ) ;
		int r = qBound ( 0, (int)( theFirstColor.red()   + t * ( theSecondColor.red()   - theFirstColor.red()   ) ), 255 ) ;
		int g = qBound ( 0, (int)( theFirstColor.green() + t * ( theSecondColor.green() - theFirstColor.green() ) ), 255 ) ;
		int b = qBound ( 0, (int)( theFirstColor.blue()  + t * ( theSecondColor.blue()  - theFirstColor.blue()  ) ), 255 ) ;

		QRectF band ;
		switch ( (int)whichWay )
		{
			case GRAD_UP: {
				qreal h = theRect.height() / steps ;
				band = QRectF ( theRect.left(), theRect.bottom() - ( i + 1 ) * h, theRect.width(), h + 1 ) ;
				break ;
			}
			case GRAD_DOWN: {
				qreal h = theRect.height() / steps ;
				band = QRectF ( theRect.left(), theRect.top() + i * h, theRect.width(), h + 1 ) ;
				break ;
			}
			case GRAD_LEFT: {
				qreal w = theRect.width() / steps ;
				band = QRectF ( theRect.right() - ( i + 1 ) * w, theRect.top(), w + 1, theRect.height() ) ;
				break ;
			}
			case GRAD_RIGHT: {
				qreal w = theRect.width() / steps ;
				band = QRectF ( theRect.left() + i * w, theRect.top(), w + 1, theRect.height() ) ;
				break ;
			}
		}

		painter->fillRect ( band, QColor ( r, g, b ) ) ;
	}

	painter->restore() ;
}
