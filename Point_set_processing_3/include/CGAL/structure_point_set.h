// Copyright (c) 2015 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : 
//

#ifndef CGAL_STRUCTURE_POINT_SET_3_H
#define CGAL_STRUCTURE_POINT_SET_3_H

#include <CGAL/property_map.h>
#include <CGAL/point_set_processing_assertions.h>
#include <CGAL/assertions.h>

#include <CGAL/centroid.h>

#include <CGAL/Kd_tree.h>
#include <CGAL/Fuzzy_sphere.h>
#include <CGAL/Fuzzy_iso_box.h>
#include <CGAL/Search_traits_d.h>

#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>

#include <iterator>
#include <list>


namespace CGAL {


// ----------------------------------------------------------------------------
// Private section
// ----------------------------------------------------------------------------
/// \cond SKIP_IN_MANUAL
namespace internal {

  template <typename Traits>
  class Point_set_structuring
  {
  public:

    typedef Point_set_structuring<Traits> Self;

    typedef typename Traits::FT FT;
    typedef typename Traits::Point_3 Point;
    typedef typename Traits::Vector_3 Vector;
    typedef typename Traits::Segment_3 Segment;
    typedef typename Traits::Line_3 Line;
    typedef typename Traits::Plane_3 Plane;

    typedef typename Traits::Point_2 Point_2;

    typedef typename Traits::Point_map Point_map;
    typedef typename Traits::Normal_map Normal_map;
    typedef typename Traits::Input_range Input_range;

    typedef typename Input_range::iterator Input_iterator;

    typedef Shape_detection_3::Shape_base<Traits> Shape; 
    typedef Shape_detection_3::Plane<Traits> Plane_shape;

    enum Point_status { POINT, RESIDUS, PLANE, EDGE, CORNER, SKIPPED };
    enum Coherence_type
      {
        INCOHERENT = -1, // Incoherent
        FREEFORM = 0,    // FF-coherent
        VERTEX = 1,      // S-coherent
        CREASE = 2,      // S-coherent
        PLANAR = 3       // S-coherent
      };

  private:

    const std::size_t minus1;
    
    class My_point_property_map{
      const std::vector<Point>& points;
    public:
      typedef Point value_type;
      typedef const value_type& reference;
      typedef std::size_t key_type;
      typedef boost::lvalue_property_map_tag category;  
      My_point_property_map (const std::vector<Point>& pts) : points (pts) {}
      reference operator[] (key_type k) const { return points[k]; }
      friend inline reference get (const My_point_property_map& ppmap, key_type i) 
      { return ppmap[i]; }
    };

    struct On_the_fly_pair{
      const std::vector<Point>& points;
      typedef std::pair<Point, std::size_t> result_type;

      On_the_fly_pair(const std::vector<Point>& points) : points(points) {}
  
      result_type
      operator()(std::size_t i) const
      {
        return result_type(points[i],i);
      }
    };
    
    struct Edge
    {
      CGAL::cpp11::array<std::size_t, 2> planes;
      std::vector<std::size_t> indices; // Points belonging to intersection
      Line support;
      bool active;

      Edge (std::size_t a, std::size_t b)
      { planes[0] = a; planes[1] = b; active = true; }
    };
    struct Corner
    {
      std::vector<std::size_t> planes;
      std::vector<std::size_t> edges;
      std::vector<Vector> directions;
      Point support;
      bool active;

      Corner (std::size_t p1, std::size_t p2, std::size_t p3,
              std::size_t e1, std::size_t e2, std::size_t e3)
      {
        planes.resize (3); planes[0] = p1; planes[1] = p2; planes[2] = p3;
        edges.resize (3); edges[0] = e1; edges[1] = e2; edges[2] = e3;
        active = true;
      }
    };
      

    Traits m_traits;

    std::vector<Point> m_points;
    std::vector<Vector> m_normals;
    std::vector<std::size_t> m_indices;
    std::vector<Point_status> m_status;
    Point_map m_point_pmap;
    Normal_map m_normal_pmap;
    
    std::vector<boost::shared_ptr<Plane_shape> > m_planes;
    std::vector<Edge> m_edges;
    std::vector<Corner> m_corners;
    
  public:

    Point_set_structuring (Traits t = Traits ())
      : minus1 (static_cast<std::size_t>(-1)), m_traits (t)
    {
    }

    
    Point_set_structuring (Input_iterator begin, Input_iterator end,
                           const Shape_detection_3::Efficient_RANSAC<Traits>& shape_detection)
      : minus1 (static_cast<std::size_t>(-1)), m_traits (shape_detection.traits())
    {
      for (Input_iterator it = begin; it != end; ++ it)
        {
          m_points.push_back (get(m_point_pmap, *it));
          m_normals.push_back (get(m_normal_pmap, *it));
        }
      
      m_indices = std::vector<std::size_t> (m_points.size (), minus1);
      m_status = std::vector<Point_status> (m_points.size (), POINT);

      BOOST_FOREACH (boost::shared_ptr<Shape> shape, shape_detection.shapes())
        {
          boost::shared_ptr<Plane_shape> pshape
            = boost::dynamic_pointer_cast<Plane_shape>(shape);
        
          // Ignore all shapes other than plane
          if (pshape == boost::shared_ptr<Plane_shape>())
            continue;
          m_planes.push_back (pshape);

          for (std::size_t i = 0; i < pshape->indices_of_assigned_points().size (); ++ i)
            {
              m_indices[pshape->indices_of_assigned_points()[i]] = m_planes.size () - 1;
              m_status[pshape->indices_of_assigned_points()[i]] = PLANE;
            }
        }

    }

    
    virtual ~Point_set_structuring ()
    {
    }

    void clean ()
    {
      std::vector<Point> points;
      std::vector<Vector> normals;
      std::vector<std::size_t> indices;
      std::vector<Point_status> status;
      
      for (std::size_t i = 0; i < m_points.size (); ++ i)
        if (m_status[i] != SKIPPED)
          {
            points.push_back (m_points[i]);
            normals.push_back (m_normals[i]);
            status.push_back (m_status[i]);
            if (m_status[i] == RESIDUS)
              status.back () = PLANE;
            indices.push_back (m_indices[i]);
          }
      
      m_points.swap (points);
      m_normals.swap (normals);
      m_indices.swap (indices);
      m_status.swap (status);
    }

