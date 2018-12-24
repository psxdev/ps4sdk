#include <stddef.h>
#include <stdint.h>

#include <ps4/stub_resolve.h>
//by default 1.76 you can change to your own
uint32_t sdkVersion=0x05050031;

__asm__("\
	.pushsection .text \n \
	.global ps4StubResolveSystemCall \n \
	.type ps4StubResolveSystemCall, @function \n \
 	ps4StubResolveSystemCall:\n \
		movq $0, %rax \n \
		jmp ps4AssemblerSystemCall \n \
	.size ps4StubResolveSystemCall, .-ps4StubResolveSystemCall \n \
	.popsection \n \
");

int ps4StubResolveSystemCall();

int ps4StubGetSdkVersion(int k)
{
	//in early stage after ps4StubResolveLoadStartModule is resolved we look for sdk version and save it in sdkVersion
	static int (*ps4StubSysctlByName)(const char *, void *, size_t *, const void *, size_t) = NULL;
	size_t len=4;
	int ret;
	if(!ps4StubSysctlByName)
	{
		if(ps4StubResolveSystemCall(591, k, "sysctlbyname", (void **)&ps4StubSysctlByName) != 0)
		{
			return -1;
		}
	}
	ret=ps4StubSysctlByName("kern.sdk_version", &sdkVersion, &len, NULL, 0);
	return ret;
}
int ps4StubResolve(void *stub, char *moduleName, char *functionName, int *moduleHandle, void **userAddress)
{
	static int (*ps4StubResolveLoadStartModule)(const char *name, size_t argc, const void *argv, uint32_t flags, void *a, void *b) = NULL;
	if(stub == NULL || moduleName == NULL || functionName == NULL || moduleHandle == NULL || userAddress == NULL)
		return -1;

	if(*moduleHandle <= 0) // for 0 (elf) we would do more then needed, but its a static null initialized value
	{
		if(!ps4StubResolveLoadStartModule)
		{
			
			int k = 0;
			ps4StubResolveSystemCall(594, "libkernel_sys.sprx", 0, &k, 0);
			if(k <= 0)
			{
				k=0;
				ps4StubResolveSystemCall(594, "libkernel.sprx", 0, &k, 0);
				if(k<=0)
				{
					k=0;
					ps4StubResolveSystemCall(594, "libkernel_web.sprx", 0, &k, 0);
					if(k<=0)
					{
						return -1;
					}
				}			
			}
			
			if(ps4StubResolveSystemCall(591, k, "sceKernelLoadStartModule", (void **)&ps4StubResolveLoadStartModule) != 0)
			{
				return -1;
			}
			else
			{
				
				ps4StubGetSdkVersion(k);
			}
				
		}
		//we have not yet resolve symbols and we need libkernel first so to get our symbols resolved we load a valid libkernel module to get a valid handle
		if(sdkVersion>=0x04050001 && moduleName[0]=='l' && moduleName[1]=='i' && moduleName[2]=='b' && moduleName[3]=='k' && moduleName[4]=='e' && moduleName[5]=='r' && moduleName[6]=='n' && moduleName[7]=='e' && moduleName[8]=='l')
		{	
			//by now in 4.05 we force to use libkernel_web.sprx instead libkernel.sprx in webkit context need to see why libkernel.sprx return -1
			*moduleHandle = ps4StubResolveLoadStartModule("libkernel_sys.sprx", 0, NULL, 0, NULL, NULL);
			
		}
		else
		{
			*moduleHandle = ps4StubResolveLoadStartModule(moduleName, 0, NULL, 0, NULL, NULL);
		}
		if(*moduleHandle <= 0)
			return -1;
	}

	if(ps4StubResolveSystemCall(591, *moduleHandle, functionName, userAddress) != 0)
		return -1;

	return 0;
}
