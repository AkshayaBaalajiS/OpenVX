#include <iostream>
#include <VX/vx.h>
using namespace std;

int main()
{
	cout<<"This is the code to understand the vx_graph in OpenVX\n";

	vx_status stat = VX_SUCCESS;

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

	stat = vxProcessGraph(gra);
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxProcessGraph success\n";
	}
	else{
		cout<<"[DEBUG PRINT] vxProcessGraph failed\n";
	}
	

	vxReleaseContext(&cont);
}

/*
This is the code to understand the graph


Note:
	A graph without node will fail in vxVerifyGraph(gra_Obj);
*/