    std::size_t number_of_points () const { return m_points.size (); }
    const Point& point (std::size_t i) const { return m_points[i]; }
    const Vector& normal (std::size_t i) const { return m_normals[i]; }

    void run (double epsilon, double attraction_factor = 3.)
    {
      if (m_planes.empty ())
        return;
      
      double radius = epsilon * attraction_factor;
#define CGAL_PSP3_VERBOSE
#ifdef CGAL_PSP3_VERBOSE
      std::cerr << "Computing planar points... " << std::endl;
#endif
      
      project_inliers ();
      resample_planes (epsilon);
      
#ifdef CGAL_PSP3_VERBOSE
      std::cerr << " -> Done" << std::endl;
      std::cerr << "Finding adjacent primitives... " << std::endl;
#endif
      
      find_pairs_of_adjacent_primitives (radius);

#ifdef CGAL_PSP3_VERBOSE
      std::cerr << " -> Found " << m_edges.size () << " pair(s) of adjacent primitives." << std::endl;
      std::cerr << "Computing edges... " << std::endl;
#endif
      
      compute_edges (epsilon);

#ifdef CGAL_PSP3_VERBOSE
      std::cerr << " -> Done" << std::endl;
      std::cerr << "Creating edge-anchor points... " << std::endl;
      {
        std::size_t size_before = m_points.size ();
#endif

        create_edge_anchor_points (radius, epsilon);

#ifdef CGAL_PSP3_VERBOSE
        std::cerr << " -> " << m_points.size () - size_before << " anchor point(s) created." << std::endl;
      }

      std::cerr << "Computating first set of corners... " << std::endl;
#endif
      
      compute_corners (radius);

#ifdef CGAL_PSP3_VERBOSE
      std::cerr << " -> Found " << m_corners.size () << " triple(s) of adjacent primitives/edges." << std::endl;
      std::cerr << "Merging corners... " << std::endl;
      {
        std::size_t size_before = m_points.size ();
#endif
        
        merge_corners (radius);

#ifdef CGAL_PSP3_VERBOSE
        std::cerr << " -> " << m_points.size () - size_before << " corner point(s) created." << std::endl;
      }

      std::cerr << "Merging corners... " << std::endl;
#endif
      
      compute_corner_directions (epsilon);

#ifdef CGAL_PSP3_VERBOSE
      std::cerr << " -> Done" << std::endl;
      std::cerr << "Refining sampling... " << std::endl;
#endif
      
      refine_sampling (epsilon);

#ifdef CGAL_PSP3_VERBOSE
      std::cerr << " -> Done" << std::endl;

      std::cerr << "Cleaning data set... " << std::endl;
#endif
      
      clean ();

#ifdef CGAL_PSP3_VERBOSE
      std::cerr << " -> Done" << std::endl;
#endif
    }


    template <typename OutputIterator>
    void get_output (OutputIterator pts) const
    {
      for (std::size_t i = 0; i < m_points.size (); ++ i)
        *(pts ++) = std::make_pair (m_points[i], m_normals[i]);
    }

    template <typename OutputIterator>
    void get_detailed_output (OutputIterator pts_planes,
                              OutputIterator pts_edges,
                              OutputIterator pts_corners) const
    {
      for (std::size_t i = 0; i < m_points.size (); ++ i)
        if (m_status[i] == PLANE || m_status[i] == RESIDUS)
          *(pts_planes ++) = std::make_pair (m_points[i], m_normals[i]);
        else if (m_status[i] == EDGE)
          *(pts_edges ++) = std::make_pair (m_points[i], m_normals[i]);
        else if (m_status[i] == CORNER)
          *(pts_corners ++) = std::make_pair (m_points[i], m_normals[i]);
    }

    template <typename OutputIterator>
    void get_coherent_delaunay_facets (OutputIterator facets,
                                       double epsilon) const
    {
      double d_DeltaEdge = std::sqrt (2.) * epsilon;
      
      typedef typename Traits::Base_kernel K;
      typedef CGAL::Triangulation_vertex_base_with_info_3<std::size_t, K> Vb;
      typedef CGAL::Triangulation_cell_base_3<K> Cb;
      typedef CGAL::Triangulation_data_structure_3<Vb,Cb> Tds;
      typedef CGAL::Delaunay_triangulation_3<K, Tds> Triangulation;

      std::vector<std::size_t> point_indices(boost::counting_iterator<std::size_t>(0),
                                             boost::counting_iterator<std::size_t>(m_points.size()));

      Triangulation dt (boost::make_transform_iterator(point_indices.begin(), On_the_fly_pair(m_points)),
                        boost::make_transform_iterator(point_indices.end(), On_the_fly_pair(m_points)));

      for (typename Triangulation::Finite_facets_iterator it = dt.finite_facets_begin ();
           it != dt.finite_facets_end (); ++ it)
        {
          CGAL::cpp11::array<std::size_t, 3> f;
          for (std::size_t i = 0; i < 3; ++ i)
            f[i] = it->first->vertex ((it->second + 1 + i)%4)->info();


          if (CGAL::squared_distance (m_points[f[0]], m_points[f[1]]) > d_DeltaEdge * d_DeltaEdge
              || CGAL::squared_distance (m_points[f[0]], m_points[f[2]]) > d_DeltaEdge * d_DeltaEdge
              || CGAL::squared_distance (m_points[f[1]], m_points[f[2]]) > d_DeltaEdge * d_DeltaEdge)
            continue;
          
          if (facet_coherence (f) > 0)
            *(facets ++) = f;
        }
    }
    
