#include <iostream>
#include <VX/vx.h>
using namespace std;

int main()
{
	cout<<"vxCopyScalar in OpenVX \n";
	vx_context cont = vxCreateContext();

	vx_float32 f32 = 32.23432f;

	vx_scalar scal = vxCreateScalar(cont, VX_TYPE_FLOAT32, &f32);

	vx_float32 f32_1;
	
	cout<<"[DEBUG PRINT] f32_1 = " << f32_1 <<endl;
	vxCopyScalar(scal, &f32_1, VX_READ_ONLY, VX_MEMORY_TYPE_HOST ); 
	cout<<"[DEBUG PRINT] after vxCopyScalar f32_1 = " << f32_1 <<endl;

	vxReleaseScalar(&scal);
	vxReleaseContext(&cont);
}