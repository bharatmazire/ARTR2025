del VulkanBase.exe

cls

cl.exe /c /EHsc VulkanBase.c

rc.exe VulkanBase.rc

link.exe VulkanBase.obj VulkanBase.res user32.lib gdi32.lib kernel32.lib /SUBSYSTEM:WINDOWS

del VulkanBase.obj

VulkanBase.exe