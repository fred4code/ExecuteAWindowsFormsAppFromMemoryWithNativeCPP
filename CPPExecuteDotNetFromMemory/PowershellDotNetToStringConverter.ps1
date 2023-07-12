<#
This .cs file is part of the Visual Studio Project WindowsFormsHelloWorldApp
WindowsFormsHelloWorldApp Visual Studio Project project is part of the Visual Studio Solution ExecuteAWindowsFormsAppFromMemoryWithNativeCPP
This .cs file, the project mentioned and the Visual studio solution mentioned are licensed under the MIT License. See http://opensource.org/licenses/MIT for more information.

Copyright(c) 2023 fred4code
#>

$exePath = Join-Path -Path $PSScriptRoot -ChildPath "WindowsFormsHelloWorldApp.exe"
$fileContent = Get-Content -Path $exePath -Encoding Byte
$hexString = [BitConverter]::ToString($fileContent) -replace '-',''
$outPath = Join-Path -Path $PSScriptRoot -ChildPath "WindowsFormsHelloWorldAppToString.txt"
Set-Content -Path $outPath -Value $hexString