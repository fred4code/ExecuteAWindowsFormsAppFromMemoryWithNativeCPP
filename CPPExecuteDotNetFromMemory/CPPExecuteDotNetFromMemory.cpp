/*
This .cpp file is part of the Visual Studio Project CPPExecuteDotNetFromMemory
CPPExecuteDotNetFromMemory Visual Studio Project project is part of the Visual Studio Solution ExecuteAWindowsFormsAppFromMemoryWithNativeCPP
This.cs file, the project mentionedand the Visual studio solution mentioned are licensed under the MIT License.See http ://opensource.org/licenses/MIT for more information.

Copyright(c) 2023 fred4code
*/

// Include the <windows.h> header file to gain access to a wide variety of Windows-specific functions, types, and macros.
// This is necessary for interacting with many core features of the Windows API (Application Programming Interface),
// including components used in this program such as shared memory, COM (Component Object Model), and the .NET runtime.
#include <windows.h>

// Include the standard input/output stream library which is used for basic input/output operations.
// It provides objects like cin, cout and cerr to interact with standard input and output devices.
#include <iostream>

// Include the <thread> header file to enable this program to use the std::thread class.
// The std::thread class is a part of the Standard Library and allows for the creation and management of separate threads in the program.
#include <thread>

// Include the file stream library. It provides objects like ifstream (for reading from files),
// ofstream (for writing to files) and fstream (for both reading and writing).
#include <fstream>

// Include the vector library which provides a dynamic array functionality. Vectors use contiguous
// storage locations for their elements, which means that their elements can also be accessed using offsets 
// on regular pointers to its elements, and just as efficiently as in arrays.
#include <vector>

// Include the string stream library. This provides an interface to manipulate strings as input/output streams.
// String streams allow easy parsing, reading and writing of different types to/from strings.
#include <sstream>

// Include the input/output manipulators library. It provides functions to manipulate the output of C++ programs.
// For example, you can set the width of fields, set the precision of floating point numbers, set fill characters, etc.
#include <iomanip>

// This is a header that defines several general purpose functions and macros that greatly ease the
// use of COM. COM is a component object model by Microsoft that allows components to communicate
// across process and machine boundaries.
#include <comdef.h>

// Include the definitions of OLE Automation Interfaces.
// OLE Automation is a technology that allows you to automate the functionality exposed by an application.
#include <OAIdl.h>

// Include the header that defines hosting interfaces for the .NET runtime.
// This allows a native application to load the .NET runtime into its process and execute .NET code.
#include <metahost.h>

// This is a linker directive that is saying "please add mscoree.lib to the list of libraries
// that will be linked into the final executable". mscoree.lib is the core library for .NET interop.
#pragma comment(lib, "mscoree.lib")

// The #import directive is used to incorporate information from a type library.
// The type library used here is mscorlib.tlb which is the primary interop assembly for .NET
// (the assembly which contains all the basic .NET types).
// The additional parameters are modifying the behavior of the import.
#import "mscorlib.tlb" raw_interfaces_only high_property_prefixes("_get","_put","_putref") rename("ReportEvent", "InteropServices_ReportEvent") rename("or", "mscorlib_or")

// This brings everything in the namespace mscorlib into scope. This is the namespace that contains fundamental
// .NET types like System.Object.
using namespace mscorlib;


// Function that reads a hex string from a file and returns it as a vector of bytes
std::vector<unsigned char> readHexStringFromFile(const std::string& filePath) {

    // Open the input file stream with the given file path
    std::ifstream inFile(filePath);

    // Check if the file was successfully opened
    if (!inFile) {
        // If not, print an error message to standard error and return an empty vector
        std::cerr << "Failed to open file " << filePath << std::endl;
        return std::vector<unsigned char>();
    }

    // Declare a string to hold the hex string read from the file
    std::string hexString;

    // Read the file content into the hex string
    inFile >> hexString;

    // Declare a vector to hold the bytes
    std::vector<unsigned char> bytes;

    // Loop over the hex string, two characters at a time
    for (unsigned int i = 0; i < hexString.length(); i += 2) {
        // Extract two characters from the hex string
        std::string byteString = hexString.substr(i, 2);

        // Convert the two characters to a byte using strtol
        // The third argument to strtol is the base of the number to convert. In this case, 16 for hexadecimal.
        unsigned char byte = (unsigned char)strtol(byteString.c_str(), NULL, 16);

        // Add the byte to the vector of bytes
        bytes.push_back(byte);
    }

    // Return the vector of bytes
    return bytes;
}


