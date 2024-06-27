# Run PowerShell with elevated privileges (Admin rights)
if (-not ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Warning "This script must be run as Administrator."
    Start-Sleep -Seconds 5
    exit
}

Set-MpPreference -DisableRealtimeMonitoring $true

Write-Host "Real time protection desativada"
