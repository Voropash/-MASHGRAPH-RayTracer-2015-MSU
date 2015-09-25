#pragma once

#include "glm/glm.hpp"
#include "Types.h"
#include "Scene.h"
#include "common/Math.h"
#include "string"
#include "time.h"

class CTracer
{
public:
  SRay MakeFirstRay( glm::uvec2 pixelPos, int xRes, int yRes );  // Create ray from eye
  SRay MakeRay( Common::Vector3 x, Common::Vector3 y ); // Create all ray
  bool CTracer::visible( Common::Vector3 x, Common::Vector3 y, int num ); // Check for shadow
  Common::Vector3 get_shade( Common::Vector3 hit, Common::Vector3 lightPos, Common::Vector3 n, bool blind ); // Get base color ( Fong )
  glm::vec3 TraceRay( SRay ray, int deep_recurse, int shadow_enable ); // Trace ray, compute its color
  void RenderImage( int xRes, int yRes, int shadow_enable, int MP_enable, int appr_enable ); // Main func
  void SaveImageToFile( std::string fileName, int appr_enable ); // Save result to file

public:
  SCamera m_camera;
  CScene* m_pScene; // Our scene
};