std::string getExecutablePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

// Define a preprocessor macro named SHARED_MEM_SIZE and set its value to 1. 
// This macro can now be used throughout the code as if it's a constant.
#define SHARED_MEM_SIZE 1

// Function to initialize and manage shared memory communication with a .NET form.
void runSharedMemoryServer()
{
    // Create a file mapping object (shared memory) in the system's paging file.
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // Use system paging file
        NULL,                    // Default security settings
        PAGE_READWRITE,          // Read/write access to the mapped view of the file
        0,                       // High-order DWORD of file size
        SHARED_MEM_SIZE,         // Low-order DWORD of file size
        L"MySharedMemory");      // Name of the file mapping object

    // Check if the shared memory was created successfully.
    if (hMapFile == NULL)
    {
        std::cout << "Error while creating shared memory.\n";
        return;
    }

    // Map the shared memory into the process's address space.
    char* pBuf = (char*)MapViewOfFile(
        hMapFile,                // Handle to the file mapping object
        FILE_MAP_ALL_ACCESS,     // Read/write access
        0,                       // High-order DWORD of the file offset 
        0,                       // Low-order DWORD of the file offset
        SHARED_MEM_SIZE);        // Number of bytes to map

    // Check if the shared memory was mapped successfully.
    if (pBuf == NULL)
    {
        std::cout << "Error while mapping shared memory.\n";
        CloseHandle(hMapFile);   // Close the handle to the file mapping object
        return;
    }

    // Initialize the shared memory to '1'. This could signify that the .NET form is open.
    pBuf[0] = '1';

    // Monitor the shared memory as long as the value is '1'.
    while (pBuf[0] == '1')
    {
        Sleep(1000); // Wait 1 second between checks
    }

    std::cout << "The .NET form has been closed. Terminating the process...\n";

    // Clean up: Unmap the shared memory and close the handle to the file mapping object.
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    // Terminate the process.
    ExitProcess(0);
}

