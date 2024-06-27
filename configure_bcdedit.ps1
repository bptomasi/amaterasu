# Run PowerShell with elevated privileges (Admin rights)
if (-not ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Warning "This script must be run as Administrator."
    Start-Sleep -Seconds 5
    exit
}

# Disable driver signature enforcement
Start-Process bcdedit.exe -ArgumentList "/set nointegritychecks on" -Verb RunAs -Wait

# Enable test mode for driver signature enforcement
Start-Process bcdedit.exe -ArgumentList "/set testsigning on" -Verb RunAs -Wait

# Enable debug mode
Start-Process bcdedit.exe -ArgumentList "-debug on" -Verb RunAs -Wait

Write-Host "Commands executed successfully."
