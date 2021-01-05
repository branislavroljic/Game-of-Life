#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <CL/cl.h>
#include <string>
#include <iostream>



const char* TranslateOpenCLError(cl_int errorCode)
{
	switch (errorCode)
	{
	case CL_SUCCESS:                            return "CL_SUCCESS";
	case CL_DEVICE_NOT_FOUND:                   return "CL_DEVICE_NOT_FOUND";
	case CL_DEVICE_NOT_AVAILABLE:               return "CL_DEVICE_NOT_AVAILABLE";
	case CL_COMPILER_NOT_AVAILABLE:             return "CL_COMPILER_NOT_AVAILABLE";
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:      return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case CL_OUT_OF_RESOURCES:                   return "CL_OUT_OF_RESOURCES";
	case CL_OUT_OF_HOST_MEMORY:                 return "CL_OUT_OF_HOST_MEMORY";
	case CL_PROFILING_INFO_NOT_AVAILABLE:       return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case CL_MEM_COPY_OVERLAP:                   return "CL_MEM_COPY_OVERLAP";
	case CL_IMAGE_FORMAT_MISMATCH:              return "CL_IMAGE_FORMAT_MISMATCH";
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:         return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case CL_BUILD_PROGRAM_FAILURE:              return "CL_BUILD_PROGRAM_FAILURE";
	case CL_MAP_FAILURE:                        return "CL_MAP_FAILURE";
	case CL_MISALIGNED_SUB_BUFFER_OFFSET:       return "CL_MISALIGNED_SUB_BUFFER_OFFSET";                          //-13
	case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:    return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";   //-14
	case CL_COMPILE_PROGRAM_FAILURE:            return "CL_COMPILE_PROGRAM_FAILURE";                               //-15
	case CL_LINKER_NOT_AVAILABLE:               return "CL_LINKER_NOT_AVAILABLE";                                  //-16
	case CL_LINK_PROGRAM_FAILURE:               return "CL_LINK_PROGRAM_FAILURE";                                  //-17
	case CL_DEVICE_PARTITION_FAILED:            return "CL_DEVICE_PARTITION_FAILED";                               //-18
	case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:      return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";                         //-19
	case CL_INVALID_VALUE:                      return "CL_INVALID_VALUE";
	case CL_INVALID_DEVICE_TYPE:                return "CL_INVALID_DEVICE_TYPE";
	case CL_INVALID_PLATFORM:                   return "CL_INVALID_PLATFORM";
	case CL_INVALID_DEVICE:                     return "CL_INVALID_DEVICE";
	case CL_INVALID_CONTEXT:                    return "CL_INVALID_CONTEXT";
	case CL_INVALID_QUEUE_PROPERTIES:           return "CL_INVALID_QUEUE_PROPERTIES";
	case CL_INVALID_COMMAND_QUEUE:              return "CL_INVALID_COMMAND_QUEUE";
	case CL_INVALID_HOST_PTR:                   return "CL_INVALID_HOST_PTR";
	case CL_INVALID_MEM_OBJECT:                 return "CL_INVALID_MEM_OBJECT";
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:    return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case CL_INVALID_IMAGE_SIZE:                 return "CL_INVALID_IMAGE_SIZE";
	case CL_INVALID_SAMPLER:                    return "CL_INVALID_SAMPLER";
	case CL_INVALID_BINARY:                     return "CL_INVALID_BINARY";
	case CL_INVALID_BUILD_OPTIONS:              return "CL_INVALID_BUILD_OPTIONS";
	case CL_INVALID_PROGRAM:                    return "CL_INVALID_PROGRAM";
	case CL_INVALID_PROGRAM_EXECUTABLE:         return "CL_INVALID_PROGRAM_EXECUTABLE";
	case CL_INVALID_KERNEL_NAME:                return "CL_INVALID_KERNEL_NAME";
	case CL_INVALID_KERNEL_DEFINITION:          return "CL_INVALID_KERNEL_DEFINITION";
	case CL_INVALID_KERNEL:                     return "CL_INVALID_KERNEL";
	case CL_INVALID_ARG_INDEX:                  return "CL_INVALID_ARG_INDEX";
	case CL_INVALID_ARG_VALUE:                  return "CL_INVALID_ARG_VALUE";
	case CL_INVALID_ARG_SIZE:                   return "CL_INVALID_ARG_SIZE";
	case CL_INVALID_KERNEL_ARGS:                return "CL_INVALID_KERNEL_ARGS";
	case CL_INVALID_WORK_DIMENSION:             return "CL_INVALID_WORK_DIMENSION";
	case CL_INVALID_WORK_GROUP_SIZE:            return "CL_INVALID_WORK_GROUP_SIZE";
	case CL_INVALID_WORK_ITEM_SIZE:             return "CL_INVALID_WORK_ITEM_SIZE";
	case CL_INVALID_GLOBAL_OFFSET:              return "CL_INVALID_GLOBAL_OFFSET";
	case CL_INVALID_EVENT_WAIT_LIST:            return "CL_INVALID_EVENT_WAIT_LIST";
	case CL_INVALID_EVENT:                      return "CL_INVALID_EVENT";
	case CL_INVALID_OPERATION:                  return "CL_INVALID_OPERATION";
	case CL_INVALID_GL_OBJECT:                  return "CL_INVALID_GL_OBJECT";
	case CL_INVALID_BUFFER_SIZE:                return "CL_INVALID_BUFFER_SIZE";
	case CL_INVALID_MIP_LEVEL:                  return "CL_INVALID_MIP_LEVEL";
	case CL_INVALID_GLOBAL_WORK_SIZE:           return "CL_INVALID_GLOBAL_WORK_SIZE";                           //-63
	case CL_INVALID_PROPERTY:                   return "CL_INVALID_PROPERTY";                                   //-64
	case CL_INVALID_IMAGE_DESCRIPTOR:           return "CL_INVALID_IMAGE_DESCRIPTOR";                           //-65
	case CL_INVALID_COMPILER_OPTIONS:           return "CL_INVALID_COMPILER_OPTIONS";                           //-66
	case CL_INVALID_LINKER_OPTIONS:             return "CL_INVALID_LINKER_OPTIONS";                             //-67
	case CL_INVALID_DEVICE_PARTITION_COUNT:     return "CL_INVALID_DEVICE_PARTITION_COUNT";                     //-68
																												//    case CL_INVALID_PIPE_SIZE:                  return "CL_INVALID_PIPE_SIZE";                                  //-69
																												//    case CL_INVALID_DEVICE_QUEUE:               return "CL_INVALID_DEVICE_QUEUE";                               //-70    

	default:
		return "UNKNOWN ERROR CODE";
	}
}