    Coherence_type facet_coherence (CGAL::cpp11::array<std::size_t, 3>& f) const
    {
      // O- FREEFORM CASE
      if (m_status[f[0]] == POINT &&
          m_status[f[1]] == POINT &&
          m_status[f[2]] == POINT)
        return FREEFORM;
      
      // 1- PLANAR CASE
      if (m_status[f[0]] == PLANE &&
          m_status[f[1]] == PLANE &&
          m_status[f[2]] == PLANE)
        {
          if (m_indices[f[0]] == m_indices[f[1]] &&
              m_indices[f[0]] == m_indices[f[2]])
            return PLANAR;
          else
            return INCOHERENT;
        }

      for (std::size_t i = 0; i < 3; ++ i)
        {
          Point_status sa = m_status[f[(i+1)%3]];
          Point_status sb = m_status[f[(i+2)%3]];
          Point_status sc = m_status[f[(i+3)%3]];
          std::size_t a = m_indices[f[(i+1)%3]];
          std::size_t b = m_indices[f[(i+2)%3]];
          std::size_t c = m_indices[f[(i+3)%3]];

          // O- FREEFORM CASE
          if (sa == POINT && sb == POINT && sc == PLANE)
            return FREEFORM;
          if (sa == POINT && sb == PLANE && sc == PLANE)
            {
              if (b == c)
                return FREEFORM;
              else
                return INCOHERENT;
            }
          
          // 2- CREASE CASES
          if (sa == EDGE && sb == EDGE && sc == PLANE)
            {
              if ((c == m_edges[a].planes[0] ||
                   c == m_edges[a].planes[1]) &&
                  (c == m_edges[b].planes[0] ||
                   c == m_edges[b].planes[1]))
                return CREASE;
              else
                return INCOHERENT;
            }

          if (sa == EDGE && sb == PLANE && sc == PLANE)
            {
              if (b == c &&
                  (b == m_edges[a].planes[0] ||
                   b == m_edges[a].planes[1]))
                return CREASE;
              else
                return INCOHERENT;
            }


          // 3- CORNER CASES
          if (sc == CORNER)
            {
              if (sa == EDGE && sb == EDGE)
                {
                  bool a0 = false, a1 = false, b0 = false, b1 = false;

                  if ((m_edges[a].planes[0] != m_edges[b].planes[0] &&
                       m_edges[a].planes[0] != m_edges[b].planes[1] &&
                       m_edges[a].planes[1] != m_edges[b].planes[0] &&
                       m_edges[a].planes[1] != m_edges[b].planes[1]))
                    return INCOHERENT;
                  
                  for (std::size_t j = 0; j < m_corners[c].planes.size (); ++ j)
                    {
                      if (m_corners[c].planes[j] == m_edges[a].planes[0])
                        a0 = true;
                      else if (m_corners[c].planes[j] == m_edges[a].planes[1])
                        a1 = true;
                      if (m_corners[c].planes[j] == m_edges[b].planes[0])
                        b0 = true;
                      else if (m_corners[c].planes[j] == m_edges[b].planes[1])
                        b1 = true;
                    }
                  if (a0 && a1 && b0 && b1)
                    return VERTEX;
                  else
                    return INCOHERENT;
                }
              else if (sa == PLANE && sb == PLANE)
                {
                  if (a != b)
                    return INCOHERENT;

                  for (std::size_t j = 0; j < m_corners[c].planes.size (); ++ j)
                    if (m_corners[c].planes[j] == a)
                      return VERTEX;
                  
                  return INCOHERENT;
                }
              else if (sa == PLANE && sb == EDGE)
                {
                  bool pa = false, b0 = false, b1 = false;
                  if (a != m_edges[b].planes[0] && a != m_edges[b].planes[1])
                    return INCOHERENT;
                  
                  for (std::size_t j = 0; j < m_corners[c].planes.size (); ++ j)
                    {
                      if (m_corners[c].planes[j] == a)
                        pa = true;
                      if (m_corners[c].planes[j] == m_edges[b].planes[0])
                        b0 = true;
                      else if (m_corners[c].planes[j] == m_edges[b].planes[1])
                        b1 = true;
                    }
                  if (pa && b0 && b1)
                    return VERTEX;
                  else
                    return INCOHERENT;
                }
              else if (sa == EDGE && sb == PLANE)
                {
                  bool a0 = false, a1 = false, pb = false;
                  if (b != m_edges[a].planes[0] && b != m_edges[a].planes[1])
                    return INCOHERENT;
                  
                  for (std::size_t j = 0; j < m_corners[c].planes.size (); ++ j)
                    {
                      if (m_corners[c].planes[j] == b)
                        pb = true;
                      if (m_corners[c].planes[j] == m_edges[a].planes[0])
                        a0 = true;
                      else if (m_corners[c].planes[j] == m_edges[a].planes[1])
                        a1 = true;
                    }
                  if (a0 && a1 && pb)
                    return VERTEX;
                  else
                    return INCOHERENT;
                }
              else
                return INCOHERENT;
            }
        }


      return INCOHERENT;
    }


  private:

    void project_inliers ()
    {
      for(std::size_t i = 0; i < m_planes.size (); ++ i)
        for (std::size_t j = 0; j < m_planes[i]->indices_of_assigned_points ().size(); ++ j)
          {
            std::size_t ind = m_planes[i]->indices_of_assigned_points ()[j];
            m_points[ind] = static_cast<Plane> (*(m_planes[i])).projection (m_points[ind]);
          }
    }

