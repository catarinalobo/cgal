namespace CGAL {

/*!
\ingroup PkgGenerators
The class `Ogon_2` is a representation of the orthogonal polygons (ogons, for short).

An instance of `Ogon_2` provides a list of vertices of a particular orthogonal polygon and
the grid orthogonal polygon (grid ogon) which originated it. 
Any ogon's vertex has non-negative integer coordinates.

\sa `CGAL::Grid_ogon_2`
*/
  
class Ogon_2: public Grid_ogon_2 {
  public:

    /// \name Creation 
    /// @{

    /*!
    Default constructor. 
    A unit square is created. 
    */ 
  	Ogon_2();

    /*!
    Constructor that creates the orthogonal polygon with vertices defined by the range `[first, last)`.
    `vertices` must follow the counterclockwise order.

    \cgalHeading{Requires}
    - The value type of \c InputIterator must be \c Point_2.
    */
    template<class InputIterator> Ogon_2(Grid_ogon_2 grid_ogon, InputIterator first, InputIterator last);


    /*!
    Copy constructor.
    */
    Ogon_2(const Ogon_2& ogon);

    /// @}

    /// \name Operations 
    /// @{

    /*!
    assignment operator. 
    */ 
    Ogon_2& operator=(Ogon_2 ogon);

  
    /*!
    returns the grid orthogonal polygon that originated (i.e., that identifies) the orthogonal polygon.
    */
    Grid_ogon_2 get_grid_ogon_2();

    /// @}

};/* end Ogon_2 */

} /* end namespace CGAL */