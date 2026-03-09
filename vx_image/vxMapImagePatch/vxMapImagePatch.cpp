#include <iostream>
#include <VX/vx.h>

using namespace std;

int main()
{
	cout<<"This is the code to understand the vxMapImagePath in OpenVX \n";
	
	vx_context cont = vxCreateContext();

	vx_graph gra = vxCreateGraph(cont);

	vx_image iimg =vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);
	vx_image oimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);


	vx_node node1 = vxGaussian3x3Node(gra, iimg, oimg); 

	vx_status stat= vxVerifyGraph(gra);
	if(stat== VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph success\n";
		stat = vxProcessGraph(gra);
	}
	else
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph failed\n";
		return -1;
	}

	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxProcessGraph success \n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxProcessGraph failed\n";
		return -1;
	}

	// Memory handling 
	// set the Region of Intrest 
	vx_rectangle_t rect ;
	rect.start_x =0;
	rect.start_y =0;
	rect.end_x = 640;
	rect.end_y = 640;

	vx_map_id map_id ;
	vx_imagepatch_addressing_t addr;
	void * ptr ;

	stat = vxMapImagePatch(oimg,&rect, 0, &map_id, &addr, &ptr, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxMapImagePath success\n";
	}
	else
	{	
		cout<<"[DEBUG PRINT] vxMapImagePath failed\n";
		return -1;
	}

	// accessing raw pointer 
	for(int i=0;i<640;i++) // height
	{
		for(int j=0;j<640;j++) // width
		{
			uint8_t * pixel = (uint8_t *)ptr + i* addr.stride_y + j*addr.stride_x; 
		}
	}

	vxUnmapImagePathch(oimg, map_id);

	vxReleaseImage(&iimg);
	vxReleaseImage(&oimg);

	vxReleaseGraph(&gra);

	vxReleaseContext(&cont);
}