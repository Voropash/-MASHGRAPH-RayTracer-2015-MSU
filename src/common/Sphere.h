#ifndef __Sphere_H_
#define __Sphere_H_


#include "Vector3.h"

namespace Common {


	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/
	/** A sphere primitive, mostly used for bounds checking. 
    @remarks
        A sphere in math texts is normally represented by the function
        x^2 + y^2 + z^2 = r^2 (for sphere's centered on the origin). Ogre stores spheres
        simply as a center point and a radius.
    */
    class Sphere
    {
    protected:
        float mRadius;
        Vector3 mCenter;
    public:
        /** Standard constructor - creates a unit sphere around the origin.*/
        Sphere() : mRadius(1.0), mCenter(Vector3::ZERO) {}
        /** Constructor allowing arbitrary spheres. 
            @param center The center point of the sphere.
            @param radius The radius of the sphere.
        */
        Sphere(const Vector3& center, float radius)
            : mRadius(radius), mCenter(center) {}

        /** Returns the radius of the sphere. */
        float getRadius(void) const { return mRadius; }

        /** Sets the radius of the sphere. */
        void setRadius(float radius) { mRadius = radius; }

        /** Returns the center point of the sphere. */
        const Vector3& getCenter(void) const { return mCenter; }

        /** Sets the center point of the sphere. */
        void setCenter(const Vector3& center) { mCenter = center; }

		/** Returns whether or not this sphere intersects another sphere. */
		bool intersects(const Sphere& s) const
		{
            return (s.mCenter - mCenter).squaredLength() <=
                Math::Sqr(s.mRadius + mRadius);
		}
		/** Returns whether or not this sphere intersects a box. */
		bool intersects(const AxisAlignedBox& box) const
		{
			return Math::intersects(*this, box);
		}
		/** Returns whether or not this sphere intersects a plane. */
		bool intersects(const Plane& plane) const
		{
			return Math::intersects(*this, plane);
		}
		/** Returns whether or not this sphere intersects a point. */
		bool intersects(const Vector3& v) const
		{
            return ((v - mCenter).squaredLength() <= Math::Sqr(mRadius));
		}
		/** Merges another Sphere into the current sphere */
		void merge(const Sphere& oth)
		{
			Vector3 diff =  oth.getCenter() - mCenter;
			float lengthSq = diff.squaredLength();
			float radiusDiff = oth.getRadius() - mRadius;
			
			// Early-out
			if (Math::Sqr(radiusDiff) >= lengthSq) 
			{
				// One fully contains the other
				if (radiusDiff <= 0.0f) 
					return; // no change
				else 
				{
					mCenter = oth.getCenter();
					mRadius = oth.getRadius();
					return;
				}
			}
			
			float length = Math::Sqrt(lengthSq);
			
			Vector3 newCenter;
			float newRadius;
			if ((length + oth.getRadius()) > mRadius) 
			{
				float t = (length + radiusDiff) / (2.0f * length);
				newCenter = mCenter + diff * t;
			} 
			// otherwise, we keep our existing center
			
			newRadius = 0.5f * (length + mRadius + oth.getRadius());
			
			mCenter = newCenter;
			mRadius = newRadius;
		}
        

    };
	/** @} */
	/** @} */

}

#endif

