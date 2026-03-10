#include <iostream>
#include <VX/vx.h>
using namespace std;

int main()
{
	cout<<"This is the program to understand the vxImage Create, Query, Patch\n";

	vx_context cont = vxCreateContext();

	vx_graph gra = vxCreateGraph(cont);

	vx_image iimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);
	// vx_image iimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U16); // gaussian accept the image to be of type U8 so verifyGraph Failed 
	vx_image oimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);

	// Query image 
	vx_uint32 width ;
	vx_uint32 height ;
	vx_df_image form;
	vxQueryImage(iimg, VX_IMAGE_HEIGHT, &height, sizeof(width) );
	vxQueryImage(iimg, VX_IMAGE_WIDTH, &width, sizeof(width) );
	vxQueryImage(iimg, VX_IMAGE_FORMAT, &form, sizeof(form));
	cout<<"[DEBUG PRINT] width = "<<  width << " height = " << height <<endl; 


	vx_node nodeObj = vxGaussian3x3Node(gra, iimg, oimg);

	vx_status stat = vxVerifyGraph(gra);
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph failed\n";
	}

	stat = vxProcessGraph(gra);
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxProcessGraph success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxProcessGraph failed\n";
	}


	// Take Raw image 
	vx_rectangle_t rect;
	rect.start_x = 0;
	rect.start_y = 0;
	rect.end_x = width;
	rect.end_y = height;

	vx_map_id map_id;
	vx_imagepatch_addressing_t addr;
	void * ptr ;
	stat = vxMapImagePatch(oimg,&rect, 0, &map_id, &addr, &ptr, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxMapImagePatch success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxMapImagePatch failed\n";
	}

	FILE * fptr = fopen("output.ppm", "wb");
	fprintf(fptr, "P5\n640 640\n255\n");
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			fwrite((uint8_t *)ptr + i*addr.stride_y, 1, 640, fptr);
		}
	}
	fclose(fptr);

	vxUnmapImagePatch(oimg, map_id);
	// Release image
	vxReleaseImage(&iimg);
	vxReleaseImage(&oimg);

	// Release Graph 
	vxReleaseGraph(&gra);

	// Release context
	vxReleaseContext(&cont);
}