    void resample_planes (double epsilon)
    {
      double grid_length = epsilon * (std::sqrt(2.) - 1e-3);

      for (std::size_t c = 0; c < m_planes.size (); ++ c)
        {
          //plane attributes and 2D projection vectors
          Plane plane = static_cast<Plane> (*(m_planes[c]));
          Vector vortho = plane.orthogonal_vector();
          Vector b1 = plane.base1();
          Vector b2 = plane.base2();
			
          b1 = b1 / std::sqrt (b1 * b1);
          b2 = b2 / std::sqrt (b2 * b2);

          std::vector<Point_2> points_2d;

          //storage of the 2D points in "pt_2d"
          for (std::size_t j = 0; j < m_planes[c]->indices_of_assigned_points ().size(); ++ j)
            {
              std::size_t ind = m_planes[c]->indices_of_assigned_points ()[j];
              const Point& pt = m_points[ind];
              points_2d.push_back (Point_2 (b1.x() * pt.x() + b1.y() * pt.y() + b1.z() * pt.z(),
                                            b2.x() * pt.x() + b2.y() * pt.y() + b2.z() * pt.z()));
            }


          //creation of a 2D-grid with cell width = grid_length, and image structures
          CGAL::Bbox_2 box_2d = CGAL::bbox_2 (points_2d.begin(), points_2d.end());
          int Nx = (box_2d.xmax() - box_2d.xmin()) / grid_length + 1;
          int Ny = (box_2d.ymax() - box_2d.ymin()) / grid_length + 1;
          
          std::vector<std::vector<bool> > Mask (Nx, std::vector<bool> (Ny, false));
          std::vector<std::vector<bool> > Mask_border (Nx, std::vector<bool> (Ny, false));
          std::vector<std::vector<std::vector<int> > >
            point_map (Nx, std::vector<std::vector<int> > (Ny, std::vector<int>()));

          //storage of the points in the 2D-grid "point_map"
          for (std::size_t i = 0; i < points_2d.size(); ++ i)
            {
              std::size_t ind_x = (std::size_t)((points_2d[i].x() - box_2d.xmin()) / grid_length);
              std::size_t ind_y = (std::size_t)((points_2d[i].y() - box_2d.ymin()) / grid_length);
              Mask[ind_x][ind_y] = true;
              point_map[ind_x][ind_y].push_back (m_planes[c]->indices_of_assigned_points ()[i]);
            }

          //hole filing in Mask in 4-connexity
          for (int j = 1; j < Ny - 1; ++ j)
            for (int i = 1; i < Nx - 1; ++ i)
              if( !Mask[i][j]
                  && Mask[i-1][j] && Mask[i][j-1]
                  && Mask[i][j+1] && Mask[i+1][j] )
                Mask[i][j]=true;
					
          //finding mask border in 8-connexity	
          for (int j = 1; j < Ny - 1; ++ j)
            for (int i = 1; i < Nx - 1; ++ i)
              if( Mask[i][j] &&
                  ( !Mask[i-1][j-1] || !Mask[i-1][j] ||
                    !Mask[i-1][j+1] || !Mask[i][j-1] ||
                    !Mask[i][j+1] || !Mask[i+1][j-1] ||
                    !Mask[i+1][j]|| !Mask[i+1][j+1] ) )
                Mask_border[i][j]=true;
          
          for (int j = 0; j < Ny; ++ j)
            {
              if (Mask[0][j])
                Mask_border[0][j]=true;
              if (Mask[Nx-1][j])
                Mask_border[Nx-1][j]=true;
            }

          for (int i = 0; i < Nx; ++ i)
            {
              if(Mask[i][0])
                Mask_border[i][0]=true;
              if(Mask[i][Ny-1])
                Mask_border[i][Ny-1]=true;
            }

          //saving of points to keep
          for (int j = 0; j < Ny; ++ j)
            for (int i = 0; i < Nx; ++ i)
              if( point_map[i][j].size()>0)
                {
                  //inside: recenter (cell center) the first point of the cell and desactivate the others points 
                  if (!Mask_border[i][j] && Mask[i][j])
                    {
                      double x2pt = (i+0.5) * grid_length + box_2d.xmin();
                      double y2pt = (j+0.4) * grid_length + box_2d.ymin();
							
                      if (i%2 == 1)
                        {
                          x2pt = (i+0.5) * grid_length + box_2d.xmin();
                          y2pt = (j+0.6) * grid_length + box_2d.ymin();
                        }

                      FT X1 = x2pt * b1.x() + y2pt * b2.x() - plane.d() * vortho.x();
                      FT X2 = x2pt * b1.y() + y2pt * b2.y() - plane.d() * vortho.y();
                      FT X3 = x2pt * b1.z() + y2pt * b2.z() - plane.d() * vortho.z();

                      std::size_t index_pt = point_map[i][j][0];
                      m_points[index_pt] = Point (X1, X2, X3);
                      m_normals[index_pt] = m_planes[c]->plane_normal();
                      m_status[index_pt] = PLANE;

                      for (std::size_t np = 1; np < point_map[i][j].size(); ++ np)
                        m_status[point_map[i][j][np]] = SKIPPED;
                    }

                  //border: recenter (barycenter) the first point of the cell and desactivate the others points
                  else if (Mask_border[i][j] && Mask[i][j])
                    {
                      std::vector<Point> pts;
                      for (std::size_t np = 0; np < point_map[i][j].size(); ++ np)
                        pts.push_back (m_points[point_map[i][j][np]]);

                      m_points[point_map[i][j][0]] = CGAL::centroid (pts.begin (), pts.end ());
                      m_status[point_map[i][j][0]] = PLANE;
                      for (std::size_t np = 1; np < point_map[i][j].size(); ++ np)
                        m_status[point_map[i][j][np]] = SKIPPED;
                    }
                }
          // point use to filling 4-connexity holes are store in HPS_residus
              else if (point_map[i][j].size()==0 && !Mask_border[i][j] && Mask[i][j])
                {
                  double x2pt = (i+0.5) * grid_length + box_2d.xmin();
                  double y2pt = (j+0.49) * grid_length + box_2d.ymin();
                  if(i%2==1)
                    {
                      x2pt = (i+0.5) * grid_length + box_2d.xmin();
                      y2pt = (j+0.51) * grid_length + box_2d.ymin();
                    }
                  FT X1 = x2pt * b1.x() + y2pt * b2.x() - plane.d() * vortho.x();
                  FT X2 = x2pt * b1.y() + y2pt * b2.y() - plane.d() * vortho.y();
                  FT X3 = x2pt * b1.z() + y2pt * b2.z() - plane.d() * vortho.z();

                  m_points.push_back (Point (X1, X2, X3));
                  m_normals.push_back (m_planes[c]->plane_normal());
                  m_indices.push_back (c);
                  m_status.push_back (RESIDUS);
                }
        }

    }

    void find_pairs_of_adjacent_primitives (double radius)
    {
      typedef typename Traits::Search_traits Search_traits_base;
      typedef Search_traits_adapter <std::size_t, My_point_property_map, Search_traits_base> Search_traits;
      typedef CGAL::Kd_tree<Search_traits> Tree;
      typedef CGAL::Fuzzy_sphere<Search_traits> Fuzzy_sphere;

      My_point_property_map pmap (m_points);

      Tree tree (boost::counting_iterator<std::size_t> (0),
                 boost::counting_iterator<std::size_t> (m_points.size()),
                 typename Tree::Splitter(),
                 Search_traits (pmap));

      std::vector<std::vector<bool> > adjacency_table (m_planes.size (),
                                                       std::vector<bool> (m_planes.size (), false));

      //compute a basic adjacency relation (two primitives are neighbors
      //if at least one point of the primitive 1 is a k-nearest neighbor
      //of a point of the primitive 2 and vice versa)
      for (std::size_t i = 0; i < m_points.size (); ++ i)
        {
          std::size_t ind_i = m_indices[i];

          if (ind_i == minus1)
            continue;

          Fuzzy_sphere query (i, radius, 0., tree.traits());
          
          std::vector<std::size_t> neighbors;
          tree.search (std::back_inserter (neighbors), query);

          
          for (std::size_t k = 0; k < neighbors.size(); ++ k)
            {
              std::size_t ind_k = m_indices[neighbors[k]];
              if (ind_k != minus1 && ind_k != ind_i)
                adjacency_table[ind_i][ind_k] = true;
            }
        }

      //verify the symmetry and store the pairs of primitives in
      //m_edges
      for (std::size_t i = 0; i < adjacency_table.size() - 1; ++ i)
        for (std::size_t j = i + 1; j < adjacency_table[i].size(); ++ j)
          if ((adjacency_table[i][j]) && (adjacency_table[j][i]))
            m_edges.push_back (Edge (i, j));

    }

