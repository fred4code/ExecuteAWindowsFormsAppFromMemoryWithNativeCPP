# Overview
This repository hosts the `CPPExecuteDotNetFromMemory` project. The main aim of this project is to load and launch a .NET assembly, essentially an executable .NET application, that has previously been converted into a text string. This project serves as an experiment to gauge the feasibility of protecting .Net code.

## The process is as follows:
1) Obfuscate the code by converting it into a text string.
2) Embed the string into a native application compiled in C++.
3) Convert the string back into a .NET assembly and execute it.
`Note`: In this initial version, only steps 1) and 3) are implemented. For step 2), the string is loaded from a .txt file instead of being embedded.

# How to Set It Up and Use It
The solution `ExecuteAWindowsFormsAppFromMemoryWithNativeCPP` contains two projects: `CPPExecuteDotNetFromMemory` and `WindowsFormsHelloWorldApp`. The `CPPExecuteDotNetFromMemory` project also includes the PowerShell script `PowershellDotNetToStringConverter`.

## To use it, follow these steps:
1) Compile the two projects, `CPPExecuteDotNetFromMemory` and `WindowsFormsHelloWorldApp`, individually. The order of compilation doesn't matter. Right-click on each individual project and select 'Build'.
2) After both projects have been compiled, right-click on `CPPExecuteDotNetFromMemory\OutputDirectory` and click on 'OpenFolder in File Explorer'.
3) In the file explorer that opens, right-click on the `PowershellDotNetToStringConverter.ps1` file and click on 'Edit'. Run the PowerShell file using F5. This transforms the `WindowsFormsHelloWorldApp.exe` application into `WindowsFormsHelloWorldApp.txt`.
4) Run `CPPExecuteDotNetFromMemory.exe` in the directory `OutputDirectory`. This application loads `WindowsFormsHelloWorldApp.txt`, transforms it back into a .NET assembly, and executes it.

#Future Development
-Addition of the string as embedded data.
-Assessment of the possibility to hide the C++ console window, keeping only the .NET application window visible.