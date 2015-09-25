#include "Tracer.h"
#include "atlimage.h"
//#include <omp.h>  // if there'is not std lib OpenMP
using namespace glm;

#define EPSILON 0.00001 // for comparing double with zero
#define MAX_DEEP 2 // maximum deep of reflecting
float distance_to_image = 150; // base distance for std resolution


SRay CTracer::MakeFirstRay(glm::uvec2 pixelPos, int xRes, int yRes) // Create ray from eye
{
	auto ray = SRay();
	ray.m_dir = Common::Vector3 (1.0*pixelPos.x-1.0*xRes/2, 1.0*pixelPos.y-1.0*yRes/2, distance_to_image);
	ray.m_dir.normalise();
	ray.m_start = Common::Vector3(0,0,0); // eye pos is fixed in 0
	return ray;
}


SRay CTracer::MakeRay(Common::Vector3 x, Common::Vector3 y) // Create all ray
{
	auto ray = SRay();
	ray.m_dir = y - x; // easy
	ray.m_start = x;
	return ray;
}

	// check intersection of ray and box 
	// from ray-tracing.ru/articles244.html ( see our readme.txt )
	// it was in .pdf with task
bool RayBoxIntersection( Common::Vector3 ray_pos, Common::Vector3 inv_dir, Common::Vector3 boxMin, Common::Vector3 boxMax, float& tmin, float& tmax )
{
  inv_dir = 1 / inv_dir;
  float lo = inv_dir.x * ( boxMin.x - ray_pos.x );
  float hi = inv_dir.x * ( boxMax.x - ray_pos.x );
  tmin  = min( lo, hi );
  tmax = max( lo, hi );
 
  float lo1 = inv_dir.y * ( boxMin.y - ray_pos.y );
  float hi1 = inv_dir.y * ( boxMax.y - ray_pos.y );
  tmin = max( tmin, min(lo1, hi1) );
  tmax = min( tmax, max(lo1, hi1) );
 
  float lo2 = inv_dir.z * ( boxMin.z - ray_pos.z );
  float hi2 = inv_dir.z * ( boxMax.z - ray_pos.z );
  tmin = max( tmin, min( lo2, hi2 ) );
  tmax = min( tmax, max( lo2, hi2 ) );
 
  return ( tmin <= tmax ) && ( tmax > 0.f );
}

	// array of crossed boxes by fixed ray
vector <int> get_cross_boxes( SRay ray, CScene * m_pScene ) 
{
	vector <int> result; // array of number of boxes
	vector <float> min_mas; // array of minimum  distance to boxes
	for (int i = 0; i < m_pScene -> obj.size(); i++ )
	{
		float min, max; // for using our function. max - not use
		if ( RayBoxIntersection ( ray.m_start, ray.m_dir, m_pScene -> obj[i] -> box.getMinimum(), m_pScene -> obj[i] -> box.getMaximum(), min, max ) == true )
		{
				// if crossing 
			result.push_back( i );
			min_mas.push_back( min );
		}
	}
		//sorting our array. 
		// it's very small arrays => easy sorting
	 for ( int i = 0; i < result.size(); i++ )
       for ( int j = i + 1; j < result.size(); j++ )
          if ( min_mas[i] > min_mas[j] )
          {
				// swap
             auto buf = min_mas[i];
             min_mas[i] = min_mas[j];
             min_mas[j] = buf;
				// swap
			 auto buf1 = result[i];
             result[i] = result[j];
             result[j] = buf1;
          }
	return result;
}

	// Check for shadow
	// integer num - for checking crossing with other boxes  
bool CTracer::visible( Common::Vector3 x, Common::Vector3 y, int num )
{
	SRay ray = MakeRay( y, x ); // ray from light
	vector < int > cross_boxes = get_cross_boxes( ray, m_pScene ); // array crossing boxes
	if ( cross_boxes.size() == 0 ) return true; // no crossing boxes
		// else branch
	double t; // distance to hit
	VertexData* tmp = new VertexData; // temp variable for using our func without changes
		int i = 0;
		// check hiting in every box
	while ( ( i < cross_boxes.size() ) && ( ( m_pScene -> obj[cross_boxes[i]] -> shadow == false ) || !( m_pScene -> obj[cross_boxes[i]] -> check3( ray, t, tmp, tmp, tmp, Common::Vector3() ) ) ) ) i++;
		// free memory
	delete tmp;
		// no hit
	if ( i >= cross_boxes.size() ) return true;
		// bad hit 
	if ( cross_boxes[i] == num ) return true;
	return false;
}

	// Get base color ( Fong )
