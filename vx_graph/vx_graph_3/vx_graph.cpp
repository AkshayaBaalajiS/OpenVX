#include <iostream>
#include <VX/vx.h>
using namespace std;

int main()
{
	cout<<"This is the code to understand the vx_graph in OpenVX\n";

	vx_status stat = VX_SUCCESS;

	// context creation 
	// vx_context cont = vxCreateContext() ; // this is initialization of the context 
	vx_context cont;
	// we can also assign a context 
	cont = vxCreateContext();

	if(cont == nullptr)
	{	
		cout<<"[DEBUG PRINT] vxCreateContext failed\n";
		return -1;
	}
	
	if(cont != nullptr)
	{	
		cout<<"[DEBUG PRINT] vxCreateContext success..\n";
	}
	
	

	// graph creation 
	vx_graph gra = vxCreateGraph(cont);

	if(gra == nullptr)
	{	
		cout<<"[DEBUG PRINT] vxCreateGraph failed\n";
		return -1;
	}
	// if(gra == VX_SUCCESS) // this cant be done 
	if(gra != nullptr)
	{	
		cout<<"[DEBUG PRINT] vxCreateGraph success\n";
	}

	// data object 
	vx_image inputImg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);
	vx_image OpImg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);
	if(inputImg != nullptr || OpImg != nullptr )
	{
		cout<<"[DEBUG PRINT] vxCreateImage success for input and output \n";
	}
	else
	{
		cout<<"[DEBIG PRINT] vxCreateImage failed for input and output \n";
	}


	// node creation 
	vx_node nodeobj = vxGaussian3x3Node(gra, inputImg, OpImg);
	if(nodeobj != nullptr)
	{
		cout<<"[DEBUG PRINT] vxGaussian3x3Node success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxGaussian3x3Node failed\n";
	}
	


	// verify graph
	stat = vxVerifyGraph(gra);
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph failed\n";
		return -1;	
	}	

	// process graph 
	stat = vxProcessGraph(gra);
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxProcessGraph success\n";
	}
	else{
		cout<<"[DEBUG PRINT] vxProcessGraph failed\n";
	}
	

	vxReleaseGraph(&gra);

	vxReleaseContext(&cont);
}