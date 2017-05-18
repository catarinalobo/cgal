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


#ifndef CGAL_RANDOM_SPIRAL_GRID_OGON_2_H
#define CGAL_RANDOM_SPIRAL_GRID_OGON_2_H

#include <CGAL/Random_grid_ogon_2/random_path_grid_ogon_2.h>


namespace CGAL {

  /* Generate a spiral grid ogon by Inflate-Paste Algorithm. */
  inline Grid_ogon_2 Random_grid_ogon_2::generate_spiral_grid_ogon_2() {
    
    init_grid_ogon();

    /* convex_vertex_type_limit[i] can be the id of the new reflex vertex, if the convex vertex with id i in the convex_vertices set is from Type I;
       -1, if it is from Type II. */
    int* convex_vertex_type_limit = new int[4];
    int n_reflex_vertices = (n_vertices-4)/2;

    /* Base case */
    if(n_reflex_vertices-- > 0) {

      int v = convex_vertices[rand->uniform_int<int>(0,3)];
      int v_H_adjacent = get_adjacent(v,CGAL_H);  
        
      FSN_info* fsn_info = get_FSN_base_case_path(v,v_H_adjacent);

      int* cell_coordinates = get_random_cell_in_FSN(fsn_info);

      bool cell_is_in_eH;
      inflate_paste(v,v_H_adjacent,cell_coordinates,cell_is_in_eH);

      /* Update convex_vertices set. */
      int c = get_adjacent(v,CGAL_H);
      int c_V_adjacent = get_adjacent(c,CGAL_V);
      init_types_spiral(v,v_H_adjacent,c,c_V_adjacent,cell_is_in_eH,convex_vertex_type_limit);
      n_convex_vertices = 2;

      delete [] cell_coordinates;
    }

    /* Induction step */
    while(n_reflex_vertices-- > 0) {
      
      int v = convex_vertices[rand->uniform_int<int>(0,1)];
      int v_H_adjacent = get_adjacent(v,CGAL_H);  

      FSN_info* fsn_info = get_FSN_path(v,v_H_adjacent,convex_vertex_type_limit);

      int* cell_coordinates = get_random_cell_in_FSN(fsn_info);

      bool cell_is_in_eH;
      inflate_paste(v,v_H_adjacent,cell_coordinates,cell_is_in_eH);
    
      /* Update convex_vertices set. */
      int c = get_adjacent(v,CGAL_H);
      int c_V_adjacent = get_adjacent(c,CGAL_V);
      update_types_spiral(v,v_H_adjacent,c_V_adjacent,convex_vertex_type_limit);

      delete [] cell_coordinates;
    }

    delete [] convex_vertex_type_limit;

    std::vector<Random_grid_ogon_2::Point_2> grid_ogon = get_grid_ogon();
    return Grid_ogon_2(grid_ogon.begin(), grid_ogon.end());
  }

  /* Update the convex_vertices set and define the type of each convex vertex in that updated set, when the grid ogon has only one reflex vertex. */
	inline void Random_grid_ogon_2::init_types_spiral(int v, int v_H_adjacent, int c, int c_V_adjacent, bool cell_is_in_eH, int* convex_vertex_type_limit) {

    if(cell_is_in_eH) { //cell is inside of eH
      /* For Type I */
      add_convex_vertex_path(v_H_adjacent,0,c_V_adjacent,convex_vertex_type_limit);
      vertex_info[get_adjacent(v_H_adjacent,CGAL_V)].convex_id = -1;
      /* For Type II */
      add_convex_vertex_path(v,1,-1,convex_vertex_type_limit);
      vertex_info[c].convex_id = -1;
    }
    else { //cell is not inside of eH
      /* For Type I */
      vertex_info[c].convex_id = -1;
      add_convex_vertex_path(c_V_adjacent,0,v_H_adjacent,convex_vertex_type_limit);
      /* For Type II */
      add_convex_vertex_path(get_adjacent(v,CGAL_V),1,-1,convex_vertex_type_limit);
      vertex_info[get_adjacent(v_H_adjacent,CGAL_V)].convex_id = -1;
    }    
  }

  /* Update the convex_vertices set and define the type of each convex vertex in that updated set, when the grid ogon has more than one reflex vertex. */
  inline void Random_grid_ogon_2::update_types_spiral(int v, int v_H_adjacent, int c_V_adjacent, int* convex_vertex_type_limit) {
    
    if(convex_vertex_type_limit[vertex_info[v].convex_id] >= 0) { //v is from Type I
      convex_vertex_type_limit[vertex_info[v].convex_id] = -1;
    }
    else { //v is from Type II
      add_convex_vertex_path(c_V_adjacent,vertex_info[v].convex_id,v_H_adjacent,convex_vertex_type_limit);
      vertex_info[v].convex_id = -1;
    }
  }
}

#endif // CGAL_RANDOM_SPIRAL_GRID_OGON_2_H //