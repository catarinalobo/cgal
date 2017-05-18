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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <list>

#include <QMainWindow>
#include <QMenuBar>

#include "Window.h"


class MainWindow : public QMainWindow {

Q_OBJECT

public:
	MainWindow();

private slots:
	void genGridOgon();
	void genRowConvexGridOgon();
	void genColumnConvexGridOgon();
	void genConvexGridOgon();
	void genPathGridOgon();
	void genSpiralGridOgon();

	void genOgon();
	void genRowConvexOgon();
	void genColumnConvexOgon();
	void genConvexOgon();
	void genPathOgon();
	void genSpiralOgon();
	void genOgonFromGridOgon();

	void clearAll();

private:
	typedef CGAL::Cartesian<int>             Kernel;
	typedef Kernel::Point_2                  Point_2;
	typedef CGAL::Grid_ogon_2                Grid_ogon_2;
	typedef CGAL::Ogon_2                	 Ogon_2;
	typedef CGAL::Random                     Random;

	void paintEvent(QPaintEvent*);
	void createActions();

	QMenu* menu_generate;

	QMenu* menu_grid_ogon;
	QAction* gen_grid_ogon;
	QAction* gen_row_convex_grid_ogon;
	QAction* gen_column_convex_grid_ogon;
	QAction* gen_convex_grid_ogon;
	QAction* gen_path_grid_ogon;
	QAction* gen_spiral_grid_ogon;

	QMenu* menu_ogon;
	QAction* gen_ogon;
	QAction* gen_row_convex_ogon;
	QAction* gen_column_convex_ogon;
	QAction* gen_convex_ogon;
	QAction* gen_path_ogon;
	QAction* gen_spiral_ogon;
	QAction* gen_ogon_from_grid_ogon;

	QMenu* menu_clear;
	QAction* clear_all;

	std::list<Window*> windows;

	bool draw;
	int subclass;
	Grid_ogon_2 grid_ogon; //last grid ogon
	QPolygon ogon;
};

inline void MainWindow::paintEvent(QPaintEvent*) {

    if(subclass == 0)
    	return;

    QPainter painter(this);
    painter.setBrush(QBrush(QColor(133,198,208), Qt::SolidPattern));

    if(!draw) {
    	painter.drawPolygon(ogon);
    	return;
    }

    int n_vertices;
	Point_2 max_coords;
	float col_prob;
	int s;
    Random rnd;

    std::vector<Point_2> p;
    QVector<QPoint> points;

	n_vertices = 2*rnd.uniform_int(25, 50);
	max_coords = Point_2(rnd.uniform_int(n_vertices/2-1,2*n_vertices-1), rnd.uniform_int(n_vertices/2-1,2*n_vertices-1));
	col_prob = rnd.uniform_real<float>();
	s = rnd.uniform_int(0, 2*n_vertices);
	  
	p.resize(n_vertices);
	   
	if(subclass == -1) {
		grid_ogon = CGAL::random_grid_ogon_2(n_vertices, rnd);
		grid_ogon.get_polygon_2(p.begin());
	}
	else if(subclass == -2) {
		grid_ogon = CGAL::random_row_convex_grid_ogon_2(n_vertices, rnd);
		grid_ogon.get_polygon_2(p.begin());
	}
	else if(subclass == -3) {
		grid_ogon = CGAL::random_column_convex_grid_ogon_2(n_vertices, rnd);
		grid_ogon.get_polygon_2(p.begin());
	}
	else if(subclass == -4) {
		grid_ogon = CGAL::random_convex_grid_ogon_2(n_vertices, rnd);
		grid_ogon.get_polygon_2(p.begin());
	}
	else if(subclass == -5) {
		grid_ogon = CGAL::random_path_grid_ogon_2(n_vertices, rnd);
		grid_ogon.get_polygon_2(p.begin());
	}
	else if(subclass == -6) {
		grid_ogon = CGAL::random_spiral_grid_ogon_2(n_vertices, rnd);
		grid_ogon.get_polygon_2(p.begin());
	}
	else {
		grid_ogon = Grid_ogon_2();
	}
	if(subclass == 1)
		CGAL::random_ogon_2(n_vertices, max_coords, col_prob, s, rnd).get_polygon_2(p.begin());
	else if(subclass == 2)
		CGAL::random_row_convex_ogon_2(n_vertices, max_coords, col_prob, s, rnd).get_polygon_2(p.begin());
	else if(subclass == 3)
		CGAL::random_column_convex_ogon_2(n_vertices, max_coords, col_prob, s, rnd).get_polygon_2(p.begin());
	else if(subclass == 4)
		CGAL::random_convex_ogon_2(n_vertices, max_coords, col_prob, s, rnd).get_polygon_2(p.begin());
	else if(subclass == 5)
		CGAL::random_path_ogon_2(n_vertices, max_coords, col_prob, s, rnd).get_polygon_2(p.begin());
	else if(subclass == 6)
		CGAL::random_spiral_ogon_2(n_vertices, max_coords, col_prob, s, rnd).get_polygon_2(p.begin());


	Point_2 m(0,0);
	for(std::vector<Point_2>::iterator it=p.begin(); it!=p.end(); ++it) {
		m = Point_2(std::max(m.x(), it->x()), std::max(m.y(), it->y()));
	}
	Point_2 c(300/m.x(), 300/m.y());
	
	for(std::vector<Point_2>::iterator it=p.begin(); it!=p.end(); ++it) {
		
		points.append(QPoint((400-c.x()*m.x())/2 + c.x()*it->x(), (400-c.y()*m.y())/2 + c.y()*it->y()));		
	}

	ogon = QPolygon(points);
	painter.drawPolygon(ogon);

	draw = false;
}


