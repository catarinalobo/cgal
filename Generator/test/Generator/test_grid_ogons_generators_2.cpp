#include <iostream>
#include <vector>

#include <CGAL/ogons_generators_2.h>

#include <CGAL/Cartesian.h>
#include <CGAL/Homogeneous.h>

#include <CGAL/Polygon_2.h>


typedef CGAL::Cartesian<int>                       Kernel_cartesian;
typedef Kernel_cartesian::Point_2                  Point_2_cartesian;
typedef CGAL::Polygon_2<Kernel_cartesian>          Polygon_2_cartesian;

typedef CGAL::Homogeneous<int>                     Kernel_homogeneous;
typedef Kernel_homogeneous::Point_2                Point_2_homogeneous;
typedef CGAL::Polygon_2<Kernel_homogeneous>        Polygon_2_homogeneous;

typedef CGAL::Grid_ogon_2                          Grid_ogon_2;
typedef CGAL::Random                               Random;


template<typename Point_2, typename Polygon_2> void print_grid_ogon(Grid_ogon_2 grid_ogon) {
  
  Polygon_2 polygon; 
  std::vector<Point_2> p;

  p.resize(grid_ogon.get_number_of_vertices());
  grid_ogon.get_polygon_2(p.begin());

  polygon = Polygon_2(p.begin(), p.end());

  std::cout << polygon << std::endl;
  /*typedef typename Polygon_2::Vertex_iterator Vertex_iterator;
  
  for(Vertex_iterator vi=polygon.vertices_begin(); vi!=polygon.vertices_end(); ++vi)
  	std::cout << CGAL::to_double(vi->x())/10 << " " << CGAL::to_double(vi->y())/10 << std::endl;
  std::cout << std::endl;*/

}

void generate_grid_ogons(CGAL::Grid_ogon_2 generator(int, Random&), Random &rnd) {

  int n_vertices;
  Grid_ogon_2 grid_ogon;

  for(int i=0; i<100; i++) {
    
    n_vertices = 2 * rnd.uniform_int(32, 64);
    grid_ogon = generator(n_vertices, rnd);

    std::cout << "Grid Ogon #" << i << std::endl;
    //std::cout << n_vertices << std::endl;
    print_grid_ogon<Point_2_cartesian, Polygon_2_cartesian>(grid_ogon);
    //std::cout << n_vertices << std::endl;
    print_grid_ogon<Point_2_homogeneous, Polygon_2_homogeneous>(grid_ogon);

  }

  std::cout << std::endl;
}

int main() {

  Random rnd;

  CGAL::set_pretty_mode(std::cout);
  //std::cout << "1800" << std::endl;
  std::cout << "----- 100 Generic Grid Ogons -----" << std::endl;
  generate_grid_ogons(CGAL::random_grid_ogon_2, rnd);

  std::cout << "----- 100 Row-Convex Grid Ogons -----" << std::endl;
  generate_grid_ogons(CGAL::random_row_convex_grid_ogon_2, rnd);
  
  std::cout << "----- 100 Column-Convex Grid Ogons -----" << std::endl;
  generate_grid_ogons(CGAL::random_column_convex_grid_ogon_2, rnd);
  
  std::cout << "----- 100 Convex Grid Ogons -----" << std::endl;
  generate_grid_ogons(CGAL::random_convex_grid_ogon_2, rnd);
  
  std::cout << "----- 100 Path Grid Ogons -----" << std::endl;
  generate_grid_ogons(CGAL::random_path_grid_ogon_2, rnd);
  
  std::cout << "----- 100 Spiral Grid Ogons -----" << std::endl;
  generate_grid_ogons(CGAL::random_spiral_grid_ogon_2, rnd); 
      
}