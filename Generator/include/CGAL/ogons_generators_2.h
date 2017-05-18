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


#ifndef CGAL_OGONS_GENERATORS_2_H
#define CGAL_OGONS_GENERATORS_2_H

#include <CGAL/Random_ogon_2.h>


namespace CGAL {

	inline Grid_ogon_2 random_grid_ogon_2(int n=4, Random &rnd=get_default_random()) {

		return Random_grid_ogon_2(n, rnd).generate_grid_ogon_2();
	}

	inline Grid_ogon_2 random_row_convex_grid_ogon_2(int n=4, Random &rnd=get_default_random()) {

		return Random_grid_ogon_2(n, rnd).generate_row_convex_grid_ogon_2();
	}

	inline Grid_ogon_2 random_column_convex_grid_ogon_2(int n=4, Random &rnd=get_default_random()) {

		return Random_grid_ogon_2(n, rnd).generate_column_convex_grid_ogon_2();
	}

	inline Grid_ogon_2 random_convex_grid_ogon_2(int n=4, Random &rnd=get_default_random()) {

		return Random_grid_ogon_2(n, rnd).generate_convex_grid_ogon_2();
	}

	inline Grid_ogon_2 random_path_grid_ogon_2(int n=4, Random &rnd=get_default_random()) {

		return Random_grid_ogon_2(n, rnd).generate_path_grid_ogon_2();
	}

	inline Grid_ogon_2 random_spiral_grid_ogon_2(int n=4, Random &rnd=get_default_random()) {
		
		return Random_grid_ogon_2(n, rnd).generate_spiral_grid_ogon_2();
	}

	template<class Point_2> inline Ogon_2 random_ogon_2(int n=4, Point_2 max_coords=Point_2(0,0), float col_prob=0.0, int s=0, Random &rnd=get_default_random()) {
		
		int* max_coordinate;
		if(max_coords == Point_2(0,0))
			max_coordinate = new int[2]{n/2-1, n/2-1};
		else
			max_coordinate = new int[2]{(int) to_double(max_coords.x()), (int) to_double(max_coords.y())};

		return Random_ogon_2(n, rnd).generate_ogon_2(max_coordinate, col_prob, s);
	}
	
	template<class Point_2> inline Ogon_2 random_row_convex_ogon_2(int n=4, Point_2 max_coords=Point_2(0,0), float col_prob=0.0, int s=0, Random &rnd=get_default_random()) {
		
		int* max_coordinate;
		if(max_coords == Point_2(0,0))
			max_coordinate = new int[2]{n/2-1, n/2-1};
		else
			max_coordinate = new int[2]{(int) to_double(max_coords.x()), (int) to_double(max_coords.y())};

		return Random_ogon_2(n, rnd).generate_row_convex_ogon_2(max_coordinate, col_prob, s);
	}
  
  	template<class Point_2> inline Ogon_2 random_column_convex_ogon_2(int n=4, Point_2 max_coords=Point_2(0,0), float col_prob=0.0, int s=0, Random &rnd=get_default_random()) {
		
		int* max_coordinate;
		if(max_coords == Point_2(0,0))
			max_coordinate = new int[2]{n/2-1, n/2-1};
		else
			max_coordinate = new int[2]{(int) to_double(max_coords.x()), (int) to_double(max_coords.y())};

		return Random_ogon_2(n, rnd).generate_column_convex_ogon_2(max_coordinate, col_prob, s);
	}
  
  	template<class Point_2> inline Ogon_2 random_convex_ogon_2(int n=4, Point_2 max_coords=Point_2(0,0), float col_prob=0.0, int s=0, Random &rnd=get_default_random()) {
		
		int* max_coordinate;
		if(max_coords == Point_2(0,0))
			max_coordinate = new int[2]{n/2-1, n/2-1};
		else
			max_coordinate = new int[2]{(int) to_double(max_coords.x()), (int) to_double(max_coords.y())};

		return Random_ogon_2(n, rnd).generate_convex_ogon_2(max_coordinate, col_prob, s);
	}
  
  	template<class Point_2> inline Ogon_2 random_path_ogon_2(int n=4, Point_2 max_coords=Point_2(0,0), float col_prob=0.0, int s=0, Random &rnd=get_default_random()) {
		
		int* max_coordinate;
		if(max_coords == Point_2(0,0))
			max_coordinate = new int[2]{n/2-1, n/2-1};
		else
			max_coordinate = new int[2]{(int) to_double(max_coords.x()), (int) to_double(max_coords.y())};

		return Random_ogon_2(n, rnd).generate_path_ogon_2(max_coordinate, col_prob, s);
	}
  
  	template<class Point_2> inline Ogon_2 random_spiral_ogon_2(int n=4, Point_2 max_coords=Point_2(0,0), float col_prob=0.0, int s=0, Random &rnd=get_default_random()) {
		
		int* max_coordinate;
		if(max_coords == Point_2(0,0))
			max_coordinate = new int[2]{n/2-1, n/2-1};
		else
			max_coordinate = new int[2]{(int) to_double(max_coords.x()), (int) to_double(max_coords.y())};

		return Random_ogon_2(n, rnd).generate_spiral_ogon_2(max_coordinate, col_prob, s);
	}

  	template<class Point_2> inline Ogon_2 random_ogon_from_grid_ogon_2(Grid_ogon_2 grid_ogon=Grid_ogon_2(), Point_2 max_coords=Point_2(0,0), float col_prob=0.0,
  																int s=0, Random &rnd=get_default_random()) {	

  		int* max_coordinate;
		if(max_coords == Point_2(0,0))
			max_coordinate = new int[2]{grid_ogon.get_number_of_vertices()/2-1, grid_ogon.get_number_of_vertices()/2-1};
		else
			max_coordinate = new int[2]{(int) to_double(max_coords.x()), (int) to_double(max_coords.y())};

		return Random_ogon_2(grid_ogon.get_number_of_vertices(), rnd).generate_ogon_from_grid_ogon_2(max_coordinate, col_prob, s, grid_ogon);
  }
}

#endif // CGAL_OGONS_GENERATORS_2_H //