    void compute_edges (double epsilon)
    {
      for (std::size_t i = 0; i < m_edges.size(); ++ i)
        {
          boost::shared_ptr<Plane_shape> plane1 = m_planes[m_edges[i].planes[0]];
          boost::shared_ptr<Plane_shape> plane2 = m_planes[m_edges[i].planes[1]];       

          double angle_A = std::acos (std::abs (plane1->plane_normal() * plane2->plane_normal()));
          double angle_B = CGAL_PI - angle_A;

          CGAL::Object ob_temp = CGAL::intersection (static_cast<Plane>(*plane1),
                                                     static_cast<Plane>(*plane2));
          if (!assign (m_edges[i].support, ob_temp))
            {
              std::cerr << "Warning: bad plane/plane intersection" << std::endl;
              continue;
            }

          Vector direction_p1 (0., 0., 0.);
          for (std::size_t k = 0; k < plane1->indices_of_assigned_points ().size(); ++ k)
            {
              std::size_t index_point = plane1->indices_of_assigned_points ()[k];
              
              const Point& point = m_points[index_point];
              Point projected = m_edges[i].support.projection (point);
              if (std::sqrt (CGAL::squared_distance (point, projected))
                  < 2 * std::min (4., 1 / std::sin (angle_A)) * epsilon
                  && m_status[index_point] != SKIPPED)
                direction_p1 = direction_p1 + Vector (projected, point);
            }
          if (direction_p1.squared_length() > 0)
            direction_p1 = direction_p1 / std::sqrt (direction_p1 * direction_p1);

          Vector direction_p2 (0., 0., 0.);
          for (std::size_t k = 0; k < plane2->indices_of_assigned_points ().size(); ++ k)
            {
              std::size_t index_point = plane2->indices_of_assigned_points ()[k];
              
              const Point& point = m_points[index_point];
              Point projected = m_edges[i].support.projection (point);
              if (std::sqrt (CGAL::squared_distance (point, projected))
                  < 2 * std::min (4., 1 / std::sin (angle_A)) * epsilon
                  && m_status[index_point] != SKIPPED)
                direction_p2 = direction_p2 + Vector (projected, point);
            }
          if (direction_p2.squared_length() > 0)
            direction_p2 = direction_p2 / std::sqrt (direction_p2 * direction_p2);

          double angle = std::acos (direction_p1 * direction_p2);
      
          if (direction_p1.squared_length() == 0
              || direction_p2.squared_length() == 0
              || (std::fabs (angle - angle_A) > 1e-2
                  && std::fabs (angle - angle_B) > 1e-2 ))
            {
              m_edges[i].active = false;
            }
        }
    }