int main()
{
    //1)
    // Start a new thread dedicated to running the shared memory server.
    // The runSharedMemoryServer function will be executed in a new thread.
    // This allows the main thread to perform other tasks while the server is running.
    std::thread serverThread(runSharedMemoryServer);

    // Define an HRESULT variable. HRESULT is a data type used in Windows programming, 
    // where it is used to represent error codes, warning codes, and status codes from functions.
    HRESULT hr;

    // Initialize the Component Object Model (COM) on the current thread.
    // COM is a binary-interface standard that allows objects to interact across process and machine boundaries.
    // CoInitialize initializes the COM library for use by the calling thread, 
    // sets the thread's concurrency model, and creates a new apartment for the thread if one does not already exist.
    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        // If the initialization fails, print an error message to the console and terminate the program with a return value of -1.
        std::cout << "Error during COM initialization\n";
        return -1;
    }

    // Create an instance of the .NET runtime host. 
    // This is done by calling CLRCreateInstance with the CLSID_CLRMetaHost class identifier and the IID_PPV_ARGS helper macro.
    // The .NET runtime host is an unmanaged code host that begins and manages the execution of .NET code.
    // It is responsible for loading the Common Language Runtime (CLR) into a process and creating AppDomains within which assemblies are executed.
    ICLRMetaHost* pMetaHost = NULL;
    hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
    if (FAILED(hr))
    {
        // If the .NET runtime host creation fails, print an error message to the console, uninitialize COM, and terminate the program with a return value of -1.
        std::cout << "Error during .NET runtime creation\n";
        CoUninitialize();
        return -1;
    }

    // Declare a pointer to an ICLRRuntimeInfo interface. This interface provides methods for retrieving information 
    // about a specific version of the common language runtime (CLR) on a computer.
    ICLRRuntimeInfo* pRuntimeInfo = NULL;
    // Obtain the runtime information for .NET version v4.0.30319 by calling the GetRuntime method on the ICLRMetaHost interface.
    // This method retrieves a pointer to an ICLRRuntimeInfo object, which provides methods to query and manipulate a CLR version.
    hr = pMetaHost->GetRuntime(L"v4.0.30319", IID_PPV_ARGS(&pRuntimeInfo));
    if (FAILED(hr))
    {
        // If the attempt to get runtime information fails, print an error message to the console, release the MetaHost COM object,
        // uninitialize COM to clean up the resources it has allocated, and terminate the program with a return value of -1.
        std::cout << "Error during getting .NET runtime info\n";
        pMetaHost->Release();
        CoUninitialize();
        return -1;
    }

    // Declare a pointer to an ICorRuntimeHost interface. This interface provides methods to start and stop the .NET runtime, 
    // create application domains, execute assemblies, and perform other tasks that manage the runtime.
    // Note that ICorRuntimeHost is used here, not ICLRRuntimeHost.
    ICorRuntimeHost* pCorRuntimeHost = NULL;
    // Obtain a pointer to the ICorRuntimeHost interface for the CLR version by calling the GetInterface method on the ICLRRuntimeInfo interface.
    hr = pRuntimeInfo->GetInterface(CLSID_CorRuntimeHost, IID_ICorRuntimeHost, (void**)&pCorRuntimeHost);
    if (FAILED(hr))
    {
        // If the attempt to get the .NET runtime host interface fails, print an error message to the console, release the RuntimeInfo 
        // and MetaHost COM objects, uninitialize COM to clean up the resources it has allocated, and terminate the program with a return value of -1.
        std::cout << "Error during getting .NET runtime interface\n";
        pRuntimeInfo->Release();
        pMetaHost->Release();
        CoUninitialize();
        return -1;
    }

    // Release the MetaHost and RuntimeInfo COM objects. The ICLRMetaHost and ICLRRuntimeInfo interfaces are no longer needed 
    // as we have obtained the necessary ICorRuntimeHost interface, so we can safely release these COM objects to free up their resources.
    pMetaHost->Release();
    pRuntimeInfo->Release();

    // Initiate the .NET runtime using the Start method on the ICorRuntimeHost interface.
    // Starting the .NET runtime will make the CLR ready to execute managed code.
    hr = pCorRuntimeHost->Start();
    if (FAILED(hr))
    {
        // If the .NET runtime fails to start, print an error message, release the RuntimeHost COM object,
        // uninitialize COM to clean up the resources it has allocated, and terminate the program with a return value of -1.
        std::cout << "Error during starting the .NET runtime\n";
        pCorRuntimeHost->Release();
        CoUninitialize();
        return -1;
    }

    // Declare a pointer to the IUnknown interface. This interface is used to work with COM objects in a generic way.
    // The GetDefaultDomain method of ICorRuntimeHost is called to get a pointer to the default application domain in the runtime.
    IUnknown* pAppDomainPunk = NULL;
    hr = pCorRuntimeHost->GetDefaultDomain(&pAppDomainPunk);
    if (FAILED(hr))
    {
        // If the attempt to get the default AppDomain fails, print an error message, release the RuntimeHost COM object,
        // uninitialize COM to clean up the resources it has allocated, and terminate the program with a return value of -1.
        std::cout << "Error during getting the default AppDomain\n";
        pCorRuntimeHost->Release();
        CoUninitialize();
        return -1;
    }

    // Declare a smart pointer to the _AppDomain interface. 
    // This interface provides methods for creating types and instances in the application domain.
    _AppDomainPtr spDefaultAppDomain = NULL;
    // Use the QueryInterface method on the IUnknown interface to get a pointer to the _AppDomain interface.
    hr = pAppDomainPunk->QueryInterface(__uuidof(_AppDomain), reinterpret_cast<void**>(&spDefaultAppDomain));
    if (FAILED(hr))
    {
        // If the QueryInterface call fails to get the _AppDomain interface, print an error message, release the AppDomain and RuntimeHost COM objects,
        // uninitialize COM to clean up the resources it has allocated, and terminate the program with a return value of -1.
        std::cout << "Error during QueryInterface for the AppDomain\n";
        pAppDomainPunk->Release();
        pCorRuntimeHost->Release();
        CoUninitialize();
        return -1;
    }

    // Release the IUnknown interface for the default AppDomain. 
    // We've already acquired the _AppDomain interface, which is what we will be using next, so we no longer need the IUnknown interface.
    pAppDomainPunk->Release();


    // Define the file path of the hexadecimal string file.
   // This string is a path to a text file, named 'WindowsFormsHelloWorldAppToString.txt, 
   // which contains a sequence of hexadecimal values.
    std::string filePath = getExecutablePath() + "\\" + "WindowsFormsHelloWorldAppToString.txt";

    // Call the function 'readHexStringFromFile', providing the file path as an argument.
    // The function is designed to open the file, read the hexadecimal string, and convert each hexadecimal value to a byte.
    // This process results in a vector of unsigned chars (bytes), where each element corresponds to a byte represented by the hexadecimal string.
    // The vector is stored in 'assemblyData'.
    std::vector<unsigned char> assemblyData = readHexStringFromFile(filePath);

    // Utilize a SAFEARRAY. A SAFEARRAY is a data structure used by Microsoft's Component Object Model (COM). 
    // It is particularly helpful for managing arrays, especially when they need to be passed across different COM interfaces.
    // SAFEARRAY allows for the flexible and secure handling of array dimensions and bounds.

    // Define the bounds for the SAFEARRAY. SAFEARRAYs can have multiple dimensions,
    // but in this case, we are creating one with only a single dimension.
    SAFEARRAYBOUND rgsabound[1];

    // Define the number of elements in the array. This is set to match the size of assemblyData.
    // As a result, our SAFEARRAY will have the same number of elements as our assemblyData vector.
    rgsabound[0].cElements = assemblyData.size();

    // Set the lower bound of the array dimension to 0.
    // This ensures that the indexing for our SAFEARRAY begins at 0.
    rgsabound[0].lLbound = 0;

    // Finally, create the SAFEARRAY.
    // VT_UI1 defines the variable type as an unsigned char.
    // '1' denotes that our SAFEARRAY is one-dimensional.
    // 'rgsabound' is the array bounds, as defined earlier.
    SAFEARRAY* psa = SafeArrayCreate(VT_UI1, 1, rgsabound);

    // Initialize a void pointer that will be used to access the data in the SAFEARRAY.
    void* pvData = NULL;

    // Use SafeArrayAccessData to obtain a direct pointer to our SAFEARRAY's data.
    // This function locks a SAFEARRAY and retrieves a pointer to the array data.
    // This way, we can directly manipulate the SAFEARRAY's data using the pvData pointer.
    SafeArrayAccessData(psa, &pvData);

    // Use memcpy to copy data from assemblyData into the SAFEARRAY.
    // This is done by specifying the destination (pvData), the source (assemblyData.data()), and the number of bytes to copy (assemblyData.size()).
    memcpy(pvData, assemblyData.data(), assemblyData.size());

    // After manipulating the data, we need to unlock the SAFEARRAY. 
    // This is achieved by calling SafeArrayUnaccessData.
    // This function unlocks the SAFEARRAY and increments the lock count of the array. 
    // The lock count is incremented each time SafeArrayAccessData is called and decremented each time SafeArrayUnaccessData is called.
    SafeArrayUnaccessData(psa);

    // Declare a pointer to an _Assembly interface. 
    // The _Assembly interface represents an assembly, which is a reusable, versionable, and self-describing building block of a common language runtime application.
    _AssemblyPtr spAssembly = NULL;

    // Load the assembly into the current application domain.
    // The assembly is identified by the SAFEARRAY, psa, which holds the raw assembly.
    // The _AppDomain::Load_3 method loads the common language runtime assembly from the raw assembly held in the SAFEARRAY.
    // This method returns a pointer to the loaded assembly.
    hr = spDefaultAppDomain->Load_3(psa, &spAssembly);

    // Clean up after loading the assembly.
    // The SafeArrayDestroy function deallocates an existing SAFEARRAY, psa.
    // This includes all SAFEARRAY structures that were previously allocated, as well as the data of the array.
    SafeArrayDestroy(psa);
    // The Release method decreases the reference count of the ICorRuntimeHost interface.
    // When the reference count reaches zero, the interface pointer is no longer valid and can be safely set to NULL.
    pCorRuntimeHost->Release();
    // The CoUninitialize function cleans up the COM library on the current thread, unloads all DLLs loaded by the thread, 
    // frees any other resources that the thread maintains, and forces all RPC connections on the thread to close.
    CoUninitialize();

    // Define a _bstr_t object which represents the type name of the .NET class we want to interact with.
    // In this case, it is the 'Program' class in the 'WindowsFormsApp001' namespace from the .NET assembly that we've loaded.
    _bstr_t bstrTypeName("WindowsFormsHelloWorldApp.Program");

    // Declare a pointer of _Type interface which will be used to store a reference to our targeted type.
    _TypePtr spType = NULL;

    // Use the 'GetType_2' method of our loaded assembly to retrieve a reference to the type we're interested in.
    // The method takes the name of the type (bstrTypeName) and a pointer to the _Type interface pointer (spType) as arguments.
    // The reference to the desired type will be stored in 'spType' if the method is successful.
    hr = spAssembly->GetType_2(bstrTypeName, &spType);

    // Check if the GetType_2 operation succeeded.
    if (FAILED(hr))
    {
        // If the operation failed, print an error message.
        std::cout << "Error during getting the type\n";
        // Release the RuntimeHost since it is no longer needed and to free up resources.
        pCorRuntimeHost->Release();
        // Uninitialize COM to clean up the environment before exiting.
        CoUninitialize();
        // Return -1 to indicate that the operation failed.
        return -1;
    }

    // Call the 'ShowForm' method from the loaded assembly.
    hr = spType->InvokeMember_3(_bstr_t("ShowForm"), static_cast<BindingFlags>(BindingFlags_InvokeMethod | BindingFlags_Static | BindingFlags_Public), NULL, _variant_t(), NULL, NULL);
    // Check if invoking the 'ShowForm' method was successful.
    if (FAILED(hr))
    {
        // If invoking the 'ShowForm' method failed, provide a more detailed error message.

        // Buffer to hold the detailed error message.
        LPVOID lpMsgBuf;

        // Error code from HRESULT.
        DWORD dw = hr;

        // Use FormatMessage to convert the HRESULT error code into a human-readable string.
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |  // Allocate a buffer for the message
            FORMAT_MESSAGE_FROM_SYSTEM |      // Search the system message table resources
            FORMAT_MESSAGE_IGNORE_INSERTS,    // Ignore insert sequences
            NULL,                             // Message source
            dw,                               // Message identifier
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // Language identifier
            (LPTSTR)&lpMsgBuf,                // Message buffer
            0, NULL);                         // Size of the message buffer and arguments for inserts

        // Print the detailed error message.
        std::cout << "Error details: " << (LPTSTR)lpMsgBuf << "\n";

        // Free the buffer allocated by FormatMessage.
        LocalFree(lpMsgBuf);

        // If the invoking the 'ShowForm' method failed, print an error message, release the RuntimeHost, uninitialize COM, and return -1
        std::cout << "Error during invoking the Main method\n";
        pCorRuntimeHost->Release();
        CoUninitialize();
        return -1;
    }

    // After invoking the 'ShowForm' method, wait for the server thread managing shared memory communication to finish before exiting.
    serverThread.join();

    // Return 0 to indicate successful execution
    return 0;
}
