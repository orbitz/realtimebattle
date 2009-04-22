/*
RealTimeBattle, a robot programming game for Unix
Copyright (C) 1998-2000  Erik Ouchterlony and Ragnar Ouchterlony

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <math.h>
#include "Vector2D.h"

#include <string>
using namespace std;

void Error(const bool fatal, const string& function_name, const string& error_msg);

//  Vector2D::Vector2D()
//  {
//    vector[0] = 0.0;
//    vector[1] = 0.0;
//  }

//  Vector2D::Vector2D(const Vector2D& old)
//  {
//    vector[0] = old.vector[0];
//    vector[1] = old.vector[1];
//  }

//  Vector2D::Vector2D(const double x, const double y)
//  {
//    vector[0] = x;
//    vector[1] = y;
//  }

Vector2D&
Vector2D::copy_vector(const Vector2D& other)
{
  vector[0] = other.vector[0];
  vector[1] = other.vector[1];
  return( *this );
}


Vector2D
operator+(const Vector2D& vec1, const Vector2D& vec2)
{
  return( Vector2D( vec1.vector[0] + vec2.vector[0],
                    vec1.vector[1] + vec2.vector[1] ) );
}



Vector2D
operator-(const Vector2D& vec)
{
  return Vector2D( -vec.vector[0], -vec.vector[1] );
}


Vector2D
rotate(const Vector2D& vec, const double angle)
{
  return( Vector2D( cos(angle)*vec.vector[0] - sin(angle)*vec.vector[1], 
                    sin(angle)*vec.vector[0] + cos(angle)*vec.vector[1] ) ); 
}

Vector2D
rotate90(const Vector2D& vec)
{
  return Vector2D( -vec.vector[1], vec.vector[0] );
}

Vector2D
operator*(const Vector2D& vec, const double factor)
{
  return Vector2D( vec.vector[0] * factor, vec.vector[1] * factor );
}

Vector2D
operator*(const double factor, const Vector2D& vec)
{
  return( vec*factor );
}

Vector2D
operator/(const Vector2D& vec, const double denom)
{
  return Vector2D( vec.vector[0]/denom, vec.vector[1]/denom );
}

Vector2D&
Vector2D::operator=(const Vector2D& other)
{
  copy_vector(other);
  return( *this );
}

int
operator==(const Vector2D& vec1, const Vector2D& vec2)
{
  return( vec1.vector[0] == vec2.vector[0] && 
          vec1.vector[1] == vec2.vector[1] ); 
}

ostream&
operator<<(ostream& os, const Vector2D& vec)
{
  os.setf(ios::fixed , ios::floatfield);
  os.precision(6);
  os << "( " << vec.vector[0] << " , " << vec.vector[1] << " )";
  return( os );
}

istream&
operator>>(istream& is, Vector2D& vec)
{
  is >> vec.vector[0] >> vec.vector[1];
  return( is );
}

double
Vector2D::operator[](int index) const
{
  if( index >= 0 && index <= 1 ) return( vector[index] );
  Error(true, "Vector index out of range", "Vector2D::operator[]");

  return 0.0;
}

Vector2D&
Vector2D::operator+=(const Vector2D& other)
{
  vector[0] += other.vector[0];  
  vector[1] += other.vector[1];  
  return( *this );
}

Vector2D&
Vector2D::operator-=(const Vector2D& other)
{
  vector[0] -= other.vector[0];  
  vector[1] -= other.vector[1];  
  return( *this );
}

Vector2D&
Vector2D::operator*=(const double factor)
{
  vector[0] *= factor;
  vector[1] *= factor;
  return( *this );
}

Vector2D&
Vector2D::operator/=(const double denom)
{
  vector[0] /= denom;
  vector[1] /= denom;
  return( *this );
}

double
length(const Vector2D& vec)
{
  return( sqrt(vec.vector[0]*vec.vector[0] + 
               vec.vector[1]*vec.vector[1]) );
}

double
vec2angle(const Vector2D& vec)
{
  return atan2(vec.vector[1], vec.vector[0]);
}


Vector2D
angle2vec(const double angle)
{
  return Vector2D(cos(angle),sin(angle));
}


Vector2D&
Vector2D::normalize()
{
  *this /= length(*this);
  return( *this ); 
}

Vector2D
unit(const Vector2D& vec)
{
  Vector2D uvec(vec);
  uvec /= length(vec);
  return( uvec ); 
}