    void create_edge_anchor_points (double radius, double epsilon)
    {
      double d_DeltaEdge = std::sqrt (2.) * epsilon;
      double r_edge = d_DeltaEdge / 2.;
      
      for (std::size_t i = 0; i < m_edges.size(); ++ i)
        {
          boost::shared_ptr<Plane_shape> plane1 = m_planes[m_edges[i].planes[0]];
          boost::shared_ptr<Plane_shape> plane2 = m_planes[m_edges[i].planes[1]];

          const Line& line = m_edges[i].support;

          if (!(m_edges[i].active))
            {
              continue;
            }

          Vector normal = 0.5 * plane1->plane_normal () + 0.5 * plane2->plane_normal();
							
          //find set of points close (<attraction_radius) to the edge and store in intersection_points
          std::vector<std::size_t> intersection_points;
          for (std::size_t k = 0; k < plane1->indices_of_assigned_points().size(); ++ k)
            {
              std::size_t index_point = plane1->indices_of_assigned_points()[k];
              Point point = m_points[index_point];
              Point projected = line.projection (point);
              if (CGAL::squared_distance (point, projected) < radius * radius)
                intersection_points.push_back (index_point);
            }
          for (std::size_t k = 0; k < plane2->indices_of_assigned_points().size(); ++ k)
            {
              std::size_t index_point = plane2->indices_of_assigned_points()[k];
              Point point = m_points[index_point];
              Point projected = line.projection (point);
              if (CGAL::squared_distance (point, projected) < radius * radius)
                intersection_points.push_back (index_point);
            }

          if (intersection_points.empty ())
            {
              continue;
            }

          const Point& t0 = m_points[intersection_points[0]];
          Point t0p = line.projection (t0);
          double dmin = 0.;
          double dmax = 0.;
          Point Pmin = t0p;
          Point Pmax = t0p;
          Vector dir = line.to_vector ();
          
          //compute the segment of the edge
          for (std::size_t k = 0; k < intersection_points.size(); ++ k)
            {
              std::size_t ind = intersection_points[k];
              const Point& point = m_points[ind];
              Point projected = line.projection (point);
              double d = Vector (t0p, projected) * dir;
                  
              if (d < dmin)
                {
                  dmin = d;
                  Pmin = projected;
                }
              else if (d > dmax)
                {
                  dmax = d;
                  Pmax = projected;
                }
            }

          //faire un partitionnement ds une image 1D en votant si
          //a la fois au moins un point de plan1 et aussi de plan
          //2 tombent dans une case (meme pas que pour les plans).
          Segment seg (Pmin,Pmax);
          int number_of_division = std::sqrt (seg.squared_length ()) / d_DeltaEdge + 1;
          std::vector<std::vector<std::size_t> > division_tab (number_of_division);

          for (std::size_t k = 0; k < intersection_points.size(); ++ k)
            {
              std::size_t ind = intersection_points[k];
              const Point& point = m_points[ind];
              Point projected = line.projection (point);

              std::size_t tab_index = std::sqrt (CGAL::squared_distance (seg[0], projected)) / d_DeltaEdge;

              division_tab[tab_index].push_back (ind);
            }

          //C1-CREATE the EDGE
          std::vector<int> index_of_edge_points;
          for (std::size_t j = 0; j < division_tab.size(); ++ j)
            {
              bool p1found = false, p2found = false;
              for (std::size_t k = 0; k < division_tab[j].size () && !(p1found && p2found); ++ k)
                {
                  if (m_indices[division_tab[j][k]] == m_edges[i].planes[0])
                    p1found = true;
                  if (m_indices[division_tab[j][k]] == m_edges[i].planes[1])
                    p2found = true;
                }

              if (!(p1found && p2found))
                {
                  division_tab[j].clear();
                  continue;
                }

              Point perfect (seg[0].x() + (seg[1].x() - seg[0].x()) * (j + 0.5) / (double)number_of_division,
                             seg[0].y() + (seg[1].y() - seg[0].y()) * (j + 0.5) / (double)number_of_division,
                             seg[0].z() + (seg[1].z() - seg[0].z()) * (j + 0.5) / (double)number_of_division);

              // keep closest point, replace it by perfect one and skip the others
              double dist_min = (std::numeric_limits<double>::max)();
              std::size_t index_best = 0;

              for (std::size_t k = 0; k < division_tab[j].size(); ++ k)
                {
                  std::size_t inde = division_tab[j][k];

                  if (CGAL::squared_distance (line, m_points[inde]) < d_DeltaEdge * d_DeltaEdge)
                    m_status[inde] = SKIPPED; // Deactive points too close (except best, see below)
                  
                  double distance = CGAL::squared_distance (perfect, m_points[inde]);
                  if (distance < dist_min)
                    {
                      dist_min = distance;
                      index_best = inde;
                    }
                }

              m_points[index_best] = perfect;
              m_normals[index_best] = normal;
              m_status[index_best] = EDGE;
              m_indices[index_best] = i;
              m_edges[i].indices.push_back (index_best);

            }

          //C2-CREATE the ANCHOR
          Vector direction_p1(0,0,0);
          Vector direction_p2(0,0,0);

          for (std::size_t j = 0; j < division_tab.size() - 1; ++ j)
            {
              if (division_tab[j].empty () || division_tab[j+1].empty ())
                continue;
              Point anchor (seg[0].x() + (seg[1].x() - seg[0].x()) * (j + 1) / (double)number_of_division,
                            seg[0].y() + (seg[1].y() - seg[0].y()) * (j + 1) / (double)number_of_division,
                            seg[0].z() + (seg[1].z() - seg[0].z()) * (j + 1) / (double)number_of_division);
              
              Plane ortho = seg.supporting_line().perpendicular_plane(anchor); 

              std::vector<Point> pts1, pts2;
              //Computation of the permanent angle and directions
              for (std::size_t k = 0; k < division_tab[j].size(); ++ k)
                { 
                  std::size_t inde = division_tab[j][k];
                  std::size_t plane = m_indices[inde];
                  if (plane == m_edges[i].planes[0])
                    pts1.push_back (m_points[inde]);
                  else if (plane == m_edges[i].planes[1])
                    pts2.push_back (m_points[inde]);
                }

              Point centroid1 = CGAL::centroid (pts1.begin (), pts1.end ());
              Point centroid2 = CGAL::centroid (pts2.begin (), pts2.end ());

              Line line_p1;
              CGAL::Object ob_temp1 = CGAL::intersection (static_cast<Plane> (*plane1), ortho);
              if (!assign(line_p1, ob_temp1))
                std::cout<<"Warning: bad plane/plane intersection"<<std::endl;
              else
                {
                  Vector vecp1 = line_p1.to_vector();
                  vecp1 = vecp1/ std::sqrt (vecp1 * vecp1);
                  Vector vtest1 (anchor, centroid1);
                  if (vtest1 * vecp1<0)
                    vecp1 = -vecp1;

                  direction_p1 = direction_p1+vecp1;

                  Point anchor1 = anchor + vecp1 * r_edge;
                  m_points.push_back (anchor1);
                  m_normals.push_back (m_planes[m_edges[i].planes[0]]->plane_normal());
                  m_indices.push_back (m_edges[i].planes[0]);
                  m_status.push_back (PLANE);
                }

              Line line_p2;
              CGAL::Object ob_temp2 = CGAL::intersection (static_cast<Plane> (*plane2),ortho);
              if (!assign(line_p2, ob_temp2))
                std::cout<<"Warning: bad plane/plane intersection"<<std::endl;
              else
                {
                  Vector vecp2 = line_p2.to_vector();
                  vecp2 = vecp2 / std::sqrt (vecp2 * vecp2);
                  Vector vtest2 (anchor, centroid2);
                  if (vtest2 * vecp2 < 0)
                    vecp2 =- vecp2;

                  direction_p2 = direction_p2+vecp2;

                  Point anchor2 = anchor + vecp2 * r_edge;
                  m_points.push_back (anchor2);
                  m_normals.push_back (m_planes[m_edges[i].planes[1]]->plane_normal());
                  m_indices.push_back (m_edges[i].planes[1]);
                  m_status.push_back (PLANE);
                }

            }

          //if not information enough (not enough edges to create
          //anchor) we unactivate the edge, else we update the angle
          //and directions
          if ( !(direction_p1.squared_length()>0 || direction_p2.squared_length()>0) )
            {
              m_edges[i].active = false;
              for (std::size_t j = 0; j < m_edges[i].indices.size (); ++ j)
                m_status[m_edges[i].indices[j]] = SKIPPED;
            }
        }
    }

