// Copyright (c) 2015-2017  
// Faculty of Sciences of the University of Porto (Portugal). All rights reserved. 
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licenses holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Catarina Lobo Ferreira <up201105249@fc.up.pt>, DCC/FCUP
//                 Ana Paula Tomás <apt@dcc.fc.up.pt>, DCC/FCUP


#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPolygon>
#include <QPainter>

#include <CGAL/ogons_generators_2.h>
#include <CGAL/Cartesian.h>


class Window : public QWidget {

private:
	typedef CGAL::Cartesian<int>             Kernel;
	typedef Kernel::Point_2                  Point_2;
	typedef CGAL::Grid_ogon_2                Grid_ogon_2;
	typedef CGAL::Random                     Random;

	Grid_ogon_2 grid_ogon;
	QPolygon ogon;

	bool draw;

	void paintEvent(QPaintEvent*);

public:
	Window();

	void genOgonFromGridOgon(Grid_ogon_2 grid_ogon);
};

inline Window::Window() : QWidget() {

	this->setFixedSize(400, 400);
	this->setWindowTitle("Ogons_Generators_2");
	this->setWindowIcon(QIcon("cgal.ico"));

	draw = false;
}


inline void Window::genOgonFromGridOgon(Grid_ogon_2 grid_ogon) {

	draw = true;

	this->grid_ogon = grid_ogon;
	this->update();	
}

inline void Window::paintEvent(QPaintEvent*) {

	QPainter painter(this);
    painter.setBrush(QBrush(QColor(133,198,208), Qt::SolidPattern));

    if(!draw) {
    	painter.drawPolygon(ogon);
    	return;
    }

    Random rnd;
    int n_vertices = grid_ogon.get_number_of_vertices();
    Point_2 max_coords(rnd.uniform_int(n_vertices/2-1,2*n_vertices-1), rnd.uniform_int(n_vertices/2-1,2*n_vertices-1));
	float col_prob = rnd.uniform_real<float>();
	int s = rnd.uniform_int(0, 2*n_vertices);

    std::vector<Point_2> p;
    QVector<QPoint> points;

	p.resize(n_vertices);

	CGAL::random_ogon_from_grid_ogon_2(grid_ogon, max_coords, col_prob, s, rnd).get_polygon_2(p.begin());

	Point_2 m(0,0);
	for(std::vector<Point_2>::iterator it=p.begin(); it!=p.end(); ++it) {
		m = Point_2(std::max(m.x(), it->x()), std::max(m.y(), it->y()));
	}
	Point_2 c(300/m.x(), 300/m.y());
		
	for(std::vector<Point_2>::iterator it=p.begin(); it!=p.end(); ++it)		
		points.append(QPoint((400-c.x()*m.x())/2 + c.x()*it->x(), (400-c.y()*m.y())/2 + c.y()*it->y()));		

	ogon = QPolygon(points);
	painter.drawPolygon(ogon);

	draw = false;
}

#endif // WINDOW_H