inline void MainWindow::genGridOgon() {

	subclass = -1;
	draw = true;
	gen_ogon_from_grid_ogon->setVisible(true);
	this->update();
}

inline void MainWindow::genRowConvexGridOgon() {

	subclass = -2;
	draw = true;
	gen_ogon_from_grid_ogon->setVisible(true);
	this->update();
}

inline void MainWindow::genColumnConvexGridOgon() {

	subclass = -3;
	draw = true;
	gen_ogon_from_grid_ogon->setVisible(true);
	this->update();
}

inline void MainWindow::genConvexGridOgon() {

	subclass = -4;
	draw = true;
	gen_ogon_from_grid_ogon->setVisible(true);
	this->update();
}

inline void MainWindow::genPathGridOgon() {

	subclass = -5;
	draw = true;
	gen_ogon_from_grid_ogon->setVisible(true);
	this->update();
}

inline void MainWindow::genSpiralGridOgon() {

	subclass = -6;
	draw = true;
	gen_ogon_from_grid_ogon->setVisible(true);
	this->update();
}

inline void MainWindow::genOgon() {

	subclass = 1;
	draw = true;
	gen_ogon_from_grid_ogon->setVisible(false);
	this->update();
}

inline void MainWindow::genRowConvexOgon() {

	subclass = 2;
	draw = true;
	gen_ogon_from_grid_ogon->setVisible(false);
	this->update();
}

inline void MainWindow::genColumnConvexOgon() {

	subclass = 3;
	draw = true;
	gen_ogon_from_grid_ogon->setVisible(false);
	this->update();
}

inline void MainWindow::genConvexOgon() {

	subclass = 4;
	draw = true;
	gen_ogon_from_grid_ogon->setVisible(false);
	this->update();
}

inline void MainWindow::genPathOgon() {

	subclass = 5;
	draw = true;
	gen_ogon_from_grid_ogon->setVisible(false);
	this->update();
}

inline void MainWindow::genSpiralOgon() {

	subclass = 6;
	draw = true;
	gen_ogon_from_grid_ogon->setVisible(false);
	this->update();
}

inline void MainWindow::genOgonFromGridOgon() {

	this->update();

	if(subclass >= 0)
    	return;

    Window *newWindow = new Window();
	newWindow->genOgonFromGridOgon(grid_ogon);
	newWindow->show();
	windows.push_back(newWindow);
}

