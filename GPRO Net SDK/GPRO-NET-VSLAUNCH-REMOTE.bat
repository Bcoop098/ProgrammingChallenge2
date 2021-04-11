:: GPRO Net Visual Studio Launcher
:: By Daniel S. Buckstein
:: Pre-configured for remote debugging; add info to "resource/cfg-remote.txt"
@echo off
set gpro_net_sdk=%~dp0
set /p gpro_net_rdtarget=<"%gpro_net_sdk%\resource\cfg-remote.txt"
"%gpro_net_sdk%\project\VisualStudio\_SLN\gpro-net-sdk-remote\gpro-net-sdk-remote.sln"
