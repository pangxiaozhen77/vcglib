/****************************************************************************
* VCGLib                                                            o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004                                                \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *   
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
/****************************************************************************
  History

$Log: not supported by cvs2svn $
Revision 1.6  2004/10/09 13:48:02  ponchio
Fixed bug in init.

Revision 1.5  2004/05/10 13:15:54  cignoni
missing ending newline

Revision 1.4  2004/05/05 08:20:47  cignoni
syntax error in set and better comment

Revision 1.3  2004/04/28 16:36:55  turini
Changed :
in Distance(plane, point) :
return plane.Direction() * point - plane.Offset;
in
return plane.Direction() * point - plane.Offset();

Revision 1.2  2004/04/28 11:19:52  turini
Changed :
in Init(p0, norm)   _dist = p0 * _dir;   in   _offset = p0 * _dir;
Changed :
in Init(p0, p1, p2)   _offset = p0 * _dist;   in   _offset = p0 * _dir;

Revision 1.1  2004/03/31 22:19:24  ponchio
Untested first draft.


****************************************************************************/



#ifndef VCG_PLANE3_H
#define VCG_PLANE3_H

#include <vcg/space/point3.h>

namespace vcg {

/** \addtogroup space */
/*@{*/
/** 
Templated class for 2D planes in 3D spaces.
  This is the class for infinite planes in 3D space. A Plane is stored just as a Point3 and a scalar:
	a direction (not necessarily normalized), and a distance from the origin
	@param T (template parameter) Specifies the type of scalar used to represent coords.
	@param NORM: if on, the direction is always Normalized
*/
template <class T, bool NORM=true> class Plane3 {
public: 
  typedef T ScalarType;
	typedef Point3<T> PointType;

private:
  /// Distance
  ScalarType _offset;
  ///Direction (not necessarily normalized unless NORM is true)
  PointType _dir;

public:
  //@{
	 /** @name Constructors 
	**/
	/// The empty constructor
  Plane3() {}
  /// The (distance, direction) constructor
  Plane3(const ScalarType &dist, const PointType &dir) { Set(dist, dir); }

//@{
	 /** @name Members to access the distance or direction
	   Direction() cannot be assigned directly.
		 Use SetDirection() or Set() instead. This is mandatory to make possible the automatic autonormalization template mechanism.
     Note that if you have to set both direction and offset it can be more efficient to set them toghether 
	**/		
  const ScalarType &Offset() const { return _offset; } 
  ScalarType &Offset() { return _offset; } 
  	/// sets the origin
	void SetOffset( const ScalarType &o ) {	_offset=o; }
	
  const PointType &Direction() const { return _dir; } 
		/// sets the direction
	void SetDirection( const PointType & dir)	{	
    _dir=dir; 
    if (NORM) _dir.Normalize();  
  }
		/// sets origin and direction.
	void Set( const ScalarType & off, const PointType & dir ) {
    SetOffset(off); 
    SetDirection(dir); 
  }
  void Set( const PointType & dir, const ScalarType & off) {Set(off,dir);}
	
    /// Operator to compare two lines
	bool operator==(Plane3 const &p) const	{	
    return _offset == p._offset && _dir == p._dir; 
  }
	  /// Operator to dispare two lines
	bool operator!=(Plane3 const &p) const	{	
    return _offset != p._offset || _dir != p._dir; 
  }
	
  ///Project a point on the plane
  PointType Projection(PointType &p) const	{
		ScalarType k = p * _dir - _offset;
		return p - _dir * k;
	}
		
  /// Function to normalize direction
  void Normalize() {
	  _dir.Normalize();
  }

	/// Calculates the plane passing through three points (Rename this method)
  void Init(const PointType &p0, const PointType &p1, const PointType &p2) {
    _dir = (p2 - p0) ^ (p1 - p0);
    if(NORM) Normalize();
    _offset = p0 * _dir;
  }

  /// Calculates the plane passing through a point and the normal (Rename this method
  inline void Init(const PointType &p0, const PointType &norm) {
	  _dir = norm;
	  _offset = p0 * _dir;
  }
};	// end class Plane3

typedef Plane3<float>  Plane3f;
typedef Plane3<double> Plane3d;

///Distance plane - point (Move this function to somewhere else)
template<class T> T Distance(const Plane3<T> &plane, const Point3<T> &point) {   
	return plane.Direction() * point - plane.Offset();      
}

	///Distance point-plane (Move this function to somewhere else)
template<class T> T Distance(const Point3<T> &point, const Plane3<T> &plane) {
  return plane.Direction() * point - plane.Offset;      
}

} // end namespace


#endif
