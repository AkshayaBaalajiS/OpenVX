#include <iostream>
#include <VX/vx.h>
using namespace std;

int main()
{
	cout<<"This code is to understand the vx_context in OpenVX\n";

	vx_status stat = VX_SUCCESS;

	vx_context cont = vxCreateContext();
	if(cont ==  nullptr)
	{
		cout<<"[DEBUG PRINT] : vxCreateContext failed \n";
		return -1;
	}

	vxReleaseContext(&cont);
}