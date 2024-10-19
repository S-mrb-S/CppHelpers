
// انجام محاسبات ریاضی سنگین روی GPU
// با OpenCL
#include <iostream>
#include <thread>
#include <CL/cl.h>

#define VECTOR_SIZE 1024

// Kernel code to add two vectors
const char* kernelSource = 
"__kernel void vec_add(__global int* A, __global int* B, __global int* C) { \n"
"    int id = get_global_id(0); \n"
"    C[id] = A[id] + B[id]; \n"
"}\n";

void gpu_computation() {
    // Allocate space for vectors A, B, and C
    int *A = new int[VECTOR_SIZE];
    int *B = new int[VECTOR_SIZE];
    int *C = new int[VECTOR_SIZE];

    // Initialize vectors A and B with some values
    for (int i = 0; i < VECTOR_SIZE; i++) {
        A[i] = i;
        B[i] = VECTOR_SIZE - i;
    }

    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    // Create an OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    // Create memory buffers on the device for each vector
    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE * sizeof(int), NULL, &ret);
    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE * sizeof(int), NULL, &ret);
    cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, VECTOR_SIZE * sizeof(int), NULL, &ret);

    // Copy the lists A and B to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, VECTOR_SIZE * sizeof(int), A, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, VECTOR_SIZE * sizeof(int), B, 0, NULL, NULL);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, NULL, &ret);

    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "vec_add", &ret);

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

    // Execute the OpenCL kernel on the list
    size_t global_item_size = VECTOR_SIZE; // Process the entire list
    size_t local_item_size = 64;           // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

    // Read the memory buffer C on the device to the local variable C
    ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, VECTOR_SIZE * sizeof(int), C, 0, NULL, NULL);

    // Display the result
    std::cout << "Result of vector addition:\n";
    for (int i = 0; i < 10; i++) { // Print the first 10 elements
        std::cout << A[i] << " + " << B[i] << " = " << C[i] << std::endl;
    }

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(b_mem_obj);
    ret = clReleaseMemObject(c_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    delete[] A;
    delete[] B;
    delete[] C;
}

void cpu_task() {
    std::cout << "Immediate Task executed\n";
}

int main() {
    // Create a thread to execute the CPU task
    std::thread cpuThread(cpu_task);

    // Run the GPU computation on the main thread
    gpu_computation();

    // Wait for the CPU thread to finish
    cpuThread.join();

    return 0;
}
