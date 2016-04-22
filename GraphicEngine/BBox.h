#pragma once

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Utils.h"
#include "Vectors.h"

class BBox {

public:
	double x0, x1, y0, y1, z0, z1;
		
	BBox(void);										
						
	BBox(	const double x0, const double x1,			
			const double y0, const double y1, 
			const double z0, const double z1);

	BBox(const Vector3 p0, const Vector3 p1);

	BBox(const BBox& bbox);							

	BBox& 												
	operator= (const BBox& rhs);			

	~BBox(void);										
		
	bool 															
	hit(const Ray& ray) const;
	
	bool
	inside(const Vector3& p) const;
};


