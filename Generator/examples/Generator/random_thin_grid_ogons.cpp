#include <iostream>
#include <vector>

#include <CGAL/ogons_generators_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2.h>


typedef CGAL::Cartesian<int>             Kernel;
typedef Kernel::Point_2                  Point_2;
typedef CGAL::Polygon_2<Kernel>          Polygon_2;
typedef CGAL::Grid_ogon_2                Grid_ogon_2;
typedef CGAL::Random                     Random;


int main() {

  Random rnd;

  int n_vertices = 2 * rnd.uniform_int(25, 75);
  
  Grid_ogon_2 thin_grid_ogon;

  Polygon_2 polygon;

  std::vector<Point_2> p;
  p.resize(n_vertices);

  std::cout << "100 " << n_vertices << "-vertex thin grid ogons were generated..." << std::endl << std::endl; 
  CGAL::set_pretty_mode(std::cout);

  for(int i=0; i<100; i++) {
    
    thin_grid_ogon = CGAL::random_path_grid_ogon_2(n_vertices, rnd);

    thin_grid_ogon.get_polygon_2(p.begin());
    polygon = Polygon_2(p.begin(), p.end());

    std::cout << "Thin grid ogon #" << i << ":" << std::endl;
    std::cout << polygon << std::endl;

  }
      
}