Common::Vector3 CTracer::get_shade( Common::Vector3 hit, Common::Vector3 lightPos, Common::Vector3 n, bool blink )
{
		// standart Fong formules
	Common::Vector3 eyePos = Common::Vector3(0,0,0);
	Common::Vector3 p = hit;
	Common::Vector3 l =  ( lightPos - p );   
		l.normalise();
    Common::Vector3 v = ( eyePos   - p );                 
		v.normalise();
		n.normalise();

	Common::Vector3  diffColor = Common::Vector3 ( 0.7, 0.7, 0.7 );
    Common::Vector3 specColor = Common::Vector3 ( 0.3, 0.3, 0.3 ) * blink; // no blinking material

	const double specPower = 40;

    Common::Vector3 r = -v;
		r =  r.reflect( n );
    Common::Vector3 diff = diffColor * max ( n.dotProduct(l), 0 );
	double loc = l.dotProduct( r );
    Common::Vector3 spec = specColor * pow (max( loc, 0 ), specPower);

    return diff + spec;
}


	// this function already in math.h
	// getting reflect direction of ray
Common::Vector3 reflect_dir(Common::Vector3 dir, Common::Vector3 normal)
{
	Common::Vector3 temp = normal * dir.dotProduct(normal) *(-2);
    Common::Vector3 rVec = temp + dir;
    rVec.normalise();
    return rVec;
}

	 // Trace ray, compute its color
glm::vec3 CTracer::TraceRay( SRay ray, int deep_recurse=0, int shadow_enable = 0 )
{
		// std color
	vec3 color(0, 0, 0);

		// tracing ray
	vector <int> cross_boxes = get_cross_boxes( ray, m_pScene );
	if ( cross_boxes.size() == 0 ) { return color; }
		//else branch
	double t; // distance to hit
	int i = 0;
		// sides of triangle 
	VertexData * ug_1 = new VertexData;
	VertexData * ug_2 = new VertexData;
	VertexData * ug_3 = new VertexData;
	Common::Vector3 n;

		// check hit in every box
	while ( ( i < cross_boxes.size() ) && !( m_pScene -> obj[cross_boxes[i]] -> check3( ray, t, ug_1, ug_2, ug_3, n) ) ) i++;
		// free memory
	delete ug_1;
	delete ug_2;
	delete ug_3;
		// no hit
	if ( i >= cross_boxes.size() ) { return color; } 
		// else branch
		
		// compute point of hit 
	auto hit_point = ray.m_start + ray.m_dir*t;
		// position of light 
		// it's behid eye
		// we don't see it 
	Common::Vector3 light = Common::Vector3( -105, 25, 7 );
	
		// check for shadow
	if ( !shadow_enable || visible( hit_point, light, cross_boxes[i] ) )
	{
			// get fong shade
        auto shade = get_shade ( hit_point, light, n, m_pScene -> obj[cross_boxes[i]] -> blink );
			// add this shade
		color += vec3(shade.x,shade.y,shade.z);
			// translate color
		color = glm::vec3( color.x * m_pScene -> obj[cross_boxes[i]] -> translate_color.x, color.y *m_pScene -> obj[cross_boxes[i]] -> translate_color.y, color.z * m_pScene -> obj[cross_boxes[i]] -> translate_color.z );
	}
	
		// reflecting ray
	if ( m_pScene -> obj[cross_boxes[i]] -> reflection > EPSILON && deep_recurse < MAX_DEEP )
	{
		SRay reflRay;
			n.normalise();
			//getting reflect ray
		reflRay.m_dir = ray.m_dir.reflect(n);
		reflRay.m_start = hit_point;
			// flag up
		deep_recurse++;
			// small translate for no zero hit
		reflRay.m_start += EPSILON * 10000 * reflRay.m_dir;
		color += m_pScene -> obj[cross_boxes[i]] -> reflection * TraceRay( reflRay, deep_recurse, shadow_enable );
			//flag down
		deep_recurse--;
	}
	return color;
}
	// Main func
