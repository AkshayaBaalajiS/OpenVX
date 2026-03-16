#include <iostream>
#include <VX/vx.h>
using namespace std;

int main()
{
	cout<<"scalar type in OpenVX \n";
	bool boolVariable = false;
	vx_context cont = vxCreateContext();

	vx_scalar scal = vxCreateScalar(cont, VX_TYPE_BOOL, &boolVariable);

	cout<<"[DEBUG PRINT]scal = " << scal <<endl;

	vxReleseScalar(&scal);
	vxReleaseContext(&cont); 

}