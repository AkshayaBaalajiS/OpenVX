#include <iostream>
#include <VX/vx.h>

using namespace std;

int main()
{
	cout<<"vx_context creation in cpp\n";
	vx_context cont = vxCreateContext();
	if(cont == nullptr)
	{
		cout<<"Context creation failed \n";
		return -1;	
	}
	else{
		cout<<"Context created\n";
	}

	vxReleaseContext(&cont);

}