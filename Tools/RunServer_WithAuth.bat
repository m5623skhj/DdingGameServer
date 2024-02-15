@echo off
pushd "%~dp0\..\bin"

start "DBServer" "DBServer.exe"
start "GameServer" "GameServer.exe"
start "AuthServer" "AuthServer.exe"