static char* readKernelSource(const char* filename) {

	char* kernelSource = nullptr;
	long length;
	FILE* file = fopen(filename, "r");
	if (file) {
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, 0, SEEK_SET);
		kernelSource = (char*)calloc(length, sizeof(char));
		if (kernelSource) {
			fread(kernelSource, sizeof(char), length, file);
		}
		fclose(file);
	}
	return kernelSource;
}

void convertMatrix(unsigned char* matrix, unsigned int length) {
	for (size_t i = 0; i < length; i++)
	{
		if (matrix[i] == 1)
			matrix[i] = 255;
	}
}

static void writeImage(const char* filename, const unsigned char* array, const int width, const int height) {
	
	FILE* file = fopen(filename, "wb");
	fprintf(file, "P5\n%d %d\n255\n", width, height);
	fwrite(array, sizeof(unsigned char), (size_t)width * (size_t)height, file);
	fclose(file);
}

static void readImage(const char* filename, unsigned char*& array, int& width, int& height) {
	FILE* file = fopen(filename, "rb");

	if (!fscanf(file, "P5\n%d %d\n255\n", &width, &height)) {
		throw "error";
	}
	FILE* fileptr;
	char* buffer;
	long filelen;

	//fileptr = fopen("myfile.txt", "rb");  // Open the file in binary mode
	//fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	//filelen = ftell(fileptr);             // Get the current byte offset in the file
	//rewind(fileptr);                      // Jump back to the beginning of the file

	//buffer = (char*)malloc(filelen * sizeof(char)); // Enough memory for the file
	//fread(buffer, filelen, 1, fileptr); // Read in the entire file
	//fclose(fileptr); // Close the file
	//fseek(file, 0, SEEK_END);
	//int filelen = ftell(file);
	//rewind(fileptr);

	unsigned char* image = (unsigned char*)calloc((size_t)width * height, sizeof(unsigned char));
	//printf("%d   %d", sizeof(image), fgetc(file));
	/*do {
		char c = fgetc(file);
		if (feof(file)) {
			break;
		}
		printf("%c", c);
	} while (1);*/
	fread(image, sizeof(unsigned char), (size_t)width * (size_t)height, file);
	fclose(file);
	printf("%d", sizeof(image));
	for (char i = 0; i < sizeof(image)/sizeof(image[0]); i++)
	{
		printf("%c", image[i]);
	}
	const std::string outFile = std::string("imageBane") + std::string(".pgm");
	writeImage(outFile.c_str(), image, width, height);
	array = image;
}