inline void MainWindow::clearAll() {

	subclass = 0;
	gen_ogon_from_grid_ogon->setVisible(false);
	this->update();

	while(!windows.empty()) {
		if(windows.front()->isVisible())
			windows.front()->close();
		windows.pop_front();
	}
}

inline void MainWindow::createActions() {

	gen_grid_ogon = menu_grid_ogon->addAction("Generic");	
	connect(gen_grid_ogon, SIGNAL(triggered()), this, SLOT(genGridOgon()));

	gen_row_convex_grid_ogon = menu_grid_ogon->addAction("Row-Convex");	
	connect(gen_row_convex_grid_ogon, SIGNAL(triggered()), this, SLOT(genRowConvexGridOgon()));

	gen_column_convex_grid_ogon = menu_grid_ogon->addAction("Column-Convex");
	connect(gen_column_convex_grid_ogon, SIGNAL(triggered()), this, SLOT(genColumnConvexGridOgon()));

	gen_convex_grid_ogon = menu_grid_ogon->addAction("Convex");
	connect(gen_convex_grid_ogon, SIGNAL(triggered()), this, SLOT(genConvexGridOgon()));

	gen_path_grid_ogon = menu_grid_ogon->addAction("Path");
	connect(gen_path_grid_ogon, SIGNAL(triggered()), this, SLOT(genPathGridOgon()));

	gen_spiral_grid_ogon = menu_grid_ogon->addAction("Spiral");
	connect(gen_spiral_grid_ogon, SIGNAL(triggered()), this, SLOT(genSpiralGridOgon()));


	gen_ogon = menu_ogon->addAction("Generic");	
	connect(gen_ogon, SIGNAL(triggered()), this, SLOT(genOgon()));

	gen_row_convex_ogon = menu_ogon->addAction("Row-Convex");
	connect(gen_row_convex_ogon, SIGNAL(triggered()), this, SLOT(genRowConvexOgon()));
	
	gen_column_convex_ogon = menu_ogon->addAction("Column-Convex");
	connect(gen_column_convex_ogon, SIGNAL(triggered()), this, SLOT(genColumnConvexOgon()));
	
	gen_convex_ogon = menu_ogon->addAction("Convex");
	connect(gen_convex_ogon, SIGNAL(triggered()), this, SLOT(genConvexOgon()));
	
	gen_path_ogon = menu_ogon->addAction("Path");
	connect(gen_path_ogon, SIGNAL(triggered()), this, SLOT(genPathOgon()));
	
	gen_spiral_ogon = menu_ogon->addAction("Spiral");
	connect(gen_spiral_ogon, SIGNAL(triggered()), this, SLOT(genSpiralOgon()));

	gen_ogon_from_grid_ogon = menu_ogon->addAction("From Grid Ogon");
	connect(gen_ogon_from_grid_ogon, SIGNAL(triggered()), this, SLOT(genOgonFromGridOgon()));
	gen_ogon_from_grid_ogon->setVisible(false);	

	
	clear_all = menu_clear->addAction("Clear All");
	connect(clear_all, SIGNAL(triggered()), this, SLOT(clearAll()));		
}

inline MainWindow::MainWindow() : QMainWindow() {

	this->setFixedSize(400, 400);
	this->setWindowTitle("Ogons_Generators_2");
	this->setWindowIcon(QIcon("cgal.ico"));

	QWidget *centralWidget = new QWidget(this);
	QMenuBar *menu = new QMenuBar(centralWidget);

	menu_generate = new QMenu("Generate");

	menu_grid_ogon = new QMenu("Grid Ogon");
	menu_generate->addMenu(menu_grid_ogon);
	
	menu_ogon = new QMenu("Ogon");
	menu_generate->addMenu(menu_ogon);

	menu_clear = new QMenu("Clear");

	createActions();
	
	menu->addMenu(menu_generate);
	menu->addMenu(menu_clear);

	this->setCentralWidget(centralWidget);

	subclass = 0;
	draw = false;
}

#endif // MAINWINDOW_H