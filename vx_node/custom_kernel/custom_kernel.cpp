#include <iostream>
#include <VX/vx.h>

using namespace std;

vx_status VX_CALLBACK customKernelFunc(vx_node node, const vx_reference * param, vx_uint32 num)
{
	cout<<"-------------------------------\n";
	vx_image iimgg= (vx_image)param[0];
	vx_image oimgg= (vx_image)param[1];
	cout<<"[DEBUG PRINT] inside CUSTOM KERNEL FUNCTION \n";
	cout<<"-------------------------------\n";

	return VX_SUCCESS;
}

// vx_status VX_CALLBACK customKernelValidator(
//     vx_node node,
//     const vx_reference parameters[],
//     vx_uint32 num,
//     vx_meta_format metas[]
// )
// {
//     return VX_SUCCESS;
// }
vx_status VX_CALLBACK customKernelValidator(
    vx_node node,
    const vx_reference parameters[],
    vx_uint32 num,
    vx_meta_format metas[]
)
{
    vx_image input = (vx_image)parameters[0];

    vx_uint32 width;
    vx_uint32 height;
    vx_df_image format;

    vxQueryImage(input, VX_IMAGE_WIDTH, &width, sizeof(width));
    vxQueryImage(input, VX_IMAGE_HEIGHT, &height, sizeof(height));
    vxQueryImage(input, VX_IMAGE_FORMAT, &format, sizeof(format));

    vxSetMetaFormatAttribute(
        metas[1],
        VX_IMAGE_WIDTH,
        &width,
        sizeof(width));

    vxSetMetaFormatAttribute(
        metas[1],
        VX_IMAGE_HEIGHT,
        &height,
        sizeof(height));

    vxSetMetaFormatAttribute(
        metas[1],
        VX_IMAGE_FORMAT,
        &format,
        sizeof(format));

    return VX_SUCCESS;
}

int main()
{
	cout<<"This is the code to understand the custom kernel in OpenVX\n";

	vx_context cont = vxCreateContext();

	vx_graph gra = vxCreateGraph(cont); 

	vx_image iimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);
	vx_image oimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);

	vx_node nodeObj = vxGaussian3x3Node(gra, iimg, oimg);

	// custom Kernel creation 
	// 1. Register Kernel 
	// vx_kernel custKernel = vxAddUserKernel(cont, "user.CustomKernel", 8989, customKernelFunc, 2, NULL, NULL, NULL); // the last 3 arg are validate function, initialize function, deinitialize function 
	vx_kernel custKernel = vxAddUserKernel(cont, "user.CustomKernel", 8989, customKernelFunc, 2, customKernelValidator , NULL, NULL); // the last 3 arg are validate function, initialize function, deinitialize function 
	if(custKernel == nullptr)
	{
	    cout<<"Kernel registration failed\n";
	}
		
	// 2. Add Kernel Parameter 
	vxAddParameterToKernel(custKernel, 0, VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED);
	vxAddParameterToKernel(custKernel, 1, VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED);

	// 3. finalize kernel 
	vxFinalizeKernel(custKernel);

	// 4. Create Node 
	vx_kernel custkernel = vxGetKernelByName(cont, "user.CustomKernel"); 
	vx_node nodeObj1 = vxCreateGenericNode(gra, custkernel);

	// 5. Set param
	vx_image niimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);
	vx_image noimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);
	vxSetParameterByIndex(nodeObj1, 0, (vx_reference)niimg);
	vxSetParameterByIndex(nodeObj1, 1, (vx_reference)noimg);


	vx_status stat = VX_SUCCESS;
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
	else
	{
		cout<<"[DEBUG PRINT] vxProcessGraph failed\n";
		return -1;
	}


	vxReleaseGraph(&gra);

	vxReleaseContext(&cont);
}