unsigned char* initMatrix(size_t* , size_t* );
unsigned char* initMatrixWithImage(size_t* , size_t* );
unsigned char* compressMatrix(unsigned char* matrix, const unsigned int width, const unsigned int height);

unsigned char* convertBinaryToBlackAndWhiteMatrix(unsigned char* matrix, unsigned int length) {

	unsigned char* retMatrix = (unsigned char*)malloc(length);
	for (size_t i = 0; i < length; i++)
	{
		retMatrix[i] = matrix[i] == 0 ? 0 : 255;
	}
	return retMatrix;
}

int main() {
	int width = -1;
	int height = -1;
	//unsigned char* buffer = nullptr;
	
	// Number of total work items - localSize must be devisor
	
	/*globalSize[0] = (size_t)ceil(sqrt(globalSizePerMatrix));
	globalSize[1] = (size_t)ceil(sqrt(globalSizePerMatrix));*/

	cl_mem d_InMatrix;
	cl_mem d_OutMatrix;

	cl_platform_id cpPlatform;        // OpenCL platform
	cl_device_id device_id;           // device ID
	cl_context context;               // context
	cl_command_queue queue;           // command queue
	cl_program program;               // program
	cl_kernel kernel;                 // kernel

	cl_int err;


	//unsigned char* matrix = nullptr;

	// Number of work items in each local work group


	//matrix = (unsigned char*)malloc(globalSize[0] * globalSize[1]);
	// Bind to platform
	err = clGetPlatformIDs(1, &cpPlatform, NULL);

	// Get ID for the device
	err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

	unsigned long deviceGlobalMemSize;
	clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(deviceGlobalMemSize), &deviceGlobalMemSize, NULL);

	printf("\tCL_DEVICE_GLOBAL_MEM_SIZE : %lu\n", deviceGlobalMemSize);

	// Create a context  
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

	// Create a command queue 
	//cl_queue_properties prop[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_ON_DEVICE | CL_QUEUE_ON_DEVICE_DEFAULT,  CL_QUEUE_SIZE, 16, 0 };
	queue = clCreateCommandQueue(context, device_id, 0, &err);

	char* kernelSource = readKernelSource("GameOfLife.cl");

	// Create the compute program from the source buffer
	program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &err);

	// Build the program executable 
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	

	if (err)
	{
		// Determine the size of the log
		size_t log_size;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
		char* log = (char*)malloc(log_size);

		// Get the log
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
		printf("%s\n", log);

		free(log);
	}

	int imageWidth, imageHeight;
	unsigned char* imageBuffer = nullptr;
	readImage("image.pgm", imageBuffer, imageWidth, imageHeight);

	writeImage("slika.pgm", imageBuffer, imageWidth, imageHeight);

	size_t globalSize[2], localSize[2];
	localSize[0] = localSize[1] = 16;

	int maxGlobalSize = deviceGlobalMemSize;
	printf("max global size %d\n", maxGlobalSize);
	int globalSizePerMatrix = maxGlobalSize / 10;

	globalSize[0] = (size_t)ceil((size_t)((1000) / (float)localSize[0])) * localSize[0];
	globalSize[1] = (size_t)ceil((size_t)((1000) / (float)localSize[1])) * localSize[1];
	printf("global size %d\n", globalSize[0]);

	int imageSize = globalSize[0] * globalSize[1];

	unsigned char* matrix = nullptr;
	matrix = initMatrix(globalSize, localSize);
	/*for (size_t i = 0; i < globalSize[0] * globalSize[1]; i++)
	{
		printf("%d ", matrix[i]);
	}*/

	

	/*for (size_t i = 0; i < globalSize[0] * globalSize[1] / 8; i++)
	{
		printf("%d ", compressedMatrix[i]);
	}
	*/
	/*printf("velicina obicne: %d ", sizeof(matrix) / sizeof(matrix[0]));
	printf("velicina compressed: %d ", sizeof(compressedMatrix) / sizeof(compressedMatrix[0]));*/

	const std::string outFile = std::string("matrica") + std::string(".pgm");

	writeImage(outFile.c_str(), matrix, globalSize[0], globalSize[1]);


	d_InMatrix = clCreateBuffer(context, CL_MEM_READ_WRITE, imageSize, NULL, NULL);

	d_OutMatrix = clCreateBuffer(context, CL_MEM_READ_WRITE, imageSize, NULL, NULL);

	//Write out data set into the input array in device memory
	err = clEnqueueWriteBuffer(queue, d_InMatrix, CL_TRUE, 0, imageSize, matrix, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(queue, d_OutMatrix, CL_TRUE, 0, imageSize, matrix, 0, NULL, NULL);

	clFinish(queue);

	unsigned int xStart, yStart;

	std::cout << "Unesite koordinate : " << std::endl;
	//std::cin >> xStart >> yStart;
	scanf("%d", &xStart);
	scanf("%d", &yStart);
	/*readImage("C:\\Users\\PC\\source\\repos\\TreciZadatak\\OposZadaci2020\\image0.pgm", matrix, width, height);
	int imageSize = width * height;*/

	//std::cout << "Unijeli ste koordinate :" << xStart + " " + yStart << std::endl;
	printf("unijeli ste koordinate: %d  %d\n", xStart, yStart);
	kernel = clCreateKernel(program, "initMatrixWithImage", &err);

	printf(TranslateOpenCLError(err));

	
	cl_mem d_image;

	d_image = clCreateBuffer(context, CL_MEM_READ_WRITE, static_cast<size_t>(imageWidth) * imageHeight, NULL, NULL);
	err = clEnqueueWriteBuffer(queue, d_image, CL_TRUE, 0, static_cast<size_t>(imageWidth) * imageHeight, imageBuffer, 0, NULL, NULL);

	printf(TranslateOpenCLError(err));
	unsigned int widthBoundary = xStart + imageWidth > globalSize[0] ? globalSize[0] : ((size_t)xStart + imageWidth);
	unsigned int heightBoundary =yStart + imageHeight > globalSize[1] ? globalSize[1] : ((size_t)yStart + imageHeight);

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_InMatrix);
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_image);
	err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &xStart);
	err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &yStart);
	err |= clSetKernelArg(kernel, 4, sizeof(unsigned int), &widthBoundary);
	err |= clSetKernelArg(kernel, 5, sizeof(unsigned int), &heightBoundary);
	err |= clSetKernelArg(kernel, 6, sizeof(unsigned int), &globalSize[0]);
	err |= clSetKernelArg(kernel, 7, sizeof(unsigned int), &imageWidth);
	// Execute the kernel over the entire range of the data set  
	err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);

	printf(TranslateOpenCLError(err));
	// Wait for the command queue to get serviced before reading back results
	clFinish(queue);


	clEnqueueReadBuffer(queue, d_InMatrix, CL_TRUE, 0, imageSize, matrix, 0, NULL, NULL);

	clFinish(queue);
	
	const std::string outFileWithImage = std::string("matricaSaSlikom") +  std::string(".pgm");
	writeImage(outFileWithImage.c_str(), convertBinaryToBlackAndWhiteMatrix(matrix, globalSize[0]*globalSize[1]), globalSize[0], globalSize[1]);

	clReleaseMemObject(d_InMatrix);
	clReleaseMemObject(d_OutMatrix);
	clReleaseMemObject(d_image);



	clFinish(queue);

	unsigned char* compressedMatrix = nullptr;
	//printf("kopresovana-------------------------------------------------");
	compressedMatrix = compressMatrix(matrix, globalSize[0], globalSize[1]);
	unsigned int compressedImageSize = imageSize / 8;

	err = clEnqueueWriteBuffer(queue, d_InMatrix, CL_TRUE, 0, compressedImageSize, matrix, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(queue, d_OutMatrix, CL_TRUE, 0, compressedImageSize, matrix, 0, NULL, NULL);

//	int imageSize = globalSize[0] * globalSize[1];
	
	// Create the compute kernel in the program we wish to run
	kernel = clCreateKernel(program, "gameOfLife", &err);
	/*for (size_t i = 0; i < globalSize[0] * globalSize[1]; i++)
	{
		printf("%d ", matrix[i]);
	}*/
	//
	//writeImage("prviFajl.pgm", matrix, globalSize[0], globalSize[1]);
	// Create the input and output arrays in device memory for our calculation
	

	for (int i = 0; i < 30; i++)
	{
		// Set the arguments to our compute kernel
		err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_InMatrix);
		err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_OutMatrix);
		err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &globalSize[0]);
		// Execute the kernel over the entire range of the data set  
		err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);
		
		printf(TranslateOpenCLError(err));
		// Wait for the command queue to get serviced before reading back results
		clFinish(queue);

		// Read the results from the device
		clEnqueueReadBuffer(queue, d_OutMatrix, CL_TRUE, 0, imageSize, matrix, 0, NULL, NULL);
		 
		clFinish(queue);
		err = clEnqueueWriteBuffer(queue, d_InMatrix, CL_TRUE, 0, imageSize, matrix, 0, NULL, NULL);
		clFinish(queue);
		const std::string outFile = std::string("image") + std::to_string(i + 1) + std::string(".pgm");
		writeImage(outFile.c_str(), convertBinaryToBlackAndWhiteMatrix(matrix, globalSize[0] * globalSize[1]), globalSize[0], globalSize[1]);
	}

	// release OpenCL resources
	clReleaseMemObject(d_InMatrix);
	clReleaseMemObject(d_OutMatrix);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	free(kernelSource);
}


