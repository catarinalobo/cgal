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


#ifndef CGAL_RANDOM_COLUMN_CONVEX_GRID_OGON_2_H
#define CGAL_RANDOM_COLUMN_CONVEX_GRID_OGON_2_H

#include <CGAL/Random_grid_ogon_2/random_row_convex_grid_ogon_2.h>


namespace CGAL {

  /* Generate a row-convex grid ogon by Inflate-Paste Algorithm. */
  inline Grid_ogon_2 Random_grid_ogon_2::generate_column_convex_grid_ogon_2() {
    
    generate_row_convex_grid_ogon_2();

    //Swap lines
    int swap;
    for(int i=1; i<=current_dim; i++) {

      swap = line_info[CGAL_H][i].coordinate;
      line_info[CGAL_H][i].coordinate = line_info[CGAL_V][i].coordinate;
      line_info[CGAL_V][i].coordinate = swap;

      swap = line_info[CGAL_H][i].vertex_id;
      line_info[CGAL_H][i].vertex_id = line_info[CGAL_V][i].vertex_id;
      line_info[CGAL_V][i].vertex_id = swap;

      swap = grid[CGAL_H][i];
      grid[CGAL_H][i] = grid[CGAL_V][i];
      grid[CGAL_V][i] = swap;
    }
    for(int i=0; i<n_vertices; i++) {

      swap = vertex_info[i].line_id[CGAL_H];
      vertex_info[i].line_id[CGAL_H] = vertex_info[i].line_id[CGAL_V];
      vertex_info[i].line_id[CGAL_V] = swap;
    }

    std::vector<Random_grid_ogon_2::Point_2> grid_ogon = get_grid_ogon();
    return Grid_ogon_2(grid_ogon.begin(), grid_ogon.end());
  }
}

#endif // CGAL_RANDOM_COLUMN_CONVEX_GRID_OGON_2_H //