    void compute_corners (double radius)
    {
      if (m_edges.size () < 3)
        return;

      for (std::size_t i = 0; i < m_edges.size () - 2; ++ i)
        {
          if (!(m_edges[i].active))
            continue;

          for (std::size_t j = i + 1; j < m_edges.size () - 1; ++ j)
            {
              if (!(m_edges[j].active))
                continue;

              if (m_edges[i].planes[0] != m_edges[j].planes[0] &&
                  m_edges[i].planes[0] != m_edges[j].planes[1] &&
                  m_edges[i].planes[1] != m_edges[j].planes[0] &&
                  m_edges[i].planes[1] != m_edges[j].planes[1])
                continue;

              for (std::size_t k = j + 1; k < m_edges.size (); ++ k)
                {
                  if (!(m_edges[k].active))
                    continue;

                  std::set<std::size_t> planes;
                  planes.insert (m_edges[i].planes[0]);
                  planes.insert (m_edges[i].planes[1]);
                  planes.insert (m_edges[j].planes[0]);
                  planes.insert (m_edges[j].planes[1]);
                  planes.insert (m_edges[k].planes[0]);
                  planes.insert (m_edges[k].planes[1]);

                  if (planes.size () == 3) // Triple found
                    {
                      std::vector<std::size_t> vecplanes (planes.begin (), planes.end ());
                      m_corners.push_back (Corner (vecplanes[0], vecplanes[1], vecplanes[2],
                                                   i, j, k));
                    }
                }
            }
        }

      for (std::size_t i = 0; i < m_corners.size (); ++ i)
        {
          //calcul pt d'intersection des 3 plans
          Plane plane1 = static_cast<Plane> (*(m_planes[m_corners[i].planes[0]]));
          Plane plane2 = static_cast<Plane> (*(m_planes[m_corners[i].planes[1]]));
          Plane plane3 = static_cast<Plane> (*(m_planes[m_corners[i].planes[2]]));
          Line line;

          CGAL::Object ob_temp = CGAL::intersection(plane1, plane2);
          if (!assign(line, ob_temp))
            {
              std::cerr << "Warning: bad plane/plane intersection" << std::endl;
              continue;
            }
          else
            {
              CGAL::Object ob_temp2 = CGAL::intersection (line, plane3);
              if (!assign (m_corners[i].support, ob_temp2))
                {
                  std::cerr << "Warning: bad plane/line intersection" << std::endl;
                  continue;
                }
            }

          // test if point is in bbox + delta
          CGAL::Bbox_3 bbox = CGAL::bbox_3 (m_points.begin (), m_points.end ());
          
          double margin_x = 0.1 * (bbox.xmax() - bbox.xmin());
          double X_min = bbox.xmin() - margin_x;
          double X_max = bbox.xmax() + margin_x; 
          double margin_y = 0.1 * (bbox.ymax() - bbox.ymin());
          double Y_min = bbox.ymin() - margin_y;
          double Y_max = bbox.ymax() + margin_y; 
          double margin_z = 0.1* (bbox.zmax() - bbox.zmin());
          double Z_min = bbox.zmin() - margin_z;
          double Z_max = bbox.zmax() + margin_z;
          
          if ((m_corners[i].support.x() < X_min) || (m_corners[i].support.x() > X_max)
              || (m_corners[i].support.y() < Y_min) || (m_corners[i].support.y() > Y_max)
              || (m_corners[i].support.z() < Z_min) || (m_corners[i].support.z() > Z_max))
            {
              m_corners[i].active = false;
              continue;
            }

          // test if corner is in neighborhood of at least one point each of the 3 planes
          std::vector<bool> neighborhood (3, false);

          for (std::size_t k = 0; k < 3; ++ k)
            {
              for (std::size_t j = 0; j < m_edges[m_corners[i].edges[k]].indices.size(); ++ j)
                {
                  const Point& p = m_points[m_edges[m_corners[i].edges[k]].indices[j]];

                  if (CGAL::squared_distance (m_corners[i].support, p) < radius * radius)
                    {
                      neighborhood[k] = true;
                      break;
                    }
                }
            }

          if ( !(neighborhood[0] && neighborhood[1] && neighborhood[2]) )
            m_corners[i].active = false;
        }
    }

    void merge_corners (double radius)
    {
      for (std::size_t k = 0; k < m_corners.size(); ++ k)
        {
          if (!(m_corners[k].active))
            continue;

          int count_plane_number=3;
          
          for (std::size_t kb = k + 1; kb < m_corners.size(); ++ kb)
            {
              if (!(m_corners[kb].active))
                continue;

              int count_new_plane = 0;

              if (CGAL::squared_distance (m_corners[kb].support, m_corners[k].support) >= radius * radius)
                continue;

              for (std::size_t i = 0; i < m_corners[kb].planes.size (); ++ i)
                {
                  bool testtt = true; 
                  for (std::size_t l = 0; l < m_corners[k].planes.size(); ++ l)
                    if (m_corners[kb].planes[i] == m_corners[k].planes[l])
                      {
                        testtt = false;
                        break;
                      }
                  if (!testtt)
                    continue;

                  m_corners[k].planes.push_back (m_corners[kb].planes[i]);
                  ++ count_new_plane;
                  m_corners[kb].active = false;

                  std::vector<bool> is_edge_in (3, false);
                  for (std::size_t l = 0; l < m_corners[k].edges.size(); ++ l)
                    {
                      for (std::size_t j = 0; j < 3; ++ j)
                        if (m_corners[k].edges[l] == m_corners[kb].edges[j])
                          is_edge_in[j] = true;
                    }
                  for (std::size_t j = 0; j < 3; ++ j)
                    if (!(is_edge_in[j]))
                      m_corners[k].edges.push_back (m_corners[kb].edges[j]);

                }
              
              //update barycenter
              m_corners[k].support = CGAL::barycenter (m_corners[k].support, count_plane_number,
                                                       m_corners[kb].support, count_new_plane);
              count_plane_number += count_new_plane;
            }

          // Compute normal vector
          Vector normal (0., 0., 0.);
          for (std::size_t i = 0; i < m_corners[k].planes.size(); ++ i)
            normal = normal + (1. / (double)(m_corners[k].planes.size()))
              * m_planes[m_corners[k].planes[i]]->plane_normal();
          
          m_points.push_back (m_corners[k].support);
          m_normals.push_back (normal);
          m_indices.push_back (k);
          m_status.push_back (CORNER);
        }
    }

