#include <iostream>
#include <VX/vx.h>
using namespace std;

int main()
{
	cout<<"This is the program to understand the vx_image in OpenVX\n";

	vx_status stat = VX_SUCCESS;

	vx_context cont = vxCreateContext();
	if(cont == nullptr)
	{
		cout<<"[DEBUG PRINT] vxCreateContext failed \n";
		return -1;
	}
	else
	{
		cout<<"[DEBUG PRINT] vxCreateContext success\n";
	}

	vx_graph gra = vxCreateGraph(cont);
	if(gra != nullptr)
	{
		cout<<"[DEBUG PRINT] vxCreateGraph success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxCreateGraph failed \n";
	}


	// image creation 
	vx_image iimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8); // (context, width, height, format);
	vx_image oimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);
	// vx_image oimg = vxCreateImage(cont, 512, 512, VX_DF_IMAGE_U8); // with wrong object size the verify graph will fail 
	if(iimg!=nullptr || oimg!=nullptr)
	{
		cout<<"[DEBUG PRINT] vxCreateImage success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxCreateImage failed\n";
		return -1;
	}

	// node creation for gaussian 
	vx_node nodeObj = vxGaussian3x3Node(gra, iimg, oimg);
	if(nodeObj != nullptr)
	{
		cout<<"[DEBUG PRINT] vxGaussian3x3Node success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxGaussian3x3Node failed\n";	
	}



	stat =vxVerifyGraph(gra);
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph success\n";
		stat = vxProcessGraph(gra);
	}
	else
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph failed \n";
		return -1;
	}
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxProcessGraph success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxProcessGraph failed \n";
		return -1;
	}









	vxReleaseImage(&iimg);
	vxReleaseImage(&oimg);

	vxReleaseGraph(&gra);

	vxReleaseContext(&cont);
}