unsigned char ToByte(unsigned char b[8])
{
	unsigned char c = 0;
	for (int i = 0; i < 8; ++i)
		if (b[i])
			c |= 1 << i;
	return c;
}

void FromByte(unsigned char c, unsigned char b[8])
{
	for (int i = 0; i < 8; ++i)
		b[i] = (c & (1 << i)) != 0;
}

unsigned char* compressMatrix(unsigned char* matrix, const unsigned int width, const unsigned int height) {

	unsigned char* compMat = (unsigned char*)malloc(width * height / 8);
	unsigned char* eightBitArr = (unsigned char*)malloc(8);
	for (size_t i = 0, j = 0; i < width*height; i+=8, j++)
	{
		memcpy(eightBitArr, &matrix[i], 8 * sizeof(*matrix));
		compMat[j] = ToByte(eightBitArr);
	}
	return compMat;
}

 unsigned char* initMatrix(size_t *globalSize, size_t *localSize) {


	 cl_mem d_matrix;

	 cl_mem d_a;
	 unsigned char* matrix = nullptr;

	 cl_platform_id cpPlatform;        // OpenCL platform
	 cl_device_id device_id;           // device ID
	 cl_context context;               // context
	 cl_command_queue queue;           // command queue
	 cl_program program;               // program
	 cl_kernel kernel;                 // kernel



	 cl_int err;
	 // Number of work items in each local work group


	 matrix = (unsigned char*)malloc(globalSize[0] * globalSize[1]);
	 // Bind to platform
	 err = clGetPlatformIDs(1, &cpPlatform, NULL);

	 // Get ID for the device
	 err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

	 // Create a context  
	 context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

	 // Create a command queue 
	 //cl_queue_properties prop[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_ON_DEVICE | CL_QUEUE_ON_DEVICE_DEFAULT,  CL_QUEUE_SIZE, 16, 0 };
	 queue = clCreateCommandQueue(context, device_id, 0, &err);

	 char* kernelSource = readKernelSource("GameOfLife.cl");

	 // Create the compute program from the source buffer
	 program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &err);

	 // Build the program executable 
	 err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	 if (err)
	 {
		 // Determine the size of the log
		 size_t log_size;
		 clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		 // Allocate memory for the log
		 char* log = (char*)malloc(log_size);

		 // Get the log
		 clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		 // Print the log
		 printf("%s\n", log);

		 free(log);
	 }
	 // Create the compute kernel in the program we wish to run
	 kernel = clCreateKernel(program, "initMatrix", &err);

	 // Create the input and output arrays in device memory for our calculation
	 d_matrix = clCreateBuffer(context, CL_MEM_READ_WRITE, globalSize[0] * globalSize[1], NULL, NULL);
	 //printf("%s", TranslateOpenCLError(err));
	 //Write out data set into the input array in device memory
	 //err = clEnqueueWriteBuffer(queue, d_a, CL_TRUE, 0, imageSize, buffer, 0, NULL, NULL);

	 // Set the arguments to our compute kernel
	 err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_matrix);
	 err |= clSetKernelArg(kernel, 1, sizeof(unsigned int), &globalSize[0]);
	 err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &globalSize[1]);

	 //printf("%s", TranslateOpenCLError(err));
	 // Execute the kernel over the entire range of the data set  
	 err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);
	 printf("%s", TranslateOpenCLError(err));
	 clFinish(queue);

	 clEnqueueReadBuffer(queue, d_matrix, CL_TRUE, 0, globalSize[0] * globalSize[1], matrix, 0, NULL, NULL);
	 //printf("%s", TranslateOpenCLError(err));
	 clFinish(queue);

	 /*for (size_t i = 0; i < globalSize[0]*globalSize[1]; i++)
	 {
		 printf("%d ", matrix[i]
		 );
	 }*/

	 // release OpenCL resources
	 clReleaseMemObject(d_matrix);
	 clReleaseProgram(program);
	 clReleaseKernel(kernel);
	 clReleaseCommandQueue(queue);
	 clReleaseContext(context);

	 return matrix;

}

