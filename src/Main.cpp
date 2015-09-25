#include "Tracer.h"
#include "stdio.h"
#include "Math.h"


void main(int argc, char** argv)
{
		// basis template
  CTracer tracer;
  CScene scene;
		// default settings
  int xRes = 1024; 
  int yRes = 768;
  int shadow_enable = 1;
  int MP_enable = 1;
  int appr_enable = 0;
		// std name of settings
  if(argc == 1) { argc = 2; argv[1] = "set.txt"; }

  if(argc == 2) // There is input file in parameters
  {
		// read settings from file
    FILE* file = fopen(argv[1], "r");
    if(file)
    {
      int xResFromFile = 0;
      int yResFromFile = 0;
	  int shadow_enable_file = 1;
	  int MP_enable_file = 1;
	  int appr_enable_file = 0;
      if(fscanf(file, "%d %d %d %d %d", &xResFromFile, &yResFromFile, &shadow_enable_file, &MP_enable_file, &appr_enable_file) == 5)
      {
        xRes = xResFromFile;
        yRes = yResFromFile;
		shadow_enable = shadow_enable_file;
		MP_enable = MP_enable_file;
		appr_enable = appr_enable_file;
      }
      else
        printf("Invalid config format! Using default parameters.\r\n");

      fclose(file);
    }
    else
      printf("Invalid config path! Using default parameters.\r\n");
  }
  else
    printf("No config! Using default parameters.\r\n");

  tracer.m_pScene = &scene;
  tracer.RenderImage(xRes, yRes, shadow_enable, MP_enable, appr_enable);
  tracer.SaveImageToFile("Result.png", appr_enable);

  system("pause");


}