void CTracer::RenderImage( int xRes, int yRes, int shadow_enable, int MP_enable, int appr_enable )
{
	if ( appr_enable ) // AAx4
	{
		xRes *= 2;
		yRes *= 2;
	}
  m_camera.m_resolution = uvec2( xRes, yRes );
  m_camera.m_pixels.resize( xRes * yRes );
  if (yRes!=768)
  {
		distance_to_image=distance_to_image*yRes/768;
  }

	// start timer
  time_t t = time(NULL); 

  if (MP_enable>0) 
  {
	cout<<"\r\nMultiprocessing";
		// OpenMP
	#pragma omp parallel for
	  for(int i = 0; i < yRes; i++)
	  {
		for(int j = 0; j < xRes; j++)
		{
		  SRay ray = MakeFirstRay(uvec2(j, i),xRes,yRes);
		  int deep_recurse = 0;
		  auto k = TraceRay(ray,deep_recurse,shadow_enable);
		  m_camera.m_pixels[i * xRes + j] = k;
		}
	  }
  }
  else 
  {
	  cout<<"\r\nNON Multiprocessing";
	  for(int i = 0; i < yRes; i++)
	  {
		for(int j = 0; j < xRes; j++)
		{
		  SRay ray = MakeFirstRay(uvec2(j, i),xRes,yRes);
		  int deep_recurse = 0;
		  auto k = TraceRay(ray,deep_recurse,shadow_enable);
		  m_camera.m_pixels[i * xRes + j] = k;
		}
	  }
  }
  		// timer
  cout << "\r\n Time: "<<(int)((time(NULL)-t)*0.7 /* coef. which our notebook slower, 
												than most slowest our PC*/ ) << " sec." << endl;
}

	// saving image
void CTracer::SaveImageToFile(std::string fileName, int appr_enable)
{
  cout<<"\r\nSaving image\r\n";
  CImage image;
  if (!appr_enable){
	  int width = m_camera.m_resolution.x;
	  int height = m_camera.m_resolution.y;

	  image.Create(width, height, 24);
    
		int pitch = image.GetPitch();
		unsigned char* imageBuffer = (unsigned char*)image.GetBits();

		if (pitch < 0)
		{
			imageBuffer += pitch * (height - 1);
			pitch =- pitch;
		}

		int i, j;
		int imageDisplacement = 0;
		int textureDisplacement = 0;

		for (i = 0; i < height; i++)
		{
		for (j = 0; j < width; j++)
		{
		  vec3 color = m_camera.m_pixels[textureDisplacement + j];

		  imageBuffer[imageDisplacement + j * 3] = clamp(color.b, 0.0f, 1.0f) * 255.0f;
		  imageBuffer[imageDisplacement + j * 3 + 1] = clamp(color.g, 0.0f, 1.0f) * 255.0f;
		  imageBuffer[imageDisplacement + j * 3 + 2] = clamp(color.r, 0.0f, 1.0f) * 255.0f;
		}

			imageDisplacement += pitch;
			textureDisplacement += width;
		}
  }
  else // aa4x
  {
	  int width = m_camera.m_resolution.x/2;
	  int height = m_camera.m_resolution.y/2;

	  image.Create(width, height, 24);
    
		int pitch = image.GetPitch();
		unsigned char* imageBuffer = (unsigned char*)image.GetBits();

		if (pitch < 0)
		{
			imageBuffer += pitch * (height - 1);
			pitch =- pitch;
		}

		int i, j;
		int imageDisplacement = 0;
		int textureDisplacement = 0;

		for (i = 0; i+1 < height*2; i+=2)
		{
			for (j = 0; j+1 < width*2; j+=2)
			{
				//cout<<textureDisplacement + j<<" ";
			  vec3 color1 = m_camera.m_pixels[textureDisplacement + j];
			  vec3 color2 = m_camera.m_pixels[textureDisplacement + j+1];
			  vec3 color3 = m_camera.m_pixels[textureDisplacement+width*2 + j];
			  vec3 color4 = m_camera.m_pixels[textureDisplacement+width*2 + j+1];

			  vec3 color = ( color1 + color2+ color3 + color4);

			  imageBuffer[imageDisplacement + j/2 * 3] = clamp(color.b/4, 0.0f, 1.0f) * 255.0f;
			  imageBuffer[imageDisplacement + j/2 * 3 + 1] = clamp(color.g/4, 0.0f, 1.0f) * 255.0f;
			  imageBuffer[imageDisplacement + j/2 * 3 + 2] = clamp(color.r/4, 0.0f, 1.0f) * 255.0f;
			}
			imageDisplacement += pitch;
			textureDisplacement += width*4;
		}
  }
  image.Save(fileName.c_str());
  image.Destroy();
  cout<<"\r\nALL FINISHED!\r\n";
}