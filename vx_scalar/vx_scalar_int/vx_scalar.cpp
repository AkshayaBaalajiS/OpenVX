#include <iostream>
#include <VX/vx.h>

using namespace std;

int main()
{
	cout<<"vx_Scalar creation in OpenVX\n";
	vx_context cont = vxCreateContext();

	int number = 123;
	vx_scalar sca = vxCreateScalar(cont, VX_TYPE_INT32, &number);

	cout<<"[DEBUG PRINT] sca = " << sca <<endl;

	vxReleaseScalar(&sca);

	vxReleaseContext(&cont);
}