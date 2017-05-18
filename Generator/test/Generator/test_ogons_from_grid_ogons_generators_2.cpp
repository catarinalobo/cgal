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
typedef CGAL::Ogon_2                          	   Ogon_2;
typedef CGAL::Random                               Random;


template<typename Point_2, typename Polygon_2> void print_ogon(Grid_ogon_2 ogon) {
  
  Polygon_2 polygon; 
  std::vector<Point_2> p;

  p.resize(ogon.get_number_of_vertices());
  ogon.get_polygon_2(p.begin());

  polygon = Polygon_2(p.begin(), p.end());

  std::cout << polygon << std::endl;
  /*typedef typename Polygon_2::Vertex_iterator Vertex_iterator;

  for(Vertex_iterator vi=polygon.vertices_begin(); vi!=polygon.vertices_end(); ++vi)
    std::cout << CGAL::to_double(vi->x())/10 << " " << CGAL::to_double(vi->y())/10 << std::endl;
  std::cout << std::endl;*/

}

void generate_ogons(CGAL::Grid_ogon_2 generator(int, Random&), Random &rnd) {

  int n_vertices = 2 * rnd.uniform_int(32, 64);
  Point_2_cartesian max_coordinates;
  float colinearity_probability;
  int n_stretchings;

  Grid_ogon_2 grid_ogon = generator(n_vertices, rnd);
  Ogon_2 ogon;

  std::cout << "Grid Ogon" << std::endl;
  //std::cout << n_vertices << std::endl;
  print_ogon<Point_2_cartesian, Polygon_2_cartesian>(grid_ogon);
  //std::cout << n_vertices << std::endl;
  print_ogon<Point_2_homogeneous, Polygon_2_homogeneous>(grid_ogon);

  for(int i=0; i<100; i++) {
    
    max_coordinates = Point_2_cartesian(rnd.uniform_int(n_vertices/2, n_vertices), rnd.uniform_int(n_vertices/2, n_vertices));
    colinearity_probability = rnd.uniform_real<float>();
    n_stretchings = rnd.uniform_int(1, 2*n_vertices);

    ogon = CGAL::random_ogon_from_grid_ogon_2(grid_ogon, max_coordinates, colinearity_probability, n_stretchings, rnd);

    std::cout << "Ogon #" << i << std::endl;
    //std::cout << n_vertices << std::endl;
    print_ogon<Point_2_cartesian, Polygon_2_cartesian>(ogon);
    //std::cout << n_vertices << std::endl;
    print_ogon<Point_2_homogeneous, Polygon_2_homogeneous>(ogon);
    
  }

  std::cout << std::endl;
}

int main() {

  Random rnd;

  CGAL::set_pretty_mode(std::cout);
  //std::cout << "1212" << std::endl;
  std::cout << "----- 100 Generic Ogons from the same Grid Ogon -----" << std::endl;
  generate_ogons(CGAL::random_grid_ogon_2, rnd);

  std::cout << "----- 100 Row-Convex Ogons from the same Grid Ogon -----" << std::endl;
  generate_ogons(CGAL::random_row_convex_grid_ogon_2, rnd);
  
  std::cout << "----- 100 Column-Convex Ogons from the same Grid Ogon -----" << std::endl;
  generate_ogons(CGAL::random_column_convex_grid_ogon_2, rnd);
  
  std::cout << "----- 100 Convex Ogons from the same Grid Ogon -----" << std::endl;
  generate_ogons(CGAL::random_convex_grid_ogon_2, rnd);
  
  std::cout << "----- 100 Path Ogons from the same Grid Ogon -----" << std::endl;
  generate_ogons(CGAL::random_path_grid_ogon_2, rnd);
  
  std::cout << "----- 100 Spiral Ogons from the same Grid Ogon -----" << std::endl;
  generate_ogons(CGAL::random_spiral_grid_ogon_2, rnd);   
       
}