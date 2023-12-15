@echo off
pushd "%~dp0bin"

start "DBServer" "DBServer.exe"
start "RIO_Core" "RIO_Core.exe"