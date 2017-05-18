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

template<typename Point_2, typename Polygon_2> void generate_ogons(CGAL::Ogon_2 generator(int, Point_2, float, int, Random&), Random &rnd) {

  int n_vertices;
  Point_2 max_coordinates;
  float colinearity_probability;
  int n_stretchings;

  Ogon_2 ogon;

  for(int i=0; i<100; i++) {
    
    n_vertices = 2 * rnd.uniform_int(32, 64);
    max_coordinates = Point_2(rnd.uniform_int(n_vertices/2, n_vertices), rnd.uniform_int(n_vertices/2, n_vertices));
    colinearity_probability = rnd.uniform_real<float>();
    n_stretchings = rnd.uniform_int(1, 2*n_vertices);

    ogon = generator(n_vertices, max_coordinates, colinearity_probability, n_stretchings, rnd);

    std::cout << "Ogon #" << i << std::endl;
    //std::cout << n_vertices << std::endl;
    print_ogon<Point_2, Polygon_2>(ogon);
    std::cout << "Ogon #" << i << " was obtained from the following grid ogon:" << std::endl;
    //std::cout << n_vertices << std::endl;
    print_ogon<Point_2, Polygon_2>(ogon.get_grid_ogon_2());
    
  }

  std::cout << std::endl;
}

int main() {

  Random rnd;

  CGAL::set_pretty_mode(std::cout);
  //std::cout << "2400" << std::endl;
  std::cout << "----- 100 Generic Ogons -----" << std::endl;
  generate_ogons<Point_2_cartesian, Polygon_2_cartesian>(CGAL::random_ogon_2, rnd);
  generate_ogons<Point_2_homogeneous, Polygon_2_homogeneous>(CGAL::random_ogon_2, rnd);

  std::cout << "----- 100 Row-Convex Ogons -----" << std::endl;
  generate_ogons<Point_2_cartesian, Polygon_2_cartesian>(CGAL::random_row_convex_ogon_2, rnd);
  generate_ogons<Point_2_homogeneous, Polygon_2_homogeneous>(CGAL::random_row_convex_ogon_2, rnd);
  
  std::cout << "----- 100 Column-Convex Ogons -----" << std::endl;
  generate_ogons<Point_2_cartesian, Polygon_2_cartesian>(CGAL::random_column_convex_ogon_2, rnd);
  generate_ogons<Point_2_homogeneous, Polygon_2_homogeneous>(CGAL::random_column_convex_ogon_2, rnd);
  
  std::cout << "----- 100 Convex Ogons -----" << std::endl;
  generate_ogons<Point_2_cartesian, Polygon_2_cartesian>(CGAL::random_convex_ogon_2, rnd);
  generate_ogons<Point_2_homogeneous, Polygon_2_homogeneous>(CGAL::random_convex_ogon_2, rnd);
  
  std::cout << "----- 100 Path Ogons -----" << std::endl;
  generate_ogons<Point_2_cartesian, Polygon_2_cartesian>(CGAL::random_path_ogon_2, rnd);
  generate_ogons<Point_2_homogeneous, Polygon_2_homogeneous>(CGAL::random_path_ogon_2, rnd);
  
  std::cout << "----- 100 Spiral Ogons -----" << std::endl;
  generate_ogons<Point_2_cartesian, Polygon_2_cartesian>(CGAL::random_spiral_ogon_2, rnd);   
  generate_ogons<Point_2_homogeneous, Polygon_2_homogeneous>(CGAL::random_spiral_ogon_2, rnd);   
       
}