    void compute_corner_directions (double epsilon)
    {
      for (std::size_t k = 0; k < m_corners.size(); ++ k)
        {
          for (std::size_t ed = 0; ed < m_corners[k].edges.size(); ++ ed)
            {
              if (m_corners[k].edges[ed] < m_edges.size())
                {  
                  const Edge& edge = m_edges[m_corners[k].edges[ed]];

                  Vector direction (0., 0., 0.);
                  for (std::size_t i = 0; i < edge.indices.size(); ++ i)
                    {
                      std::size_t index_pt = edge.indices[i];
                      if (std::sqrt (CGAL::squared_distance (m_corners[k].support,
                                                             m_points[index_pt])) < 5 * epsilon)
                        direction = direction + Vector (m_corners[k].support, m_points[index_pt]);
                    }

                  if (direction.squared_length() > 1e-5)
                    m_corners[k].directions.push_back (direction / std::sqrt (direction * direction));
                  else
                    m_corners[k].directions.push_back (Vector (0., 0., 0.));
                }
              else
                m_corners[k].directions.push_back (Vector (0., 0., 0.));
            }
        }
    }
    
    void refine_sampling (double epsilon)
    {
      double d_DeltaEdge = std::sqrt (2.) * epsilon;

      for (std::size_t k = 0; k < m_corners.size(); ++ k)
        {
          if (!(m_corners[k].active))
            continue;
          
          for (std::size_t ed = 0; ed < m_corners[k].edges.size(); ++ ed)
            {
              const Edge& edge = m_edges[m_corners[k].edges[ed]];

              for (std::size_t i = 0; i < edge.indices.size(); ++ i)
                {
                  //if too close from a corner, ->remove
                  if (CGAL::squared_distance (m_corners[k].support, m_points[edge.indices[i]])
                      < d_DeltaEdge * d_DeltaEdge)
                    m_status[edge.indices[i]] = SKIPPED;
				
                  //if too close from a corner (non dominant side), ->remove
                  if (m_corners[k].directions[ed].squared_length() > 0
                      && (m_corners[k].directions[ed]
                          * Vector (m_corners[k].support, m_points[edge.indices[i]]) < 0)
                      && (CGAL::squared_distance (m_corners[k].support, m_points[edge.indices[i]])
                          < 4 * d_DeltaEdge * d_DeltaEdge))
                    m_status[edge.indices[i]] = SKIPPED;
                }
              
            }
        }

      for (std::size_t k = 0; k < m_corners.size(); ++ k)
        {
          if (!(m_corners[k].active))
            continue;
		
          for (std::size_t ed = 0; ed < m_corners[k].edges.size(); ++ ed)
            {
              if (m_corners[k].directions[ed].squared_length() <= 0.)
                continue;
              
              Edge& edge = m_edges[m_corners[k].edges[ed]];

              //rajouter un edge a epsilon du cote dominant si pas de point entre SS_edge/2 et 3/2*SS_edge
              bool is_in_interval = false;
              for (std::size_t i = 0; i < edge.indices.size(); ++ i)
                {
                  std::size_t index_pt = edge.indices[i];
                  double dist = CGAL::squared_distance (m_corners[k].support,
                                                        m_points[index_pt]);
                  if (m_status[index_pt] != SKIPPED
                      && dist < 1.5 * d_DeltaEdge && dist > d_DeltaEdge / 2)
                    {
                      Vector move (m_corners[k].support,
                                   m_points[index_pt]);
                      if (move * m_corners[k].directions[ed] > 0.)
                        {
                          is_in_interval = true;
                          break;
                        }
                    }
                }

              //rajouter un edge a 1 epsilon du cote dominant si pas de point entre SS_edge/2 et 3/2*SS_edge
              if (!is_in_interval)
                {
                  Point new_edge = m_corners[k].support + m_corners[k].directions[ed] * d_DeltaEdge;
                  m_points.push_back (new_edge);
                  m_normals.push_back (0.5 * m_planes[m_edges[m_corners[k].edges[ed]].planes[0]]->plane_normal()
                                       + 0.5 * m_planes[m_edges[m_corners[k].edges[ed]].planes[1]]->plane_normal());
                  m_status.push_back (EDGE);
                  m_indices.push_back (m_corners[k].edges[ed]);
                  edge.indices.push_back (m_points.size() - 1);
                }
						
              //rajouter un edge a 1/3 epsilon du cote dominant
              Point new_edge = m_corners[k].support + m_corners[k].directions[ed] * d_DeltaEdge / 3;
              m_points.push_back (new_edge);
              m_normals.push_back (0.5 * m_planes[m_edges[m_corners[k].edges[ed]].planes[0]]->plane_normal()
                                   + 0.5 * m_planes[m_edges[m_corners[k].edges[ed]].planes[1]]->plane_normal());
              m_status.push_back (EDGE);
              m_indices.push_back (m_corners[k].edges[ed]);
              edge.indices.push_back (m_points.size() - 1);
            }
        }

    }
    
  };
  
} /* namespace internal */
/// \endcond



// ----------------------------------------------------------------------------
// Public section
// ----------------------------------------------------------------------------

/// \ingroup PkgPointSetProcessing
  
/// This is an implementation of the Point Set Structuring algorithm. This
/// algorithm takes advantage of a set of detected planes: it detects adjacency
/// relationships between planes and resamples the detected planes, edges and
/// corners to produce a structured point set.
///
/// The size parameter `epsilon` is used both for detecting adjacencies and for
/// setting the sampling density of the structured point set.
///
/// For more details, please refer to \cgalCite{cgal:la-srpss-13}.
///
/// @tparam InputIterator Iterator over input points
///
/// @tparam OutputIterator Type of the output iterator. The type of the objects
/// put in it is `std::pair<Traits::Point_3, Traits::Vector_3>`.  Note that the
/// user may use a <A HREF="http://www.boost.org/libs/iterator/doc/function_output_iterator.html">function_output_iterator</A>
/// to match specific needs.
///
/// @tparam Traits A model of `EfficientRANSACTraits`
  

// This variant requires the kernel.
template <typename OutputIterator,
          typename InputIterator,
          typename Traits
>
OutputIterator
structure_point_set (InputIterator first,  ///< iterator over the first input point.
                     InputIterator beyond, ///< past-the-end iterator over the input points.
                     OutputIterator output, ///< output iterator where output points are put
                     Shape_detection_3::Efficient_RANSAC<Traits>&
                     shape_detection, ///< shape detection engine
                     double epsilon) ///< size parameter
{
  internal::Point_set_structuring<Traits> pss
    (first, beyond, shape_detection);

  pss.run (epsilon);

  pss.get_output (output);

  return output;
}



} //namespace CGAL

#endif // CGAL_STRUCTURE_POINT_SET_3_H

